#include "multicore_master.h"
#include "baseinc.h"
#include "crg.h"
#include "cpm.h"
#include "debug.h"
#include "interrupt.h"
#include "ipcm.h"
#include "multicore_protocol.h"

#define CPU1_ENTRY_ADDRESS          0x03080004U
#define CPU2_ENTRY_ADDRESS          0x030C0004U
#define IPCM_STARTUP_WAIT_POLLS     500U
#define IPCM_PING_INTERVAL_POLLS    1000U
#define IPCM_PING_TIMEOUT_POLLS     2000U

static IPCM_Handle g_masterIpcm;
static volatile unsigned int g_core1AckCount;
static volatile unsigned int g_core2AckCount;
static volatile unsigned int g_core1AckSequence;
static volatile unsigned int g_core2AckSequence;
static volatile unsigned int g_core1BusinessAckCount;
static volatile unsigned int g_core2BusinessAckCount;
static volatile unsigned int g_core1BusinessAckId;
static volatile unsigned int g_core2BusinessAckId;
static volatile unsigned int g_core1BusinessAckCommand;
static volatile unsigned int g_core2BusinessAckCommand;
static volatile unsigned int g_core1BusinessAckResult;
static volatile unsigned int g_core2BusinessAckResult;
static volatile unsigned int g_core1BusinessAckSequence;
static volatile unsigned int g_core2BusinessAckSequence;
static unsigned int g_pingSequence;
static unsigned int g_commandSequence;
static unsigned int g_servicePolls;
static unsigned int g_pingWaitPolls;
static unsigned char g_pingInFlight;
static unsigned char g_masterReady;
static unsigned int g_core1BusinessAckPrinted;
static unsigned int g_core2BusinessAckPrinted;

static void MultiCoreMaster_SaveAck(IPCM_Handle *ipcm, IPCM_MbxChId channel,
                                    volatile unsigned int *pingAckSequence,
                                    volatile unsigned int *pingAckCount,
                                    volatile unsigned int *businessAckCount,
                                    volatile unsigned int *businessAckId,
                                    volatile unsigned int *businessAckCommand,
                                    volatile unsigned int *businessAckResult,
                                    volatile unsigned int *businessAckSequence)
{
    unsigned int id = HAL_IPCM_GetData(ipcm, channel, MBX_DATA_INDEX_0);
    unsigned int command = HAL_IPCM_GetData(ipcm, channel, MBX_DATA_INDEX_1);
    unsigned int result = HAL_IPCM_GetData(ipcm, channel, MBX_DATA_INDEX_2);
    unsigned int sequence = HAL_IPCM_GetData(ipcm, channel, MBX_DATA_INDEX_3);

    if (id == MC_MOTOR_ID_PING && command == MC_CMD_PING) {
        if (result == MC_RESULT_OK) {
            *pingAckSequence = sequence;
            (*pingAckCount)++;
        }
        return;
    }

    *businessAckId = id;
    *businessAckCommand = command;
    *businessAckResult = result;
    *businessAckSequence = sequence;
    (*businessAckCount)++;
}

static void MultiCoreMaster_Core1Ack(void *handle)
{
    IPCM_Handle *ipcm = (IPCM_Handle *)handle;

    MultiCoreMaster_SaveAck(ipcm, MBX_CH_0,
                            &g_core1AckSequence, &g_core1AckCount,
                            &g_core1BusinessAckCount, &g_core1BusinessAckId,
                            &g_core1BusinessAckCommand, &g_core1BusinessAckResult,
                            &g_core1BusinessAckSequence);
}

static void MultiCoreMaster_Core2Ack(void *handle)
{
    IPCM_Handle *ipcm = (IPCM_Handle *)handle;

    MultiCoreMaster_SaveAck(ipcm, MBX_CH_1,
                            &g_core2AckSequence, &g_core2AckCount,
                            &g_core2BusinessAckCount, &g_core2BusinessAckId,
                            &g_core2BusinessAckCommand, &g_core2BusinessAckResult,
                            &g_core2BusinessAckSequence);
}

static BASE_StatusType MultiCoreMaster_InitChannel(IPCM_MbxChId channel, ChipCoreId destination,
                                                    IPCM_CallbackFunc callback)
{
    IPCM_MbxChAttr cfg = {
        .srcCoreId = CHIP_CORE_0,
        .destCoreId = destination,
        .mbxChMode = MBX_AUTO_REPLY_MODE,
    };

    if (HAL_IPCM_Init(&g_masterIpcm, channel, cfg) != BASE_STATUS_OK)
        return BASE_STATUS_ERROR;

    HAL_IPCM_RegisterCallback(&g_masterIpcm, channel, READ_FINISH_TYPE, callback);
    return BASE_STATUS_OK;
}

BASE_StatusType MultiCoreMaster_InitAndStart(void)
{
    BASE_StatusType ret;

    HAL_CRG_IpEnableSet(IPCM_BASE, IP_CLK_ENABLE);
    g_masterIpcm.baseAddress = IPCM;

    ret = MultiCoreMaster_InitChannel(MBX_CH_0, CHIP_CORE_1, MultiCoreMaster_Core1Ack);
    if (ret != BASE_STATUS_OK)
        return ret;
    ret = MultiCoreMaster_InitChannel(MBX_CH_1, CHIP_CORE_2, MultiCoreMaster_Core2Ack);
    if (ret != BASE_STATUS_OK)
        return ret;

    IRQ_Register(IRQ_IPC, HAL_IPCM_IrqHandler, &g_masterIpcm);
    IRQ_SetPriority(IRQ_IPC, 1U);
    IRQ_EnableN(IRQ_IPC);

    ret = CPM_SetStartAddr(CHIP_CORE_1, CPU1_ENTRY_ADDRESS);
    if (ret != BASE_STATUS_OK)
        return ret;
    ret = CPM_Start(CHIP_CORE_1);
    if (ret != BASE_STATUS_OK)
        return ret;

    ret = CPM_SetStartAddr(CHIP_CORE_2, CPU2_ENTRY_ADDRESS);
    if (ret != BASE_STATUS_OK)
        return ret;
    ret = CPM_Start(CHIP_CORE_2);
    if (ret != BASE_STATUS_OK)
        return ret;

    g_core1AckCount = 0U;
    g_core2AckCount = 0U;
    g_core1AckSequence = 0U;
    g_core2AckSequence = 0U;
    g_core1BusinessAckCount = 0U;
    g_core2BusinessAckCount = 0U;
    g_core1BusinessAckPrinted = 0U;
    g_core2BusinessAckPrinted = 0U;
    g_pingSequence = 0U;
    g_commandSequence = 0U;
    g_servicePolls = 0U;
    g_pingWaitPolls = 0U;
    g_pingInFlight = 0U;
    g_masterReady = 1U;
    return BASE_STATUS_OK;
}

static BASE_StatusType MultiCoreMaster_SendPing(void)
{
    BASE_StatusType ret1;
    BASE_StatusType ret2;

    /* An initialized idle channel is SOURCE, not FREE, on this IPCM block. */
    if (HAL_IPCM_GetStatus(&g_masterIpcm, MBX_CH_0) != MBX_STATUS_SOURCE ||
        HAL_IPCM_GetStatus(&g_masterIpcm, MBX_CH_1) != MBX_STATUS_SOURCE)
        return BASE_STATUS_ERROR;

    g_pingSequence++;
    (void)HAL_IPCM_SetData(&g_masterIpcm, MBX_CH_0, MBX_DATA_INDEX_0, MC_MOTOR_ID_PING);
    (void)HAL_IPCM_SetData(&g_masterIpcm, MBX_CH_0, MBX_DATA_INDEX_1, MC_CMD_PING);
    (void)HAL_IPCM_SetData(&g_masterIpcm, MBX_CH_0, MBX_DATA_INDEX_2, MC_PING_MAGIC);
    (void)HAL_IPCM_SetData(&g_masterIpcm, MBX_CH_0, MBX_DATA_INDEX_3, g_pingSequence);
    (void)HAL_IPCM_SetData(&g_masterIpcm, MBX_CH_1, MBX_DATA_INDEX_0, MC_MOTOR_ID_PING);
    (void)HAL_IPCM_SetData(&g_masterIpcm, MBX_CH_1, MBX_DATA_INDEX_1, MC_CMD_PING);
    (void)HAL_IPCM_SetData(&g_masterIpcm, MBX_CH_1, MBX_DATA_INDEX_2, MC_PING_MAGIC);
    (void)HAL_IPCM_SetData(&g_masterIpcm, MBX_CH_1, MBX_DATA_INDEX_3, g_pingSequence);

    ret1 = HAL_IPCM_SendData(&g_masterIpcm, MBX_CH_0);
    ret2 = HAL_IPCM_SendData(&g_masterIpcm, MBX_CH_1);
    if (ret1 != BASE_STATUS_OK || ret2 != BASE_STATUS_OK)
        return BASE_STATUS_ERROR;

    g_pingInFlight = 1U;
    g_pingWaitPolls = 0U;
    // DBG_PRINTF("multicore ping seq=%u\r\n", g_pingSequence);
    return BASE_STATUS_OK;
}

BASE_StatusType MultiCoreMaster_SendMotorCommand(unsigned int motorId,
                                                 unsigned int command,
                                                 unsigned int value)
{
    IPCM_MbxChId channel;
    unsigned int sequence;
    BASE_StatusType ret;

    if (g_masterReady == 0U)
        return BASE_STATUS_ERROR;
    if (motorId == MC_MOTOR_ID_2)
        channel = MBX_CH_0;
    else if (motorId == MC_MOTOR_ID_3)
        channel = MBX_CH_1;
    else
        return BASE_STATUS_ERROR;
    if (command > MC_CMD_STOP || (command == MC_CMD_MOVE && value > 180U))
        return BASE_STATUS_ERROR;
    /* SOURCE means configured and idle; DEST/REPLY means a transfer is active. */
    if (HAL_IPCM_GetStatus(&g_masterIpcm, channel) != MBX_STATUS_SOURCE)
        return BASE_STATUS_ERROR;

    sequence = ++g_commandSequence;
    (void)HAL_IPCM_SetData(&g_masterIpcm, channel, MBX_DATA_INDEX_0, motorId);
    (void)HAL_IPCM_SetData(&g_masterIpcm, channel, MBX_DATA_INDEX_1, command);
    (void)HAL_IPCM_SetData(&g_masterIpcm, channel, MBX_DATA_INDEX_2, value);
    (void)HAL_IPCM_SetData(&g_masterIpcm, channel, MBX_DATA_INDEX_3, sequence);
    ret = HAL_IPCM_SendData(&g_masterIpcm, channel);
    if (ret == BASE_STATUS_OK) {
        DBG_PRINTF("mc route: seq=%u id=%u cmd=%u data=%u -> cpu%u\r\n",
                   sequence, motorId, command, value,
                   (motorId == MC_MOTOR_ID_2) ? 1U : 2U);
    }
    return ret;
}

static void MultiCoreMaster_PrintBusinessAcks(void)
{
    if (g_core1BusinessAckPrinted != g_core1BusinessAckCount) {
        g_core1BusinessAckPrinted = g_core1BusinessAckCount;
        DBG_PRINTF("mc ack: cpu1 seq=%u id=%u cmd=%u result=%u\r\n",
                   g_core1BusinessAckSequence, g_core1BusinessAckId,
                   g_core1BusinessAckCommand, g_core1BusinessAckResult);
    }
    if (g_core2BusinessAckPrinted != g_core2BusinessAckCount) {
        g_core2BusinessAckPrinted = g_core2BusinessAckCount;
        DBG_PRINTF("mc ack: cpu2 seq=%u id=%u cmd=%u result=%u\r\n",
                   g_core2BusinessAckSequence, g_core2BusinessAckId,
                   g_core2BusinessAckCommand, g_core2BusinessAckResult);
    }
}

void MultiCoreMaster_Service(void)
{
    if (g_masterReady == 0U)
        return;

    MultiCoreMaster_PrintBusinessAcks();

    if (g_pingInFlight != 0U) {
        if (g_core1AckSequence == g_pingSequence && g_core2AckSequence == g_pingSequence) {
            // DBG_PRINTF("multicore ack seq=%u cpu1=%u cpu2=%u\r\n",
            //            g_pingSequence, g_core1AckCount, g_core2AckCount);
            g_pingInFlight = 0U;
            g_servicePolls = 0U;
            return;
        }

        g_pingWaitPolls++;
        if (g_pingWaitPolls >= IPCM_PING_TIMEOUT_POLLS) {
            DBG_PRINTF("multicore timeout seq=%u cpu1Seq=%u cpu2Seq=%u\r\n",
                       g_pingSequence, g_core1AckSequence, g_core2AckSequence);
            g_pingInFlight = 0U;
            g_servicePolls = 0U;
        }
        return;
    }

    g_servicePolls++;
    if ((g_pingSequence == 0U && g_servicePolls >= IPCM_STARTUP_WAIT_POLLS) ||
        (g_pingSequence != 0U && g_servicePolls >= IPCM_PING_INTERVAL_POLLS)) {
        if (MultiCoreMaster_SendPing() != BASE_STATUS_OK) {
            DBG_PRINTF("multicore ping send failed\r\n");
            g_servicePolls = 0U;
        }
    }
}

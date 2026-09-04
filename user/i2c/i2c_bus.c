#include "i2c_bus.h"

#include "i2c.h"
#include "i2c_ex.h"
#include "crg.h"
#include "gpio.h"
#include "interrupt.h"
#include "iocmg.h"
#include "iocmg_ex.h"
#include "iomap.h"

/*
 * The board currently has no external I2C pull-up resistors.  Internal pull-ups
 * are enabled only for short-wire, low-speed bring-up.  Production hardware
 * still needs external pull-ups on SCL and SDA.
 */
#define I2C_BUS_FREQ_HZ          100000U
#define I2C_BUS_ADDR_BASE        0x20U
#define I2C_BUS_SDA_HOLD_TIME    0x0AU
#define I2C_BUS_SPIKE_FILTER     0x02U
#define I2C_BUS_SDA_DELAY        0x00U
#define I2C_BUS_IDLE_STABLE_POLLS 20U
#define I2C_BUS_SDA_PIN          GPIO_PIN_3
#define I2C_BUS_SCL_PIN          GPIO_PIN_4
#define I2C_BUS_MONITOR_PINS     (I2C_BUS_SDA_PIN | I2C_BUS_SCL_PIN)

static I2C_Handle g_i2cBus;
static GPIO_Handle g_i2cBusPins;
static volatile uint8_t g_rxReady;
static volatile uint8_t g_rearmNeeded;
static volatile uint8_t g_txPending;
static volatile uint8_t g_txActive;
static volatile uint8_t g_txDone;
static volatile uint8_t g_initialized;
static volatile uint8_t g_busActive;
static volatile uint8_t g_reinitRequested;
static volatile uint32_t g_rxCount;
static volatile uint32_t g_txCount;
static volatile uint32_t g_errorCount;
static volatile uint32_t g_startCount;
static volatile uint32_t g_addressMatchCount;
static volatile uint32_t g_ackUnmatchCount;
static volatile uint32_t g_sclTimeoutCount;
static volatile uint32_t g_stopCount;
static volatile uint32_t g_shortStopCount;
static volatile uint32_t g_lastRawStatus;
static volatile uint32_t g_lastRxAddress;
static volatile uint32_t g_lastRxDirection;
static volatile uint32_t g_reinitCount;
static uint8_t g_slaveAddress;
static uint8_t g_rxFrame[I2C_BUS_FRAME_SIZE];
static uint8_t g_txFrame[I2C_BUS_REPLY_MAX_SIZE];
static uint8_t g_txLength;
static uint8_t g_readIndex;
static uint8_t g_idleHighPolls;

static BASE_StatusType I2cBus_ArmReceive(void)
{
    BASE_StatusType ret;

    if (g_busActive == 0U || g_rxReady != 0U || g_txPending != 0U || g_txActive != 0U ||
        g_i2cBus.state != I2C_STATE_READY)
        return BASE_STATUS_BUSY;

    ret = HAL_I2C_SlaveReadIT(&g_i2cBus, g_rxFrame, I2C_BUS_FRAME_SIZE);
    if (ret == BASE_STATUS_OK) {
        g_rearmNeeded = 0U;
    } else if (ret == BASE_STATUS_BUSY) {
        g_rearmNeeded = 1U;
    } else {
        g_rearmNeeded = 0U;
        g_errorCount++;
        g_reinitRequested = 1U;
    }
    return ret;
}

static void I2cBus_RxComplete(void *handle)
{
    (void)handle;
    g_rxReady = 1U;
    g_rxCount++;
}

static void I2cBus_TxComplete(void *handle)
{
    (void)handle;
    g_txActive = 0U;
    g_txDone = 1U;
    g_txCount++;
}

static void I2cBus_Error(void *handle)
{
    (void)handle;
    g_errorCount++;
    g_txPending = 0U;
    g_txActive = 0U;
    g_txDone = 0U;
    g_rearmNeeded = 0U;
    /* Do not rebuild the peripheral in interrupt context. */
    g_reinitRequested = 1U;
}

/*
 * Keep diagnostics out of HAL_I2C_IrqHandler(): read the raw cause before the
 * HAL clears it, then let the original driver do all normal processing.
 */
static void I2cBus_IrqHandler(void *handle)
{
    uint32_t raw;
    uint8_t shortReceiveStop;

    raw = I2C0->I2C_INTR_RAW.reg;
    g_lastRawStatus = raw;

    if (I2C0->I2C_INTR_RAW.BIT.start_det_raw != 0U)
        g_startCount++;
    if (I2C0->I2C_INTR_RAW.BIT.slv_addr_match_raw != 0U) {
        g_addressMatchCount++;
        g_lastRxAddress = I2C0->I2C_RX_ADDR.BIT.rx_addr;
        g_lastRxDirection = I2C0->I2C_RX_ADDR.BIT.rx_rw;
    }
    if (I2C0->I2C_INTR_RAW.BIT.slv_rx_ack_unmatch_raw != 0U)
        g_ackUnmatchCount++;
    if (I2C0->I2C_INTR_RAW.BIT.scl_low_timeout_raw != 0U)
        g_sclTimeoutCount++;
    if (I2C0->I2C_INTR_RAW.BIT.stop_det_raw != 0U)
        g_stopCount++;

    /*
     * An address-only probe ends with STOP before the requested six bytes
     * arrive.  The SDK completion path only returns to READY after all six
     * bytes, so recover the local receive state after the HAL has consumed the
     * interrupt.  The service task will arm a fresh six-byte receive.
     */
    shortReceiveStop = (I2C0->I2C_INTR_RAW.BIT.stop_det_raw != 0U &&
                        g_i2cBus.state == I2C_STATE_BUSY_SLAVE_RX &&
                        g_i2cBus.transferCount < g_i2cBus.transferSize) ? 1U : 0U;

    HAL_I2C_IrqHandler(handle);

    if (shortReceiveStop != 0U && g_reinitRequested == 0U) {
        g_shortStopCount++;
        I2C0->I2C_INTR_EN.reg = I2C_INTR_EN_ALL_DISABLE;
        I2C0->I2C_INTR_RAW.reg = I2C_INTR_RAW_ALL_ENABLE;
        g_i2cBus.state = I2C_STATE_READY;
        g_rearmNeeded = 1U;
    }
}

/*
 * Keep both lines high impedance while the 3516 side is still booting.  The
 * GPIO input mode is only used to observe whether the shared bus is idle; it
 * must never drive SCL or SDA.
 */
static void I2cBus_ConfigMonitorPins(void)
{
    HAL_CRG_IpEnableSet(GPIO16_BASE, IP_CLK_ENABLE);

    HAL_IOCMG_SetPinAltFuncMode(GPIO16_4_AS_GPIO16_4);
    HAL_IOCMG_SetPinPullMode(GPIO16_4_AS_GPIO16_4, PULL_UP);
    HAL_IOCMG_SetPinSchmidtMode(GPIO16_4_AS_GPIO16_4, SCHMIDT_ENABLE);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO16_4_AS_GPIO16_4, OPEN_DRAIN_ENABLE);

    HAL_IOCMG_SetPinAltFuncMode(GPIO16_3_AS_GPIO16_3);
    HAL_IOCMG_SetPinPullMode(GPIO16_3_AS_GPIO16_3, PULL_UP);
    HAL_IOCMG_SetPinSchmidtMode(GPIO16_3_AS_GPIO16_3, SCHMIDT_ENABLE);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO16_3_AS_GPIO16_3, OPEN_DRAIN_ENABLE);

    g_i2cBusPins.baseAddress = GPIO16;
    g_i2cBusPins.pins = I2C_BUS_MONITOR_PINS;
    HAL_GPIO_Init(&g_i2cBusPins);
    HAL_GPIO_SetDirection(&g_i2cBusPins, I2C_BUS_MONITOR_PINS, GPIO_INPUT_MODE);
    (void)HAL_GPIO_SetIrqType(&g_i2cBusPins, I2C_BUS_MONITOR_PINS, GPIO_INT_TYPE_NONE);
}

static uint8_t I2cBus_IsIdleHigh(void)
{
    return (HAL_GPIO_GetPinValue(&g_i2cBusPins, I2C_BUS_SCL_PIN) == GPIO_HIGH_LEVEL &&
            HAL_GPIO_GetPinValue(&g_i2cBusPins, I2C_BUS_SDA_PIN) == GPIO_HIGH_LEVEL) ? 1U : 0U;
}

static void I2cBus_ConfigPeripheralPins(void)
{
    /* PIN72 / GPIO16_4: I2C0 SCL. */
    HAL_IOCMG_SetPinAltFuncMode(GPIO16_4_AS_I2C0_SCL);
    HAL_IOCMG_SetPinPullMode(GPIO16_4_AS_I2C0_SCL, PULL_UP);
    HAL_IOCMG_SetPinSchmidtMode(GPIO16_4_AS_I2C0_SCL, SCHMIDT_ENABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO16_4_AS_I2C0_SCL, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO16_4_AS_I2C0_SCL, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO16_4_AS_I2C0_SCL, OPEN_DRAIN_ENABLE);

    /* PIN71 / GPIO16_3: I2C0 SDA. */
    HAL_IOCMG_SetPinAltFuncMode(GPIO16_3_AS_I2C0_SDA);
    HAL_IOCMG_SetPinPullMode(GPIO16_3_AS_I2C0_SDA, PULL_UP);
    HAL_IOCMG_SetPinSchmidtMode(GPIO16_3_AS_I2C0_SDA, SCHMIDT_ENABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO16_3_AS_I2C0_SDA, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO16_3_AS_I2C0_SDA, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO16_3_AS_I2C0_SDA, OPEN_DRAIN_ENABLE);
}

/* Stop the damaged transaction and return to passive bus monitoring. */
static void I2cBus_StopHardwareForRecovery(void)
{
    (void)IRQ_DisableN(IRQ_I2C0);

    if (g_busActive != 0U) {
        I2C0->I2C_INTR_EN.reg = I2C_INTR_EN_ALL_DISABLE;
        I2C0->I2C_INTR_RAW.reg = I2C_INTR_RAW_ALL_ENABLE;
        (void)HAL_I2C_Deinit(&g_i2cBus);
    }

    (void)HAL_CRG_IpClkResetSet(I2C0_BASE, BASE_CFG_SET);
    (void)HAL_CRG_IpClkResetSet(I2C0_BASE, BASE_CFG_UNSET);
    HAL_CRG_IpEnableSet(I2C0_BASE, IP_CLK_ENABLE);

    g_busActive = 0U;
    g_reinitRequested = 0U;
    g_rxReady = 0U;
    g_rearmNeeded = 0U;
    g_txPending = 0U;
    g_txActive = 0U;
    g_txDone = 0U;
    g_txLength = 0U;
    g_readIndex = 0U;
    g_idleHighPolls = 0U;
    g_i2cBus.state = I2C_STATE_RESET;
    g_i2cBus.errorCode = BASE_STATUS_OK;

    I2cBus_ConfigMonitorPins();
}

/* Start a clean slave instance only after the bus has returned to idle. */
static BASE_StatusType I2cBus_StartHardware(void)
{
    BASE_StatusType ret;

    (void)IRQ_DisableN(IRQ_I2C0);
    (void)HAL_CRG_IpClkResetSet(I2C0_BASE, BASE_CFG_SET);
    (void)HAL_CRG_IpClkResetSet(I2C0_BASE, BASE_CFG_UNSET);
    HAL_CRG_IpEnableSet(I2C0_BASE, IP_CLK_ENABLE);
    I2cBus_ConfigPeripheralPins();

    g_i2cBus.errorCode = BASE_STATUS_OK;
    ret = HAL_I2C_Init(&g_i2cBus);
    if (ret != BASE_STATUS_OK)
        return ret;
    g_busActive = 1U;

    ret = HAL_I2C_RegisterCallback(&g_i2cBus, I2C_SLAVE_RX_COMPLETE_CB_ID,
                                   I2cBus_RxComplete);
    if (ret != BASE_STATUS_OK)
        return ret;
    ret = HAL_I2C_RegisterCallback(&g_i2cBus, I2C_SLAVE_TX_COMPLETE_CB_ID,
                                   I2cBus_TxComplete);
    if (ret != BASE_STATUS_OK)
        return ret;
    ret = HAL_I2C_RegisterCallback(&g_i2cBus, I2C_ERROR_CB_ID, I2cBus_Error);
    if (ret != BASE_STATUS_OK)
        return ret;

    IRQ_Register(IRQ_I2C0, I2cBus_IrqHandler, &g_i2cBus);
    IRQ_SetPriority(IRQ_I2C0, 2U);
    IRQ_EnableN(IRQ_I2C0);

    ret = I2cBus_ArmReceive();
    return ret;
}

BASE_StatusType I2cBus_Init(uint8_t boardId)
{
    g_rxReady = 0U;
    g_rearmNeeded = 0U;
    g_txPending = 0U;
    g_txActive = 0U;
    g_txDone = 0U;
    g_initialized = 0U;
    g_busActive = 0U;
    g_reinitRequested = 0U;
    g_rxCount = 0U;
    g_txCount = 0U;
    g_errorCount = 0U;
    g_startCount = 0U;
    g_addressMatchCount = 0U;
    g_ackUnmatchCount = 0U;
    g_sclTimeoutCount = 0U;
    g_stopCount = 0U;
    g_shortStopCount = 0U;
    g_lastRawStatus = 0U;
    g_lastRxAddress = 0U;
    g_lastRxDirection = 0U;
    g_reinitCount = 0U;
    g_txLength = 0U;
    g_readIndex = 0U;
    g_idleHighPolls = 0U;
    g_slaveAddress = (uint8_t)(I2C_BUS_ADDR_BASE + (boardId & 0x03U));

    /* Observe the bus first; do not enable the slave while SCL/SDA are low. */
    I2cBus_ConfigMonitorPins();
    HAL_CRG_IpEnableSet(I2C0_BASE, IP_CLK_ENABLE);

    g_i2cBus.baseAddress = I2C0;
    g_i2cBus.functionMode = I2C_MODE_SELECT_SLAVE_ONLY;
    g_i2cBus.addrMode = I2C_7_BITS;
    g_i2cBus.slaveOwnAddress = g_slaveAddress;
    g_i2cBus.sdaHoldTime = I2C_BUS_SDA_HOLD_TIME;
    g_i2cBus.freq = I2C_BUS_FREQ_HZ;
    g_i2cBus.ignoreAckFlag = I2C_IGNORE_NAK_DISABLE;
    g_i2cBus.generalCallMode = BASE_CFG_DISABLE;
    g_i2cBus.rxWaterMark = 1U;
    g_i2cBus.txWaterMark = 1U;
    g_i2cBus.handleEx.spikeFilterTime = I2C_BUS_SPIKE_FILTER;
    g_i2cBus.handleEx.sdaDelayTime = I2C_BUS_SDA_DELAY;
    g_i2cBus.handleEx.slaveOwnXmbAddressEnable = BASE_CFG_DISABLE;
    g_i2cBus.handleEx.slaveOwnXmbAddress = 0U;
    g_i2cBus.state = I2C_STATE_RESET;
    g_i2cBus.errorCode = BASE_STATUS_OK;
    g_initialized = 1U;
    return BASE_STATUS_OK;
}

uint8_t I2cBus_GetFrame(uint8_t frame[I2C_BUS_FRAME_SIZE])
{
    unsigned int i;

    if (frame == 0 || g_rxReady == 0U)
        return 0U;

    for (i = 0U; i < I2C_BUS_FRAME_SIZE; i++)
        frame[i] = g_rxFrame[i];

    g_rxReady = 0U;
    g_readIndex = 0U;
    g_rearmNeeded = 1U;
    return 1U;
}

uint8_t I2cBus_HasData(void)
{
    return (g_rxReady != 0U && g_readIndex < I2C_BUS_FRAME_SIZE) ? 1U : 0U;
}

uint8_t I2cBus_ReadByte(void)
{
    uint8_t data;

    if (I2cBus_HasData() == 0U)
        return 0U;

    data = g_rxFrame[g_readIndex++];
    if (g_readIndex >= I2C_BUS_FRAME_SIZE) {
        g_readIndex = 0U;
        g_rxReady = 0U;
        g_rearmNeeded = 1U;
    }
    return data;
}

BASE_StatusType I2cBus_PrepareReply(const uint8_t *data, uint8_t length)
{
    unsigned int i;

    if (g_initialized == 0U || g_busActive == 0U || data == 0 || length == 0U ||
        length > I2C_BUS_REPLY_MAX_SIZE)
        return BASE_STATUS_ERROR;
    if (g_txPending != 0U || g_txActive != 0U)
        return BASE_STATUS_BUSY;

    for (i = 0U; i < length; i++)
        g_txFrame[i] = data[i];

    g_txLength = length;
    g_txPending = 1U;
    g_rearmNeeded = 0U;
    return BASE_STATUS_OK;
}

void I2cBus_Service(void)
{
    BASE_StatusType ret;

    if (g_initialized == 0U)
        return;

    /*
     * Error callbacks only set a flag.  Resetting clocks, pin mux and the HAL
     * is deliberately performed here in task context.
     */
    if (g_reinitRequested != 0U) {
        g_reinitRequested = 0U;
        g_reinitCount++;
        I2cBus_StopHardwareForRecovery();
        return;
    }

    /*
     * Startup and recovery both wait for a genuinely idle bus.  Requiring
     * several consecutive service polls avoids starting on a short glitch.
     */
    if (g_busActive == 0U) {
        if (I2cBus_IsIdleHigh() == 0U) {
            g_idleHighPolls = 0U;
            return;
        }

        if (g_idleHighPolls < I2C_BUS_IDLE_STABLE_POLLS)
            g_idleHighPolls++;
        if (g_idleHighPolls < I2C_BUS_IDLE_STABLE_POLLS)
            return;

        g_idleHighPolls = 0U;
        ret = I2cBus_StartHardware();
        if (ret != BASE_STATUS_OK) {
            if (g_reinitRequested == 0U)
                g_errorCount++;
            g_reinitCount++;
            I2cBus_StopHardwareForRecovery();
        }
        return;
    }

    /* The driver changes state to READY after returning from the TX callback. */
    if (g_txDone != 0U && g_i2cBus.state == I2C_STATE_READY) {
        g_txDone = 0U;
        g_rearmNeeded = 1U;
    }

    if (g_txPending != 0U && g_i2cBus.state == I2C_STATE_READY) {
        ret = HAL_I2C_SlaveWriteIT(&g_i2cBus, g_txFrame, g_txLength);
        if (ret == BASE_STATUS_OK) {
            g_txPending = 0U;
            g_txActive = 1U;
        } else if (ret != BASE_STATUS_BUSY) {
            g_txPending = 0U;
            g_errorCount++;
            g_rearmNeeded = 0U;
            g_reinitRequested = 1U;
        }
        return;
    }

    if (g_rearmNeeded != 0U && g_rxReady == 0U)
        (void)I2cBus_ArmReceive();
}

uint8_t I2cBus_GetSlaveAddress(void)
{
    return g_slaveAddress;
}

uint32_t I2cBus_GetRxCount(void)
{
    return g_rxCount;
}

uint32_t I2cBus_GetTxCount(void)
{
    return g_txCount;
}

uint32_t I2cBus_GetErrorCount(void)
{
    return g_errorCount;
}

void I2cBus_GetDiag(I2cBusDiag *diag)
{
    if (diag == 0)
        return;

    diag->busActive = g_busActive;
    diag->sclLevel = (uint32_t)HAL_GPIO_GetPinValue(&g_i2cBusPins, I2C_BUS_SCL_PIN);
    diag->sdaLevel = (uint32_t)HAL_GPIO_GetPinValue(&g_i2cBusPins, I2C_BUS_SDA_PIN);
    diag->reinitCount = g_reinitCount;
    diag->startCount = g_startCount;
    diag->addressMatchCount = g_addressMatchCount;
    diag->ackUnmatchCount = g_ackUnmatchCount;
    diag->sclTimeoutCount = g_sclTimeoutCount;
    diag->stopCount = g_stopCount;
    diag->shortStopCount = g_shortStopCount;
    diag->rxCount = g_rxCount;
    diag->txCount = g_txCount;
    diag->errorCount = g_errorCount;
    diag->lastRawStatus = g_lastRawStatus;
    diag->lastRxAddress = g_lastRxAddress;
    diag->lastRxDirection = g_lastRxDirection;
    diag->driverState = (uint32_t)g_i2cBus.state;
    if (g_busActive != 0U) {
        diag->ownAddressRegister = I2C0->I2C_OWN_ADDR.reg;
        diag->interruptEnableRegister = I2C0->I2C_INTR_EN.reg;
        diag->sclTimeoutRegister = I2C0->I2C_SCL_TIMEOUT.reg;
        diag->fsmStatusRegister = I2C0->I2C_FSM_STAT.reg;
    } else {
        diag->ownAddressRegister = 0U;
        diag->interruptEnableRegister = 0U;
        diag->sclTimeoutRegister = 0U;
        diag->fsmStatusRegister = 0U;
    }
}

/**
  * @copyright Copyright (c) 2025, HiSilicon (Shanghai) Technologies Co., Ltd. All rights reserved.
  * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
  * following conditions are met:
  * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
  * disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
  * following disclaimer in the documentation and/or other materials provided with the distribution.
  * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
  * products derived from this software without specific prior written permission.
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
  * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * @file      mailbox.c
  * @author    MCU Driver Team
  * @brief     Mailbox module driver
  * @details   This file provides firmware functions to manage the following functionalities of the Mailbox.
  *             + Initialization functions.
  *             + Mailbox Set And Get Functions.
  *             + Interrupt Handler Functions.
  */

/* Includes ------------------------------------------------------------------*/
#include "mailbox.h"
#include "systickinit.h"
#include "debug.h"

#define SYSTICK_MS_DIV            1000

/**
  * @brief Config Mailbox attribute, include that channelId, share memory and channal attribute.
  * @param handle Value of @ref MBX_Handle
  * @param cfgAttr Value of @ref MBX_ConfigAttr
  * @param chId Value of @ref IPCM_MbxChId
  * @param shmAttr Value of @ref MBX_ShmAttr
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MBX_AttrConfig(MBX_Handle* handle, MBX_ConfigAttr cfgAttr,
                                      IPCM_MbxChId chId, MBX_ShmAttr shmAttr)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(handle->ipcmHandle != NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(chId), BASE_STATUS_ERROR);
    if (HAL_IPCM_Init(handle->ipcmHandle, chId, cfgAttr.chCfg) != BASE_STATUS_OK) {   /* init ipcm channel */
        return BASE_STATUS_ERROR;
    }
    if (shmAttr.enableConfig == BASE_CFG_ENABLE) {   /* use share memory to buffer msg data */
        for (unsigned int i = 0; i < shmAttr.lenth; ++i) {
            *(unsigned char *)(shmAttr.startAddr + i) = 0x0;   /* init share memory space */
        }
    } else {   /* use data register to buffer msg data */
        HAL_IPCM_SetData(handle->ipcmHandle, chId, MBX_DATA_INDEX_0, 0);   /* init ipcm data0 register */
        HAL_IPCM_SetData(handle->ipcmHandle, chId, MBX_DATA_INDEX_1, 0);   /* init ipcm data1 register */
        HAL_IPCM_SetData(handle->ipcmHandle, chId, MBX_DATA_INDEX_2, 0);   /* init ipcm data2 register */
        HAL_IPCM_SetData(handle->ipcmHandle, chId, MBX_DATA_INDEX_3, 0);   /* init ipcm data3 register */
    }
    HAL_IPCM_NotifyCore(handle->ipcmHandle, chId, BASE_CFG_DISABLE);
    if (cfgAttr.writeFinishCallback != NULL) {
        HAL_IPCM_RegisterCallback(handle->ipcmHandle, chId, WRITE_FINISH_TYPE, cfgAttr.writeFinishCallback);
    }
    if (cfgAttr.readFinishCallback != NULL) {
        HAL_IPCM_RegisterCallback(handle->ipcmHandle, chId, READ_FINISH_TYPE, cfgAttr.readFinishCallback);
    }
    return BASE_STATUS_OK;
}


/**
  * @brief Init Mailbox, include that source to dest config and dest to source config.
  * @param handle Value of @ref MBX_Handle
  * @param srcToDestCfg source to dest config, Value of @ref MBX_ConfigAttr
  * @param destToSrcCfg dest to source config, Value of @ref MBX_ConfigAttr
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_Init(MBX_Handle* handle, MBX_ConfigAttr srcToDestCfg, MBX_ConfigAttr destToSrcCfg)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(handle->ipcmHandle != NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(handle->srcToDestChId), BASE_STATUS_ERROR);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(handle->destToSrcChId), BASE_STATUS_ERROR);
    if (MBX_AttrConfig(handle, srcToDestCfg, handle->srcToDestChId, handle->srcToDestShmAttr) != BASE_STATUS_OK) {
        return BASE_STATUS_ERROR;   /* init mailbox source to dest direction config attribute */
    }
    if (MBX_AttrConfig(handle, destToSrcCfg, handle->destToSrcChId, handle->destToSrcShmAttr) != BASE_STATUS_OK) {
        return BASE_STATUS_ERROR;   /* init mailbox dest to source direction config attribute */
    }
    return BASE_STATUS_OK;
}

/**
  * @brief DeInit mailbox.
  * @param handle Value of @ref SRM_Handle.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_DeInit(MBX_Handle* handle)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(handle->srcToDestChId), BASE_STATUS_ERROR);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(handle->destToSrcChId), BASE_STATUS_ERROR);
    if (HAL_IPCM_DeInit(handle->ipcmHandle, handle->srcToDestChId) != BASE_STATUS_OK) {
        return BASE_STATUS_ERROR;  /* deinit mailbox source to dest direction attribute */
    }
    if (HAL_IPCM_DeInit(handle->ipcmHandle, handle->destToSrcChId) != BASE_STATUS_OK) {
        return BASE_STATUS_ERROR;  /* deinit mailbox dest to source direction attribute */
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Get current Mailbox info, include that channelId, share memory and channal attribute.
  * @param handle Value of @ref MBX_Handle
  * @param retChId get current channal id from handle, Value of @ref IPCM_MbxChId
  * @param retShmAttr get current share memory from handle, Value of @ref MBX_ShmAttr
  * @param retMsg get current message from handle, Value of @ref MBX_Msg
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MBX_GetCurrentInfo(MBX_Handle* handle, IPCM_MbxChId retChId,
                                          MBX_ShmAttr* retShmAttr, MBX_Msg* retMsg)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(retChId), BASE_STATUS_ERROR);
    MBX_ASSERT_PARAM(retShmAttr != NULL);
    MBX_ASSERT_PARAM(retMsg != NULL);
    if (retChId == handle->srcToDestChId) { /* get source core infomation from handle */
        retShmAttr->enableConfig = handle->srcToDestShmAttr.enableConfig; /* get share memory */
        retShmAttr->lenth = handle->srcToDestShmAttr.lenth;
        retShmAttr->startAddr = handle->srcToDestShmAttr.startAddr;
        retMsg->dataAddr = handle->srcToDestMsg.dataAddr; /* get message */
        retMsg->dataIndex = handle->srcToDestMsg.dataIndex;
        retMsg->dataRemainSize = handle->srcToDestMsg.dataRemainSize;
        retMsg->status = handle->srcToDestMsg.status;
    } else if (retChId == handle->destToSrcChId) { /* get dest core infomation from handle */
        retShmAttr->enableConfig = handle->destToSrcShmAttr.enableConfig; /* get share memory */
        retShmAttr->lenth = handle->destToSrcShmAttr.lenth;
        retShmAttr->startAddr = handle->destToSrcShmAttr.startAddr;
        retMsg->dataAddr = handle->destToSrcMsg.dataAddr; /* get message */
        retMsg->dataIndex = handle->destToSrcMsg.dataIndex;
        retMsg->dataRemainSize = handle->destToSrcMsg.dataRemainSize;
        retMsg->status = handle->destToSrcMsg.status;
    } else {
        return BASE_STATUS_ERROR;   /* current coreId is not belong to the maibox handle */
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Updata current Mailbox info, include that channelId, share memory and channal attribute.
  * @param handle Value of @ref MBX_Handle
  * @param chId updata current channal id to handle, Value of @ref IPCM_MbxChId
  * @param shmAttr updata current share memory to handle, Value of @ref MBX_ShmAttr
  * @param msg updata current message to handle, Value of @ref MBX_Msg
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MBX_UpdataCurrentInfo(MBX_Handle* handle, IPCM_MbxChId chId, MBX_ShmAttr shmAttr, MBX_Msg msg)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(chId), BASE_STATUS_ERROR);
    if (chId == handle->srcToDestChId) { /* set source core infomation to handle */
        handle->srcToDestShmAttr.enableConfig = shmAttr.enableConfig; /* update share memory */
        handle->srcToDestShmAttr.lenth = shmAttr.lenth;
        handle->srcToDestShmAttr.startAddr = shmAttr.startAddr;
        handle->srcToDestMsg.dataAddr = msg.dataAddr; /* update message */
        handle->srcToDestMsg.dataIndex = msg.dataIndex;
        handle->srcToDestMsg.dataRemainSize = msg.dataRemainSize;
        handle->srcToDestMsg.status = msg.status;
    } else if (chId == handle->destToSrcChId) {  /* set dest core infomation to handle */
        handle->destToSrcShmAttr.enableConfig = shmAttr.enableConfig; /* update share memory */
        handle->destToSrcShmAttr.lenth = shmAttr.lenth;
        handle->destToSrcShmAttr.startAddr = shmAttr.startAddr;
        handle->destToSrcMsg.dataAddr = msg.dataAddr; /* update message */
        handle->destToSrcMsg.dataIndex = msg.dataIndex;
        handle->destToSrcMsg.dataRemainSize = msg.dataRemainSize;
        handle->destToSrcMsg.status = msg.status;
    } else {
        return BASE_STATUS_ERROR;  /* current coreId is not belong to the maibox handle */
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox single transmit message writting by blocking cpu.
  * @param handle Value of @ref MBX_Handle
  * @param currentChId updata current channal id to handle, Value of @ref IPCM_MbxChId
  * @param currentLenth current data buffer lenth.
  * @param currentShmAttr updata current share memory to handle, Value of @ref MBX_ShmAttr
  * @param currentMsg updata current message to handle, Value of @ref MBX_Msg
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MbxSingleWriteBlocking(MBX_Handle* handle, IPCM_MbxChId currentChId, MBX_ShmAttr currentShmAttr,
                                              unsigned int currentLenth, MBX_Msg* currentMsg)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(currentMsg!= NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(currentChId), BASE_STATUS_ERROR);

    if (currentShmAttr.enableConfig == BASE_CFG_ENABLE) {  /* use share memory */
        *(unsigned char *)(void *)(uintptr_t)(currentShmAttr.startAddr + currentMsg->dataIndex) = \
        *(unsigned char *)(currentMsg->dataAddr++);
    } else {   /* use data register */
        HAL_IPCM_SetData(handle->ipcmHandle, currentChId, currentMsg->dataIndex, \
            *(unsigned char *)(currentMsg->dataAddr++));
    }
    currentMsg->dataIndex++;
    currentMsg->dataRemainSize--;
    if (currentMsg->dataIndex % currentLenth == 0 || currentMsg->dataRemainSize == 0) { /* slice msg data read finish */
        if (currentShmAttr.enableConfig == BASE_CFG_ENABLE) {  /* use share memory */
            HAL_IPCM_SetData(handle->ipcmHandle, currentChId, MBX_DATA_INDEX_0, currentShmAttr.startAddr);
            HAL_IPCM_SetData(handle->ipcmHandle, currentChId, MBX_DATA_INDEX_1, currentShmAttr.lenth);
        }
        if (HAL_IPCM_SendData(handle->ipcmHandle, currentChId) != BASE_STATUS_OK) {
            return BASE_STATUS_ERROR;
        }
        currentMsg->dataIndex = 0;
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox check transmit message finish by write blocking cpu.
  * @param handle Value of @ref MBX_Handle
  * @param currentChId updata current channal id to handle, Value of @ref IPCM_MbxChId
  * @param currentMsg updata current message to handle, Value of @ref MBX_Msg
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MbxCheckWriteBlockingFinish(MBX_Handle* handle, IPCM_MbxChId currentChId, MBX_Msg* currentMsg)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(currentMsg!= NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(currentChId), BASE_STATUS_ERROR);
    if (currentMsg->dataRemainSize == 0) { /* transmit message finish */
        /* clear int status, chid status restore to source */
        ChipCoreId currentCoreId = GetChipCoreId();
        DCL_IPCM_ClearMbxChannelIntStatus(handle->ipcmHandle->baseAddress, currentChId, currentCoreId);
    } else {
        return BASE_STATUS_ERROR;
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Restore mailbox channel status to source.
  * @param handle Value of @ref IPCM_Handle
  * @param currentChId updata current channal id to handle, Value of @ref IPCM_MbxChId
  * @retval None.
  */
static void MBX_StateRestoration(IPCM_Handle *handle, IPCM_MbxChId currentChId)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_PARAM_CHECK_NO_RET(IsMbxChannelId(currentChId));
    ChipCoreId srcCoreId = GetChipCoreId();
    ChipCoreId destCoreId = handle->baseAddress->IPC_MBX[currentChId].MBX_DSTATUS.BIT.dstatus;
    IPCM_MbxChStatus status = handle->baseAddress->IPC_MBX[currentChId].MBX_MODE.BIT.state_status;
    if (status == MBX_STATUS_DEST) { /* transition to reply */
        DCL_IPCM_DestStartSend(handle->baseAddress, currentChId, destCoreId);
        DCL_IPCM_ClearMbxChannelIntStatus(handle->baseAddress, currentChId, destCoreId);
    }
    DCL_IPCM_ClearMbxChannelIntStatus(handle->baseAddress, currentChId, srcCoreId); /* transition to source */
}

/**
  * @brief Time out detection.
  * @param setTick tick count for timeout detection.
  * @param preTick tick count recorded at the start of the function.
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MBX_TimeoutDetection(unsigned long long setTick, unsigned int preTick)
{
    unsigned int curTick = DCL_SYSTICK_GetTick();  /* current tick count */
    unsigned long long deltaTick = (curTick > preTick) ? (curTick - preTick) : (SYSTICK_MAX_VALUE - preTick + curTick);
    if (deltaTick >= setTick) { /* time out */
        return BASE_STATUS_TIMEOUT;
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Wait dest core read and mailbox channel status to reply.
  * @param handle Value of @ref IPCM_Handle
  * @param currentChId updata current channal id to handle, Value of @ref IPCM_MbxChId
  * @param currentMsg updata current message to handle, Value of @ref MBX_Msg
  * @param setTick tick count for timeout detection.
  * @param preTick tick count recorded at the start of the function.
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MBX_WaitDestReply(IPCM_Handle *handle, IPCM_MbxChId currentChId, MBX_Msg* currentMsg,
                                         unsigned long long setTick, unsigned int preTick)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(currentChId), BASE_STATUS_ERROR);
    MBX_ASSERT_PARAM(currentMsg != NULL);
    while (currentMsg->status != MBX_STATUS_REPLY) { /* judge dest core whether has been read finish */
        if (MBX_TimeoutDetection(setTick, preTick) != BASE_STATUS_OK) { /* time out */
            MBX_StateRestoration(handle, currentChId); /* restore mailbox channel status to source. */
            return BASE_STATUS_TIMEOUT;
        }
        currentMsg->status = DCL_IPCM_GetMbxChannelStatus(handle->baseAddress, currentChId);
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox transmit message writting by blocking cpu. support wdata larger than share memory lenth.
  * @param handle Value of @ref MBX_Handle
  * @param wdata data to be write. source core or dest core transmit message.
  * @param size data size.
  * @param timeoutMs Mailbox transmit message timeout.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_WriteBlocking(MBX_Handle* handle, unsigned char *wdata, unsigned int size, unsigned int timeoutMs)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(wdata != NULL);
    IPCM_MbxChId currentChId = handle->srcToDestChId;
    MBX_ShmAttr currentShmAttr = {0};
    MBX_Msg currentMsg = {0};  /* get current core infomation from handle */
    MBX_GetCurrentInfo(handle, currentChId, &currentShmAttr, &currentMsg); /* set current core infomation */
    unsigned int currentLenth = currentShmAttr.enableConfig ? currentShmAttr.lenth : MBX_DATA_INDEX_MAX;
    currentMsg.status = HAL_IPCM_GetStatus(handle->ipcmHandle, currentChId); /* get channel status */
    if (currentMsg.status != MBX_STATUS_SOURCE) { /* write only at source status of channal */
        return BASE_STATUS_ERROR;
    }
    currentMsg.dataRemainSize = size;
    currentMsg.dataAddr = wdata;
    currentMsg.dataIndex = 0;
    unsigned long long setTick = SYSTICK_GetCRGHZ() / SYSTICK_MS_DIV * timeoutMs;
    unsigned int preTick = DCL_SYSTICK_GetTick();
    while (currentMsg.dataRemainSize > 0) { /* judge msg data whether has been write finish */
        if (MBX_TimeoutDetection(setTick, preTick) != BASE_STATUS_OK) { /* time out */
            MBX_StateRestoration(handle->ipcmHandle, currentChId);  /* restore mailbox channel status to source. */
            return BASE_STATUS_TIMEOUT;
        }
        currentMsg.status = DCL_IPCM_GetMbxChannelStatus(handle->ipcmHandle->baseAddress, currentChId);
        if (currentMsg.status == MBX_STATUS_REPLY) { /* clear status convert channel status to source state */
            ChipCoreId currentCoreId = GetChipCoreId();
            DCL_IPCM_ClearMbxChannelIntStatus(handle->ipcmHandle->baseAddress, currentChId, currentCoreId);
        }
        if (currentMsg.status != MBX_STATUS_SOURCE) { /* multi transmit wait channel state convert to source state */
            continue;
        }
        MbxSingleWriteBlocking(handle, currentChId, currentShmAttr, currentLenth, &currentMsg);
    }
    if (MBX_WaitDestReply(handle->ipcmHandle, currentChId, &currentMsg, setTick, preTick) != BASE_STATUS_OK) {
        return BASE_STATUS_TIMEOUT;  /* time out */
    }
    MbxCheckWriteBlockingFinish(handle, currentChId, &currentMsg);
    MBX_UpdataCurrentInfo(handle, currentChId, currentShmAttr, currentMsg); /* set current core infomation */

    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox Check transmit message finish by write it.
  * @param handle Value of @ref MBX_Handle
  * @param retChId updata current channal id to handle, Value of @ref IPCM_MbxChId
  * @param retMsg updata current message to handle, Value of @ref MBX_Msg
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MbxCheckWriteItFinish(MBX_Handle* handle, IPCM_MbxChId retChId)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(retChId), BASE_STATUS_ERROR);
    ChipCoreId coreId = GetChipCoreId();
    /* write finished, disable current core interrupt at current mbx chid */
    HAL_IPCM_Config(handle->ipcmHandle, retChId, TYPE_DISABLE_INT_NOTIFY_CORE, (unsigned int)coreId);
    if (handle->ipcmHandle->userCallBack[retChId].WriteItFinishCallback != NULL) {
        /* user function callback */
        handle->ipcmHandle->userCallBack[retChId].WriteItFinishCallback(handle->ipcmHandle);
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox write data in interrupt callback. message data no larger than share memory lenth.
  * @param handle Value of @ref MBX_Handle
  * @param retChId current channel id. source core or dest core transmit message.
  * @param retShmAttr current share memory. source core or dest core transmit message.
  * @param retMsg current message data. source core or dest core transmit message.
  * @retval None.
  */
static void MbxWriteITCallBack(MBX_Handle* handle, IPCM_MbxChId retChId, MBX_ShmAttr retShmAttr, MBX_Msg* retMsg)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(retMsg!= NULL);
    MBX_PARAM_CHECK_NO_RET(IsMbxChannelId(retChId));
    unsigned int currentLenth = MBX_DATA_INDEX_MAX; /* defualt transmit by data register, lenth is MBX_DATA_INDEX_MAX */
    if (retShmAttr.enableConfig) {
        currentLenth = retShmAttr.lenth;  /* if use share memory, lenth is share memory lenth */
    }
    retMsg->status = HAL_IPCM_GetStatus(handle->ipcmHandle, retChId); /* get channel status */
    if (retMsg->status != MBX_STATUS_SOURCE) {
        return;
    }
    if (retMsg->dataRemainSize == 0) {
        MbxCheckWriteItFinish(handle, retChId);
        return;
    }
    while (retMsg->dataRemainSize > 0) {
        if (retShmAttr.enableConfig) { /* use share memory */
            *(unsigned char *)(void *)(uintptr_t)(retShmAttr.startAddr + retMsg->dataIndex) = \
            *(unsigned char *)(retMsg->dataAddr++);
        } else { /* use data register */
            HAL_IPCM_SetData(handle->ipcmHandle, retChId, retMsg->dataIndex, *(unsigned char *)(retMsg->dataAddr++));
        }
        retMsg->dataIndex++;
        retMsg->dataRemainSize--;
        if (retMsg->dataIndex % currentLenth == 0 || retMsg->dataRemainSize == 0) {
            if (retShmAttr.enableConfig) { /* use share memory, data has transmit finish */
                HAL_IPCM_SetData(handle->ipcmHandle, retChId, MBX_DATA_INDEX_0, retShmAttr.startAddr);
                HAL_IPCM_SetData(handle->ipcmHandle, retChId, MBX_DATA_INDEX_1, retMsg->dataIndex);
            }
            retMsg->dataIndex = 0;
            MBX_UpdataCurrentInfo(handle, retChId, retShmAttr, *retMsg);
            HAL_IPCM_SendData(handle->ipcmHandle, retChId);  /* send after info update */
            break;  /* slice msg data and triggle interrupt */
        }
    }
}

/**
  * @brief Mailbox transmit message writting by interrupt. support wdata larger than share memory lenth.
  * @param handle Value of @ref MBX_Handle
  * @param wdata data to be write. source core or dest core transmit message.
  * @param size data size.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_WriteIT(MBX_Handle* handle, unsigned char *wdata, unsigned int size)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(wdata != NULL);
    IPCM_MbxChId currentChId = handle->srcToDestChId;
    MBX_ShmAttr currentShmAttr = {0};
    MBX_Msg currentMsg = {0};  /* get current core infomation from handle */
    ChipCoreId coreId = GetChipCoreId();
    MBX_GetCurrentInfo(handle, currentChId, &currentShmAttr, &currentMsg);
    currentMsg.status = HAL_IPCM_GetStatus(handle->ipcmHandle, currentChId);
    if (currentMsg.status == MBX_STATUS_SOURCE) { /* write only at source status of channal */
        currentMsg.dataAddr = wdata;
        currentMsg.dataRemainSize = size;
        currentMsg.dataIndex = 0;
        /* enable interrupt notify core function */
        HAL_IPCM_Config(handle->ipcmHandle, currentChId, TYPE_ENABLE_INT_NOTIFY_CORE, (unsigned int)coreId);
        MbxWriteITCallBack(handle, currentChId, currentShmAttr, &currentMsg); /* mailbox write interrupt callback */
    } else {
        return BASE_STATUS_BUSY;
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox single transmit message by read blocking cpu.
  * @param handle Value of @ref MBX_Handle
  * @param currentChId updata current channal id to handle, Value of @ref IPCM_MbxChId
  * @param currentShmAttr updata current share memory to handle, Value of @ref MBX_ShmAttr
  * @param currentLenth current data buffer lenth.
  * @param currentMsg updata current message to handle, Value of @ref MBX_Msg
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MbxSingleReadBlocking(MBX_Handle* handle, IPCM_MbxChId currentChId, MBX_ShmAttr currentShmAttr,
                                             unsigned int currentLenth, MBX_Msg* currentMsg)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(currentMsg!= NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(currentChId), BASE_STATUS_ERROR);
    unsigned int rcvLen = currentShmAttr.enableConfig ? currentShmAttr.lenth : currentLenth;
    if (currentShmAttr.enableConfig == BASE_CFG_ENABLE) { /* use share memory */
        *(unsigned char *)(currentMsg->dataAddr++) = \
        *(unsigned char *)(void *)(uintptr_t)(currentShmAttr.startAddr + currentMsg->dataIndex);
    } else { /* use data register */
        *(unsigned char *)(currentMsg->dataAddr++) = \
        HAL_IPCM_GetData(handle->ipcmHandle, currentChId, currentMsg->dataIndex);
    }
    currentMsg->dataIndex++;
    currentMsg->dataRemainSize--;
    if (currentMsg->dataIndex % rcvLen == 0 || currentMsg->dataRemainSize == 0) { /* slice msg data read finish */
        currentMsg->dataIndex = 0;                   /* dest state convert to reply state */
        if (HAL_IPCM_SendData(handle->ipcmHandle, currentChId) != BASE_STATUS_OK) {
            return BASE_STATUS_ERROR;
        }
        ChipCoreId currentCoreId = GetChipCoreId();
        DCL_IPCM_ClearMbxChannelIntStatus(handle->ipcmHandle->baseAddress, currentChId, currentCoreId);
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox transmit message reading by blocking cpu. support wdata larger than share memory lenth.
  * @param handle Value of @ref MBX_Handle
  * @param rData save read data. source core or dest core transmit message.
  * @param size data size.
  * @param timeoutMs Mailbox transmit message timeout.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_ReadBlocking(MBX_Handle* handle, unsigned char* rData, unsigned int rsize, unsigned int timeoutMs)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(rData != NULL);
    IPCM_MbxChId currentChId = handle->destToSrcChId;
    MBX_ShmAttr currentShmAttr = {0};
    MBX_Msg currentMsg = {0};  /* get current core infomation from handle */

    MBX_GetCurrentInfo(handle, currentChId, &currentShmAttr, &currentMsg);
    unsigned int currentLenth = MBX_DATA_INDEX_MAX; /* defualt transmit by data register, lenth is MBX_DATA_INDEX_MAX */
    currentMsg.dataRemainSize = rsize;
    currentMsg.dataAddr = rData;
    currentMsg.dataIndex = 0;
    unsigned long long setTick = HAL_CRG_GetIpFreq(SYSTICK_BASE) / SYSTICK_MS_DIV * timeoutMs;
    unsigned int preTick = DCL_SYSTICK_GetTick(); /* tick count recorded at the start of the function. */
    while (currentMsg.dataRemainSize > 0) {
        /* Time out detection */
        if (MBX_TimeoutDetection(setTick, preTick) != BASE_STATUS_OK) { /* time out */
            MBX_StateRestoration(handle->ipcmHandle, currentChId);  /* restore mailbox channel status to source. */
            return BASE_STATUS_TIMEOUT;
        }
        currentMsg.status = DCL_IPCM_GetMbxChannelStatus(handle->ipcmHandle->baseAddress, currentChId);
        /* when msg data need multi transmit, wait channel state convert to source state */
        if (currentMsg.status != MBX_STATUS_DEST) {
            continue;
        }
        if (currentMsg.dataIndex == 0) {  /* get data start address and lenth from ipcm */
            currentShmAttr.startAddr = HAL_IPCM_GetData(handle->ipcmHandle, currentChId, MBX_DATA_INDEX_0);
            currentShmAttr.lenth = HAL_IPCM_GetData(handle->ipcmHandle, currentChId, MBX_DATA_INDEX_1);
        }
        MbxSingleReadBlocking(handle, currentChId, currentShmAttr, currentLenth, &currentMsg);
    }
    if (MBX_UpdataCurrentInfo(handle, currentChId, currentShmAttr, currentMsg) != BASE_STATUS_OK) {
        return BASE_STATUS_ERROR;  /* set current core infomation to handle */
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox Check read message finish by write it.
  * @param handle Value of @ref MBX_Handle
  * @param retChId updata current channal id to handle, Value of @ref IPCM_MbxChId
  * @param retMsg updata current message to handle, Value of @ref MBX_Msg
  * @retval status @ref BASE_StatusType.
  */
static BASE_StatusType MbxCheckReadItFinish(MBX_Handle* handle, IPCM_MbxChId retChId, MBX_Msg* retMsg)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(retMsg!= NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxChannelId(retChId), BASE_STATUS_ERROR);
    ChipCoreId coreId = GetChipCoreId();
    if (retMsg->dataRemainSize == 0) { /* read finish excute callback */
        /* read finished, disable current core interrupt at current mbx chid */
        HAL_IPCM_Config(handle->ipcmHandle, retChId, TYPE_DISABLE_INT_NOTIFY_CORE, (unsigned int)coreId);
        if (handle->ipcmHandle->userCallBack[retChId].ReadItFinishCallback != NULL) {
            handle->ipcmHandle->userCallBack[retChId].ReadItFinishCallback(handle->ipcmHandle);
        }  /* execute read finish callback function */
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox read data in interrupt callback. message data no larger than share memory lenth.
  * @param handle Value of @ref MBX_Handle
  * @param retChId current channel id. source core or dest core transmit message.
  * @param retShmAttr current share memory. source core or dest core transmit message.
  * @param retMsg current message data. source core or dest core transmit message.
  * @retval None.
  */
static void MbxReadITCallBack(MBX_Handle* handle, IPCM_MbxChId retChId, MBX_ShmAttr retShmAttr, MBX_Msg* retMsg)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(retMsg != NULL);    /* param check */
    MBX_PARAM_CHECK_NO_RET(IsMbxChannelId(retChId));
    unsigned int rcvStartAddr = 0;
    unsigned int rcvLen = MBX_DATA_INDEX_MAX;
    ChipCoreId coreId = GetChipCoreId();
    retMsg->status = HAL_IPCM_GetStatus(handle->ipcmHandle, retChId);
    if (retMsg->status != MBX_STATUS_DEST) { /* read only at dest status of channal */
        return;
    }
    if (retShmAttr.enableConfig == BASE_CFG_ENABLE) {
        rcvStartAddr = HAL_IPCM_GetData(handle->ipcmHandle, retChId, MBX_DATA_INDEX_0);
        rcvLen = HAL_IPCM_GetData(handle->ipcmHandle, retChId, MBX_DATA_INDEX_1);
    }
    while (retMsg->dataRemainSize > 0) {
        if (retShmAttr.enableConfig == BASE_CFG_ENABLE) {  /* use share memory */
            *(unsigned char *)(retMsg->dataAddr++) = \
            *(unsigned char *)(void *)(uintptr_t)(rcvStartAddr + retMsg->dataIndex);
        } else {  /* use data register */
            *(unsigned char *)(retMsg->dataAddr++) = \
            HAL_IPCM_GetData(handle->ipcmHandle, retChId, retMsg->dataIndex);
        }
        retMsg->dataIndex++;
        retMsg->dataRemainSize--;
        if (retMsg->dataIndex % rcvLen == 0 || retMsg->dataRemainSize == 0) {
            retMsg->dataIndex = 0;
            MBX_UpdataCurrentInfo(handle, retChId, retShmAttr, *retMsg);
            HAL_IPCM_SendData(handle->ipcmHandle, retChId);
            DCL_IPCM_ClearMbxChannelIntStatus(handle->ipcmHandle->baseAddress, retChId, coreId);
            break;   /* slice read finish and return */
        }
    }
    MbxCheckReadItFinish(handle, retChId, retMsg);
}


/**
  * @brief Mailbox transmit message reading by interrupt. support wdata larger than share memory lenth.
  * @param handle Value of @ref MBX_Handle
  * @param rdata save read data. source core or dest core transmit message.
  * @param size data size.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_ReadIT(MBX_Handle* handle, unsigned char *rdata, unsigned int size)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(rdata != NULL);
    IPCM_MbxChId currentChId = handle->destToSrcChId;
    MBX_ShmAttr currentShmAttr = {0};
    MBX_Msg currentMsg = {0};  /* get current core infomation from handle */
    ChipCoreId coreId = GetChipCoreId();
    
    MBX_GetCurrentInfo(handle, currentChId, &currentShmAttr, &currentMsg);
    currentMsg.dataAddr = rdata;
    currentMsg.dataRemainSize = size;
    currentMsg.dataIndex = 0;
    if (MBX_UpdataCurrentInfo(handle, currentChId, currentShmAttr, currentMsg) != BASE_STATUS_OK) {
        return BASE_STATUS_ERROR;  /* set current core infomation to handle */
    }
    /* read begin, enable current core interrupt at current mbx chid */
    HAL_IPCM_Config(handle->ipcmHandle, currentChId, TYPE_ENABLE_INT_NOTIFY_CORE, (unsigned int)coreId);
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox write and read get message infomation by blocking cpu. support wdata larger than share memory lenth.
  * @param handle Value of @ref MBX_Handle
  * @param wdata data to be write. source core or dest core transmit message.
  * @param wsize write data size.
  * @param rData save read data. source core or dest core transmit message.
  * @param rsize read data size.
  * @param timeoutMs Mailbox transmit message timeout.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_WriteReadBlocking(MBX_Handle* handle, unsigned char *wdata, unsigned int wsize,
                                      unsigned char *rdata, unsigned int rsize, unsigned int timeoutMs)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(wdata != NULL);
    MBX_ASSERT_PARAM(rdata != NULL);
    if (handle->handleEx.getInfoMode == MODE_WRITE) {     /* current core is source core */
        if (MBX_WriteBlocking(handle, wdata, wsize, timeoutMs) != BASE_STATUS_OK) {   /* write blocking first */
            return BASE_STATUS_ERROR;
        }
        if (MBX_ReadBlocking(handle, rdata, rsize, timeoutMs) != BASE_STATUS_OK) {    /* read blocking second */
            return BASE_STATUS_ERROR;
        }
    } else if (handle->handleEx.getInfoMode == MODE_READ) {   /* current core is dest core */
        if (MBX_ReadBlocking(handle, rdata, rsize, timeoutMs) != BASE_STATUS_OK) {    /* read blocking first */
            return BASE_STATUS_ERROR;
        }
        if (MBX_WriteBlocking(handle, wdata, wsize, timeoutMs) != BASE_STATUS_OK) {   /* write blocking second */
            return BASE_STATUS_ERROR;
        }
    } else {     /* current core is not from current handle */
        return BASE_STATUS_ERROR;
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox write and read get message infomation by interrupt. support wdata larger than share memory lenth.
  * @param handle Value of @ref MBX_Handle
  * @param wdata data to be write. source core or dest core transmit message.
  * @param wsize write data size.
  * @param rData save read data. source core or dest core transmit message.
  * @param rsize read data size.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_WriteReadIT(MBX_Handle* handle, unsigned char *wdata, unsigned int wsize,
                                unsigned char *rdata, unsigned int rsize)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_ASSERT_PARAM(wdata != NULL);
    MBX_ASSERT_PARAM(rdata != NULL);
    if (MBX_WriteIT(handle, wdata, wsize) != BASE_STATUS_OK) { /* write it */
        return BASE_STATUS_ERROR;
    }
    if (MBX_ReadIT(handle, rdata, rsize) != BASE_STATUS_OK) { /* read it */
        return BASE_STATUS_ERROR;
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Due to ipcm only has one IrqNum, but mbxhandle include two channel id,
  *        multicore have multiple mbxhandle groups, So need to register mbxhandle to groups.
  * @param mbxGroups Value of @ref MBX_Groups
  * @param handle Value of @ref MBX_Handle
  * @param connectNum Value of @ref MBX_ConnectGroup
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_RegisterHandleToGroups(MBX_Groups* mbxGroups, MBX_ConnectGroup connectNum, MBX_Handle* handle)
{
    MBX_ASSERT_PARAM(mbxGroups != NULL);
    MBX_PARAM_CHECK_WITH_RET(IsConnectGroup(connectNum), BASE_STATUS_ERROR);
    MBX_ASSERT_PARAM(handle != NULL);
    mbxGroups->mbxHandles[connectNum] = handle;   /* register mbxhandle into mbx groups */
    return BASE_STATUS_OK;
}

/**
  * @brief Register mailbox channel interrupt callback in different interrupt type.
  * @param handle Value of @ref MBX_Handle
  * @param mbxChId Value of @ref IPCM_MbxChId
  * @param type Value of @ref IPCM_CallbackType
  * @param callBackFunc callback function of @ref IPCM_CallbackFunc
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType MBX_RegisterCallback(MBX_Handle* handle, IPCM_CallbackType type, IPCM_CallbackFunc callBackFunc)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_PARAM_CHECK_WITH_RET(IsMbxCallbackType(type), BASE_STATUS_ERROR);
    IPCM_MbxChId currentChId = MBX_CH_MAX;
    if (type == WRITE_FINISH_TYPE) {  /* current core is source core */
        currentChId = handle->srcToDestChId;  /* get source core infomation from handle */
    } else if (type == READ_FINISH_TYPE) { /* current core is dest core */
        currentChId = handle->destToSrcChId;  /* get dest core infomation from handle */
    }
    HAL_IPCM_RegisterCallback(handle->ipcmHandle, currentChId, type, callBackFunc);
    return BASE_STATUS_OK;
}

/**
  * @brief Mailbox handle interrupt function. param is a mailbox groups, because ipcm only has one irqNumber.
  * @param handle is a mailbox groups
  * @retval None.
  */
void MBX_IrqHandler(void* handle)
{
    MBX_ASSERT_PARAM(handle != NULL);
    MBX_Groups* mbxGroups = (MBX_Groups *)handle;   /* convert into mbx groups */
    IPCM_MbxChId currentChId = MBX_CH_MAX;
    MBX_ShmAttr currentShmAttr = {0};
    MBX_Msg currentMsg = {0};
    ChipCoreId coreId = GetChipCoreId();
    unsigned int intMbxChBitSet = 0x00000000U;
    for (unsigned int i = 0 ; i < CONNECT_GROUP_MAX; ++i) {
        MBX_Handle* currentMbx = mbxGroups->mbxHandles[i];
        if (currentMbx == (MBX_Handle *)NULL) {
            continue;
        }
        MBX_ASSERT_PARAM(currentMbx->ipcmHandle != NULL);
        intMbxChBitSet = DCL_IPCM_GetCoreMaskIntStatus(currentMbx->ipcmHandle->baseAddress, coreId);
        if (intMbxChBitSet & (1 << currentMbx->srcToDestChId)) { /* srcToDestChId interrupt */
            currentChId = currentMbx->srcToDestChId;
        } else if (intMbxChBitSet & (1 << currentMbx->destToSrcChId)) { /* destToSrcChId interrupt */
            currentChId = currentMbx->destToSrcChId;
        } else { /* current mailbox has no interrupt */
            continue;
        }
        /* get current core infomation from handle */
        MBX_GetCurrentInfo(currentMbx, currentChId, &currentShmAttr, &currentMsg);
        /* msg data larger than share memory lenth or data register lenth, write next slice data at reply state */
        if (HAL_IPCM_GetStatus(currentMbx->ipcmHandle, currentChId) == MBX_STATUS_REPLY) {
            DCL_IPCM_ClearMbxChannelIntStatus(currentMbx->ipcmHandle->baseAddress, currentChId, coreId);
            MbxWriteITCallBack(currentMbx, currentChId, currentShmAttr, &currentMsg);
        } else if (HAL_IPCM_GetStatus(currentMbx->ipcmHandle, currentChId) == MBX_STATUS_DEST) {
            /* msg data larger than share memory lenth or data register lenth, read next slice data at reply state */
            MbxReadITCallBack(currentMbx, currentChId, currentShmAttr, &currentMsg);
        }
    }
    return;
}
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
  * @file      ipcm.c
  * @author    MCU Driver Team
  * @brief     IPCM module driver
  * @details   This file provides firmware functions to manage the following functionalities of the IPCM.
  *             + Initialization functions.
  *             + IPCM Set And Get Functions.
  *             + Interrupt Handler Functions.
  */

/* Includes ------------------------------------------------------------------*/
#include "interrupt.h"
#include "ipcm.h"

/**
  * @brief Init IPCM mailbox channel.
  * @param handle Value of @ref IPCM_Handle
  * @param mbxChId Value of @ref IPCM_MbxChId
  * @param chCfg Value of @ref IPCM_MbxChAttr
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_IPCM_Init(IPCM_Handle *handle, IPCM_MbxChId mbxChId, IPCM_MbxChAttr chCfg)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_PARAM_CHECK_WITH_RET(IsMbxChannelId(mbxChId), BASE_STATUS_ERROR);
    IPCM_PARAM_CHECK_WITH_RET(IsChipCoreId(chCfg.srcCoreId), BASE_STATUS_ERROR);
    IPCM_PARAM_CHECK_WITH_RET(IsChipCoreId(chCfg.destCoreId), BASE_STATUS_ERROR);
    IPCM_PARAM_CHECK_WITH_RET(IsMbxChannelMode(chCfg.mbxChMode), BASE_STATUS_ERROR);
    DCL_IPCM_UnLockRegs(handle->baseAddress); /* unlock register */
    IPCM_MbxChStatus status = DCL_IPCM_GetMbxChannelStatus(handle->baseAddress, mbxChId);
    if (status != MBX_STATUS_FREE) {
        return BASE_STATUS_ERROR;
    }
    DCL_IPCM_EnableMbxChannelSrc(handle->baseAddress, mbxChId, chCfg.srcCoreId);  /* enable channel source core */
    DCL_IPCM_EnableMbxChannelDest(handle->baseAddress, mbxChId, chCfg.destCoreId);  /* enable channel dest core */
    DCL_IPCM_SetMbxChannelMode(handle->baseAddress, mbxChId, chCfg.mbxChMode); /* set channel transmit mode */
    return BASE_STATUS_OK;
}

/**
  * @brief DeInit IPCM mailbox channel.
  * @param handle Value of @ref IPCM_Handle.
  * @param mbxChId Value of @ref IPCM_MbxChId.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_IPCM_DeInit(IPCM_Handle *handle, IPCM_MbxChId mbxChId)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_PARAM_CHECK_WITH_RET(IsMbxChannelId(mbxChId), BASE_STATUS_ERROR);
    DCL_IPCM_DisableMbxChannelDest(handle->baseAddress, mbxChId); /* disable channel dest core */
    DCL_IPCM_DisableMbxChannelSrc(handle->baseAddress, mbxChId); /* disable channel source core */
    return BASE_STATUS_OK;
}

/**
  * @brief Config IPCM mailbox channel different data value by type.
  * @param handle Value of @ref IPCM_Handle.
  * @param mbxChId Value of @ref IPCM_MbxChId.
  * @param type Value of @ref IPCM_ConfigType.
  * @param value data value to be set.
  * @retval None.
  */
void HAL_IPCM_Config(IPCM_Handle *handle, IPCM_MbxChId mbxChId, IPCM_ConfigType type, unsigned int value)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_PARAM_CHECK_NO_RET(IsMbxChannelId(mbxChId));
    switch (type) {
        case TYPE_ENABLE_SRC_CORE_ID:   /* set channel source id */
            DCL_IPCM_EnableMbxChannelSrc(handle->baseAddress, mbxChId, (ChipCoreId)(value & CHIP_CORE_MASK));
            break;
        case TYPE_ENABLE_DEST_CORE_ID:  /* set channel dest id */
            DCL_IPCM_EnableMbxChannelDest(handle->baseAddress, mbxChId, (ChipCoreId)(value & CHIP_CORE_MASK));
            break;
        case TYPE_ENABLE_MBX_CH_MODE:   /* set channel transmit mode */
            DCL_IPCM_SetMbxChannelMode(handle->baseAddress, mbxChId, (IPCM_MbxChMode)(value & MBX_MODE_MASK));
            break;
        case TYPE_ENABLE_INT_NOTIFY_CORE:   /* enable channel interrupt */
            DCL_IPCM_EnableMbxChannelInterrupt(handle->baseAddress, mbxChId, (ChipCoreId)(value & CHIP_CORE_MASK));
            break;
        case TYPE_DISABLE_INT_NOTIFY_CORE:   /* disable channel interrupt */
            DCL_IPCM_DisableMbxChannelInterrupt(handle->baseAddress, mbxChId, (ChipCoreId)(value & CHIP_CORE_MASK));
            break;
        case TYPE_CH_WRITE_FINISH_CALLBACK:   /* set channel write finish callback */
            handle->userCallBack[mbxChId].WriteItFinishCallback = (IPCM_CallbackFunc)(void *)(uintptr_t)value;
            break;
        case TYPE_CH_READ_FINISH_CALLBACK:   /* set channel read finish callback */
            handle->userCallBack[mbxChId].ReadItFinishCallback = (IPCM_CallbackFunc)(void *)(uintptr_t)value;
            break;
        default:
            break;
    }
}

/**
  * @brief Config IPCM mailbox channel.
  * @param handle Value of @ref IPCM_Handle.
  * @param mbxChId Value of @ref IPCM_MbxChId.
  * @param enableConfig enable or disable notify core function.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_IPCM_NotifyCore(IPCM_Handle *handle, IPCM_MbxChId mbxChId, bool enableConfig)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_PARAM_CHECK_WITH_RET(IsMbxChannelId(mbxChId), BASE_STATUS_ERROR);
    ChipCoreId srcCoreId = DCL_IPCM_GetMbxChannelSrc(handle->baseAddress, mbxChId);
    ChipCoreId destCoreId = DCL_IPCM_GetMbxChannelDest(handle->baseAddress, mbxChId);
    if (enableConfig) {            /* enable channel interrupt */
        DCL_IPCM_EnableMbxChannelInterrupt(handle->baseAddress, mbxChId, srcCoreId);
        DCL_IPCM_EnableMbxChannelInterrupt(handle->baseAddress, mbxChId, destCoreId);
    } else {           /* disable channel interrupt */
        DCL_IPCM_DisableMbxChannelInterrupt(handle->baseAddress, mbxChId, srcCoreId);
        DCL_IPCM_DisableMbxChannelInterrupt(handle->baseAddress, mbxChId, destCoreId);
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Send IPCM mailbox channel data.
  * @param handle Value of @ref IPCM_Handle.
  * @param mbxChId Value of @ref IPCM_MbxChId.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_IPCM_SendData(IPCM_Handle *handle, IPCM_MbxChId mbxChId)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_PARAM_CHECK_WITH_RET(IsMbxChannelId(mbxChId), BASE_STATUS_ERROR);
    IPCM_MbxChStatus status = DCL_IPCM_GetMbxChannelStatus(handle->baseAddress, mbxChId);
    ChipCoreId currentCoreId = GetChipCoreId();
    unsigned int coreIds;
    switch (status) {
        case MBX_STATUS_SOURCE:      /* source core send data */
            coreIds = DCL_IPCM_GetMbxChannelSrc(handle->baseAddress, mbxChId);
            if ((currentCoreId & coreIds) == 0) { /* source status only src core can send */
                return BASE_STATUS_ERROR;
            }
            DCL_IPCM_SrcStartSend(handle->baseAddress, mbxChId, currentCoreId);
            break;
        case MBX_STATUS_DEST:        /* dest core send data */
            coreIds = DCL_IPCM_GetMbxChannelDest(handle->baseAddress, mbxChId);
            if ((currentCoreId & coreIds) == 0) { /* Destination status only dest core can send */
                return BASE_STATUS_ERROR;
            }
            DCL_IPCM_DestStartSend(handle->baseAddress, mbxChId, currentCoreId);
            break;
        default:
            break;
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Set channel data by index.
  * @param handle Value of @ref IPCM_Handle.
  * @param mbxChId Value of @ref IPCM_MbxChId.
  * @param dataIndex Value of @ref IPCM_MbxChDataIndex.
  * @param dataVal data value
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_IPCM_SetData(IPCM_Handle *handle, IPCM_MbxChId mbxChId,
                                 IPCM_MbxChDataIndex dataIndex, unsigned int dataVal)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_PARAM_CHECK_WITH_RET(IsMbxChannelId(mbxChId), BASE_STATUS_ERROR);
    IPCM_PARAM_CHECK_WITH_RET(IsMbxChannelDataIndex(dataIndex), BASE_STATUS_ERROR);
    DCL_IPCM_SetMbxData(handle->baseAddress, mbxChId, dataIndex, dataVal); /* set channel data by index */
    return BASE_STATUS_OK;
}

/**
  * @brief Get channel data by index.
  * @param handle Value of @ref IPCM_Handle.
  * @param mbxChId Value of @ref IPCM_MbxChId.
  * @param dataIndex Value of @ref IPCM_MbxChDataIndex.
  * @retval unsigned int data of index.
  */
unsigned int HAL_IPCM_GetData(IPCM_Handle *handle, IPCM_MbxChId mbxChId, IPCM_MbxChDataIndex dataIndex)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxChId));
    IPCM_ASSERT_PARAM(IsMbxChannelDataIndex(dataIndex));
    unsigned int ret = DCL_IPCM_GetMbxData(handle->baseAddress, mbxChId, dataIndex); /* get channel data by index */
    return ret;
}

/**
  * @brief Get IPCM mailbox channel status.
  * @param handle Value of @ref IPCM_Handle.
  * @param mbxChId Value of @ref IPCM_MbxChId.
  * @retval status @ref BASE_StatusType.
  */
IPCM_MbxChStatus HAL_IPCM_GetStatus(IPCM_Handle *handle, IPCM_MbxChId mbxChId)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_PARAM_CHECK_WITH_RET(IsMbxChannelId(mbxChId), MBX_STATUS_MAX);
    IPCM_MbxChStatus chStatus = DCL_IPCM_GetMbxChannelStatus(handle->baseAddress, mbxChId);
    return chStatus;
}


/**
  * @brief Register mailbox channel interrupt callback in different interrupt type.
  * @param handle Value of @ref IPCM_Handle
  * @param mbxChId Value of @ref IPCM_MbxChId
  * @param type Value of @ref IPCM_CallbackType
  * @param callBackFunc callback function of @ref IPCM_CallbackFunc
  * @retval None.
  */
void HAL_IPCM_RegisterCallback(IPCM_Handle *handle, IPCM_MbxChId mbxChId,
                               IPCM_CallbackType type, IPCM_CallbackFunc callBackFunc)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_ASSERT_PARAM(callBackFunc != NULL);
    IPCM_PARAM_CHECK_NO_RET(IsMbxChannelId(mbxChId));
    IPCM_PARAM_CHECK_NO_RET(IsMbxCallbackType(type));
    if (type == WRITE_FINISH_TYPE) {   /* register write finish callback */
        handle->userCallBack[mbxChId].WriteItFinishCallback = callBackFunc;
    } else if (type == READ_FINISH_TYPE) {    /* register read finish callback */
        handle->userCallBack[mbxChId].ReadItFinishCallback = callBackFunc;
    }
}

/**
  * @brief Handle ipcm interrupt.
  * @param handle Value of @ref IPCM_Handle
  * @retval None.
  */
void HAL_IPCM_IrqHandler(void *handle)
{
    IPCM_ASSERT_PARAM(handle != NULL);
    IPCM_Handle *ipcmHandle = (IPCM_Handle *)handle;
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcmHandle->baseAddress));
    ChipCoreId currentCoreId;
    IPCM_MbxChStatus chStatus = MBX_STATUS_MAX;
    unsigned int intMbxChBitSet = 0x00000000U;
    IPCM_MbxChId mbxChCurrentId = MBX_CH_0;
    unsigned int mbxChCurrentBit = 0x00000000U;
    currentCoreId = GetChipCoreId();    /* get current core id */
    intMbxChBitSet = DCL_IPCM_GetCoreMaskIntStatus(ipcmHandle->baseAddress, currentCoreId);
    /* Determine which pin sets the callback function. */
    while ((intMbxChBitSet >> mbxChCurrentId) != BASE_CFG_UNSET && mbxChCurrentId < MBX_CH_MAX) {
        mbxChCurrentBit = intMbxChBitSet & (1 << mbxChCurrentId);
        chStatus = DCL_IPCM_GetMbxChannelStatus(ipcmHandle->baseAddress, mbxChCurrentId);
        if (!mbxChCurrentBit) {
            mbxChCurrentId++;
            continue;
        }
        /* when channel status is dest, then execute write finish callback */
        if (chStatus == MBX_STATUS_DEST) {
            if (ipcmHandle->userCallBack[mbxChCurrentId].WriteItFinishCallback != NULL) {
                ipcmHandle->userCallBack[mbxChCurrentId].WriteItFinishCallback(ipcmHandle);
            }
        }
        /* when channel status is reply, then execute read finish callback */
        if (chStatus == MBX_STATUS_REPLY) {
            if (ipcmHandle->userCallBack[mbxChCurrentId].ReadItFinishCallback != NULL) {
                ipcmHandle->userCallBack[mbxChCurrentId].ReadItFinishCallback(ipcmHandle);
            }
        }
        /* clear current core interrupt */
        DCL_IPCM_ClearMbxChannelIntStatus(ipcmHandle->baseAddress, mbxChCurrentId, currentCoreId);
        mbxChCurrentId++;
    }
}
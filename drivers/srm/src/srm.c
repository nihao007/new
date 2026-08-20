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
  * @file      srm.c
  * @author    MCU Driver Team
  * @brief     SRM module driver
  * @details   This file provides firmware functions to manage the following functionalities of the SRM.
  *             + Initialization functions.
  *             + SRM Set And Get Functions.
  *             + Interrupt Handler Functions.
  */

/* Includes ------------------------------------------------------------------*/
#include "interrupt.h"
#include "srm.h"

/**
  * @brief Init SRM handle.
  * @param handle Value of @ref SRM_Handle
  * @param srmChId Value of @ref SRM_ChannelId
  * @param attr Value of @ref SRM_ChannelAttr
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_SRM_Init(SRM_Handle *handle, SRM_ChannelId srmChId, SRM_ChannelAttr attr)
{
    SRM_ASSERT_PARAM(handle != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmChId), BASE_STATUS_ERROR);
    ChipCoreId coreId = GetChipCoreId();  /* get current core id */
    if (attr.intEnable) {   /* interrupt enable */
        DCL_SRM_EnableCoreChannelInt(handle->baseAddress, coreId, srmChId);   /* enable interrupt by chId */
    } else {
        DCL_SRM_DisableCoreChannelInt(handle->baseAddress, coreId, srmChId);   /* disable interrupt by chId */
    }
    return BASE_STATUS_OK;
}

/**
  * @brief DeInit SRM handle.
  * @param handle Value of @ref SRM_Handle.
  * @param srmChId Value of @ref SRM_ChannelId.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_SRM_DeInit(SRM_Handle *handle, SRM_ChannelId srmChId)
{
    SRM_ASSERT_PARAM(handle != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmChId), BASE_STATUS_ERROR);
    ChipCoreId coreId = GetChipCoreId();  /* get current core id */
    DCL_SRM_DisableCoreChannelInt(handle->baseAddress, coreId, srmChId);  /* disable interrupt by chId */
    DCL_SRM_UnLockChannel(handle->baseAddress, srmChId);   /* unlock srm channel */
    DCL_SRM_DisableWfi(handle->baseAddress);    /* disable srm channel wfi */
    return BASE_STATUS_OK;
}

/**
  * @brief Check SRM channel lock status.
  * @param handle Value of @ref SRM_Handle.
  * @param srmChId Value of @ref SRM_ChannelId.
  * @retval status true means lock status, false means unlock status.
  */
bool HAL_SRM_CheckLockStatus(SRM_Handle *handle, SRM_ChannelId srmChId)
{
    SRM_ASSERT_PARAM(handle != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmChId), false);
    return DCL_SRM_GetChannelLockStatus(handle->baseAddress, srmChId);   /* get srm channel lock status */
}

/**
  * @brief Lock SRM channnel by block cpu untill locked.
  * @param handle Value of @ref SRM_Handle
  * @param srmChId Value of @ref SRM_ChannelId
  * @param lockTimeOutUs block cpu timeout untill lock srm channel
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_SRM_Lock(SRM_Handle *handle, SRM_ChannelId srmChId, unsigned int lockTimeOutUs)
{
    SRM_ASSERT_PARAM(handle != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmChId), BASE_STATUS_ERROR);
    unsigned int currentTimeTick = 0;
    unsigned long long deltaTimeUs = 0;
    unsigned int sysClkFreq = BASE_FUNC_GetCpuFreqHz();  /* get cpu frequence */
    if (sysClkFreq == 0) {
        return BASE_STATUS_ERROR;
    }
    unsigned int preTimeTick = BASE_FUNC_GetTick();
    while (deltaTimeUs < lockTimeOutUs) { /* check srm channel status */
        if (DCL_SRM_GetChannelLockStatus(handle->baseAddress, srmChId) == false) {
            DCL_SRM_LockChannel(handle->baseAddress, srmChId);  /* lock srm channel */
            if (DCL_SRM_GetChannelCoreId(handle->baseAddress, srmChId) == GetChipCoreId()) {
                return BASE_STATUS_OK;
            }
        }
        currentTimeTick = BASE_FUNC_GetTick();
        deltaTimeUs = ((currentTimeTick > preTimeTick) ? (currentTimeTick - preTimeTick) :
                       (unsigned long long)((SYSTICK_MAX_VALUE - preTimeTick + currentTimeTick))) *
                       1000000 / sysClkFreq; /* 1000000 : us */
    }
    return BASE_STATUS_ERROR;
}

/**
  * @brief Try to lock srm channel.
  * @param handle Value of @ref SRM_Handle
  * @param srmChId Value of @ref SRM_MbxChId
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_SRM_TryLock(SRM_Handle *handle, SRM_ChannelId srmChId)
{
    SRM_ASSERT_PARAM(handle != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmChId), BASE_STATUS_ERROR);
    if (DCL_SRM_GetChannelLockStatus(handle->baseAddress, srmChId)) { /* check srm channel whether locked */
        return BASE_STATUS_ERROR;
    }
    DCL_SRM_LockChannel(handle->baseAddress, srmChId); /* if srm channel not lock, then lock it */
    if (DCL_SRM_GetChannelCoreId(handle->baseAddress, srmChId) != GetChipCoreId()) {
        return BASE_STATUS_ERROR;
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Unlock srm channel by id.
  * @param handle Value of @ref SRM_Handle
  * @param srmChId Value of @ref SRM_ChannelId
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_SRM_Unlock(SRM_Handle *handle, SRM_ChannelId srmChId)
{
    SRM_ASSERT_PARAM(handle != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmChId), BASE_STATUS_ERROR);
    DCL_SRM_UnLockChannel(handle->baseAddress, srmChId);  /* unlock srm channel */
    return BASE_STATUS_OK;
}

/**
  * @brief Config srm wfi channel.
  * @param handle Value of @ref SRM_Handle
  * @param srmChId Value of @ref SRM_ChannelId
  * @param timeCycle Value of srm wfi timeout.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType HAL_SRM_ConfigWfiChannel(SRM_Handle *handle, SRM_ChannelId srmChId, unsigned char timeCycle)
{
    SRM_ASSERT_PARAM(handle != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmChId), BASE_STATUS_ERROR);
    DCL_SRM_SetWfiChannel(handle->baseAddress, srmChId);  /* set srm wfi channel */
    DCL_SRM_SetWfiTime(handle->baseAddress, timeCycle);  /* set srm wfi timeout */
    DCL_SRM_EnableWfi(handle->baseAddress);  /* enable srm wfi */
    ChipCoreId coreId = GetChipCoreId();  /* get current core id */
    DCL_SRM_EnableCoreChannelInt(handle->baseAddress, coreId, srmChId);  /* enable srm channel interrupt */
    return BASE_STATUS_OK;
}

/**
  * @brief Register srm channel callback by id.
  * @param handle Value of @ref SRM_Handle
  * @param srmChId Value of @ref SRM_ChannelId
  * @param callBackFunc Value of @ref SRM_CallbackFunc
  * @retval None.
  */
void HAL_SRM_RegisterCallback(SRM_Handle *handle, SRM_ChannelId srmChId, SRM_CallbackFunc callBackFunc)
{
    SRM_ASSERT_PARAM(handle != NULL);
    SRM_ASSERT_PARAM(callBackFunc != NULL);
    BASE_FUNC_PARAMCHECK_NO_RET(IsSrmChannelId(srmChId));
    handle->userCallBack[srmChId].FinishCallbackFunc = callBackFunc; /* register callback by srm id */
}

/**
  * @brief Handle srm channel interrupt event.
  * @param handle Value of @ref SRM_Handle
  * @retval None.
  */
void HAL_SRM_IrqHandler(void *handle)
{
    SRM_ASSERT_PARAM(handle != NULL);
    SRM_Handle *srmHandle = (SRM_Handle *)handle;
    SRM_ASSERT_PARAM(IsSRMInstance(srmHandle->baseAddress));
    unsigned int srmChInt = srmHandle->baseAddress->SRM_CORE[GetChipCoreId() >> 1].SRM_CORE_INT;
    SRM_ChannelId srmChId = SRM_CH_ID_0;
    while (srmChId < SRM_CH_ID_MAX) {
        /* check which srm channel trigger interrupt */
        if ((srmChInt >> srmChId) & BASE_CFG_SET) {
            DCL_SRM_ClearChannelInt(srmHandle->baseAddress, srmChId); /* clear srm channel interrupt */
            if (srmHandle->userCallBack[srmChId].FinishCallbackFunc != NULL) {
                srmHandle->userCallBack[srmChId].FinishCallbackFunc(srmHandle);   /* execute callbck */
            }
        }
        srmChId++;
    }
}
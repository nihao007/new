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
  * @file      cpm.c
  * @author    MCU Driver Team
  * @brief     SRM module driver
  * @details   This file provides firmware functions to manage the following functionalities of the SRM.
  *             + Initialization functions.
  *             + SRM Set And Get Functions.
  *             + Interrupt Handler Functions.
  */

/* Includes ------------------------------------------------------------------*/
#include "cpm.h"
#include "crg.h"
#include "srm.h"
#include "flash.h"
#include "pmc.h"

/**
  * @brief Coprocessor set wfi param.
  * @param coreWfiChId Value of @ref SRM_ChannelId
  * @param offPeakTimeCycle off peak time, uint cycle
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType CPM_SetWfiParam(SRM_ChannelId coreWfiChId, unsigned char offPeakTimeCycle)
{
    CPM_PARAM_CHECK_WITH_RET(IsSrmChannelId(coreWfiChId), BASE_STATUS_ERROR);
    DCL_SRM_SetWfiChannel(SRM, coreWfiChId);   /* set srm wfi channel */
    DCL_SRM_SetWfiTime(SRM, offPeakTimeCycle);   /* set srm wfi time */
    DCL_SRM_EnableWfi(SRM);    /* enable srm wfi */
    return BASE_STATUS_OK;
}

/**
  * @brief Set coprocessor start address.
  * @param coreId Value of @ref ChipCoreId.
  * @param startAddr start address.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType CPM_SetStartAddr(ChipCoreId coreId, unsigned int startAddr)
{
    CPM_PARAM_CHECK_WITH_RET(IsChipCoreId(coreId), BASE_STATUS_ERROR);
    if (coreId == CHIP_CORE_0) {  /* main core needn't set */
        return BASE_STATUS_ERROR;
    }
    DCL_SYSCTRL_SetCoreStartAddr(coreId, startAddr);
    return BASE_STATUS_OK;
}

/**
  * @brief Load coprocessor image.
  * @param imageAttr Value of @ref CPM_ImageAttr.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType CPM_LoadImage(CPM_ImageAttr imageAttr)
{
    if (imageAttr.imageType == IMAGE_TYPE_NORMAL) {  /* image normal type, not compress */
        if (imageAttr.startAddr != imageAttr.loadAddr) {  /* run at sram, need copy code to sram */
            while (imageAttr.imageSize--) {
                *(unsigned char *)imageAttr.startAddr = *(unsigned char *)imageAttr.loadAddr;
                imageAttr.startAddr++;
                imageAttr.loadAddr++;
            }
        }
    } else if (imageAttr.imageType == IMAGE_TYPE_COMPRESS) {  /* image compress type */
    } else if (imageAttr.imageType == IMAGE_TYPE_DECOMPRESS) {
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Start coprocessor by coreId.
  * @param coreId Value of @ref ChipCoreId.
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType CPM_Start(ChipCoreId coreId)
{
    CPM_PARAM_CHECK_WITH_RET(IsChipCoreId(coreId), BASE_STATUS_ERROR);
    SRM_ChannelId wfiChId = DCL_SRM_GetWfiChannel(SRM);  /* get wfi srm channel */
    DCL_SRM_LockChannel(SRM, wfiChId); /* lock wfi channel */
    if (DCL_SRM_GetChannelCoreId(SRM, wfiChId) != GetChipCoreId()) { /* check wfi channel whether locked */
        return BASE_STATUS_ERROR;
    }
    DCL_CRG_UnLockCoreResetStatus(CRG, coreId);  /* unlock core reset status */
    DCL_CRG_EnableCoreClk(CRG, coreId);    /* enable core clock */
    DCL_SRM_UnLockChannel(SRM, wfiChId);  /* unlock wfi srm channel */
    return BASE_STATUS_OK;
}

/**
  * @brief Stop coprocessor by coreId.
  * @param coreId Value of @ref ChipCoreId
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType CPM_Stop(ChipCoreId coreId)
{
    CPM_PARAM_CHECK_WITH_RET(IsChipCoreId(coreId), BASE_STATUS_ERROR);
    SRM_ChannelId wfiChId = DCL_SRM_GetWfiChannel(SRM);  /* get wfi srm channel */
    DCL_SRM_LockChannel(SRM, wfiChId); /* lock wfi channel */
    if (DCL_SRM_GetChannelCoreId(SRM, wfiChId) != GetChipCoreId()) { /* check wfi channel whether locked */
        return BASE_STATUS_ERROR;
    }
    DCL_CRG_LockCoreResetStatus(CRG, coreId);  /* lock core reset status */
    DCL_CRG_DisableCoreClk(CRG, coreId);  /* disable core clock */
    DCL_SRM_UnLockChannel(SRM, wfiChId);  /* unlock wfi srm channel */
    return BASE_STATUS_OK;
}

/**
  * @brief Suspend coprocessor.
  * @param void None
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType CPM_Suspend(void)
{
    SRM_ChannelId wfiChId = DCL_SRM_GetWfiChannel(SRM);  /* get wfi srm channel */
    DCL_SRM_LockChannel(SRM, wfiChId); /* lock wfi channel */
    if (DCL_SRM_GetChannelCoreId(SRM, wfiChId) != GetChipCoreId()) { /* check wfi channel whether locked */
        return BASE_STATUS_ERROR;
    }
    DCL_PMC_EnterSleep();    /* Enter sleep */
    DCL_SRM_UnLockChannel(SRM, wfiChId);  /* unlock wfi srm channel */
    return BASE_STATUS_OK;
}

/**
  * @brief Resume coprocessor.
  * @param coreId Value of @ref ChipCoreId
  * @retval status @ref BASE_StatusType.
  */
BASE_StatusType CPM_Resume(ChipCoreId coreId)
{
    SRM_ChannelId wfiChId = DCL_SRM_GetWfiChannel(SRM);  /* get wfi srm channel */
    DCL_SRM_EnableCoreChannelInt(SRM, coreId, wfiChId);   /* enable interrupt by chId */
    DCL_SRM_LockChannel(SRM, wfiChId); /* lock wfi channel */
    if (DCL_SRM_GetChannelCoreId(SRM, wfiChId) != GetChipCoreId()) { /* check wfi channel whether locked */
        return BASE_STATUS_ERROR;
    }
    DCL_SRM_UnLockChannel(SRM, wfiChId);  /* unlock wfi srm channel */
    return BASE_STATUS_OK;
}
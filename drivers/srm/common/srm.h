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
  * @file      srm.h
  * @author    MCU Driver Team
  * @brief     SRM module driver
  * @details   The header file contains the following declaration:
  *             + SRM handle structure definition.
  *             + Initialization functions.
  *             + SRM Set And Get Functions.
  *             + Interrupt Handler Functions.
  */

#ifndef McuMagicTag_SRM_H
#define McuMagicTag_SRM_H

/* Includes ------------------------------------------------------------------*/
#include "srm_ip.h"
/**
  * @defgroup SRM SRM
  * @brief SRM module.
  * @{
  */

/**
  * @defgroup SRM_Common SRM Common
  * @brief SRM common external module.
  * @{
  */

/**
  * @defgroup SRM_Handle_Definition SRM Handle Definition
  * @{
  */

/* Typedef definitions -------------------------------------------------------*/
typedef void (* SRM_CallbackFunc)(void *handle);

/**
  * @brief SRM channel attribute structure definition.
  */
typedef struct {
    bool                  intEnable;   /**< SRM channel interrupt enable */
} SRM_ChannelAttr;

/**
  * @brief SRM handle structure definition.
  */
typedef struct _SRM_Handle {
    SRM_RegStruct       *baseAddress;     /**< SRM base address. */
    SRM_UserCallBack     userCallBack;    /**< User callback */
    SRM_ExtendHandle     handleEx;        /**< SRM extend parameter */
} SRM_Handle;

/**
  * @}
  */
 
/**
  * @defgroup SRM_API_Declaration SRM HAL API
  * @{
  */
BASE_StatusType HAL_SRM_Init(SRM_Handle *handle, SRM_ChannelId srmChId, SRM_ChannelAttr attr);
BASE_StatusType HAL_SRM_DeInit(SRM_Handle *handle, SRM_ChannelId srmChId);
bool HAL_SRM_CheckLockStatus(SRM_Handle *handle, SRM_ChannelId srmChId);
BASE_StatusType HAL_SRM_Lock(SRM_Handle *handle, SRM_ChannelId srmChId, unsigned int lockTimeOutUs);
BASE_StatusType HAL_SRM_TryLock(SRM_Handle *handle, SRM_ChannelId srmChId);
BASE_StatusType HAL_SRM_Unlock(SRM_Handle *handle, SRM_ChannelId srmChId);
BASE_StatusType HAL_SRM_ConfigWfiChannel(SRM_Handle *handle, SRM_ChannelId srmChId, unsigned char timeCycle);
void HAL_SRM_RegisterCallback(SRM_Handle *handle, SRM_ChannelId srmChId, SRM_CallbackFunc callBackFunc);
void HAL_SRM_IrqHandler(void *handle);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* McuMagicTag_SRM_H */
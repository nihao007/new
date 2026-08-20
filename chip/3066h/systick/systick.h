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
  * @file    systick.h
  * @author  MCU Driver Team.
  * @brief   SYSTICK module driver.
  *          This file provides functions declaration of SYSTICK.
  *           + SYSTICK's initialization and de-initialization functions.
  *           + SYSTICK's register callback function.
  *           + Config the register of systick.
  */

#ifndef __McuMagicTag_SYSTICK_H__
#define __McuMagicTag_SYSTICK_H__
#include "systick_ip.h"


#define SYSTICK_MAX_VALUE 0xFFFFFFFFUL

/**
  * @defgroup SYSTICK SYSTICK
  * @brief SYSTICK module.
  * @{
  */

/**
  * @defgroup SYSTICK_Common SYSTICK Common
  * @brief SYSTICK common external module.
  * @{
  */


/**
  * @defgroup SYSTICK_Common_Param SYSTICK Common Parameters
  * @{
  */

/**
  * @brief Definition of callback function type
  */
typedef void (* SYSTICK_CallbackType)(void *hanlde);

#ifdef NOS_TASK_SUPPORT
#ifndef CFG_SYSTICK_TICKINTERVAL_US
#define CFG_SYSTICK_TICKINTERVAL_US 100
#endif
unsigned int SYSTICK_GetTickInterval(void);
unsigned int SYSTICK_GetTimeStampUs(void);
#endif

/**
  * @brief SYSTICK Handle
  */
typedef struct {
    SYSTICK_RegStruct            *baseAddress;
    SYSTICK_DebugCountMode        debugCountMode;
    unsigned short                div;
    SYSTICK_UserCallBack          userCallBack;
    SYSTICK_ExtendHandle          handleEx;
} SYSTICK_Handle;

/**
  * @}
  */

/**
  * @defgroup SYSTICK_API_Declaration SYSTICK HAL API
  * @{
  */
BASE_StatusType HAL_SYSTICK_Init(SYSTICK_Handle *handle);
BASE_StatusType HAL_SYSTICK_DeInit(SYSTICK_Handle *handle);
BASE_StatusType HAL_SYSTICK_Start(SYSTICK_Handle *handle);
BASE_StatusType HAL_SYSTICK_Stop(SYSTICK_Handle *handle);
unsigned int HAL_SYSTICK_GetTick(SYSTICK_Handle *handle);
BASE_StatusType HAL_SYSTICK_Config(SYSTICK_Handle *handle, SYSTICK_DebugCountMode mode, unsigned int div);
BASE_StatusType HAL_SYSTICK_SetTimerPeriod(SYSTICK_Handle *handle, unsigned int periodCnt);
BASE_StatusType HAL_SYSTICK_RegisterCallBack(SYSTICK_Handle *handle, SYSTICK_CallbackType pcallback);
void HAL_SYSTICK_IrqHandler(void *handle);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif
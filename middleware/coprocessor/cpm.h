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
  * @brief     RCM module driver
  * @details   The header file contains the following declaration:
  *             + RCM handle structure definition.
  *             + Initialization functions.
  *             + RCM Set And Get Functions.
  *             + Interrupt Handler Functions.
  */

#ifndef McuMagicTag_CPM_H
#define McuMagicTag_CPM_H

/* Includes ------------------------------------------------------------------*/
#include "baseinc.h"
#include "srm.h"
/**
  * @defgroup CPM Coprocessor
  * @brief CPM module.
  * @{
  */

/**
  * @defgroup CPM_Common CPM Common
  * @brief CPM common external module.
  * @{
  */

/**
  * @defgroup CPM_Handle CPM Handle Definition
  * @{
  */

/* Typedef definitions -------------------------------------------------------*/

#ifdef CPM_PARAM_CHECK
    #define CPM_ASSERT_PARAM         BASE_FUNC_ASSERT_PARAM
    #define CPM_PARAM_CHECK_NO_RET   BASE_FUNC_PARAMCHECK_NO_RET
    #define CPM_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
    #define CPM_ASSERT_PARAM(para)                ((void)0U)
    #define CPM_PARAM_CHECK_NO_RET(para)          ((void)0U)
    #define CPM_PARAM_CHECK_WITH_RET(param, ret)  ((void)0U)
#endif

/**
  * @brief CPM Image type structure definition.
  */
typedef enum {
    IMAGE_TYPE_NORMAL,
    IMAGE_TYPE_COMPRESS,
    IMAGE_TYPE_DECOMPRESS,
    IMAGE_TYPE_MAX
} CPM_ImageType;

/**
  * @brief CPM Image attribute structure definition.
  */
typedef struct __CPM_ImageAttr {
    unsigned int             loadAddr;
    unsigned int             startAddr;
    unsigned int             imageSize;
    CPM_ImageType            imageType;
} CPM_ImageAttr;

/**
  * @}
  */
 
/**
  * @defgroup CPM_API_Declaration CPM HAL API
  * @{
  */
BASE_StatusType CPM_SetWfiParam(SRM_ChannelId coreWfiChId, unsigned char offPeakTimeCycle);
BASE_StatusType CPM_SetStartAddr(ChipCoreId coreId, unsigned int startAddr);
BASE_StatusType CPM_LoadImage(CPM_ImageAttr imageAttr);
BASE_StatusType CPM_Start(ChipCoreId coreId);
BASE_StatusType CPM_Stop(ChipCoreId coreId);
BASE_StatusType CPM_Suspend(void);
BASE_StatusType CPM_Resume(ChipCoreId coreId);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* McuMagicTag_CPM_H */
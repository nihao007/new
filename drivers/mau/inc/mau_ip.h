/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2025. All rights reserved.
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
 * @file    mau_ip.h
 * @author  MCU Driver Team
 * @brief   Programmable Gain Apmlifier HAL level module driver head file.
 *          This file provides firmware functions to manage the following
 *          functionalities of the Amplifier.
 *           + Initialization and de-initialization functions
 *           + Programmable Gain Amplifier set gain value functions
 */
#ifndef McuMagicTag_MAU_IP_H
#define McuMagicTag_MAU_IP_H

#include "baseinc.h"

#ifdef MAU_PARAM_CHECK
#define MAU_ASSERT_PARAM BASE_FUNC_ASSERT_PARAM
#define MAU_PARAM_CHECK_NO_RET BASE_FUNC_PARAMCHECK_NO_RET
#define MAU_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
#define MAU_ASSERT_PARAM(para) ((void)0U)
#define MAU_PARAM_CHECK_NO_RET(para) ((void)0U)
#define MAU_PARAM_CHECK_WITH_RET(para, ret) ((void)0U)
#endif


/**
  * @addtogroup MAU
  * @{
  */

/**
  * @defgroup MAU_IP MAU_IP
  * @brief MAU_IP: mau_v0.
  * @{
  */

/**
  * @defgroup MAU_ENUM_Definition MAU Enum define.
  * @brief MAU Register Structure Definition.
  * @{
  */
/**
  * @brief The MAU function parmeter enum
  */
typedef enum {
    COM_MAU_FUNC_RCP_X          = 0,
    COM_MAU_FUNC_RSQ_X          = 1,
    COM_MAU_FUNC_EXP2_X         = 2,
    COM_MAU_FUNC_LOG2_X         = 3,
    COM_MAU_FUNC_SQRT_X         = 4,
    COM_MAU_FUNC_SIN_PI_2_X     = 5,
    COM_MAU_FUNC_COS_PI_2_X     = 6,
    COM_MAU_FUNC_ATAN_X_2_PI    = 7,
    COM_MAU_FUNC_LN_X           = 8,
    COM_MAU_FUNC_MAX,
} COM_MAU_FuncTypeEnum;

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
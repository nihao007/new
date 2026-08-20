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
  * @file    dac_ex.h
  * @author  MCU Driver Team
  * @brief   DAC module driver
  * @details This file provides DCL functions to manage DAC and Definition of
  *          specific parameters.
  *           + DAC sine wave mode.
  */
#ifndef McuMagicTag_DAC_EX_H
#define McuMagicTag_DAC_EX_H

#include "dac.h"
/**
 * @addtogroup DAC_IP
 * @{
 */

/**
 * @brief Vset type Enumeration Definition
 */
typedef enum {
    TYPE_VREF_SRC = 1U,
    TYPE_VSET_SRC,
    TYPE_VSET_LOAD_MODE,
    TYPE_VSET_SYNCP_SRC,
    TYPE_VSET_VALUE,
    TYPE_BASIC_MAX
} DAC_BasicType;

/**
 * @brief RAMP type Enumeration Definition
 */
typedef enum {
    TYPE_RAMP_LOAD_MODE = 1U,
    TYPE_RAMP_XTRIG,
    TYPE_RAMP_DIR,
    TYPE_RAMP_STEP,
    TYPE_RAMP_CLK_DIV,
    TYPE_RAMP_REF_VAL,
    TYPE_RAMP_DELAY_VAL,
    TYPE_RAMP_DEBUG_MODE,
    TYPE_RAMP_RT_REF_VAL,
    TYPE_RAMP_RT_DELAY_VAL,
    TYPE_RAMP_MAX
} DAC_RampType;

/**
 * @defgroup DAC_EX_API_Declaration DAC HAL API EX
 * @{
 */

 /* DAC extend function. */
BASE_StatusType HAL_DAC_InitVsetAttrEx(DAC_Handle *dacHandle, DAC_VsetAttr vsetAttr);
BASE_StatusType HAL_DAC_InitRampAttrEx(DAC_Handle *dacHandle, DAC_RampAttr rampAttr);
BASE_StatusType HAL_DAC_SetBasicConfigEx(DAC_Handle *dacHandle, DAC_BasicType basicType, unsigned int value);
BASE_StatusType HAL_DAC_SetRampConfigEx(DAC_Handle *dacHandle, DAC_RampType rampType, unsigned int value);
unsigned int HAL_DAC_GetBasicConfigEx(DAC_Handle *dacHandle, DAC_BasicType basicType);
unsigned int HAL_DAC_GetRampConfigEx(DAC_Handle *dacHandle, DAC_RampType rampType);
 /**
  * @}
  */

/**
  * @}
  */
#endif
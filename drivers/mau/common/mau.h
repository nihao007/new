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
 * @file    mau.h
 * @author  MCU Driver Team
 * @brief   Programmable Gain Apmlifier HAL level module driver head file.
 *          This file provides firmware functions to manage the following
 *          functionalities of the Amplifier.
 *           + Initialization and de-initialization functions
 *           + Programmable Gain Amplifier set gain value functions
 */
#ifndef McuMagicTag_MAU_H
#define McuMagicTag_MAU_H

#include "mau_ip.h"

/**
  * @defgroup MAU MAU
  * @brief MAU module.
  * @{
  */

/**
  * @defgroup MAU_Common MAU Common
  * @brief MAU common external module.
  * @{
  */


/**
  * @defgroup MAU_API_Declaration MAU HAL API
  * @{
  */
/**
  * @brief The MAU function
  */
float HAL_MAU_Rcp(float val);       /* y=1/x calculation. */
float HAL_MAU_Rsq(float val);       /* y=1/√x calculation. */
float HAL_MAU_Exp2(float val);      /* y=2^x calculation. */
float HAL_MAU_Log2(float val);      /* y=Log2 calculation. */
float HAL_MAU_Ln(float val);        /* y=Ln calculation. */
float HAL_MAU_Sin(float angle);     /* y=sin(x*pi/2). calculation. */
float HAL_MAU_Cos(float angle);     /* y=cos(x*pi/2). calculation. */
float HAL_MAU_Sqrt(float val);      /* y=√x calculation. */
float HAL_MAU_ATan(float val);      /* y=arctan(x). calculation. */
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
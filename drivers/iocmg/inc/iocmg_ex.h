/**
  * @copyright Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2025. All rights reserved.
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
 * @file    iocmg_ex.h
 * @author  MCU Driver Team
 * @brief   IOCMG module driver
 * @details Provides extension functions about iocmg reg init and config.
 */

/* Macro definitions */
#ifndef McuMagicTag_IOCMG_EX_H
#define McuMagicTag_IOCMG_EX_H

/* Includes ------------------------------------------------------------------*/
#include "iocmg.h"
/* Macro definitions ---------------------------------------------------------*/

/**
  * @addtogroup IOCMG_IP
  * @{
  */

/**
  * @defgroup IOCMG_EX_API_Declaration IOCMG HAL API EX
  * @{
  */

IOCMG_Status HAL_IOCMG_SetPinOpenDrainModeEx(unsigned int pinTypedef, IOCMG_OpenDrainMode openDrainMode);

IOCMG_OpenDrainMode HAL_IOCMG_GetPinOpenDrainModeEx(unsigned int pinTypedef);

#endif
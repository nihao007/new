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
  * @file      iocmg_ex.c
  * @author    MCU Driver Team
  * @brief     IOCMG module driver
  * @details   Provides extension functions about iocmg reg init and config.
  */

/* Includes ------------------------------------------------------------------*/
#include "iocmg_ex.h"

/* Macro definitions ---------------------------------------------------------*/

/**
  * @brief Set Pin Open Drain Mode
  * @param pinTypedef the pin type defined in iomap.h
  * @param openDrainMode function define as @ref IOCMG_OpenDrainMode
  * @retval IOCMG_Status @ref IOCMG_Status.
  */
IOCMG_Status HAL_IOCMG_SetPinOpenDrainModeEx(unsigned int pinTypedef, IOCMG_OpenDrainMode openDrainMode)
{
    IOCMG_PARAM_CHECK_WITH_RET(openDrainMode >= OPEN_DRAIN_ENABLE && openDrainMode <= OPEN_DRAIN_DISABLE, \
                               IOCMG_PARAM_ERROR);
    /* get iocmg reg address */
    IOCMG_REG* iocmgRegx = IOCMG_GetRegAddr(pinTypedef);
    DCL_IOCMG_SetOpenDrainMode(iocmgRegx, openDrainMode);
    return IOCMG_STATUS_OK;
}

/**
  * @brief Get Pin Open Drain Mode
  * @param pinTypedef the pin type defined in iomap.h
  * @retval IOCMG_OpenDrainMode @ref IOCMG_OpenDrainMode.
  */
IOCMG_OpenDrainMode HAL_IOCMG_GetPinOpenDrainModeEx(unsigned int pinTypedef)
{
    /* get iocmg reg address */
    IOCMG_REG* iocmgRegx = IOCMG_GetRegAddr(pinTypedef);
    return DCL_IOCMG_GetOpenDrainMode(iocmgRegx);
}

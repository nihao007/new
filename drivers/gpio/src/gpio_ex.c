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
  * @file      gpio_ex.c
  * @author    MCU Driver Team
  * @brief     GPIO module driver
  * @details   This file provides firmware functions to manage the following functionalities of the GPIO.
  *             + GPIO configuration definetion.
  *             + GPIO Set And Get Functions.
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio_ex.h"

/**
  * @brief Setting GPIO report interrupt to CPU.
  * @param handle Value of @ref GPIO_Handle.
  * @param pins OR logical combination of pin.
  * @param cpu The num of CPU to report the interrupt.
  * @retval Value of @ref BASE_StatusType.
  */
BASE_StatusType HAL_GPIO_SetPinsIrqCpuIDEx(GPIO_Handle *handle, unsigned int pins, ChipCoreId cpuId)
{
    GPIO_ASSERT_PARAM(handle != NULL);
    GPIO_ASSERT_PARAM(IsGPIOInstance(handle->baseAddress));
    GPIO_PARAM_CHECK_WITH_RET(IsGpioPins(pins), BASE_STATUS_ERROR);
    GPIO_PARAM_CHECK_WITH_RET(IsChipCoreId(cpuId), BASE_STATUS_ERROR);
    GPIO_PARAM_CHECK_WITH_RET(IsGpioPinsFirstSetCpuInt(handle->baseAddress, pins), BASE_STATUS_ERROR);
    if (cpuId == CHIP_CORE_0) {
        handle->baseAddress->GPIO_CPU0_INT_EN.reg |= pins; /* config the pin interrupt to CPU0 */
    } else if (cpuId == CHIP_CORE_1) {
        handle->baseAddress->GPIO_CPU1_INT_EN.reg |= pins; /* config the pin interrupt to CPU1 */
    } else if (cpuId == CHIP_CORE_2) {
        handle->baseAddress->GPIO_CPU2_INT_EN.reg |= pins; /* config the pin interrupt to CPU2 */
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Getting GPIO report interrupt to CPU num.
  * @param handle Value of @ref GPIO_Handle.
  * @param pins OR logical combination of pin.
  * @retval Value of CPU number.
  */
ChipCoreId HAL_GPIO_GetPinIrqCpuIDEx(GPIO_Handle *handle, GPIO_PIN pin)
{
    GPIO_ASSERT_PARAM(handle != NULL);
    GPIO_ASSERT_PARAM(IsGPIOInstance(handle->baseAddress));
    GPIO_ASSERT_PARAM(IsGpioPin(pin));
    ChipCoreId cpuId = 0;
    if (handle->baseAddress->GPIO_CPU0_INT_EN.reg & pin) { /* Check the pin whether is configged to CPU0 */
        cpuId = CHIP_CORE_0;
    } else if (handle->baseAddress->GPIO_CPU1_INT_EN.reg & pin) { /* Check the pin whether is configged to CPU1 */
        cpuId = CHIP_CORE_1;
    } else if (handle->baseAddress->GPIO_CPU2_INT_EN.reg & pin) { /* Check the pin whether is configged to CPU2 */
        cpuId = CHIP_CORE_2;
    }
    return cpuId;
}
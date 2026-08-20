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
 * @file    dac.c
 * @author  MCU Driver Team.
 * @brief   DAC HAL level module driver.
 *          This file provides firmware functions to manage the following
 *          functionalities of the DAC and Comparator.
 *           + DAC's Initialization and de-initialization functions
 *           + Set DAC value function
 */
#include "dac.h"

/**
  * @brief DAC HAL Init
  * @param dacHandle: DAC handle.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_DAC_Init(DAC_Handle *dacHandle)
{
    /* Repeat config stable time */
    BASE_FUNC_DELAY_US(4); /* delay 4us */
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    DAC_PARAM_CHECK_WITH_RET(IsDacConfigureValue(dacHandle->dacValue), BASE_STATUS_ERROR);
    /* Set DAC Vref source. */
    DCL_DAC_SetVrefSrc(dacHandle->baseAddress, dacHandle->handleEx.vrefSelect);
    /* Dac pin output mode config */
    DCL_DAC_SetPinOutputConfig(dacHandle->baseAddress, dacHandle->handleEx.pinOutputEn);
    /* Turn on the DAC. */
    DCL_DAC_Enable(dacHandle->baseAddress);
    /* Wait output stable */
    BASE_FUNC_DELAY_US(60);  /* delay 60us */
    return BASE_STATUS_OK;
}

/**
  * @brief DAC HAL DeInit
  * @param dacHandle: DAC handle.
  * @retval BASE_StatusType: OK
  */
BASE_StatusType HAL_DAC_DeInit(DAC_Handle *dacHandle)
{
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    DCL_DAC_Disable(dacHandle->baseAddress);   /* Disable DAC, clears the count value. */
    DCL_DAC_SetValue(dacHandle->baseAddress, 0);  /* Clear DAC value. */
    DCL_DAC_SetPinOutputConfig(dacHandle->baseAddress, BASE_CFG_DISABLE);  /* disable dac pin output */
    return BASE_STATUS_OK;
}

/**
  * @brief Set DAC value
  * @param dacHandle: DAC handle.
  * @param value: DAC value.
  * @retval None.
  */
void HAL_DAC_SetValue(DAC_Handle *dacHandle, unsigned int value)
{
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    DAC_PARAM_CHECK_NO_RET(value <= DAC_MAX_OUT_VALUE);
    /* Change the conversion value of the DAC. */
    if (DCL_DAC_GetVsetSrc(dacHandle->baseAddress) == VSET_SRC_FROM_REG) {
        DCL_DAC_SetValue(dacHandle->baseAddress, value);
    }
}
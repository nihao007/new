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
  * @file    systick.c
  * @author  MCU Driver Team.
  * @brief   SYSTICK HAL level module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the SYSTICK and Comparator.
  *           + SYSTICK's initialization and de-initialization functions.
  */

#include "systick.h"

/**
  * @brief Get systick high 64bit value.
  * @param handle SYSTICK handle.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
  */
static unsigned int GetMtimerH(SYSTICK_Handle *handle)
{
    unsigned int high = 0;
    unsigned int low = 0;
    unsigned int high2 = 1;
    while (high != high2) { /* Read right condition. */
        high = handle->baseAddress->MTIME_H;
        low = handle->baseAddress->MTIME;
        high2 = handle->baseAddress->MTIME_H;
    }
    BASE_FUNC_UNUSED(low);
    return high2; /* return the high32 bit value. */
}

/**
  * @brief Init the SYSTICK module.
  * @param handle SYSTICK handle.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
  */
BASE_StatusType HAL_SYSTICK_Init(SYSTICK_Handle *handle)
{
    SYSTICK_ASSERT_PARAM(handle != NULL);
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(handle->baseAddress));
    SYSTICK_PARAM_CHECK_WITH_RET(IsSysytickCountDiv(handle->div), BASE_STATUS_ERROR);
    SYSTICK_PARAM_CHECK_WITH_RET(IsDebugCountMode(handle->debugCountMode), BASE_STATUS_ERROR);
    /* Configuring Allocation and Counting Modes. */
    DCL_SYSTICK_SetDebugCountMode(handle->baseAddress, handle->debugCountMode);
    DCL_SYSTICK_SetSystickCountDiv(handle->baseAddress, handle->div);

    DCL_SYSTICK_SetTick(0x00000000U);
    DCL_SYSTICK_SetTick32bitH(handle->baseAddress, 0x00000000U);
    return BASE_STATUS_OK;
}

/**
  * @brief Deinitialize the SYSTICK module.
  * @param handle SYSTICK handle.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
  */
BASE_StatusType HAL_SYSTICK_DeInit(SYSTICK_Handle *handle)
{
    SYSTICK_ASSERT_PARAM(handle != NULL);
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(handle->baseAddress));
    SYSTICK_PARAM_CHECK_WITH_RET(IsSysytickCountDiv(handle->div), BASE_STATUS_ERROR);
    SYSTICK_PARAM_CHECK_WITH_RET(IsDebugCountMode(handle->debugCountMode), BASE_STATUS_ERROR);
    
    handle->userCallBack.PeriodCallBack = NULL;  /* Clear all user call back function. */
    DCL_SYSTICK_SetTick(0);
    DCL_SYSTICK_SetTick32bitH(handle->baseAddress, 0);
    DCL_SYSTICK_ClearSystickInterrupt(handle->baseAddress);
    DCL_SYSTICK_SetDebugCountMode(handle->baseAddress, SYSTICK_DEBUG_CONTINUE_COUNT);
    DCL_SYSTICK_SetSystickCountDiv(handle->baseAddress, SYSTICK_COUNT_CLK_DIV_MIN);
    return BASE_STATUS_OK;
}

/**
  * @brief Start the systick count.
  * @param handle SYSTICK handle.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
  */
BASE_StatusType HAL_SYSTICK_Start(SYSTICK_Handle *handle)
{
    SYSTICK_ASSERT_PARAM(handle != NULL);
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(handle->baseAddress));
    DCL_SYSTICK_EnableSystickCount(handle->baseAddress);
    return BASE_STATUS_OK;
}

/**
  * @brief Stop the systick count.
  * @param handle SYSTICK handle.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
  */
BASE_StatusType HAL_SYSTICK_Stop(SYSTICK_Handle *handle)
{
    SYSTICK_ASSERT_PARAM(handle != NULL);
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(handle->baseAddress));
    DCL_SYSTICK_DisableSystickCount(handle->baseAddress);
    return BASE_STATUS_OK;
}

/**
  * @brief Get the current systick count.
  * @param handle SYSTICK handle.
  * @retval the value of systick count.
  */
unsigned int HAL_SYSTICK_GetTick(SYSTICK_Handle *handle)
{
    SYSTICK_ASSERT_PARAM(handle != NULL);
    BASE_FUNC_UNUSED(handle);
    return DCL_SYSTICK_GetTick();
}

/**
  * @brief SYSTICK parameter configuration.
  * @param handle SYSTICK handle.
  * @param mode The count mode during debugging.
  * @param div Timer frequency division value. The count register is incremented by 1 every div+1 timing clock cycle.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
  */
BASE_StatusType HAL_SYSTICK_Config(SYSTICK_Handle *handle, SYSTICK_DebugCountMode mode, unsigned int div)
{
    SYSTICK_ASSERT_PARAM(handle != NULL);
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(handle->baseAddress));
    SYSTICK_PARAM_CHECK_WITH_RET(IsSysytickCountDiv(div), BASE_STATUS_ERROR);
    SYSTICK_PARAM_CHECK_WITH_RET(IsDebugCountMode(mode), BASE_STATUS_ERROR);

    DCL_SYSTICK_DisableSystickCount(handle->baseAddress);
    /* Configuring Allocation and Counting Modes. */
    DCL_SYSTICK_SetDebugCountMode(handle->baseAddress, mode);
    DCL_SYSTICK_SetSystickCountDiv(handle->baseAddress, div);
    /* Enable systick count. */
    DCL_SYSTICK_EnableSystickCount(handle->baseAddress);
    return BASE_STATUS_OK;
}

/**
  * @brief Callback Function Registration.
  * @param handle SYSTICK handle.
  * @param pcallback Pointer to the address of the registered callback function.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
  */
BASE_StatusType HAL_SYSTICK_RegisterCallBack(SYSTICK_Handle *handle, SYSTICK_CallbackType pcallback)
{
    SYSTICK_ASSERT_PARAM(handle != NULL);
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(handle->baseAddress));
    SYSTICK_ASSERT_PARAM(pcallback != NULL);
    /* Callback Function Registration. */
    handle->userCallBack.PeriodCallBack = pcallback;
    return BASE_STATUS_OK;
}

/**
  * @brief Config timer period value.
  * @param handle SYSTICK handle.
  * @param periodCnt the value of period, unit: systick clock.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
  */
BASE_StatusType HAL_SYSTICK_SetTimerPeriod(SYSTICK_Handle *handle, unsigned int periodCnt)
{
    SYSTICK_ASSERT_PARAM(handle != NULL);
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(handle->baseAddress));
    handle->handleEx.periodCount = periodCnt; /* Record Period Value. */

    /* Configure the 64-bit period value. */
    DCL_SYSTICK_SetSystickCompare32bitH(handle->baseAddress, 0xFFFFFFFF);
    DCL_SYSTICK_SetSystickCompare32bitL(handle->baseAddress, periodCnt);
    unsigned int highBit = GetMtimerH(handle);
    DCL_SYSTICK_SetSystickCompare32bitH(handle->baseAddress, highBit);
    return BASE_STATUS_OK;
}

/**
  * @brief Interrupt Handling Function.
  * @param handle Handle pointers.
  * @retval None.
  */
void HAL_SYSTICK_IrqHandler(void *handle)
{
    SYSTICK_Handle *systickHandle = (SYSTICK_Handle *)handle;
    SYSTICK_ASSERT_PARAM(systickHandle != NULL);
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickHandle->baseAddress));

    unsigned int mtimeCmpLow32bit = DCL_SYSTICK_GetSystickCompare32bitL(systickHandle->baseAddress);
    unsigned int mtimeCmpHigh32bit = DCL_SYSTICK_GetSystickCompare32bitH(systickHandle->baseAddress);
    unsigned long long newMtimeCmpLow32Bit = (unsigned long long)mtimeCmpLow32bit +
                                             (unsigned long long)systickHandle->handleEx.periodCount;

    volatile unsigned int highBit = GetMtimerH(handle);

    if (newMtimeCmpLow32Bit >= 0xFFFFFFFF) {
        unsigned int setValueLow32bit = (newMtimeCmpLow32Bit % 0xFFFFFFFF);
        if (mtimeCmpHigh32bit >= 0xFFFFFFFF) { /* Reset high 32 bit systick count. */
            DCL_SYSTICK_DisableSystickCount(systickHandle->baseAddress);
            DCL_SYSTICK_SetTick32bitH(systickHandle->baseAddress, 0x00000000U); /* Reset high 32 bit. */
            DCL_SYSTICK_EnableSystickCount(systickHandle->baseAddress);
            /* Configure the 64-bit period value. */
            DCL_SYSTICK_SetSystickCompare32bitH(systickHandle->baseAddress, 0xFFFFFFFF);
            DCL_SYSTICK_SetSystickCompare32bitL(systickHandle->baseAddress, setValueLow32bit);
            /* Keep the count continuous to prevent timing errors. */
            DCL_SYSTICK_SetSystickCompare32bitH(systickHandle->baseAddress, 0x01);
        } else {
            /* Configure the 64-bit period value. */
            DCL_SYSTICK_SetSystickCompare32bitH(systickHandle->baseAddress, 0xFFFFFFFF);
            DCL_SYSTICK_SetSystickCompare32bitL(systickHandle->baseAddress, setValueLow32bit);
            DCL_SYSTICK_SetSystickCompare32bitH(systickHandle->baseAddress, (highBit + 0x01));  /* Config low 32bit. */
        }
    } else {
        /* Configure the 64-bit period value. */
        DCL_SYSTICK_SetSystickCompare32bitH(systickHandle->baseAddress, 0xFFFFFFFF);
        DCL_SYSTICK_SetSystickCompare32bitL(systickHandle->baseAddress, (newMtimeCmpLow32Bit & 0xFFFFFFFF));
        DCL_SYSTICK_SetSystickCompare32bitH(systickHandle->baseAddress, highBit);  /* Config low 32bit. */
    }
    /* Invoke the user registration callback function. */
    if (systickHandle->userCallBack.PeriodCallBack != NULL) {
        systickHandle->userCallBack.PeriodCallBack(handle);
    }
}
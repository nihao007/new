/**
  * @copyright Copyright (c) 2024, HiSilicon (Shanghai) Technologies Co., Ltd. All rights reserved.
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
  * @file      systickinit.c
  * @author    MCU Driver Team
  * @brief     systick init modlue.
  * @details   systick initialization function during startup
  */
#include "baseaddr.h"
#include "crg.h"
#include "timer.h"
#include "systick.h"
#include "systickinit.h"
#include "feature.h"
#ifdef NOS_TASK_SUPPORT
#include "interrupt.h"
#include "nosinit.h"
#endif

SYSTICK_Handle g_systickHandle;
#ifdef NOS_TASK_SUPPORT
#define NOS_TickPostDispatch OsHwiDispatchTick

static void SYSTICK_Default_Callback(void *handle)
{
    /* The default systick callback when using th nos task */
    BASE_FUNC_UNUSED(handle);
    NOS_TickPostDispatch();
}

void SYSTICK_IRQ_Enable(void)
{   
    unsigned int periodCnt = (HAL_CRG_GetIpFreq(SYSTICK_BASE) / CRG_FREQ_1MHz) * CFG_SYSTICK_TICKINTERVAL_US;
    HAL_SYSTICK_SetTimerPeriod(&g_systickHandle, periodCnt);
    HAL_SYSTICK_RegisterCallBack(&g_systickHandle, SYSTICK_Default_Callback);
    IRQ_SetPriority(IRQ_MTIMER, 1);  /* interrupt priority 1 */
    IRQ_Register(IRQ_MTIMER, HAL_SYSTICK_IrqHandler, &g_systickHandle);
    IRQ_EnableN(IRQ_MTIMER); /* Interrupt number shuold match cpu. */
}

unsigned int SYSTICK_GetTickInterval(void)
{
    /* Get the tick interval(the number of usecond per tick) */
    return CFG_SYSTICK_TICKINTERVAL_US;
}

unsigned int SYSTICK_GetTimeStampUs(void)
{
    /* Get the systick timestamp(convert from the systick value) */
    return DCL_SYSTICK_GetTick() / (SYSTICK_GetCRGHZ() / CRG_FREQ_1MHz);
}

#endif

/**
  * @brief   Init the systick
  * @param   None
  * @retval  None
  */
void SYSTICK_Init(void)
{
    /* Get the baseaddress of the current chip core. */
    SYSTICK_RegStruct *systickx = DCL_SYSTICK_GetChipCoreSystickAddr();
    g_systickHandle.baseAddress = systickx;
    g_systickHandle.div = SYSTICK_COUNT_CLK_DIV_MIN;
    g_systickHandle.debugCountMode = SYSTICK_DEBUG_STOP_COUNT;
    HAL_SYSTICK_Stop(&g_systickHandle);
    HAL_SYSTICK_DeInit(&g_systickHandle);
    HAL_SYSTICK_Init(&g_systickHandle);
    HAL_SYSTICK_Start(&g_systickHandle); /* Enable systick count. */
}

/**
  * @brief   Get the Systick frep(Hz)
  * @param   None
  * @retval  Clock frep of systick(Hz)
  */
unsigned int SYSTICK_GetCRGHZ(void)
{
#ifdef NOS_TASK_SUPPORT
    return HAL_CRG_GetCoreClkFreq();
#else
    /* Get the Systick IP */
    SYSTICK_RegStruct *systickx = DCL_SYSTICK_GetChipCoreSystickAddr();
    return HAL_CRG_GetIpFreq((const void *)systickx);
#endif
}
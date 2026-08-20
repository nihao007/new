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
  * @file    systick_ip.h
  * @author  MCU Driver Team
  * @brief   Header file containing SYSTICK module DCL driver functions.
  *          This file provides functions to manage the following functionalities of SYSTICK module.
  *          + Definition of SYSTICK configuration parameters.
  *          + SYSTICK registers mapping structures.
  *          + Direct Configutration Layer driver functions.
  */
#ifndef McuMagicTag_SYSTICK_IP_H
#define McuMagicTag_SYSTICK_IP_H

#include "baseinc.h"

#ifdef SYSTICK_PARAM_CHECK
#define SYSTICK_ASSERT_PARAM BASE_FUNC_ASSERT_PARAM
#define SYSTICK_PARAM_CHECK_NO_RET BASE_FUNC_PARAMCHECK_NO_RET
#define SYSTICK_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
#define SYSTICK_ASSERT_PARAM(para) ((void)0U)
#define SYSTICK_PARAM_CHECK_NO_RET(para) ((void)0U)
#define SYSTICK_PARAM_CHECK_WITH_RET(para, ret) ((void)0U)
#endif

#define SYSTICK_COUNT_CLK_DIV_MAX   0x000003FF
#define SYSTICK_COUNT_CLK_DIV_MIN   0x00000000
/**
  * @addtogroup SYSTICK
  * @{
  */

/**
  * @defgroup SYSTICK_IP SYSTICK_IP
  * @brief SYSTICK_IP: systick_v0.
  * @{
  */

/**
 * @defgroup SYSTICK_Param_Def SYSTICK Parameters Definition
 * @brief Definition of SYSTICK configuration parameters
 * @{
 */

/**
  * @brief Enumeration definition of the count mode during debugging.
  */
typedef enum {
    SYSTICK_DEBUG_CONTINUE_COUNT = 0x00000000U,
    SYSTICK_DEBUG_STOP_COUNT     = 0x00000001U,
} SYSTICK_DebugCountMode;

/**
  * @brief Struct definition of SYSTICK_UserCallBack.
  */
typedef struct {
    void (* PeriodCallBack)(void *handle);
} SYSTICK_UserCallBack;

/**
  * @brief Struct definition of SYSTICK_ExtendHandle.
  */
typedef struct {
    unsigned int periodCount;
} SYSTICK_ExtendHandle;

/**
  * @}
  */

/**
  * @defgroup SYSTICK_REG_Definition SYSTICK Register Structure.
  * @brief SYSTICK Register Structure Definition.
  * @{
  */
/**
  * @brief SYSTICK control register.
  */
typedef union {
    unsigned int   reg;
    struct {
        unsigned int   enable      : 1;  /**< Mtimer enable. */
        unsigned int   clksrc      : 1;  /**< Mtimer clock source select. */
        unsigned int   stop_tmr_en : 1;  /**< Counting stop control in debugging mode. */
        unsigned int   reserved    : 29;
    } BIT;
} TIMER_CTRL_REG;

/**
  * @brief SYSTICK DIV register.
  */
typedef union {
    unsigned int   reg;
    struct {
        unsigned int   div      : 10;  /**< Timer frequency division control. */
        unsigned int   reserved : 22;
    } BIT;
} TIMER_DIV_REG;

/**
  * @brief SYSTICK registers definition structure.
  */
typedef struct {
    TIMER_CTRL_REG       TIMER_CTRL; /**< Mtimer control register. Offset address: 0x00000000U. */
    TIMER_DIV_REG        TIMER_DIV;  /**< Mtimer frequency divider register. Offset address: 0x00000004U. */
    unsigned int         MTIME;      /**< Mtimer count value lower 32-bit register. Offset address: 0x00000008U. */
    unsigned int         MTIME_H;    /**< Upper 32-bit register for Mtimer count value. Offset address: 0x0000000CU. */
    unsigned int         MTIMECMP;   /**< Mtimer comparison value lower 32-bit register. Offset address: 0x00000010U. */
    unsigned int         MTIMECMP_H; /**< Upper 32-bit Mtimer comparison value register. Offset address: 0x00000014U. */
} volatile SYSTICK_RegStruct;

/**
  * @brief Check debug count mode.
  * @param mode value of debug count mode.
  * @retval true
  * @retval false
  */
static inline bool IsDebugCountMode(SYSTICK_DebugCountMode mode)
{
    return (mode == SYSTICK_DEBUG_STOP_COUNT || mode == SYSTICK_DEBUG_CONTINUE_COUNT);
}

/**
  * @brief Check systick clk div.
  * @param mode value of systick clk div.
  * @retval true
  * @retval false
  */
static inline bool IsSysytickCountDiv(unsigned int div)
{
    return (div >= SYSTICK_COUNT_CLK_DIV_MIN || div <= SYSTICK_COUNT_CLK_DIV_MAX);
}

/**
  * @brief Enable systick start count.
  * @param systickx SYSTICK register base address.
  * @retval None.
  */
static inline void DCL_SYSTICK_EnableSystickCount(SYSTICK_RegStruct *systickx)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    systickx->TIMER_CTRL.BIT.enable = BASE_CFG_ENABLE;
}

/**
  * @brief Diasble systick start count.
  * @param systickx SYSTICK register base address.
  * @retval None.
  */
static inline void DCL_SYSTICK_DisableSystickCount(SYSTICK_RegStruct *systickx)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    systickx->TIMER_CTRL.BIT.enable = BASE_CFG_DISABLE;
}

/**
  * @brief Get the state of systick.
  * @param systickx SYSTICK register base address.
  * @retval the value of state, 0: disable, 1:enable.
  */
static inline unsigned int DCL_SYSTICK_GetSystickState(SYSTICK_RegStruct *systickx)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    return systickx->TIMER_CTRL.BIT.enable;
}

/**
  * @brief Set the mode of debug count.
  * @param systickx SYSTICK register base address.
  * @param mode value of debug count mode.
  * @retval None.
  */
static inline void DCL_SYSTICK_SetDebugCountMode(SYSTICK_RegStruct *systickx, SYSTICK_DebugCountMode mode)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    SYSTICK_ASSERT_PARAM(IsDebugCountMode(mode));
    systickx->TIMER_CTRL.BIT.stop_tmr_en = mode;
}

/**
  * @brief Get the mode of debug count.
  * @param systickx SYSTICK register base address.
  * @retval value of debug count mode.
  */
static inline SYSTICK_DebugCountMode DCL_SYSTICK_GetDebugCountMode(SYSTICK_RegStruct *systickx)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    return (SYSTICK_DebugCountMode)(systickx->TIMER_CTRL.BIT.stop_tmr_en);
}

/**
  * @brief Set the div of systick count clk.
  * @param systickx SYSTICK register base address.
  * @param div value of systick count clk.
  * @retval None.
  */
static inline void DCL_SYSTICK_SetSystickCountDiv(SYSTICK_RegStruct *systickx, unsigned int div)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    SYSTICK_ASSERT_PARAM(IsSysytickCountDiv(div));
    systickx->TIMER_DIV.BIT.div = div;
}

/**
  * @brief Get the div of systick count clk.
  * @param systickx SYSTICK register base address.
  * @retval value of systick count clk.
  */
static inline unsigned int DCL_SYSTICK_GetSystickCountDiv(SYSTICK_RegStruct *systickx)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    return systickx->TIMER_DIV.BIT.div;
}

/**
  * @brief Get the count of systick.
  * @param None.
  * @retval value of systick count.
  */
static inline unsigned int DCL_SYSTICK_GetTick(void)
{
    /* Get the baseaddress of the current chip core. */
    SYSTICK_RegStruct *systickx = SYSTICK;
    ChipCoreId chipId = GetChipCoreId();
    if (chipId == CHIP_CORE_1) {
        systickx = SYSTICK1;
    } else if (chipId == CHIP_CORE_2) {
        systickx = SYSTICK2;
    }

    return systickx->MTIME;
}

/**
  * @brief Set the count of systick.
  * @param None.
  * @retval value of systick count.
  */
static inline void DCL_SYSTICK_SetTick(unsigned int intValue)
{
    /* Get the baseaddress of the current chip core. */
    SYSTICK_RegStruct *systickx = SYSTICK;
    ChipCoreId chipId = GetChipCoreId();
    if (chipId == CHIP_CORE_1) {
        systickx = SYSTICK1;
    } else if (chipId == CHIP_CORE_2) {
        systickx = SYSTICK2;
    }

    systickx->MTIME = intValue;
}

/**
  * @brief Get the count of systick high 32bit.
  * @param systickx SYSTICK register base address.
  * @retval value of systick count high 32bit.
  */
static inline unsigned int DCL_SYSTICK_GetTick32bitH(SYSTICK_RegStruct *systickx)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    return systickx->MTIME_H;
}

/**
  * @brief Set the count of systick high 32bit.
  * @param systickx SYSTICK register base address.
  * @param intValue the tick high 32bit start value.
  * @retval None.
  */
static inline void DCL_SYSTICK_SetTick32bitH(SYSTICK_RegStruct *systickx, unsigned int intValue)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    systickx->MTIME_H = intValue;
}

/**
  * @brief Set the count of systick high 32bit.
  * @param systickx SYSTICK register base address.
  * @param value the value of systick high 32bit.
  * @retval None.
  */
static inline void DCL_SYSTICK_SetSystickCompare32bitH(SYSTICK_RegStruct *systickx, unsigned int value)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    systickx->MTIMECMP_H = value;
}

/**
  * @brief Get the count of systick high 32bit.
  * @param systickx SYSTICK register base address.
  * @retval the value of systick high 32bit.
  */
static inline unsigned int DCL_SYSTICK_GetSystickCompare32bitH(SYSTICK_RegStruct *systickx)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    return systickx->MTIMECMP_H;
}

/**
  * @brief Set the compare count of systick low 32bit.
  * @param systickx SYSTICK register base address.
  * @param value the compare value of systick low 32bit.
  * @retval None.
  */
static inline void DCL_SYSTICK_SetSystickCompare32bitL(SYSTICK_RegStruct *systickx, unsigned int value)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    systickx->MTIMECMP = value;
}

/**
  * @brief Get the compare count of systick low 32bit.
  * @param systickx SYSTICK register base address.
  * @retval the compare value of systick low 32bit.
  */
static inline unsigned int DCL_SYSTICK_GetSystickCompare32bitL(SYSTICK_RegStruct *systickx)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    return systickx->MTIMECMP;
}

/**
  * @brief Clear the interrupt of systick.
  * @param systickx SYSTICK register base address.
  * @retval None.
  */
static inline void DCL_SYSTICK_ClearSystickInterrupt(SYSTICK_RegStruct *systickx)
{
    SYSTICK_ASSERT_PARAM(IsSYSTICKInstance(systickx));
    unsigned int regMtimercmp32bitL = systickx->MTIMECMP;
    systickx->MTIMECMP = regMtimercmp32bitL;
}

/**
  * @brief Get the current chip core systick baseaddress.
  * @param None.
  * @retval the systick baseaddress of current chip core.
  */
static inline SYSTICK_RegStruct* DCL_SYSTICK_GetChipCoreSystickAddr(void)
{
    SYSTICK_RegStruct *systickx = SYSTICK;

    /* Get the baseaddress of the current chip core. */
    ChipCoreId chipId = GetChipCoreId();
    if (chipId == CHIP_CORE_1) {
        systickx = SYSTICK1;
    } else if (chipId == CHIP_CORE_2) {
        systickx = SYSTICK2;
    }

    return systickx;
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* McuMagicTag_SYSTICK_IP_H */
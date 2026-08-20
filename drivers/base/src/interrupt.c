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
  * @file    interrupt.c
  * @author  MCU Driver Team
  * @brief   Provides the handle template functions for processing exceptions and interrupts supported by the current
  *          functionalities of the interrupt.
  *           + Initialization and de-initialization functions
  *           + Regester and de-regester interrupt
  *           + Enable and disable interrupt
  *           + Configure interrupt
  */

/* Includes ------------------------------------------------------------------ */
#include "interrupt.h"
#include "baseinc.h"

/* Macro definitions ---------------------------------------------------------*/
#define IRQ_CLIC_BASE               (0x0C000000)
#define IRQ_CLIC_MEMORY_ADDR_OFFSET (0x1000)
#define IRQ_LEVEL_WIDTH             (4)

#define IRQ_PRI_MASK                (0xF0)
#define IRQ_PRI_OFFSET              (4)

/* Typedef definitions -------------------------------------------------------*/
static void IRQ_DummyHandler(void *arg);
static void IRQ_SetCallBack(unsigned int irqNum, IRQ_PROC_FUNC func, void *arg);

#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
static inline unsigned int IRQ_GetCpuPrivilege(void);

volatile unsigned int g_RiscvPrivMode = 0;
#endif

typedef union {
    unsigned int reg;
    struct {
        unsigned int clic_ip : 8;    /**< clic int pending. */
        unsigned int clic_ie : 8;    /**< clic int en. */
        unsigned int clic_attr : 8;  /**< clic int attr. */
        unsigned int clic_ctrl : 8;
    } BIT;
} volatile IRQ_CLIC_REG;

/**
  * @brief Interrupt vector table, supports up to IRQ_MAX interrupts, except for IRQ_VECTOR_CNT internal
  *        standard interrupts, which can be configured according to actual conditions.
  */
IRQ_ARG_FUNC g_irqCallbackFunc[IRQ_MAX];

/* Initialization and de-initialization functions ----------------------------*/
static inline unsigned int GetClicRegAddr(unsigned int irqNum)
{
    return (IRQ_CLIC_BASE + IRQ_CLIC_MEMORY_ADDR_OFFSET + IRQ_LEVEL_WIDTH * irqNum);
}

/**
  * @brief Exception/Interrupt Handler Entry.
  * @param irqNum external interrupt number.
  * @retval None
  */
void InterruptEntry(unsigned int irqNum)
{
    g_irqCallbackFunc[irqNum].pfnHandler(g_irqCallbackFunc[irqNum].param);
}

/**
  * @brief Irq initialization.
  * @param none.
  * @retval None
  */
void IRQ_Init(void)
{
    unsigned int index;

    for (index = 0; index < IRQ_MAX; index++) {
        g_irqCallbackFunc[index].pfnHandler = IRQ_DummyHandler;
        g_irqCallbackFunc[index].param = NULL;
    }
}

/* Register and Unregister interrupt -----------------------------------------*/
/**
  * @brief Register IRQ Callback function and parameter.
  * @param irqNum  External interrupt number.
  * @param func    Callback function.
  * @param arg     Parameter of callback function.
  * @retval BASE_STATUS_OK(success) or IRQ_ERRNO_ALREADY_CREATED(fail) or IRQ_ERRNO_NUM_INVALID.
  * @note    In the corresponding interrupt handler, manually clear the interrupt source and the corresponding interrupt
  *          flag bit (call the IRQ_ClearN function to clear the interrupt), otherwise the interrupt will always be
  *          triggered.
  */
unsigned int IRQ_Register(unsigned int irqNum, IRQ_PROC_FUNC func, void *arg)
{
    INTERRUPT_ASSERT_PARAM(func != NULL);
    INTERRUPT_PARAM_CHECK_WITH_RET(irqNum < IRQ_MAX, IRQ_ERRNO_NUM_INVALID);

    if (g_irqCallbackFunc[irqNum].pfnHandler != IRQ_DummyHandler) {
        return IRQ_ERRNO_ALREADY_CREATED;
    }
    IRQ_SetCallBack(irqNum, func, arg);
    return BASE_STATUS_OK;
}

/**
  * @brief Unregister IRQ Callback.
  * @param irqNum   External interrupt number.
  * @retval BASE_STATUS_OK or IRQ_ERRNO_NUM_INVALID.
  */
unsigned int IRQ_Unregister(unsigned int irqNum)
{
    INTERRUPT_PARAM_CHECK_WITH_RET(irqNum < IRQ_MAX, IRQ_ERRNO_NUM_INVALID);
    g_irqCallbackFunc[irqNum].pfnHandler = IRQ_DummyHandler;
    g_irqCallbackFunc[irqNum].param = NULL;
    return BASE_STATUS_OK;
}

/* Enable and disable interrupt ----------------------------------------------*/
/**
  * @brief  Global Interrupt Enable.
  * @retval None.
  */
void IRQ_Enable(void)
{
#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
    for (unsigned int index = IRQ_VECTOR_CNT; index < IRQ_MAX; index++) {
        IRQ_DisableN(index);
    }
#else
    SET_CSR(mstatus, MSTATUS_MIE);
#endif
}

/**
  * @brief Global Interrupt Disable.
  * @retval BASE_STATUS_OK.
  * @note   Must be called in Interrupt(Machine mode)
  */
void IRQ_Disable(void)
{
#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
    for (unsigned int index = IRQ_VECTOR_CNT; index < IRQ_MAX; index++) {
        IRQ_DisableN(index);
    }
#else
    CLEAR_CSR(mstatus, MSTATUS_MIE | MSTATUS_MPIE);
#endif
}

/**
  * @brief Enable the specified interrupt.
  * @param irqNum  External interrupt number.
  * @retval BASE_STATUS_OK or IRQ_ERRNO_NUM_INVALID.
  */
unsigned int IRQ_EnableN(unsigned int irqNum)
{
#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
    unsigned int priv = IRQ_GetCpuPrivilege();
#endif

    INTERRUPT_PARAM_CHECK_WITH_RET((irqNum >= IRQ_VECTOR_CNT && irqNum < IRQ_MAX), IRQ_ERRNO_NUM_INVALID);

    /* The interrupt enable bits that can be controlled in the mie register (32 bits), up to 32
       can be controlled, and each bit corresponds to an interrupt enable */

    RISCV_PRIV_MODE_SWITCH(priv);

    IRQ_CLIC_REG *clicRegAddr = (IRQ_CLIC_REG *)GetClicRegAddr(irqNum);
    clicRegAddr->BIT.clic_ie = 0x01;

    RISCV_PRIV_MODE_SWITCH(priv);

    return BASE_STATUS_OK;
}

/**
  * @brief Disable the specified interrupt.
  * @param irqNum  External interrupt number.
  * @retval BASE_STATUS_OK or IRQ_ERRNO_NUM_INVALID or IRQ_ERRNO_NOT_CREATED.
  */
unsigned int IRQ_DisableN(unsigned int irqNum)
{
#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
    unsigned int priv = IRQ_GetCpuPrivilege();
#endif

    INTERRUPT_PARAM_CHECK_WITH_RET((irqNum >= IRQ_VECTOR_CNT && irqNum < IRQ_MAX), IRQ_ERRNO_NUM_INVALID);
    INTERRUPT_PARAM_CHECK_WITH_RET((g_irqCallbackFunc[irqNum].pfnHandler != IRQ_DummyHandler), IRQ_ERRNO_NOT_CREATED);

    RISCV_PRIV_MODE_SWITCH(priv);

    IRQ_CLIC_REG *clicRegAddr = (IRQ_CLIC_REG *)GetClicRegAddr(irqNum);
    clicRegAddr->BIT.clic_ie = BASE_CFG_DISABLE;

    RISCV_PRIV_MODE_SWITCH(priv);

    return BASE_STATUS_OK;
}

/**
  * @brief Clear the interrupt status.
  * @param irqNum  External interrupt number.
  * @retval None.
  */
void IRQ_ClearN(unsigned int irqNum)
{
    /* Get the base address of the interrupt number. */
    IRQ_CLIC_REG *clicRegAddr = (IRQ_CLIC_REG *)GetClicRegAddr(irqNum);
    if (clicRegAddr->BIT.clic_ip) {
        clicRegAddr->BIT.clic_ip = BASE_CFG_UNSET;  /* Clear the pending. */
    }
}

/**
  * @brief Print RISCV register.
  * @param context.
  * @note  The actual code is generated by IDE
  * @retval None.
  */
__weak void SysErrPrint(const SyserrContext *context)
{
    BASE_FUNC_UNUSED(context);
}

/**
  * @brief System error completion processing
  * @param None.
  * @retval None.
  */
static void SysErrFinish(void)
{
}

/**
  * @brief Exception Handler Entry.
  * @param context error context.
  * @retval None.
  */
void SysErrExcEntry(const SyserrContext *context)
{
    SysErrPrint(context);
    SysErrFinish();
}

/**
  * @brief NMI Interrupt Handler Entry.
  * @param context error context.
  * @retval None.
  */
void SysErrNmiEntry(const SyserrContext *context)
{
    INTERRUPT_ASSERT_PARAM(context != NULL);
    SysErrPrint(context);
    SysErrFinish();
}

/**
  * @brief Set the priority of external interrupt.
  * @param irqNum  External interrupt number.
  * @param priority.
  * @retval IRQ_ERRNO_NUM_INVALID or IRQ_ERRNO_PRIORITY_INVALID or BASE_STATUS_OK.
  */
unsigned int IRQ_SetPriority(unsigned int irqNum, unsigned int priority)
{
    INTERRUPT_PARAM_CHECK_WITH_RET((irqNum >= IRQ_VECTOR_CNT && irqNum < IRQ_MAX), IRQ_ERRNO_NUM_INVALID);
    INTERRUPT_PARAM_CHECK_WITH_RET((priority >= IRQ_PRIO_LOWEST && priority <= IRQ_PRIO_HIGHEST), \
                                   IRQ_ERRNO_PRIORITY_INVALID);

    #if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
        unsigned int priv = IRQ_GetCpuPrivilege();
    #endif

    RISCV_PRIV_MODE_SWITCH(priv);

    /* Get the base address of the interrupt number. */
    IRQ_CLIC_REG *clicRegAddr = (IRQ_CLIC_REG *)GetClicRegAddr(irqNum);
    unsigned char curCtrl = clicRegAddr->BIT.clic_ctrl;
    curCtrl &= (~IRQ_PRI_MASK);
    curCtrl |= (priority << IRQ_PRI_OFFSET);
    clicRegAddr->BIT.clic_ctrl = curCtrl; /* Set priority level. */

    RISCV_PRIV_MODE_SWITCH(priv);

    return BASE_STATUS_OK;
}

/**
  * @brief Get the priority of external interrupt.
  * @param irqNum  External interrupt number.
  * @output priority.
  * @retval IRQ_ERRNO_NUM_INVALID or IRQ_ERRNO_PRIORITY_INVALID or BASE_STATUS_OK.
  */
unsigned int IRQ_GetPriority(unsigned int irqNum, unsigned int *priority)
{
    INTERRUPT_PARAM_CHECK_WITH_RET(irqNum < IRQ_MAX, IRQ_ERRNO_NUM_INVALID);

    #if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
        unsigned int priv = IRQ_GetCpuPrivilege();
    #endif

    RISCV_PRIV_MODE_SWITCH(priv);

    /* Get the base address of the interrupt number. */
    IRQ_CLIC_REG *clicRegAddr = (IRQ_CLIC_REG *)GetClicRegAddr(irqNum);
    unsigned char curCtrl = clicRegAddr->BIT.clic_ctrl;
    *priority = (curCtrl & IRQ_PRI_MASK) >> IRQ_PRI_OFFSET;

    RISCV_PRIV_MODE_SWITCH(priv);

    return BASE_STATUS_OK;
}

/**
  * @brief  Clear all external interrupts
  * @retval unsigned int, 0: success.
  */
unsigned int IRQ_ClearAll(void)
{
    unsigned int index;
    for (index = IRQ_VECTOR_CNT; index < IRQ_MAX; index++) {
        IRQ_ClearN(index);
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Interrupt dummy handler
  * @param arg     Not used
  * @retval None.
  */
static void IRQ_DummyHandler(void *arg)
{
    BASE_FUNC_UNUSED(arg);
}

/**
  * @brief Construct a new irq setcallback object
  * @param irqNum external interrupt number
  * @param func   callback function
  * @param arg    callback arg
  * @retval None.
  */
static inline void IRQ_SetCallBack(unsigned int irqNum, IRQ_PROC_FUNC func, void *arg)
{
    g_irqCallbackFunc[irqNum].param = arg;
    g_irqCallbackFunc[irqNum].pfnHandler = func;
}

/**
  * @brief Get CPU Privilege by ecall
  * @param none
  * @retval mcause value
  */
#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
static inline unsigned int IRQ_GetCpuPrivilege(void)
{
    return (g_RiscvPrivMode == 0) ? RISCV_U_MODE : RISCV_M_MODE;
}
#endif
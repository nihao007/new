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
  * @file    startup.S
  * @author  MCU Application Driver Team
  * @brief   RISC-V trap handling and startup code
  */

#ifndef ENTRY_S
#define ENTRY_S

#include "feature.h"

#ifdef NOS_TASK_SUPPORT
.extern OsHwiPostHandle
.extern OsTaskTrueSwitch
#define NOS_HwiPostDispatch OsHwiPostHandle
#define NOS_TaskDispatch OsTaskTrueSwitch
#define NOS_TASK_SWITCH_MAGIC_NUM 0xACBCCCDC
#define TICK_IRQ_CLIC_REG_ADDR     0xC001054  /* 0xC000000 + 0x1000 + 4*21 */
#endif

/* Importing External Variables. */
.extern                             __stack_top
.extern                             __irq_stack_top
.extern                             SysErrNmiEntry
.extern                             SysErrExcEntry
.extern                             trap_entry
#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
.extern                             g_RiscvPrivMode
#endif

/* Defines the operation instruction and bit width. */
#define LREG                        lw
#define SREG                        sw
#define FLREG                       flwp          /* Performance acceleration:
                                                     write or read two words floating-point storage instruction sets at a time. */
#define FSREG                       fswp
#define REGBYTES                    4             /* Only riscv-32 is supported. */

/* EFC Initialization Configuration Address Macro. */
#define EFC_BASE_ADDR               0x14710000    /* efc rdacckr base address */
#define EFC_RDACCLR_BASE_ADDR       0x14711000    /* efc rdacckr base address */
#define EFC_PFEFETCH_OFFSET         0x120         /* efc prefetch register offset */
#define EFC_CACHE_OFFSET            0x124         /* efc cache register offset */

#define SYSRAM_ERROR                0x10108300

#define INT_REGISTER_NUM            16

#define CLIC_MEMORY_MAP_OFFSET      0x1000
#define CLIC_INT_RSV_NUM            16            /* Low interrupt 16 that cannot be configured as the vector mode */
#define CLIC_INT_TOTAL_NUM          154           /* Maximum Total Interrupt Number */
#define IT_VECTOR_MODE_INIT_CFG     0x0FC10000    
#define IT_DIRECT_MODE_INIT_CFG     0x0FC00000


#define MCAUSE_MASK_CODE_BIT        0x00000FFF    /* Exception and Interrupt Number Mask */
#define MCAUSE_INT_EXCEPTION_MASK   0x80000000    /* Exception and Interrupt Mask */

#define MSTATUS_MPP_MACHINE         0x00001800
#define MCAUSE_ECALL_FROM_MMODE     11
#define MCAUSE_EXCEPTION_CODE_MASK  0x7FF
#define MCAUSE_ECALL_FROM_UMODE     8

#define MSTATUS_MIE                 0x00000008
#define MCAUSE_NMI                  (0xFFF)

#define SRAM_UPLIMIT                0x4040000

/* Size of the stack space allocated for interrupts or exceptions. */
#define SYSERR_INT_SIZE_ON_STACK    (48 * REGBYTES)

#ifdef NOS_TASK_SUPPORT /* Support Multi-task needs more stack to restore TickIRQEnable */
#define NOS_TOTAL_UNI_SIZE_ON_STACK  (16 * REGBYTES)
#ifdef FLOAT_SUPPORT
#define NOS_TOTAL_INT_SIZE_ON_STACK     (44 * REGBYTES)
#else
#define NOS_TOTAL_INT_SIZE_ON_STACK     (24 * REGBYTES)
#endif
#endif

#ifdef FLOAT_SUPPORT
#define TOTAL_INT_SIZE_ON_STACK     (32 * REGBYTES)
#else
#define TOTAL_INT_SIZE_ON_STACK     (4 * REGBYTES)
#endif

    .section      .data.magic
    .word         0xA37E95BD        /* eflash magic number, bootrom will check it */

    .section      .text.entry
    .global       _start
    .option       norvc

_start:
    j handle_reset


.section      .text.trap_entry      /* The interrupt or exception entry base address is 64-byte aligned. */
.align 6                                                    
TrapVector:                         /* Exception or direct address mode interrupt entry. */
    j TrapHandler

.align 6                                               
IntVector:
    .skip REGBYTES * CLIC_INT_RSV_NUM, 0x0
    .rept (CLIC_INT_TOTAL_NUM - CLIC_INT_RSV_NUM)
        .4byte IntHandler
    .endr


.section .text.entry
.macro push_reg
    addi sp, sp, -(SYSERR_INT_SIZE_ON_STACK)
    SREG ra, 0 * REGBYTES(sp)
    SREG t0, 1 * REGBYTES(sp)
    SREG t1, 2 * REGBYTES(sp)
    SREG t2, 3 * REGBYTES(sp)
    SREG a0, 4 * REGBYTES(sp)
    SREG a1, 5 * REGBYTES(sp)
    SREG a2, 6 * REGBYTES(sp)
    SREG a3, 7 * REGBYTES(sp)
    SREG a4, 8 * REGBYTES(sp)
    SREG a5, 9 * REGBYTES(sp)
    SREG a6, 10 * REGBYTES(sp)
    SREG a7, 11 * REGBYTES(sp)
    SREG t3, 12 * REGBYTES(sp)
    SREG t4, 13 * REGBYTES(sp)
    SREG t5, 14 * REGBYTES(sp)
    SREG t6, 15 * REGBYTES(sp)
.endm

.macro pop_reg
    LREG ra, 0 * REGBYTES(sp)
    LREG t0, 1 * REGBYTES(sp)
    LREG t1, 2 * REGBYTES(sp)
    LREG t2, 3 * REGBYTES(sp)
    LREG a0, 4 * REGBYTES(sp)
    LREG a1, 5 * REGBYTES(sp)
    LREG a2, 6 * REGBYTES(sp)
    LREG a3, 7 * REGBYTES(sp)
    LREG a4, 8 * REGBYTES(sp)
    LREG a5, 9 * REGBYTES(sp)
    LREG a6, 10 * REGBYTES(sp)
    LREG a7, 11 * REGBYTES(sp)
    LREG t3, 12 * REGBYTES(sp)
    LREG t4, 13 * REGBYTES(sp)
    LREG t5, 14 * REGBYTES(sp)
    LREG t6, 15 * REGBYTES(sp)
    addi sp, sp, (SYSERR_INT_SIZE_ON_STACK)
.endm

.macro push_freg
#ifdef FLOAT_SUPPORT
    FSREG ft0, ft1, (0 * REGBYTES)(sp)
    FSREG ft2, ft3, (2 * REGBYTES)(sp)
    FSREG ft4, ft5, (4 * REGBYTES)(sp)
    FSREG ft6, ft7, (6 * REGBYTES)(sp)
    FSREG fa0, fa1, (8 * REGBYTES)(sp)
    FSREG fa2, fa3, (10 * REGBYTES)(sp)
    FSREG fa4, fa5, (12 * REGBYTES)(sp)
    FSREG fa6, fa7, (14 * REGBYTES)(sp)
    FSREG ft8, ft9, (16 * REGBYTES)(sp)
    FSREG ft10, ft11, (18 * REGBYTES)(sp)
    csrr  a0, fcsr       /* The Floating-Point Control and Status Register (fcsr) must have thread storage. */
    SREG  a0, 20 * REGBYTES(sp)
#endif
.endm

.macro pop_freg
#ifdef FLOAT_SUPPORT
    FLREG ft0, ft1, (0 * REGBYTES)(sp)
    FLREG ft2, ft3, (2 * REGBYTES)(sp)
    FLREG ft4, ft5, (4 * REGBYTES)(sp)
    FLREG ft6, ft7, (6 * REGBYTES)(sp)
    FLREG fa0, fa1, (8 * REGBYTES)(sp)
    FLREG fa2, fa3, (10 * REGBYTES)(sp)
    FLREG fa4, fa5, (12 * REGBYTES)(sp)
    FLREG fa6, fa7, (14 * REGBYTES)(sp)
    FLREG ft8, ft9, (16 * REGBYTES)(sp)
    FLREG ft10, ft11, (18 * REGBYTES)(sp)
    LREG  a0, 20 * REGBYTES(sp)
    csrw  fcsr, a0
#endif
.endm

.macro SAVE_SYSERR_REGS
    SREG s0, 16 * REGBYTES(sp)
    SREG s1, 17 * REGBYTES(sp)
    SREG s2, 18 * REGBYTES(sp)
    SREG s3, 19 * REGBYTES(sp)
    SREG s4, 20 * REGBYTES(sp)
    SREG s5, 21 * REGBYTES(sp)
    SREG s6, 22 * REGBYTES(sp)
    SREG s7, 23 * REGBYTES(sp)
    SREG s8, 24 * REGBYTES(sp)
    SREG s9, 25 * REGBYTES(sp)
    SREG s10, 26 * REGBYTES(sp)
    SREG s11, 27 * REGBYTES(sp)
    addi a1,  sp, 0
    SREG a1,  28 * REGBYTES(sp)           /* save original sp */
    SREG gp,  29 * REGBYTES(sp)
    SREG tp,  30 * REGBYTES(sp)
    csrr a0,  mepc
    csrr a1,  mstatus
    csrr a2,  mtval
    csrr a3,  mcause
    SREG a0,  31 * REGBYTES(sp)   /* mepc */
    SREG a1,  32 * REGBYTES(sp)  /* mstatus */
    SREG a2,  33 * REGBYTES(sp)  /* mtval */
    SREG a3,  34 * REGBYTES(sp)  /* mcause */
    csrr a0,  cxcptsc
    SREG a0,  35 * REGBYTES(sp)  /* mcause */
    mv a0,sp
.endm

.align 2
IntHandler:                                                /* Interrupt handling process. */
#ifdef NOS_TASK_SUPPORT
    addi  sp, sp, -(NOS_TOTAL_INT_SIZE_ON_STACK)

    SREG  a0, 3 * REGBYTES(sp)
    SREG  a1, 4 * REGBYTES(sp)

#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
    la    a0, g_RiscvPrivMode
    lw    a1, (a0)
    addi  a1, a1, 1
    sw    a1, (a0)
#endif
    csrr  a0, mintstatus  /* Reads the current interrupt priority */
    csrr  a1, mintthresh  /* read prithd */
    csrw  mintthresh, a0  /* save prithd */
    SREG  a1, 6 * REGBYTES(sp)
    csrr  a1, mstatus               /* read mstatus */
    SREG  a1, 7 * REGBYTES(sp)      /* save mstatus */
    csrr  a1, mepc                  /* read mepc */
    SREG  a1, 8 * REGBYTES(sp)      /* save mepc */
    csrr  a1, mcause                  /* read mcause */
    SREG  a1, 20 * REGBYTES(sp)      /* save mcause */
    li  a1, (3<<11)
    csrs  mstatus, a1

.align 2
custom_nested_irq_main_handler_entry:
    SREG  t0, 0 * REGBYTES(sp)
    SREG  t1, 1 * REGBYTES(sp)
    SREG  t2, 2 * REGBYTES(sp)
    SREG  a2, 5 * REGBYTES(sp)
    SREG  ra, 9 * REGBYTES(sp)
    SREG  a3, 10 * REGBYTES(sp)
    SREG  a4, 11 * REGBYTES(sp)
    SREG  a5, 12 * REGBYTES(sp)
    SREG  a6, 13 * REGBYTES(sp)
    SREG  a7, 14 * REGBYTES(sp)
    SREG  t3, 15 * REGBYTES(sp)
    SREG  t4, 16 * REGBYTES(sp)
    SREG  t5, 17 * REGBYTES(sp)
    SREG  t6, 18 * REGBYTES(sp)
    la a0, TICK_IRQ_CLIC_REG_ADDR
    lw t0, 0(a0)
    SREG  t0, 19 * REGBYTES(sp)

#ifdef FLOAT_SUPPORT
    FSREG ft0, ft1, (23 * REGBYTES)(sp)
    FSREG ft2, ft3, (25 * REGBYTES)(sp)
    FSREG ft4, ft5, (27 * REGBYTES)(sp)
    FSREG ft6, ft7, (29 * REGBYTES)(sp)
    FSREG fa0, fa1, (31 * REGBYTES)(sp)
    FSREG fa2, fa3, (33 * REGBYTES)(sp)
    FSREG fa4, fa5, (35 * REGBYTES)(sp)
    FSREG fa6, fa7, (37 * REGBYTES)(sp)
    FSREG ft8, ft9, (37 * REGBYTES)(sp)
    FSREG ft10, ft11, (41 * REGBYTES)(sp)
    frcsr  t0
    SREG   t0, 43 * REGBYTES(sp)
#endif

    LREG  a2, 6 * REGBYTES(sp)
    bnez  a2, IrqCallback /* Should determine whether need to switch stack */
    csrw  mscratch, sp
    la  sp, __irq_stack_top - 16

IrqCallback:
    addi  sp, sp, -4 * REGBYTES
    SREG  a2, 0 * REGBYTES(sp) /* save prithd in irq stack */
    li    a1, MCAUSE_MASK_CODE_BIT
    csrr  a0, mcause
    and   a0, a0, a1

IrqCallbackNew:
    call  InterruptEntry
    li      a2, 0x8
    csrrc   zero, mstatus, a2 /* When using NOS_TASK_SUPPORT, disable gloal irq before any stack operation */
    LREG  a2, 0 * REGBYTES(sp) /* restore prithd in irq stack */
    addi  sp, sp, 4 * REGBYTES

    bnez  a2, BacktoIrq
    csrr  sp, mscratch
    tail  NOS_HwiPostDispatch /* Should determine whether need to reschedule */

BacktoIrq:
    LREG  t1, 1 * REGBYTES(sp)
    LREG  t2, 2 * REGBYTES(sp)
    LREG  a2, 5 * REGBYTES(sp)
    LREG  ra, 9 * REGBYTES(sp)
    LREG  a3, 10 * REGBYTES(sp)
    LREG  a4, 11 * REGBYTES(sp)
    LREG  a5, 12 * REGBYTES(sp)
    LREG  a6, 13 * REGBYTES(sp)
    LREG  a7, 14 * REGBYTES(sp)
    LREG  t3, 15 * REGBYTES(sp)
    LREG  t4, 16 * REGBYTES(sp)
    LREG  t5, 17 * REGBYTES(sp)
    LREG  t6, 18 * REGBYTES(sp)
    LREG  t0, 19 * REGBYTES(sp)
    li t1, TICK_IRQ_CLIC_REG_ADDR
    sw t0, (t1)
    LREG  t1, 1 * REGBYTES(sp)

#ifdef FLOAT_SUPPORT
    FLREG ft0, ft1, (23 * REGBYTES)(sp)
    FLREG ft2, ft3, (25 * REGBYTES)(sp)
    FLREG ft4, ft5, (27 * REGBYTES)(sp)
    FLREG ft6, ft7, (29 * REGBYTES)(sp)
    FLREG fa0, fa1, (31 * REGBYTES)(sp)
    FLREG fa2, fa3, (33 * REGBYTES)(sp)
    FLREG fa4, fa5, (35 * REGBYTES)(sp)
    FLREG fa6, fa7, (37 * REGBYTES)(sp)
    FLREG ft8, ft9, (39 * REGBYTES)(sp)
    FLREG ft10, ft11, (41 * REGBYTES)(sp)
    LREG   t0,  43 * REGBYTES(sp)
    fscsr  t0
#endif

quit_int:
    /*
     *  Since the interrupt is already turned off when loading mstatus (after entering the interrupt,
     *  the hardware will turn off the interrupt, so when saving mstatus, the interrupt is already turned off),
     *  so there is no need to turn off the interrupt separately.
     */

    LREG  a0, 7 * REGBYTES(sp)  /* load mstatus */
    LREG  a1, 8 * REGBYTES(sp)  /* load mepc */

restore_mstatus:
    csrw  mstatus, a0

    LREG  t0, 0 * REGBYTES(sp)
    csrw  mepc, a1
    LREG  a0, 6 * REGBYTES(sp)  /* load prithd */
    csrw  mintthresh, a0
    LREG  a0, 20 * REGBYTES(sp) 
    csrw mcause, a0

#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
    la    a0, g_RiscvPrivMode
    lw    a1, (a0)
    addi  a1, a1, -1
    sw    a1, (a0)
#endif

    LREG  a1, 4 * REGBYTES(sp)   /* 2 consecutive csrw instructions will have a bubble */
    LREG  a0, 3 * REGBYTES(sp)
    addi  sp, sp, NOS_TOTAL_INT_SIZE_ON_STACK
    mret

#else /* #ifdef NOS_TASK_SUPPORT */
    intnesten -(TOTAL_INT_SIZE_ON_STACK), 3                /* 3: machine mode. */
#ifdef FLOAT_SUPPORT
    push_freg
#endif
#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
    la    a0, g_RiscvPrivMode
    lw    a1, (a0)
    addi  a1, a1, 1
    sw    a1, (a0)
#endif
    csrr  a0, mcause
    li    a1, MCAUSE_MASK_CODE_BIT                                
    and   a0, a0, a1
    call  InterruptEntry
#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
    la    a0, g_RiscvPrivMode
    lw    a1, (a0)
    addi  a1, a1, -1
    sw    a1, (a0)
#endif
#ifdef FLOAT_SUPPORT
    pop_freg 
#endif
    intnestdis (TOTAL_INT_SIZE_ON_STACK), 3
    mret
#endif /* #ifdef NOS_TASK_SUPPORT */

.align 2
TrapHandler:    /* Exception or direct address mode interrupt entry. */

#ifdef NOS_TASK_SUPPORT /* Support Multi-task needs to check mcause == ecall and a0 = magic word for taskSwitch */
    addi sp, sp, -8
    SREG  a1, 0(sp)
    SREG  a2, 4(sp)
    csrr  a1, mcause
    li    a2, MCAUSE_EXCEPTION_CODE_MASK
    and a1, a1, a2
    li    a2, MCAUSE_ECALL_FROM_MMODE
    bne   a1, a2, IsTrap
    li    a2, NOS_TASK_SWITCH_MAGIC_NUM
    bne   a0, a2, IsTrap
    LREG  a2, 4(sp)
    LREG  a1, 0(sp)
    addi  sp, sp, 8
    tail  NOS_TaskDispatch

IsTrap:
    LREG  a2, 4(sp)
    LREG  a1, 0(sp)
    addi  sp, sp, 8
#endif

    push_reg
    li   a1, MCAUSE_ECALL_FROM_MMODE
    li   a2, MCAUSE_MASK_CODE_BIT
    csrr a3, mcause
    and  a0, a3, a2
#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
    beq a0, a1, switch_to_umode
#else
    beq a0, a1, switch_to_mmode
#endif
    li  a1, MCAUSE_ECALL_FROM_UMODE
    beq a0, a1, switch_to_mmode
    li  a1, MCAUSE_NMI
    beq a0, a1, NmiEntry
    j   TrapEntry

.align 2
NmiEntry:
    SAVE_SYSERR_REGS
    call SysErrNmiEntry
deadLoop1:
    tail deadLoop1
    nop

.align 2
TrapEntry:
    SAVE_SYSERR_REGS
    csrc mstatus, MSTATUS_MIE     /* Exception run with interrupts masked */
    call SysErrExcEntry
deadLoop2:
    tail deadLoop2

#if defined(USER_MODE_ENABLE) && (USER_MODE_ENABLE == 1)
.align 2
switch_to_umode:
    li   a1, MSTATUS_MPP_MACHINE
    csrc mstatus, a1
    csrr a2, mepc
    addi a1, a2, 4
    csrw mepc, a1
    pop_reg
    mret
#endif

.align 2
switch_to_mmode:
    li   a1, MSTATUS_MPP_MACHINE
    csrs mstatus, a1
    csrr a2, mepc
    addi a1, a2, 4
    csrw mepc, a1
    pop_reg
    mret

.align 2
mem_cpy:
    bge  t0, t2, cpy_done
    lw   t3, (t1)
    sw   t3, (t0)
    addi t0, t0, 4
    addi t1, t1, 4
    j    mem_cpy
cpy_done:
    ret

.align 2
handle_reset:
    csrwi mstatus, 0
    csrci mstatus, 0x08
    la    t0, TrapVector
    ori    t0, t0, 3
    csrw  mtvec, t0      /* The lower two bits are fixed at 11. All trap are redirected to this address. */
    la    t0, IntVector
    csrw  mtvt, t0
    csrw mcountinhibit, 0

flash_init:
/* eflash prefetch enable */
    li  t0, EFC_RDACCLR_BASE_ADDR
    lw  t1, EFC_PFEFETCH_OFFSET(t0)
    ori t1, t1, 0x3                  /* 0x3: enable bank0 and bank1 prefetch. */
    sw  t1, EFC_PFEFETCH_OFFSET(t0)

/* eflash cache enable */
    lw  t1, EFC_CACHE_OFFSET(t0)
    ori t1, t1, 0x03
    sw  t1, EFC_CACHE_OFFSET(t0)

/* initialize global pointer */
    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

/* initialize stack pointer */
#ifdef NOS_TASK_SUPPORT /* Support Multi-task needs to use irq stack */
    la sp, __init_stack_top
#else
    la sp, __stack_top
#endif

/* perform the rest of initialization in C */
clear_sram:
    /* clear sysram parity error */
    li  t0, SYSRAM_ERROR
    lw  t1, (t0)
    ori t1, t1, 1
    sw  t1, (t0)

    la t0, SRAM_START
    la t1, SRAM_END
    li t2, 0
    li t3, SRAM_UPLIMIT
    bge t0, t1, clear_share_sram_cfg
    bge t0, t3, clear_share_sram_cfg

clear_sram_loop:
    sw      t2, (t0)                   /* clear sram */
    addi    t0, t0, 4                  /* increment clear index pointer */
    blt     t0, t1, clear_sram_loop    /* are we at the end yet, if not , contiue till the end */

clear_share_sram_cfg:
    la t0, SRAM_DATA_SHARE_MEMORY_START
    la t1, SRAM_DATA_SHARE_MEMORY_END
    li t2, 0
    bge t0, t1, start_coderom_code_copy
    bge t0, t3, start_coderom_code_copy

clear_share_sram_loop:
    csrr    t3, mhartid                /* get cpu id */
    bne     t3, t2, start_coderom_code_copy  /* cpu1 or cpu2 break loop */
    sw      t2, (t0)                   /* clear share sram */
    addi    t0, t0, 4                  /* increment clear index pointer */
    blt     t0, t1, clear_share_sram_loop    /* are we at the end yet, if not , contiue till the end */

start_coderom_code_copy:
    la t0, __sram_code_start_addr      /* SRAM addr */
    la t1, __sram_code_load_addr       /* ROM addr  */
    la t2, __sram_code_end_addr
    jal mem_cpy

start_reserved_data_copy:
    la t0, __reserved_code_start_addr  /* SRAM addr */
    la t1, __reserved_code_load_addr   /* ROM addr  */
    la t2, __reserved_code_end_addr
    jal mem_cpy

start_coderom_data_copy:
    la t0, __data_start                /* SRAM addr */
    la t1, __data_load                 /* ROM addr  */
    la t2, __data_end
    jal mem_cpy

stack_sram_bound_data_copy:
    la t0, __stack_sram_bound_data_start              /* SRAM addr */
    la t1, __stack_sram_bound_data_load               /* ROM addr  */
    la t2, __stack_sram_bound_data_end
    jal mem_cpy

/* support float */
    li t0, 0x2008           
    csrs mstatus, t0
    li t0, 0x20
    csrs misa, t0      /* Enable single-precision floating-point extension */

/* Initialize the interrupt configuration. */
    csrr t0, mclicbase
    li   t1, IT_VECTOR_MODE_INIT_CFG
    li   t2, CLIC_MEMORY_MAP_OFFSET + REGBYTES * CLIC_INT_RSV_NUM
    add  t2, t2, t0
    li   t3, CLIC_MEMORY_MAP_OFFSET + REGBYTES * CLIC_INT_TOTAL_NUM
    add  t3, t3, t0

clic_init:
    sw   t1, (t2)
    lw   t4, (t2)
    addi t2, t2, REGBYTES
    bgeu t2, t3, clic_init_quit
    j  clic_init

clic_init_quit:
    ecall
    jal Chip_Init

/* jump to C func. */
    jal main

dead_loop:
    j dead_loop

#endif

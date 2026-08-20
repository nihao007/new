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
  * @file    flash.lds
  * @author  MCU Application Driver Team
  * @brief   RISCV flash link script
  */
OUTPUT_ARCH( "riscv" )
ENTRY(_start)

/* USER CODE BEGIN 0 */
/* USER CODE 区域内代码不会被覆盖，区域外会被生成的默认代码覆盖（其余USER CODE 区域同理） */
/* USER CODE END 0 */

/* NOTE: The last 28K(0x4039000-0x4040000) of SRAM is reserved. 
   -16K(0x4039000-0x403D000) is used for program burn and debug.
   -12K(0x403D000-0x4040000) is used for the CAN module. 
*/

/* RAM DATA and STACK SPACE CONFIG. */
SRAM_START                    = 0x3FC0000;
SRAM_SIZE                     = 0x7D000;
SRAM_END                      = SRAM_START + SRAM_SIZE;
SRAM_DATA_SHARE_MEMORY_START  = SRAM_END;                         /* Multi-core applications share SRAM memory. */
SRAM_DATA_SHARE_MEMORY_SIZE   = 0;
SRAM_DATA_SHARE_MEMORY_END    = SRAM_DATA_SHARE_MEMORY_START + SRAM_DATA_SHARE_MEMORY_SIZE;

/* RAM CODE SPACE CONFIG. */
RAM_CODE_START = SRAM_START;
RAM_CODE_SIZE  = 0; /* Recommend 64K alignment, minimum 1K alignment. */

/* RAM DATA : FOR DIAGNOSE BUFFER SPACE CONFIG. */
RAM_DIAGNOSE_BUF_START = RAM_CODE_START + RAM_CODE_SIZE;
RAM_DIAGNOSE_BUF_SIZE  = 0;

/* RAM DATA : FOR RESERVER DATA SPACE CONFIG. */
RAM_RESERVE_DATA_START = RAM_DIAGNOSE_BUF_START + RAM_DIAGNOSE_BUF_SIZE;
RAM_RESERVE_DATA_SIZE  = 0;

/* RAM DATA : FOR DATA and BSS SPACE CONFIG. */
RAM_START = RAM_RESERVE_DATA_START + RAM_RESERVE_DATA_SIZE;
RAM_SIZE  = 0x50000;

/* RAM DATA : FOR STACK BOUND SPACE CONFIG. */
STACK_SRAM_BOUND_START = RAM_START + RAM_SIZE;
STACK_SRAM_BOUND_SIZE  = 0;  /* enable, size fix 0x10 byte */

/* RAM DATA : FOR STACK SPACE CONFIG. */
STACK_START     = STACK_SRAM_BOUND_START + STACK_SRAM_BOUND_SIZE;
NMI_STACK_SIZE  = 1024;
STACK_SIZE      = 0x29000 - NMI_STACK_SIZE;
INIT_STACK_SIZE = 1024;

/* FLASH SPACE CONFIG. */
FLASH_START     = 0x3000000;
FLASH_SIZE      = 0xFFFFC;

/* The address global variable is used for PMP protection. */
g_itcmStartLds = SRAM_START;
g_itcmEndLds = RAM_DIAGNOSE_BUF_START;
g_dtcmStartLds = RAM_DIAGNOSE_BUF_START;
g_dtcmEndLds = SRAM_END;
g_shareMemoryStartLds = SRAM_DATA_SHARE_MEMORY_START;
g_shareMemoryEndLds = SRAM_DATA_SHARE_MEMORY_END;

MEMORY
{
    /* ram for code */
    RAM_CODE(xr) : ORIGIN = RAM_CODE_START, LENGTH = RAM_CODE_SIZE
    /* ram for diagnose buf */
    RAM_DIAGNOSE_BUF(rw) : ORIGIN = RAM_DIAGNOSE_BUF_START, LENGTH = RAM_DIAGNOSE_BUF_SIZE
    /* ram for reserved data */
    RAM_RESERVE_DATA(rw) : ORIGIN = RAM_RESERVE_DATA_START, LENGTH = RAM_RESERVE_DATA_SIZE
    /* ram for common bss and data */
    RAM_DATA(xrw) : ORIGIN = RAM_START, LENGTH = RAM_SIZE
    /* ram for common bss and data */
    STACK_SRAM_BOUND(xrw) : ORIGIN = STACK_SRAM_BOUND_START, LENGTH = STACK_SRAM_BOUND_SIZE
    /* ram for stack */
    RAM_STACK(xrw) : ORIGIN = STACK_START, LENGTH = STACK_SIZE + NMI_STACK_SIZE
    /* magic number */
    FLASH_MAGIC(rw) : ORIGIN = FLASH_START, LENGTH = 4
    /* ram for target */
    FLASH_CODE(rx) : ORIGIN = FLASH_START + 4, LENGTH = FLASH_SIZE

/* USER CODE BEGIN 1 */
/* USER CODE 区域内代码不会被覆盖，区域外会被生成的默认代码覆盖（其余USER CODE 区域同理） */
/* USER CODE END 1 */
}

SECTIONS
{
    /* The startup code goes first into FLASH */
    .data.magic : ALIGN(4)
    {
        __FLASH_BEGIN__ = ORIGIN(FLASH_MAGIC);
        KEEP(*(.data.magic))
    } > FLASH_MAGIC
    g_falshStartLds = __FLASH_BEGIN__;

    /* The startup code goes first into FLASH_CODE */
    .text.entry : ALIGN(4)
    {
        KEEP(*(.text.entry))
    } > FLASH_CODE

    /* The startup code goes first into FLASH_CODE */
    .text.trap_entry : ALIGN(4)
    {
        KEEP(*(.text.trap_entry))
    } > FLASH_CODE

    /* Stack in SRAM at Highest addresses */
    .stacks (NOLOAD) :
    {
        . = ALIGN(4);
        __SYSTEM_STACK_BEGIN__ = ORIGIN(RAM_STACK);
        KEEP(*(.stacks))
        __SYSTEM_STACK_END__ = ORIGIN(RAM_STACK) + STACK_SIZE;
        . = ALIGN(0x20);
        __INTERRUPT_STACK_BEGIN__ = __SYSTEM_STACK_END__;
        . = ALIGN(0x20);
        __NMI_STACK_BEGIN__ = __SYSTEM_STACK_END__;
        __nmi_stack_bottom = .;
        . += NMI_STACK_SIZE;
        __nmi_stack_top = .;
    } > RAM_STACK
    __stack_top = __SYSTEM_STACK_END__;
    __init_stack_top = __SYSTEM_STACK_BEGIN__ + INIT_STACK_SIZE;
    __irq_stack_top = __SYSTEM_STACK_END__;

    .text.sram : ALIGN(4)
    {
        __sram_code_load_addr = LOADADDR(.text.sram);
        __sram_code_start_addr = .;
        *(.text.sram)
        . = ALIGN(4);
        __sram_code_end_addr = .;
    } > RAM_CODE AT > FLASH_CODE

    .reserved.data : ALIGN(4)
    {
        __reserved_code_load_addr = LOADADDR(.reserved.data);
        __reserved_code_start_addr = .;
        *(.reserved.data*)
        . = ALIGN(4);
        __reserved_code_end_addr = .;
    } > RAM_RESERVE_DATA AT > FLASH_CODE

    .text : ALIGN(4)
    {
        __start_addr = .;
        *(.text*)
        *(.ram.text*)
        . = ALIGN(4);
        __rodata_start = .;
        *(.rodata*)
        . = ALIGN(4);
        __rodata_end = .;
        *(.got*)
        __text_end = .;
    } > FLASH_CODE

    /* data section */
    .data : ALIGN(4)
    {
        __data_load = LOADADDR(.data);
        __data_start = .;
        *(.data*)
        *(.sdata*)
        . = ALIGN(4);
        __data_end = .;
    } > RAM_DATA AT> FLASH_CODE
    __data_size = __data_end - __data_start;

    .stackBound : ALIGN(4)
    {
        __stack_sram_bound_data_load  = LOADADDR(.stackBound);
        __stack_sram_bound_data_start = .;
        *(STACK_SRAM_BOUND)
        . = ALIGN(4);
        __stack_sram_bound_data_end   = .;
    } > STACK_SRAM_BOUND AT> FLASH_CODE

    .ramBuf : ALIGN(4)
    {
        *(RAM_DIAGNOSE_BUF)
    } > RAM_DIAGNOSE_BUF AT> FLASH_CODE

    .checkSum : ALIGN(4)
    {
        __checksum_addr = .;
        *(CHECKSUM)
        __checksum_end = .;
    } > FLASH_CODE

    /* bss section */
    .bss (NOLOAD) : ALIGN(4)
    {
        __bss_begin__ = .;
        *(.bss*)
        *(.sbss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    }  > RAM_DATA
    __bss_size__ = __bss_end__ - __bss_begin__;
    __global_pointer$ = __data_start + ((__data_size + __bss_size__) / 2);

/* USER CODE BEGIN 2 */
/* USER CODE 区域内代码不会被覆盖，区域外会被生成的默认代码覆盖（其余USER CODE 区域同理） */
/* USER CODE END 2 */
}
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
  * @file      fotp_info_read.c
  * @author    MCU Driver Team
  * @brief     This file provides firmware functions to manage the following
  *            functionalities of the fotp control register.
  *                + FOTP INFO Read API
  */
#include "chipinc.h"
#include "flash.h"
#include "fotp_info_read.h"

#define FOTP_INFO_BASE_ADDR          0x3800000
#define INFO_PAGE_SIZE_BYTE          0x400
#define REG_SIZE_BYTE                16
#define REG_SIZE_WORD                4

#define FOTP_FLASH_BANK_MODE_ADDR    0x03800490
#define REMAP_CFG_ADDR               0x3807c00
#define ERASE_REMAP_PAGE             0
#define WRITE_REMAP_PAGE             1
#define REMAP_INFO_LOCK_OFF          8
#define REMAP_PAGE_ERASE_CMD         0x05
#define REMAP_PAGE_WRITE_CMD         0x03
#define CLEAR_FLASH_INT_REG          0xFFFFFFFF

/**
  * @brief Read Four words of FOTP.
  * @param type  FOTP Range Type
  * @param index FOTP register index
  * @param buf   Buffer of read data
  * @retval BASE_STATUS_ERROR fail.
  * @retval BASE_STATUS_OK success.
  */
unsigned int FOTP_InfoGet(FOTP_InfoRngType type, unsigned int index, FOTP_CommonData *buf)
{
    unsigned int addr;

    if (buf == NULL) {
        return BASE_STATUS_ERROR;
    }
    
    if ((type >= FOTP_INFO_MAXTYPE) || (index > FOTP_INFO_REG_MAX_ID)) {
        return BASE_STATUS_ERROR;
    }

    /* Configure the read command parameters and start the read command */
    addr = FOTP_INFO_BASE_ADDR + (unsigned int)type * INFO_PAGE_SIZE_BYTE;
    addr += index * REG_SIZE_BYTE;

    unsigned int *targetAddr = (unsigned int *)(void *)(uintptr_t)addr;
    /* Read data from FIFO to buffer */
    for (unsigned int i = 0; i < REG_SIZE_WORD; ++i) {
        buf->data[i] = *targetAddr;
        targetAddr++;
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Get flash bank mode.
  * @param None
  * @retval FLASH_BankMode.
  */
FLASH_BankMode FOTP_GetBankMode(void)
{
    FLASH_BankMode mode = *(unsigned int *)FOTP_FLASH_BANK_MODE_ADDR & FLASH_DOUBLE_BANK_MODE;
    return mode;
}

/**
  * @brief Erase remap flash page.
  * @param efcx FLASH register base address.
  * @retval None.
  */
static void EraseOperate(EFC_RegStruct *efcx)
{
    /* Configure the erase start address and erase mode. */
    efcx->EFLASH_ADDR.reg = REMAP_CFG_ADDR;
    efcx->EFLASH_CMD.BIT.cmd_code  = REMAP_PAGE_ERASE_CMD;
}

/**
  * @brief Write remap flash page.
  * @param efcx FLASH register base address.
  * @param srcAddr the target address of write page.
  * @retval None.
  */
static void WriteOperate(EFC_RegStruct *efcx, const unsigned int srcAddr)
{
    unsigned int *data = NULL;
    unsigned int writeSize;

    /* The mask of program wdata celar is 0xFF. */
    if ((efcx->BUF_CLEAR.reg >> FLASH_PGM_WBUF_CNT_POS) & 0xFF) {
        efcx->BUF_CLEAR.BIT.pgm_wdata_clr = BASE_CFG_SET; /* program wdata celar enable. */
    }

    /* Step 1: Calculated the cmd program size, get srcAddress and get destAddress. */
    writeSize = FLASH_ONE_PAGE_WORD_SIZE / FLASH_ONE_WORD_BYTES_SIZE;
    data = (unsigned int *)(uintptr_t)srcAddr;
    efcx->EFLASH_ADDR.reg = REMAP_CFG_ADDR;
    for (unsigned int i = 0; i < FLASH_ONE_PAGE_WORD_SIZE; i++) {
        efcx->PGM_WDATA = *data;
        data++;
    }
    /* Step 2: Configure the parameters and start programming. */
    efcx->EFLASH_CMD.BIT.cmd_pgm_size = writeSize;
    efcx->EFLASH_CMD.BIT.cmd_code = REMAP_PAGE_WRITE_CMD;
}

/**
  * @brief Operate remap flash page, select erase or write.
  * @param operation operate flash mode.
  * @param srcAddr the target address of write or erase page.
  * @retval BASE_StatusType.
  */
static BASE_StatusType OpreateRemapPage(unsigned int operation, unsigned int srcAddr)
{
    EFC_RegStruct *efcx = EFC;
   /* Make sure the last operation is complete. */
    if (efcx->EFLASH_CMD.BIT.cmd_start) {
        return BASE_STATUS_BUSY;
    }
    efcx->MAGIC_LOCK = FLASH_KEY_REGISTER_UNLOCK_VALUE;
    /* Step 1: Configure the start address and mode, then make cmd_satrt enable. */
    if (operation == ERASE_REMAP_PAGE) {
        EraseOperate(efcx);
    } else {
        WriteOperate(efcx, srcAddr);
    }
    efcx->EFLASH_CMD.BIT.cmd_start = BASE_CFG_SET;

    /* Step 2: wait until the operation is complete. */
    while (efcx->EFLASH_CMD.BIT.cmd_start) {
        ;
    }
    /* Check whether errors occur. */
    if (efcx->INT_RAW_STATUS.BIT.int_raw_err_illegal ||
        efcx->INT_RAW_STATUS.BIT.int_raw_err_erase || efcx->FLASH_STATUS.reg != 0) {
        efcx->HIRDACCLR_CACHE_CTRL.BIT.cache_invalid_req0 = BASE_CFG_SET;
        efcx->HIRDACCLR_CACHE_CTRL.BIT.cache_invalid_req1 = BASE_CFG_SET;
        efcx->INT_CLEAR.reg = CLEAR_FLASH_INT_REG;
        efcx->MAGIC_LOCK = FLASH_KEY_REGISTER_LOCK_VALUE;
        return BASE_STATUS_ERROR;
    }
    /* Clear cache. */
    efcx->HIRDACCLR_CACHE_CTRL.BIT.cache_invalid_req0 = BASE_CFG_SET;
    efcx->HIRDACCLR_CACHE_CTRL.BIT.cache_invalid_req1 = BASE_CFG_SET;
    efcx->BUF_CLEAR.BIT.pgm_wdata_clr = 1;
    efcx->MAGIC_LOCK = FLASH_KEY_REGISTER_LOCK_VALUE;

    return BASE_STATUS_OK;
}

/**
  * @brief Toggle flash remap mode.
  * @param None.
  * @retval BASE_StatusType.
  */
BASE_StatusType ToggleFlashRemap(void)
{
    BASE_StatusType ret;
    unsigned char tempBuffer[FLASH_ONE_PAGE_SIZE] = {0};
    unsigned char *srcAddr = (unsigned char *)(uintptr_t)(void *)REMAP_CFG_ADDR;
    /* step 1 : Read back flash memory data. */
    for (unsigned int i = 0; i < FLASH_ONE_PAGE_SIZE; i++) {
        tempBuffer[i] = *srcAddr;
        srcAddr++;
    }
    /* step 2 : check info page lock status. */
    if (tempBuffer[REMAP_INFO_LOCK_OFF] != 0xFF) {
        return BASE_STATUS_ERROR; /* Flash has been locked and cannot be modified. */
    }
    /* step 3 : toggle flash remap config. */
    tempBuffer[0] ^= BASE_CFG_SET;
    /* step 4 : erase flash remap page. */
    ret =  OpreateRemapPage(ERASE_REMAP_PAGE, 0x0);
    if (ret != BASE_STATUS_OK) {
        return ret;
    }
    /* step 5 : write flash remap page. */
    ret =  OpreateRemapPage(WRITE_REMAP_PAGE, (const unsigned int)(&tempBuffer[0]));
    return ret;
}

/**
  * @brief Get flash remap mode.
  * @param None.
  * @retval FLASH_RemapMode.
  */
FLASH_RemapMode GetFlashRemapStatus(void)
{
    unsigned int remapCfg = *(unsigned int *)(uintptr_t)(void *)REMAP_CFG_ADDR;
    return (FLASH_RemapMode)(remapCfg & FLASH_ADDRESS_REAMP_ENABLE);
}
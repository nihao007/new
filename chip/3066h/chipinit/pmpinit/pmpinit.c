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
  * @file      pmpinit.c
  * @author    MCU Driver Team
  * @brief     pmpinit modlue.
  * @details   Physical Memory Protection (PMP) feature initialization configuration
  */
#include "interrupt.h"
#include "pmpinit.h"

#define FLASH_PMP_START_ADDRESS         0x3000000U

#ifndef DISABLE_RESERVE_SRAM_PMP
/**
 * @brief Configure the physical memory protection settings for the register area.
 * @param None.
 * @retval None
 */
static void ConfigPmpForRegister(void)
{
    /* Physical Memory Protection Config. */
    PMP_PreviousProtectInfo info;
    PMP_TargetProtectArea targetArea;
    /* Config the pmp for the register. */
    BASE_StatusType ret = HAL_PMP_GetLastProtectAreaAddr(&info);
    if (ret != BASE_STATUS_OK) {
        return;
    }
    targetArea.startAddr = (unsigned int)0x04040000;
    targetArea.indexPmp = info.indexPmp;
    targetArea.endAddr = (unsigned int)0x1C0E1000;
    targetArea.targetZonePermission = PMP_READ | PMP_WRITE; /* Enable read and execute. */
    HAL_PMP_ConfigProtectArea(info.protectEndAddr, PMP_NONE, targetArea);
}

/**
 * @brief Configure the physical memory protection settings for the reserved SRAM area.
 * @param None.
 * @retval None
 */
static void ConfigPmpForReserveSram(void)
{
    /* Physical Memory Protection Config. */
    PMP_PreviousProtectInfo info;
    PMP_TargetProtectArea targetArea;
    /* Config the pmp for the reserve sram memory(for can module). */
    if ((unsigned int)(uintptr_t)(void *)(&g_shareMemoryEndLds) < 0x04040000) {
        BASE_StatusType ret = HAL_PMP_GetLastProtectAreaAddr(&info);
        if (ret != BASE_STATUS_OK) {
            return;
        }
        targetArea.startAddr = (unsigned int)(uintptr_t)(void *)(&g_shareMemoryEndLds);
        targetArea.indexPmp = info.indexPmp;
        targetArea.endAddr = 0x04040000;
        targetArea.targetZonePermission = PMP_ALL;  /* Enable read and execute. */
        HAL_PMP_ConfigProtectArea(info.protectEndAddr, PMP_NONE, targetArea);
    }
}
#endif

/**
 * @brief Configure the physical memory protection settings for the shared memory area.
 * @param None.
 * @retval None
 */
static void ConfigPmpForShareMemory(void)
{
    /* Physical Memory Protection Config. */
    PMP_PreviousProtectInfo info;
    PMP_TargetProtectArea targetArea;
    /* Config the pmp for the share memory. */
    if (((unsigned int)(uintptr_t)(void *)(&g_shareMemoryStartLds) < \
         (unsigned int)(uintptr_t)(void *)(&g_shareMemoryEndLds)) &&
         ((unsigned int)(uintptr_t)(void *)(&g_shareMemoryStartLds) >= \
         (unsigned int)(uintptr_t)(void *)(&g_dtcmEndLds))) {
        BASE_StatusType ret = HAL_PMP_GetLastProtectAreaAddr(&info);
        if (ret != BASE_STATUS_OK) {
            return;
        }
        targetArea.startAddr = (unsigned int)(uintptr_t)(void *)(&g_shareMemoryStartLds);
        targetArea.indexPmp = info.indexPmp;
        targetArea.endAddr = (unsigned int)(uintptr_t)(void *)(&g_shareMemoryEndLds);
        targetArea.targetZonePermission = PMP_ALL; /* Enable read and execute. */
        HAL_PMP_ConfigProtectArea(info.protectEndAddr, PMP_NONE, targetArea);
    }
}

/**
 * @brief Configure the physical memory protection settings for the DTCM area.
 * @param None.
 * @retval None
 */
static void ConfigPmpForDtcm(void)
{
    /* Physical Memory Protection Config. */
    PMP_PreviousProtectInfo info;
    PMP_TargetProtectArea targetArea;
    /* Config the pmp for the DTCM. */
    BASE_StatusType ret = HAL_PMP_GetLastProtectAreaAddr(&info);
    if (ret != BASE_STATUS_OK) {
        return;
    }
    targetArea.startAddr = (unsigned int)(uintptr_t)(void *)(&g_dtcmStartLds);
    targetArea.indexPmp = info.indexPmp;
    targetArea.endAddr = (unsigned int)(uintptr_t)(void *)(&g_dtcmEndLds);
    if (targetArea.startAddr >= targetArea.endAddr) {
        return;
    }
    targetArea.targetZonePermission = PMP_READ | PMP_WRITE; /* Enable read and execute. */
    HAL_PMP_ConfigProtectArea(info.protectEndAddr, PMP_NONE, targetArea);
}

/**
 * @brief Configure the physical memory protection settings for the ITCM area.
 * @param None.
 * @retval None
 */
static void ConfigPmpForItcm(void)
{
    /* Physical Memory Protection Config. */
    PMP_PreviousProtectInfo info;
    PMP_TargetProtectArea targetArea;
    /* Config the pmp for the ITCM. */
    BASE_StatusType ret = HAL_PMP_GetLastProtectAreaAddr(&info);
    if (ret != BASE_STATUS_OK) {
        return;
    }
    targetArea.startAddr = (unsigned int)(uintptr_t)(void *)(&g_itcmStartLds);
    targetArea.indexPmp = info.indexPmp;
    targetArea.endAddr = (unsigned int)(uintptr_t)(void *)(&g_itcmEndLds);
    if (targetArea.startAddr >= targetArea.endAddr) {
        return;
    }
    targetArea.targetZonePermission = PMP_READ | PMP_EXEC; /* Enable read and execute. */
    HAL_PMP_ConfigProtectArea(info.protectEndAddr, PMP_NONE, targetArea);
}

/**
 * @brief Configure the physical memory protection settings for the Flash area.
 * @param None.
 * @retval None
 */
static void ConfigPmpForFlash(void)
{
    /* Physical Memory Protection Config. */
    PMP_PreviousProtectInfo info;
    PMP_TargetProtectArea targetArea;
    /* Config the pmp for the flash. */
    BASE_StatusType ret = HAL_PMP_GetLastProtectAreaAddr(&info);
    if (ret != BASE_STATUS_OK) {
        return;
    }
    targetArea.startAddr = info.protectEndAddr;
    targetArea.indexPmp = info.indexPmp;
    targetArea.endAddr = 0x3C00000; /* All flash region permission is consistent. */
    targetArea.targetZonePermission = PMP_READ | PMP_EXEC; /* Enable read and execute. */
    HAL_PMP_ConfigProtectArea(info.protectEndAddr, PMP_NONE, targetArea);
}

/**
 * @brief Configure the physical memory protection settings for the area before Flash.
 * @param None.
 * @retval None
 */
static void ConfigPmpForbeforeFlashArea(void)
{
    /* Physical Memory Protection Config. */
    PMP_PreviousProtectInfo info;
    PMP_TargetProtectArea targetArea;
    /* Config the pmp for the area before flash, all permissions in the reserved area are disable. */
    BASE_StatusType ret = HAL_PMP_GetLastProtectAreaAddr(&info);
    if (ret != BASE_STATUS_OK) {
        return;
    }
    targetArea.startAddr = info.protectEndAddr;
    targetArea.indexPmp = info.indexPmp;
    targetArea.endAddr = FLASH_PMP_START_ADDRESS;
    targetArea.targetZonePermission = PMP_NONE; /* Disable all permission. */
    HAL_PMP_ConfigProtectArea(info.protectEndAddr, PMP_NONE, targetArea);
}

/**
 * @brief Configure the physical memory protection settings for different memory regions.
 * This function sets up the protection zones for Flash, ITCM, DTCM, shared memory, and registers.
 * Each region is configured with specific permissions to ensure secure memory access.
 * @param None.
 * @retval None
 */
void ConfigPhysicalMemoryProtect(void)
{
    /* Config the pmp for the area before flash, all permissions in the reserved area are disable. */
    ConfigPmpForbeforeFlashArea();
    /* Config the pmp for the flash. */
    ConfigPmpForFlash();
    /* Config the pmp for the ITCM. */
    ConfigPmpForItcm();
    /* Config the pmp for the DTCM. */
    ConfigPmpForDtcm();
    /* Config the pmp for the share memory. */
    ConfigPmpForShareMemory();
#ifndef DISABLE_RESERVE_SRAM_PMP
    /* Config the pmp for the reserve sram memory(for can module). */
    ConfigPmpForReserveSram();
    ConfigPmpForRegister();
#endif
    WRITE_CSR(mseccfg, 0x0); /* Lock PMP protection to prevent application layer modifications. */
}
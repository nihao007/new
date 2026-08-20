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
  * @file    pmp.c
  * @author  MCU Driver Team.
  * @brief   PMP module driver.
  *          This file provides functions declaration of Physical Memory Protection.
  *           + PMP's initialization and de-initialization functions.
  */

#include "interrupt.h"
#include "pmp.h"

#define PMP_CONFIG_MASK                  0x18
#define PMP_ADDR_LOCK_MASK               0x08
#define PMP_RGE_CONFIG_NUM               0x04
#define PMP_REG_CONFIG_OFFSET_BIT        8
#define PMP_TOP_ADDR_LOCK_ENABLE         0x88
#define PMP_REG_CONFIG_NUM               4

#define PMP_INDEX_MASK_ZERO              0
#define PMP_INDEX_MASK_ONE               1
#define PMP_INDEX_MASK_TWO               2
#define PMP_INDEX_MASK_THREE             3

#define PMP_PEOTECT_ADDR_OFFSET          2

/**
 * @brief Find the first available PMP configuration index.
 * @param pmpcfg The PMP configuration register value.
 * @retval The index of the first available PMP configuration.
 */
static unsigned int GetAvailablePmpCfg(unsigned int pmpcfg)
{
    unsigned int rightCfg;
    unsigned int index;
    for (index = 0; index < PMP_RGE_CONFIG_NUM; index++) {
        rightCfg = (pmpcfg >> (index * PMP_REG_CONFIG_OFFSET_BIT)) & PMP_CONFIG_MASK;
        if (rightCfg == PMP_ADDR_LOCK_MASK) {
            continue; /* Has been used. */
        } else {
            break;
        }
    }
    return index; /* Available indexes. */
}

/**
 * @brief Get the target PMP group 0 address based on the given index.
 * @param indexPmp The index of the PMP region (0-3).
 * @retval The target address for the specified PMP region in group 0.
 */
static unsigned int GetTargetPmpGroup0Addr(unsigned int indexPmp)
{
    unsigned int retAddr = 0;
    switch (indexPmp) {
        case PMP_INDEX_MASK_ZERO:
            retAddr = READ_CSR(pmpaddr0); /*  Read address from pmpaddr0 */
            break;
        case PMP_INDEX_MASK_ONE:
            retAddr = READ_CSR(pmpaddr1);
            break;
        case PMP_INDEX_MASK_TWO:
            retAddr = READ_CSR(pmpaddr2); /*  Read address from pmpaddr2 */
            break;
        case PMP_INDEX_MASK_THREE:
            retAddr = READ_CSR(pmpaddr3);
            break;
        default:
            break;
    }
    return (retAddr << PMP_PEOTECT_ADDR_OFFSET); /* Return the address */
}

/**
 * @brief Get the target PMP group 1 address based on the given index.
 * @param indexPmp The index of the PMP region (0-3).
 * @retval The target address for the specified PMP region in group 1.
 */
static unsigned int GetTargetPmpGroup1Addr(unsigned int indexPmp)
{
    unsigned int retAddr = 0;
    switch (indexPmp) {
        case PMP_INDEX_MASK_ZERO:
            retAddr = READ_CSR(pmpaddr4); /*  Read address from pmpaddr4 */
            break;
        case PMP_INDEX_MASK_ONE:
            retAddr = READ_CSR(pmpaddr5);
            break;
        case PMP_INDEX_MASK_TWO:
            retAddr = READ_CSR(pmpaddr6); /*  Read address from pmpaddr6 */
            break;
        case PMP_INDEX_MASK_THREE:
            retAddr = READ_CSR(pmpaddr7);
            break;
        default:
            break;
    }
    return (retAddr << PMP_PEOTECT_ADDR_OFFSET); /* Return the address */
}

/**
 * @brief Get the target PMP group 2 address based on the given index.
 * @param indexPmp The index of the PMP region (0-3).
 * @retval The target address for the specified PMP region in group 2.
 */
static unsigned int GetTargetPmpGroup2Addr(unsigned int indexPmp)
{
    unsigned int retAddr = 0;
    switch (indexPmp) {
        case PMP_INDEX_MASK_ZERO:
            retAddr = READ_CSR(pmpaddr8); /* Set address for PMP region */
            break;
        case PMP_INDEX_MASK_ONE:
            retAddr = READ_CSR(pmpaddr9); /* Set address for PMP region */
            break;
        case PMP_INDEX_MASK_TWO:
            retAddr = READ_CSR(pmpaddr10); /* Set address for PMP region */
            break;
        case PMP_INDEX_MASK_THREE:
            retAddr = READ_CSR(pmpaddr11); /* Set address for PMP region */
            break;
        default:
            break;
    }
    return (retAddr << PMP_PEOTECT_ADDR_OFFSET); /* Return the address */
}

/**
 * @brief Get the target PMP group 3 address based on the given index.
 * @param indexPmp The index of the PMP region (0-3).
 * @retval The target address for the specified PMP region in group 3.
 */
static unsigned int GetTargetPmpGroup3Addr(unsigned int indexPmp)
{
    unsigned int retAddr = 0;
    switch (indexPmp) {
        case PMP_INDEX_MASK_ZERO:
            retAddr = READ_CSR(pmpaddr12); /* Set address for PMP region */
            break;
        case PMP_INDEX_MASK_ONE:
            retAddr = READ_CSR(pmpaddr14); /* Set address for PMP region */
            break;
        case PMP_INDEX_MASK_TWO:
            retAddr = READ_CSR(pmpaddr15); /* Set address for PMP region */
            break;
        case PMP_INDEX_MASK_THREE:
            retAddr = READ_CSR(pmpaddr16); /* Set address for PMP region */
            break;
        default:
            break;
    }
    return (retAddr << PMP_PEOTECT_ADDR_OFFSET); /* Return the address */
}

/**
 * @brief Set the target PMP group 0 address for the specified region.
 * @param indexPmp The index of the PMP region (PMP_REGION_NUM0 to PMP_REGION_NUM3).
 * @param addr The address to set for the specified PMP region.
 * @retval None.
 */
static void SetTargetPmpGroup0Addr(PMP_RegionNum indexPmp, unsigned int addr)
{
    switch (indexPmp) {
        case PMP_REGION_NUM0:
            WRITE_CSR(pmpaddr0, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM1:
            WRITE_CSR(pmpaddr1, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM2:
            WRITE_CSR(pmpaddr2, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM3:
            WRITE_CSR(pmpaddr3, addr); /* Set address for PMP region */
            break;
        default:
            break;
    }
}

/**
 * @brief Set the target PMP group 1 address for the specified region.
 * @param indexPmp The index of the PMP region (PMP_REGION_NUM4 to PMP_REGION_NUM7).
 * @param addr The address to set for the specified PMP region.
 * @retval None.
 */
static void SetTargetPmpGroup1Addr(PMP_RegionNum indexPmp, unsigned int addr)
{
    switch (indexPmp) {
        case PMP_REGION_NUM4:
            WRITE_CSR(pmpaddr4, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM5:
            WRITE_CSR(pmpaddr5, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM6:
            WRITE_CSR(pmpaddr6, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM7:
            WRITE_CSR(pmpaddr7, addr); /* Set address for PMP region */
            break;
        default:
            break;
    }
}

/**
 * @brief Set the target PMP group 2 address for the specified region.
 * @param indexPmp The index of the PMP region (PMP_REGION_NUM8 to PMP_REGION_NUM11).
 * @param addr The address to set for the specified PMP region.
 * @retval None.
 */
static void SetTargetPmpGroup2Addr(PMP_RegionNum indexPmp, unsigned int addr)
{
    switch (indexPmp) {
        case PMP_REGION_NUM8:
            WRITE_CSR(pmpaddr8, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM9:
            WRITE_CSR(pmpaddr9, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM10:
            WRITE_CSR(pmpaddr10, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM11:
            WRITE_CSR(pmpaddr11, addr); /* Set address for PMP region */
            break;
        default:
            break;
    }
}

/**
 * @brief Set the target PMP group 3 address for the specified region.
 * @param indexPmp The index of the PMP region (PMP_REGION_NUM12 to PMP_REGION_NUM15).
 * @param addr The address to set for the specified PMP region.
 * @retval None.
 */
static void SetTargetPmpGroup3Addr(PMP_RegionNum indexPmp, unsigned int addr)
{
    switch (indexPmp) {
        case PMP_REGION_NUM12:
            WRITE_CSR(pmpaddr12, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM13:
            WRITE_CSR(pmpaddr13, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM14:
            WRITE_CSR(pmpaddr14, addr); /* Set address for PMP region */
            break;
        case PMP_REGION_NUM15:
            WRITE_CSR(pmpaddr15, addr); /* Set address for PMP region */
            break;
        default:
            break;
    }
}

/**
 * @brief Select and configure the PMP address and permissions for the specified region.
 * @param indexPmp The index of the PMP region.
 * @param addr The address to set for the specified PMP region.
 * @param permission The permissions to apply to the specified PMP region.
 * @retval None.
 */
static void SelectPmpAddrConfigFunc(PMP_RegionNum indexPmp, unsigned int addr, PMP_Permissions permission)
{
    unsigned int targetIndex = indexPmp % PMP_REG_CONFIG_NUM;
    unsigned int setRegAddr = addr >> PMP_PEOTECT_ADDR_OFFSET;
    unsigned int rightReg;
    if (indexPmp <= PMP_REGION_NUM3) {
        SetTargetPmpGroup0Addr(indexPmp, setRegAddr); /* Set the address for the first PMP configuration group. */
        rightReg = READ_CSR(pmpcfg0);
        rightReg |= (permission | PMP_TOP_ADDR_LOCK_ENABLE) << (PMP_REG_CONFIG_OFFSET_BIT * targetIndex);
        WRITE_CSR(pmpcfg0, rightReg);
    } else if (indexPmp <= PMP_REGION_NUM7) {
        SetTargetPmpGroup1Addr(indexPmp, setRegAddr); /* Set the address for the second PMP configuration group. */
        rightReg = READ_CSR(pmpcfg1);
        rightReg |= (permission | PMP_TOP_ADDR_LOCK_ENABLE) << (PMP_REG_CONFIG_OFFSET_BIT * targetIndex);
        WRITE_CSR(pmpcfg1, rightReg);
    } else if (indexPmp <= PMP_REGION_NUM11) {
        SetTargetPmpGroup2Addr(indexPmp, setRegAddr); /* Set the address for the third PMP configuration group. */
        rightReg = READ_CSR(pmpcfg2);
        rightReg |= (permission | PMP_TOP_ADDR_LOCK_ENABLE) << (PMP_REG_CONFIG_OFFSET_BIT * targetIndex);
        WRITE_CSR(pmpcfg2, rightReg);
    } else if (indexPmp <= PMP_REGION_NUM15) {
        SetTargetPmpGroup3Addr(indexPmp, setRegAddr); /* Set the address for the fourth PMP configuration group. */
        rightReg = READ_CSR(pmpcfg3);
        rightReg |= (permission | PMP_TOP_ADDR_LOCK_ENABLE) << (PMP_REG_CONFIG_OFFSET_BIT * targetIndex);
        WRITE_CSR(pmpcfg3, rightReg); /* Write the updated value back to pmpcfg3. */
    }
}

/**
 * @brief Configure a physical memory protection area.
 * @param lastProtectAddr The address of the last protected area.
 * @param skipZonePermission The permissions to apply to the skip zone.
 * @param targetArea The target protection area to configure.
 * @retval BASE_StatusType: OK if successful, ERROR if the start address is invalid.
 */
BASE_StatusType HAL_PMP_ConfigProtectArea(unsigned int lastProtectAddr, PMP_Permissions skipZonePermission,
                                          PMP_TargetProtectArea targetArea)
{
    if (lastProtectAddr > targetArea.startAddr) {
        return BASE_STATUS_ERROR;
    }

    /* Consume 1 PMP protection to skip the area. */
    if ((lastProtectAddr) != targetArea.startAddr) {
        SelectPmpAddrConfigFunc(targetArea.indexPmp, targetArea.startAddr, skipZonePermission);
        SelectPmpAddrConfigFunc((targetArea.indexPmp + 1), targetArea.endAddr, targetArea.targetZonePermission);
    } else {
        SelectPmpAddrConfigFunc(targetArea.indexPmp, targetArea.endAddr, targetArea.targetZonePermission);
    }
    return BASE_STATUS_OK;
}

static BASE_StatusType GetLastProtectAreaAddrPmpCfg3(PMP_PreviousProtectInfo *info)
{
    unsigned int index;
    unsigned int addrOffset = 0;
    unsigned int pmpRetIndex = 0xFFFFFFFF;
    unsigned int pmpCfgReg;
    /* Get the PMP that is available in pmpcfg3. */
    pmpCfgReg =  READ_CSR(pmpcfg3); /* Read the fourth PMP configuration register */
    index = GetAvailablePmpCfg(pmpCfgReg);
    if (index < PMP_REG_CONFIG_NUM) {
        pmpRetIndex = index + (addrOffset * PMP_REG_CONFIG_NUM);
        info->indexPmp = pmpRetIndex;
        if (index == 0) {
            info->protectEndAddr = GetTargetPmpGroup2Addr(PMP_INDEX_MASK_THREE); /* Set the protection end address. */
        } else {
            info->protectEndAddr = GetTargetPmpGroup3Addr(index - 1); /* Set the protection end address. */
        }
        return BASE_STATUS_OK;
    }
    return BASE_STATUS_ERROR;
}

/**
 * @brief Get the last protected area's address information.
 * @param info A pointer to a structure where the index and end address will be stored.
 * @retval BASE_StatusType: OK if successful, ERROR if no PMP configurations are available.
 */
BASE_StatusType HAL_PMP_GetLastProtectAreaAddr(PMP_PreviousProtectInfo *info)
{
    unsigned int index;
    unsigned int addrOffset = 0;
    unsigned int pmpRetIndex = 0xFFFFFFFF;
    unsigned int pmpCfgReg;
    /* Get the PMP that is available in pmpcfg0. */
    pmpCfgReg = READ_CSR(pmpcfg0); /* Read the fourth PMP configuration register */
    index = GetAvailablePmpCfg(pmpCfgReg);
    if (index < PMP_REG_CONFIG_NUM) {
        pmpRetIndex = index + (addrOffset * PMP_REG_CONFIG_NUM);
        info->indexPmp = pmpRetIndex;
        index = (index == 0) ? index : (index - 1);
        info->protectEndAddr = GetTargetPmpGroup0Addr(index); /* Set the protection end address. */
        return BASE_STATUS_OK;
    }
    addrOffset++;
    /* Get the PMP that is available in pmpcfg1. */
    pmpCfgReg = READ_CSR(pmpcfg1); /* Read the fourth PMP configuration register */
    index = GetAvailablePmpCfg(pmpCfgReg);
    if (index < PMP_REG_CONFIG_NUM) {
        pmpRetIndex = index + (addrOffset * PMP_REG_CONFIG_NUM);
        info->indexPmp = pmpRetIndex;
        if (index == 0) {
            info->protectEndAddr = GetTargetPmpGroup0Addr(PMP_INDEX_MASK_THREE); /* Set the protection end address. */
        } else {
            info->protectEndAddr = GetTargetPmpGroup1Addr(index - 1); /* Set the protection end address. */
        }
        return BASE_STATUS_OK;
    }
    addrOffset++;
    /* Get the PMP that is available in pmpcfg2. */
    pmpCfgReg = READ_CSR(pmpcfg2); /* Read the fourth PMP configuration register */
    index = GetAvailablePmpCfg(pmpCfgReg);
    if (index < PMP_REG_CONFIG_NUM) {
        pmpRetIndex = index + (addrOffset * PMP_REG_CONFIG_NUM);
        info->indexPmp = pmpRetIndex;
        if (index == 0) {
            info->protectEndAddr = GetTargetPmpGroup1Addr(PMP_INDEX_MASK_THREE); /* Set the protection end address. */
        } else {
            info->protectEndAddr = GetTargetPmpGroup2Addr(index - 1); /* Set the protection end address. */
        }
        return BASE_STATUS_OK;
    }
    addrOffset++;
    /* Get the PMP that is available in pmpcfg3. */
    return GetLastProtectAreaAddrPmpCfg3(info);
}
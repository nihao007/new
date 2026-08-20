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
  * @file    pmp.h
  * @author  MCU Driver Team.
  * @brief   PMP module driver.
  *          This file provides functions declaration of Physical Memory Protection.
  *           + PMP's initialization and de-initialization functions.
  */

#ifndef __McuMagicTag_PMP_H__
#define __McuMagicTag_PMP_H__

#include "baseinc.h"

/**
  * @defgroup PMP PMP
  * @brief PMP module.
  * @{
  */

/**
  * @defgroup PMP_Common PMP Common
  * @brief PMP common external module.
  * @{
  */


/**
  * @defgroup PMP_Common_Param PMP Common Parameters
  * @{
  */

typedef enum {
    PMP_NONE     = 0x00,
    PMP_READ     = 0x01,
    PMP_WRITE    = 0x02,
    PMP_EXEC     = 0x04,
    PMP_ALL      = PMP_READ | PMP_WRITE | PMP_EXEC,
} PMP_Permissions;

typedef enum {
    PMP_REGION_NUM0 = 0,
    PMP_REGION_NUM1 = 1,
    PMP_REGION_NUM2 = 2,
    PMP_REGION_NUM3 = 3,
    PMP_REGION_NUM4 = 4,
    PMP_REGION_NUM5 = 5,
    PMP_REGION_NUM6 = 6,
    PMP_REGION_NUM7 = 7,
    PMP_REGION_NUM8 = 8,
    PMP_REGION_NUM9 = 9,
    PMP_REGION_NUM10 = 10,
    PMP_REGION_NUM11 = 11,
    PMP_REGION_NUM12 = 12,
    PMP_REGION_NUM13 = 13,
    PMP_REGION_NUM14 = 14,
    PMP_REGION_NUM15 = 15,
} PMP_RegionNum;

typedef struct {
    unsigned int startAddr;
    unsigned int endAddr;
    PMP_Permissions targetZonePermission;
    PMP_RegionNum indexPmp;
} PMP_TargetProtectArea;

typedef struct {
    unsigned int protectEndAddr;
    PMP_RegionNum indexPmp;
} PMP_PreviousProtectInfo;

/**
  * @}
  */

/**
  * @defgroup PMP_API_Declaration PMP HAL API
  * @{
  */
BASE_StatusType HAL_PMP_ConfigProtectArea(unsigned int preProtectEndAddr, PMP_Permissions skipZonePermission,
                                          PMP_TargetProtectArea targetArea);
BASE_StatusType HAL_PMP_GetLastProtectAreaAddr(PMP_PreviousProtectInfo *info);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif
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
  * @file      info.h
  * @author    MCU Driver Team
  * @brief     Defines chip attributes.
  */

/* Define to prevent recursive inclusion ------------------------------------- */
#ifndef McuMagicTag_INFO_H
#define McuMagicTag_INFO_H

#define CHIP_DELAY_CYCLES_PER_LOOP (4) /**< CPU cycles. Known number of this CPU cycles required to execute the \
                                           BASE_FUNC_delay() loop. */
#define CHIP_CORE_MAX              (3)

typedef enum {
    CHIP_CORE_0 = 0x00000001u,
    CHIP_CORE_1 = 0x00000002u,
    CHIP_CORE_2 = 0x00000004u,
    CHIP_CORE_MASK = 0x00000007u
} ChipCoreId;

typedef enum {
    CORE_STATUS_ACTIVE = 0x00000001u,
    CORE_STATUS_IDLE = 0x00000002u,
    CORE_STATUS_SUSPEND = 0x00000003u,
    CORE_STATUS_RESUME = 0x00000004u,
    CORE_STATUS_SHUTDOWN = 0x00000005u,
    CORE_STATUS_MAX
} CoreStatus;

/**
  * @brief Check core type whether is right.
  * @param coreType chip core id, data logic and or.
  * @retval bool True or false.
  */
static inline _Bool IsChipCoreId(ChipCoreId coreIdType)
{
    return ((coreIdType & CHIP_CORE_MASK) == coreIdType);
}

/**
  * @brief Get chip core id from cpu id, cpu core0 = 0, cpu core1 = 1, cpu core2 = 2.
  * @param coreType chip core id, data logic and or.
  * @retval chip id @ref ChipCoreId.
  */
static inline ChipCoreId GetChipCoreId(void)
{
    unsigned int cpuId;
    asm volatile ("csrr %0, mhartid" : "=r"(cpuId));
    return (1 << cpuId);     /* 1 : cpu id conver to chip core id */
}


#endif /* McuMagicTag_INFO_H */
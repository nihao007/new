/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2025. All rights reserved.
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
  * @file      mcs_mtpa_tables.h
  * @author    MCU Algorithm Team
  * @brief     This file provides the definition of motor MTPA parameter table.
  */
#ifndef McuMagicTag_MCS_MTPA_TABLES_H
#define McuMagicTag_MCS_MTPA_TABLES_H

#define MTPA_LUT_POINT 5
#define INDUCTANCE_LUT_POINT 3
/* compressor MTPA TYPE2 calibration method current-angle table */
extern const float g_mtpaIsAngleLut[2][MTPA_LUT_POINT];
/* compressor MTPA TYPE1 formula current-inductance table */
extern const float g_isIndLut[INDUCTANCE_LUT_POINT][INDUCTANCE_LUT_POINT];

/* Table look-up method for MTPA TYPE2 of wind turbine:
 * query the current dq current distribution angle according to the current vector magnitude.
 */
extern const float g_mtpaIsAngleLutFan[2][MTPA_LUT_POINT];

/* Fan MTPA TYPE1 formula method:
 * Query the Ld Lq inductance table based on the current and use the inductance parameters to calculate the mtpa.
 */
extern const float g_isIndLutFan[INDUCTANCE_LUT_POINT][INDUCTANCE_LUT_POINT];

/* FAN2 */
extern const float g_mtpaIsAngleLutFan2[2][MTPA_LUT_POINT];
extern const float g_isIndLutFan2[INDUCTANCE_LUT_POINT][INDUCTANCE_LUT_POINT];
#endif
/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2024-2025. All rights reserved.
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
  * @file      mcs_grid_pll.h
  * @author    MCU Algorithm Team
  * @brief     This file provides functions declaration for grid pll(Single-phase PLL).
  */

#ifndef McuMagicTag_MCS_GRID_PLL_H
#define McuMagicTag_MCS_GRID_PLL_H

#include "mcs_sogi.h"
#include "mcs_pid_ctrl.h"
#include "mcs_math_const.h"

#define PFC_RECT_BUS_FREQ    100.0f  /* Rectified voltage frequency without control, Hz */
#define GRID_PLL_WFF      (PFC_RECT_BUS_FREQ * ONE_PI)

typedef struct {
    /* Parameters. */

    /* Inner variables. */
    float sigRecon;
    SOGI_Handle sogi;

    /* Outputs. */
    float phs;
    float w;
} GRID_PLL_Handle;

void GRID_PLL_Init(GRID_PLL_Handle *p, PI_Param *piParam, float ts, float gain);
void GRID_PLL_Clear(GRID_PLL_Handle *p);
void GRID_PLL_Exec(GRID_PLL_Handle *p, float sig);

#endif
/**
  * @copyright Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2026-2026. All rights reserved.
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
  * @file      mcs_bemfobs_ii.h
  * @author    MCU Algorithm Team
  * @brief     This file provides function API declarations of bemf observer ii.
  */

/* Define to prevent recursive inclusion ------------------------------------------------------- */
#ifndef McuMagicTag_MCS_BEMFOBS_II_H
#define McuMagicTag_MCS_BEMFOBS_II_H

/* Includes ------------------------------------------------------------------------------------ */
#include "mcs_typedef.h"
#include "mcs_mtr_param.h"
#include "mcs_pll.h"
#include "mcs_lpfRk4.h"

/* Macros -------------------------------------------------------------------------------------- */
/* Default limit value for estimated variables, can only be modified by API: BEMFOBS_II_SetEstLimit. */
#define BEMFOBS_II_CURR_EST_LIMIT_DEFAULT      100.0f
#define BEMFOBS_II_BEMF_EST_LIMIT_DEFAULT      1000.0f

/**
  * @brief Bemf observer ii data struct definition.
  */
typedef struct {
    MOTOR_Param        *mtrParam;     /* Motor parameters. */
    /* Variables */
    float              ts;            /* Control period (s). */
    float              gain;          /* Observer gain. */
    float              pllBdw;        /* PLL bandwidth (Hz). */
    float              spdFc;         /* Cutoff frequency for speed filter (Hz). */

    float              currEstLimit;
    float              bemfEstLimit;
    AlbeAxis           ialbeEst;
    AlbeAxis           ealbeEst;
    /* PLL */
    PLL_Handle         pll;
    /* Speed filter */
    LPF_RK4_Handle     spdFilter;
    /* Output */
    float              elecAngle;     /* Estimated rotor angle (rad). */
    float              elecSpeed;     /* Estimated rotor speed (Hz). */
} BEMFOBS_II_Handle;

/* Function API declarations ------------------------------------------------------------------- */
void BEMFOBS_II_Init(BEMFOBS_II_Handle *obs, MOTOR_Param *mtrParam, float gain, float pllBdw, float spdFc, float ts);
void BEMFOBS_II_Clear(BEMFOBS_II_Handle *obs);
RAM_CODE void BEMFOBS_II_Exec(BEMFOBS_II_Handle *obs, AlbeAxis *ialbeFbk, AlbeAxis *valbeRef);

RAM_CODE void BEMFOBS_II_SetObsGain(BEMFOBS_II_Handle *obs, float gain);
RAM_CODE void BEMFOBS_II_SetPllBdw(BEMFOBS_II_Handle *obs, float pllBdw);
RAM_CODE void BEMFOBS_II_SetSpdFc(BEMFOBS_II_Handle *obs, float spdFc);
RAM_CODE void BEMFOBS_II_SetTs(BEMFOBS_II_Handle *obs, float ts);
RAM_CODE void BEMFOBS_II_SetEstLimit(BEMFOBS_II_Handle *obs, float currEstLim, float bemfEstLim);

#endif
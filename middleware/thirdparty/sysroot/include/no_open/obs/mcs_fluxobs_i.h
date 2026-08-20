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
  * @file      mcs_fluxobs_i.h
  * @author    MCU Algorithm Team
  * @brief     This file provides function API declarations of flux observer i.
  */

/* Define to prevent recursive inclusion ------------------------------------------------------- */
#ifndef McuMagicTag_MCS_FLUXOBS_I_H
#define McuMagicTag_MCS_FLUXOBS_I_H

/* Includes ------------------------------------------------------------------------------------ */
#include "mcs_typedef.h"
#include "mcs_mtr_param.h"
#include "mcs_pll.h"
#include "mcs_filter.h"

/* Macros -------------------------------------------------------------------------------------- */
/* Default limit value for estimated variables, can only be modified by API: FLUXOBS_I_SetEstLimit. */
#define FLUXOBS_I_FLUX_EST_LIMIT_DEFAULT      100.0f

/**
  * @brief Flux observer i data struct definition.
  */
typedef struct {
    MOTOR_Param        *mtrParam;     /* Motor parameters. */
    /* Variables */
    float              ts;            /* Control period (s). */
    float              gain;          /* Observer gain. */
    float              pllBdw;        /* PLL bandwidth (Hz). */
    float              spdFc;         /* Cutoff frequency for speed filter (Hz). */

    float              fluxEstLimit;
    AlbeAxis           cFlux;
    AlbeAxis           vFlux;
    /* PLL */
    PLL_Handle         pll;
    /* Speed filter */
    FOFLT_Handle       spdFilter;
    /* Output */
    float              elecAngle;     /* Estimated rotor angle (rad). */
    float              elecSpeed;     /* Estimated rotor speed (Hz). */
} FLUXOBS_I_Handle;

/* Function API declarations ------------------------------------------------------------------- */
void FLUXOBS_I_Init(FLUXOBS_I_Handle *obs, MOTOR_Param *mtrParam, float gain, float pllBdw, float spdFc, float ts);
void FLUXOBS_I_Clear(FLUXOBS_I_Handle *obs);
RAM_CODE void FLUXOBS_I_Exec(FLUXOBS_I_Handle *obs, AlbeAxis *ialbeFbk, AlbeAxis *valbeRef);

RAM_CODE void FLUXOBS_I_SetObsGain(FLUXOBS_I_Handle *obs, float gain);
RAM_CODE void FLUXOBS_I_SetPllBdw(FLUXOBS_I_Handle *obs, float pllBdw);
RAM_CODE void FLUXOBS_I_SetSpdFc(FLUXOBS_I_Handle *obs, float spdFc);
RAM_CODE void FLUXOBS_I_SetTs(FLUXOBS_I_Handle *obs, float ts);
RAM_CODE void FLUXOBS_I_SetEstLimit(FLUXOBS_I_Handle *obs, float fluxEstLim);

#endif
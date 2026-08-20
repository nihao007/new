/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2024. All rights reserved.
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
  * @file      mcs_nl_flux_obs.h
  * @author    MCU Algorithm Team
  * @brief     This file provides functions declaration of nonlinear flux observer.
  */

/* Define to prevent recursive inclusion ------------------------------------------------------- */
#ifndef McuMagicTag_MCS_NL_FLUX_OBS_H
#define McuMagicTag_MCS_NL_FLUX_OBS_H

/* Includes ------------------------------------------------------------------------------------ */
#include "mcs_mtr_param.h"
#include "mcs_typedef.h"
#include "mcs_pll.h"
#include "mcs_filter.h"
#include "mcs_solpf.h"

/* Typedef definitions ------------------------------------------------------------------------- */
/**
  * @brief nonlinear flux observer data struct definition.
  */
typedef struct {
    MOTOR_Param  *mtrParam;     /**< Motor parameters. */
    float            ts;            /**< control period (s). */
    float            angleEst;      /**< estimated angle (rad). */
    float            x1Est;
    float            x2Est;
    float            spdEst;        /**< estimated electronic speed (Hz). */
    float            oneDivPsif;    /**< flux reciprocal. */
    AlbeAxis         fluxAlbe;      /**< alpha beta flux. */
    PLL_Handle       pll;           /**< PLL handle. */
    float spdEstFltFc;
    SOLPF_Handle spdSoFlt;
} NlFluxObs_Handle;

void FLUXOBS_Init(NlFluxObs_Handle *obs, MOTOR_Param *mtrParam, float ts, float bdw, float fc);
void FLUXOBS_Clear(NlFluxObs_Handle *obs);
void FLUXOBS_Exec(NlFluxObs_Handle *obs, AlbeAxis *ialbeFbk, AlbeAxis *valbeRef, float gamma);
void FLUXOBS_SetTs(NlFluxObs_Handle *obs, float ts);
#endif
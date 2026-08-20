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
  * @file      mcs_cogtrq.h
  * @author    MCU Algorithm Team
  * @brief     This file provides functions declaration of ladrc control.
  */
#ifndef McuMagicTag_MCS_COGTRQCOMP_H
#define McuMagicTag_MCS_COGTRQCOMP_H

#include "typedefs.h"
#include "mcs_pid_ctrl.h"

#define COMPPOINT 200
typedef struct {
    PID_Handle cogPi;               /* PI regulator coefficients for cogging torque compensation */
    float cogCompMap[COMPPOINT];    /* Generate cogging torque compensation lookup table (LUT) */
    float cogCompIq;                /* Q-Axis compensation current */
    float cogCompCalc;              /* PI-compensated current reference */
    float cogAngle;                 /* Electrical angle for cogging torque computation */
    float lastCogAngle;             /* Previous motor angle required for cogging force calculation */
    float motorSpeed;	              /* Motor speed required for cogging force calculation */
    float sampResolution;           /* Sampling resolution: dividing single electrical cycle into COMPPOINT segments */
    bool sampStartFlag;             /* Sampling start flag */
    bool compStartFlag;             /* Compensation start flag */
    bool sampFinishFlag;            /* Sampling finish flag */
    int sampPoint;		              /* Actual sampling points */
    int compPoint;
} COGTRQCOMP_Handle;

/* Cogging torque initialization */
void COGTRQCOMP_Init(COGTRQCOMP_Handle *cogTrqComp, PID_Handle *piParam, int point);
/* Cogging torque data acquisition */
void COGTRQCOMP_DataSamp(COGTRQCOMP_Handle *cogTrqComp, bool enableSamp, float angle, float speedError);
/* Cogging torque data compensation */
void COGTRQCOMP_Exec(COGTRQCOMP_Handle *cogTrqComp, bool enableComp, float angle);
void COGTRQCOMP_Clear(COGTRQCOMP_Handle *cogTrqComp);

#endif
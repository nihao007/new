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
  * @file     mcs_trq_comp_adpt.h
  * @author   MCU Algorithm Team
  * @brief    This file provides functions declaration.
*/
#ifndef McuMagicTag_MCS_TRQ_COMP_ADAT_H
#define McuMagicTag_MCS_TRQ_COMP_ADAT_H

#include "mcs_ramp_mgmt.h"
#include "mcs_pid_ctrl.h"
#include "mcs_assert.h"
#include "typedefs.h"

typedef struct {
    float spdAvgBuff[24];
    float iqAvgBuff[24];
    float iqMechPrdAvg;
    float accBuff[24];

    float elecTheta;
    float angleSecTheta;

    unsigned int spdAvgCalcCnt;
    int crsZeroFltWinCnt;
    int crsZeroFltWin;

    float accMinSet;
    float compTrqGain;
    float compTrqGainSet;
    float ts;
    PID_Handle pi;
    RMG_Handle trqGainRmg;

    float spdAvgCalcSum;
    float iqAvgCalcSum;
    float iqSlope;
    float iqComp;
    float iqCurr;
    float spdRipple;
    unsigned short accCalcSecNumSet;
    unsigned short mechAnglePosFlag;
    unsigned short secNumNow;
    unsigned short secNumSet;
    unsigned short polePair;
    signed short secShift;
    unsigned short secNumPrev;
    bool piIsRunning;
    bool enableFlag;
    bool enable;
    bool start;

    float outputLim;
} TRQCOMP_Handle;

typedef struct {
    float ts;
    float outputLim;
    float compTrqGain;
    unsigned short polePair;
    signed short secShift;
} TRQCOMP_Param;

void TRQCOMP_Init(TRQCOMP_Handle *comp, const PI_Param *piParam, TRQCOMP_Param compParam);
void TRQCOMP_Clear(TRQCOMP_Handle *comp);
void TRQCOMP_GetIqComp(TRQCOMP_Handle *comp, float elecTheta, float spdFbkRip, float iqFbk);
void TRQCOMP_SpdIntervalEnable(TRQCOMP_Handle *comp, float spdRef, float startSpd, float endSpd);

#endif
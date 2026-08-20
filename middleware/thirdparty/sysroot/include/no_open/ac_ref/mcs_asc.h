/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2023. All rights reserved.
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
  * @file      mcs_asc.h
  * @author    MCU Algorithm Team
  * @brief     Brake controller of motor control
  *            This file provides functions declaration of brake module.
  */
#ifndef McuMagicTag_MCS_AC_ASC_H
#define McuMagicTag_MCS_AC_ASC_H

#include "mcs_typedef.h"
#include "typedefs.h"

/* Typedef definitions ------------------------------------------------------------------------- */
/**
  * @brief Structure of the brake configuration parameter.
  */
typedef struct {
    float ts;                   /**< control period (s). */
    float brkTime;              /**< brake time (s). */
    float sampleWinTime;        /**< sample window times. */
    float maxBrkCurr;           /**< maximum brake current (A). */
    float minBrkDutyStep;       /**< small brake duty step. */
    float maxBrkDutyStep;       /**< large brake duty step. */
    float fastBrkCurrCoeff;     /**< current threshold coefficient for fast braking. */
    float openLoopBrkDutyStep;  /**< open-loop brake duty step, recommend */
    float openLoopBrkCurrCoeff; /**< current threshold coefficient for open-loop braking. */
} ASC_Param;

/**
  * @brief Brake Struct.
  */typedef struct {
    float brkDuty;              /**< pwm duty ratio of lower switch during brake condition (0~1). */
    float sampleShiftDuty;      /**< phase shift duty of sample point for brake current (0~1). */
    unsigned int tickCnt;       /**< counter for calculating brake time. */
    unsigned int tickNum;       /**< count number corresponding to brake time. */
    bool brkFinished;           /**< brake finish flag. */
    ASC_Param ascParam;
} ASC_Handle;

void ASC_Init(ASC_Handle *brake, ASC_Param ascParam);
void ASC_Clear(ASC_Handle *brake);
void ASC_Exec(ASC_Handle *brake, float brkCurr);

#endif  // McuMagicTag_ASC_H

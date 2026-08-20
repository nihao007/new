/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2024. All rights reserved.
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
  * @file     mcs_sohc.h
  * @author   MCU Algorithm Team
  * @brief    This file provides functions declaration for harmonic suppression function.
  */

#ifndef McuMagicTag_MCS_HOSC_H
#define McuMagicTag_MCS_HOSC_H

/* Includes ------------------------------------------------------------------------------------ */
#include "typedefs.h"
#include "mcs_typedef.h"
#include "mcs_assert.h"
#include "mcs_pid_ctrl.h"
#include "mcs_solpf.h"


#define  SOHC_ORDER_NEG_5  (-5)
#define  SOHC_ORDER7       (7)

/**
  * @brief Structure of the harmonic suppression controller.
  */
typedef struct {
    float lpfFc;
    float ang;
    SOLPF_Handle iqSoLpf;
    SOLPF_Handle idSoLpf;

    PID_Handle pidId;
    PID_Handle pidIq;
    float valComp;
    float vbeComp;
    float vd;
    float vq;
    bool enable;
    bool start;
} SOHC_Handle;

void SOHC_Init(SOHC_Handle *sohc, PID_Param *param, float ts, float fc);
void SOHC_SetKp(SOHC_Handle *sohc, float kp);
void SOHC_SetKi(SOHC_Handle *sohc, float ki);
void SOHC_Clear(SOHC_Handle *sohc);
void SOHC_Exec(SOHC_Handle *sohc, UvwAxis *iuvw, float ang, int order, bool enable);
#endif
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
  * @file      mcs_fw_ctrl.h
  * @author    MCU Algorithm Team
  * @brief     This file provides functions declaration of Flux-Weakening control.
  */
#ifndef McuMagicTag_MCS_MTPA_FW_CTRL_H
#define McuMagicTag_MCS_MTPA_FW_CTRL_H

#include "typedefs.h"
#include "mcs_typedef.h"
#include "mcs_pid_ctrl.h"
#include "mcs_spd_ctrl.h"
#include "mcs_filter.h"
#include "mcs_mtpa_tables.h"

typedef enum {
    MTPA_TYPE1 = 0,
    MTPA_TYPE2,
} MTPA_TYPE_ENUM;

/**
  * @brief Structure of the mtpa fw configuration parameter.
  */
typedef struct {
    /* MTPA */
    float currMax;
    float idLimit;
    const float (*mtpaIsAngleLut)[MTPA_LUT_POINT];
    unsigned short mtpaIsAngleLutLen;
    const float (*mtpaIsIndLut)[INDUCTANCE_LUT_POINT];
    unsigned short mtpaIsIndLutLen;
    float psif;
    /* FW */
    float udcThreshPer;
    float ts;
} CURRTRAJ_PARAM_Handle;

/**
  * @brief Structure of the mtpa fw Enabling switch.
  */
typedef struct {
    unsigned short mtpaEnable;
    MTPA_TYPE_ENUM mtpaType;
    unsigned short fwEnable;
} CURRTRAJ_ENABLE_Handle;

/**
  * @brief Structure of the mtpa fw current trajectory control.
  */
typedef struct {
    CURRTRAJ_ENABLE_Handle enable;
    CURRTRAJ_PARAM_Handle param;
    /* MTPA */
    float angleMtpa;

    /* FW */
    PID_Handle pi;
    FOFLT_Handle udqAmpLpf;
    FOFLT_Handle idRefLpf;
    FOFLT_Handle iqRefLpf;
    float angleFw;
    float udqRefAmp;
    float udcLimit;
    /* Result */
    float angle;
    DqAxis idqRefRaw;
    DqAxis idqRef;    /* reference instruction value. */
} CURRTRAJ_Handle;

/* MTPA can be implemented in two modes based on user requirements:
 * SPMSM: skip MTPA
 * IPMSM:
 * 1.Fixed inductance and flux linkage parameters/with current - inductance table - lookup table +
 *   formula method to calculate idqRef;
 * 2.With current - current angle table - lookup table
 * (Current-current angle table creation requires scanning of the MTPA operating point with script running.)
 */

void CURRTRAJ_Init(CURRTRAJ_Handle *p,
                   const PI_Param *piParam,
                   CURRTRAJ_ENABLE_Handle *enable,
                   CURRTRAJ_PARAM_Handle *param,
                   float idqRefLpfFreq);
void CURRTRAJ_Clear(CURRTRAJ_Handle *p);
void CURRTRAJ_Exec(CURRTRAJ_Handle *p, float isRef, DqAxis udqRef, float udc, float spdRefHz);
void CURRTRAJ_SetTs(CURRTRAJ_Handle *p, float ts);

#endif
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
  * @file      mcs_ladrc.h
  * @author    MCU Algorithm Team
  * @brief     This file provides functions declaration of ladrc control.
  *
  */

#ifndef McuMagicTag_MCS_LADRC_H
#define McuMagicTag_MCS_LADRC_H

#include "typedefs.h"

typedef enum {
    LADRC_TYPE_1ST = 0,               /* Conventional type of 1st-ladrc: No error constraint */
    LADRC_TYPE_1ST_ERR_CONSTRAINT_Z1, /* Full decoupled type of 1st-ladrc: Error constraint with z1 for feedback */
    LADRC_TYPE_1ST_ERR_CONSTRAINT_X1, /* Enhanced type of 1st-ladrc: Error constraint with x1 for feedback */
    LADRC_TYPE_2ND,                   /* Conventional type of 2nd-LADRC */
} LADRC_Type;

typedef struct {
    float ts;              /**< Control period (s). */
    /* user parameter */
    float wc;              /**< Control bandwidth (rad/s). */
    float wo;              /**< Eso bandwidth (rad/s). */
    float b0;              /**< System compensation parameter. */
    float outLim;          /**< Output limitation. */
    /* ESO calc param */
    float z1;              /**< Estimation of state x1, denoted as x1hat. */
    float z2;              /**< Estimation of state x2, denoted as x2hat. */
    float z3;              /**< Estimation of state x3, denoted as x3hat. Only for ladrc2, f_dot_hat. */
    float e1;              /**< State error, defined as (z1 - x1). */
    float e1Last;          /**< Last value of e1. */
    /* Acceleration Feedforward */
    float ffGain;          /**< Acceleration feedforward gain. */
    /* Quasi-Resonance Controller (QRC) param */
    bool qrcEn;            /**< Enable flag of QRC. */
    float qrcKr;           /**< Resonance gian of QRC. */
    float qrcWc;           /**< Resonance bandwidth of QRC (rad/s). */
    float qrcWr;           /**< Resonance frequency of QRC (rad/s). */
    /* QRC calc param */
    float err;             /**< Control error, defined as (ref - fbk). */
    float errLast;         /**< Last value of err. */
    float errLLast;        /**< Last value of errLast. */
    float uqrc;            /**< Output of QRC. */
    float uqrcLast;        /**< Last value of uqrc. */
    float uqrcLLast;       /**< Last value of uqrcLast. */

    LADRC_Type adrcType;   /**< Ladrc type selection. */

    float output;          /**< Ladrc output. */
} LADRC_Handle;

/**
 * For LADRC, x_dot = a*x + b*u, y = c*x + d*u
 * u: controller output or system input, y: system output, x: system state, z: system state estimation.
 * wc, Used to adjust the response speed of the system.
 * wo, Used to adjust the response speed for the observer convergence.
 * b0, Used for system disturbance feedforward compensation.
 *
 * Experience parameters: wc = 4/tr, wo = 3~10 * wc
 * Speed loop parameter：  b0 = 1.5f*np*psif/J
 * Current loop parameter：bd = 1/Ld, bq = 1/Lq
*/


void LADRC_Init(LADRC_Handle *ladrc, float ctrlBdw, float b0, float outLim, float ts);

void LADRC_Clear(LADRC_Handle *ladrc);

float LADRC_Exec(LADRC_Handle *ladrc, float ref, float refDot, float fbk);

void LADRC_SetFfGain(LADRC_Handle *ladrc, float gain);

void LADRC_SetType(LADRC_Handle *ladrc, LADRC_Type adrcType);

void LADRC_SetQrcParam(LADRC_Handle *ladrc, float qrcFo, float qrcGain, float qrcBdw);

void LADRC_EnableQrc(LADRC_Handle *ladrc, bool en);

#endif

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
  * @file      mcs_flying_start.h
  * @author    MCU Algorithm Team
  * @brief     This file provides functions declaration of flying start.
  */

#ifndef McuMagicTag_MCS_FLYING_START_H
#define McuMagicTag_MCS_FLYING_START_H

#include "mcs_typedef.h"
#include "typedefs.h"
#include "mcs_asc.h"
#include "mcs_assert.h"

/* Flying start brake control pwm callback function */
typedef void (*MCS_SetPwmDutyCb)(UvwAxis *dutyUvwLeft, UvwAxis *dutyUvwRight);

/**
  * @brief The flying start process status enum.
  */
typedef enum {
    FS_INIT_DETECT, /**< Flying start initial speed detection stage. */
    FS_DECIDE,      /**< Decide on the start mode based on the speed. */
    FS_BREAK,       /**< Flying start break. */
    FS_BREAK_ERR,   /**< Unregistered brake PWM waveform setting function error. */
    FS_FINISH,      /**< Flying start finish. */
} FS_STATE_ENUM;

/**
  * @brief The flying startup mode enum.
  */
typedef enum {
    FS_STARTUP_DETECT,   /**< Flying startup detect stage. */
    FS_OPEN_LOOP_START,  /**< Flying startup mode is open loop startup. */
    FS_CLOSE_LOOP_START, /**< Flying startup mode is close loop startup. */
    FS_NO_START,         /**< Flying startup mode is not start. */
} FS_PROCESS_ENUM;

typedef struct {
    float decisSpd; /**< Flying start decision speed is used to judge braking and closed-loop start-up. */
    float decisSpdMax;
    float ts;                       /**< Control period (s). */
    unsigned int cnt;               /**< Time Count. */
    unsigned int spdDetectCnt;      /**< Speed detection times. */
    ASC_Handle brake;               /**< Brake control handle. */
    FS_PROCESS_ENUM startUpState;   /**< Flying startUp status. */
    FS_STATE_ENUM state;            /**< Flying execution status. */
    MCS_SetPwmDutyCb setPwmDuty;    /**< Flying start brake control pwm callback function. */
} FlyingStart_Handle;

void FS_Init(FlyingStart_Handle *flyStart, float ts, float decisSpd, float decisSpdMax, float spdDetTime);
void FS_BrakeInit(FlyingStart_Handle *flyStart, ASC_Param ascParam);
void FS_Clear(FlyingStart_Handle *flyStart);
void FS_Exec(FlyingStart_Handle *flyStart, float obsSpd, DqAxis *idqRef, float brakeCurr);
void FS_RegPwmCallback(FlyingStart_Handle *flyStart, MCS_SetPwmDutyCb pCallback);
bool FS_IsFinish(FlyingStart_Handle *flyStart);
FS_PROCESS_ENUM FS_GetState(FlyingStart_Handle *flyStart);

#endif
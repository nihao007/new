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
  * @file      motor_heating.h
  * @author    MCU Algorithm Team
  * @brief     This file provides motor heating function.
  **/
#ifndef McuMagicTag_MCS_HEATING_H
#define McuMagicTag_MCS_HEATING_H

#include "typedefs.h"
#include "mcs_typedef.h"

/**
  * @brief motor heat mode define.
  * @details heat mode type:
  *          + HEAT_CURR_MODE   -- Constant current mode heating.
  *          + HEAT_POW_MODE    -- Constant power mode heating.
  */
typedef enum {
    HEAT_CURR_MODE,
    HEAT_POW_MODE
} HEAT_MODE_ENUM;

/**
  * @brief Parameters related to the configuration of the heating module.
  */
typedef struct {
    float ts;           /**< ctrl period */
    float heatTime;     /**< Total heating duration (S) */
    float heatCurr;     /**< Sets the heating current in constant current mode. */
    float maxCurr;      /**< Maximum allowable heating current */
    float heatPower;    /**< Sets the heating power in constant power mode. */
    float ctrlStep;     /**< Current control step in heating state */
    HEAT_MODE_ENUM mode; /**< Heating control mode */
} HEAT_PARAM;

typedef struct {
    float timer;
    float curr;
    float power;
    float maxCurr;
    float ctrlStep;
    float cnt;
    unsigned int angleSwitchCnt;
    bool finished;
    HEAT_MODE_ENUM mode;
} HEAT_Handle;

void HEAT_Init(HEAT_Handle *heat, HEAT_PARAM param);
void HEAT_Clear(HEAT_Handle *heat);
void HEAT_Exec(HEAT_Handle *heat, float *angle, DqAxis *idqRef, float powFbk);

#endif
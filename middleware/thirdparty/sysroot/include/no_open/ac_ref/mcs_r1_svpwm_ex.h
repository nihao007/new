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
  * @file      mcs_r1_svpwm.h
  * @author    MCU Algorithm Team
  * @brief     This file provides functions declaration of Space-vector pulse-width-modulation calculations.
  */

/* Define to prevent recursive inclusion ------------------------------------- */
#ifndef McuMagicTag_MCS_R1_SVPWM_EX_H
#define McuMagicTag_MCS_R1_SVPWM_EX_H

/* Includes ------------------------------------------------------------------*/
#include "mcs_typedef.h"
#include "mcs_svpwm.h"
#include "typedefs.h"
/** The ADC sampling twice for one resistor motor control application, SOCA + SOCB */
#define R1_ADC_SAMPLE_NUMS_EX 4
#define SOCA_UP 0
#define SOCB_UP 1
#define SOCA_DOWN 2
#define SOCB_DOWN 3
/**
  * @defgroup R1_SVPWM_MODULE  R1 SVPWM MODULE
  * @brief The SVPWM module for R1(One Resistor) application.
  * @{
  */

/**
  * @defgroup R1_SVPWM_STRUCT  R1 SVPWM STRUCT
  * @brief The SVPWM module's struct definition for R1(One Resistor) application.
  * @{
  */
/* Typedef definitions -------------------------------------------------------*/

/**
  * @brief Structure of temporary variables for R1SVPWM calculation.
  */

typedef struct {
    SVPWM_CALC_Handle svCalc;
    float compLeft[SVPWM_COMP_VAL_TOTAL];
    float compRight[SVPWM_COMP_VAL_TOTAL];
    float downSoc[2];
    float upSoc[2];
    bool lRshiftFlag;
} R1SVPWM_CALC_EX_Handle;

/**
  * @brief R1SVPWM struct members and parameters.
  */
typedef struct {
    float voltPu;           /**< Voltage per unit value. */
    float oneDivVoltPu;     /**< Reciprocal of voltage unit value. */
    float sampleWindow;     /**< Sampling Window */
    float samplePointShift; /**< Sampling point phase shift */
    short voltIndex;        /**< Index of voltage sector. */
    short voltIndexLast;    /**< Index of last voltage sector. */
    float voltUtiliRateThr; /**< The voltage utilization rate threshold is used to determine the phase shift mode.*/
    float samplePoint[R1_ADC_SAMPLE_NUMS_EX]; /**< Sample point of twice sample. */
    bool  shiftMode;        /**< true: Sector-based left-right phase shift. false: Fixed right shift. */
    bool  lRshiftEnable;    /**< Sector-based left-right phase shift mode enable. */
    bool  lRshiftFlag;
    bool  lRshiftFlagLast;
    R1SVPWM_CALC_EX_Handle r1SvCalc;
} R1SVPWM_EX_Handle;
/**
  * @}
  */

/**
  * @defgroup R1_SVPWM_API  R1 SVPWM API
  * @brief The SVPWM module's API declaration for R1(One Resistor) application.
  * @{
  */

void R1SVPWM_InitEx(R1SVPWM_EX_Handle *r1svHandle, float voltPu, float samplePointShift, float sampleWindow);
void R1SVPWM_ClearEx(R1SVPWM_EX_Handle *r1svHandle);
void R1SVPWM_ExecEx(R1SVPWM_EX_Handle *r1svHandle, const AlbeAxis *uAlbe, UvwAxis *dutyUvwLeft, UvwAxis *dutyUvwRight);
void R1CurrReconstructLr(short sectorIndex, bool lRshiftFlag, float *currSocUp, float *currSocDown, UvwAxis *curr);
void R1SVPWM_PhaseShiftSetEx(R1SVPWM_EX_Handle *r1svHandle, bool lRshiftMode, float voltUtiliRateThr);
/**
  * @}
  */

/**
  * @}
  */

#endif  /* McuMagicTag_MCS_SVPWM_H */

/**
  * @copyright Copyright (c) 2025, HiSilicon (Shanghai) Technologies Co., Ltd. All rights reserved.
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
  * @file    apt_ex.c
  * @author  MCU Driver Team
  * @brief   APT module driver.
  * @details This file provides extend functions realization of the APT module.
  *           + Initialization and de-initialization functions.
  *           + APT Service Functions.
  */

#include "apt_ex.h"

/* ---------------------------------------------------------------------------------------------- */
/* HRPWM----------------------------------------------------------------------------------------- */
/**
  * @brief Initialize the high resolution PWM function.
  * @param aptHandle APT module handle.
  * @param hrParam HRPWM configuration parameters.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_HRPWMInitEx(APT_Handle *aptHandle, APT_HRPWMParam *hrParam)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(aptHandle->waveform.cntMode != APT_COUNT_MODE_DOWN, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(aptHandle->baseAddress->TC_MODE.BIT.rg_cnt_mode != APT_COUNT_MODE_DOWN, \
                             BASE_STATUS_ERROR);
    APT_ASSERT_PARAM(hrParam != NULL);

    APT_RegStruct *aptx = aptHandle->baseAddress;
    /* Set timer period */
    DCL_APT_SetTimeBasePeriodHr(aptx, hrParam->cntPrdHr);
    /* Set phase count */
    DCL_APT_SetCounterPhaseHr(aptx, hrParam->cntPhsHr);
    /* Set deadband count */
    aptx->DG_RED.BIT.rg_dg_red_hr = hrParam->cntDbHr;
    aptx->DG_FED.BIT.rg_dg_fed_hr = hrParam->cntDbHr;
    /* Set compare value */
    DCL_APT_SetCounterCompareHr(aptx, APT_COMPARE_REFERENCE_A, hrParam->cntCmpAHr);
    DCL_APT_SetCounterCompareHr(aptx, APT_COMPARE_REFERENCE_B, hrParam->cntCmpBHr);
    DCL_APT_SetCounterCompareHr(aptx, APT_COMPARE_REFERENCE_C, hrParam->cntCmpCHr);
    DCL_APT_SetCounterCompareHr(aptx, APT_COMPARE_REFERENCE_D, hrParam->cntCmpDHr);

    /* Enable high resolution function */
    DCL_APT_EnableHRPWM(aptx);

    return BASE_STATUS_OK;
}

/**
  * @brief De-initialize the high resolution PWM function.
  * @param aptHandle APT module handle.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_HRPWMDeInitEx(APT_Handle *aptHandle)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    /* Enable high resolution function */
    DCL_APT_DisableHRPWM(aptHandle->baseAddress);
    return BASE_STATUS_OK;
}

/**
  * @brief Set HRPWM period.
  * @param aptHandle APT module handle.
  * @param prdLr New period of low resolution part.
  * @param prdHr New period of high resolution part.
  * @param prdLoadMode Buffer load mode, recommend: APT_BUFFER_INDEPENDENT_LOAD.
  * @param prdLoadEvt Period event load mode, recommend: APT_PERIOD_LOAD_EVENT_ZERO.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_SetHRPWMPrdEx(APT_Handle *aptHandle, \
                                      unsigned short prdLr, \
                                      unsigned char prdHr, \
                                      APT_BufferLoadMode prdLoadMode, \
                                      APT_PrdLoadEvent prdLoadEvt)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    /* This parameter can be set only when high resolution is enabled. */
    if (aptHandle->baseAddress->HR_CFG.BIT.rg_hr_en == BASE_CFG_DISABLE) {
        return BASE_STATUS_ERROR;
    }
    
    /* Set LR-HR period and load mode and load event */
    DCL_APT_SetTimeBasePeriodHr(aptHandle->baseAddress, prdHr);
    HAL_APT_SetTimerPeriod(aptHandle, prdLr, prdLoadMode, prdLoadEvt);
    return BASE_STATUS_OK;
}

/**
  * @brief Set HRPWM phase.
  * @param aptHandle APT module handle.
  * @param phsLr New phase of low resolution part.
  * @param phsHr New phase of high resolution part.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_SetHRPWMPhsEx(APT_Handle *aptHandle, unsigned short phsLr, unsigned char phsHr)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    /* This parameter can be set only when high resolution is enabled. */
    if (aptHandle->baseAddress->HR_CFG.BIT.rg_hr_en == BASE_CFG_DISABLE) {
        return BASE_STATUS_ERROR;
    }
    /* Sets the period value of the member variable. */
    DCL_APT_SetCounterPhase(aptHandle->baseAddress, phsLr);
    DCL_APT_SetCounterPhaseHr(aptHandle->baseAddress, phsHr);
    return BASE_STATUS_OK;
}

/**
  * @brief Attribute configuration of any reference point.
  * @param aptHandle APT module handle.
  * @param refDotSelect Selection of reference points.
  * @param refDotParameters The properties of the reference point.
  * @retval BASE_StatusType: OK, ERROR.
  */
BASE_StatusType HAL_APT_ConfigHRPWMRefDotEx(APT_Handle *aptHandle, APT_RefDotSelect refDotSelect, \
                                            APT_RefDotParameters *refDotParameters)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(refDotParameters != NULL);
    APT_PARAM_CHECK_WITH_RET(refDotSelect >= APT_REFERENCE_DOTA, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(refDotSelect <= APT_REFERENCE_DOTD, BASE_STATUS_ERROR);
    /* This parameter can be set only when high resolution is enabled. */
    if (aptHandle->baseAddress->HR_CFG.BIT.rg_hr_en == BASE_CFG_DISABLE) {
        return BASE_STATUS_ERROR;
    }
    /* Set HR reference point */
    DCL_APT_SetCounterCompareHr(aptHandle->baseAddress, \
                                (APT_CompareRef)refDotSelect, \
                                refDotParameters->refDotValueHr);
    /* refDotParameters pointer content check following. */
    return HAL_APT_ConfigRefDot(aptHandle, refDotSelect, refDotParameters);
}

/**
  * @brief Set the count compare points along the left and right edges of PWM waveform.
  * @param aptHandle APT module handle.
  * @param cntCmpLeftLr The low resolution count compare point of the left edge of PWM waveform.
  * @param cntCmpLeftHr The high resolution count compare point of the left edge of PWM waveform.
  * @param cntCmpRightLr The low resolution count compare point of the right edge of PWM waveform.
  * @param cntCmpRightHr The high resolution count compare point of the right edge of PWM waveform.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_SetHRPWMDutyEx(APT_Handle *aptHandle, \
                                       unsigned short cntCmpLeftLr, \
                                       unsigned char  cntCmpLeftHr, \
                                       unsigned short cntCmpRightLr, \
                                       unsigned char  cntCmpRightHr)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    /* This parameter can be set only when high resolution is enabled. */
    if (aptHandle->baseAddress->HR_CFG.BIT.rg_hr_en == BASE_CFG_DISABLE) {
        return BASE_STATUS_ERROR;
    }
    /* Set high resolution compare value of point C or point D */
    DCL_APT_SetCounterCompare(aptHandle->baseAddress, APT_COMPARE_REFERENCE_C, cntCmpLeftLr);
    DCL_APT_SetCounterCompareHr(aptHandle->baseAddress, APT_COMPARE_REFERENCE_C, cntCmpLeftHr);
    DCL_APT_SetCounterCompare(aptHandle->baseAddress, APT_COMPARE_REFERENCE_D, cntCmpRightLr);
    DCL_APT_SetCounterCompareHr(aptHandle->baseAddress, APT_COMPARE_REFERENCE_D, cntCmpRightHr);
    return BASE_STATUS_OK;
}

/**
  * @brief Calculate the high resolution and low resolution values from the floating comparison point.
  * @param cmpF Floating comparison point.
  * @param cmpLr Low-resolution calculation results.
  * @param cmpHr High-resolution calculation results.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
static void APT_LrHrCmpPointCalc(float cmpF, unsigned short *cmpLr, unsigned char *cmpHr)
{
    *cmpLr = (unsigned short)cmpF;
    *cmpHr = (unsigned char)((cmpF - (float)*cmpLr) * 0x100); /* 0x100: coeff 256 */
}

/**
  * @brief Clamp upper and lower limitation for low resolution compare point of HRPWM.
  * @param val The original value.
  * @param upperLim Upper limitation.
  * @param lowerLim Lower limitation.
  * @retval Value after limiting.
  */
static unsigned short APT_HRPWMClamp(unsigned short val, unsigned short upperLim, unsigned short lowerLim)
{
    return (val >= upperLim ? upperLim : (val <= lowerLim ? lowerLim : val));
}

/**
  * @brief Calculation of the left and right comparison points in up_down mode when hardware conversion enabled.
  * @param period Period value of apt timer count.
  * @param dutyF PWM duty. Range: 0.0f ~ 1.0f.
  * @param leftEdgeLr Left edge low resolution compare point.
  * @param leftEdgeHr Left edge high resolution compare point.
  * @param rightEdgeLr Right edge low resolution compare point.
  * @param rightEdgeHr Right edge high resolution compare point.
  * @retval None.
  */
static void APT_HwConvertHRPWMDutyF(float period, float dutyF, \
                                    unsigned short *leftEdgeLr, unsigned char *leftEdgeHr, \
                                    unsigned short *rightEdgeLr, unsigned char *rightEdgeHr)
{
    /* 1 high precision may be lost due to truncation of floating point number calculation */
    float cmpLeft = period * (1.0f - dutyF);
    /* Calculate left edge (point C) compare point */
    APT_LrHrCmpPointCalc(cmpLeft, leftEdgeLr, leftEdgeHr);
    /* Point D is equal to Point C */
    *rightEdgeLr = *leftEdgeLr;
    *rightEdgeHr = *leftEdgeHr;
}


/**
  * @brief Calculation of the left and right comparison points in up_down mode when hardware conversion disabled.
  * @param period Period value of apt timer count.
  * @param dutyF PWM duty. Range: 0.0f ~ 1.0f.
  * @param leftEdgeLr Left edge low resolution compare point.
  * @param leftEdgeHr Left edge high resolution compare point.
  * @param rightEdgeLr Right edge low resolution compare point.
  * @param rightEdgeHr Right edge high resolution compare point.
  * @retval None.
  */
static void APT_SwConvertHRPWMDutyF(float period, float dutyF, \
                                    unsigned short *leftEdgeLr, unsigned char *leftEdgeHr, \
                                    unsigned short *rightEdgeLr, unsigned char *rightEdgeHr)
{
    /* 1 high precision may be lost due to truncation of floating point number calculation */
    float cmpLeft = period * (1.0f - dutyF);
    /* Calculate left edge (point C) compare point */
    APT_LrHrCmpPointCalc(cmpLeft, leftEdgeLr, leftEdgeHr);
    /** Calculate right edge (point D) compare point,
     *  according to the result of the assistance point (left),
     */
    if (*leftEdgeHr == 0) {
        /* If the left comparison point is an integer value, so as the right comparison point. */
        *rightEdgeLr = *leftEdgeLr;
        *rightEdgeHr = 0;
    } else {
        *rightEdgeLr = *leftEdgeLr + 1;
        /* Ensure that the sum of the high resolution of the left and right points is 256. */
        *rightEdgeHr = 0x100 - *leftEdgeHr;
    }
}


/**
  * @brief Set the count compare points for HRPWM using float duty.
  * @param aptHandle APT module handle.
  * @param dutyF PWM duty. Range: 0.0f ~ 1.0f.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_SetHRPWMDutyFEx(APT_Handle *aptHandle, float dutyF)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_PARAM_CHECK_WITH_RET(dutyF >= 0.0f && dutyF <= 1.0f, BASE_STATUS_ERROR);

    /* This parameter can be set only when high resolution is enabled. */
    if (aptHandle->baseAddress->HR_CFG.BIT.rg_hr_en == BASE_CFG_DISABLE) {
        return BASE_STATUS_ERROR;
    }

    /* Obtain timer period */
    /* 0x100: 256, avoid resolution loss */
    float period = (float)aptHandle->baseAddress->TC_PRD.BIT.rg_cnt_prd + \
                   (float)aptHandle->baseAddress->TC_PRD.BIT.rg_cnt_prd_hr / 0x100;

    unsigned short leftEdgeLr  = 0;
    unsigned short rightEdgeLr = 0;
    unsigned char  leftEdgeHr  = 0;
    unsigned char  rightEdgeHr = 0;
    unsigned short safeZoneUpperLim = 0;
    unsigned short safeZoneLowerLim = 0;
    switch (aptHandle->waveform.cntMode) {
        case APT_COUNT_MODE_UP_DOWN:
            if (!aptHandle->baseAddress->HR_CFG.BIT.rg_hr_ref_autoconv_off) {
                APT_HwConvertHRPWMDutyF(period, dutyF, &leftEdgeLr, &leftEdgeHr, &rightEdgeLr, &rightEdgeHr);
            } else {
                APT_SwConvertHRPWMDutyF(period, dutyF, &leftEdgeLr, &leftEdgeHr, &rightEdgeLr, &rightEdgeHr);
            }
            /* Clamp compare point of up_down mode: 0 ~ prd */
            safeZoneUpperLim = aptHandle->baseAddress->TC_PRD.BIT.rg_cnt_prd;
            safeZoneLowerLim = 0;
            if (safeZoneLowerLim > safeZoneUpperLim) {
                return BASE_STATUS_ERROR;
            }
            leftEdgeLr  = APT_HRPWMClamp(leftEdgeLr, safeZoneUpperLim, safeZoneLowerLim);
            rightEdgeLr = APT_HRPWMClamp(rightEdgeLr, safeZoneUpperLim, safeZoneLowerLim);
            break;

        case APT_COUNT_MODE_UP:
            /* Fix left edge (low point/point C) compare point */
            leftEdgeLr = 0;
            leftEdgeHr = 0;
            /* Calculate right edge (high point/point D) compare point: UP_MODE */
            APT_LrHrCmpPointCalc((period * dutyF + leftEdgeLr), &rightEdgeLr, &rightEdgeHr);
            /* Clamp compare point of up mode: 0 ~ prd */
            safeZoneUpperLim = aptHandle->baseAddress->TC_PRD.BIT.rg_cnt_prd;
            safeZoneLowerLim = 0;
            if (safeZoneLowerLim > safeZoneUpperLim) {
                return BASE_STATUS_ERROR;
            }
            rightEdgeLr = APT_HRPWMClamp(rightEdgeLr, safeZoneUpperLim, safeZoneLowerLim);
            break;

        default:
            break;
    }

    return HAL_APT_SetHRPWMDutyEx(aptHandle, leftEdgeLr, leftEdgeHr, rightEdgeLr, rightEdgeHr);
}

/**
  * @brief Set the count compare points to trigger the ADC sampling.
  * @param aptHandle APT module handle.
  * @param cntCmpSOCALr The low resolution count compare point for triggering SOCA.
  * @param cntCmpSOCAHr The high resolution count compare point for triggering SOCA.
  * @param cntCmpSOCBLr The low resolution count compare point for triggering SOCB.
  * @param cntCmpSOCBHr The high resolution count compare point for triggering SOCB.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_SetHRPWMADCTriggerTimeEx(APT_Handle *aptHandle, \
                                                 unsigned short cntCmpSOCALr, \
                                                 unsigned char cntCmpSOCAHr, \
                                                 unsigned short cntCmpSOCBLr, \
                                                 unsigned char cntCmpSOCBHr)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(cntCmpSOCALr >= 0, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(cntCmpSOCALr <= aptHandle->baseAddress->TC_PRD.BIT.rg_cnt_prd, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(cntCmpSOCBLr >= 0, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(cntCmpSOCBLr <= aptHandle->baseAddress->TC_PRD.BIT.rg_cnt_prd, BASE_STATUS_ERROR);
    /* This parameter can be set only when high resolution is enabled. */
    if (aptHandle->baseAddress->HR_CFG.BIT.rg_hr_en == BASE_CFG_DISABLE) {
        return BASE_STATUS_ERROR;
    }
    /* Set compare point A */
    DCL_APT_SetCounterCompare(aptHandle->baseAddress, APT_COMPARE_REFERENCE_A, cntCmpSOCALr);   /* SOCA-LR */
    DCL_APT_SetCounterCompareHr(aptHandle->baseAddress, APT_COMPARE_REFERENCE_A, cntCmpSOCAHr); /* SOCA-HR */
    /* Set compare point B */
    DCL_APT_SetCounterCompare(aptHandle->baseAddress, APT_COMPARE_REFERENCE_B, cntCmpSOCBLr);   /* SOCB-LR */
    DCL_APT_SetCounterCompareHr(aptHandle->baseAddress, APT_COMPARE_REFERENCE_B, cntCmpSOCBHr); /* SOCB-HR */

    return BASE_STATUS_OK;
}

/**
  * @brief Initialize the chopper function, not support independent or global load.
  * @param aptHandle APT module handle.
  * @param chpParam Chopper configuration patameters.
  *                 + psclkDiv8x chopper clock which can be divided by 8/16/24/.../256 (0/1/2/.../31) of aptclk.
  *                 + initWidth chopper initial pulse width which can be set as 1-16 (0-15) PSCK.
  *                 + shape chopper shape which can be configured by 8bit shape.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_ChopperInitEx(APT_Handle *aptHandle, APT_ChopperParam *chpParam)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_ASSERT_PARAM(chpParam != NULL);

    APT_RegStruct *aptx = aptHandle->baseAddress;
    /* Set chopper clock division */
    DCL_APT_SetChopperFreqDiv(aptx, chpParam->psclkDiv8x);
    /* Set chopper initial width */
    DCL_APT_SetChopperInitWidth(aptx, chpParam->initWidth);
    /* Set chopper shape */
    DCL_APT_SetChopperShape(aptx, chpParam->shape);

    return BASE_STATUS_OK;
}

/**
  * @brief Set the chopper initial pulse width and shape.
  * @param aptHandle APT module handle.
  * @param initWidth Chopper initial pulse width which can be set as 1~16 (0-15) PSCK.
  * @param shape Chopper shape which can be configured by 8bit shape.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_SetChopperShapeEx(APT_Handle *aptHandle, unsigned char initWidth, unsigned char shape)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    /* Set chopper initial width */
    DCL_APT_SetChopperInitWidth(aptHandle->baseAddress, initWidth);
    /* Set chopper shape */
    DCL_APT_SetChopperShape(aptHandle->baseAddress, shape);
    return BASE_STATUS_OK;
}

/**
 * @brief Enable/Disable the pwm chopper function.
 * @param aptHandle APT module handle.
 * @param ch Channel A or Channel B.
 * @param en Enable/Disable switch.
 * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
 */
BASE_StatusType HAL_APT_ChopperEnableEx(APT_Handle *aptHandle, APT_PWMChannel ch, bool en)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    if (en) {
        /* Enable */
        DCL_APT_EnablePwmChopper(aptHandle->baseAddress, ch);
    } else {
        /* Disable */
        DCL_APT_DisablePwmChopper(aptHandle->baseAddress, ch);
    }
    return BASE_STATUS_OK;
}

/* ---------------------------------------------------------------------------------------------- */
/* BURST----------------------------------------------------------------------------------------- */
/**
  * @brief Initialize the burst function, support independent or global load.
  * @param aptHandle APT module handle.
  * @param ch Channel A or Channel B.
  * @param brstParam Burst configuration patameters.
  *                 + m Busrt period is m.
  *                 + n N period limit the PWM output to low
  *                 + bstMode Select whether output pwm first or later.
  *                 + bstLoadMode Support independent or global load.
  *                 + bstLoadEvent Load event.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_BurstInitEx(APT_Handle *aptHandle, APT_PWMChannel ch, APT_BurstParam *brstParam)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    APT_ASSERT_PARAM(brstParam != NULL);
    APT_PARAM_CHECK_WITH_RET(brstParam->brstPeriod >= brstParam->closedPwmPeriod, BASE_STATUS_ERROR);
    APT_RegStruct *aptx = aptHandle->baseAddress;
    DCL_APT_SetBurstClosedPol(aptx, ch, brstParam->closedPwmPol);
    /* Config burst period m and n */
    DCL_APT_SetBurstMN(aptx, ch, brstParam->brstPeriod, brstParam->closedPwmPeriod);
    /* Config burst mode. */
    DCL_APT_SetBurstMode(aptx, ch, brstParam->bstMode);
    /* Config buff load mode. */
    DCL_APT_SetBurstLoadMode(aptx, ch, brstParam->bstLoadMode);
    /* Config buff load event. */
    DCL_APT_SetBurstLoadEvent(aptx, ch, brstParam->bstLoadEvent);

    return BASE_STATUS_OK;
}

/**
  * @brief Initialize the burst function, support independent or global load.
  * @param aptHandle APT module handle.
  * @param ch Channel A or Channel B.
  * @param brstPeriod Busrt period is m.
  * @param closedPwmPeriod Disable pwm output period is n..
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_SetBurstMNEx(APT_Handle *aptHandle, \
                                     APT_PWMChannel ch, \
                                     unsigned char brstPeriod, \
                                     unsigned char closedPwmPeriod)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(brstPeriod >= closedPwmPeriod, BASE_STATUS_ERROR);
    /* Set burst period */
    DCL_APT_SetBurstMN(aptHandle->baseAddress, ch, brstPeriod, closedPwmPeriod);
    return BASE_STATUS_OK;
}

/**
 * @brief Enable/Disable the pwm burst function.
 * @param aptHandle APT module handle.
 * @param ch Channel A or Channel B.
 * @param en Enable/Disable switch.
 * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
 */
BASE_StatusType HAL_APT_BurstEnableEx(APT_Handle *aptHandle, APT_PWMChannel ch, bool en)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    if (en) {
        /* Enable */
        DCL_APT_EnablePwmBurst(aptHandle->baseAddress, ch);
    } else {
        /* Disable */
        DCL_APT_DisablePwmBurst(aptHandle->baseAddress, ch);
    }
    return BASE_STATUS_OK;
}

// ----------------------------------------------------------------------------
// MDB----------------------------------------------------------------------------

/**
  * @brief Initialize the minimal deadband function.
  *        Note1: The MINDB_XBAR must be configured when xbar is selected as ref singal,
  *               use API HAL_XBAR_ConfigMDBXbarEx to config MINDB_XBAR.
  *        Note2: The output of MINDB is the OC_PWMx when MindB is disabled.
  * @param aptHandle APT module handle.
  * @param ch Channel A or Channel B.
  * @param mdbParam Minimal deadband configuration patameters.
  *                 + mdbRefSigPol Reference signal polarity of MINDB module.
  *                 + mdbDelayMode Block signal delay mode of the channel A/B.
  *                 + delay Delay value.
  *                 + mdbBlkSigSel Select Block signal to deal with the local pwm signal.
  *                 + outputPol MinDB output signal polarity.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_MDBInitEx(APT_Handle *aptHandle, APT_PWMChannel ch, APT_MDBParam *mdbParam)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(mdbParam != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    APT_RegStruct *aptx = aptHandle->baseAddress;

    /* Select the reference signal */
    DCL_APT_SetMDBRefSig(aptx, ch, mdbParam->mdbRefSig);
    /* Set the reference signal polarity */
    DCL_APT_SetMDBRefSigPolarity(aptx, ch, mdbParam->mdbRefSigPol);
    /* Set the reference signal delay mode */
    DCL_APT_SetMDBBlkSigDlyMode(aptx, ch, mdbParam->mdbDelayMode, mdbParam->delay);
    /* Set Block A/B signal which processed with the local channel pwm signal. */
    DCL_APT_SelectMDBBlkSig(aptx, ch, mdbParam->mdbBlkSigSel);
    /* Sets the MDB output signal polarity. */
    DCL_APT_SetMDBOutSigPol(aptx, ch, mdbParam->outputPol);
    return BASE_STATUS_OK;
}

/**
  * @brief Deinitialize the minimal deadband function, shut down this function.
  * @param aptHandle APT module handle.
  * @param ch Channel A or Channel B.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_MDBDeInitEx(APT_Handle *aptHandle, APT_PWMChannel ch)
{
    if (ch == APT_PWM_CHANNEL_A) {
        /* Deinitialize MINDB funtion of PWMA */
        aptHandle->baseAddress->MINDB_CFG.reg &= 0xFFFF0000;  /* Offset value of PWMA */
        aptHandle->baseAddress->MINDB_DLY.BIT.rg_mdb_dly_a = 0;
    } else {
        /* Deinitialize MINDB funtion of PWMB */
        aptHandle->baseAddress->MINDB_CFG.reg &= 0x0000FFFF;  /* Offset value of PWMB */
        aptHandle->baseAddress->MINDB_DLY.BIT.rg_mdb_dly_b = 0;
    }
    return BASE_STATUS_OK;
}

/**
 * @brief Enable/Disable the minimal deadband function.
 * @param aptHandle APT module handle.
 * @param ch Channel A or Channel B.
 * @param en Enable/Disable switch.
 * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
 */
BASE_StatusType HAL_APT_MDBEnableEx (APT_Handle *aptHandle, APT_PWMChannel ch, bool en)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    if (en) {
        /* Enable */
        DCL_APT_EnableMDB(aptHandle->baseAddress, ch);
    } else {
        /* Disable */
        DCL_APT_DisableMDB(aptHandle->baseAddress, ch);
    }
    return BASE_STATUS_OK;
}

/* ---------------------------------------------------------------------------- */
/* OLUT------------------------------------------------------------------------ */

/**
  * @brief Initialize the output lookup table function.
  *        Note: The OLUT_XBAR source must be configured before,
  *              use API HAL_XBAR_ConfigOLUTXbarEx to config OLUT_XBAR.
  * @param aptHandle APT module handle.
  * @param ch Channel A or Channel B.
  * @param olutParam OLUT initialization parameters.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_OLUTInitEx(APT_Handle *aptHandle, APT_PWMChannel ch, APT_OLUTParam *olutParam)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(olutParam != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    APT_RegStruct *aptx = aptHandle->baseAddress;

    /* Set input signal of IN3 */
    DCL_APT_SetOLUTInput3Src(aptx, ch, olutParam->in3Sig);
    /* Set truth table */
    DCL_APT_SetOLUT(aptx, ch, olutParam->outTable, APT_OLUT_OUTPUT_COMBINATION_MAX_NUM);

    return BASE_STATUS_OK;
}

/**
  * @brief Deinitialize the output lookup table function.
  * @param aptHandle APT module handle.
  * @param ch Channel A or Channel B.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_OLUTDeInitEx(APT_Handle *aptHandle, APT_PWMChannel ch)
{
    if (ch == APT_PWM_CHANNEL_A) {
        /* Deinitialize the output lookup table function of PWMA. */
        aptHandle->baseAddress->OLUTA_CFG.reg = 0;
    } else {
        /* Deinitialize the output lookup table function of PWMB. */
        aptHandle->baseAddress->OLUTB_CFG.reg = 0;
    }
    return BASE_STATUS_OK;
}

/**
 * @brief Enable/Disable the output lookup table function.
 * @param aptHandle APT module handle.
 * @param ch Channel A or Channel B.
 * @param en Enable/Disable switch.
 * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
 */
BASE_StatusType HAL_APT_OLUTEnableEx(APT_Handle *aptHandle, APT_PWMChannel ch, bool en)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);

    if (en) {
        /* Enable */
        DCL_APT_EnableOLUT(aptHandle->baseAddress, ch);
    } else {
        /* Disable */
        DCL_APT_DisableOLUT(aptHandle->baseAddress, ch);
    }
    return BASE_STATUS_OK;
}

/* ---------------------------------------------------------------------------- */
/* LINK------------------------------------------------------------------------ */

/**
 * @brief Set the all link events to APTx, Disable the APTx link source when select aptLinkSrc as APT_LINK_DISABLE.
 * @param aptHandle APT module handle.
 * @param aptLinkSrc APT link source.
 * @param linkEvt Link events.
 * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
 */
BASE_StatusType HAL_APT_SetLinkEventAndSrcEx(APT_Handle *aptHandle, APT_LinkSrc aptLinkSrc, APT_LinkEvent linkEvt)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(aptLinkSrc <= APT_LINK_DISABLE, BASE_STATUS_ERROR);
    /* The function offset of each LINK_CFG register is represented by five bits. */
    unsigned int funcOffset = linkEvt & 0b11111;
    /* Bit6 and bit7 indicate LINK_CFGx. */
    unsigned int linkCfgReg = (linkEvt & 0b1100000) >> 0x5; /* Shift right by 5 bits to obtain the register index. */
    switch (linkCfgReg) {
        case 0: /* LINK_CFG0 */
            /* Clear first, then set value. 0b11111 indicate 5bits occupied. */
            aptHandle->baseAddress->LINK_CFG0.reg &= ~(0b11111 << funcOffset);
            aptHandle->baseAddress->LINK_CFG0.reg |= (aptLinkSrc << funcOffset);
            break;
        case 1: /* LINK_CFG1 */
            /* Clear first, then set value. 0b11111 indicate 5bits occupied. */
            aptHandle->baseAddress->LINK_CFG1.reg &= ~(0b11111 << funcOffset);
            aptHandle->baseAddress->LINK_CFG1.reg |= (aptLinkSrc << funcOffset);
            break;
        case 2: /* LINK_CFG2 */
            /* Clear first, then set value. 0b11111 indicate 5bits occupied. */
            aptHandle->baseAddress->LINK_CFG2.reg &= ~(0b11111 << funcOffset);
            aptHandle->baseAddress->LINK_CFG2.reg |= (aptLinkSrc << funcOffset);
            break;
        case 3: /* LINK_CFG3 */
            /* Clear first, then set value. 0b11111 indicate 5bits occupied. */
            aptHandle->baseAddress->LINK_CFG3.reg &= ~(0b11111 << funcOffset);
            aptHandle->baseAddress->LINK_CFG3.reg |= (aptLinkSrc << funcOffset);
            break;
        default:
            break;
    }
    return BASE_STATUS_OK;
}

/* ---------------------------------------------------------------------------- */
/* OC DELAY PROTECT AND RECOVERY----------------------------------------------- */
/**
  * @brief Configuring CBC-to-OSHT Protection.
  * @param aptHandle APT module handle.
  * @param cbc2OshtTime Duration time for CBC switch to OSHT protection, unit: apt_clk.
  * 1. cbc2OshtTime = 0 ~ 0xFFFFFFFE: Config value.
  * 2. cbc2OshtTime = 0xFFFFFFFF: Disable CBC switch OSHT function.
  * @param cbcClrTime OC_CBC_TIMER is cleared when CBC protection does not occur within this time, unit: apt_clk.
  * 1. cbcClrTime = 0 ~ 0xFFFFFFFE: Clear CBC_Timer when CBC low level time exceeds cbcClrTime.
  * 2. cbcClrTime = 0xFFFFFFFF: Disable CBC_Timer clear funtion.
  * @retval None.
  */
BASE_StatusType HAL_APT_SetCbc2OshtEx(APT_Handle *aptHandle, unsigned int cbc2OshtTime, unsigned int cbcClrTime)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    /* Enable CBC_Timer First */
    DCL_APT_EnableCbcProtTimer(aptHandle->baseAddress);
    /* 0xFFFFFFFF: Disable CBC switch OSHT function. */
    if (cbc2OshtTime == 0xFFFFFFFF) {
        DCL_APT_DisableCbcSwitchToOshtProtTimer(aptHandle->baseAddress);
    } else {
        /* Config CBC to OSHT duration time, CBC will switch to OSHT when OC_CBC_TIMER is more than cbc2OshtTime. */
        DCL_APT_SetCbcSwitchToOshtProtTimer(aptHandle->baseAddress, cbc2OshtTime);
    }

    /* 0xFFFFFFFF: Disable CBC_Timer clear funtion. */
    if (cbcClrTime == 0xFFFFFFFF) {
        DCL_APT_DisableCbcSwitchToOshtClrTimer(aptHandle->baseAddress);
    } else {
        /* Config CBC_Timer clear, OC_CBC_TIMER is cleared when CBC protection does not occur within cbcClrTime. */
        DCL_APT_SetCbcSwitchToOshtClrTimer(aptHandle->baseAddress, cbcClrTime);
    }

    return BASE_STATUS_OK;
}

/**
 * @brief Set CBC/OSHT/FREE delay protection of channel A and channel B.
 * @param aptHandle APT module handle.
 * @param ocMode Selection of CBC or OSHT or FREE.
 * @param ch Channel selection.
 * @param protDlyAptClk Delay protection time.
 * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
 */
BASE_StatusType HAL_APT_SetDelayProtEx(APT_Handle *aptHandle, \
                                       APT_OutCtrlMode ocMode, \
                                       APT_PWMChannel ch, \
                                       unsigned short protDlyAptClk)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ocMode >= APT_OUT_CTRL_CYCLE_BY_CYCLE, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ocMode <= APT_OUT_CTRL_FREE, BASE_STATUS_ERROR);
    APT_RegStruct *aptx = aptHandle->baseAddress;

    if (ch == APT_PWM_CHANNEL_A) {
        /* Set CBC/OSHT/FREE delay protection of channel A */
        DCL_APT_SetDelayProtChA(aptx, ocMode, protDlyAptClk);
    } else {
        /* Set CBC/OSHT/FREE delay protection of channel B */
        DCL_APT_SetDelayProtChB(aptx, ocMode, protDlyAptClk);
    }

    return BASE_STATUS_OK;
}

/**
 * @brief Set CBC/OSHT/FREE delay recovery of channel A and channel B.
 * @param aptHandle APT module handle.
 * @param ocMode Selection of CBC or OSHT or FREE.
 * @param ch Channel selection.
 * @param recyDlyAptClk Delay recovery time.
 * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
 */
BASE_StatusType HAL_APT_SetDelayRecyEx(APT_Handle *aptHandle, \
                                       APT_OutCtrlMode ocMode, \
                                       APT_PWMChannel ch, \
                                       unsigned short recyDlyAptClk)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(ocMode >= APT_OUT_CTRL_CYCLE_BY_CYCLE, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ocMode <= APT_OUT_CTRL_FREE, BASE_STATUS_ERROR);
    APT_RegStruct *aptx = aptHandle->baseAddress;

    if (ch == APT_PWM_CHANNEL_A) {
        /* Set CBC/OSHT/FREE delay recovery of channel A */
        DCL_APT_SetDelayRecyChA(aptx, ocMode, recyDlyAptClk);
    } else {
        /* Set CBC/OSHT/FREE delay recovery of channel B */
        DCL_APT_SetDelayRecyChB(aptx, ocMode, recyDlyAptClk);
    }

    return BASE_STATUS_OK;
}

/**
 * @brief Set OSHT alignment protection or recovery of channel A or channel B.
 * @param aptHandle APT module handle.
 * @param alignEvt Aligment events at zero or period.
 * @param ch Channel selection.
 * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
 */
BASE_StatusType HAL_APT_SetOshtAlignEx(APT_Handle *aptHandle, APT_OshtAlignEvent alignEvt, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(alignEvt >= APT_OC_OSHT_ALIGNMENT_PROT_AT_CNTR_ZERO, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(alignEvt <= APT_OC_OSHT_ALIGNMENT_RECY_AT_CNTR_PERIOD, BASE_STATUS_ERROR);
    APT_RegStruct *aptx = aptHandle->baseAddress;

    if (ch == APT_PWM_CHANNEL_A) {
        /* Set alignment protection and recovery of channel A */
        DCL_APT_SetOshtAlignChA(aptx, alignEvt);
    } else {
        /* Set alignment protection and recovery of channel B */
        DCL_APT_SetOshtAlignChB(aptx, alignEvt);
    }

    return BASE_STATUS_OK;
}

/**
  * @brief Disable CBC/OSHT/FREE delay/align protection of channel A and B.
  * @param aptHandle APT module handle.
  * @param ocMode Selection of CBC or OSHT or FREE, it supports |.
  * @param ch PWM channel.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_DisableDelayProtEx(APT_Handle *aptHandle, APT_OutCtrlMode ocMode, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(IsAptOcMode(ocMode), BASE_STATUS_ERROR);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Channel A */
        DCL_APT_DisableDelayProtChA(aptHandle->baseAddress, ocMode);
    } else {
        /* Channel B */
        DCL_APT_DisableDelayProtChB(aptHandle->baseAddress, ocMode);
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Disable CBC/OSHT/FREE delay/align recovery of channel A and B.
  * @param aptHandle APT module handle.
  * @param ocMode Selection of CBC or OSHT or FREE, it supports |.
  * @param ch PWM channel.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_DisableDelayRecyEx(APT_Handle *aptHandle, APT_OutCtrlMode ocMode, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(IsAptOcMode(ocMode), BASE_STATUS_ERROR);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Channel A */
        DCL_APT_DisableDelayRecyChA(aptHandle->baseAddress, ocMode);
    } else {
        /* Channel B */
        DCL_APT_DisableDelayRecyChB(aptHandle->baseAddress, ocMode);
    }
    return BASE_STATUS_OK;
}
/* ---------------------------------------------------------------------------- */
/* Status --------------------------------------------------------------------- */
/**
  * @brief Get Trip realtime status, only support trip1/2/3!!!
  * @param aptHandle APT module handle.
  * @param tripx The trip whose status you want to obtain.
  * @retval APT_TripStatus: Low, High.
  */
APT_TripStatus HAL_APT_GetTripStatus(APT_Handle *aptHandle, XBAR_TripEvent tripx)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    APT_PARAM_CHECK_WITH_RET(tripx >= XBAR_TRIP_EVENT_TRIP1, APT_TRIP_LOW_LEVEL);
    APT_PARAM_CHECK_WITH_RET(tripx <= XBAR_TRIP_EVENT_TRIP3, APT_TRIP_LOW_LEVEL);
    switch (tripx) {
        case XBAR_TRIP_EVENT_TRIP1:
            /* Get trip1 status */
            return DCL_APT_GetTrip1Status(aptHandle->baseAddress);
            break;

        case XBAR_TRIP_EVENT_TRIP2:
            /* Get trip2 status */
            return DCL_APT_GetTrip2Status(aptHandle->baseAddress);
            break;

        case XBAR_TRIP_EVENT_TRIP3:
            /* Get trip3 status */
            return DCL_APT_GetTrip3Status(aptHandle->baseAddress);
            break;
        
        default:
            return APT_TRIP_LOW_LEVEL;
            break;
    }
}

/**
  * @brief Get PWM Channelx output status.
  * @param aptHandle APT module handle.
  * @param pwmChannel Channel of HRPMWM.
  * @retval APT_PwmStatus: Status of PWM.
  */
APT_PwmStatus HAL_APT_GetPwmStatus(APT_Handle *aptHandle, APT_PWMChannel pwmChannel)
{
    APT_ASSERT_PARAM(aptHandle != NULL);
    APT_ASSERT_PARAM(IsAPTInstance(aptHandle->baseAddress));
    /* PWM Channel Check */
    APT_ASSERT_PARAM(pwmChannel >= APT_PWM_CHANNEL_A);
    APT_ASSERT_PARAM(pwmChannel <= APT_PWM_CHANNEL_B);
    return (pwmChannel == APT_PWM_CHANNEL_A)? DCL_APT_GetPWMAStatus(aptHandle->baseAddress)\
                                            : DCL_APT_GetPWMBStatus(aptHandle->baseAddress);
}

/**
  * @brief Get PWM Channelx output status.
  * @param poex POE0~15 enum variable.
  * @param fltCycle POE filter cycle, 0~ 255.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_APT_SetPoeFilterCycleEx(APT_POEx poex, unsigned char fltCycle)
{
    APT_PARAM_CHECK_WITH_RET(poex >= APT_POE0, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(poex <= APT_POE15, BASE_STATUS_ERROR);
    /* Enable POEx filter. */
    SYSCTRL1->APT_POE_FILTER[poex].BIT.poe_filter_en = BASE_CFG_ENABLE;
    /* Set filter nums. */
    SYSCTRL1->APT_POE_FILTER[poex].BIT.poe_filter_level = fltCycle;
    return BASE_STATUS_OK;
}

// ----------------------------------------------------------------------------
// XBAR------------------------------------------------------------------------
/**
  * @brief Set the input polarity of 64 TRIP-XBAR channels.
  * @param xbarEvt Slect 64 trip-xbar event, it supports |.
  *              0: means trip input-bitx NOT_INVERT(HIGH).
  *              1: means trip input-bitx INVERT(LOW).
  * @param inPol Trip xbar input polarity.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_XBAR_SetTripXbarInPolEx(XBAR_TripXbarEvt xbarEvt, XBAR_InputPolarity inPol)
{
    XBAR_RegStruct *xbar = XBAR;
    /* Set input xbar polarity */
    DCL_XBAR_SetTripXbarInPol(xbar, xbarEvt, inPol);
    return BASE_STATUS_OK;
}

/**
  * @brief Configure the input source and output polarity of TRIPx.
  * @param tripx Tripx event.
  * @param tripSrc Slect 64 trip-xbar event source, it supports |.
  * @param tripOutPol Trip-x output polarity.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_XBAR_ConfigTripXbarEx(XBAR_TripEvent tripx, \
                                          XBAR_TripXbarEvt tripSrc, \
                                          XBAR_TripOutPolarity tripOutPol)
{
    XBAR_RegStruct *xbar = XBAR;
    /* Set tripx input source */
    DCL_XBAR_SetTripxInSrc(xbar, tripx, tripSrc);
    /* Set tripx output polarity */
    DCL_XBAR_SetTripxOutPol(xbar, tripx, tripOutPol);
    return BASE_STATUS_OK;
}

/**
  * @brief Configure the input source of MINDB Xbar.
  * @param mdbXbarx Minimal Xbar-x event.
  * @param mdbXbarSrc Input source of minimal xbar-x event.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_XBAR_ConfigMDBXbarEx(XBAR_MDBXbarEvent mdbXbarx, XBAR_MDBSrc mdbXbarSrc)
{
    XBAR_RegStruct *xbar = XBAR;
    /* Set minimal xbar input source */
    DCL_XBAR_SetMDBXbarInSrc(xbar, mdbXbarx, mdbXbarSrc);
    return BASE_STATUS_OK;
}

/**
  * @brief Configure the OLUT Xbar input source which comes from the output of the MINDB module.
  *        The output of MINDB is the OC_PWMx when MindB is disabled.
  * @param olutXbarx Output lookup table xbar-x event.
  * @param olutXbarSrc Input source of OLUT Xbar-x.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_XBAR_ConfigOLUTXbarEx(XBAR_OLUTXbarEvent olutXbarx, XBAR_OLUTSrc olutXbarSrc)
{
    XBAR_RegStruct *xbar = XBAR;
    /* Set olut xbar input source */
    DCL_XBAR_SetOLUTXbarInSrc(xbar, olutXbarx, olutXbarSrc);
    return BASE_STATUS_OK;
}
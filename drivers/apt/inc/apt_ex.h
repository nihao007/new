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
  * @file    apt_ex.h
  * @author  MCU Driver Team
  * @brief   APT module driver.
  * @details This file provides extend functions declaration of the APT module.
  *           + Initialization and de-initialization functions.
  *           + APT Service Functions.
  */

#ifndef McuMagicTag_APT_EX_H
#define McuMagicTag_APT_EX_H

#include "apt.h"
#include "typedefs.h"

/**
  * @brief Parameters for configuring the HRPWM function.
  */
typedef struct {
    unsigned char  cntPrdHr;    /**< High resolution period. */
    unsigned char  cntPhsHr;    /**< High resolution phase. */
    unsigned char  cntDbHr;     /**< High resolution deadband. */
    unsigned char  cntCmpAHr;   /**< High resolution compare point A. */
    unsigned char  cntCmpBHr;   /**< High resolution compare point B. */
    unsigned char  cntCmpCHr;   /**< High resolution compare point C. */
    unsigned char  cntCmpDHr;   /**< High resolution compare point D. */
} APT_HRPWMParam;

/**
  * @brief Parameters for configuring the CHOPPER function.
  */
typedef struct {
    unsigned char psclkDiv8x; /**< 5bit: value = 0/1/2/.../31 ==> function = 8x/16x/24x/.../256x division. */
    unsigned char initWidth;  /**< 4bit: value = 0/1/2/.../15 ==> function = 1/2/3/.../16 PSCLK. */
    unsigned char shape;      /**< 8bit: value = 0b_ _ _ _ _ _ _ _ ==> max = 0b11111111, 100%. */
} APT_ChopperParam;

/**
  * @brief Parameters for configuring the BURST function.
  */
typedef struct {
    unsigned char brstPeriod;         /**< Burst period: m pwm carry periods. */
    unsigned char closedPwmPeriod;   /**< Output low period: n pwm carry periods. */
    APT_BurstClosedPol  closedPwmPol;      /**< Polarity of the closed pwm. */
    APT_BurstMode bstMode;           /**< Burst mode: pwm first or low level first. */
    APT_BufferLoadMode bstLoadMode;  /**< Burst buffer load mode. */
    APT_BurstLoadEvent bstLoadEvent; /**< Burst buffer load event. */
} APT_BurstParam;

/**
  * @brief Parameters for configuring the MINDB function.
  */
typedef struct {
    APT_MDBRefSig mdbRefSig;             /**< Reference signal selection. */
    APT_MDBRefSigPol mdbRefSigPol;       /**< Polarity of reference signal selection. */
    APT_MDBBlkSigDelayMode mdbDelayMode; /**< MDB delay mode: rising edge, falling edge, double edge, filter. */
    unsigned short delay;                /**< MDB delay time, unit: APT_CLK, range:0-65535 */
    APT_MDBBlkSig mdbBlkSigSel;          /**< MDB block signal selection of current channel. */
    APT_MDBOutSigPol outputPol;          /**< MDB output plarity. */
} APT_MDBParam;

/**
  * @brief Parameters for configuring the OLUT function.
  */
typedef struct {
    XBAR_OLUTXbarEvent in3Sig;        /**< Input signal of IN3. */
    /**
     * Output truth table of 8 combination (IN3, IN2, IN1).
     *   IN3(selected): select from XBAR_OLUTXbarEvent.
     *   IN2(fixed): APTx_MINDB of complementary channel B/A.
     *   IN1(fixed): APTx_MINDB of current channel  A/B.
     */
    bool outTable[APT_OLUT_OUTPUT_COMBINATION_MAX_NUM];
} APT_OLUTParam;

/**
  * @brief POEx definition.
  */
typedef enum {
    APT_POE0 = 0,
    APT_POE1,
    APT_POE2,
    APT_POE3,
    APT_POE4,
    APT_POE5,
    APT_POE6,
    APT_POE7,
    APT_POE8,
    APT_POE9,
    APT_POE10,
    APT_POE11,
    APT_POE12,
    APT_POE13,
    APT_POE14,
    APT_POE15,
    APT_POE_MAX_NUMS,
} APT_POEx;
/**
  * @defgroup APT_API_Declaration
  * @brief APT HAL API.
  * @{
  */
/* HRPWM Part */
BASE_StatusType HAL_APT_HRPWMInitEx(APT_Handle *aptHandle, APT_HRPWMParam *hrParam);
BASE_StatusType HAL_APT_HRPWMDeInitEx(APT_Handle *aptHandle);
BASE_StatusType HAL_APT_SetHRPWMPrdEx(APT_Handle *aptHandle, \
                                      unsigned short prdLr, \
                                      unsigned char prdHr, \
                                      APT_BufferLoadMode prdLoadMode, \
                                      APT_PrdLoadEvent prdLoadEvt);
BASE_StatusType HAL_APT_SetHRPWMPhsEx(APT_Handle *aptHandle, unsigned short phsLr, unsigned char phsHr);
BASE_StatusType HAL_APT_ConfigHRPWMRefDotEx(APT_Handle *aptHandle, APT_RefDotSelect refDotSelect, \
                                            APT_RefDotParameters *refDotParameters);
BASE_StatusType HAL_APT_SetHRPWMDutyEx(APT_Handle *aptHandle, \
                                       unsigned short cntCmpLeftLr, \
                                       unsigned char  cntCmpLeftHr, \
                                       unsigned short cntCmpRightLr, \
                                       unsigned char  cntCmpRightHr);
BASE_StatusType HAL_APT_SetHRPWMDutyFEx(APT_Handle *aptHandle, float dutyF);
BASE_StatusType HAL_APT_SetHRPWMADCTriggerTimeEx(APT_Handle *aptHandle, \
                                                 unsigned short cntCmpSOCALr, \
                                                 unsigned char cntCmpSOCAHr, \
                                                 unsigned short cntCmpSOCBLr, \
                                                 unsigned char cntCmpSOCBHr);

/* CHOPPER Part */
BASE_StatusType HAL_APT_ChopperInitEx(APT_Handle *aptHandle, APT_ChopperParam *chpParam);
BASE_StatusType HAL_APT_SetChopperShapeEx(APT_Handle *aptHandle, unsigned char initWidth, unsigned char shape);
BASE_StatusType HAL_APT_ChopperEnableEx(APT_Handle *aptHandle, APT_PWMChannel ch, bool en);

/* BURST Part */
BASE_StatusType HAL_APT_BurstInitEx(APT_Handle *aptHandle, APT_PWMChannel ch, APT_BurstParam *brstParam);
BASE_StatusType HAL_APT_SetBurstMNEx(APT_Handle *aptHandle, \
                                     APT_PWMChannel ch, \
                                     unsigned char brstPeriod, \
                                     unsigned char closedPwmPeriod);
BASE_StatusType HAL_APT_BurstEnableEx(APT_Handle *aptHandle, APT_PWMChannel ch, bool en);

/* MINDB Part */
BASE_StatusType HAL_APT_MDBInitEx(APT_Handle *aptHandle, APT_PWMChannel ch, APT_MDBParam *mdbParam);
BASE_StatusType HAL_APT_MDBDeInitEx(APT_Handle *aptHandle, APT_PWMChannel ch);
BASE_StatusType HAL_APT_MDBEnableEx (APT_Handle *aptHandle, APT_PWMChannel ch, bool en);

/* OLUT Part */
BASE_StatusType HAL_APT_OLUTInitEx(APT_Handle *aptHandle, APT_PWMChannel ch, APT_OLUTParam *olutParam);
BASE_StatusType HAL_APT_OLUTDeInitEx(APT_Handle *aptHandle, APT_PWMChannel ch);
BASE_StatusType HAL_APT_OLUTEnableEx(APT_Handle *aptHandle, APT_PWMChannel ch, bool en);

/* LINK Part */
BASE_StatusType HAL_APT_SetLinkEventAndSrcEx(APT_Handle *aptHandle, APT_LinkSrc aptLinkSrc, APT_LinkEvent linkEvt);

/* DELAY PROTECT and DELAY RECOVERTY Part */
BASE_StatusType HAL_APT_SetCbc2OshtEx(APT_Handle *aptHandle, unsigned int cbc2OshtTime, unsigned int cbcClrTime);
BASE_StatusType HAL_APT_SetDelayProtEx(APT_Handle *aptHandle, \
                                       APT_OutCtrlMode ocMode, \
                                       APT_PWMChannel ch, \
                                       unsigned short protDlyAptClk);
BASE_StatusType HAL_APT_SetDelayRecyEx(APT_Handle *aptHandle, \
                                       APT_OutCtrlMode ocMode, \
                                       APT_PWMChannel ch, \
                                       unsigned short recyDlyAptClk);
BASE_StatusType HAL_APT_SetOshtAlignEx(APT_Handle *aptHandle, APT_OshtAlignEvent alignEvt, APT_PWMChannel ch);
BASE_StatusType HAL_APT_DisableDelayProtEx(APT_Handle *aptHandle, APT_OutCtrlMode ocMode, APT_PWMChannel ch);
BASE_StatusType HAL_APT_DisableDelayRecyEx(APT_Handle *aptHandle, APT_OutCtrlMode ocMode, APT_PWMChannel ch);

/* Status */
BASE_StatusType HAL_APT_SetPoeFilterCycleEx(APT_POEx poex, unsigned char fltCycle);
APT_PwmStatus HAL_APT_GetPwmStatus(APT_Handle *aptHandle, APT_PWMChannel pwmChannel);
APT_TripStatus HAL_APT_GetTripStatus(APT_Handle *aptHandle, XBAR_TripEvent tripx);

/* XBAR Part */
BASE_StatusType HAL_XBAR_SetTripXbarInPolEx(XBAR_TripXbarEvt xbarEvt, XBAR_InputPolarity inPol);
BASE_StatusType HAL_XBAR_ConfigTripXbarEx(XBAR_TripEvent tripx, \
                                          XBAR_TripXbarEvt xbarEvt, \
                                          XBAR_TripOutPolarity tripOutPol);
BASE_StatusType HAL_XBAR_ConfigMDBXbarEx(XBAR_MDBXbarEvent mdbXbarx, XBAR_MDBSrc mdbXbarSrc);
BASE_StatusType HAL_XBAR_ConfigOLUTXbarEx(XBAR_OLUTXbarEvent olutXbarx, XBAR_OLUTSrc olutXbarSrc);

#endif
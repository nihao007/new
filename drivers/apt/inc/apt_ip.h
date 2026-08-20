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
  * @file    apt_ip.h
  * @author  MCU Driver Team
  * @brief   Header file containing APT module DCL driver functions.
  *          This file provides functions to manage the following functionalities of APT module.
  *          + Definition of APT configuration parameters.
  *          + APT registers mapping structure.
  *          + Direct Configuration Layer driver functions.
  */

#ifndef McuMagicTag_APT_IP_H
#define McuMagicTag_APT_IP_H

#include "baseinc.h"

#ifdef APT_PARAM_CHECK
    #define APT_ASSERT_PARAM            BASE_FUNC_ASSERT_PARAM
    #define APT_PARAM_CHECK_NO_RET      BASE_FUNC_PARAMCHECK_NO_RET
    #define APT_PARAM_CHECK_WITH_RET    BASE_FUNC_PARAMCHECK_WITH_RET
#else
    #define APT_ASSERT_PARAM(para)               ((void)0U)
    #define APT_PARAM_CHECK_NO_RET(para)         ((void)0U)
    #define APT_PARAM_CHECK_WITH_RET(param, ret) ((void)0U)
#endif


#define RERF   4
#define MAX_DUTY 100
#define ALL_EVT_INT_FLAGS 0xFFF70000U

#define APT_UNLOCK_REG_CMD    0x55
#define APT_LOCK_REG_CMD      0x00

#define XBAR_UNLOCK_REG_CMD    0x55
#define XBAR_LOCK_REG_CMD      0x00

/**
  * @defgroup APT_IP APT_IP
  * @brief APT_IP: apt_v3.
  * @{
  */

/**
 * @defgroup APT_Param_Def APT Parameters Definition
 * @brief Definition of APT configuration parameters
 * @{
 */

/* Bitmask of the aptx_run bits in SYSCTRL1 register. */
#define RUN_APT0                        0x00000001U
#define RUN_APT1                        0x00000002U
#define RUN_APT2                        0x00000004U
#define RUN_APT3                        0x00000008U
#define RUN_APT4                        0x00000010U
#define RUN_APT5                        0x00000020U
#define RUN_APT6                        0x00000040U
#define RUN_APT7                        0x00000080U
#define RUN_APT8                        0x00000100U
#define RUN_APT9                        0x00000200U
#define RUN_APT10                       0x00000400U
#define RUN_APT11                       0x00000800U
#define RUN_APT12                       0x00001000U
#define RUN_APT13                       0x00002000U
#define RUN_APT14                       0x00004000U
#define RUN_APT15                       0x00008000U

/* Limited values for some configuration items of APT module. */
#define DIVIDER_FACTOR_MAX              0x00000FFFU
#define TIMEBASE_COUNTER_MAX            0x0000FFFFU
#define TIMER_INTERRUPT_CNT_MAX         0x0000000FU
#define ADC_CONVERSION_START_CNT_MAX    0x0000000FU
#define VCAP_STARY_STOP_EDGE_CNT_MAX    0x0000000FU
#define EDGE_FILTER_EDGE_CNT_MAX        0x0000000FU
#define CNTR_SYNC_SOURCE_MAX            0x00000007U
#define SYNC_OUT_SOURCE_MAX             0x000000FFU
#define GLOBAL_LOAD_CNT_MAX             0x0000000FU

/* Safe bouendary of hrpwm compare point: width >= 7, set 8. */
#define APT_HRPWM_SAFE_BOUNDARY_UP_DOWN_MODE   4
#define APT_HRPWM_SAFE_BOUNDARY_UP_MODE        8

/* Chopper limitation */
#define APT_CHOPPER_PSCLK_DIV_MAX_APTCLK   0x1F   /* 8 * 0x1F = 256 division. */
#define APT_CHOPPER_INIT_WIDTH_MAX_PSCLK   0xF
#define APT_CHOPPER_SHAPE_CONFIG_MAX       0xFF

#define APT_BURST_LOAD_SHIFT_8BIT             8
#define APT_OLUT_OUTPUT_COMBINATION_MAX_NUM   8

/**
  * @brief Buffer load event of period register.
  */
typedef enum {
    APT_PERIOD_LOAD_EVENT_ZERO      = 0x00000001U,
    APT_PERIOD_LOAD_EVENT_A1        = 0x00000004U,
    APT_PERIOD_LOAD_EVENT_B1        = 0x00000008U,
    APT_PERIOD_LOAD_EVENT_SYNC      = 0x00000010U,
    APT_PERIOD_LOAD_EVENT_MAX_VALUE = 0x0000001FU,
} APT_PrdLoadEvent;

/**
  * @brief The buffer load events of TC_REFA, TC_REFB, TC_REFC, TC_REFD register.
  */
typedef enum {
    APT_COMPARE_LOAD_EVENT_ZERO          =  0x00000001U,
    APT_COMPARE_LOAD_EVENT_PERIOD        =  0x00000002U,
    APT_COMPARE_LOAD_EVENT_ZERO_PERIOD   =  0x00000003U,
    APT_COMPARE_LOAD_EVENT_A1            =  0x00000004U,
    APT_COMPARE_LOAD_EVENT_B1            =  0x00000008U,
    APT_COMPARE_LOAD_EVENT_SYNC          =  0x00000010U,
    APT_COMPARE_LOAD_EVENT_MAX_VALUE     =  0x0000001FU,
} APT_CmpLoadEvent;

/**
  * @brief Counter direction that can be returned by DCL_APT_GetCounterDirection().
  */
typedef enum {
    APT_COUNTER_STATUS_COUNT_DOWN        = 0x00000000U,
    APT_COUNTER_STATUS_COUNT_UP          = 0x00000001U,
} APT_CounterDir;

/**
  * @brief The buffer independent load events of PG_ACT_A or PG_ACT_B register.
  */
typedef enum {
    APT_ACTION_LOAD_EVENT_ZERO       =  0x00000001U,
    APT_ACTION_LOAD_EVENT_PERIOD     =  0x00000002U,
    APT_ACTION_LOAD_EVENT_A1         =  0x00000004U,
    APT_ACTION_LOAD_EVENT_B1         =  0x00000008U,
    APT_ACTION_LOAD_EVENT_SYNC       =  0x00000010U,
    APT_ACTION_LOAD_EVENT_MAX_VALUE  =  0x0000001FU,
} APT_ActLoadEvent;

/**
  * @brief The independent buffer load events of PG_ACT_FRC register.
  */
typedef enum {
    APT_ACTION_FORCE_LOAD_EVENT_ZERO    = 0x00010000U,
    APT_ACTION_FORCE_LOAD_EVENT_PERIOD  = 0x00020000U,
    APT_ACTION_FORCE_LOAD_EVENT_SYNC    = 0x00100000U,
} APT_FrcActLoadEvent;

/**
  * @brief Buffer load event for DG_CFG, DG_FED, DG_RED register.
  */
typedef enum {
    APT_DEAD_BAND_LOAD_EVENT_ZERO    = 0x00000001U,
    APT_DEAD_BAND_LOAD_EVENT_PERIOD  = 0x00000002U,
} APT_DgLoadEvent;

/**
  * @brief Emulation stop mode of APT module.
  */
typedef enum {
    APT_EMULATION_NO_STOP               = 0x00000001U,
    APT_EMULATION_STOP_COUNTER          = 0x00000002U,
    APT_EMULATION_STOP_APT              = 0x00000003U,
} APT_EmulationMode;

/**
  * @brief Count mode of time-base counter.
  */
typedef enum {
    APT_COUNT_MODE_UP                   = 0x00000000U,
    APT_COUNT_MODE_DOWN                 = 0x00000001U,
    APT_COUNT_MODE_UP_DOWN              = 0x00000002U,
    APT_COUNT_MODE_FREEZE               = 0x00000003U,
} APT_CountMode;

/**
  * @brief Count mode after synchronization for slave APT module.
  */
typedef enum {
    APT_COUNT_MODE_AFTER_SYNC_DOWN      = 0x00000000U,
    APT_COUNT_MODE_AFTER_SYNC_UP        = 0x00000001U,
} APT_SyncCountMode;

/**
  * @brief Count compare reference of time-base counter.
  */
typedef enum {
    APT_COMPARE_REFERENCE_A             = 0x00000000U,
    APT_COMPARE_REFERENCE_B             = 0x00000001U,
    APT_COMPARE_REFERENCE_C             = 0x00000002U,
    APT_COMPARE_REFERENCE_D             = 0x00000003U,
} APT_CompareRef;

/**
  * @brief PWM waveform output channel.
  */
typedef enum {
    APT_PWM_CHANNEL_A                   = 0x00000000U,
    APT_PWM_CHANNEL_B                   = 0x00000001U,
} APT_PWMChannel;

/**
  * @brief PWM waveform action on PWM action events.
  */
typedef enum {
    APT_PWM_ACTION_HOLD                 = 0x00000000U,
    APT_PWM_ACTION_LOW                  = 0x00000001U,
    APT_PWM_ACTION_HIGH                 = 0x00000002U,
    APT_PWM_ACTION_TOGGLE               = 0x00000003U,
} APT_PWMAction;

/**
  * @brief Count compare event for generating PWM waveform actions.
  *        The enumeration values are the register bit field offset of the corresponding action events.
  */
typedef enum {
    APT_PWM_ACTION_ON_TIMEBASE_ZERO     = 0U,
    APT_PWM_ACTION_ON_TIMEBASE_PERIOD   = 2U,
    APT_PWM_ACTION_ON_CMPA_COUNT_UP     = 4U,
    APT_PWM_ACTION_ON_CMPA_COUNT_DOWN   = 6U,
    APT_PWM_ACTION_ON_CMPB_COUNT_UP     = 8U,
    APT_PWM_ACTION_ON_CMPB_COUNT_DOWN   = 10U,
    APT_PWM_ACTION_ON_CMPC_COUNT_UP     = 12U,
    APT_PWM_ACTION_ON_CMPC_COUNT_DOWN   = 14U,
    APT_PWM_ACTION_ON_CMPD_COUNT_UP     = 16U,
    APT_PWM_ACTION_ON_CMPD_COUNT_DOWN   = 18U,
    APT_PWM_ACTION_ON_C1_COUNT_UP       = 20U,
    APT_PWM_ACTION_ON_C1_COUNT_DOWN     = 22U,
    APT_PWM_ACTION_ON_C2_COUNT_UP       = 24U,
    APT_PWM_ACTION_ON_C2_COUNT_DOWN     = 26U,
} APT_PWMActionEvent;

/**
  * @brief PWM action when using software continuous action.
  */
typedef enum {
    APT_PWM_CONTINUOUS_ACTION_HOLD      = 0x00000000U,
    APT_PWM_CONTINUOUS_ACTION_LOW       = 0x00000001U,
    APT_PWM_CONTINUOUS_ACTION_HIGH      = 0x00000002U,
} APT_PWMContAction;

/**
  * @brief PWM Generation event C1 and C2.
  */
typedef enum {
    APT_PWM_GENERATION_EVENT_C1         = 0x00000000U,
    APT_PWM_GENERATION_EVENT_C2         = 0x00000001U,
} APT_PGEventCx;

/**
  * @brief Source of PWM Generation event C1 and C2.
  * IO_EVENT1/2/3 = TRIP1/2/3
  */
typedef enum {
    APT_PG_EVT_C_FORBIDDEN              = 0x00000000U,
    APT_PG_EVT_C_COMBINE_EVENT_A1       = 0x00000001U,
    APT_PG_EVT_C_COMBINE_EVENT_A2       = 0x00000002U,
    APT_PG_EVT_C_COMBINE_EVENT_B1       = 0x00000003U,
    APT_PG_EVT_C_COMBINE_EVENT_B2       = 0x00000004U,
    APT_PG_EVT_C_COMBINE_EVENT_FILT     = 0x00000005U,
    APT_PG_EVT_C_IO_EVENT1              = 0x00000006U,
    APT_PG_EVT_C_IO_EVENT2              = 0x00000007U,
    APT_PG_EVT_C_IO_EVENT3              = 0x00000008U,
    APT_PG_EVT_C_SYNC_IN                = 0x00000009U,
} APT_PGEventCxSrc;

/**
  * @brief Input source of Dead-Band rising edge delay counter.
  * @details Input source:
  *         + APT_DB_RED_INPUT_PWM_A -- Dead-Band rising edge delay input is PWM channel A
  *         + APT_DB_RED_INPUT_PWM_B -- Dead-Band rising edge delay input is PWM channel B
  */
typedef enum {
    APT_DB_RED_INPUT_PWM_A              = 0x00000000U,
    APT_DB_RED_INPUT_PWM_B              = 0x00000001U,
} APT_REDInput;

/**
  * @brief Output mode of Dead-Band rising edge delay counter.
  * @details Output mode:
  *         + APT_DB_RED_OUTPUT_NOT_INVERT -- Dead-Band rising edge delay output is not inverted
  *         + APT_DB_RED_OUTPUT_INVERT -- Dead-Band rising edge delay output is inverted
  *         + APT_DB_RED_OUTPUT_PWM_A -- Dead-Band rising edge delay is bypassed
  */
typedef enum {
    APT_DB_RED_OUTPUT_NOT_INVERT        = 0x00000000U,
    APT_DB_RED_OUTPUT_INVERT            = 0x00000002U,
    APT_DB_RED_OUTPUT_PWM_A             = 0x00000003U,
} APT_REDOutMode;

/**
  * @brief Input source of Dead-Band falling edge delay counter.
  * @details Input source:
  *         + APT_DB_FED_INPUT_PWM_B -- Dead-Band falling edge delay input is PWM channel B
  *         + APT_DB_FED_INPUT_PWM_A -- Dead-Band falling edge delay input is PWM channel A
  *         + APT_DB_FED_INPUT_RED_OUT -- Falling edge delay input is rising edge delay output
  *         + APT_DB_FED_INPUT_ZERO -- Dead-Band falling edge delay input is 0
  */
typedef enum {
    APT_DB_FED_INPUT_PWM_B              = 0x00000000U,
    APT_DB_FED_INPUT_PWM_A              = 0x00000001U,
    APT_DB_FED_INPUT_RED_OUT            = 0x00000002U,
    APT_DB_FED_INPUT_ZERO               = 0x00000003U,
} APT_FEDInput;

/**
  * @brief Output mode of Dead-Band falling edge delay counter.
  * @details Output mode:
  *         + APT_DB_FED_OUTPUT_NOT_INVERT -- Dead-Band falling edge delay output is not inverted
  *         + APT_DB_FED_OUTPUT_INVERT -- Dead-Band falling edge delay output is inverted
  *         + APT_DB_FED_OUTPUT_PWM_B -- Dead-Band falling edge delay is bypassed
  */
typedef enum {
    APT_DB_FED_OUTPUT_NOT_INVERT        = 0x00000000U, /**< Dead-Band falling edge delay output is not inverted */
    APT_DB_FED_OUTPUT_INVERT            = 0x00000002U, /**< Dead-Band falling edge delay output is inverted */
    APT_DB_FED_OUTPUT_PWM_B             = 0x00000003U, /**< Dead-Band falling edge delay is bypassed */
} APT_FEDOutMode;

/**
  * @brief Burst mode definition.
  */
typedef enum {
    APT_BURST_MODE_OFF_FIRST = 0x0U,
    APT_BURST_MODE_PWM_FIRST = 0x1U,
} APT_BurstMode;

/**
  * @brief Burst closed pwm polarity.
  */
typedef enum {
    APT_BURST_CLOSED_PWM_POL_LOW  = 0x00000000U,
    APT_BURST_CLOSED_PWM_POL_HIGH = 0x00000001U,
} APT_BurstClosedPol;

/**
  * @brief Buffer load mode of the registers that support buffer register.
  * @details Load mode:
  *          + APT_BUFFER_DISABLE -- Disable register buffer
  *          + APT_BUFFER_INDEPENDENT_LOAD -- Enable register buffer and load independently
  *          + APT_BUFFER_GLOBAL_LOAD -- enable register buffer and load globally
  */
typedef enum {
    APT_BUFFER_DISABLE                  = 0x00000000U,
    APT_BUFFER_INDEPENDENT_LOAD         = 0x00000001U,
    APT_BUFFER_GLOBAL_LOAD              = 0x00000003U,
} APT_BufferLoadMode;

/**
  * @brief Burst load event.
  */
typedef enum {
    APT_BURST_LOAD_EVENT_DISABLE = 0x00000000U,
    APT_BURST_LOAD_EVENT_ZERO    = 0x00000001U,
    APT_BURST_LOAD_EVENT_PERIOD  = 0x00000002U,
} APT_BurstLoadEvent;

/**
 * @brief Obtain the PWM status of the current burst output.
 */
typedef enum {
    APT_BURST_OUTPUT_PWM   = 0x0U,
    APT_BURST_OUTPUT_LOW   = 0x1U,
    APT_BURST_OUTPUT_ERROR = 0x2U,
} APT_BurstOutputStatus;

/**
  * @brief Output control events.
  */
typedef enum {
    APT_OC_NO_EVENT                     = 0x00000000U,
    APT_OC_TRIP_EVENT_1                 = 0x00000001U,
    APT_OC_TRIP_EVENT_2                 = 0x00000002U,
    APT_OC_TRIP_EVENT_3                 = 0x00000004U,
    APT_OC_SYSTEM_EVENT_1               = 0x00000100U,  /* Debug_mode */
    APT_OC_SYSTEM_EVENT_2               = 0x00000200U,  /* Clock_fail */
    APT_OC_SYSTEM_EVENT_3               = 0x00000400U,  /* Sys_mem_fail */
    APT_OC_SYSTEM_EVENT_4               = 0x00000800U,  /* Pvd_toggle */
    APT_OC_COMBINE_EVENT_A1             = 0x00010000U,
    APT_OC_COMBINE_EVENT_A2             = 0x00020000U,
    APT_OC_COMBINE_EVENT_B1             = 0x00040000U,
    APT_OC_COMBINE_EVENT_B2             = 0x00080000U,
    APT_OC_COMBINE_EVENT_MAX_VALUE      = 0x000FFFFFU,
} APT_OutCtrlEvent;

/**
  * @brief Output control event mode.
  */
typedef enum {
    APT_OUT_CTRL_CYCLE_BY_CYCLE         = 0x00000001U,
    APT_OUT_CTRL_ONE_SHOT               = 0x00000010U,
    APT_OUT_CTRL_FREE                   = 0x00000100U,
} APT_OutCtrlMode;

/**
  * @brief Advanced output control events take into consideration of the direction of time-base counter.
  *        The enumeration values are the register bit field offset of the corresponding output control events.
  */
typedef enum {
    APT_OC_EVT_OSHT_OR_CBC_OR_FREE_INC   = 0U,
    APT_OC_EVT_COMBINE_EVENT_A1_INC      = 3U,
    APT_OC_EVT_COMBINE_EVENT_A2_INC      = 6U,
    APT_OC_EVT_COMBINE_EVENT_B1_INC      = 9U,
    APT_OC_EVT_COMBINE_EVENT_B2_INC      = 12U,
    APT_OC_EVT_OSHT_OR_CBC_OR_FREE_DEC = 16U,
    APT_OC_EVT_COMBINE_EVENT_A1_DEC    = 19U,
    APT_OC_EVT_COMBINE_EVENT_A2_DEC    = 22U,
    APT_OC_EVT_COMBINE_EVENT_B1_DEC    = 25U,
    APT_OC_EVT_COMBINE_EVENT_B2_DEC    = 28U,
} APT_OutCtrlEventDir;

/**
  * @brief Output control action.
  * @details Control action:
  *         + APT_OUT_CTRL_ACTION_DISABLE -- Disable output protect control. Output PWM directly
  *         + APT_OUT_CTRL_ACTION_LOW -- Output low level
  *         + APT_OUT_CTRL_ACTION_HIGH -- Output high level
  *         + APT_OUT_CTRL_ACTION_HOLD -- Hold the current output state
  *         + APT_OUT_CTRL_ACTION_TOGGLE -- Toggle the current output state
  *         + APT_OUT_CTRL_ACTION_HIGH_Z -- High-impedance output
  */
typedef enum {
    APT_OUT_CTRL_ACTION_DISABLE         = 0x00000000U,
    APT_OUT_CTRL_ACTION_LOW             = 0x00000001U,
    APT_OUT_CTRL_ACTION_HIGH            = 0x00000002U,
    APT_OUT_CTRL_ACTION_HOLD            = 0x00000003U,
    APT_OUT_CTRL_ACTION_TOGGLE          = 0x00000004U,
    APT_OUT_CTRL_ACTION_HIGH_Z          = 0x00000005U,
} APT_OutCtrlAction;

/**
  * @brief Output control fault flag.
  */
typedef enum {
    APT_OC_FAULT_EVENT_CBC               = 0x00000001U,
    APT_OC_FAULT_EVENT_OSHT              = 0x00000002U,
    APT_OC_FAULT_EVENT_FREE              = 0x00000004U,
    APT_OC_FAULT_EVENT_SYSTEM_EVENT_1    = 0x00000010U,  /* Debug_mode */
    APT_OC_FAULT_EVENT_SYSTEM_EVENT_2    = 0x00000020U,  /* Clock_fail */
    APT_OC_FAULT_EVENT_SYSTEM_EVENT_3    = 0x00000040U,  /* Sys_mem_fail */
    APT_OC_FAULT_EVENT_SYSTEM_EVENT_4    = 0x00000080U,  /* Pvd_toggle */
    APT_OC_FAULT_EVENT_COMBINE_EVTA1     = 0x00000100U,
    APT_OC_FAULT_EVENT_COMBINE_EVTA2     = 0x00000200U,
    APT_OC_FAULT_EVENT_COMBINE_EVTB1     = 0x00000400U,
    APT_OC_FAULT_EVENT_COMBINE_EVTB2     = 0x00000800U,
    APT_OC_FAULT_EVENT_MAX_CBC_CHA       = 0x00001000U,
    APT_OC_FAULT_EVENT_MAX_CBC_CHB       = 0x00002000U,
    APT_OC_FAULT_EVENT_MAX_FREE_CHA      = 0x00004000U,
    APT_OC_FAULT_EVENT_MAX_FREE_CHB      = 0x00008000U,
    APT_OC_FAULT_EVENT_MAX_VALUE         = 0x0000FFFFU,
} APT_EventInterruptSrc;

/**
 * @brief Hardware clear CBC protection action at zero or period.
 */
typedef enum {
    APT_OC_CLEAR_CBC_PROTECT_EVENT_ZERO         = 0x00000001U,
    APT_OC_CLEAR_CBC_PROTECT_EVENT_PERIOD       = 0x00010000U,
} APT_OutCtrlCbcClrMode;

/**
 * @brief Software clear protection action of CBC/OSHT/FREE .
 */
typedef enum {
    APT_SW_CLEAR_PROT_ACTION_CBC   = 0x00000001U,
    APT_SW_CLEAR_PROT_ACTION_OSHT  = 0x00000002U,
    APT_SW_CLEAR_PROT_ACTION_FREE  = 0x00000004U,
    APT_SW_CLEAR_PROT_ACTION_MAX_VALUE  = 0x00000007U,
} APT_ProtMode;

/**
 * @brief Force the APT to output fault signals.
 */
typedef enum {
    APT_OC_FORCE_GENERATE_CBC_PROTECTION    = 0x00000001U,
    APT_OC_FORCE_GENERATE_OSHT_PROTECTION   = 0x00000002U,
    APT_OC_FORCE_GENERATE_FREE_PROTECTION   = 0x00000004U,
    APT_OC_FORCE_GENERATE_EM_EVTA1_SIGNAL   = 0x00000100U,
    APT_OC_FORCE_GENERATE_EM_EVTA2_SIGNAL   = 0x00000200U,
    APT_OC_FORCE_GENERATE_EM_EVTB1_SIGNAL   = 0x00000400U,
    APT_OC_FORCE_GENERATE_EM_EVTB2_SIGNAL   = 0x00000800U,
    APT_OC_FORCE_GENERATE_SIGNAL_MAX_VALUE  = 0x00000FFFU,
} APT_OutCtrlFaultSig;

/**
  * @brief Alignment protection and recovery definition of OSHT mode.
  */
typedef enum {
    APT_OC_OSHT_ALIGNMENT_PROT_AT_CNTR_ZERO   = 0x00000020U,
    APT_OC_OSHT_ALIGNMENT_PROT_AT_CNTR_PERIOD = 0x00000030U,
    APT_OC_OSHT_ALIGNMENT_RECY_AT_CNTR_ZERO   = 0x00000080U,
    APT_OC_OSHT_ALIGNMENT_RECY_AT_CNTR_PERIOD = 0x000000C0U,
} APT_OshtAlignEvent;

/**
 * @brief Trigger source of event interrupt.
 */
typedef enum {
    APT_EVENT_INTERRUPT_MODE_RISING_EDGE   =   0x00000000U,
    APT_EVENT_INTERRUPT_MODE_HIGH_LEVEL    =   0x00000001U,
} APT_EventIntTrgMode;

/**
  * @brief Trigger source of timer interrupt.
  */
typedef enum {
    APT_TIMER_INTERRUPT_SRC_CNTR_DISABLE            = 0x00000000U,
    APT_TIMER_INTERRUPT_SRC_CNTR_ZERO               = 0x00000001U,
    APT_TIMER_INTERRUPT_SRC_CNTR_PERIOD             = 0x00000002U,
    APT_TIMER_INTERRUPT_SRC_CNTR_ZERO_PERIOD        = 0x00000003U,
    APT_TIMER_INTERRUPT_SRC_CNTR_CMPA_UP            = 0x00000004U,
    APT_TIMER_INTERRUPT_SRC_CNTR_CMPA_DOWN          = 0x00000005U,
    APT_TIMER_INTERRUPT_SRC_CNTR_CMPB_UP            = 0x00000006U,
    APT_TIMER_INTERRUPT_SRC_CNTR_CMPB_DOWN          = 0x00000007U,
    APT_TIMER_INTERRUPT_SRC_CNTR_CMPC_UP            = 0x00000008U,
    APT_TIMER_INTERRUPT_SRC_CNTR_CMPC_DOWN          = 0x00000009U,
    APT_TIMER_INTERRUPT_SRC_CNTR_CMPD_UP            = 0x0000000AU,
    APT_TIMER_INTERRUPT_SRC_CNTR_CMPD_DOWN          = 0x0000000BU,
    APT_TIMER_INTERRUPT_SRC_TMR_INT_MIX             = 0x0000000CU,
} APT_TimerInterruptSrc;

/**
  * @brief Signal source that triggers the INT_TMR_MIX.
  */
typedef enum {
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_ZERO           = 0x00000001U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_PERIOD         = 0x00000002U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_CMPA_UP        = 0x00000004U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_CMPA_DOWN      = 0x00000008U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_CMPB_UP        = 0x00000010U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_CMPB_DOWN      = 0x00000020U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_CMPC_UP        = 0x00000040U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_CMPC_DOWN      = 0x00000080U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_CMPD_UP        = 0x00000100U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_CMPD_DOWN      = 0x00000200U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_COMBINE_EVENT_A1    = 0x00000400U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_COMBINE_EVENT_A2    = 0x00000800U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_COMBINE_EVENT_B1    = 0x00001000U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_COMBINE_EVENT_B2    = 0x00002000U,
    APT_INT_TIMER_MIX_TRIGGER_SRC_MAX_VALUE           = 0x00003FFFU,
} APT_TmrMixTriggerSrc;

/**
  * @brief ADC trigger channels.
  */
typedef enum {
    APT_ADC_CONVERSION_START_A          = 0x00000001U,
    APT_ADC_CONVERSION_START_B          = 0x00000002U,
} APT_ADCTriggerChannel;

/**
  * @brief Source of ADC trigger channels.
  */
typedef enum {
    APT_CS_SRC_COMBINE_EVENT_A1         = 0x00000000U,
    APT_CS_SRC_CNTR_ZERO                = 0x00000001U,
    APT_CS_SRC_CNTR_PERIOD              = 0x00000002U,
    APT_CS_SRC_CNTR_ZERO_PERIOD         = 0x00000003U,
    APT_CS_SRC_CNTR_CMPA_UP             = 0x00000004U,
    APT_CS_SRC_CNTR_CMPA_DOWN           = 0x00000005U,
    APT_CS_SRC_CNTR_CMPB_UP             = 0x00000006U,
    APT_CS_SRC_CNTR_CMPB_DOWN           = 0x00000007U,
    APT_CS_SRC_CNTR_CMPC_UP             = 0x00000008U,
    APT_CS_SRC_CNTR_CMPC_DOWN           = 0x00000009U,
    APT_CS_SRC_CNTR_CMPD_UP             = 0x0000000AU,
    APT_CS_SRC_CNTR_CMPD_DOWN           = 0x0000000BU,
    APT_CS_SRC_SOC_SELF_MIX             = 0x0000000CU,
} APT_ADCTriggerSource;

/**
  * @brief Sampling source that triggers SOCx_MIX.
  */
typedef enum {
    APT_MIX_TRIGGER_SRC_CNTR_ZERO           = 0x00000001U,
    APT_MIX_TRIGGER_SRC_CNTR_PERIOD         = 0x00000002U,
    APT_MIX_TRIGGER_SRC_CNTR_CMPA_UP        = 0x00000004U,
    APT_MIX_TRIGGER_SRC_CNTR_CMPA_DOWN      = 0x00000008U,
    APT_MIX_TRIGGER_SRC_CNTR_CMPB_UP        = 0x00000010U,
    APT_MIX_TRIGGER_SRC_CNTR_CMPB_DOWN      = 0x00000020U,
    APT_MIX_TRIGGER_SRC_CNTR_CMPC_UP        = 0x00000040U,
    APT_MIX_TRIGGER_SRC_CNTR_CMPC_DOWN      = 0x00000080U,
    APT_MIX_TRIGGER_SRC_CNTR_CMPD_UP        = 0x00000100U,
    APT_MIX_TRIGGER_SRC_CNTR_CMPD_DOWN      = 0x00000200U,
    APT_MIX_TRIGGER_SRC_COMBINE_EVENT_A1    = 0x00000400U,
    APT_MIX_TRIGGER_SRC_COMBINE_EVENT_A2    = 0x00000800U,
    APT_MIX_TRIGGER_SRC_COMBINE_EVENT_B1    = 0x00001000U,
    APT_MIX_TRIGGER_SRC_COMBINE_EVENT_B2    = 0x00002000U,
    APT_MIX_TRIGGER_SRC_MAX_VALUE           = 0x00003FFFU,
} APT_ADCMixTriggerSrc;


/**
  * @brief DMA request source of ADC Converter Start submodule.
  */
typedef enum {
    APT_CS_DMA_REQ_SRC_DISABLE          = 0x00000000U,
    APT_CS_DMA_REQ_SRC_CHANNEL_A        = 0x00000001U,
    APT_CS_DMA_REQ_SRC_CHANNEL_B        = 0x00000002U,
} APT_ADCTrgDMAReqSrc;

/**
  * @brief DMA request type of ADC Converter Start submodule.
  */
typedef enum {
    APT_CS_DMA_BURST_REQUEST            = 0x00000000U,
    APT_CS_DMA_SINGLE_REQUEST           = 0x00000002U,
} APT_ADCTrgDMAReqType;

/**
  * @brief 15 trip events that can be selected to consist of OR combine events.
  */
typedef enum {
    APT_EM_OR_EN_TRIP1        =  0x00000001U,
    APT_EM_OR_EN_TRIP2        =  0x00000002U,
    APT_EM_OR_EN_TRIP3        =  0x00000004U,
    APT_EM_OR_EN_TRIP4        =  0x00000008U,
    APT_EM_OR_EN_TRIP5        =  0x00000010U,
    APT_EM_OR_EN_TRIP6        =  0x00000020U,
    APT_EM_OR_EN_TRIP7        =  0x00000040U,
    APT_EM_OR_EN_TRIP8        =  0x00000080U,
    APT_EM_OR_EN_TRIP9        =  0x00000100U,
    APT_EM_OR_EN_TRIP10       =  0x00000200U,
    APT_EM_OR_EN_TRIP11       =  0x00000400U,
    APT_EM_OR_EN_TRIP12       =  0x00000800U,
    APT_EM_OR_EN_TRIP13       =  0x00001000U,
    APT_EM_OR_EN_TRIP14       =  0x00002000U,
    APT_EM_OR_EN_TRIP15       =  0x00004000U,
    APT_EM_OR_EN_MAX_VALUE    =  0x00007FFFU,
} APT_EMTripSrc;

/**
  * @brief TRIP_OR event groups of Event Management submodule.
  */
typedef enum {
    APT_EM_TRIP_OR_A1                     = 0x00000000U,
    APT_EM_TRIP_OR_A2                     = 0x00000001U,
    APT_EM_TRIP_OR_B1                     = 0x00000002U,
    APT_EM_TRIP_OR_B2                     = 0x00000003U,
} APT_EMTripOrGrp;

/**
  * @brief Group of combine event source input.
  */
typedef enum {
    APT_EM_COMBINE_EVENT_A1_SRC     = 0x00000000U,
    APT_EM_COMBINE_EVENT_A2_SRC     = 0x00000001U,
    APT_EM_COMBINE_EVENT_B1_SRC     = 0x00000002U,
    APT_EM_COMBINE_EVENT_B2_SRC     = 0x00000003U,
} APT_EMCombineEvtSrcGrp;

/**
  * @brief Source of combine events trip1~15, A1, A2, B1, B2.
  */
typedef enum {
    APT_EM_COMBINE_SRC_TRIP_1            = 0x00000000U,
    APT_EM_COMBINE_SRC_TRIP_2            = 0x00000001U,
    APT_EM_COMBINE_SRC_TRIP_3            = 0x00000002U,
    APT_EM_COMBINE_SRC_TRIP_4            = 0x00000003U,
    APT_EM_COMBINE_SRC_TRIP_5            = 0x00000004U,
    APT_EM_COMBINE_SRC_TRIP_6            = 0x00000005U,
    APT_EM_COMBINE_SRC_TRIP_7            = 0x00000006U,
    APT_EM_COMBINE_SRC_TRIP_8            = 0x00000007U,
    APT_EM_COMBINE_SRC_TRIP_9            = 0x00000008U,
    APT_EM_COMBINE_SRC_TRIP_10           = 0x00000009U,
    APT_EM_COMBINE_SRC_TRIP_11           = 0x0000000AU,
    APT_EM_COMBINE_SRC_TRIP_12           = 0x0000000BU,
    APT_EM_COMBINE_SRC_TRIP_13           = 0x0000000CU,
    APT_EM_COMBINE_SRC_TRIP_14           = 0x0000000DU,
    APT_EM_COMBINE_SRC_TRIP_15           = 0x0000000EU,
    APT_EM_COMBINE_SRC_ALL_EVENT_OR      = 0x0000000FU, /* based on EM_AOR_EN/EM_BOR_EN */
} APT_EMCombineEvtSrc;

/**
  * @brief Combine events of Event Management submodule.
  */
typedef enum {
    APT_EM_COMBINE_EVENT_A1_TEMP             = 0x00000000U,
    APT_EM_COMBINE_EVENT_A2_TEMP             = 0x00000001U,
    APT_EM_COMBINE_EVENT_B1_TEMP             = 0x00000002U,
    APT_EM_COMBINE_EVENT_B2_TEMP             = 0x00000003U,
} APT_EMCombineEventTemp;

/**
  * @brief Combine Mode of combine events A1, A2, B1, B2.
  * @details combine mode:
  *         + The combine result is set output to low level
  *         + The combine result is qual to event 1
  *         + The combine result is the logical AND of group event 1 high level and group event 2 low level
  *         + The combine result is the logical AND of group event 1 high level and group event 2 low level
  *         + The combine result is the logical AND of group event 1 high level and group event 2 high level
  *         + The combine result is the logical AND of group event 1 low level and group event 2 low level
  */
typedef enum {
    APT_EM_COMBINE_LOW_LEVEL            = 0x00000000U,
    APT_EM_COMBINE_EVT1_H               = 0x00000001U,
    APT_EM_COMBINE_EVT1_H_AND_EVT2_L    = 0x00000002U,
    APT_EM_COMBINE_EVT1_H_AND_EVT2_H    = 0x00000003U,
    APT_EM_COMBINE_EVT1_L_AND_EVT2_H    = 0x00000004U,
    APT_EM_COMBINE_EVT2_H               = 0x00000005U,
} APT_EMCombineEvtMode;

/**
  * @brief Combine events of Event Management submodule.
  */
typedef enum {
    APT_EM_COMBINE_EVENT_A1             = 0x00000000U,
    APT_EM_COMBINE_EVENT_A2             = 0x00000001U,
    APT_EM_COMBINE_EVENT_B1             = 0x00000002U,
    APT_EM_COMBINE_EVENT_B2             = 0x00000003U,
} APT_EMCombineEvent;

/**
  * @brief Output type of combine events.
  * @details Output type:
  *          APT_EM_COMBINE_EVENT_OUT_ORIG_SIGNAL -- The source of combine event is unfiltered
  *          APT_EM_COMBINE_EVENT_OUT_FILT_SIGNAL -- The source of combine event is filtered
  */
typedef enum {
    APT_EM_COMBINE_EVENT_OUT_ORIG_SIGNAL = 0x00000000U,
    APT_EM_COMBINE_EVENT_OUT_FILT_SIGNAL = 0x00000001U,
} APT_EMCombineEventOut;

/**
  * @brief Polarity of mask window.
  */
typedef enum {
    APT_BLANK_EVENT_INSIDE_MASK_WIN_LOW      = 0x00000000U,
    APT_BLANK_EVENT_OUTSIDE_MASK_WIN_HIGH    = 0x00000001U,
} APT_MaskWinPolarity;

/**
  * @brief Reset mode of mask window and count capture.
  */
typedef enum {
    APT_RESET_MASK_WIN_DISABLE          = 0x00000000U,
    APT_RESET_MASK_WIN_CNTR_ZERO        = 0x00000001U,
    APT_RESET_MASK_WIN_CNTR_PERIOD      = 0x00000002U,
    APT_RESET_MASK_WIN_CNTR_ZERO_PERIOD = 0x00000003U,
} APT_MaskWinResetMode;

/**
  * @brief Clock source of valley capture.
  */
typedef enum {
    APT_VALLY_CAP_USE_MAIN_CLOCK        = 0x00000000U,
    APT_VALLEY_CAP_USE_DIVIDER_CLOCK    = 0x00000001U,
} APT_ValleyCapClkMode;

/**
  * @brief Trigger source of valley capture.
  */
typedef enum {
    APT_VALLEY_CAP_SRC_DISABLE               = 0x00000000U,
    APT_VALLEY_CAP_SRC_CNTR_ZERO             = 0x00000001U,
    APT_VALLEY_CAP_SRC_CNTR_PERIOD           = 0x00000002U,
    APT_VALLEY_CAP_SRC_CNTR_ZERO_PERIOD      = 0x00000003U,
    APT_VALLEY_CAP_SRC_COMBINE_EVENT_A1_TEMP = 0x00000004U,
    APT_VALLEY_CAP_SRC_COMBINE_EVENT_A2_TEMP = 0x00000005U,
    APT_VALLEY_CAP_SRC_COMBINE_EVENT_B1_TEMP = 0x00000006U,
    APT_VALLEY_CAP_SRC_COMBINE_EVENT_B2_TEMP = 0x00000007U,
} APT_ValleyCapRstType;

/**
  * @brief Edge type of valley capture.
  */
typedef enum {
    APT_VALLEY_CAP_RISING_EDGE          = 0x00000000U,
    APT_VALLEY_CAP_FALLING_EDGE         = 0x00000001U,
} APT_ValleyCapEdgeType;

/**
  * @brief Delay calibration of valley capture.
  * @details Delay calibration:
  *         + APT_VCAP_SW_DELAY -- Delay value = software delay value
  *         + APT_VCAP_VCNT_DELAY_DIVIDE_1_SW_DELAY -- Delay value = capture count value + software delay value
  *         + APT_VCAP_VCNT_DELAY_DIVIDE_2_SW_DELAY -- Delay value = capture count value / 2 + software delay value
  *         + APT_VCAP_VCNT_DELAY_DIVIDE_4_SW_DELAY -- Delay value = capture count value / 4 + software delay value
  *         + APT_VCAP_VCNT_DELAY_DIVIDE_8_SW_DELAY -- Delay value = capture count value / 8 + software delay value
  *         + APT_VCAP_VCNT_DELAY_DIVIDE_16_SW_DELAY -- Delay value = capture count value / 16 + software delay value
  *         + APT_VCAP_VCNT_DELAY_DIVIDE_32_SW_DELAY -- Delay value = capture count value / 32 + software delay value
  */
typedef enum {
    APT_VCAP_SW_DELAY                       = 0x00000000U,
    APT_VCAP_VCNT_DELAY_DIVIDE_1_SW_DELAY   = 0x00000001U,
    APT_VCAP_VCNT_DELAY_DIVIDE_2_SW_DELAY   = 0x00000002U,
    APT_VCAP_VCNT_DELAY_DIVIDE_4_SW_DELAY   = 0x00000003U,
    APT_VCAP_VCNT_DELAY_DIVIDE_8_SW_DELAY   = 0x00000004U,
    APT_VCAP_VCNT_DELAY_DIVIDE_16_SW_DELAY  = 0x00000005U,
    APT_VCAP_VCNT_DELAY_DIVIDE_32_SW_DELAY  = 0x00000006U,
} APT_ValleyDelayMode;

/**
  * @brief Start and stop edge of valley capture.
  */
typedef enum {
    APT_VALLEY_COUNT_RISING_EDGE           = 0x00000000U,
    APT_VALLEY_COUNT_FALLING_EDGE          = 0x00000001U,
} APT_ValleyCountEdge;

/**
  * @brief Edge filter mode of Event Management submodule.
  */
typedef enum {
    APT_EM_EDGEFILTER_MODE_RISING       = 0x00000000U,
    APT_EM_EDGEFILTER_MODE_FALLING      = 0x00000002U,
    APT_EM_EDGEFILTER_MODE_BOTH         = 0x00000003U,
} APT_EMEdgeFilterMode;

/**
  * @brief Sync-in source of slave APT module.
  */
typedef enum {
    APT_SYNCIN_SRC_APT0_SYNCOUT         = 0x00000000U,
    APT_SYNCIN_SRC_APT1_SYNCOUT         = 0x00000001U,
    APT_SYNCIN_SRC_APT2_SYNCOUT         = 0x00000002U,
    APT_SYNCIN_SRC_APT3_SYNCOUT         = 0x00000003U,
    APT_SYNCIN_SRC_APT4_SYNCOUT         = 0x00000004U,
    APT_SYNCIN_SRC_APT5_SYNCOUT         = 0x00000005U,
    APT_SYNCIN_SRC_APT6_SYNCOUT         = 0x00000006U,
    APT_SYNCIN_SRC_APT7_SYNCOUT         = 0x00000007U,
    APT_SYNCIN_SRC_APT8_SYNCOUT         = 0x00000008U,
    APT_SYNCIN_SRC_CAPM0_SYNCOUT        = 0x00000009U,
    APT_SYNCIN_SRC_CAPM1_SYNCOUT        = 0x0000000AU,
    APT_SYNCIN_SRC_CAPM2_SYNCOUT        = 0x0000000BU,
    APT_SYNCIN_SRC_TRIP4                = 0x0000000CU,
    APT_SYNCIN_SRC_TRIP5                = 0x0000000DU,
    APT_SYNCIN_SRC_APT9_SYNCOUT         = 0x0000000EU,
    APT_SYNCIN_SRC_APT10_SYNCOUT        = 0x0000000FU,
    APT_SYNCIN_SRC_APT11_SYNCOUT        = 0x00000010U,
    APT_SYNCIN_SRC_APT12_SYNCOUT        = 0x00000011U,
    APT_SYNCIN_SRC_APT13_SYNCOUT        = 0x00000012U,
    APT_SYNCIN_SRC_APT14_SYNCOUT        = 0x00000013U,
    APT_SYNCIN_SRC_APT15_SYNCOUT        = 0x00000014U,
    APT_SYNCIN_SRC_DISABLE              = 0x00000015U,
} APT_SyncInSrc;

/* Event for enabling count synchronization. */
typedef enum {
    APT_CNTR_SYNC_SRC_COMBINE_EVENT_A1 = 0x00000001U,  /* Enable combine event A1 as counter synchronization source. */
    APT_CNTR_SYNC_SRC_COMBINE_EVENT_B1 = 0x00000002U,  /* Enable combine event B1 as counter synchronization source. */
    APT_CNTR_SYNC_SRC_SYNCIN           = 0x00000004U,  /* Enable Sync-In source as counter synchronization source. */
    APT_CNTR_SYNC_SRC_MAX_VALUE        = 0x00000007U,  /* Max value that user can set. */
} APT_cntrSyncEvent;

/**
  * @brief Sync-out source of master APT module.
  */
typedef enum {
    APT_SYNC_OUT_ON_CNTR_ZERO            = 0x000000001U,
    APT_SYNC_OUT_ON_CNTR_PERIOD          = 0x000000002U,
    APT_SYNC_OUT_ON_COMBINE_EVENT_A1     = 0x000000004U,
    APT_SYNC_OUT_ON_COMBINE_EVENT_B1     = 0x000000008U,
    APT_SYNC_OUT_ON_CNTR_CMPB            = 0x000000020U,
    APT_SYNC_OUT_ON_CNTR_CMPC            = 0x000000040U,
    APT_SYNC_OUT_ON_CNTR_CMPD            = 0x000000080U,
    APT_SYNC_OUT_MAX_VALUE               = 0x0000000FFU,
} APT_SyncOutSrc;

/**
  * @brief Sync-out mode of master APT module.
  * @details Sync-out mode:
  *         + APT_SYNCOUT_ONE_SHOT_MODE -- One-Shot synchronization mode
  *         + APT_SYNCOUT_MULTIPLE_MODE -- Multiple synchronization mode
  */
typedef enum {
    APT_SYNCOUT_ONE_SHOT_MODE           = 0x00000000U,
    APT_SYNCOUT_MULTIPLE_MODE           = 0x00000001U,
} APT_SyncOutMode;

/**
  * @brief  Selection of sync-out latch when using one-shot sync-out mode.
  * @details Sync-out latch:
  *         + APT_SYNCOUT_LATCH_SET_ON_SW_FORCE -- Select rg_latset_otsyn as the latch set condition
  *         + APT_SYNCOUT_LATCH_SET_ON_GLB_LOAD -- Select rg_latset_otgld as the latch set condition
  */
typedef enum {
    APT_SYNCOUT_LATCH_SET_ON_SW_FORCE   = 0x00000000U,
    APT_SYNCOUT_LATCH_SET_ON_GLB_LOAD   = 0x00000001U,
} APT_SyncOutLatSetSel;

/**
 * @brief Source of peripheral synchronization output.
 * */
typedef enum {
    APT_PERIPHERAL_SYNCOUT_DISABLE        = 0x00000000U,
    APT_PERIPHERAL_SYNCOUT_CNTR_ZERO      = 0x00000001U,
    APT_PERIPHERAL_SYNCOUT_CNTR_PERIOD    = 0x00000002U,
    APT_PERIPHERAL_SYNCOUT_CNTR_CMPC_UP   = 0x00000003U,
    APT_PERIPHERAL_SYNCOUT_CNTR_CMPC_DOWN = 0x00000004U,
    APT_PERIPHERAL_SYNCOUT_CNTR_CMPD_UP   = 0x00000005U,
    APT_PERIPHERAL_SYNCOUT_CNTR_CMPD_DOWN = 0x00000006U,
} APT_PeriphSyncOutSrc;

/**
 * @brief Global loading trigger event.
 * */
typedef enum {
    APT_GLB_LOAD_ON_CNTR_ZERO        = 0x00000001U,  /* Global buffer load when counter equals zero. */
    APT_GLB_LOAD_ON_CNTR_PERIOD      = 0x00000002U,  /* Global buffer load when counter equals period. */
    APT_GLB_LOAD_ON_CNTR_SYNC        = 0x00000004U,  /* Global buffer load when counter sync is effective. */
    APT_GLB_LOAD_MAX_VALUE           = 0x00000007U,  /* Max value that user can set. */
} APT_GlbLoadEvent;

/**
  * @brief Global buffer load mode.
  */
typedef enum {
    APT_GLB_LOAD_ONE_SHOT_MODE          = 0x00000000U,
    APT_GLB_LOAD_MULTIPLE_MODE          = 0x00000001U,
} APT_GlobalLoadMode;

/**
  * @brief The buffer of the registers that support buffer register.
  */
typedef enum {
    APT_REG_BUFFER_TC_PRD               = 0x00000001U,
    APT_REG_BUFFER_TC_REFA              = 0x00000002U,
    APT_REG_BUFFER_TC_REFB              = 0x00000004U,
    APT_REG_BUFFER_TC_REFC              = 0x00000008U,
    APT_REG_BUFFER_TC_REFD              = 0x00000010U,
    APT_REG_BUFFER_PG_ACT_A             = 0x00000100U,
    APT_REG_BUFFER_PG_ACT_B             = 0x00000200U,
    APT_REG_BUFFER_PG_OUT_FRC           = 0x00000400U,
    APT_REG_BUFFER_DG_RED               = 0x00001000U,
    APT_REG_BUFFER_DG_FED               = 0x00002000U,
    APT_REG_BUFFER_DG_CFG               = 0x00004000U,
    APT_REG_BUFFER_TC_MWD_REFA          = 0x00010000U,
    APT_REG_BUFFER_TC_MWD_REFB          = 0x00020000U,
    APT_REG_BUFFER_TC_MWD_ACT           = 0x00040000U,
    APT_REG_BUFFER_PB_CFG_A             = 0x00100000U,
    APT_REG_BUFFER_PB_CFG_B             = 0x00200000U,
    APT_REG_BUFFER_MAX_VALUE            = 0x003FFFFFU,
} APT_RegBuffer;

/**
  * @brief Software force events.
  */
typedef enum {
    APT_FORCE_EVENT_COUNTER_SYNC        = 0x00000001U,
    APT_FORCE_EVENT_SYNCOUT             = 0x00000010U,
    APT_FORCE_EVENT_SYNC_PERIPH         = 0x00000100U,
    APT_FORCE_EVENT_GLOBAL_LOAD         = 0x00001000U,
    APT_FORCE_EVENT_VALLEY_CAP_RST      = 0x00010000U,
    APT_FORCE_EVENT_ADC_START_A         = 0x00100000U,
    APT_FORCE_EVENT_ADC_START_B         = 0x00200000U,
    APT_FORCE_EVENT_TIMER_INTERRUPT     = 0x01000000U,
    APT_FORCE_EVENT_PWM_ACTION_BUF_LOAD = 0x10000000U,
    APT_FORCE_EVENT_MAX_VALUE           = 0x1FFFFFFFU,
} APT_ForceEvtType;

/**
  * @brief Events that support asynchronous software buffer load.
  */
typedef enum {
    APT_ASYNC_EVENT_TC_BUF_INDEPENDENT_LOAD   =   0x00000001U,
    APT_ASYNC_EVENT_PG_BUF_INDEPENDENT_LOAD   =   0x00000002U,
    APT_ASYNC_EVENT_DG_BUF_INDEPENDENT_LOAD   =   0x00000004U,
    APT_ASYNC_EVENT_PB_BUF_INDEPENDENT_LOAD   =   0x00000008U,
    APT_ASYNC_EVENT_GLOBAL_LOAD               =   0x00000010U,
    APT_ASYNC_EVENT_COUNT_SYNC                =   0x00000020U,
    APT_ASYNC_EVENT_MAX_VALUE                 =   0x0000003FU,
} APT_AsyncBufferLoadEvent;

/**
  * @brief PWM output status.
  */
typedef enum {
    APT_PWM_LOW_LEVEL          = 0x00000000U,
    APT_PWM_HIGH_LEVEL         = 0x00000001U,
    APT_PWM_HIGH_RESISTANCE    = 0x00000002U,
} APT_PwmStatus;

/**
  * @brief Software force events.
  * @details Reference point selection.
  *          + APT_REFERENCE_DOTA -- Select referece dot A as action trigger point.
  *          + APT_REFERENCE_DOTB -- Select referece dot B as action trigger point.
  *          + APT_REFERENCE_DOTC -- Select referece dot C as action trigger point.
  *          + APT_REFERENCE_DOTD -- Select referece dot D as action trigger point.
  */
typedef enum {
    APT_REFERENCE_DOTA = 0x00000000U,
    APT_REFERENCE_DOTB = 0x00000001U,
    APT_REFERENCE_DOTC = 0x00000002U,
    APT_REFERENCE_DOTD = 0x00000003U,
} APT_RefDotSelect;

/**
  * @brief Protection pin level status.
  */
typedef enum {
    APT_TRIP_LOW_LEVEL          = 0x00000000U,
    APT_TRIP_HIGH_LEVEL         = 0x00000001U,
} APT_TripStatus;

/**
  * @brief Reference signal of MINDB module.
  */
typedef enum {
    APT_MDB_REF_SIGNAL_PWM_LR        = 0x00000000U,
    APT_MDB_REF_SIGNAL_MDB_XBAR1     = 0x00000001U,
    APT_MDB_REF_SIGNAL_MDB_XBAR2     = 0x00000002U,
    APT_MDB_REF_SIGNAL_MDB_XBAR3     = 0x00000003U,
    APT_MDB_REF_SIGNAL_MDB_XBAR4     = 0x00000004U,
    APT_MDB_REF_SIGNAL_MDB_XBAR5     = 0x00000005U,
    APT_MDB_REF_SIGNAL_MDB_XBAR6     = 0x00000006U,
    APT_MDB_REF_SIGNAL_MDB_XBAR7     = 0x00000007U,
    APT_MDB_REF_SIGNAL_MDB_XBAR8     = 0x00000008U,
    APT_MDB_REF_SIGNAL_MDB_XBAR9     = 0x00000009U,
    APT_MDB_REF_SIGNAL_MDB_XBAR10    = 0x0000000AU,
    APT_MDB_REF_SIGNAL_MDB_XBAR11    = 0x0000000BU,
    APT_MDB_REF_SIGNAL_MDB_XBAR12    = 0x0000000CU,
    APT_MDB_REF_SIGNAL_MDB_XBAR13    = 0x0000000DU,
    APT_MDB_REF_SIGNAL_MDB_XBAR14    = 0x0000000EU,
    APT_MDB_REF_SIGNAL_MDB_XBAR15    = 0x0000000FU,
} APT_MDBRefSig;

/**
  * @brief MDBXBAR event definition.
  * */
typedef enum {
    XBAR_MDB_EVENT_MDBXBAR1   =  0x00000000U,
    XBAR_MDB_EVENT_MDBXBAR2,
    XBAR_MDB_EVENT_MDBXBAR3,
    XBAR_MDB_EVENT_MDBXBAR4,
    XBAR_MDB_EVENT_MDBXBAR5,
    XBAR_MDB_EVENT_MDBXBAR6,
    XBAR_MDB_EVENT_MDBXBAR7,
    XBAR_MDB_EVENT_MDBXBAR8,
    XBAR_MDB_EVENT_MDBXBAR9,
    XBAR_MDB_EVENT_MDBXBAR10,
    XBAR_MDB_EVENT_MDBXBAR11,
    XBAR_MDB_EVENT_MDBXBAR12,
    XBAR_MDB_EVENT_MDBXBAR13,
    XBAR_MDB_EVENT_MDBXBAR14,
    XBAR_MDB_EVENT_MDBXBAR15,
    XBAR_MDB_EVENT_MAX_NUM,
} XBAR_MDBXbarEvent;

/**
  * @brief Reference signal polarity of MINDB module.
  */
typedef enum {
    APT_MDB_REF_SIGNAL_POLARITY_NOT_INVERT = 0x00000000U,
    APT_MDB_REF_SIGNAL_POLARITY_INVERT     = 0x00000001U,
} APT_MDBRefSigPol;

/**
  * @brief Block signal definition of the channel A/B.
  */
typedef enum {
    APT_MDB_BLOCK_SIGNAL_A = 0x00000000U,
    APT_MDB_BLOCK_SIGNAL_B = 0x00000001U,
} APT_MDBBlkSig;

/**
  * @brief Block signal delay mode of the channel A/B.
  */
typedef enum {
    APT_MDB_BLOCK_SIGNAL_DELAY_MODE_FALLING_EDGE    = 0x00000000U,
    APT_MDB_BLOCK_SIGNAL_DELAY_MODE_RISING_EDGE     = 0x00000001U,
    APT_MDB_BLOCK_SIGNAL_DELAY_MODE_DOUBLE_EDGE     = 0x00000002U,
    APT_MDB_BLOCK_SIGNAL_DELAY_MODE_FILTER          = 0x00000003U,
} APT_MDBBlkSigDelayMode;

/**
  * @brief  MinDB output signal polarity.
  */
typedef enum {
    APT_MDB_OUTPUT_POLARITY_NOT_BLOCK_AND_PWM    = 0x00000000U,
    APT_MDB_OUTPUT_POLARITY_BLOCK_OR_PWM         = 0x00000001U,
    APT_MDB_OUTPUT_POLARITY_BLOCK                = 0x00000002U,
} APT_MDBOutSigPol;

/**
  * @brief OLUTXBAR event definition.
  * */
typedef enum {
    XBAR_OLUT_EVENT_OLUTXBAR1     = 0x00000000U,
    XBAR_OLUT_EVENT_OLUTXBAR2,
    XBAR_OLUT_EVENT_OLUTXBAR3,
    XBAR_OLUT_EVENT_OLUTXBAR4,
    XBAR_OLUT_EVENT_OLUTXBAR5,
    XBAR_OLUT_EVENT_OLUTXBAR6,
    XBAR_OLUT_EVENT_OLUTXBAR7,
    XBAR_OLUT_EVENT_OLUTXBAR8,
    XBAR_OLUT_EVENT_OLUTXBAR9,
    XBAR_OLUT_EVENT_OLUTXBAR10,
    XBAR_OLUT_EVENT_OLUTXBAR11,
    XBAR_OLUT_EVENT_OLUTXBAR12,
    XBAR_OLUT_EVENT_OLUTXBAR13,
    XBAR_OLUT_EVENT_OLUTXBAR14,
    XBAR_OLUT_EVENT_OLUTXBAR15,
    XBAR_OLUT_EVENT_OLUTXBAR16,
    XBAR_OLUT_EVENT_MAX_NUM,
} XBAR_OLUTXbarEvent;

/**
  * @brief  Link source definition.
  */
typedef enum {
    APT_LINK_SOURCE_APT0      = 0x00000000U,
    APT_LINK_SOURCE_APT1      = 0x00000001U,
    APT_LINK_SOURCE_APT2      = 0x00000002U,
    APT_LINK_SOURCE_APT3      = 0x00000003U,
    APT_LINK_SOURCE_APT4      = 0x00000004U,
    APT_LINK_SOURCE_APT5      = 0x00000005U,
    APT_LINK_SOURCE_APT6      = 0x00000006U,
    APT_LINK_SOURCE_APT7      = 0x00000007U,
    APT_LINK_SOURCE_APT8      = 0x00000008U,
    APT_LINK_SOURCE_APT9      = 0x00000009U,
    APT_LINK_SOURCE_APT10     = 0x0000000AU,
    APT_LINK_SOURCE_APT11     = 0x0000000BU,
    APT_LINK_SOURCE_APT12     = 0x0000000CU,
    APT_LINK_SOURCE_APT13     = 0x0000000DU,
    APT_LINK_SOURCE_APT14     = 0x0000000EU,
    APT_LINK_SOURCE_APT15     = 0x0000000FU,
    APT_LINK_DISABLE          = 0x0000001FU,
} APT_LinkSrc;

/**
  * @brief  Link configuration register definition.
  */
typedef enum {
    APT_LINK_CFG0_REG         = 0b0000000U,
    APT_LINK_CFG1_REG         = 0b0100000U,
    APT_LINK_CFG2_REG         = 0b1000000U,
    APT_LINK_CFG3_REG         = 0b1100000U,
} APT_LinkCfgReg;

/**
  * @brief  Link function offset of each LINK_CFGx.
  */
typedef enum {
    APT_LINK_FUNC_OFFSET_BIT0  = 0b0000000U,
    APT_LINK_FUNC_OFFSET_BIT5  = 0b0000101U,
    APT_LINK_FUNC_OFFSET_BIT10 = 0b0001010U,
    APT_LINK_FUNC_OFFSET_BIT16 = 0b0010000U,
    APT_LINK_FUNC_OFFSET_BIT21 = 0b0010101U,
    APT_LINK_FUNC_OFFSET_BIT26 = 0b0011010U,
} APT_LinkFuncOffset;

/**
  * @brief  Link event definition.
  */
typedef enum {
    APT_LINK_EVENT_TC_PERIOD        = APT_LINK_CFG0_REG | APT_LINK_FUNC_OFFSET_BIT0,
    APT_LINK_EVENT_TC_REFA          = APT_LINK_CFG0_REG | APT_LINK_FUNC_OFFSET_BIT5,
    APT_LINK_EVENT_TC_REFB          = APT_LINK_CFG0_REG | APT_LINK_FUNC_OFFSET_BIT10,
    APT_LINK_EVENT_TC_REFC          = APT_LINK_CFG0_REG | APT_LINK_FUNC_OFFSET_BIT16,
    APT_LINK_EVENT_TC_REFD          = APT_LINK_CFG0_REG | APT_LINK_FUNC_OFFSET_BIT21,
    APT_LINK_EVENT_SYNC_FRC         = APT_LINK_CFG0_REG | APT_LINK_FUNC_OFFSET_BIT26,
    APT_LINK_EVENT_DG_RED           = APT_LINK_CFG1_REG | APT_LINK_FUNC_OFFSET_BIT0,
    APT_LINK_EVENT_DG_FED           = APT_LINK_CFG1_REG | APT_LINK_FUNC_OFFSET_BIT5,
    APT_LINK_EVENT_DG_CFG           = APT_LINK_CFG1_REG | APT_LINK_FUNC_OFFSET_BIT10,
    APT_LINK_EVENT_PC_CFG           = APT_LINK_CFG1_REG | APT_LINK_FUNC_OFFSET_BIT16,
    APT_LINK_EVENT_PB_CFG_A         = APT_LINK_CFG1_REG | APT_LINK_FUNC_OFFSET_BIT21,
    APT_LINK_EVENT_PB_CFG_B         = APT_LINK_CFG1_REG | APT_LINK_FUNC_OFFSET_BIT26,
    APT_LINK_EVENT_PG_ACT_A         = APT_LINK_CFG2_REG | APT_LINK_FUNC_OFFSET_BIT0,
    APT_LINK_EVENT_PG_ACT_B         = APT_LINK_CFG2_REG | APT_LINK_FUNC_OFFSET_BIT5,
    APT_LINK_EVENT_PG_ACT_FRC       = APT_LINK_CFG2_REG | APT_LINK_FUNC_OFFSET_BIT10,
    APT_LINK_EVENT_PG_OUT_FRC       = APT_LINK_CFG2_REG | APT_LINK_FUNC_OFFSET_BIT16,
    APT_LINK_EVENT_OC_FRC_EVT       = APT_LINK_CFG2_REG | APT_LINK_FUNC_OFFSET_BIT21,
    APT_LINK_EVENT_OC_SW_CLR        = APT_LINK_CFG2_REG | APT_LINK_FUNC_OFFSET_BIT26,
    APT_LINK_EVENT_GLB_LOAD         = APT_LINK_CFG3_REG | APT_LINK_FUNC_OFFSET_BIT0,
    APT_LINK_EVENT_ASYNC_SWITCH     = APT_LINK_CFG3_REG | APT_LINK_FUNC_OFFSET_BIT5,
} APT_LinkEvent;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    sub_version           : 4   ; /* [3..0] */
        unsigned int    main_version          : 4   ; /* [7..4] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile VER_INFO_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    write_protect_key     : 8   ; /* [7..0] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile PROT_KEY_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_cnt_mode           : 2   ; /* [1..0] */
        unsigned int    reserved0             : 14  ; /* [15..2] */
        unsigned int    rg_div_fac            : 12  ; /* [27..16] */
        unsigned int    rg_emu_stop           : 2   ; /* [29..28] */
        unsigned int    reserved1             : 2   ; /* [31..30] */
    } BIT;
} volatile TC_MODE_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_cnt_phs            : 16  ; /* [15..0] */
        unsigned int    rg_cnt_phs_hr         : 8   ; /* [23..16] */
        unsigned int    reserved0             : 7   ; /* [30..24] */
        unsigned int    rg_cnt_dir            : 1   ; /* [31] */
    } BIT;
} volatile TC_PHS_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_cnt_ovrid          : 16  ; /* [15..0] */
        unsigned int    reserved0             : 15  ; /* [30..16] */
        unsigned int    rg_cnt_ovrid_en       : 1   ; /* [31] */
    } BIT;
} volatile TC_OVRID_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_cnt_prd            : 16  ; /* [15..0] */
        unsigned int    rg_cnt_prd_hr         : 8   ; /* [23..16] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} volatile TC_PRD_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_cnt_refa           : 16  ; /* [15..0] */
        unsigned int    rg_cnt_refa_hr        : 8   ; /* [23..16] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} volatile TC_REFA_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_cnt_refb           : 16  ; /* [15..0] */
        unsigned int    rg_cnt_refb_hr        : 8   ; /* [23..16] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} volatile TC_REFB_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_cnt_refc           : 16  ; /* [15..0] */
        unsigned int    rg_cnt_refc_hr        : 8   ; /* [23..16] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} volatile TC_REFC_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_cnt_refd           : 16  ; /* [15..0] */
        unsigned int    rg_cnt_refd_hr        : 8   ; /* [23..16] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} volatile TC_REFD_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_prd_buf_en         : 1   ; /* [0] */
        unsigned int    rg_prd_gld_en         : 1   ; /* [1] */
        unsigned int    reserved0             : 2   ; /* [3..2] */
        unsigned int    rg_refa_buf_en        : 1   ; /* [4] */
        unsigned int    rg_refa_gld_en        : 1   ; /* [5] */
        unsigned int    rg_refb_buf_en        : 1   ; /* [6] */
        unsigned int    rg_refb_gld_en        : 1   ; /* [7] */
        unsigned int    rg_refc_buf_en        : 1   ; /* [8] */
        unsigned int    rg_refc_gld_en        : 1   ; /* [9] */
        unsigned int    rg_refd_buf_en        : 1   ; /* [10] */
        unsigned int    rg_refd_gld_en        : 1   ; /* [11] */
        unsigned int    reserved1             : 20  ; /* [31..12] */
    } BIT;
} volatile TC_BUF_EN_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_prd_ld_zroen       : 1   ; /* [0] */
        unsigned int    reserved0             : 1   ; /* [1] */
        unsigned int    rg_prd_ld_a1en        : 1   ; /* [2] */
        unsigned int    rg_prd_ld_b1en        : 1   ; /* [3] */
        unsigned int    rg_prd_ld_synen       : 1   ; /* [4] */
        unsigned int    reserved1             : 27  ; /* [31..5] */
    } BIT;
} volatile TC_PRD_LOAD_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_refa_ld_zroen      : 1   ; /* [0] */
        unsigned int    rg_refa_ld_prden      : 1   ; /* [1] */
        unsigned int    rg_refa_ld_a1en       : 1   ; /* [2] */
        unsigned int    rg_refa_ld_b1en       : 1   ; /* [3] */
        unsigned int    rg_refa_ld_synen      : 1   ; /* [4] */
        unsigned int    reserved0             : 3   ; /* [7..5] */
        unsigned int    rg_refb_ld_zroen      : 1   ; /* [8] */
        unsigned int    rg_refb_ld_prden      : 1   ; /* [9] */
        unsigned int    rg_refb_ld_a1en       : 1   ; /* [10] */
        unsigned int    rg_refb_ld_b1en       : 1   ; /* [11] */
        unsigned int    rg_refb_ld_synen      : 1   ; /* [12] */
        unsigned int    reserved1             : 3   ; /* [15..13] */
        unsigned int    rg_refc_ld_zroen      : 1   ; /* [16] */
        unsigned int    rg_refc_ld_prden      : 1   ; /* [17] */
        unsigned int    rg_refc_ld_a1en       : 1   ; /* [18] */
        unsigned int    rg_refc_ld_b1en       : 1   ; /* [19] */
        unsigned int    rg_refc_ld_synen      : 1   ; /* [20] */
        unsigned int    reserved2             : 3   ; /* [23..21] */
        unsigned int    rg_refd_ld_zroen      : 1   ; /* [24] */
        unsigned int    rg_refd_ld_prden      : 1   ; /* [25] */
        unsigned int    rg_refd_ld_a1en       : 1   ; /* [26] */
        unsigned int    rg_refd_ld_b1en       : 1   ; /* [27] */
        unsigned int    rg_refd_ld_synen      : 1   ; /* [28] */
        unsigned int    reserved3             : 3   ; /* [31..29] */
    } BIT;
} volatile TC_REF_LOAD_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_cnt_val            : 16  ; /* [15..0] */
        unsigned int    ro_div_cnt            : 12  ; /* [27..16] */
        unsigned int    reserved0             : 3   ; /* [30..28] */
        unsigned int    ro_cnt_dir            : 1   ; /* [31] */
    } BIT;
} volatile TC_STS_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pga_act_zro        : 2   ; /* [1..0] */
        unsigned int    rg_pga_act_prd        : 2   ; /* [3..2] */
        unsigned int    rg_pga_act_refa_inc   : 2   ; /* [5..4] */
        unsigned int    rg_pga_act_refa_dec   : 2   ; /* [7..6] */
        unsigned int    rg_pga_act_refb_inc   : 2   ; /* [9..8] */
        unsigned int    rg_pga_act_refb_dec   : 2   ; /* [11..10] */
        unsigned int    rg_pga_act_refc_inc   : 2   ; /* [13..12] */
        unsigned int    rg_pga_act_refc_dec   : 2   ; /* [15..14] */
        unsigned int    rg_pga_act_refd_inc   : 2   ; /* [17..16] */
        unsigned int    rg_pga_act_refd_dec   : 2   ; /* [19..18] */
        unsigned int    rg_pga_act_evtc1_inc  : 2   ; /* [21..20] */
        unsigned int    rg_pga_act_evtc1_dec  : 2   ; /* [23..22] */
        unsigned int    rg_pga_act_evtc2_inc  : 2   ; /* [25..24] */
        unsigned int    rg_pga_act_evtc2_dec  : 2   ; /* [27..26] */
        unsigned int    reserved0             : 4   ; /* [31..28] */
    } BIT;
} volatile PG_ACT_A_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pgb_act_zro        : 2   ; /* [1..0] */
        unsigned int    rg_pgb_act_prd        : 2   ; /* [3..2] */
        unsigned int    rg_pgb_act_refa_inc   : 2   ; /* [5..4] */
        unsigned int    rg_pgb_act_refa_dec   : 2   ; /* [7..6] */
        unsigned int    rg_pgb_act_refb_inc   : 2   ; /* [9..8] */
        unsigned int    rg_pgb_act_refb_dec   : 2   ; /* [11..10] */
        unsigned int    rg_pgb_act_refc_inc   : 2   ; /* [13..12] */
        unsigned int    rg_pgb_act_refc_dec   : 2   ; /* [15..14] */
        unsigned int    rg_pgb_act_refd_inc   : 2   ; /* [17..16] */
        unsigned int    rg_pgb_act_refd_dec   : 2   ; /* [19..18] */
        unsigned int    rg_pgb_act_evtc1_inc  : 2   ; /* [21..20] */
        unsigned int    rg_pgb_act_evtc1_dec  : 2   ; /* [23..22] */
        unsigned int    rg_pgb_act_evtc2_inc  : 2   ; /* [25..24] */
        unsigned int    rg_pgb_act_evtc2_dec  : 2   ; /* [27..26] */
        unsigned int    reserved0             : 4   ; /* [31..28] */
    } BIT;
} volatile PG_ACT_B_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pga_act_evt_frc    : 2   ; /* [1..0] */
        unsigned int    rg_pga_evt_frc        : 1   ; /* [2] */
        unsigned int    reserved0             : 1   ; /* [3] */
        unsigned int    rg_pgb_act_evt_frc    : 2   ; /* [5..4] */
        unsigned int    rg_pgb_evt_frc        : 1   ; /* [6] */
        unsigned int    reserved1             : 25  ; /* [31..7] */
    } BIT;
} volatile PG_ACT_FRC_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pga_frc_act        : 2   ; /* [1..0] */
        unsigned int    rg_pga_frc_en         : 1   ; /* [2] */
        unsigned int    reserved0             : 1   ; /* [3] */
        unsigned int    rg_pgb_frc_act        : 2   ; /* [5..4] */
        unsigned int    rg_pgb_frc_en         : 1   ; /* [6] */
        unsigned int    reserved1             : 25  ; /* [31..7] */
    } BIT;
} volatile PG_OUT_FRC_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_acta_buf_en        : 1   ; /* [0] */
        unsigned int    rg_acta_gld_en        : 1   ; /* [1] */
        unsigned int    rg_actb_buf_en        : 1   ; /* [2] */
        unsigned int    rg_actb_gld_en        : 1   ; /* [3] */
        unsigned int    rg_frc_buf_en         : 1   ; /* [4] */
        unsigned int    rg_frc_gld_en         : 1   ; /* [5] */
        unsigned int    reserved0             : 26  ; /* [31..6] */
    } BIT;
} volatile PG_BUF_EN_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pga_actld_zroen    : 1   ; /* [0] */
        unsigned int    rg_pga_actld_prden    : 1   ; /* [1] */
        unsigned int    rg_pga_actld_a1en     : 1   ; /* [2] */
        unsigned int    rg_pga_actld_b1en     : 1   ; /* [3] */
        unsigned int    rg_pga_actld_synen    : 1   ; /* [4] */
        unsigned int    reserved0             : 3   ; /* [7..5] */
        unsigned int    rg_pgb_actld_zroen    : 1   ; /* [8] */
        unsigned int    rg_pgb_actld_prden    : 1   ; /* [9] */
        unsigned int    rg_pgb_actld_a1en     : 1   ; /* [10] */
        unsigned int    rg_pgb_actld_b1en     : 1   ; /* [11] */
        unsigned int    rg_pgb_actld_synen    : 1   ; /* [12] */
        unsigned int    reserved1             : 3   ; /* [15..13] */
        unsigned int    rg_pg_frcld_zroen     : 1   ; /* [16] */
        unsigned int    rg_pg_frcld_prden     : 1   ; /* [17] */
        unsigned int    reserved2             : 2   ; /* [19..18] */
        unsigned int    rg_pg_frcld_synen     : 1   ; /* [20] */
        unsigned int    reserved3             : 3   ; /* [23..21] */
        unsigned int    reserved4             : 8   ; /* [31..24] */
    } BIT;
} volatile PG_ACT_LD_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pga_evtc1_sel      : 4   ; /* [3..0] */
        unsigned int    rg_pga_evtc2_sel      : 4   ; /* [7..4] */
        unsigned int    rg_pgb_evtc1_sel      : 4   ; /* [11..8] */
        unsigned int    rg_pgb_evtc2_sel      : 4   ; /* [15..12] */
        unsigned int    reserved0             : 16  ; /* [31..16] */
    } BIT;
} volatile PG_EVTC_SEL_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_dg_red             : 16  ; /* [15..0] */
        unsigned int    rg_dg_red_hr          : 8   ; /* [23..16] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} volatile DG_RED_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_dg_fed             : 16  ; /* [15..0] */
        unsigned int    rg_dg_fed_hr          : 8   ; /* [23..16] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} volatile DG_FED_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_dg_red_isel        : 2   ; /* [1..0] */
        unsigned int    rg_dg_fed_isel        : 2   ; /* [3..2] */
        unsigned int    rg_dg_red_osel        : 2   ; /* [5..4] */
        unsigned int    rg_dg_fed_osel        : 2   ; /* [7..6] */
        unsigned int    rg_dga_osel           : 1   ; /* [8] */
        unsigned int    rg_dgb_osel           : 1   ; /* [9] */
        unsigned int    reserved0             : 22  ; /* [31..10] */
    } BIT;
} volatile DG_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_red_buf_en         : 1   ; /* [0] */
        unsigned int    rg_red_gld_en         : 1   ; /* [1] */
        unsigned int    rg_fed_buf_en         : 1   ; /* [2] */
        unsigned int    rg_fed_gld_en         : 1   ; /* [3] */
        unsigned int    rg_cfg_buf_en         : 1   ; /* [4] */
        unsigned int    rg_cfg_gld_en         : 1   ; /* [5] */
        unsigned int    reserved0             : 26  ; /* [31..6] */
    } BIT;
} volatile DG_BUF_EN_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_red_ld_zroen       : 1   ; /* [0] */
        unsigned int    rg_red_ld_prden       : 1   ; /* [1] */
        unsigned int    reserved0             : 6   ; /* [7..2] */
        unsigned int    rg_fed_ld_zroen       : 1   ; /* [8] */
        unsigned int    rg_fed_ld_prden       : 1   ; /* [9] */
        unsigned int    reserved1             : 6   ; /* [15..10] */
        unsigned int    rg_cfg_ld_zroen       : 1   ; /* [16] */
        unsigned int    rg_cfg_ld_prden       : 1   ; /* [17] */
        unsigned int    reserved2             : 14  ; /* [31..18] */
    } BIT;
} volatile DG_BUF_LOAD_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pc_en_b            : 1   ; /* [0] */
        unsigned int    rg_pc_en_a            : 1   ; /* [1] */
        unsigned int    reserved0             : 30  ; /* [31..2] */
    } BIT;
} volatile PC_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pc_freq            : 5   ; /* [4..0] */
        unsigned int    reserved0             : 27  ; /* [31..5] */
    } BIT;
} volatile PC_FREQ_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pc_shape           : 8   ; /* [7..0] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile PC_SHAP_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pc_init_width      : 4   ; /* [3..0] */
        unsigned int    reserved0             : 28  ; /* [31..4] */
    } BIT;
} volatile PC_INIT_WIDTH_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pb_n_a             : 8   ; /* [7..0] */
        unsigned int    rg_pb_m_a             : 8   ; /* [15..8] */
        unsigned int    rg_pb_en_a            : 1   ; /* [16] */
        unsigned int    rg_pb_op_mode_a       : 1   ; /* [17] */
        unsigned int    rg_pb_n_pol_a         : 1   ; /* [18] */
        unsigned int    reserved0             : 13  ; /* [31..19] */
    } BIT;
} volatile PB_CFG_A_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pb_n_b             : 8   ; /* [7..0] */
        unsigned int    rg_pb_m_b             : 8   ; /* [15..8] */
        unsigned int    rg_pb_en_b            : 1   ; /* [16] */
        unsigned int    rg_pb_op_mode_b       : 1   ; /* [17] */
        unsigned int    rg_pb_n_pol_b         : 1   ; /* [18] */
        unsigned int    reserved0             : 13  ; /* [31..19] */
    } BIT;
} volatile PB_CFG_B_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_pb_m_n_a           : 8   ; /* [7..0] */
        unsigned int    ro_pb_state_a         : 1   ; /* [8] */
        unsigned int    reserved0             : 7   ; /* [15..9] */
        unsigned int    ro_pb_m_n_b           : 8   ; /* [23..16] */
        unsigned int    ro_pb_state_b         : 1   ; /* [24] */
        unsigned int    reserved1             : 7   ; /* [31..25] */
    } BIT;
} volatile PB_STATUS_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pb_cfg_a_buf_en    : 1   ; /* [0] */
        unsigned int    rg_pb_cfg_a_gld_en    : 1   ; /* [1] */
        unsigned int    reserved0             : 6   ; /* [7..2] */
        unsigned int    rg_pb_cfg_b_buf_en    : 1   ; /* [8] */
        unsigned int    rg_pb_cfg_b_gld_en    : 1   ; /* [9] */
        unsigned int    reserved1             : 22  ; /* [31..10] */
    } BIT;
} volatile PB_BUF_EN_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pb_cfg_a_ld_zroen  : 1   ; /* [0] */
        unsigned int    rg_pb_cfg_a_ld_prden  : 1   ; /* [1] */
        unsigned int    reserved0             : 6   ; /* [7..2] */
        unsigned int    rg_pb_cfg_b_ld_zroen  : 1   ; /* [8] */
        unsigned int    rg_pb_cfg_b_ld_prden  : 1   ; /* [9] */
        unsigned int    reserved1             : 22  ; /* [31..10] */
    } BIT;
} volatile PB_BUF_LOAD_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oc_en_cbc_trip1    : 1   ; /* [0] */
        unsigned int    rg_oc_en_cbc_trip2    : 1   ; /* [1] */
        unsigned int    rg_oc_en_cbc_trip3    : 1   ; /* [2] */
        unsigned int    reserved0             : 5   ; /* [7..3] */
        unsigned int    rg_oc_en_cbc_sysevt1  : 1   ; /* [8] */
        unsigned int    rg_oc_en_cbc_sysevt2  : 1   ; /* [9] */
        unsigned int    rg_oc_en_cbc_sysevt3  : 1   ; /* [10] */
        unsigned int    rg_oc_en_cbc_sysevt4  : 1   ; /* [11] */
        unsigned int    reserved1             : 4   ; /* [15..12] */
        unsigned int    rg_oc_en_cbc_emevta1  : 1   ; /* [16] */
        unsigned int    rg_oc_en_cbc_emevta2  : 1   ; /* [17] */
        unsigned int    rg_oc_en_cbc_emevtb1  : 1   ; /* [18] */
        unsigned int    rg_oc_en_cbc_emevtb2  : 1   ; /* [19] */
        unsigned int    reserved2             : 12  ; /* [31..20] */
    } BIT;
} volatile OC_CBC_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oc_en_osht_trip1   : 1   ; /* [0] */
        unsigned int    rg_oc_en_osht_trip2   : 1   ; /* [1] */
        unsigned int    rg_oc_en_osht_trip3   : 1   ; /* [2] */
        unsigned int    reserved0             : 5   ; /* [7..3] */
        unsigned int    rg_oc_en_osht_sysevt1 : 1   ; /* [8] */
        unsigned int    rg_oc_en_osht_sysevt2 : 1   ; /* [9] */
        unsigned int    rg_oc_en_osht_sysevt3 : 1   ; /* [10] */
        unsigned int    rg_oc_en_osht_sysevt4 : 1   ; /* [11] */
        unsigned int    reserved1             : 4   ; /* [15..12] */
        unsigned int    rg_oc_en_osht_emevta1 : 1   ; /* [16] */
        unsigned int    rg_oc_en_osht_emevta2 : 1   ; /* [17] */
        unsigned int    rg_oc_en_osht_emevtb1 : 1   ; /* [18] */
        unsigned int    rg_oc_en_osht_emevtb2 : 1   ; /* [19] */
        unsigned int    reserved2             : 12  ; /* [31..20] */
    } BIT;
} volatile OC_OSHT_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oc_en_free_trip1   : 1   ; /* [0] */
        unsigned int    rg_oc_en_free_trip2   : 1   ; /* [1] */
        unsigned int    rg_oc_en_free_trip3   : 1   ; /* [2] */
        unsigned int    reserved0             : 5   ; /* [7..3] */
        unsigned int    rg_oc_en_free_sysevt1 : 1   ; /* [8] */
        unsigned int    rg_oc_en_free_sysevt2 : 1   ; /* [9] */
        unsigned int    rg_oc_en_free_sysevt3 : 1   ; /* [10] */
        unsigned int    rg_oc_en_free_sysevt4 : 1   ; /* [11] */
        unsigned int    reserved1             : 4   ; /* [15..12] */
        unsigned int    rg_oc_en_free_emevta1 : 1   ; /* [16] */
        unsigned int    rg_oc_en_free_emevta2 : 1   ; /* [17] */
        unsigned int    rg_oc_en_free_emevtb1 : 1   ; /* [18] */
        unsigned int    rg_oc_en_free_emevtb2 : 1   ; /* [19] */
        unsigned int    reserved2             : 12  ; /* [31..20] */
    } BIT;
} volatile OC_FREE_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oca_evtio_inc      : 3   ; /* [2..0] */
        unsigned int    rg_oca_evta1_inc      : 3   ; /* [5..3] */
        unsigned int    rg_oca_evta2_inc      : 3   ; /* [8..6] */
        unsigned int    rg_oca_evtb1_inc      : 3   ; /* [11..9] */
        unsigned int    rg_oca_evtb2_inc      : 3   ; /* [14..12] */
        unsigned int    reserved0             : 1   ; /* [15] */
        unsigned int    rg_oca_evtio_dec      : 3   ; /* [18..16] */
        unsigned int    rg_oca_evta1_dec      : 3   ; /* [21..19] */
        unsigned int    rg_oca_evta2_dec      : 3   ; /* [24..22] */
        unsigned int    rg_oca_evtb1_dec      : 3   ; /* [27..25] */
        unsigned int    rg_oca_evtb2_dec      : 3   ; /* [30..28] */
        unsigned int    reserved1             : 1   ; /* [31] */
    } BIT;
} volatile OC_ACT_A_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_ocb_evtio_inc      : 3   ; /* [2..0] */
        unsigned int    rg_ocb_evta1_inc      : 3   ; /* [5..3] */
        unsigned int    rg_ocb_evta2_inc      : 3   ; /* [8..6] */
        unsigned int    rg_ocb_evtb1_inc      : 3   ; /* [11..9] */
        unsigned int    rg_ocb_evtb2_inc      : 3   ; /* [14..12] */
        unsigned int    reserved0             : 1   ; /* [15] */
        unsigned int    rg_ocb_evtio_dec      : 3   ; /* [18..16] */
        unsigned int    rg_ocb_evta1_dec      : 3   ; /* [21..19] */
        unsigned int    rg_ocb_evta2_dec      : 3   ; /* [24..22] */
        unsigned int    rg_ocb_evtb1_dec      : 3   ; /* [27..25] */
        unsigned int    rg_ocb_evtb2_dec      : 3   ; /* [30..28] */
        unsigned int    reserved1             : 1   ; /* [31] */
    } BIT;
} volatile OC_ACT_B_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_oc_flag_cbc        : 1   ; /* [0] */
        unsigned int    ro_oc_flag_osht       : 1   ; /* [1] */
        unsigned int    ro_oc_flag_free       : 1   ; /* [2] */
        unsigned int    reserved0             : 1   ; /* [3] */
        unsigned int    ro_oc_flag_evts1      : 1   ; /* [4] */
        unsigned int    ro_oc_flag_evts2      : 1   ; /* [5] */
        unsigned int    ro_oc_flag_evts3      : 1   ; /* [6] */
        unsigned int    ro_oc_flag_evts4      : 1   ; /* [7] */
        unsigned int    ro_oc_flag_evta1      : 1   ; /* [8] */
        unsigned int    ro_oc_flag_evta2      : 1   ; /* [9] */
        unsigned int    ro_oc_flag_evtb1      : 1   ; /* [10] */
        unsigned int    ro_oc_flag_evtb2      : 1   ; /* [11] */
        unsigned int    ro_oca_flag_max_cbc   : 1   ; /* [12] */
        unsigned int    ro_ocb_flag_max_cbc   : 1   ; /* [13] */
        unsigned int    ro_oca_flag_max_free  : 1   ; /* [14] */
        unsigned int    ro_ocb_flag_max_free  : 1   ; /* [15] */
        unsigned int    rg_oc_clr_cbc         : 1   ; /* [16] */
        unsigned int    rg_oc_clr_osht        : 1   ; /* [17] */
        unsigned int    rg_oc_clr_free        : 1   ; /* [18] */
        unsigned int    reserved1             : 1   ; /* [19] */
        unsigned int    rg_oc_clr_evts1       : 1   ; /* [20] */
        unsigned int    rg_oc_clr_evts2       : 1   ; /* [21] */
        unsigned int    rg_oc_clr_evts3       : 1   ; /* [22] */
        unsigned int    rg_oc_clr_evts4       : 1   ; /* [23] */
        unsigned int    rg_oc_clr_evta1       : 1   ; /* [24] */
        unsigned int    rg_oc_clr_evta2       : 1   ; /* [25] */
        unsigned int    rg_oc_clr_evtb1       : 1   ; /* [26] */
        unsigned int    rg_oc_clr_evtb2       : 1   ; /* [27] */
        unsigned int    rg_oca_clr_max_cbc    : 1   ; /* [28] */
        unsigned int    rg_ocb_clr_max_cbc    : 1   ; /* [29] */
        unsigned int    rg_oca_clr_max_free   : 1   ; /* [30] */
        unsigned int    rg_ocb_clr_max_free   : 1   ; /* [31] */
    } BIT;
} volatile OC_EVT_FLAG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oc_clr_zroen_cbc   : 1   ; /* [0] */
        unsigned int    reserved0             : 15  ; /* [15..1] */
        unsigned int    rg_oc_clr_prden_cbc   : 1   ; /* [16] */
        unsigned int    reserved1             : 15  ; /* [31..17] */
    } BIT;
} volatile OC_PRD_CLR_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oc_sw_clr_cbc      : 1   ; /* [0] */
        unsigned int    rg_oc_sw_clr_osht     : 1   ; /* [1] */
        unsigned int    rg_oc_sw_clr_free     : 1   ; /* [2] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile OC_SW_CLR_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oc_frc_cbc         : 1   ; /* [0] */
        unsigned int    rg_oc_frc_osht        : 1   ; /* [1] */
        unsigned int    rg_oc_frc_free        : 1   ; /* [2] */
        unsigned int    reserved0             : 5   ; /* [7..3] */
        unsigned int    rg_oc_frc_evta1       : 1   ; /* [8] */
        unsigned int    rg_oc_frc_evta2       : 1   ; /* [9] */
        unsigned int    rg_oc_frc_evtb1       : 1   ; /* [10] */
        unsigned int    rg_oc_frc_evtb2       : 1   ; /* [11] */
        unsigned int    reserved1             : 20  ; /* [31..12] */
    } BIT;
} volatile OC_FRC_EVT_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_oca_cbc            : 1   ; /* [0] */
        unsigned int    ro_ocb_cbc            : 1   ; /* [1] */
        unsigned int    ro_oca_max_cbc        : 1   ; /* [2] */
        unsigned int    ro_ocb_max_cbc        : 1   ; /* [3] */
        unsigned int    reserved0             : 4   ; /* [7..4] */
        unsigned int    ro_oca_osht           : 1   ; /* [8] */
        unsigned int    ro_ocb_osht           : 1   ; /* [9] */
        unsigned int    reserved1             : 6   ; /* [15..10] */
        unsigned int    ro_oca_free           : 1   ; /* [16] */
        unsigned int    ro_ocb_free           : 1   ; /* [17] */
        unsigned int    ro_oca_max_free       : 1   ; /* [18] */
        unsigned int    ro_ocb_max_free       : 1   ; /* [19] */
        unsigned int    ro_oc_sysevt1         : 1   ; /* [20] */
        unsigned int    ro_oc_sysevt2         : 1   ; /* [21] */
        unsigned int    ro_oc_sysevt3         : 1   ; /* [22] */
        unsigned int    ro_oc_sysevt4         : 1   ; /* [23] */
        unsigned int    ro_oc_emevta1         : 1   ; /* [24] */
        unsigned int    ro_oc_emevta2         : 1   ; /* [25] */
        unsigned int    ro_oc_emevtb1         : 1   ; /* [26] */
        unsigned int    ro_oc_emevtb2         : 1   ; /* [27] */
        unsigned int    reserved2             : 4   ; /* [31..28] */
    } BIT;
} volatile OC_STATUS_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_cbc_oca_prot_mode  : 1   ; /* [0] */
        unsigned int    rg_cbc_oca_rcvy_mode  : 1   ; /* [1] */
        unsigned int    reserved0             : 2   ; /* [3..2] */
        unsigned int    rg_osht_oca_prot_mode : 2   ; /* [5..4] */
        unsigned int    rg_osht_oca_rcvy_mode : 2   ; /* [7..6] */
        unsigned int    rg_free_oca_prot_mode : 1   ; /* [8] */
        unsigned int    rg_free_oca_rcvy_mode : 1   ; /* [9] */
        unsigned int    reserved1             : 6   ; /* [15..10] */
        unsigned int    rg_cbc_ocb_prot_mode  : 1   ; /* [16] */
        unsigned int    rg_cbc_ocb_rcvy_mode  : 1   ; /* [17] */
        unsigned int    reserved2             : 2   ; /* [19..18] */
        unsigned int    rg_osht_ocb_prot_mode : 2   ; /* [21..20] */
        unsigned int    rg_osht_ocb_rcvy_mode : 2   ; /* [23..22] */
        unsigned int    rg_free_ocb_prot_mode : 1   ; /* [24] */
        unsigned int    rg_free_ocb_rcvy_mode : 1   ; /* [25] */
        unsigned int    reserved3             : 6   ; /* [31..26] */
    } BIT;
} volatile OC_EVT_DLY_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oca_prot_dly       : 16  ; /* [15..0] */
        unsigned int    rg_oca_rcvy_dly       : 16  ; /* [31..16] */
    } BIT;
} volatile OC_DLY_A_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_ocb_prot_dly       : 16  ; /* [15..0] */
        unsigned int    rg_ocb_rcvy_dly       : 16  ; /* [31..16] */
    } BIT;
} volatile OC_DLY_B_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oca_max_prot         : 16  ; /* [15..0] */
        unsigned int    reserved0               : 8   ; /* [23..16] */
        unsigned int    rg_oca_max_prot_cbc_en  : 1   ; /* [24] */
        unsigned int    reserved1               : 1   ; /* [25] */
        unsigned int    rg_oca_max_prot_free_en : 1   ; /* [26] */
        unsigned int    reserved2               : 5   ; /* [31..27] */
    } BIT;
} volatile OC_PMAX_A_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oca_min_prot         : 16  ; /* [15..0] */
        unsigned int    reserved0               : 8   ; /* [23..16] */
        unsigned int    reserved1               : 2   ; /* [25..24] */
        unsigned int    rg_oca_min_prot_free_en : 1   ; /* [26] */
        unsigned int    reserved2               : 5   ; /* [31..27] */
    } BIT;
} volatile OC_PMIN_A_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_ocb_max_prot         : 16  ; /* [15..0] */
        unsigned int    reserved0               : 8   ; /* [23..16] */
        unsigned int    rg_ocb_max_prot_cbc_en  : 1   ; /* [24] */
        unsigned int    reserved1               : 1   ; /* [25] */
        unsigned int    rg_ocb_max_prot_free_en : 1   ; /* [26] */
        unsigned int    reserved2               : 5   ; /* [31..27] */
    } BIT;
} volatile OC_PMAX_B_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_ocb_min_prot         : 16  ; /* [15..0] */
        unsigned int    reserved0               : 8   ; /* [23..16] */
        unsigned int    reserved1               : 2   ; /* [25..24] */
        unsigned int    rg_ocb_min_prot_free_en : 1   ; /* [26] */
        unsigned int    reserved2               : 5   ; /* [31..27] */
    } BIT;
} volatile OC_PMIN_B_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_oca_timer_en       : 1   ; /* [0] */
        unsigned int    rg_ocb_timer_en       : 1   ; /* [1] */
        unsigned int    rg_cbc_timer_en       : 1   ;
        unsigned int    reserved0             : 29  ; /* [31..2] */
    } BIT;
} volatile OC_TIMER_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_int_evt_mode       : 1   ; /* [0] */
        unsigned int    reserved0             : 31  ; /* [31..1] */
    } BIT;
} volatile INT_EVT_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_int_en_cbc         : 1   ; /* [0] */
        unsigned int    rg_int_en_osht        : 1   ; /* [1] */
        unsigned int    rg_int_en_free        : 1   ; /* [2] */
        unsigned int    reserved0             : 1   ; /* [3] */
        unsigned int    rg_int_en_evts1       : 1   ; /* [4] */
        unsigned int    rg_int_en_evts2       : 1   ; /* [5] */
        unsigned int    rg_int_en_evts3       : 1   ; /* [6] */
        unsigned int    rg_int_en_evts4       : 1   ; /* [7] */
        unsigned int    rg_int_en_evta1       : 1   ; /* [8] */
        unsigned int    rg_int_en_evta2       : 1   ; /* [9] */
        unsigned int    rg_int_en_evtb1       : 1   ; /* [10] */
        unsigned int    rg_int_en_evtb2       : 1   ; /* [11] */
        unsigned int    rg_int_en_oca_max_cbc : 1   ; /* [12] */
        unsigned int    rg_int_en_ocb_max_cbc : 1   ; /* [13] */
        unsigned int    rg_int_en_oca_max_free : 1   ; /* [14] */
        unsigned int    rg_int_en_ocb_max_free : 1   ; /* [15] */
        unsigned int    reserved1             : 16  ; /* [31..16] */
    } BIT;
} volatile INT_EVT_EN_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_int_en_tmr         : 1   ; /* [0] */
        unsigned int    reserved0             : 31  ; /* [31..1] */
    } BIT;
} volatile INT_TMR_EN_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_int_flag_tmr       : 1   ; /* [0] */
        unsigned int    reserved0             : 15  ; /* [15..1] */
        unsigned int    rg_int_clr_tmr        : 1   ; /* [16] */
        unsigned int    reserved1             : 15  ; /* [31..17] */
    } BIT;
} volatile INT_TMR_FLAG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_int_tmr_sel        : 4   ; /* [3..0] */
        unsigned int    reserved0             : 28  ; /* [31..4] */
    } BIT;
} volatile INT_TMR_SEL_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_int_prsc_prd       : 4   ; /* [3..0] */
        unsigned int    reserved0             : 4   ; /* [7..4] */
        unsigned int    ro_int_prsc_cnt       : 4   ; /* [11..8] */
        unsigned int    reserved1             : 4   ; /* [15..12] */
        unsigned int    rg_int_prsc_phs       : 4   ; /* [19..16] */
        unsigned int    reserved2             : 4   ; /* [23..20] */
        unsigned int    rg_int_prsc_synen     : 1   ; /* [24] */
        unsigned int    rg_int_prsc_frc       : 1   ; /* [25] */
        unsigned int    reserved3             : 6   ; /* [31..26] */
    } BIT;
} volatile INT_PRSC_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_int_mix_zro_en      : 1   ; /* [0] */
        unsigned int    rg_int_mix_prd_en      : 1   ; /* [1] */
        unsigned int    rg_int_mix_refa_inc_en : 1   ; /* [2] */
        unsigned int    rg_int_mix_refa_dec_en : 1   ; /* [3] */
        unsigned int    rg_int_mix_refb_inc_en : 1   ; /* [4] */
        unsigned int    rg_int_mix_refb_dec_en : 1   ; /* [5] */
        unsigned int    rg_int_mix_refc_inc_en : 1   ; /* [6] */
        unsigned int    rg_int_mix_refc_dec_en : 1   ; /* [7] */
        unsigned int    rg_int_mix_refd_inc_en : 1   ; /* [8] */
        unsigned int    rg_int_mix_refd_dec_en : 1   ; /* [9] */
        unsigned int    rg_int_mix_emevta1_en  : 1   ; /* [10] */
        unsigned int    rg_int_mix_emevta2_en  : 1   ; /* [11] */
        unsigned int    rg_int_mix_emevtb1_en  : 1   ; /* [12] */
        unsigned int    rg_int_mix_emevtb2_en  : 1   ; /* [13] */
        unsigned int    reserved0              : 18  ; /* [31..14] */
    } BIT;
} volatile INT_MIX_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_csa_tmr_sel        : 4   ; /* [3..0] */
        unsigned int    reserved0             : 12  ; /* [15..4] */
        unsigned int    rg_csa_en_cs          : 1   ; /* [16] */
        unsigned int    reserved1             : 15  ; /* [31..17] */
    } BIT;
} volatile CS_TMR_SELA_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_csb_tmr_sel        : 4   ; /* [3..0] */
        unsigned int    reserved0             : 12  ; /* [15..4] */
        unsigned int    rg_csb_en_cs          : 1   ; /* [16] */
        unsigned int    reserved1             : 15  ; /* [31..17] */
    } BIT;
} volatile CS_TMR_SELB_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_csa_prsc_prd       : 4   ; /* [3..0] */
        unsigned int    reserved0             : 12  ; /* [15..4] */
        unsigned int    rg_csa_prsc_phs       : 4   ; /* [19..16] */
        unsigned int    reserved1             : 4   ; /* [23..20] */
        unsigned int    rg_csa_prsc_synen     : 1   ; /* [24] */
        unsigned int    rg_csa_prsc_frc       : 1   ; /* [25] */
        unsigned int    reserved2             : 6   ; /* [31..26] */
    } BIT;
} volatile CS_PRSCA_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_csb_prsc_prd       : 4   ; /* [3..0] */
        unsigned int    reserved0             : 12  ; /* [15..4] */
        unsigned int    rg_csb_prsc_phs       : 4   ; /* [19..16] */
        unsigned int    reserved1             : 4   ; /* [23..20] */
        unsigned int    rg_csb_prsc_synen     : 1   ; /* [24] */
        unsigned int    rg_csb_prsc_frc       : 1   ; /* [25] */
        unsigned int    reserved2             : 6   ; /* [31..26] */
    } BIT;
} volatile CS_PRSCB_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_csa_flag           : 1   ; /* [0] */
        unsigned int    ro_csb_flag           : 1   ; /* [1] */
        unsigned int    reserved0             : 14  ; /* [15..2] */
        unsigned int    rg_csa_clr_flag       : 1   ; /* [16] */
        unsigned int    rg_csb_clr_flag       : 1   ; /* [17] */
        unsigned int    reserved1             : 14  ; /* [31..18] */
    } BIT;
} volatile CS_FLAG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_dma_breq_sel       : 2   ; /* [1..0] */
        unsigned int    rg_dma_sreq_sel       : 2   ; /* [3..2] */
        unsigned int    reserved0             : 28  ; /* [31..4] */
    } BIT;
} volatile CS_DMA_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_soca_mix_zro_en      : 1   ; /* [0] */
        unsigned int    rg_soca_mix_prd_en      : 1   ; /* [1] */
        unsigned int    rg_soca_mix_refa_inc_en : 1   ; /* [2] */
        unsigned int    rg_soca_mix_refa_dec_en : 1   ; /* [3] */
        unsigned int    rg_soca_mix_refb_inc_en : 1   ; /* [4] */
        unsigned int    rg_soca_mix_refb_dec_en : 1   ; /* [5] */
        unsigned int    rg_soca_mix_refc_inc_en : 1   ; /* [6] */
        unsigned int    rg_soca_mix_refc_dec_en : 1   ; /* [7] */
        unsigned int    rg_soca_mix_refd_inc_en : 1   ; /* [8] */
        unsigned int    rg_soca_mix_refd_dec_en : 1   ; /* [9] */
        unsigned int    rg_soca_mix_emevta1_en  : 1   ; /* [10] */
        unsigned int    rg_soca_mix_emevta2_en  : 1   ; /* [11] */
        unsigned int    rg_soca_mix_emevtb1_en  : 1   ; /* [12] */
        unsigned int    rg_soca_mix_emevtb2_en  : 1   ; /* [13] */
        unsigned int    reserved0               : 18  ; /* [31..14] */
    } BIT;
} volatile CS_SOCA_MIX_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_socb_mix_zro_en      : 1   ; /* [0] */
        unsigned int    rg_socb_mix_prd_en      : 1   ; /* [1] */
        unsigned int    rg_socb_mix_refa_inc_en : 1   ; /* [2] */
        unsigned int    rg_socb_mix_refa_dec_en : 1   ; /* [3] */
        unsigned int    rg_socb_mix_refb_inc_en : 1   ; /* [4] */
        unsigned int    rg_socb_mix_refb_dec_en : 1   ; /* [5] */
        unsigned int    rg_socb_mix_refc_inc_en : 1   ; /* [6] */
        unsigned int    rg_socb_mix_refc_dec_en : 1   ; /* [7] */
        unsigned int    rg_socb_mix_refd_inc_en : 1   ; /* [8] */
        unsigned int    rg_socb_mix_refd_dec_en : 1   ; /* [9] */
        unsigned int    rg_socb_mix_emevta1_en  : 1   ; /* [10] */
        unsigned int    rg_socb_mix_emevta2_en  : 1   ; /* [11] */
        unsigned int    rg_socb_mix_emevtb1_en  : 1   ; /* [12] */
        unsigned int    rg_socb_mix_emevtb2_en  : 1   ; /* [13] */
        unsigned int    reserved0               : 18  ; /* [31..14] */
    } BIT;
} volatile CS_SOCB_MIX_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_em_a1_oren         : 15  ; /* [14..0] */
        unsigned int    reserved0             : 1   ; /* [15] */
        unsigned int    rg_em_a2_oren         : 15  ; /* [30..16] */
        unsigned int    reserved1             : 1   ; /* [31] */
    } BIT;
} volatile EM_AOR_EN_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_em_b1_oren         : 15  ; /* [14..0] */
        unsigned int    reserved0             : 1   ; /* [15] */
        unsigned int    rg_em_b2_oren         : 15  ; /* [30..16] */
        unsigned int    reserved1             : 1   ; /* [31] */
    } BIT;
} volatile EM_BOR_EN_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_em_a1_sel          : 4   ; /* [3..0] */
        unsigned int    rg_em_a2_sel          : 4   ; /* [7..4] */
        unsigned int    rg_em_b1_sel          : 4   ; /* [11..8] */
        unsigned int    rg_em_b2_sel          : 4   ; /* [15..12] */
        unsigned int    rg_evta1t_sel         : 3   ; /* [18..16] */
        unsigned int    reserved0             : 1   ; /* [19] */
        unsigned int    rg_evta2t_sel         : 3   ; /* [22..20] */
        unsigned int    reserved1             : 1   ; /* [23] */
        unsigned int    rg_evtb1t_sel         : 3   ; /* [26..24] */
        unsigned int    reserved2             : 1   ; /* [27] */
        unsigned int    rg_evtb2t_sel         : 3   ; /* [30..28] */
        unsigned int    reserved3             : 1   ; /* [31] */
    } BIT;
} volatile EM_MRG_SEL_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_evta1_sel          : 1   ; /* [0] */
        unsigned int    rg_evta2_sel          : 1   ; /* [1] */
        unsigned int    rg_evtb1_sel          : 1   ; /* [2] */
        unsigned int    rg_evtb2_sel          : 1   ; /* [3] */
        unsigned int    rg_evtfilt_sel        : 2   ; /* [5..4] */
        unsigned int    reserved0             : 26  ; /* [31..6] */
    } BIT;
} volatile EM_OUT_SEL_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_mskwd_alg_zroen    : 1   ; /* [0] */
        unsigned int    rg_mskwd_alg_prden    : 1   ; /* [1] */
        unsigned int    reserved0             : 2   ; /* [3..2] */
        unsigned int    rg_mskwd_psel         : 1   ; /* [4] */
        unsigned int    reserved1             : 26  ; /* [30..5] */
        unsigned int    rg_mskwd_en           : 1   ; /* [31] */
    } BIT;
} volatile EM_WD_EN_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_mskwd_offset       : 16  ; /* [15..0] */
        unsigned int    rg_mskwd_width        : 16  ; /* [31..16] */
    } BIT;
} volatile EM_WD_CNT_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_offset_cnt         : 16  ; /* [15..0] */
        unsigned int    ro_width_cnt          : 16  ; /* [31..16] */
    } BIT;
} volatile EM_WD_STS_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_vcap_sta_edg       : 4   ; /* [3..0] */
        unsigned int    reserved0             : 4   ; /* [7..4] */
        unsigned int    rg_vcap_stp_edg       : 4   ; /* [11..8] */
        unsigned int    reserved1             : 4   ; /* [15..12] */
        unsigned int    rg_vcap_edg_sel       : 1   ; /* [16] */
        unsigned int    reserved2             : 3   ; /* [19..17] */
        unsigned int    rg_vcap_trig_sel      : 3   ; /* [22..20] */
        unsigned int    reserved3             : 7   ; /* [29..23] */
        unsigned int    rg_vcap_div_mode      : 1   ; /* [30] */
        unsigned int    rg_vcap_en            : 1   ; /* [31] */
    } BIT;
} volatile EM_VCAP_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_vcap_swdly         : 16  ; /* [15..0] */
        unsigned int    rg_vcap_dly_mode      : 3   ; /* [18..16] */
        unsigned int    reserved0             : 13  ; /* [31..19] */
    } BIT;
} volatile EM_VCAP_DLY_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_vcap_cnt           : 16  ; /* [15..0] */
        unsigned int    ro_vcap_dly           : 16  ; /* [31..16] */
    } BIT;
} volatile EM_VCAP_STS1_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_vcap_sta_edgsts    : 1   ; /* [0] */
        unsigned int    ro_vcap_stp_edgsts    : 1   ; /* [1] */
        unsigned int    reserved0             : 30  ; /* [31..2] */
    } BIT;
} volatile EM_VCAP_STS2_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_filt_edg_cnt       : 4   ; /* [3..0] */
        unsigned int    reserved0             : 4   ; /* [7..4] */
        unsigned int    rg_filt_edg_sel       : 2   ; /* [9..8] */
        unsigned int    reserved1             : 21  ; /* [30..10] */
        unsigned int    rg_filt_dly_en        : 1   ; /* [31] */
    } BIT;
} volatile EM_FILT_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_tcap_sts           : 1   ; /* [0] */
        unsigned int    reserved0             : 3   ; /* [3..1] */
        unsigned int    rg_tcap_clr           : 1   ; /* [4] */
        unsigned int    reserved1             : 26  ; /* [30..5] */
        unsigned int    rg_tcap_en            : 1   ; /* [31] */
    } BIT;
} volatile EM_TCAP_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_tcap_cnt_rt        : 16  ; /* [15..0] */
        unsigned int    ro_tcap_cnt_buf       : 16  ; /* [31..16] */
    } BIT;
} volatile EM_TCAP_VAL_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_syni_sel           : 5   ; /* [4..0] */
        unsigned int    reserved0             : 11  ; /* [15..5] */
        unsigned int    ro_syni_flag          : 1   ; /* [16] */
        unsigned int    reserved1             : 3   ; /* [19..17] */
        unsigned int    rg_syni_clr           : 1   ; /* [20] */
        unsigned int    reserved2             : 11  ; /* [31..21] */
    } BIT;
} volatile SYNI_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_syncnt_a1en        : 1   ; /* [0] */
        unsigned int    rg_syncnt_b1en        : 1   ; /* [1] */
        unsigned int    rg_syncnt_synien      : 1   ; /* [2] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile SYNCNT_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_syno_zroen         : 1   ; /* [0] */
        unsigned int    rg_syno_prden         : 1   ; /* [1] */
        unsigned int    rg_syno_a1en          : 1   ; /* [2] */
        unsigned int    rg_syno_b1en          : 1   ; /* [3] */
        unsigned int    reserved0             : 1   ; /* [4] */
        unsigned int    rg_syno_refben        : 1   ; /* [5] */
        unsigned int    rg_syno_refcen        : 1   ; /* [6] */
        unsigned int    rg_syno_refden        : 1   ; /* [7] */
        unsigned int    rg_mode_syno          : 1   ; /* [8] */
        unsigned int    rg_latset_sel         : 1   ; /* [9] */
        unsigned int    rg_latset_otsyn       : 1   ; /* [10] */
        unsigned int    reserved1             : 21  ; /* [31..11] */
    } BIT;
} volatile SYNO_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_synp_sel           : 3   ; /* [2..0] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile SYNP_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_gld_zroen          : 1   ; /* [0] */
        unsigned int    rg_gld_prden          : 1   ; /* [1] */
        unsigned int    rg_gld_cntsynen       : 1   ; /* [2] */
        unsigned int    reserved0             : 5   ; /* [7..3] */
        unsigned int    rg_gld_prsc_prd       : 4   ; /* [11..8] */
        unsigned int    rg_mode_gld           : 1   ; /* [12] */
        unsigned int    reserved1             : 3   ; /* [15..13] */
        unsigned int    rg_latset_otgld       : 1   ; /* [16] */
        unsigned int    reserved2             : 15  ; /* [31..17] */
    } BIT;
} volatile GLB_LOAD_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    tc_prd_ld_sts         : 1   ; /* [0] */
        unsigned int    tc_refa_ld_sts        : 1   ; /* [1] */
        unsigned int    tc_refb_ld_sts        : 1   ; /* [2] */
        unsigned int    tc_refc_ld_sts        : 1   ; /* [3] */
        unsigned int    tc_refd_ld_sts        : 1   ; /* [4] */
        unsigned int    reserved0             : 3   ; /* [7..5] */
        unsigned int    pg_act_a_ld_sts       : 1   ; /* [8] */
        unsigned int    pg_act_b_ld_sts       : 1   ; /* [9] */
        unsigned int    pg_out_frc_ld_sts     : 1   ; /* [10] */
        unsigned int    reserved1             : 1   ; /* [11] */
        unsigned int    dg_red_ld_sts         : 1   ; /* [12] */
        unsigned int    dg_fed_ld_sts         : 1   ; /* [13] */
        unsigned int    dg_cfg_ld_sts         : 1   ; /* [14] */
        unsigned int    reserved2             : 1   ; /* [15] */
        unsigned int    tc_mwdrefa_ld_sts     : 1   ; /* [16] */
        unsigned int    tc_mwdrefb_ld_sts     : 1   ; /* [17] */
        unsigned int    tc_mwd_act_ld_sts     : 1   ; /* [18] */
        unsigned int    reserved3             : 1   ; /* [19] */
        unsigned int    pb_cfg_a_ld_sts       : 1   ; /* [20] */
        unsigned int    pb_cfg_b_ld_sts       : 1   ; /* [21] */
        unsigned int    reserved4             : 10  ; /* [31..22] */
    } BIT;
} volatile LOAD_STS_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_syncnt_frc         : 1   ; /* [0] */
        unsigned int    reserved0             : 3   ; /* [3..1] */
        unsigned int    rg_syno_frc           : 1   ; /* [4] */
        unsigned int    reserved1             : 3   ; /* [7..5] */
        unsigned int    rg_synp_frc           : 1   ; /* [8] */
        unsigned int    reserved2             : 3   ; /* [11..9] */
        unsigned int    rg_gld_frc            : 1   ; /* [12] */
        unsigned int    reserved3             : 3   ; /* [15..13] */
        unsigned int    rg_vcap_frc           : 1   ; /* [16] */
        unsigned int    reserved4             : 3   ; /* [19..17] */
        unsigned int    rg_csa_syn_frc        : 1   ; /* [20] */
        unsigned int    rg_csb_syn_frc        : 1   ; /* [21] */
        unsigned int    reserved5             : 2   ; /* [23..22] */
        unsigned int    rg_int_syn_frc        : 1   ; /* [24] */
        unsigned int    reserved6             : 3   ; /* [27..25] */
        unsigned int    rg_synpg_frc          : 1   ; /* [28] */
        unsigned int    reserved7             : 3   ; /* [31..29] */
    } BIT;
} volatile SYN_FRC_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_spr_ld_tc_off         : 1   ; /* [0] */
        unsigned int    rg_spr_ld_pg_off         : 1   ; /* [1] */
        unsigned int    rg_spr_ld_dg_off         : 1   ; /* [2] */
        unsigned int    rg_spr_ld_pb_off         : 1   ; /* [3] */
        unsigned int    rg_glb_ld_off            : 1   ; /* [4] */
        unsigned int    rg_syncnt_off            : 1   ; /* [5] */
        unsigned int    reserved0                : 26   ; /* [31..6] */
    } BIT;
} volatile ASYNC_SWITCH_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_pwma_oen_status    : 1   ; /* [0] */
        unsigned int    ro_pwma_status        : 1   ; /* [1] */
        unsigned int    reserved0             : 6   ; /* [7..2] */
        unsigned int    ro_pwmb_oen_status    : 1   ; /* [8] */
        unsigned int    ro_pwmb_status        : 1   ; /* [9] */
        unsigned int    reserved1             : 6   ; /* [15..10] */
        unsigned int    ro_refa_status        : 1   ; /* [16] */
        unsigned int    ro_refb_status        : 1   ; /* [17] */
        unsigned int    ro_refc_status        : 1   ; /* [18] */
        unsigned int    ro_refd_status        : 1   ; /* [19] */
        unsigned int    reserved2             : 12  ; /* [31..20] */
    } BIT;
} volatile PWM_STATUS_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_trip1_flag          : 1   ; /* [0] */
        unsigned int    ro_trip2_flag          : 1   ; /* [1] */
        unsigned int    ro_trip3_flag          : 1   ; /* [2] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile APT_TRIP_FLAG_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_cnt_refa           : 16   ; /* [15..0] */
        unsigned int    ro_cnt_refa_hr        : 8    ; /* [23..16] */
        unsigned int    reserved0             : 8    ; /* [31..24] */
    } BIT;
} volatile TC_REFA_DFX_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_cnt_refb           : 16   ; /* [15..0] */
        unsigned int    ro_cnt_refb_hr        : 8    ; /* [23..16] */
        unsigned int    reserved0             : 8    ; /* [31..24] */
    } BIT;
} volatile TC_REFB_DFX_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_cnt_refc           : 16   ; /* [15..0] */
        unsigned int    ro_cnt_refc_hr        : 8    ; /* [23..16] */
        unsigned int    reserved0             : 8    ; /* [31..24] */
    } BIT;
} volatile TC_REFC_DFX_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_cnt_refd           : 16   ; /* [15..0] */
        unsigned int    ro_cnt_refd_hr        : 8    ; /* [23..16] */
        unsigned int    reserved0             : 8    ; /* [31..24] */
    } BIT;
} volatile TC_REFD_DFX_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_hr_en               : 1   ; /* [0] */
        unsigned int    rg_hr_tech             : 1   ; /* [1] */
        unsigned int    rg_hr_ref_autoconv_off : 1   ; /* [2] */
        unsigned int    reserved0              : 29  ; /* [31..3] */
    } BIT;
} volatile HR_CFG_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_hr_cnt             : 8   ; /* [7...0] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile HR_CNT_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_hrb_scale_offset   : 4   ; /* [3...0] */
        unsigned int    reserved0             : 4   ; /* [7...4] */
        unsigned int    rg_hra_scale_offset   : 4   ; /* [11...8] */
        unsigned int    reserved1             : 20  ; /* [31..12] */
    } BIT;
} volatile HR_SCALE_CFG_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_hra_scale          : 8   ; /* [7..0] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile HR_SCALE_A_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    ro_hrb_scale          : 8   ; /* [7..0] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile HR_SCALE_B_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_mdb_en_a           : 1   ; /* [0] */
        unsigned int    reserved0             : 1   ; /* [1] */
        unsigned int    rg_mdb_inv_a          : 1   ; /* [2] */
        unsigned int    rg_mdb_selblock_a     : 1   ; /* [3] */
        unsigned int    rg_mdb_sel_a          : 4   ; /* [7..4] */
        unsigned int    rg_mdb_polsel_a       : 2   ; /* [9..8] */
        unsigned int    reserved1             : 4   ; /* [13..10] */
        unsigned int    rg_mdb_dly_mode_a     : 2   ; /* [15..14] */
        unsigned int    rg_mdb_en_b           : 1   ; /* [16] */
        unsigned int    reserved2             : 1   ; /* [17] */
        unsigned int    rg_mdb_inv_b          : 1   ; /* [18] */
        unsigned int    rg_mdb_selblock_b     : 1   ; /* [19] */
        unsigned int    rg_mdb_sel_b          : 4   ; /* [23..20] */
        unsigned int    rg_mdb_polsel_b       : 2   ; /* [25..24] */
        unsigned int    reserved3             : 4   ; /* [29..26] */
        unsigned int    rg_mdb_dly_mode_b     : 2   ; /* [31..30] */
    } BIT;
} volatile MINDB_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_mdb_dly_a          : 16  ; /* [15..0] */
        unsigned int    rg_mdb_dly_b          : 16  ; /* [31..16] */
    } BIT;
} volatile MINDB_DLY_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_olut_en_a          : 1   ; /* [0] */
        unsigned int    reserved0             : 3   ; /* [3..1] */
        unsigned int    rg_selxbar_a          : 4   ; /* [7..4] */
        unsigned int    reserved1             : 8   ; /* [15..8] */
        unsigned int    rg_lutdec_a           : 8   ; /* [23..16] */
        unsigned int    reserved2             : 8   ; /* [31..24] */
    } BIT;
} volatile OLUTA_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_olut_en_b          : 1   ; /* [0] */
        unsigned int    reserved0             : 3   ; /* [3..1] */
        unsigned int    rg_selxbar_b          : 4   ; /* [7..4] */
        unsigned int    reserved1             : 8   ; /* [15..8] */
        unsigned int    rg_lutdec_b           : 8   ; /* [23..16] */
        unsigned int    reserved2             : 8   ; /* [31..24] */
    } BIT;
} volatile OLUTB_CFG_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_tc_prd_link        : 5   ; /* [4..0] */
        unsigned int    rg_tc_refa_link       : 5   ; /* [9..5] */
        unsigned int    rg_tc_refb_link       : 5   ; /* [14..10] */
        unsigned int    reserved0             : 1   ; /* [15] */
        unsigned int    rg_tc_refc_link       : 5   ; /* [20..16] */
        unsigned int    rg_tc_refd_link       : 5   ; /* [25..21] */
        unsigned int    rg_syn_frc_link       : 5   ; /* [30..26] */
        unsigned int    reserved1             : 1   ; /* [31] */
    } BIT;
} volatile LINK_CFG0_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_dg_red_link        : 5   ; /* [4..0] */
        unsigned int    rg_dg_fed_link        : 5   ; /* [9..5] */
        unsigned int    rg_dg_cfg_link        : 5   ; /* [14..10] */
        unsigned int    reserved0             : 1   ; /* [15] */
        unsigned int    rg_pc_cfg_link        : 5   ; /* [20..16] */
        unsigned int    rg_pb_cfg_a_link      : 5   ; /* [25..21] */
        unsigned int    rg_pb_cfg_b_link      : 5   ; /* [30..26] */
        unsigned int    reserved1             : 1   ; /* [31] */
    } BIT;
} volatile LINK_CFG1_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_pg_act_a_link        : 5   ; /* [4..0] */
        unsigned int    rg_pg_act_b_link        : 5   ; /* [9..5] */
        unsigned int    rg_pg_act_frc_link      : 5   ; /* [14..10] */
        unsigned int    reserved0               : 1   ; /* [15] */
        unsigned int    rg_pg_out_frc_link      : 5   ; /* [20..16] */
        unsigned int    rg_oc_frc_evt_link      : 5   ; /* [25..21] */
        unsigned int    rg_oc_sw_clr_link       : 5   ; /* [30..26] */
        unsigned int    reserved1               : 1   ; /* [31] */
    } BIT;
} volatile LINK_CFG2_REG;


typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_glb_load_link        : 5   ; /* [4..0] */
        unsigned int    rg_async_switch_link    : 5   ; /* [9..5] */
        unsigned int    reserved0               : 22  ; /* [31..10] */
    } BIT;
} volatile LINK_CFG3_REG;

//==============================================================================
/* Define the global struct */
typedef struct {
    VER_INFO_REG             VER_INFO                         ; /* 0x0 */
    PROT_KEY_REG             PROT_KEY                         ; /* 0x4 */
    unsigned int             reserved0[2]                     ; /* 0x8~0xc */
    TC_MODE_REG              TC_MODE                          ; /* 0x10 */
    TC_PHS_REG               TC_PHS                           ; /* 0x14 */
    TC_OVRID_REG             TC_OVRID                         ; /* 0x18 */
    unsigned int             reserved1                        ; /* 0x1c */
    TC_PRD_REG               TC_PRD                           ; /* 0x20 */
    unsigned int             reserved2[3]                     ; /* 0x24~0x2c */
    TC_REFA_REG              TC_REFA                          ; /* 0x30 */
    TC_REFB_REG              TC_REFB                          ; /* 0x34 */
    TC_REFC_REG              TC_REFC                          ; /* 0x38 */
    TC_REFD_REG              TC_REFD                          ; /* 0x3c */
    unsigned int             reserved3[4]                     ; /* 0x40~0x4c */
    TC_BUF_EN_REG            TC_BUF_EN                        ; /* 0x50 */
    TC_PRD_LOAD_REG          TC_PRD_LOAD                      ; /* 0x54 */
    TC_REF_LOAD_REG          TC_REF_LOAD                      ; /* 0x58 */
    unsigned int             reserved4[6]                     ; /* 0x5c~0x70 */
    TC_STS_REG               TC_STS                           ; /* 0x74 */
    unsigned int             reserved5[34]                    ; /* 0x78~0xfc */
    PG_ACT_A_REG             PG_ACT_A                         ; /* 0x100 */
    PG_ACT_B_REG             PG_ACT_B                         ; /* 0x104 */
    unsigned int             reserved6[2]                     ; /* 0x108~0x10c */
    PG_ACT_FRC_REG           PG_ACT_FRC                       ; /* 0x110 */
    PG_OUT_FRC_REG           PG_OUT_FRC                       ; /* 0x114 */
    unsigned int             reserved7[2]                     ; /* 0x118~0x11c */
    PG_BUF_EN_REG            PG_BUF_EN                        ; /* 0x120 */
    unsigned int             reserved8[3]                     ; /* 0x124~0x12c */
    PG_ACT_LD_REG            PG_ACT_LD                        ; /* 0x130 */
    unsigned int             reserved9[3]                     ; /* 0x134~0x13c */
    PG_EVTC_SEL_REG          PG_EVTC_SEL                      ; /* 0x140 */
    unsigned int             reserved10[47]                   ; /* 0x144~0x1fc */
    DG_RED_REG               DG_RED                           ; /* 0x200 */
    DG_FED_REG               DG_FED                           ; /* 0x204 */
    DG_CFG_REG               DG_CFG                           ; /* 0x208 */
    unsigned int             reserved11                       ; /* 0x20c */
    DG_BUF_EN_REG            DG_BUF_EN                        ; /* 0x210 */
    DG_BUF_LOAD_REG          DG_BUF_LOAD                      ; /* 0x214 */
    unsigned int             reserved12[18]                   ; /* 0x218~0x25c */
    PC_CFG_REG               PC_CFG                           ; /* 0x260 */
    PC_FREQ_REG              PC_FREQ                          ; /* 0x264 */
    PC_SHAP_REG              PC_SHAP                          ; /* 0x268 */
    PC_INIT_WIDTH_REG        PC_INIT_WIDTH                    ; /* 0x26c */
    unsigned int             reserved13[4]                    ; /* 0x270~0x27c */
    PB_CFG_A_REG             PB_CFG_A                         ; /* 0x280 */
    PB_CFG_B_REG             PB_CFG_B                         ; /* 0x284 */
    PB_STATUS_REG            PB_STATUS                        ; /* 0x288 */
    PB_BUF_EN_REG            PB_BUF_EN                        ; /* 0x28c */
    PB_BUF_LOAD_REG          PB_BUF_LOAD                      ; /* 0x290 */
    unsigned int             reserved14[27]                   ; /* 0x294~0x2fc */
    OC_CBC_CFG_REG           OC_CBC_CFG                       ; /* 0x300 */
    OC_OSHT_CFG_REG          OC_OSHT_CFG                      ; /* 0x304 */
    OC_FREE_CFG_REG          OC_FREE_CFG                      ; /* 0x308 */
    unsigned int             reserved15                       ; /* 0x30c */
    OC_ACT_A_REG             OC_ACT_A                         ; /* 0x310 */
    OC_ACT_B_REG             OC_ACT_B                         ; /* 0x314 */
    unsigned int             reserved16[2]                    ; /* 0x318~0x31c */
    OC_EVT_FLAG_REG          OC_EVT_FLAG                      ; /* 0x320 */
    OC_PRD_CLR_REG           OC_PRD_CLR                       ; /* 0x324 */
    OC_SW_CLR_REG            OC_SW_CLR                        ; /* 0x328 */
    unsigned int             reserved17                       ; /* 0x32c */
    OC_FRC_EVT_REG           OC_FRC_EVT                       ; /* 0x330 */
    OC_STATUS_REG            OC_STATUS                        ; /* 0x334 */
    unsigned int             reserved18[18]                   ; /* 0x338~0x37c */
    OC_EVT_DLY_REG           OC_EVT_DLY                       ; /* 0x380 */
    OC_DLY_A_REG             OC_DLY_A                         ; /* 0x384 */
    unsigned int             reserved19                       ; /* 0x388 */
    OC_DLY_B_REG             OC_DLY_B                         ; /* 0x38c */
    OC_PMAX_A_REG            OC_PMAX_A                        ; /* 0x390 */
    OC_PMIN_A_REG            OC_PMIN_A                        ; /* 0x394 */
    unsigned int             reserved20[2]                    ; /* 0x398~0x39c */
    OC_PMAX_B_REG            OC_PMAX_B                        ; /* 0x3a0 */
    OC_PMIN_B_REG            OC_PMIN_B                        ; /* 0x3a4 */
    unsigned int             reserved21[10]                   ; /* 0x3a8~0x3cc */
    OC_TIMER_CFG_REG         OC_TIMER_CFG                     ; /* 0x3d0 */
    unsigned int             OC_TIMER_A                       ; /* 0x3d4 */
    unsigned int             OC_TIMER_B                       ; /* 0x3d8 */
    unsigned int             OC_CBC_TIMER                     ; /* 0x3dc */
    unsigned int             OC_CBC_TIMER_OSHT                ; /* 0x3e0 */
    unsigned int             OC_CBC_TIMER_CLR                 ; /* 0x3e4 */
    unsigned int             reserved22[9]                    ; /* 0x3e8~0x408 */
    INT_EVT_CFG_REG          INT_EVT_CFG                      ; /* 0x40c */
    INT_EVT_EN_REG           INT_EVT_EN                       ; /* 0x410 */
    INT_TMR_EN_REG           INT_TMR_EN                       ; /* 0x414 */
    unsigned int             reserved23[2]                    ; /* 0x418~0x41c */
    INT_TMR_FLAG_REG         INT_TMR_FLAG                     ; /* 0x420 */
    INT_TMR_SEL_REG          INT_TMR_SEL                      ; /* 0x424 */
    INT_PRSC_CFG_REG         INT_PRSC_CFG                     ; /* 0x428 */
    INT_MIX_CFG_REG          INT_MIX_CFG                      ; /* 0x42c */
    unsigned int             reserved24[52]                   ; /* 0x430~0x4fc */
    CS_TMR_SELA_REG          CS_TMR_SELA                      ; /* 0x500 */
    CS_TMR_SELB_REG          CS_TMR_SELB                      ; /* 0x504 */
    CS_PRSCA_CFG_REG         CS_PRSCA_CFG                     ; /* 0x508 */
    CS_PRSCB_CFG_REG         CS_PRSCB_CFG                     ; /* 0x50c */
    CS_FLAG_REG              CS_FLAG                          ; /* 0x510 */
    unsigned int             reserved25[3]                    ; /* 0x514~0x51c */
    CS_DMA_REG               CS_DMA                           ; /* 0x520 */
    unsigned int             reserved26[3]                    ; /* 0x524~0x52c */
    CS_SOCA_MIX_CFG_REG      CS_SOCA_MIX_CFG                  ; /* 0x530 */
    CS_SOCB_MIX_CFG_REG      CS_SOCB_MIX_CFG                  ; /* 0x534 */
    unsigned int             reserved27[52]                   ; /* 0x538~0x604 */
    EM_AOR_EN_REG            EM_AOR_EN                        ; /* 0x608 */
    EM_BOR_EN_REG            EM_BOR_EN                        ; /* 0x60c */
    EM_MRG_SEL_REG           EM_MRG_SEL                       ; /* 0x610 */
    EM_OUT_SEL_REG           EM_OUT_SEL                       ; /* 0x614 */
    unsigned int             reserved28[2]                    ; /* 0x618~0x61c */
    EM_WD_EN_REG             EM_WD_EN                         ; /* 0x620 */
    EM_WD_CNT_REG            EM_WD_CNT                        ; /* 0x624 */
    unsigned int             reserved29                       ; /* 0x628 */
    EM_WD_STS_REG            EM_WD_STS                        ; /* 0x62c */
    EM_VCAP_CFG_REG          EM_VCAP_CFG                      ; /* 0x630 */
    EM_VCAP_DLY_REG          EM_VCAP_DLY                      ; /* 0x634 */
    unsigned int             reserved30[2]                    ; /* 0x638~0x63c */
    EM_VCAP_STS1_REG         EM_VCAP_STS1                     ; /* 0x640 */
    EM_VCAP_STS2_REG         EM_VCAP_STS2                     ; /* 0x644 */
    unsigned int             reserved31[2]                    ; /* 0x648~0x64c */
    EM_FILT_CFG_REG          EM_FILT_CFG                      ; /* 0x650 */
    unsigned int             reserved32[3]                    ; /* 0x654~0x65c */
    EM_TCAP_CFG_REG          EM_TCAP_CFG                      ; /* 0x660 */
    unsigned int             reserved33[3]                    ; /* 0x664~0x66c */
    EM_TCAP_VAL_REG          EM_TCAP_VAL                      ; /* 0x670 */
    unsigned int             reserved34[35]                   ; /* 0x674~0x6fc */
    SYNI_CFG_REG             SYNI_CFG                         ; /* 0x700 */
    SYNCNT_CFG_REG           SYNCNT_CFG                       ; /* 0x704 */
    SYNO_CFG_REG             SYNO_CFG                         ; /* 0x708 */
    SYNP_CFG_REG             SYNP_CFG                         ; /* 0x70c */
    GLB_LOAD_REG             GLB_LOAD                         ; /* 0x710 */
    unsigned int             reserved35[3]                    ; /* 0x714~0x71c */
    LOAD_STS_REG             LOAD_STS                         ; /* 0x720 */
    unsigned int             reserved36[3]                    ; /* 0x724~0x72c */
    SYN_FRC_REG              SYN_FRC                          ; /* 0x730 */
    unsigned int             reserved37[3]                    ; /* 0x734~0x73c */
    ASYNC_SWITCH_REG         ASYNC_SWITCH                     ; /* 0x740 */
    unsigned int             reserved38[47]                   ; /* 0x744~0x7fc */
    PWM_STATUS_REG           PWM_STATUS                       ; /* 0x800 */
    APT_TRIP_FLAG_REG        APT_TRIP_FLAG                    ; /* 0x804 */
    unsigned int             reserved39[10]                   ; /* 0x808~0x82c */
    TC_REFA_DFX_REG          TC_REFA_DFX;                     ; /* 0x830 */
    TC_REFB_DFX_REG          TC_REFB_DFX;                     ; /* 0x834 */
    TC_REFC_DFX_REG          TC_REFC_DFX;                     ; /* 0x838 */
    TC_REFD_DFX_REG          TC_REFD_DFX;                     ; /* 0x83c */
    unsigned int             reserved40[48]                   ; /* 0x840~0x8fc */
    HR_CFG_REG               HR_CFG                           ; /* 0x900 */
    HR_CNT_REG               HR_CNT                           ; /* 0x904 */
    HR_SCALE_CFG_REG         HR_SCALE_CFG                     ; /* 0x908 */
    unsigned int             reserved41                       ; /* 0x90c */
    HR_SCALE_A_REG           HR_SCALE_A                       ; /* 0x910 */
    HR_SCALE_B_REG           HR_SCALE_B                       ; /* 0x914 */
    unsigned int             reserved42[58]                   ; /* 0x918~0x9fc */
    MINDB_CFG_REG            MINDB_CFG                        ; /* 0xa00 */
    MINDB_DLY_REG            MINDB_DLY                        ; /* 0xa04 */
    unsigned int             reserved43[30]                   ; /* 0xa08~0xa7c */
    OLUTA_CFG_REG            OLUTA_CFG                        ; /* 0xa80 */
    OLUTB_CFG_REG            OLUTB_CFG                        ; /* 0xa84 */
    unsigned int             reserved44[30]                   ; /* 0xa88~0xafc */
    LINK_CFG0_REG            LINK_CFG0                        ; /* 0xb00 */
    LINK_CFG1_REG            LINK_CFG1                        ; /* 0xb04 */
    LINK_CFG2_REG            LINK_CFG2                        ; /* 0xb08 */
    LINK_CFG3_REG            LINK_CFG3                        ; /* 0xb0c */
} volatile APT_RegStruct;

// ---------------------- DCL ------------------------------------------------------------
static inline bool IsAptOcMode(APT_OutCtrlMode ocMode)
{
    APT_OutCtrlMode modeMax = APT_OUT_CTRL_CYCLE_BY_CYCLE | APT_OUT_CTRL_ONE_SHOT | APT_OUT_CTRL_FREE;
    return ((ocMode >= APT_OUT_CTRL_CYCLE_BY_CYCLE) && (ocMode <= modeMax));
}

/**
  * @brief Get the main version number and sub version number of APT module.
  * @param aptx APT register base address.
  * @param mainVer Main version number.
  * @param subVer Sub version number.
  * @retval None.
  */
static inline void DCL_APT_GetAptVerNo(APT_RegStruct *aptx, unsigned int *mainVer, unsigned int *subVer)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    *mainVer = aptx->VER_INFO.BIT.main_version;
    *subVer  = aptx->VER_INFO.BIT.sub_version;
}

/**
  * @brief Lock APT register which support write protect key.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_LockReg(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Unlock register which support write protect key.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_UnLockReg(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
}

/**
  * @brief Set the emulation stop mode of APT module.
  * @param aptx APT register base address.
  * @param emuMode Emulation stop mode.
  * @retval None.
  */
static inline void DCL_APT_SetEmulationMode(APT_RegStruct *aptx, APT_EmulationMode emuMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(emuMode >= APT_EMULATION_NO_STOP && emuMode <= APT_EMULATION_STOP_APT);
    aptx->TC_MODE.BIT.rg_emu_stop = emuMode;
}

/**
  * @brief Set the time-base divider factor.
  * @param aptx APT register base address.
  * @param divFactor Time-base divider factor.
  * @retval None.
  */
static inline void DCL_APT_SetDividerFactor(APT_RegStruct *aptx, unsigned short divFactor)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(divFactor <= DIVIDER_FACTOR_MAX);
    aptx->TC_MODE.BIT.rg_div_fac = divFactor;
}

/**
  * @brief Get the time-base divider factor.
  * @param aptx APT register base address.
  * @retval unsigned short: time-base divider factor.
  */
static inline unsigned short DCL_APT_GetDividerFactor(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->TC_MODE.BIT.rg_div_fac);
}

/**
  * @brief Set the count mode of time-base counter.
  * @param aptx APT register base address.
  * @param cntMode Count mode.
  * @retval None.
  */
static inline void DCL_APT_SetTimeBaseCountMode(APT_RegStruct *aptx, APT_CountMode cntMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(cntMode >= APT_COUNT_MODE_UP && cntMode <= APT_COUNT_MODE_FREEZE);
    aptx->TC_MODE.BIT.rg_cnt_mode = cntMode;
}

/**
  * @brief Set the period of time-base counter.
  * @param aptx APT register base address.
  * @param periodCnt Time-base counter period.
  * @retval None.
  */
static inline void DCL_APT_SetTimeBasePeriod(APT_RegStruct *aptx, unsigned short periodCnt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->TC_PRD.BIT.rg_cnt_prd = periodCnt;
}

/**
  * @brief Set the hr period of time-base counter.
  * @param aptx APT register base address.
  * @param periodCnt High resolusion time-base counter period.
  * @retval None.
  */
static inline void DCL_APT_SetTimeBasePeriodHr(APT_RegStruct *aptx, unsigned char periodCntHr)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->TC_PRD.BIT.rg_cnt_prd_hr = periodCntHr;
}

/**
  * @brief Get the period of time-base counter.
  * @param aptx APT register base address.
  * @retval unsigned short: time-base counter period
  */
static inline unsigned short DCL_APT_GetTimeBasePeriod(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->TC_PRD.BIT.rg_cnt_prd);
}

/**
  * @brief Get the hr period of time-base counter.
  * @param aptx APT register base address.
  * @retval unsigned char: time-base counter period of high resolusion part.
  */
static inline unsigned char DCL_APT_GetTimeBasePeriodHr(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->TC_PRD.BIT.rg_cnt_prd_hr);
}

/**
  * @brief Set the count mode of slave APT module after synchronization.
  * @param aptx APT register base address.
  * @param syncCntMode Count mode after synchronization.
  * @retval None.
  */
static inline void DCL_APT_SetCountModeAfterSync(APT_RegStruct *aptx, APT_SyncCountMode syncCntMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(syncCntMode >= APT_COUNT_MODE_AFTER_SYNC_DOWN);
    APT_PARAM_CHECK_NO_RET(syncCntMode <= APT_COUNT_MODE_AFTER_SYNC_UP);
    aptx->TC_PHS.BIT.rg_cnt_dir = syncCntMode;
}

/**
  * @brief Set the counter phase after synchronization.
  * @param aptx APT register base address.
  * @param cntPhase Counter phase after synchronization, 0 ~ 65535.
  * @retval None.
  */
static inline void DCL_APT_SetCounterPhase(APT_RegStruct *aptx, unsigned short cntPhase)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->TC_PHS.BIT.rg_cnt_phs = cntPhase;
}

/**
  * @brief Set the hr counter phase after synchronization.
  * @param aptx APT register base address.
  * @param cntPhase High resolusion counter phase after synchronization, 0 ~ 255.
  * @retval None.
  */
static inline void DCL_APT_SetCounterPhaseHr(APT_RegStruct *aptx, unsigned char cntPhaseHr)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->TC_PHS.BIT.rg_cnt_phs_hr = cntPhaseHr;
}

/**
  * @brief Set the software override value of time-base counter.
  * @param aptx APT register base address.
  * @param cntOvrid Software override value of time-base counter.
  * @retval None.
  */
static inline void DCL_APT_SetCounterOverride(APT_RegStruct *aptx, unsigned short cntOvrid)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->TC_OVRID.BIT.rg_cnt_ovrid = cntOvrid;
}

/**
  * @brief Force software override on time-base divider and counter.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_ForceOverride(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->TC_OVRID.BIT.rg_cnt_ovrid_en = BASE_CFG_SET;
}

/**
  * @brief Set the count compare reference value of time-base counter.
  * @param aptx APT register base address.
  * @param ref Count compare reference.
  * @param cntCmp Count compare reference value of counter.
  * @retval None.
  */
static inline void DCL_APT_SetCounterCompare(APT_RegStruct *aptx, APT_CompareRef ref, unsigned short cntCmp)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ref >= APT_COMPARE_REFERENCE_A);
    APT_PARAM_CHECK_NO_RET(ref <= APT_COMPARE_REFERENCE_D);
    switch (ref) {
        case APT_COMPARE_REFERENCE_A:
            aptx->TC_REFA.BIT.rg_cnt_refa = cntCmp;
            break;
        case APT_COMPARE_REFERENCE_B:
            aptx->TC_REFB.BIT.rg_cnt_refb = cntCmp;
            break;
        case APT_COMPARE_REFERENCE_C:
            aptx->TC_REFC.BIT.rg_cnt_refc = cntCmp;
            break;
        case APT_COMPARE_REFERENCE_D:
            aptx->TC_REFD.BIT.rg_cnt_refd = cntCmp;
            break;
        default:
            break;
    }
}

/**
  * @brief Set the hr count compare reference value of time-base counter.
  * @param aptx APT register base address.
  * @param ref Count compare reference.
  * @param cntCmp Hr count compare reference value of counter.
  * @retval None.
  */
static inline void DCL_APT_SetCounterCompareHr(APT_RegStruct *aptx, APT_CompareRef ref, unsigned char cntCmpHr)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ref >= APT_COMPARE_REFERENCE_A);
    APT_PARAM_CHECK_NO_RET(ref <= APT_COMPARE_REFERENCE_D);
    switch (ref) {
        case APT_COMPARE_REFERENCE_A:
            /* Ser high resolution compare point A. */
            aptx->TC_REFA.BIT.rg_cnt_refa_hr = cntCmpHr;
            break;
        case APT_COMPARE_REFERENCE_B:
            /* Ser high resolution compare point A. */
            aptx->TC_REFB.BIT.rg_cnt_refb_hr = cntCmpHr;
            break;
        case APT_COMPARE_REFERENCE_C:
            /* Ser high resolution compare point A. */
            aptx->TC_REFC.BIT.rg_cnt_refc_hr = cntCmpHr;
            break;
        case APT_COMPARE_REFERENCE_D:
            /* Ser high resolution compare point A. */
            aptx->TC_REFD.BIT.rg_cnt_refd_hr = cntCmpHr;
            break;
        default:
            break;
    }
}

/**
  * @brief Get the count compare reference value of time-base counter.
  * @param aptx APT register base address.
  * @param ref Count compare reference.
  * @retval unsigned short: Count compare reference value of counter.
  */
static inline unsigned short DCL_APT_GetCounterCompare(APT_RegStruct *aptx, APT_CompareRef ref)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(ref >= APT_COMPARE_REFERENCE_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ref <= APT_COMPARE_REFERENCE_D, BASE_STATUS_ERROR);
    switch (ref) {
        case APT_COMPARE_REFERENCE_A:
            return (aptx->TC_REFA.BIT.rg_cnt_refa);
        case APT_COMPARE_REFERENCE_B:
            return (aptx->TC_REFB.BIT.rg_cnt_refb);
        case APT_COMPARE_REFERENCE_C:
            return (aptx->TC_REFC.BIT.rg_cnt_refc);
        case APT_COMPARE_REFERENCE_D:
            return (aptx->TC_REFD.BIT.rg_cnt_refd);
        default:
            return 0;
    }
}

/**
  * @brief Get the hr count compare reference value of time-base counter.
  * @param aptx APT register base address.
  * @param ref Hr count compare reference.
  * @retval unsigned char: Hr count compare reference value of counter.
  */
static inline unsigned char DCL_APT_GetCounterCompareHr(APT_RegStruct *aptx, APT_CompareRef ref)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(ref >= APT_COMPARE_REFERENCE_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ref <= APT_COMPARE_REFERENCE_D, BASE_STATUS_ERROR);
    switch (ref) {
        case APT_COMPARE_REFERENCE_A:
            /* Get high resolution compare point A. */
            return (aptx->TC_REFA.BIT.rg_cnt_refa_hr);
        case APT_COMPARE_REFERENCE_B:
            /* Get high resolution compare point A. */
            return (aptx->TC_REFB.BIT.rg_cnt_refb_hr);
        case APT_COMPARE_REFERENCE_C:
            /* Get high resolution compare point A. */
            return (aptx->TC_REFC.BIT.rg_cnt_refc_hr);
        case APT_COMPARE_REFERENCE_D:
            /* Get high resolution compare point A. */
            return (aptx->TC_REFD.BIT.rg_cnt_refd_hr);
        default:
            return 0;
    }
}

/**
  * @brief Set the buffer load mode of time-base period register.
  * @param aptx APT register base address.
  * @param prdLoadMode Buffer load mode of time-base period register.
  * @retval None.
  */
static inline void DCL_APT_SetPeriodLoadMode(APT_RegStruct *aptx, APT_BufferLoadMode prdLoadMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(prdLoadMode >= APT_BUFFER_DISABLE);
    APT_PARAM_CHECK_NO_RET(prdLoadMode <= APT_BUFFER_GLOBAL_LOAD);
    aptx->TC_BUF_EN.reg &= (~0b11);      /* Clear rg_prd_buf_en and rg_prd_gld_en */
    aptx->TC_BUF_EN.reg |= prdLoadMode;  /* Write rg_prd_buf_en and rg_prd_gld_en */
}

/**
 * @brief Enable the buffer load events of TC_PRD register
 * @param aptx APT register base address.
 * @param loadEvent The buffer load events of TC_PRD register, it supports |.
 *        A logical OR of valid values that can be passed as the loadEvent parameter
 *        Valid values for loadEvent are:
 *            APT_PERIOD_LOAD_EVENT_ZERO      -   When counter value equal to zeor
 *            APT_PERIOD_LOAD_EVENT_A1        -   When combined event A1 is valid
 *            APT_PERIOD_LOAD_EVENT_B1        -   When combined event B1 is valid
 *            APT_PERIOD_LOAD_EVENT_SYNC      -   When synchronization event is valid
 * @retval None.
 */
static inline void DCL_APT_SetPeriodLoadEvent(APT_RegStruct *aptx, APT_PrdLoadEvent prdLoadEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(prdLoadEvent >= APT_PERIOD_LOAD_EVENT_ZERO);
    APT_PARAM_CHECK_NO_RET(prdLoadEvent <= APT_PERIOD_LOAD_EVENT_MAX_VALUE);
    aptx->TC_PRD_LOAD.reg = prdLoadEvent;
}

/**
  * @brief Set the buffer load mode of count compare reference register.
  * @param aptx APT register base address.
  * @param ref Count compare reference.
  * @param cmpLoadMode Buffer load mode of count compare reference register.
  * @retval None.
  */
static inline void DCL_APT_SetCompareLoadMode(APT_RegStruct *aptx,
                                              APT_CompareRef ref,
                                              APT_BufferLoadMode cmpLoadMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ref >= APT_COMPARE_REFERENCE_A);
    APT_PARAM_CHECK_NO_RET(ref <= APT_COMPARE_REFERENCE_D);
    APT_PARAM_CHECK_NO_RET(cmpLoadMode >= APT_BUFFER_DISABLE);
    APT_PARAM_CHECK_NO_RET(cmpLoadMode <= APT_BUFFER_GLOBAL_LOAD);
    unsigned int offsetA = 4;                       /* Buffer mode control bits offset of reference A */
    unsigned int tcBufField = 2;                    /* Field width of buffer load mode setting */
    unsigned int offset = offsetA + ref * tcBufField;
    aptx->TC_BUF_EN.reg &= (~(0b11 << offset));     /* Clear rg_refx_gld_en and rg_refx_buf_en */
    aptx->TC_BUF_EN.reg |= (cmpLoadMode << offset); /* Write rg_refx_gld_en and rg_refx_buf_en */
}

/**
 * @brief Enable the buffer load events of TC_REFA, TC_REFB, TC_REFC, TC_REFD register
 * @param aptx APT register base address.
 * @param ref Count compare reference
 * @param loadEvent The buffer load events of TC_REFA, TC_REFB, TC_REFC, TC_REFD register, it supports |.
 *        A logical OR of valid values can be passed as the loadEvent parameter
 *        Valid values for loadEvent are:
 *            APT_COMPARE_LOAD_EVENT_ZERO     -   When counter value equal to zero
 *            APT_COMPARE_LOAD_EVENT_PERIOD   -   When counter value equal to period
 *            APT_COMPARE_LOAD_EVENT_ZERO_PERIOD - When counter value equal to zero and period
 *            APT_COMPARE_LOAD_EVENT_A1       -   When combined event A1 is valid
 *            APT_COMPARE_LOAD_EVENT_B1       -   When combined event B1 is valid
 *            APT_COMPARE_LOAD_EVENT_SYNC     -   When synchronization event is valid
 * @retval None.
 */
static inline void DCL_APT_SetCompareLoadEvent(APT_RegStruct *aptx, APT_CompareRef ref, APT_CmpLoadEvent cmpLoadEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ref >= APT_COMPARE_REFERENCE_A);
    APT_PARAM_CHECK_NO_RET(ref <= APT_COMPARE_REFERENCE_D);
    APT_PARAM_CHECK_NO_RET(cmpLoadEvent >= APT_COMPARE_LOAD_EVENT_ZERO);
    APT_PARAM_CHECK_NO_RET(cmpLoadEvent <= APT_COMPARE_LOAD_EVENT_MAX_VALUE);

    unsigned int refBufField = 8; /* Field width of compare reference load event setting  */
    aptx->TC_REF_LOAD.reg &= (~(0x1F << (ref * refBufField))); /* Clear bit field for load event selection */
    aptx->TC_REF_LOAD.reg |= (cmpLoadEvent << (ref * refBufField));
}

/**
  * @brief Get the value of time-base divider.
  * @param aptx APT register base address.
  * @retval unsigned short: The value of time-base divider value.
  */
static inline unsigned short DCL_APT_GetDividerValue(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->TC_STS.BIT.ro_div_cnt);
}

/**
  * @brief Get the value of time-base counter.
  * @param aptx APT register base address.
  * @retval unsigned short: The value of time-base counter.
  */
static inline unsigned short DCL_APT_GetCounterValue(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->TC_STS.BIT.ro_cnt_val);
}

/**
 * @brief Return time base counter direction
 * @param aptx  APT register base address.
 * @retval APT_CounterDir: The direction of time base counter
 *         Valid return values are:
 *             APT_COUNTER_STATUS_COUNT_DOWN   -   The counter is counting down
 *             APT_COUNTER_STATUS_COUNT_UP     -   The counter is counting up
 */
static inline APT_CounterDir DCL_APT_GetCounterDirection(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->TC_STS.BIT.ro_cnt_dir);
}

/* --------------------------------------------------------------------------------------------- */
/* PWM Generation (PG) submodule Direct Configuration Layer functions -------------------------- */
/* --------------------------------------------------------------------------------------------- */
/**
  * @brief Set PWM waveform action on corresponding event.
  * @param aptx APT register base address.
  * @param channel PWM output channel.
  * @param actEvent PWM waveform action event.
  * @param action PWM waveform action.
  * @retval None.
  */
static inline void DCL_APT_SetPWMAction(APT_RegStruct *aptx,
                                        APT_PWMChannel channel,
                                        APT_PWMActionEvent actEvent,
                                        APT_PWMAction action)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(channel >= APT_PWM_CHANNEL_A && channel <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(actEvent >= APT_PWM_ACTION_ON_TIMEBASE_ZERO && actEvent <= APT_PWM_ACTION_ON_C2_COUNT_DOWN);
    APT_PARAM_CHECK_NO_RET(action >= APT_PWM_ACTION_HOLD && action <= APT_PWM_ACTION_TOGGLE);
    if (channel == APT_PWM_CHANNEL_A) {
        aptx->PG_ACT_A.reg &= (~(0b11 << actEvent));
        aptx->PG_ACT_A.reg |= (action << actEvent);
    } else if (channel == APT_PWM_CHANNEL_B) {
        aptx->PG_ACT_B.reg &= (~(0b11 << actEvent));
        aptx->PG_ACT_B.reg |= (action << actEvent);
    }
}

/**
 * @brief Select the event source of PWM Generation event C1 or C2.
 *        This function is only used when C1 or C2 event is selected as PWM action event.
 * @param aptx APT register base address.
 * @param channel PWM output channel.
 * @param eventCx The PWM Generation event, should be C1 or C2.
 * @param eventCxSrc The trigger source of PWM Generation event C1 or C2.
 * @retval None.
 */
static inline void DCL_APT_SelectCxEventSource(APT_RegStruct *aptx,
                                               APT_PWMChannel channel,
                                               APT_PGEventCx eventCx,
                                               APT_PGEventCxSrc eventCxSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET((channel >= APT_PWM_CHANNEL_A) && (channel <= APT_PWM_CHANNEL_B));
    APT_PARAM_CHECK_NO_RET(eventCx >= APT_PWM_GENERATION_EVENT_C1);
    APT_PARAM_CHECK_NO_RET(eventCx <= APT_PWM_GENERATION_EVENT_C2);
    APT_PARAM_CHECK_NO_RET(eventCxSrc >= APT_PG_EVT_C_FORBIDDEN);
    APT_PARAM_CHECK_NO_RET(eventCxSrc <= APT_PG_EVT_C_SYNC_IN);
    unsigned int chOffset = 8; /* Bit field offset of PWM output channel */
    unsigned int cxOffset = 4; /* Bit field offset of event Cx */
    aptx->PG_EVTC_SEL.reg &= (~(0b1111 << (channel * chOffset + eventCx * cxOffset)));
    aptx->PG_EVTC_SEL.reg |= eventCxSrc << (channel * chOffset + eventCx * cxOffset);
}

/**
  * @brief Set the buffer load mode of PWM action register.
  * @param aptx APT register base address.
  * @param channel PWM output channel.
  * @param loadMode Buffer load mode.
  * @retval None.
  */
static inline void DCL_APT_SetPWMActionLoadMode(APT_RegStruct *aptx,
                                                APT_PWMChannel channel,
                                                APT_BufferLoadMode loadMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(channel >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(channel <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(loadMode >= APT_BUFFER_DISABLE);
    APT_PARAM_CHECK_NO_RET(loadMode <= APT_BUFFER_GLOBAL_LOAD);
    unsigned int bufFieldWidth = 2; /* Bit field width of buffer load mode setting */
    aptx->PG_BUF_EN.reg &= (~(0b11 << (channel * bufFieldWidth)));  /* Clear rg_actx_gld_en and rg_actx_buf_en */
    aptx->PG_BUF_EN.reg |= (loadMode << (channel * bufFieldWidth)); /* Write rg_actx_gld_en and rg_actx_buf_en */
}

/**
 * @brief Enable the buffer load events of PG_ACT_A or PG_ACT_B register
 * @param aptx APT register base address.
 * @param channel PWM output channel.
 * @param loadEvent The buffer load events of PG_ACT_A or PG_ACT_B register, it supports |.
 *        A logical OR of valid values can be passed as the loadEvent parameter
 *        Valid values for loadEvent are:
 *            APT_ACTION_LOAD_EVENT_ZERO      -   When counter value equal to zero
 *            APT_ACTION_LOAD_EVENT_PERIOD    -   When counter value equal to period
 *            APT_ACTION_LOAD_EVENT_A1        -   When combined event A1 is valid
 *            APT_ACTION_LOAD_EVENT_B1        -   When combined event B1 is valid
 *            APT_ACTION_LOAD_EVENT_SYNC      -   When synchronization event is valid
 * @retval None.
 */
static inline void DCL_APT_SetPWMActionLoadEvent(APT_RegStruct *aptx,
                                                 APT_PWMChannel channel,
                                                 APT_ActLoadEvent loadEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(channel >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(channel <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(loadEvent >= APT_ACTION_LOAD_EVENT_ZERO && loadEvent <= APT_ACTION_LOAD_EVENT_MAX_VALUE);
    unsigned int actBufField = 8; /* Field width of PWM action load event setting  */
    aptx->PG_ACT_LD.reg &= (~(0x1F << (channel * actBufField)));
    aptx->PG_ACT_LD.reg |= (loadEvent << (channel * actBufField));
}

/**
  * @brief Set the PWM waveform action on one-shot action software event.
  * @param aptx APT register base address.
  * @param channel PWM output channel.
  * @param action PWM waveform action.
  * @retval None.
  */
static inline void DCL_APT_SetSwOneShotPWMAction(APT_RegStruct *aptx, APT_PWMChannel channel, APT_PWMAction action)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(channel >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(channel <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(action >= APT_PWM_ACTION_HOLD);
    APT_PARAM_CHECK_NO_RET(action <= APT_PWM_ACTION_TOGGLE);
    if (channel == APT_PWM_CHANNEL_A) {
        aptx->PG_ACT_FRC.BIT.rg_pga_act_evt_frc = action;
    } else if (channel == APT_PWM_CHANNEL_B) {
        aptx->PG_ACT_FRC.BIT.rg_pgb_act_evt_frc = action;
    }
}

/**
  * @brief Force one-shot software event.
  * @param aptx APT register base address.
  * @param channel PWM output channel.
  * @retval None.
  */
static inline void DCL_APT_ForceSwOneShotPWMAction(APT_RegStruct *aptx, APT_PWMChannel channel)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(channel >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(channel <= APT_PWM_CHANNEL_B);
    if (channel == APT_PWM_CHANNEL_A) {
        aptx->PG_ACT_FRC.BIT.rg_pga_evt_frc = BASE_CFG_SET;
    } else if (channel == APT_PWM_CHANNEL_B) {
        aptx->PG_ACT_FRC.BIT.rg_pgb_evt_frc = BASE_CFG_SET;
    }
}

/**
  * @brief Set the PWM waveform action on continuous action software event.
  * @param aptx APT register base address.
  * @param channel PWM output channel.
  * @param action PWM waveform action
  * @retval None.
  */
static inline void DCL_APT_SetSwContPWMAction(APT_RegStruct *aptx, APT_PWMChannel channel, APT_PWMContAction action)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(channel >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(channel <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(action >= APT_PWM_CONTINUOUS_ACTION_HOLD);
    APT_PARAM_CHECK_NO_RET(action <= APT_PWM_CONTINUOUS_ACTION_HIGH);
    if (channel == APT_PWM_CHANNEL_A) {
        aptx->PG_OUT_FRC.BIT.rg_pga_frc_act = action;
    } else if (channel == APT_PWM_CHANNEL_B) {
        aptx->PG_OUT_FRC.BIT.rg_pgb_frc_act = action;
    }
}

/**
  * @brief Force channel A pwm to output low.
  * @param aptx APT register base address.
  * @retval None.
  */
static void APT_ForcePWMAOutputLow(APT_RegStruct *aptx)
{
    unsigned int risingOutSelect = aptx->DG_CFG.BIT.rg_dg_red_osel;
    unsigned int fallingOutSelect = aptx->DG_CFG.BIT.rg_dg_fed_osel;
    unsigned int risingInSelect = aptx->DG_CFG.BIT.rg_dg_red_isel;
    unsigned int fallingInSelect = aptx->DG_CFG.BIT.rg_dg_fed_isel;
    /* Enable force output. */
    aptx->PG_OUT_FRC.BIT.rg_pga_frc_en = BASE_CFG_ENABLE;
    /* if PWMA invert */
    if (((risingOutSelect == APT_DB_RED_OUTPUT_INVERT) && (risingInSelect == APT_DB_RED_INPUT_PWM_A)) || \
         ((fallingOutSelect == APT_DB_FED_OUTPUT_INVERT) && (fallingInSelect == APT_DB_FED_INPUT_PWM_A))) {
        aptx->PG_OUT_FRC.BIT.rg_pga_frc_act = APT_PWM_CONTINUOUS_ACTION_HIGH; /* if invert, set high */
    } else { /* if PWMA not invert */
        aptx->PG_OUT_FRC.BIT.rg_pga_frc_act = APT_PWM_CONTINUOUS_ACTION_LOW; /* if not invert, set low */
    }
    return;
}

/**
  * @brief Force channel B pwm to output low.
  * @param aptx APT register base address.
  * @retval None.
  */
static void APT_ForcePWMBOutputLow(APT_RegStruct *aptx)
{
    unsigned int risingOutSelect = aptx->DG_CFG.BIT.rg_dg_red_osel;
    unsigned int fallingOutSelect = aptx->DG_CFG.BIT.rg_dg_fed_osel;
    unsigned int risingInSelect = aptx->DG_CFG.BIT.rg_dg_red_isel;
    unsigned int fallingInSelect = aptx->DG_CFG.BIT.rg_dg_fed_isel;
    /* Enable force output */
    aptx->PG_OUT_FRC.BIT.rg_pgb_frc_en = BASE_CFG_ENABLE;
    /* if PWMB invert */
    if (((risingOutSelect == APT_DB_RED_OUTPUT_INVERT) && (risingInSelect == APT_DB_RED_INPUT_PWM_B)) || \
         ((fallingOutSelect == APT_DB_FED_OUTPUT_INVERT) && (fallingInSelect == APT_DB_FED_INPUT_PWM_B))) {
        aptx->PG_OUT_FRC.BIT.rg_pgb_frc_act = APT_PWM_CONTINUOUS_ACTION_HIGH; /* if invert, set high */
    } else { /* if PWMB not invert */
        aptx->PG_OUT_FRC.BIT.rg_pgb_frc_act = APT_PWM_CONTINUOUS_ACTION_LOW; /* if not invert, set low */
    }
    return;
}

/**
  * @brief Both PWMA and PWMB output low level.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_ForcePWMOutputLow(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    
    APT_ForcePWMAOutputLow(aptx);
    APT_ForcePWMBOutputLow(aptx);

    return;
}

/**
  * @brief Set the buffer load mode of continuous aciton software event register.
  * @param aptx APT register base address.
  * @param loadMode Buffer load mode.
  * @retval None.
  */
static inline void DCL_APT_SetSwContActionLoadMode(APT_RegStruct *aptx, APT_BufferLoadMode loadMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(loadMode >= APT_BUFFER_DISABLE);
    APT_PARAM_CHECK_NO_RET(loadMode <= APT_BUFFER_GLOBAL_LOAD);
    unsigned int bufFieldWidth = 4; /* Bit field width of buffer load mode setting */
    aptx->PG_BUF_EN.reg &= (~(0b11 << bufFieldWidth));  /* Clear rg_frc_gld_en and rg_frc_buf_en */
    aptx->PG_BUF_EN.reg |= (loadMode << bufFieldWidth); /* Write rg_frc_gld_en and rg_frc_buf_en */
}

/**
 * @brief Enable the independent buffer load events of PG_OUT_FRC register.
 * @param aptx APT register base address.
 * @param channel PWM output channel
 * @param loadEvent The buffer load events of PG_OUT_FRC register
 *        A logical OR of valid values can be passed as the loadEvent parameter
 *        Valid values for loadEvent are:
 *            APT_ACTION_FORCE_LOAD_EVENT_ZERO      -   When counter value equal to zero
 *            APT_ACTION_FORCE_LOAD_EVENT_PERIOD    -   When counter value equal to period
 *            APT_ACTION_FORCE_LOAD_EVENT_SYNC      -   When synchronization event is valid
 * @retval None.
 */
static inline void DCL_APT_SetSwContActionLoadEvent(APT_RegStruct *aptx, APT_FrcActLoadEvent loadEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(loadEvent == APT_ACTION_FORCE_LOAD_EVENT_ZERO || \
                           loadEvent == APT_ACTION_FORCE_LOAD_EVENT_PERIOD || \
                           loadEvent == APT_ACTION_FORCE_LOAD_EVENT_SYNC);
    /* Clear first, set value then */
    aptx->PG_ACT_LD.reg &= (~loadEvent);
    aptx->PG_ACT_LD.reg |= loadEvent;
}

/**
  * @brief Enable continuous action software event.
  * @param aptx APT register base address.
  * @param channel PWM output channel.
  * @retval None.
  */
static inline void DCL_APT_EnableSwContPWMAction(APT_RegStruct *aptx, APT_PWMChannel channel)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(channel >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(channel <= APT_PWM_CHANNEL_B);
    if (channel == APT_PWM_CHANNEL_A) {
        aptx->PG_OUT_FRC.BIT.rg_pga_frc_en = BASE_CFG_SET;
    } else if (channel == APT_PWM_CHANNEL_B) {
        aptx->PG_OUT_FRC.BIT.rg_pgb_frc_en = BASE_CFG_SET;
    }
}

/**
  * @brief Disable continuous action software event.
  * @param aptx APT register base address.
  * @param channel PWM output channel.
  * @retval None.
  */
static inline void DCL_APT_DisableSwContPWMAction(APT_RegStruct *aptx, APT_PWMChannel channel)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(channel >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(channel <= APT_PWM_CHANNEL_B);
    if (channel == APT_PWM_CHANNEL_A) {
        aptx->PG_OUT_FRC.BIT.rg_pga_frc_en = BASE_CFG_UNSET;
    } else if (channel == APT_PWM_CHANNEL_B) {
        aptx->PG_OUT_FRC.BIT.rg_pgb_frc_en = BASE_CFG_UNSET;
    }
}

/* --------------------------------------------------------------------------------------------- */
/* Dead-Band Generation (DG) submodule Direct Configuration Layer functions -------------------- */
/* --------------------------------------------------------------------------------------------- */
/**
 * @brief Configure the rising edge delay (RED) count.
 * @param aptx APT register base address.
 * @param countLr Count value of low resolution.
 * @param countHr Count value of High resolution.
 * @retval None.
 */
static inline void DCL_APT_SetDgRedCount(APT_RegStruct *aptx, unsigned short countLr, unsigned char countHr)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->DG_RED.BIT.rg_dg_red = countLr;
    aptx->DG_RED.BIT.rg_dg_red_hr = countHr;
}

/**
 * @brief Configure the falling edge delay (RED) count.
 * @param aptx APT register base address.
 * @param countLr Count value of low resolution.
 * @param countHr Count value of High resolution.
 * @retval None.
 */
static inline void DCL_APT_SetDgFedCount(APT_RegStruct *aptx, unsigned short countLr, unsigned char countHr)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->DG_FED.BIT.rg_dg_fed = countLr;
    aptx->DG_FED.BIT.rg_dg_fed_hr = countHr;
}

/**
 * @brief Configure the rising edge delay (RED) of Dead-Band Generation.
 * @param aptx APT register base address.
 * @param redInput The input source of RED counter.
 * @param redOutMode The output of RED counter.
 * @param dgaOutSwap The swap mode of Dead-Band Generation output signal A.
 *                       true    -   Select the output of FED counter.
 *                       false   -   Select the output of RED counter.
 * @param redCount The count value of RED counter, in units of APT clock.
 * @retval None.
 */
static inline void DCL_APT_SetDeadBandRisingEdge(APT_RegStruct *aptx,
                                                 APT_REDInput redInput,
                                                 APT_REDOutMode redOutMode,
                                                 bool dgaOutSwap,
                                                 unsigned short redCount)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(redInput >= APT_DB_RED_INPUT_PWM_A);
    APT_PARAM_CHECK_NO_RET(redInput <= APT_DB_RED_INPUT_PWM_B);
    APT_PARAM_CHECK_NO_RET(redOutMode >= APT_DB_RED_OUTPUT_NOT_INVERT);
    APT_PARAM_CHECK_NO_RET(redOutMode <= APT_DB_RED_OUTPUT_PWM_A);
    aptx->DG_CFG.BIT.rg_dg_red_isel = redInput;
    aptx->DG_CFG.BIT.rg_dg_red_osel = redOutMode;
    aptx->DG_CFG.BIT.rg_dga_osel = dgaOutSwap;
    aptx->DG_RED.BIT.rg_dg_red = redCount;
}

/**
 * @brief Configure the hr rising edge delay (RED) of Dead-Band Generation.
 * @param aptx APT register base address.
 * @param redInput The input source of RED counter.
 * @param redOutMode The output of RED counter.
 * @param dgaOutSwap The swap mode of Dead-Band Generation output signal A.
 *                       true    -   Select the output of FED counter.
 *                       false   -   Select the output of RED counter.
 * @param redCountHr The hr count value of RED counter, in units of APT clock.
 * @retval None.
 */
static inline void DCL_APT_SetDeadBandRisingEdgeHr(APT_RegStruct *aptx,
                                                   APT_REDInput redInput,
                                                   APT_REDOutMode redOutMode,
                                                   bool dgaOutSwap,
                                                   unsigned char redCountHr)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(redInput >= APT_DB_RED_INPUT_PWM_A);
    APT_PARAM_CHECK_NO_RET(redInput <= APT_DB_RED_INPUT_PWM_B);
    APT_PARAM_CHECK_NO_RET(redOutMode >= APT_DB_RED_OUTPUT_NOT_INVERT);
    APT_PARAM_CHECK_NO_RET(redOutMode <= APT_DB_RED_OUTPUT_PWM_A);
    /* Set dead band attributes of rising edge basic. */
    aptx->DG_CFG.BIT.rg_dg_red_isel = redInput;
    aptx->DG_CFG.BIT.rg_dg_red_osel = redOutMode;
    aptx->DG_CFG.BIT.rg_dga_osel = dgaOutSwap;
    /* Set hr count of rising edge. */
    aptx->DG_RED.BIT.rg_dg_red_hr = redCountHr;
}

/**
 * @brief Configure the falling edge delay (FED) of Dead-Band Generation.
 * @param aptx APT register base address.
 * @param fedInput The input source of FED counter.
 * @param fedOutMode The output of FED counter.
 * @param dgbOutSwap The swap mode of Dead-Band Generation output signal B.
 *                       true    -   Select the output of RED counter.
 *                       false   -   Select the output of FED counter.
 * @param fedCount The count value of FED counter, in units of APT clock.
 * @retval None.
 */
static inline void DCL_APT_SetDeadBandFallingEdge(APT_RegStruct *aptx,
                                                  APT_FEDInput fedInput,
                                                  APT_FEDOutMode fedOutMode,
                                                  bool dgbOutSwap,
                                                  unsigned short fedCount)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(fedInput >= APT_DB_FED_INPUT_PWM_B);
    APT_PARAM_CHECK_NO_RET(fedInput <= APT_DB_FED_INPUT_ZERO);
    APT_PARAM_CHECK_NO_RET(fedOutMode >= APT_DB_FED_OUTPUT_NOT_INVERT);
    APT_PARAM_CHECK_NO_RET(fedOutMode <= APT_DB_FED_OUTPUT_PWM_B);
    aptx->DG_CFG.BIT.rg_dg_fed_isel = fedInput;
    aptx->DG_CFG.BIT.rg_dg_fed_osel = fedOutMode;
    aptx->DG_CFG.BIT.rg_dgb_osel = dgbOutSwap;
    aptx->DG_FED.BIT.rg_dg_fed = fedCount;
}

/**
 * @brief Configure the hr falling edge delay (FED) of Dead-Band Generation.
 * @param aptx APT register base address.
 * @param fedInput The input source of FED counter.
 * @param fedOutMode The output of FED counter.
 * @param dgbOutSwap The swap mode of Dead-Band Generation output signal B.
 *                       true    -   Select the output of RED counter.
 *                       false   -   Select the output of FED counter.
 * @param fedCountHr The hr count value of FED counter, in units of APT clock.
 * @retval None.
 */
static inline void DCL_APT_SetDeadBandFallingEdgeHr(APT_RegStruct *aptx,
                                                    APT_FEDInput fedInput,
                                                    APT_FEDOutMode fedOutMode,
                                                    bool dgbOutSwap,
                                                    unsigned short fedCountHr)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(fedInput >= APT_DB_FED_INPUT_PWM_B);
    APT_PARAM_CHECK_NO_RET(fedInput <= APT_DB_FED_INPUT_ZERO);
    APT_PARAM_CHECK_NO_RET(fedOutMode >= APT_DB_FED_OUTPUT_NOT_INVERT);
    APT_PARAM_CHECK_NO_RET(fedOutMode <= APT_DB_FED_OUTPUT_PWM_B);
    /* Set dead band attributes of falling edge basic. */
    aptx->DG_CFG.BIT.rg_dg_fed_isel = fedInput;
    aptx->DG_CFG.BIT.rg_dg_fed_osel = fedOutMode;
    aptx->DG_CFG.BIT.rg_dgb_osel = dgbOutSwap;
    /* Set hr count of rising edge. */
    aptx->DG_FED.BIT.rg_dg_fed_hr = fedCountHr;
}

/**
  * @brief Set buffer load mode of Dead-Band configuration register.
  * @param aptx APT register base address.
  * @param dgCfgLoadMode Buffer load mode.
  * @retval None.
  */
static inline void DCL_APT_SetDGConfigLoadMode(APT_RegStruct *aptx, APT_BufferLoadMode dgCfgLoadMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(dgCfgLoadMode >= APT_BUFFER_DISABLE);
    APT_PARAM_CHECK_NO_RET(dgCfgLoadMode <= APT_BUFFER_GLOBAL_LOAD);
    unsigned int bufFieldWidth = 4; /* Bit field width of buffer load mode setting */
    aptx->DG_BUF_EN.reg &= (~(0b11 << bufFieldWidth)); /* Clear rg_cfg_gld_en and rg_cfg_buf_en */
    aptx->DG_BUF_EN.reg |= (dgCfgLoadMode << bufFieldWidth); /* Write rg_cfg_gld_en and rg_cfg_buf_en */
}

/**
 * @brief Enable the buffer load events of DG_CFG register.
 * @param aptx APT register base address.
 * @param loadEvent The buffer load events of DG_CFG register.
 *        A logical OR of valid values can be passed as the loadEvent parameter.
 *        Valid values for loadEvent are:
 *            APT_DEAD_BAND_LOAD_EVENT_ZERO   -   When time base counter value equal to zero.
 *            APT_DEAD_BAND_LOAD_EVENT_PERIOD -   When time base counter value equal to period.
 * @retval None.
 */
static inline void DCL_APT_SetDGConfigLoadEvent(APT_RegStruct *aptx, APT_DgLoadEvent dgCfgLoadEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(dgCfgLoadEvent >= APT_DEAD_BAND_LOAD_EVENT_ZERO);
    APT_PARAM_CHECK_NO_RET(dgCfgLoadEvent <= APT_DEAD_BAND_LOAD_EVENT_PERIOD);
    unsigned int dgBufField = 16; /* Field width of continuous PWM action load event setting  */
    aptx->DG_BUF_LOAD.reg &= (~(0b11 << dgBufField));
    aptx->DG_BUF_LOAD.reg |= (dgCfgLoadEvent << dgBufField);
}

/**
  * @brief Set buffer load mode of Dead-Band rising edge delay counter register.
  * @param aptx APT register base address.
  * @param redCntLoadMode Buffer load mode.
  * @retval None.
  */
static inline void DCL_APT_SetREDCounterLoadMode(APT_RegStruct *aptx, APT_BufferLoadMode redCntLoadMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(redCntLoadMode >= APT_BUFFER_DISABLE);
    APT_PARAM_CHECK_NO_RET(redCntLoadMode <= APT_BUFFER_GLOBAL_LOAD);
    aptx->DG_BUF_EN.reg &= (~(0b11 << 0)); /* Clear rg_red_gld_en and rg_red_buf_en */
    aptx->DG_BUF_EN.reg |= (redCntLoadMode << 0); /* Write rg_red_gld_en and rg_red_buf_en */
}

/**
 * @brief Enable the buffer load events of DG_RED register
 * @param aptx APT register base address.
 * @param redCntLoadEvent The buffer load events of DG_RED register.
 *        A logical OR of valid values can be passed as the loadEvent parameter.
 *        Valid values for loadEvent are:
 *            APT_DEAD_BAND_LOAD_EVENT_ZERO   -   When time base counter value equal to zero.
 *            APT_DEAD_BAND_LOAD_EVENT_PERIOD -   When time base counter value equal to period.
 * @retval None.
 */
static inline void DCL_APT_SetREDCounterLoadEvent(APT_RegStruct *aptx, APT_DgLoadEvent redCntLoadEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(redCntLoadEvent >= APT_DEAD_BAND_LOAD_EVENT_ZERO);
    APT_PARAM_CHECK_NO_RET(redCntLoadEvent <= APT_DEAD_BAND_LOAD_EVENT_PERIOD);
    /* Clear first, set value then */
    aptx->DG_BUF_LOAD.reg &= (~(0b11 << 0));
    aptx->DG_BUF_LOAD.reg |= (redCntLoadEvent << 0);
}

/**
  * @brief Set buffer load mode of Dead-Band falling edge delay counter register.
  * @param aptx APT register base address.
  * @param fedCntLoadMode Buffer load mode.
  * @retval None.
  */
static inline void DCL_APT_SetFEDCounterLoadMode(APT_RegStruct *aptx, APT_BufferLoadMode fedCntLoadMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(fedCntLoadMode >= APT_BUFFER_DISABLE);
    APT_PARAM_CHECK_NO_RET(fedCntLoadMode <= APT_BUFFER_GLOBAL_LOAD);
    unsigned int bufFieldWidth = 2; /* Bit field width of buffer load mode setting */
    aptx->DG_BUF_EN.reg &= (~(0b11 << bufFieldWidth));  /* Clear rg_fed_gld_en and rg_fed_buf_en */
    aptx->DG_BUF_EN.reg |= (fedCntLoadMode << bufFieldWidth); /* Write rg_fed_gld_en and rg_fed_buf_en */
}

/**
 * @brief Enable the buffer load events of DG_FED register.
 * @param aptx APT register base address.
 * @param fedCntLoadEvent The buffer load events of DG_FED register.
 *        A logical OR of valid values can be passed as the loadEvent parameter.
 *        Valid values for loadEvent are:
 *            APT_DEAD_BAND_LOAD_EVENT_ZERO   -   When time base counter value equal to zero.
 *            APT_DEAD_BAND_LOAD_EVENT_PERIOD -   When time base counter value equal to period.
 * @retval None.
 */
static inline void DCL_APT_SetFEDCounterLoadEvent(APT_RegStruct *aptx, APT_DgLoadEvent fedCntLoadEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(fedCntLoadEvent >= APT_DEAD_BAND_LOAD_EVENT_ZERO);
    APT_PARAM_CHECK_NO_RET(fedCntLoadEvent <= APT_DEAD_BAND_LOAD_EVENT_PERIOD);
    unsigned int dgBufField = 8; /* Field width of continuous PWM action load event setting */
    aptx->DG_BUF_LOAD.reg &= (~(0b11 << dgBufField));
    aptx->DG_BUF_LOAD.reg |= (fedCntLoadEvent << dgBufField);
}

/* --------------------------------------------------------------------------------------------- */
/* PWM Chopper (PC) submodule Direct Configuration Layer functions -------------------------- */
/* --------------------------------------------------------------------------------------------- */
/**
 * @brief Enable the pwm chopper function.
 * @param aptx APT register base address.
 * @param ch Channel A or Channel B.
 * @retval None.
 */
static inline void DCL_APT_EnablePwmChopper(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Enable channel A */
        aptx->PC_CFG.BIT.rg_pc_en_a = BASE_CFG_ENABLE;
    } else {
        /* Enable channel B */
        aptx->PC_CFG.BIT.rg_pc_en_b = BASE_CFG_ENABLE;
    }
}

/**
 * @brief Disable the pwm chopper function.
 * @param aptx APT register base address.
 * @param ch Channel A or Channel B.
 * @retval None.
 */
static inline void DCL_APT_DisablePwmChopper(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Disable channel A */
        aptx->PC_CFG.BIT.rg_pc_en_a = BASE_CFG_DISABLE;
    } else {
        /* Disable channel B */
        aptx->PC_CFG.BIT.rg_pc_en_b = BASE_CFG_DISABLE;
    }
}

/**
 * @brief Set chopper frequency division.
 * @param aptx APT register base address.
 * @param div Frequency division, including 8/16/24/.../256, 5bit: 0x0 ~ 0x1F.
 * @retval None.
 */
static inline void DCL_APT_SetChopperFreqDiv(APT_RegStruct *aptx, unsigned char div)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(div >= 0x0 && div <= APT_CHOPPER_PSCLK_DIV_MAX_APTCLK);
    aptx->PC_FREQ.BIT.rg_pc_freq = div;
}

/**
 * @brief Get chopper frequency division.
 * @param aptx APT register base address.
 * @retval Frequency division, including 8/16/24/.../256, 5bit: 0x0 ~ 0x1F.
 */
static inline unsigned char DCL_APT_GetChopperFreqDiv(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->PC_FREQ.BIT.rg_pc_freq;
}

/**
 * @brief Set chopper shape.
 * @param aptx APT register base address.
 * @param shape Chopper shape, 8bit: 0b00000000 ~ 0b11111111.
 * @retval None.
 */
static inline void DCL_APT_SetChopperShape(APT_RegStruct *aptx, unsigned char shape)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(shape >= 0 && shape <= APT_CHOPPER_SHAPE_CONFIG_MAX);
    aptx->PC_SHAP.BIT.rg_pc_shape = shape;
}

/**
 * @brief Get chopper shape.
 * @param aptx APT register base address.
 * @retval Chopper shape, 8bit: 0b00000000 ~ 0b11111111.
 */
static inline unsigned char DCL_APT_GetChopperShape(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->PC_SHAP.BIT.rg_pc_shape;
}

/**
 * @brief Set chopper initial pulse width.
 * @param aptx APT register base address.
 * @param initWidth Chopper initial width, 4bit(0x0 ~ 0xF) indicates 1 ~ 16 PSLCK.
 * @retval None.
 */
static inline void DCL_APT_SetChopperInitWidth(APT_RegStruct *aptx, unsigned char initWidth)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_ASSERT_PARAM(initWidth >= 0x0 && initWidth <= 0xF); /* 0xF: max value */

    aptx->PC_INIT_WIDTH.BIT.rg_pc_init_width = initWidth;
}

/**
 * @brief Get chopper initial pulse width.
 * @param aptx APT register base address.
 * @retval Chopper initial width, 4bit(0x0 ~ 0xF) indicates 1 ~ 16 PSLCK..
 */
static inline unsigned char DCL_APT_GetChopperInitWidth(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->PC_INIT_WIDTH.BIT.rg_pc_init_width;
}

/* --------------------------------------------------------------------------------------------- */
/* PWM Burst (PB) submodule Direct Configuration Layer functions -------------------------- */
/* --------------------------------------------------------------------------------------------- */

/**
 * @brief Enable the pwm busrt function.
 * @param aptx APT register base address.
 * @param ch Channel A or channel B.
 * @retval None.
 */
static inline void DCL_APT_EnablePwmBurst(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Enable channel A */
        aptx->PB_CFG_A.BIT.rg_pb_en_a = BASE_CFG_ENABLE;
    } else {
        /* Enable channel A */
        aptx->PB_CFG_B.BIT.rg_pb_en_b = BASE_CFG_ENABLE;
    }
}

/**
 * @brief Disable the pwm busrt function.
 * @param aptx APT register base address.
 * @param ch Channel A or channel B.
 * @retval None.
 */
static inline void DCL_APT_DisablePwmBurst(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Disable channel A */
        aptx->PB_CFG_A.BIT.rg_pb_en_a = BASE_CFG_DISABLE;
    } else {
        /* Disable channel B */
        aptx->PB_CFG_B.BIT.rg_pb_en_b = BASE_CFG_DISABLE;
    }
}

/**
 * @brief Set pwm busrt mode of channel A or B.
 * @param aptx APT register base address.
 * @param ch Channel A or channel B.
 * @param brstMode Open pwmwave first or close pwmwave first.
 * @retval None.
 */
static inline void DCL_APT_SetBurstMode(APT_RegStruct *aptx, APT_PWMChannel ch, APT_BurstMode brstMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(brstMode >= APT_BURST_MODE_OFF_FIRST);
    APT_PARAM_CHECK_NO_RET(brstMode <= APT_BURST_MODE_PWM_FIRST);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Burst mode of channel A */
        aptx->PB_CFG_A.BIT.rg_pb_op_mode_a = brstMode;
    } else {
        /* Burst mode of channel B */
        aptx->PB_CFG_B.BIT.rg_pb_op_mode_b = brstMode;
    }
}

/**
 * @brief Set pwm busrt m and n of channel A or B.
 * @param aptx APT register base address.
 * @param ch Channel A or channel B.
 * @param m Burst period, 8bit: 0x00 ~ 0xFF.
 * @param n Close pwm period, 8bit: 0x00 ~ 0xFF.
 * @retval None.
 */
static inline void DCL_APT_SetBurstMN(APT_RegStruct *aptx, APT_PWMChannel ch, unsigned char m, unsigned char n)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(m >= n);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Set m and n of channel A. */
        aptx->PB_CFG_A.BIT.rg_pb_m_a = m;
        aptx->PB_CFG_A.BIT.rg_pb_n_a = n;
    } else {
        /* Set m and n of channel B. */
        aptx->PB_CFG_B.BIT.rg_pb_m_b = m;
        aptx->PB_CFG_B.BIT.rg_pb_n_b = n;
    }
}

/**
 * @brief Set the polarity of the closed pwm level.
 * @param aptx APT register base address.
 * @param ch Channel A or channel B.
 * @param pol Polarity of the closed pwm: high level or low level.
 * @retval None.
 */
static inline void DCL_APT_SetBurstClosedPol(APT_RegStruct *aptx, APT_PWMChannel ch, APT_BurstClosedPol pol)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(pol >= APT_BURST_CLOSED_PWM_POL_LOW);
    APT_PARAM_CHECK_NO_RET(pol <= APT_BURST_CLOSED_PWM_POL_HIGH);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Set burst closed pwm polarity of channel A. */
        aptx->PB_CFG_A.BIT.rg_pb_n_pol_a = pol;
    } else {
        /* Set burst closed pwm polarity of channel B. */
        aptx->PB_CFG_B.BIT.rg_pb_n_pol_b = pol;
    }
}

/**
 * @brief Set pwm busrt load mode of PB_CFG_A or PB_CFG_B.
 * @param aptx APT register base address.
 * @param ch Channel A or channel B.
 * @param brstLoadMode Buffer load mode of PB_CFG_A or PB_CFG_B register.
 * @retval None.
 */
static inline void DCL_APT_SetBurstLoadMode(APT_RegStruct *aptx, APT_PWMChannel ch, APT_BufferLoadMode brstLoadMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(brstLoadMode >= APT_BUFFER_DISABLE && brstLoadMode <= APT_BUFFER_GLOBAL_LOAD);
    if (ch == APT_PWM_CHANNEL_A) {
        /* 0b11 is used to clear load mode which occupied two bits. */
        aptx->PB_BUF_EN.reg &= (~0b11);
        aptx->PB_BUF_EN.reg |= brstLoadMode;
    } else {
        /* Clear buffer load mode of channel b and set load mode. */
        aptx->PB_BUF_EN.reg &= (~(0b11 << APT_BURST_LOAD_SHIFT_8BIT));
        aptx->PB_BUF_EN.reg |= (brstLoadMode << APT_BURST_LOAD_SHIFT_8BIT);
    }
}

/**
 * @brief Set pwm busrt load event of PB_CFG_A or PB_CFG_B.
 * @param aptx APT register base address.
 * @param ch Channel A or channel B.
 * @param brstLoadEvent Buffer load event of PB_CFG_A or PB_CFG_B register.
 * @retval None.
 */
static inline void DCL_APT_SetBurstLoadEvent(APT_RegStruct *aptx, APT_PWMChannel ch, APT_BurstLoadEvent loadEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(loadEvent >= APT_BURST_LOAD_EVENT_DISABLE && loadEvent <= APT_BURST_LOAD_EVENT_PERIOD);

    if (ch == APT_PWM_CHANNEL_A) {
        /* 0b11 is used to clear load event which occupied two bits. */
        aptx->PB_BUF_LOAD.reg &= (~0b11);
        aptx->PB_BUF_LOAD.reg |= loadEvent;
    } else {
        /* Clear buffer load event of channel b and set load event. */
        aptx->PB_BUF_LOAD.reg &= (~(0b11 << APT_BURST_LOAD_SHIFT_8BIT));
        aptx->PB_BUF_LOAD.reg |= (loadEvent << APT_BURST_LOAD_SHIFT_8BIT);
    }
}

/**
  * @brief Get current pwm burst output status.
  * @param aptx The base address of APT module.
  * @param ch Channel select, include channel A or B.
  * @retval APT_GetBurstOutputStatus: APT_BURST_OUTPUT_PWM or APT_BURST_OUTPUT_LOW.
  */
static inline APT_BurstOutputStatus DCL_APT_GetBurstPwmStatus(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B, APT_BURST_OUTPUT_ERROR);
    /* Return value */
    return ch == APT_PWM_CHANNEL_A
           ? ((APT_BurstOutputStatus)aptx->PB_STATUS.BIT.ro_pb_state_a)
           : ((APT_BurstOutputStatus)aptx->PB_STATUS.BIT.ro_pb_state_b);
}

/**
  * @brief Get current pwm cycles when burst is enable.
  * @param aptx The base address of APT module.
  * @param Channel select, include channel A or B.
  * @retval unsigned char: pwm cycles.
  */
static inline unsigned char DCL_APT_GetBurstPwmCycles(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B, false);
    return (ch == APT_PWM_CHANNEL_A)? aptx->PB_STATUS.BIT.ro_pb_m_n_a : aptx->PB_STATUS.BIT.ro_pb_m_n_b;
}

/* --------------------------------------------------------------------------------------------- */
/* Output Control (OC) submodule Direct Configuration Layer functions -------------------------- */
/* --------------------------------------------------------------------------------------------- */

/**
  * @brief Enable an output control event of oc mode.
  * @param aptx APT register base address.
  * @param ocMode Output control mode, include osht, cbc, free.
  * @param ocEvent Output control event.
  * @retval None.
  */
static inline void DCL_APT_EnableOutCtrlEvent(APT_RegStruct *aptx, APT_OutCtrlMode ocMode, APT_OutCtrlEvent ocEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ocMode >= APT_OUT_CTRL_CYCLE_BY_CYCLE);
    APT_PARAM_CHECK_NO_RET(ocMode <= APT_OUT_CTRL_FREE);
    APT_PARAM_CHECK_NO_RET(ocEvent >= APT_OC_NO_EVENT);
    APT_PARAM_CHECK_NO_RET(ocEvent <= APT_OC_COMBINE_EVENT_B2);
    /* Unlock */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    switch (ocMode) {
        case APT_OUT_CTRL_CYCLE_BY_CYCLE:
            /* Cycle by cycle mode. */
            aptx->OC_CBC_CFG.reg |= ocEvent;
            break;

        case APT_OUT_CTRL_ONE_SHOT:
            /* Oneshot mode. */
            aptx->OC_OSHT_CFG.reg |= ocEvent;
            break;

        case APT_OUT_CTRL_FREE:
            /* Free mode. */
            aptx->OC_FREE_CFG.reg |= ocEvent;
            break;

        default:
            break;
    }
    /* Lock */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable an output control event of oc mode.
  * @param aptx APT register base address.
  * @param ocMode Output control mode, include osht, cbc, free.
  * @param ocEvent Output control event, it supports |.
  * @retval None.
  */
static inline void DCL_APT_DisableOutCtrlEvent(APT_RegStruct *aptx, APT_OutCtrlMode ocMode, APT_OutCtrlEvent ocEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ocMode >= APT_OUT_CTRL_CYCLE_BY_CYCLE);
    APT_PARAM_CHECK_NO_RET(ocMode <= APT_OUT_CTRL_FREE);
    APT_PARAM_CHECK_NO_RET(ocEvent >= APT_OC_NO_EVENT && ocEvent <= APT_OC_COMBINE_EVENT_MAX_VALUE);
    /* Unlock register */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    switch (ocMode) {
        case APT_OUT_CTRL_CYCLE_BY_CYCLE:
            /* Cycle by cycle mode. */
            aptx->OC_CBC_CFG.reg &= ~ocEvent;
            break;

        case APT_OUT_CTRL_ONE_SHOT:
            /* Oneshot mode. */
            aptx->OC_OSHT_CFG.reg &= ~ocEvent;
            break;

        case APT_OUT_CTRL_FREE:
            /* Free mode. */
            aptx->OC_FREE_CFG.reg &= ~ocEvent;
            break;

        default:
            break;
    }
    /* Lock */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set output control action of an output control event.
  * @param aptx APT register base address.
  * @param channel PWM output channel.
  * @param ocEvtDir Output control event that takes into consideration of counter direction.
  * @param ocAction Output control action.
  * @retval None.
  */
static inline void DCL_APT_SetOutCtrlAction(APT_RegStruct *aptx,
                                            APT_PWMChannel channel,
                                            APT_OutCtrlEventDir ocEvtDir,
                                            APT_OutCtrlAction ocAction)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(channel >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(channel <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(ocEvtDir >= APT_OC_EVT_OSHT_OR_CBC_OR_FREE_INC);
    APT_PARAM_CHECK_NO_RET(ocEvtDir <= APT_OC_EVT_COMBINE_EVENT_B2_DEC);
    APT_PARAM_CHECK_NO_RET(ocAction >= APT_OUT_CTRL_ACTION_DISABLE);
    APT_PARAM_CHECK_NO_RET(ocAction <= APT_OUT_CTRL_ACTION_HIGH_Z);
    /* Unlock */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (channel == APT_PWM_CHANNEL_A) {
        aptx->OC_ACT_A.reg &= (~(0b111 << ocEvtDir));
        aptx->OC_ACT_A.reg |= (ocAction << ocEvtDir);
    } else if (channel == APT_PWM_CHANNEL_B) {
        aptx->OC_ACT_B.reg &= (~(0b111 << ocEvtDir));
        aptx->OC_ACT_B.reg |= (ocAction << ocEvtDir);
    }
    /* Lock */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get the fault flag of an output control event.
  * @param aptx APT register base address.
  * @param faultEvt Output contorl fault event, it supports |.
  * @retval bool: true, false.
  */
static inline bool DCL_APT_GetOutCtrlEventFlag(APT_RegStruct *aptx, APT_EventInterruptSrc faultEvt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(faultEvt >= APT_OC_FAULT_EVENT_CBC && faultEvt <= APT_OC_FAULT_EVENT_MAX_VALUE, false);
    return ((aptx->OC_EVT_FLAG.reg & faultEvt) == faultEvt);
}

/**
  * @brief Clear the fault flag of an output control event.
  * @param aptx APT register base address.
  * @param faultEvt Output control fault event, it can use |.
  * @retval None.
  */
static inline void DCL_APT_ClearOutCtrlEventFlag(APT_RegStruct *aptx, APT_EventInterruptSrc faultEvt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(faultEvt >= APT_OC_FAULT_EVENT_CBC && faultEvt <= APT_OC_FAULT_EVENT_MAX_VALUE);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    unsigned int ocFlgOffset = 16; /* Offset of output control flag clear */
    aptx->OC_EVT_FLAG.reg |= (faultEvt << ocFlgOffset);
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Hardware: clear the CBC protection action of PWMA and PWMB at evevt zero or period or zero_period.
  * @param aptx APT register base address.
  * @param cbcClrEvevt clear cbc protection event, it supports |.
  * @retval None.
  */
static inline void DCL_APT_HwClearCbcProtEvent(APT_RegStruct *aptx, APT_OutCtrlCbcClrMode cbcClrEvevt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(cbcClrEvevt >= APT_OC_CLEAR_CBC_PROTECT_EVENT_ZERO);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->OC_PRD_CLR.reg &= (~cbcClrEvevt);
    aptx->OC_PRD_CLR.reg |= cbcClrEvevt;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Software: clear the protection action of PWMA and PWMB when write these bits.
  * @param aptx APT register base address.
  * @param ocMode Output control mode(cbc, osht, free), it supports |.
  *               (1) APT_SW_CLEAR_PROT_ACTION_CBC: This bit is automatically cleared when 1 is written.
  *               (2) APT_SW_CLEAR_PROT_ACTION_OSHT: This bit is automatically cleared when 1 is written.
  *               (3) APT_SW_CLEAR_PROT_ACTION_FREE: Free protection invalid when 1 is written.
  *                                      It can write 0 to this bit to restore free protection.
  * @retval None.
  */
static inline void DCL_APT_SwClearProtAction(APT_RegStruct *aptx, APT_ProtMode protMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(protMode >= APT_SW_CLEAR_PROT_ACTION_CBC);
    APT_PARAM_CHECK_NO_RET(protMode <= APT_SW_CLEAR_PROT_ACTION_MAX_VALUE);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->OC_SW_CLR.reg &= (~protMode);
    aptx->OC_SW_CLR.reg |= protMode;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Force generate the specified fault signal.
  * @param aptx APT register base address.
  * @param faultSig Specified fault signal, it supports |.
  * @retval None.
  */
static inline void DCL_APT_GenerateForceFaultSig(APT_RegStruct *aptx, APT_OutCtrlFaultSig faultSig)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(faultSig >= APT_OC_FORCE_GENERATE_CBC_PROTECTION);
    APT_PARAM_CHECK_NO_RET(faultSig <= APT_OC_FORCE_GENERATE_SIGNAL_MAX_VALUE);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->OC_FRC_EVT.reg &= (~faultSig);
    aptx->OC_FRC_EVT.reg |= faultSig;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set and enable CBC/OSHT/FREE delay protection of channel A.
  * @param aptx APT register base address.
  * @param ocMode Selection of CBC or OSHT or FREE, it supports |.
  * @param protDlyChA Delay protection time of channel A, unit: apt_clk.
  * @retval None.
  */
static inline void DCL_APT_SetDelayProtChA(APT_RegStruct *aptx, APT_OutCtrlMode ocMode, unsigned short protDlyChA)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(IsAptOcMode(ocMode));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* OSHT: Clear OSHT mode two bits */
    if (ocMode & APT_OUT_CTRL_ONE_SHOT) {
        aptx->OC_EVT_DLY.BIT.rg_osht_oca_prot_mode = 0;
    }
    /* CBC OSHT FREE: Set the corresponding bits to 1 to enable function */
    aptx->OC_EVT_DLY.reg |= ocMode;
    aptx->OC_DLY_A.BIT.rg_oca_prot_dly = (protDlyChA >= 1? protDlyChA - 1 : 0);
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable CBC/OSHT/FREE delay protection of channel A.
  * @param aptx APT register base address.
  * @param ocMode Selection of CBC or OSHT or FREE, it supports |.
  * @retval None.
  */
static inline void DCL_APT_DisableDelayProtChA(APT_RegStruct *aptx, APT_OutCtrlMode ocMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(IsAptOcMode(ocMode));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* OSHT: Clear OSHT mode two bits */
    if (ocMode & APT_OUT_CTRL_ONE_SHOT) {
        aptx->OC_EVT_DLY.BIT.rg_osht_oca_prot_mode = 0;
    }
    /* CBC OSHT FREE: Set the corresponding bits to 0 to disable function */
    aptx->OC_EVT_DLY.reg &= ~ocMode;

    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set and enable CBC/OSHT/FREE delay protection of channel B.
  * @param aptx APT register base address.
  * @param ocMode Selection of CBC or OSHT or FREE, it supports |.
  * @param protDlyChB Delay protection time of channel B, unit: apt_clk.
  * @retval None.
  */
static inline void DCL_APT_SetDelayProtChB(APT_RegStruct *aptx, APT_OutCtrlMode ocMode, unsigned short protDlyChB)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(IsAptOcMode(ocMode));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* OSHT: Clear OSHT mode two bits */
    if (ocMode & APT_OUT_CTRL_ONE_SHOT) {
        aptx->OC_EVT_DLY.BIT.rg_osht_ocb_prot_mode = 0;
    }
    /* CBC OSHT FREE: Set the corresponding bits to 1 to enable function */
    aptx->OC_EVT_DLY.reg |= (ocMode << 16); /* 16 is the offset of channel B */

    aptx->OC_DLY_B.BIT.rg_ocb_prot_dly = (protDlyChB >= 1? protDlyChB - 1 : 0);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable CBC/OSHT/FREE delay protection of channel B.
  * @param aptx APT register base address.
  * @param ocMode Selection of CBC or OSHT or FREE, it supports |.
  * @retval None.
  */
static inline void DCL_APT_DisableDelayProtChB(APT_RegStruct *aptx, APT_OutCtrlMode ocMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(IsAptOcMode(ocMode));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* OSHT: Clear OSHT mode two bits */
    if (ocMode & APT_OUT_CTRL_ONE_SHOT) {
        aptx->OC_EVT_DLY.BIT.rg_osht_ocb_prot_mode = 0;
    }
    /* CBC OSHT FREE: Set the corresponding bits to 0 to disable function */
    /* 16 is the offset of channel B */
    aptx->OC_EVT_DLY.reg &= (~(ocMode << 16));
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set and enable CBC/OSHT/FREE delay recovery of channel A.
  * @param aptx APT register base address.
  * @param ocMode Selection of CBC or OSHT or FREE, it supports |.
  * @param recyDlyChA Delay recovery time of channel A, unit: apt_clk.
  * @retval None.
  */
static inline void DCL_APT_SetDelayRecyChA(APT_RegStruct *aptx, APT_OutCtrlMode ocMode, unsigned short recyDlyChA)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(IsAptOcMode(ocMode));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* OSHT: Clear OSHT mode two bits to prevent change the OSHT delay mode. */
    if (ocMode & APT_OUT_CTRL_ONE_SHOT) {
        aptx->OC_EVT_DLY.BIT.rg_osht_oca_rcvy_mode = 0;
    }
    /* CBC OSHT FREE: Set the corresponding bits to 1 to enable function */
    /* 2 is the offset from recovery mode to protection mode of OSHT mode of channel A */
    aptx->OC_EVT_DLY.reg |= (((ocMode & ~APT_OUT_CTRL_ONE_SHOT) << 1) | ((ocMode & APT_OUT_CTRL_ONE_SHOT) << 2));

    aptx->OC_DLY_A.BIT.rg_oca_rcvy_dly = (recyDlyChA >= 1? recyDlyChA - 1 : 0);
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable CBC/OSHT/FREE delay recovery of channel A.
  * @param aptx APT register base address.
  * @param ocMode Selection of CBC or OSHT or FREE, it supports |.
  * @retval None.
  */
static inline void DCL_APT_DisableDelayRecyChA(APT_RegStruct *aptx, APT_OutCtrlMode ocMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(IsAptOcMode(ocMode));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* OSHT: Clear OSHT mode two bits */
    if (ocMode & APT_OUT_CTRL_ONE_SHOT) {
        aptx->OC_EVT_DLY.BIT.rg_osht_oca_rcvy_mode = 0;
    }
    /* CBC OSHT FREE: Set the corresponding bits to 1 to enable function */
    /* 2 is the offset from recovery mode to protection mode of OSHT mode of channel A */
    aptx->OC_EVT_DLY.reg &= ~(((ocMode & ~APT_OUT_CTRL_ONE_SHOT) << 1) | ((ocMode & APT_OUT_CTRL_ONE_SHOT) << 2));

    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set and enable CBC/OSHT/FREE delay recovery of channel B.
  * @param aptx APT register base address.
  * @param mode Selection of CBC or OSHT or FREE, it supports |.
  * @param recyDlyChB Delay recovery time of channel B, unit: apt_clk.
  * @retval None.
  */
static inline void DCL_APT_SetDelayRecyChB(APT_RegStruct *aptx, APT_OutCtrlMode mode, unsigned short recyDlyChB)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(IsAptOcMode(mode));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* OSHT: Clear OSHT mode two bits to prevent change the OSHT delay mode. */
    if (mode & APT_OUT_CTRL_ONE_SHOT) {
        aptx->OC_EVT_DLY.BIT.rg_osht_ocb_rcvy_mode = 0;
    }
    /* 2 is the offset from recy mode to prot mode of OSHT of channel A, 16 is the offset from channel A to B */
    aptx->OC_EVT_DLY.reg |= ((((mode & ~APT_OUT_CTRL_ONE_SHOT) << 1) | ((mode & APT_OUT_CTRL_ONE_SHOT) << 2)) << 16);

    aptx->OC_DLY_B.BIT.rg_ocb_rcvy_dly = (recyDlyChB >= 1? recyDlyChB - 1 : 0);
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable CBC/OSHT/FREE delay recovery of channel B.
  * @param aptx APT register base address.
  * @param mode Selection of CBC or OSHT or FREE, it supports |.
  * @retval None.
  */
static inline void DCL_APT_DisableDelayRecyChB(APT_RegStruct *aptx, APT_OutCtrlMode mode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(IsAptOcMode(mode));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* OSHT: Clear OSHT mode two bits to prevent change the OSHT delay mode. */
    if (mode & APT_OUT_CTRL_ONE_SHOT) {
        aptx->OC_EVT_DLY.BIT.rg_osht_ocb_rcvy_mode = 0;
    }
    /* 2 is the offset from recy mode to prot mode of OSHT of channel A, 16 is the offset from channel A to B */
    aptx->OC_EVT_DLY.reg &= ~((((mode & ~APT_OUT_CTRL_ONE_SHOT) << 1) | ((mode & APT_OUT_CTRL_ONE_SHOT) << 2)) << 16);

    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set and enable OSHT aligment protection and recovery event of channel A.
  * @param aptx APT register base address.
  * @param alignEvt Aligment event of OSHT mode.
  * @retval None.
  */
static inline void DCL_APT_SetOshtAlignChA(APT_RegStruct *aptx, APT_OshtAlignEvent alignEvt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(alignEvt >= APT_OC_OSHT_ALIGNMENT_PROT_AT_CNTR_ZERO);
    APT_PARAM_CHECK_NO_RET(alignEvt <= APT_OC_OSHT_ALIGNMENT_RECY_AT_CNTR_PERIOD);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* Clear OSHT two bits first */
    unsigned int a = aptx->OC_EVT_DLY.BIT.rg_osht_oca_prot_mode;
    unsigned int b = aptx->OC_EVT_DLY.BIT.rg_osht_oca_rcvy_mode;
    aptx->OC_EVT_DLY.BIT.rg_osht_oca_prot_mode = (alignEvt & APT_OC_OSHT_ALIGNMENT_PROT_AT_CNTR_ZERO)? 0 : a;
    aptx->OC_EVT_DLY.BIT.rg_osht_oca_rcvy_mode = (alignEvt & APT_OC_OSHT_ALIGNMENT_RECY_AT_CNTR_ZERO)? 0 : b;
    /* Set aligment event */
    aptx->OC_EVT_DLY.reg |= alignEvt;
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set and enable OSHT aligment protection and recovery event of channel B.
  * @param aptx APT register base address.
  * @param alignEvt Aligment event of OSHT mode.
  * @retval None.
  */
static inline void DCL_APT_SetOshtAlignChB(APT_RegStruct *aptx, APT_OshtAlignEvent alignEvt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(alignEvt >= APT_OC_OSHT_ALIGNMENT_PROT_AT_CNTR_ZERO);
    APT_PARAM_CHECK_NO_RET(alignEvt <= APT_OC_OSHT_ALIGNMENT_RECY_AT_CNTR_PERIOD);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* Clear OSHT two bits first */
    unsigned int a = aptx->OC_EVT_DLY.BIT.rg_osht_ocb_prot_mode;
    unsigned int b = aptx->OC_EVT_DLY.BIT.rg_osht_ocb_rcvy_mode;
    aptx->OC_EVT_DLY.BIT.rg_osht_ocb_prot_mode = (alignEvt & APT_OC_OSHT_ALIGNMENT_PROT_AT_CNTR_ZERO)? 0 : a;
    aptx->OC_EVT_DLY.BIT.rg_osht_ocb_rcvy_mode = (alignEvt & APT_OC_OSHT_ALIGNMENT_RECY_AT_CNTR_ZERO)? 0 : b;
    /* 16 is the offset from channel A to channel B */
    aptx->OC_EVT_DLY.reg |= (alignEvt << 16);
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Enable the maximum time CBC protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline void DCL_APT_EnableCbcMaxProtTime(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Enable channel A. */
        aptx->OC_PMAX_A.BIT.rg_oca_max_prot_cbc_en = BASE_CFG_ENABLE;
    } else {
        /* Enable channel B. */
        aptx->OC_PMAX_B.BIT.rg_ocb_max_prot_cbc_en = BASE_CFG_ENABLE;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable the maximum time CBC protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline void DCL_APT_DisableCbcMaxProtTime(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Disable channel A. */
        aptx->OC_PMAX_A.BIT.rg_oca_max_prot_cbc_en = BASE_CFG_DISABLE;
    } else {
        /* Disable channel B. */
        aptx->OC_PMAX_B.BIT.rg_ocb_max_prot_cbc_en = BASE_CFG_DISABLE;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Enable the maximum time Free protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline void DCL_APT_EnableFreeMaxProtTime(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Enable channel A. */
        aptx->OC_PMAX_A.BIT.rg_oca_max_prot_free_en = BASE_CFG_ENABLE;
    } else {
        /* Enable channel B. */
        aptx->OC_PMAX_B.BIT.rg_ocb_max_prot_free_en = BASE_CFG_ENABLE;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable the maximum time Free protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline void DCL_APT_DisableFreeMaxProtTime(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Disable channel A. */
        aptx->OC_PMAX_A.BIT.rg_oca_max_prot_free_en = BASE_CFG_DISABLE;
    } else {
        /* Disable channel B. */
        aptx->OC_PMAX_B.BIT.rg_ocb_max_prot_free_en = BASE_CFG_DISABLE;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set the maximum time CBC and Free protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @param maxTime Maximum protection time, unit: apt_clk.
  * @retval None.
  */
static inline void DCL_APT_SetCbcAndFreeMaxProtTime(APT_RegStruct *aptx, APT_PWMChannel ch, unsigned short maxTime)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Channel A. */
        aptx->OC_PMAX_A.BIT.rg_oca_max_prot = maxTime;
    } else {
        /* Channel B. */
        aptx->OC_PMAX_B.BIT.rg_ocb_max_prot = maxTime;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get the maximum time CBC and Free protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline unsigned short DCL_APT_GetCbcAndFreeMaxProtTime(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Channel A. */
        return aptx->OC_PMAX_A.BIT.rg_oca_max_prot;
    } else {
        /* Channel B. */
        return aptx->OC_PMAX_B.BIT.rg_ocb_max_prot;
    }
}

/**
  * @brief Enable the minimum time Free protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline void DCL_APT_EnableFreeMinProtTime(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Enable channel A. */
        aptx->OC_PMIN_A.BIT.rg_oca_min_prot_free_en = BASE_CFG_ENABLE;
    } else {
        /* Enable channel B. */
        aptx->OC_PMIN_B.BIT.rg_ocb_min_prot_free_en = BASE_CFG_ENABLE;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
}

/**
  * @brief Disable the minimum time Free protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline void DCL_APT_DisableFreeMinProtTime(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Disable channel A. */
        aptx->OC_PMIN_A.BIT.rg_oca_min_prot_free_en = BASE_CFG_DISABLE;
    } else {
        /* Disable channel B. */
        aptx->OC_PMIN_B.BIT.rg_ocb_min_prot_free_en = BASE_CFG_DISABLE;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set the minimum time Free protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @param minTime Minimum protection time, unit: apt_clk.
  * @retval None.
  */
static inline void DCL_APT_SetFreeMinProtTime(APT_RegStruct *aptx, APT_PWMChannel ch, unsigned short minTime)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Channel A. */
        aptx->OC_PMIN_A.BIT.rg_oca_min_prot = minTime;
    } else {
        /* Channel B. */
        aptx->OC_PMIN_B.BIT.rg_ocb_min_prot = minTime;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get the minimum time Free protection limit.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline unsigned short DCL_APT_GetFreeMinProtTime(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Channel A. */
        return aptx->OC_PMIN_A.BIT.rg_oca_min_prot;
    } else {
        /* Channel B. */
        return aptx->OC_PMIN_B.BIT.rg_ocb_min_prot;
    }
}

/**
  * @brief Enable protection accumulated timer of channel A/B.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline void DCL_APT_EnableProtTimer(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Channel A. */
        aptx->OC_TIMER_CFG.BIT.rg_oca_timer_en = BASE_CFG_ENABLE;
    } else {
        /* Channel B. */
        aptx->OC_TIMER_CFG.BIT.rg_ocb_timer_en = BASE_CFG_ENABLE;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable protection accumulated timer of channel A/B.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval None.
  */
static inline void DCL_APT_DisableProtTimer(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    if (ch == APT_PWM_CHANNEL_A) {
        /* Channel A. */
        aptx->OC_TIMER_CFG.BIT.rg_oca_timer_en = BASE_CFG_DISABLE;
    } else {
        /* Channel B. */
        aptx->OC_TIMER_CFG.BIT.rg_ocb_timer_en = BASE_CFG_DISABLE;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get the accumulated protection time.
  * @param aptx APT register base address.
  * @param ch Pwm channel select.
  * @retval unit: apt_clk.
  */
static inline unsigned int DCL_APT_GetProtTimer(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Channel A. */
        return aptx->OC_TIMER_A;
    } else {
        /* Channel B. */
        return aptx->OC_TIMER_B;
    }
}

/**
  * @brief Enable CBC accumulated protection timer.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_EnableCbcProtTimer(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->OC_TIMER_CFG.BIT.rg_cbc_timer_en = BASE_CFG_ENABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable CBC accumulated protection timer.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_DisableCbcProtTimer(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->OC_TIMER_CFG.BIT.rg_cbc_timer_en = BASE_CFG_DISABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get the accumulated time of the CBC protection timer.
  * @param aptx APT register base address.
  * @retval unit: apt_clk.
  */
static inline unsigned int DCL_APT_GetCbcProtTimer(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->OC_CBC_TIMER;
}

/**
  * @brief Disable CBC protection switching to OSHT timer.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_DisableCbcSwitchToOshtProtTimer(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* 0xFFFFFFFF: disable the function that CBC protection switch to OSHT protection. */
    aptx->OC_CBC_TIMER_OSHT = 0xFFFFFFFF;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Enable and Set CBC protection switching to OSHT.
  * @param aptx APT register base address.
  * @param cbc2OshtTimer Duration time for CBC switch to OSHT protection.
  * @retval None.
  */
static inline void DCL_APT_SetCbcSwitchToOshtProtTimer(APT_RegStruct *aptx, unsigned int cbc2OshtTimer)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* 0xFFFFFFFF is the threshold value. */
    APT_PARAM_CHECK_NO_RET(cbc2OshtTimer < 0xFFFFFFFF);
    aptx->OC_CBC_TIMER_OSHT = cbc2OshtTimer;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get CBC protection switching to OSHT time.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline unsigned int DCL_APT_GetCbcSwitchToOshtProtTimer(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->OC_CBC_TIMER_OSHT;
}


/**
  * @brief Disable CBC protection switching to OSHT clearance timer.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_DisableCbcSwitchToOshtClrTimer(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* 0xFFFFFFFF: disable the clearance function that CBC protection switch to OSHT protection. */
    aptx->OC_CBC_TIMER_CLR = 0xFFFFFFFF;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Enable and Set CBC protection switching to OSHT clearance timer.
  * @param aptx APT register base address.
  * @param cbc2OshtClrTimer OC_CBC_TIMER is cleared when CBC protection does not occur within this time.
  * @retval None.
  */
static inline void DCL_APT_SetCbcSwitchToOshtClrTimer(APT_RegStruct *aptx, unsigned int cbc2OshtClrTimer)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    /* 0xFFFFFFFF is the threshold value. */
    APT_PARAM_CHECK_NO_RET(cbc2OshtClrTimer < 0xFFFFFFFF);
    aptx->OC_CBC_TIMER_CLR = cbc2OshtClrTimer;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get CBC protection switching to OSHT clearance timer.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline unsigned int DCL_APT_GetCbcSwitchToOshtClrTimer(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->OC_CBC_TIMER_CLR;
}

/* --------------------------------------------------------------------------------------------- */
/* Interrupt Generation (IG) submodule Direct Configuration Layer functions -------------------- */
/* --------------------------------------------------------------------------------------------- */

/**
  * @brief Set the trigger mode of output control event interrupt.
  * @param aptx APT register base address.
  * @param trgMode Event intrrupt trigger mode: RISING_EDGE/HIGH_LEVEL.
  * @retval None.
  */
static inline void DCL_APT_SetEventIntTrgMode(APT_RegStruct *aptx, APT_EventIntTrgMode trgMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(trgMode >= APT_EVENT_INTERRUPT_MODE_RISING_EDGE);
    APT_PARAM_CHECK_NO_RET(trgMode <= APT_EVENT_INTERRUPT_MODE_HIGH_LEVEL);
    aptx->INT_EVT_CFG.BIT.rg_int_evt_mode = trgMode;
}

/**
  * @brief Enable the output control event to generate event interrupt.
  * @param aptx APT register base address.
  * @param faultEvent Output control event, including CBC and Free timeout interrupts, it supports |.
  * @retval None.
  */
static inline void DCL_APT_EnableEventInterrupt(APT_RegStruct *aptx, APT_EventInterruptSrc faultEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(faultEvent >= APT_OC_FAULT_EVENT_CBC && faultEvent <= APT_OC_FAULT_EVENT_MAX_VALUE);
    aptx->INT_EVT_EN.reg |= faultEvent;
}

/**
  * @brief Disable the output control event to generate event interrupt.
  * @param aptx APT register base address.
  * @param faultEvent Output control event, including CBC and Free timeout interrupts, it supports |.
  * @retval None.
  */
static inline void DCL_APT_DisableEventInterrupt(APT_RegStruct *aptx, APT_EventInterruptSrc faultEvent)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(faultEvent >= APT_OC_FAULT_EVENT_CBC && faultEvent <= APT_OC_FAULT_EVENT_MAX_VALUE);
    aptx->INT_EVT_EN.reg &= (~faultEvent);
}

/**
  * @brief Enable timer interrupt of APT module.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_EnableTimerInterrupt(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->INT_TMR_EN.BIT.rg_int_en_tmr = BASE_CFG_SET;
}

/**
  * @brief Disable timer interrupt of APT module.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_DisableTimerInterrupt(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->INT_TMR_EN.BIT.rg_int_en_tmr = BASE_CFG_UNSET;
}

/**
  * @brief Get the timer interrupt flag.
  * @param aptx APT register base address.
  * @retval bool: true, false.
  */
static inline bool DCL_APT_GetTimerInterruptFlag(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->INT_TMR_FLAG.BIT.ro_int_flag_tmr);
}

/**
  * @brief Clear the timer interrupt flag.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_ClearTimerInterruptFlag(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->INT_TMR_FLAG.BIT.rg_int_clr_tmr = BASE_CFG_SET;
}

/**
  * @brief Select the source of timer interrupt.
  * @param aptx APT register base address.
  * @param tmrIntSrc Source of timer interrupt.
  * @retval None.
  */
static inline void DCL_APT_SetTimerInterruptSrc(APT_RegStruct *aptx, APT_TimerInterruptSrc tmrIntSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(tmrIntSrc >= APT_TIMER_INTERRUPT_SRC_CNTR_DISABLE);
    APT_PARAM_CHECK_NO_RET(tmrIntSrc <= APT_TIMER_INTERRUPT_SRC_TMR_INT_MIX);
    aptx->INT_TMR_SEL.BIT.rg_int_tmr_sel = tmrIntSrc;
}

/**
  * @brief Enable the synchronization of timer interrupt scale initial count value.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_EnableTimerInterruptCountSyncInit(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->INT_PRSC_CFG.BIT.rg_int_prsc_synen = BASE_CFG_SET;
}

/**
  * @brief Disable the synchronization of timer interrupt scale initial count value.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_DisableTimerInterruptCountSyncInit(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->INT_PRSC_CFG.BIT.rg_int_prsc_synen = BASE_CFG_UNSET;
}

/**
  * @brief Set the initial count value of timer interrupt scale.
  * @param aptx APT register base address.
  * @param intCntInitVal Initial count value of timer interrupt scale, 4 bits.
  * @retval None.
  */
static inline void DCL_APT_SetTimerInterruptCountSyncInitVal(APT_RegStruct *aptx, unsigned short intCntInitVal)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(intCntInitVal <= TIMER_INTERRUPT_CNT_MAX);
    aptx->INT_PRSC_CFG.BIT.rg_int_prsc_phs = intCntInitVal;
}

/**
  * @brief Set the count period of timer interrupt scale.
  * @param aptx APT register base address.
  * @param intCntPeriod Count period of timer interrupt scale.
  * @retval None.
  */
static inline void DCL_APT_SetTimerInterruptCountPeriod(APT_RegStruct *aptx, unsigned short intCntPeriod)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(intCntPeriod <= TIMER_INTERRUPT_CNT_MAX);
    aptx->INT_PRSC_CFG.BIT.rg_int_prsc_prd = intCntPeriod;
}

/**
  * @brief Get the count value of timer interrupt scale.
  * @param aptx APT register base address.
  * @retval unsigned short: Count value of timer interrupt scale.
  */
static inline unsigned short DCL_APT_GetTimerInterruptCount(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->INT_PRSC_CFG.BIT.ro_int_prsc_cnt);
}

/**
  * @brief Force the count value of timer interrupt scale to increase.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_ForceTimerInterruptCountIncr(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->INT_PRSC_CFG.BIT.rg_int_prsc_frc = BASE_CFG_SET;
}

/**
  * @brief Set and Enable sources to trigger INT_TMR_MIX.
  * @param aptx APT register base address.
  * @param tmrMixSrc Signal source that triggers the INT_TMR_MIX, it supports |.
  * @retval None.
  */
static inline void DCL_APT_SetTimerMixTrigSrc(APT_RegStruct *aptx, APT_TmrMixTriggerSrc tmrMixSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(tmrMixSrc >= APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_ZERO);
    APT_PARAM_CHECK_NO_RET(tmrMixSrc <= APT_INT_TIMER_MIX_TRIGGER_SRC_MAX_VALUE);
    aptx->INT_MIX_CFG.reg |= tmrMixSrc;
}

/**
  * @brief Disable sources to trigger INT_TMR_MIX.
  * @param aptx APT register base address.
  * @param tmrMixSrc Signal source that triggers the INT_TMR_MIX, it supports |.
  * @retval None.
  */
static inline void DCL_APT_DisableTimerMixTrigSrc(APT_RegStruct *aptx, APT_TmrMixTriggerSrc tmrMixSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(tmrMixSrc >= APT_INT_TIMER_MIX_TRIGGER_SRC_CNTR_ZERO);
    APT_PARAM_CHECK_NO_RET(tmrMixSrc <= APT_INT_TIMER_MIX_TRIGGER_SRC_MAX_VALUE);
    aptx->INT_MIX_CFG.reg &= (~tmrMixSrc);
}

/* --------------------------------------------------------------------------------------------- */
/* ADC Converter Start (CS) submodule Direct Configuration Layer functions --------------------- */
/* --------------------------------------------------------------------------------------------- */

/**
  * @brief Enable the ADC trigger channel.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @retval None.
  */
static inline void DCL_APT_EnableADCTrigger(APT_RegStruct *aptx, APT_ADCTriggerChannel csTrgCh)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        aptx->CS_TMR_SELA.BIT.rg_csa_en_cs = BASE_CFG_SET;
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        aptx->CS_TMR_SELB.BIT.rg_csb_en_cs = BASE_CFG_SET;
    }
}

/**
  * @brief Disable the ADC trigger channel.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @retval None.
  */
static inline void DCL_APT_DisableADCTrigger(APT_RegStruct *aptx, APT_ADCTriggerChannel csTrgCh)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        aptx->CS_TMR_SELA.BIT.rg_csa_en_cs = BASE_CFG_UNSET;
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        aptx->CS_TMR_SELB.BIT.rg_csb_en_cs = BASE_CFG_UNSET;
    }
}

/**
  * @brief Select the source of ADC trigger channel.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @param csTrgSrc Source of ADC trigger.
  * @retval None.
  */
static inline void DCL_APT_SetADCTriggerSrc(APT_RegStruct *aptx,
                                            APT_ADCTriggerChannel csTrgCh,
                                            APT_ADCTriggerSource csTrgSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    APT_PARAM_CHECK_NO_RET(csTrgSrc >= APT_CS_SRC_COMBINE_EVENT_A1);
    APT_PARAM_CHECK_NO_RET(csTrgSrc <= APT_CS_SRC_SOC_SELF_MIX);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        aptx->CS_TMR_SELA.BIT.rg_csa_tmr_sel = csTrgSrc;
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        aptx->CS_TMR_SELB.BIT.rg_csb_tmr_sel = csTrgSrc;
    }
}

/**
  * @brief Enable synchronization of ADC trigger scale initial count value.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @retval None.
  */
static inline void DCL_APT_EnableADCTriggerCountSyncInit(APT_RegStruct *aptx, APT_ADCTriggerChannel csTrgCh)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        aptx->CS_PRSCA_CFG.BIT.rg_csa_prsc_synen = BASE_CFG_SET;
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        aptx->CS_PRSCB_CFG.BIT.rg_csb_prsc_synen = BASE_CFG_SET;
    }
}

/**
  * @brief Disable synchronization of ADC trigger scale initial count value.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @retval None.
  */
static inline void DCL_APT_DisableADCTriggerCountSyncInit(APT_RegStruct *aptx, APT_ADCTriggerChannel csTrgCh)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        aptx->CS_PRSCA_CFG.BIT.rg_csa_prsc_synen = BASE_CFG_UNSET;
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        aptx->CS_PRSCB_CFG.BIT.rg_csb_prsc_synen = BASE_CFG_UNSET;
    }
}

/**
  * @brief Set the initial count value of ADC trigger scale.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @param csCntInitVal Initial count value of ADC trigger scale.
  * @retval None.
  */
static inline void DCL_APT_SetADCTriggerCountSyncInitVal(APT_RegStruct *aptx,
                                                         APT_ADCTriggerChannel csTrgCh,
                                                         unsigned short csCntInitVal)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    APT_PARAM_CHECK_NO_RET(csCntInitVal <= ADC_CONVERSION_START_CNT_MAX);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        aptx->CS_PRSCA_CFG.BIT.rg_csa_prsc_phs = csCntInitVal;
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        aptx->CS_PRSCB_CFG.BIT.rg_csb_prsc_phs = csCntInitVal;
    }
}

/**
  * @brief Set the count period of ADC trigger scale.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @param csCntPeriod Count period of ADC trigger scale.
  * @retval None.
  */
static inline void DCL_APT_SetADCTriggerCountPeriod(APT_RegStruct *aptx,
                                                    APT_ADCTriggerChannel csTrgCh,
                                                    unsigned short csCntPeriod)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    APT_PARAM_CHECK_NO_RET(csCntPeriod <= ADC_CONVERSION_START_CNT_MAX);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        aptx->CS_PRSCA_CFG.BIT.rg_csa_prsc_prd = csCntPeriod;
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        aptx->CS_PRSCB_CFG.BIT.rg_csb_prsc_prd = csCntPeriod;
    }
}

/**
  * @brief Force the count value of ADC trigger scale to increase.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @retval None.
  */
static inline void DCL_APT_ForceADCTriggerCountIncr(APT_RegStruct *aptx, APT_ADCTriggerChannel csTrgCh)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        aptx->CS_PRSCA_CFG.BIT.rg_csa_prsc_frc = BASE_CFG_SET;
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        aptx->CS_PRSCB_CFG.BIT.rg_csb_prsc_frc = BASE_CFG_SET;
    }
}

/**
  * @brief Get the flag of ADC trigger.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @retval bool: true, false.
  */
static inline bool DCL_APT_GetADCTriggerFlag(APT_RegStruct *aptx, APT_ADCTriggerChannel csTrgCh)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(csTrgCh >= APT_ADC_CONVERSION_START_A, false);
    APT_PARAM_CHECK_WITH_RET(csTrgCh <= APT_ADC_CONVERSION_START_B, false);
    return ((aptx->CS_FLAG.reg & csTrgCh) == csTrgCh);
}

/**
  * @brief Clear the flag of ADC trigger.
  * @param aptx APT register base address.
  * @param csTrgCh ADC trigger channel.
  * @retval None.
  */
static inline void DCL_APT_ClearADCTriggerFlag(APT_RegStruct *aptx, APT_ADCTriggerChannel csTrgCh)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    unsigned int trgFlgOffset = 16; /* Offset of ADC trigget flag clear */
    aptx->CS_FLAG.reg |= (csTrgCh << trgFlgOffset);
}

/**
  * @brief Configure the DMA request of ADC trigger.
  * @param aptx APT register base address.
  * @param csDMAReqSrc DMA request source of ADC Converter Start submodule.
  * @param csDMAType DMA request type of ADC Converter Start submodule.
  * @retval None.
  */
static inline void DCL_APT_SetADCTriggerDMAReq(APT_RegStruct *aptx,
                                               APT_ADCTrgDMAReqSrc csDMAReqSrc,
                                               APT_ADCTrgDMAReqType csDMAType)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csDMAReqSrc >= APT_CS_DMA_REQ_SRC_DISABLE);
    APT_PARAM_CHECK_NO_RET(csDMAReqSrc <= APT_CS_DMA_REQ_SRC_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(csDMAType >= APT_CS_DMA_BURST_REQUEST);
    APT_PARAM_CHECK_NO_RET(csDMAType <= APT_CS_DMA_SINGLE_REQUEST);
    aptx->CS_DMA.reg &= (~(0b11 << csDMAType));
    aptx->CS_DMA.reg |= (csDMAReqSrc << csDMAType);
}

/**
  * @brief Set and Enable sources to trigger SOCA_MIX or SOCB_MIX.
  * @param aptx APT register base address.
  * @param socMixSrc Signal source that triggers the ADC_MIX, it supports |.
  * @retval None.
  */
static inline void DCL_APT_SetSocMixTrigSrc(APT_RegStruct *aptx, \
                                            APT_ADCTriggerChannel csTrgCh, \
                                            APT_ADCMixTriggerSrc socMixSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    APT_PARAM_CHECK_NO_RET(socMixSrc >= APT_MIX_TRIGGER_SRC_CNTR_ZERO);
    APT_PARAM_CHECK_NO_RET(socMixSrc <= APT_MIX_TRIGGER_SRC_MAX_VALUE);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        /* Set adc mix trigger source of SOCA. */
        aptx->CS_SOCA_MIX_CFG.reg |= socMixSrc;
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        /* Set adc mix trigger source of SOCB. */
        aptx->CS_SOCB_MIX_CFG.reg |= socMixSrc;
    }
}

/**
  * @brief Disable sources to trigger SOCA_MIX or SOCB_MIX.
  * @param aptx APT register base address.
  * @param socMixSrc Signal source that triggers the ADC_MIX, it supports |.
  * @retval None.
  */
static inline void DCL_APT_DisableSocMixTrigSrc(APT_RegStruct *aptx, \
                                                APT_ADCTriggerChannel csTrgCh, \
                                                APT_ADCMixTriggerSrc socMixSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(csTrgCh >= APT_ADC_CONVERSION_START_A);
    APT_PARAM_CHECK_NO_RET(csTrgCh <= APT_ADC_CONVERSION_START_B);
    APT_PARAM_CHECK_NO_RET(socMixSrc >= APT_MIX_TRIGGER_SRC_CNTR_ZERO);
    APT_PARAM_CHECK_NO_RET(socMixSrc <= APT_MIX_TRIGGER_SRC_MAX_VALUE);
    if (csTrgCh == APT_ADC_CONVERSION_START_A) {
        /* Disable adc mix trigger source of SOCA. */
        aptx->CS_SOCA_MIX_CFG.reg &= (~socMixSrc);
    } else if (csTrgCh == APT_ADC_CONVERSION_START_B) {
        /* Disable adc mix trigger source of SOCA. */
        aptx->CS_SOCB_MIX_CFG.reg &= (~socMixSrc);
    }
}

/* --------------------------------------------------------------------------------------------- */
/* Event Management (EM) submodule Direct Configuration Layer functions -------------------------- */
/* --------------------------------------------------------------------------------------------- */

/**
 * @brief When the logicial OR result of TRIP events and MUX events is selected as the source of EM group event,
 *        this function is called to enable which events can participate in the logical OR operation.
 * @param aptx APT register base address.
 * @param tripOrGroup The group of Event Management, which can be APT_EM_MODULE_A or APT_EM_MODULE_B.
 *                Each EM group has 2 events. All the 4 group events are enumerated in APT_EMGroupEvent.
 * @param tripSrc The logical OR operation source of group event 1 from trip1~trip15, it supports |.
 * @retval None.
 */
static inline void DCL_APT_SetEMTripOR(APT_RegStruct *aptx,
                                       APT_EMTripOrGrp tripOrGroup,
                                       APT_EMTripSrc tripSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(tripOrGroup >= APT_EM_TRIP_OR_A1);
    APT_PARAM_CHECK_NO_RET(tripOrGroup <= APT_EM_TRIP_OR_B2);
    APT_PARAM_CHECK_NO_RET(tripSrc >= APT_EM_OR_EN_TRIP1 && tripSrc <= APT_EM_OR_EN_MAX_VALUE);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    switch (tripOrGroup) {
        case APT_EM_TRIP_OR_A1:
            /* Set TRIP_OR_A1 event. */
            aptx->EM_AOR_EN.BIT.rg_em_a1_oren = tripSrc;
            break;
        case APT_EM_TRIP_OR_A2:
            /* Set TRIP_OR_A2 event. */
            aptx->EM_AOR_EN.BIT.rg_em_a2_oren = tripSrc;
            break;
        case APT_EM_TRIP_OR_B1:
            /* Set TRIP_OR_B1 event. */
            aptx->EM_BOR_EN.BIT.rg_em_b1_oren = tripSrc;
            break;
        case APT_EM_TRIP_OR_B2:
            /* Set TRIP_OR_B2 event. */
            aptx->EM_BOR_EN.BIT.rg_em_b2_oren = tripSrc;
            break;
        default:
            break;
    }
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Select the combine event source of GRP_A1, GRP_A2, GRP_B1, GRP_B2.
  * @param aptx APT register base address.
  * @param evtGroup Combine event source group.
  * @param combineEvtSrc Combine event source.
  * @retval None.
  */
static inline void DCL_APT_SetCombineGroupSrc(APT_RegStruct *aptx,
                                              APT_EMCombineEvtSrcGrp evtGroup,
                                              APT_EMCombineEvtSrc combineEvtSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(evtGroup >= APT_EM_COMBINE_EVENT_A1_SRC);
    APT_PARAM_CHECK_NO_RET(evtGroup <= APT_EM_COMBINE_EVENT_B2_SRC);
    APT_PARAM_CHECK_NO_RET(combineEvtSrc >= APT_EM_COMBINE_SRC_TRIP_1);
    APT_PARAM_CHECK_NO_RET(combineEvtSrc <= APT_EM_COMBINE_SRC_ALL_EVENT_OR);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    unsigned int grpEvtFieldWidth = 4; /* Bit field width of combine event group input source setting */
    aptx->EM_MRG_SEL.reg &= (~(0b1111 << (evtGroup * grpEvtFieldWidth)));
    aptx->EM_MRG_SEL.reg |= (combineEvtSrc << (evtGroup * grpEvtFieldWidth));
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Select combine event temp.
  * @param aptx APT register base address.
  * @param cmbEvtTmp Combine event.
  * @param cmbMode Combine mode.
  * @retval None.
  */
static inline void DCL_APT_SetCombineEventSrc(APT_RegStruct *aptx,
                                              APT_EMCombineEventTemp cmbEvtTmp,
                                              APT_EMCombineEvtMode cmbMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(cmbEvtTmp >= APT_EM_COMBINE_EVENT_A1_TEMP && cmbEvtTmp <= APT_EM_COMBINE_EVENT_B2_TEMP);
    APT_PARAM_CHECK_NO_RET(cmbMode >= APT_EM_COMBINE_LOW_LEVEL && cmbMode <= APT_EM_COMBINE_EVT2_H);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    unsigned int cmbModeOffset = 16; /* Offset of combine mode */
    unsigned int cmbModeFieldWidth = 4; /* Bit field width of combine mode */
    aptx->EM_MRG_SEL.reg &= (~(0b111 << (cmbModeOffset + cmbEvtTmp * cmbModeFieldWidth)));
    aptx->EM_MRG_SEL.reg |= (cmbMode << (cmbModeOffset + cmbEvtTmp * cmbModeFieldWidth));
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Select the source of Event Management submodule filter event.
  * @param aptx APT register base address.
  * @param cmbEvtFilSrc Select filter input from combine events temp.
  * @retval None.
  */
static inline void DCL_APT_SelectFilterEventInput(APT_RegStruct *aptx, APT_EMCombineEventTemp cmbEvtFilSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(cmbEvtFilSrc >= APT_EM_COMBINE_EVENT_A1_TEMP);
    APT_PARAM_CHECK_NO_RET(cmbEvtFilSrc <= APT_EM_COMBINE_EVENT_B2_TEMP);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_OUT_SEL.BIT.rg_evtfilt_sel = cmbEvtFilSrc;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set the output type of combine event.
  * @param aptx APT register base address.
  * @param cmbEvt Combine event.
  * @param filter Whether the output of combine event is filtered.
  * @retval None.
  */
static inline void DCL_APT_SetCombineEventOut(APT_RegStruct *aptx,
                                              APT_EMCombineEvent cmbEvt,
                                              APT_EMCombineEventOut filter)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(cmbEvt >= APT_EM_COMBINE_EVENT_A1);
    APT_PARAM_CHECK_NO_RET(cmbEvt <= APT_EM_COMBINE_EVENT_B2);
    APT_PARAM_CHECK_NO_RET(filter >= APT_EM_COMBINE_EVENT_OUT_ORIG_SIGNAL);
    APT_PARAM_CHECK_NO_RET(filter <= APT_EM_COMBINE_EVENT_OUT_FILT_SIGNAL);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_OUT_SEL.reg &= (~(0b1 << cmbEvt));
    aptx->EM_OUT_SEL.reg |= (filter << cmbEvt);
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}


/**
  * @brief Enable mask window.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_EnableMaskWindow(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_WD_EN.BIT.rg_mskwd_en = BASE_CFG_SET;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable mask window.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_DisableMaskWindow(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_WD_EN.BIT.rg_mskwd_en = BASE_CFG_UNSET;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Configure the polarity and reset mode of mask window.
  * @param aptx APT register base address.
  * @param polar Polarity of mask window.
  * @param rstMode Reset mode of mask window.
  * @retval None.
  */
static inline void DCL_APT_SetMaskWindow(APT_RegStruct *aptx, APT_MaskWinPolarity polar, APT_MaskWinResetMode rstMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(polar >= APT_BLANK_EVENT_INSIDE_MASK_WIN_LOW);
    APT_PARAM_CHECK_NO_RET(polar >= APT_BLANK_EVENT_OUTSIDE_MASK_WIN_HIGH);
    APT_PARAM_CHECK_NO_RET(rstMode >= APT_RESET_MASK_WIN_DISABLE);
    APT_PARAM_CHECK_NO_RET(rstMode <= APT_RESET_MASK_WIN_CNTR_ZERO_PERIOD);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_WD_EN.BIT.rg_mskwd_psel = polar;
    aptx->EM_WD_EN.reg &= (~0b11);
    aptx->EM_WD_EN.reg |= rstMode;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set the offset and width of mask window.
  * @param aptx APT register base address.
  * @param mskWinOffset Offset of mask window, in units of time-base clock frequency.
  * @retval None.
  */
static inline void DCL_APT_SetMaskWindowOffsetAndWidth(APT_RegStruct *aptx,
                                                       unsigned short mskWinOffset,
                                                       unsigned short mskWinWidth)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_WD_CNT.BIT.rg_mskwd_offset = mskWinOffset;
    aptx->EM_WD_CNT.BIT.rg_mskwd_width = mskWinWidth;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get the count value of mask window offset.
  * @param aptx APT register base address.
  * @retval unsigned short: Count value of mask window offset.
  */
static inline unsigned short DCL_APT_GetMaskWindowOffsetCount(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->EM_WD_STS.BIT.ro_offset_cnt);
}

/**
  * @brief Get the count value of mask window width.
  * @param aptx APT register base address.
  * @retval unsigned short: Count value of mask window width.
  */
static inline unsigned short DCL_APT_GetMaskWindowWidthCount(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->EM_WD_STS.BIT.ro_width_cnt);
}

/**
  * @brief Enable valley capture.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_EnableValleyCapture(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_VCAP_CFG.BIT.rg_vcap_en = BASE_CFG_ENABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable valley capture.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_DisableValleyCapture(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_VCAP_CFG.BIT.rg_vcap_en = BASE_CFG_DISABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Select the clock source of valley capture.
  * @param aptx APT register base address.
  * @param clkMode Clock source of valley capture.
  * @retval None.
  */
static inline void DCL_APT_SetValleyCapClockMode(APT_RegStruct *aptx, APT_ValleyCapClkMode clkMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(clkMode >= APT_VALLY_CAP_USE_MAIN_CLOCK);
    APT_PARAM_CHECK_NO_RET(clkMode <= APT_VALLEY_CAP_USE_DIVIDER_CLOCK);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_VCAP_CFG.BIT.rg_vcap_div_mode = clkMode;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Select the trigge source of valley capture.
  * @param aptx APT register base address.
  * @param vcapSrc Source of valley capture.
  * @param edge Edge type of valley capture.
  * @retval None.
  */
static inline void DCL_APT_SetValleyCapSrc(APT_RegStruct *aptx,
                                           APT_ValleyCapRstType vcapSrc,
                                           APT_ValleyCapEdgeType edge)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(vcapSrc >= APT_VALLEY_CAP_SRC_DISABLE);
    APT_PARAM_CHECK_NO_RET(vcapSrc <= APT_VALLEY_CAP_SRC_COMBINE_EVENT_B2_TEMP);
    APT_PARAM_CHECK_NO_RET(edge >= APT_VALLEY_CAP_RISING_EDGE);
    APT_PARAM_CHECK_NO_RET(edge <= APT_VALLEY_CAP_FALLING_EDGE);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_VCAP_CFG.BIT.rg_vcap_trig_sel = vcapSrc;
    aptx->EM_VCAP_CFG.BIT.rg_vcap_edg_sel = edge;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set the valley capture count value of start edge and stop edge.
  * @param aptx  APT register base address.
  * @param startCount Count value of start edge, range 4bits.
  * @param stopCount Count value of stop edge, range 4bits.
  * @retval None.
  */
static inline void DCL_APT_SetValleyCapEdgeCount(APT_RegStruct *aptx,
                                                 unsigned short startCount,
                                                 unsigned short stopCount)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(startCount < stopCount);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_VCAP_CFG.BIT.rg_vcap_sta_edg = startCount;
    aptx->EM_VCAP_CFG.BIT.rg_vcap_stp_edg = stopCount;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Set the delay calibration of valley capture.
  * @param aptx APT register base address.
  * @param delayMode Delay mode of valley capture.
  * @param swDelay Software delay value.
  * @retval None.
  */
static inline void DCL_APT_SetValleyCapDelay(APT_RegStruct *aptx,
                                             APT_ValleyDelayMode delayMode,
                                             unsigned short swDelay)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(delayMode >= APT_VCAP_SW_DELAY);
    APT_PARAM_CHECK_NO_RET(delayMode <= APT_VCAP_VCNT_DELAY_DIVIDE_32_SW_DELAY);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_VCAP_DLY.BIT.rg_vcap_dly_mode = delayMode;
    aptx->EM_VCAP_DLY.BIT.rg_vcap_swdly = swDelay;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get the delay value of valley capture, including capture count value and software delay value.
  * @param aptx APT register base address.
  * @retval unsigned short: Value of valley capture.
  */
static inline unsigned short DCL_APT_GetValleyCapDelay(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->EM_VCAP_STS1.BIT.ro_vcap_dly);
}

/**
  * @brief Get the count value between start edge and stop edge.
  * @param aptx APT register base address.
  * @retval unsigned short: Count value between start edge and stop edge.
  */
static inline unsigned short DCL_APT_GetValleyCapCount(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->EM_VCAP_STS1.BIT.ro_vcap_cnt);
}

/**
  * @brief Get valley capture status of start edge of stop edge.
  * @param aptx APT register base address.
  * @param edge Start or stop edge of valley capture.
  * @retval bool: true, false.
  */
static inline bool DCL_APT_GetValleyEdgeStatus(APT_RegStruct *aptx, APT_ValleyCountEdge edge)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(edge >= APT_VALLEY_COUNT_RISING_EDGE, false);
    APT_PARAM_CHECK_WITH_RET(edge <= APT_VALLEY_COUNT_FALLING_EDGE, false);
    if (edge == APT_VALLEY_COUNT_RISING_EDGE) {
        return (aptx->EM_VCAP_STS2.BIT.ro_vcap_sta_edgsts);
    } else {
        return (aptx->EM_VCAP_STS2.BIT.ro_vcap_stp_edgsts);
    }
}

/**
 * @brief Enable the Event Management submodule edge filter to generate events after configured number of edges.
 * @param aptx APT register base address.
 * @retval None.
 */
static inline void DCL_APT_EnableFilterDelay(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_FILT_CFG.BIT.rg_filt_dly_en = BASE_CFG_ENABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
 * @brief Disable the Event Management submodule edge filter.
 * @param aptx APT register base address.
 * @retval None.
 */
static inline void DCL_APT_DisableEdgeFilter(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_FILT_CFG.BIT.rg_filt_dly_en = BASE_CFG_DISABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Configure the edge filter of Event Management submodule.
  * @param aptx APT register base address.
  * @param edgeFiltMode Edge filter mode.
  * @param edgeCnt Edge count threshold of edge filter, range 4 bits.
  * @retval None.
  */
static inline void DCL_APT_SetEdgeFilter(APT_RegStruct *aptx,
                                         APT_EMEdgeFilterMode edgeFiltMode,
                                         unsigned short edgeCnt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(edgeFiltMode >= APT_EM_EDGEFILTER_MODE_RISING);
    APT_PARAM_CHECK_NO_RET(edgeFiltMode <= APT_EM_EDGEFILTER_MODE_BOTH);
    APT_PARAM_CHECK_NO_RET(edgeCnt <= EDGE_FILTER_EDGE_CNT_MAX);
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_FILT_CFG.BIT.rg_filt_edg_sel = edgeFiltMode;
    aptx->EM_FILT_CFG.BIT.rg_filt_edg_cnt = edgeCnt;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Enable time-base counter capture.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_EnableTimerCapture(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_TCAP_CFG.BIT.rg_tcap_en = BASE_CFG_ENABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable time-base counter capture.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_DisableTimerCapture(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_TCAP_CFG.BIT.rg_tcap_en = BASE_CFG_DISABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get the capture status of time-base counter.
  * @param aptx APT register base address.
  * @retval bool: true, false
  */
static inline bool DCL_APT_GetTimerCapStatus(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->EM_TCAP_CFG.BIT.ro_tcap_sts);
}

/**
  * @brief Clear the capture status of time-base counter.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_ClearTimerCapStatus(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->EM_TCAP_CFG.BIT.rg_tcap_clr = BASE_CFG_SET;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get buffer value of the time-base counter capture.
  * @param aptx APT register base address.
  * @retval unsigned short: Buffer value of the time-base counter capture
  */
static inline unsigned short DCL_APT_GetTimerCapBufferValue(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->EM_TCAP_VAL.BIT.ro_tcap_cnt_buf);
}

/**
  * @brief Get realtime value of the time-base counter capture.
  * @param aptx APT register base address.
  * @retval unsigned short: Realtime value of the time-base counter capture.
  */
static inline unsigned short DCL_APT_GetTimerCapRealTimeValue(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->EM_TCAP_VAL.BIT.ro_tcap_cnt_rt);
}

/* --------------------------------------------------------------------------------------------- */
/* Synchronize (SYNI) submodule Direct Configuration Layer functions -------------------------- */
/* --------------------------------------------------------------------------------------------- */

/**
  * @brief Select the sync-in source of slave APT module.
  * @param aptx APT register base address.
  * @param syncInSrc Sync-in source of slave APT module.
  * @retval None.
  */
static inline void DCL_APT_SelectSyncInPulseSrc(APT_RegStruct *aptx, APT_SyncInSrc syncInSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(syncInSrc >= APT_SYNCIN_SRC_APT0_SYNCOUT);
    APT_PARAM_CHECK_NO_RET(syncInSrc <= APT_SYNCIN_SRC_DISABLE);
    aptx->SYNI_CFG.BIT.rg_syni_sel = syncInSrc;
}

/**
  * @brief Get the flag of sync-in pulse.
  * @param aptx APT register base address.
  * @retval bool: true, false.
  */
static inline bool DCL_APT_GetSyncInPulseFlag(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return (aptx->SYNI_CFG.BIT.ro_syni_flag);
}

/**
  * @brief Clear the flag of sync-in pulse.
  * @param aptx APT register base address.
  * @retval None.
  */
static inline void DCL_APT_ClearSyncInPulseFlag(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->SYNI_CFG.BIT.rg_syni_clr = BASE_CFG_SET;
}

/**
 * @brief Set and Enable the synchronization source of the time-base counter.
 * @param aptx APT register base address.
 * @param cntrSyncSrc The selection of synchronization source for the time-base counter, it supports |.
 * @retval None.
 */
static inline void DCL_APT_SetTimeBaseCounterSyncSrc(APT_RegStruct *aptx, APT_cntrSyncEvent cntrSyncSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(cntrSyncSrc >= APT_CNTR_SYNC_SRC_COMBINE_EVENT_A1);
    APT_PARAM_CHECK_NO_RET(cntrSyncSrc <= APT_CNTR_SYNC_SRC_MAX_VALUE);
    aptx->SYNCNT_CFG.reg |= cntrSyncSrc;
}

/**
 * @brief Disable the synchronization source of the time-base counter.
 * @param aptx APT register base address.
 * @param cntrSyncSrc The selection of synchronization source for the time-base counter, it supports |.
 * @retval None.
 */
static inline void DCL_APT_DisableTimeBaseCounterSyncSrc(APT_RegStruct *aptx, APT_cntrSyncEvent cntrSyncSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(cntrSyncSrc >= APT_CNTR_SYNC_SRC_COMBINE_EVENT_A1);
    APT_PARAM_CHECK_NO_RET(cntrSyncSrc <= APT_CNTR_SYNC_SRC_MAX_VALUE);
    aptx->SYNCNT_CFG.reg &= (~cntrSyncSrc);
}

/**
 * @brief Set and Enable the source of synchronization out pulse.
 * @param aptx APT register base address.
 * @param syncOutSrc The source of synchronization out pulse, it supports |.
 * @retval None.
 */
static inline void DCL_APT_SetSyncOutPulseSrc(APT_RegStruct *aptx, APT_SyncOutSrc syncOutSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(syncOutSrc >= APT_SYNC_OUT_ON_CNTR_ZERO && syncOutSrc <= APT_SYNC_OUT_MAX_VALUE);
    aptx->SYNO_CFG.reg |= syncOutSrc;
}

/**
 * @brief Disable the source of synchronization out pulse.
 * @param aptx APT register base address.
 * @param syncOutSrc The source of synchronization out pulse, it supports |.
 * @retval None.
 */
static inline void DCL_APT_DisableSyncOutPulseSrc(APT_RegStruct *aptx, APT_SyncOutSrc syncOutSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(syncOutSrc >= APT_SYNC_OUT_ON_CNTR_ZERO && syncOutSrc <= APT_SYNC_OUT_MAX_VALUE);
    aptx->SYNO_CFG.reg &= (~syncOutSrc);
}

/**
  * @brief Set synchronization mode of master APT module.
  * @param aptx APT register base address.
  * @param syncOutMode Synchronization mode of master APT module.
  * @retval None.
  */
static inline void DCL_APT_SetSyncOutMode(APT_RegStruct *aptx, APT_SyncOutMode syncOutMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(syncOutMode >= APT_SYNCOUT_ONE_SHOT_MODE);
    APT_PARAM_CHECK_NO_RET(syncOutMode <= APT_SYNCOUT_MULTIPLE_MODE);
    aptx->SYNO_CFG.BIT.rg_mode_syno = syncOutMode;
}

/**
  * @brief Select the latch source of one-shot sync-out mode.
  * @param aptx APT register base address.
  * @param latSetSel Latch source of one-shot sync-out mode.
  * @retval None.
  */
static inline void DCL_APT_SelectSyncOutOneShotLatch(APT_RegStruct *aptx, APT_SyncOutLatSetSel latSetSel)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(latSetSel >= APT_SYNCOUT_LATCH_SET_ON_SW_FORCE);
    APT_PARAM_CHECK_NO_RET(latSetSel <= APT_SYNCOUT_LATCH_SET_ON_GLB_LOAD);
    aptx->SYNO_CFG.BIT.rg_latset_sel = latSetSel;
}

/**
 * @brief When in one-shot sync out mode and rg_latset_otsyn is selected as the latch set condition,
 *        this function is called to turn the one-shot latch condition ON.
 *        Upon occurrence of a chosen sync out source event, a sync out pulse is generated and the latch
 *        will be cleared. Hence writing 1 to rg_latset_otsyn will allow a sync out event to pass through
 *        and block other sync out source event.
 * @param aptx APT register base address.
 * @retval None.
 */
static inline void DCL_APT_SetSyncOutOneShotLatch(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->SYNO_CFG.BIT.rg_latset_otsyn = BASE_CFG_SET;
}


/**
 * @brief Select the peripheral synchronization output source.
 * @param aptx APT register base address.
 * @param APT_PeriphSyncOutSrc Source of peripheral synchronization output.
 * @retval None.
 */
static inline void DCL_APT_SetPeriphSyncOutSrc(APT_RegStruct *aptx, APT_PeriphSyncOutSrc perSyncOutSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(perSyncOutSrc >= APT_PERIPHERAL_SYNCOUT_DISABLE);
    APT_PARAM_CHECK_NO_RET(perSyncOutSrc <= APT_PERIPHERAL_SYNCOUT_CNTR_CMPD_DOWN);

    aptx->SYNP_CFG.BIT.rg_synp_sel = perSyncOutSrc;
}

/**
 * @brief Select the pulse that causes global buffer load.
 * @param aptx APT register base address.
 * @param glbLoadEvt The pulse that causes global buffer load, it supports |.
 * @retval None.
 */
static inline void DCL_APT_SetGlobalLoadEvent(APT_RegStruct *aptx, APT_GlbLoadEvent glbLoadEvt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(glbLoadEvt >= APT_GLB_LOAD_ON_CNTR_ZERO  && glbLoadEvt <= APT_GLB_LOAD_MAX_VALUE);
    aptx->GLB_LOAD.reg |= glbLoadEvt;
}

/**
 * @brief Disable the pulse that causes global buffer load.
 * @param aptx APT register base address.
 * @param glbLoadEvt The pulse that causes global buffer load, it supports |.
 * @retval None.
 */
static inline void DCL_APT_DisableGlobalLoadEvent(APT_RegStruct *aptx, APT_GlbLoadEvent glbLoadEvt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(glbLoadEvt >= APT_GLB_LOAD_ON_CNTR_ZERO  && glbLoadEvt <= APT_GLB_LOAD_MAX_VALUE);
    aptx->GLB_LOAD.reg &= (~glbLoadEvt);
}

/**
  * @brief Set the prescale value of multiple global buffer load mode.
  * @param aptx APT register base address.
  * @param gldCntPeriod Prescale value of multiple global buffer load mode, scale: 0x1 ~ 0xF.
  * @retval None.
  */
static inline void DCL_APT_SetGlobalLoadPrescale(APT_RegStruct *aptx, unsigned short gldCntPeriod)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(gldCntPeriod <= GLOBAL_LOAD_CNT_MAX);
    aptx->GLB_LOAD.BIT.rg_gld_prsc_prd = gldCntPeriod;
}

/**
  * @brief Set the global buffer load mode.
  * @param aptx APT register base address.
  * @param glbLoadMode Global buffer load mode.
  * @retval None.
  */
static inline void DCL_APT_SetGlobalLoadMode(APT_RegStruct *aptx, APT_GlobalLoadMode glbLoadMode)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(glbLoadMode >= APT_GLB_LOAD_ONE_SHOT_MODE);
    APT_PARAM_CHECK_NO_RET(glbLoadMode <= APT_GLB_LOAD_MULTIPLE_MODE);
    aptx->GLB_LOAD.BIT.rg_mode_gld = glbLoadMode;
}

/**
 * @brief When in one-shot global buffer load mode, this function is called to turn the one-shot latch condition ON.
 *        Upon occurrence of a chosen global buffer load event, the registers that is set to global buffer load mode
 *        will load the buffer, and the one-shot latch will be cleared. Hence writing 1 to rg_latset_otgld will
 *        allow a global buffer load event to pass through and block other global buffer load event.
 * @param aptx APT register base address.
 * @retval None.
 */
static inline void DCL_APT_SetGlobalLoadOneShotLatch(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    aptx->GLB_LOAD.BIT.rg_latset_otgld = BASE_CFG_SET;
}

/**
 * @brief Get buffer status of the registers that enable buffer load.
 * @param aptx The base address of APT module.
 * @param regBuf The buffer of the registers that enable buffer load, it supports |.
 * @retval true: The register buffer is full.
 * @retval false: The register buffer is not full.
 */
static inline bool DCL_APT_GetRegBufferStatus(APT_RegStruct *aptx, APT_RegBuffer regBuf)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(regBuf >= APT_REG_BUFFER_TC_PRD && regBuf <= APT_REG_BUFFER_MAX_VALUE, false);
    return ((aptx->LOAD_STS.reg & regBuf) == regBuf);
}

/**
  * @brief Generate a synchronization force event.
  * @param aptx The base address of APT module.
  * @param frcEvt Synchronization force event, it suports |.
  * @retval None.
  */
static inline void DCL_APT_ForceEvent(APT_RegStruct *aptx, APT_ForceEvtType frcEvt)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(frcEvt >= APT_FORCE_EVENT_COUNTER_SYNC && frcEvt <= APT_FORCE_EVENT_MAX_VALUE);
    aptx->SYN_FRC.reg |= frcEvt;
}

/**
  * @brief Enable the asynchronous buffer load function.
  * ShadowReg -- zero/prd/(condition) -- AsyncBufferSwitch(condition) -- ActiveReg
  * @param aptx The base address of APT module.
  * @param event Events that support asynchronous software buffer load, it suports |.
  * @retval None.
  */
static inline void DCL_APT_EnableAsyncBufferLoad(APT_RegStruct *aptx, APT_AsyncBufferLoadEvent event)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(event >= APT_ASYNC_EVENT_TC_BUF_INDEPENDENT_LOAD && event <= APT_ASYNC_EVENT_MAX_VALUE);
    /* 0 indicates enable/pass, default. */
    aptx->ASYNC_SWITCH.reg &= (~event);
}

/**
  * @brief Disable the asynchronous buffer load function.
  * ShadowReg -- zero/prd/(condition) -- AsyncBufferSwitch(condition) -- ActiveReg
  * @param aptx The base address of APT module.
  * @param event Events that support asynchronous software buffer load, it suports |.
  * @retval None.
  */
static inline void DCL_APT_DisableAsyncBufferLoad(APT_RegStruct *aptx, APT_AsyncBufferLoadEvent event)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(event >= APT_ASYNC_EVENT_TC_BUF_INDEPENDENT_LOAD && event <= APT_ASYNC_EVENT_MAX_VALUE);
    /* 1 indicates disable/disconnected */
    aptx->ASYNC_SWITCH.reg |= event;
}

/* --------------------------------------------------------------------------------------------- */
/* Status submodule Direct Configuration Layer functions -------------------------- */
/* --------------------------------------------------------------------------------------------- */

/**
  * @brief PWM Channel A output status.
  * @param aptx The base address of APT module.
  * @retval APT_PwmStatus: Status of PWM.
  */
static inline APT_PwmStatus DCL_APT_GetPWMAStatus(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    if (aptx->PWM_STATUS.BIT.ro_pwma_oen_status == BASE_CFG_DISABLE) {
        return aptx->PWM_STATUS.BIT.ro_pwma_status == 0? APT_PWM_LOW_LEVEL : APT_PWM_HIGH_LEVEL;
    }
    return APT_PWM_HIGH_RESISTANCE;
}

/**
  * @brief PWM Channel B output status.
  * @param aptx The base address of APT module.
  * @retval APT_PwmStatus: Status of PWM.
  */
static inline APT_PwmStatus DCL_APT_GetPWMBStatus(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    if (aptx->PWM_STATUS.BIT.ro_pwmb_oen_status == BASE_CFG_DISABLE) {
        return aptx->PWM_STATUS.BIT.ro_pwmb_status == 0? APT_PWM_LOW_LEVEL : APT_PWM_HIGH_LEVEL;
    }
    return APT_PWM_HIGH_RESISTANCE;
}

/**
  * @brief Get reference dot status.
  * @param aptx The base address of APT module.
  * @param refDot Reference dot.
  * @retval The level is inverted when the reference point is valid.
  */
static inline bool DCL_APT_GetRefDotStatus(APT_RegStruct *aptx, APT_RefDotSelect refDot)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(refDot >= APT_REFERENCE_DOTA, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(refDot <= APT_REFERENCE_DOTD, BASE_STATUS_ERROR);
    /* 16 is the bit shift of refdot */
    return (aptx->PWM_STATUS.reg & (0b1 << (16 + refDot)));
}

/**
  * @brief Status of the APT input TRIP1 event.
  * @param aptx The base address of APT module.
  * @retval APT_TripStatus: Low level, High level.
  */
static inline APT_TripStatus DCL_APT_GetTrip1Status(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->APT_TRIP_FLAG.BIT.ro_trip1_flag == 0? APT_TRIP_LOW_LEVEL : APT_TRIP_HIGH_LEVEL;
}

/**
  * @brief Status of the APT input TRIP2 event.
  * @param aptx The base address of APT module.
  * @retval APT_TripStatus: Low level, High level.
  */
static inline APT_TripStatus DCL_APT_GetTrip2Status(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->APT_TRIP_FLAG.BIT.ro_trip2_flag == 0? APT_TRIP_LOW_LEVEL : APT_TRIP_HIGH_LEVEL;
}

/**
  * @brief Status of the APT input TRIP3 event.
  * @param aptx The base address of APT module.
  * @retval APT_TripStatus: Low level, High level.
  */
static inline APT_TripStatus DCL_APT_GetTrip3Status(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->APT_TRIP_FLAG.BIT.ro_trip3_flag == 0? APT_TRIP_LOW_LEVEL : APT_TRIP_HIGH_LEVEL;
}

/* --------------------------------------------------------------------------------------------- */
/* High Resolution PWM (HRPWM) submodule Direct Configuration Layer functions ------------------ */
/* --------------------------------------------------------------------------------------------- */

/**
  * @brief Enable High resolution pwm.
  * @param aptx The base address of APT module.
  * @retval None.
  */
static inline void DCL_APT_EnableHRPWM(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->HR_CFG.BIT.rg_hr_en = BASE_CFG_ENABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Disable High resolution pwm.
  * @param aptx The base address of APT module.
  * @retval None.
  */
static inline void DCL_APT_DisableHRPWM(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    /* UNLOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_UNLOCK_REG_CMD;
    aptx->HR_CFG.BIT.rg_hr_en = BASE_CFG_DISABLE;
    /* LOCK */
    aptx->PROT_KEY.BIT.write_protect_key = APT_LOCK_REG_CMD;
}

/**
  * @brief Get high resolution realtime count.
  * @param aptx The base address of APT module.
  * @retval None.
  */
static inline unsigned char DCL_APT_GetHRCnt(APT_RegStruct *aptx)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    return aptx->HR_CNT.BIT.ro_hr_cnt;
}


/**
  * @brief Get the number of high resolution scales.
  * @param aptx The base address of APT module.
  * @param ch Select Apt channel.
  * @retval High resolution scales.
  */
static inline unsigned char DCL_APT_GetHrScales(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A, BASE_STATUS_ERROR);
    APT_PARAM_CHECK_WITH_RET(ch <= APT_PWM_CHANNEL_B, BASE_STATUS_ERROR);

    return ch == APT_PWM_CHANNEL_A? aptx->HR_SCALE_A.BIT.ro_hra_scale : aptx->HR_SCALE_B.BIT.ro_hrb_scale;
}

/* --------------------------------------------------------------------------------------------- */
/* Minimal DeadBand (MINDB) submodule Direct Configuration Layer functions ------------------ */
/* --------------------------------------------------------------------------------------------- */
/**
  * @brief Enable MINDB module.
  * @param aptx The base address of APT module.
  * @param ch Select apt channel.
  * @retval None.
  */
static inline void DCL_APT_EnableMDB(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Enable MDB of channel A. */
        aptx->MINDB_CFG.BIT.rg_mdb_en_a = BASE_CFG_SET;
    } else {
        /* Enable MDB of channel B. */
        aptx->MINDB_CFG.BIT.rg_mdb_en_b = BASE_CFG_SET;
    }
}

/**
  * @brief Disable MINDB module.
  * @param aptx The base address of APT module.
  * @param ch Select apt channel.
  * @retval None.
  */
static inline void DCL_APT_DisableMDB(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Disable MDB of channel A. */
        aptx->MINDB_CFG.BIT.rg_mdb_en_a = BASE_CFG_UNSET;
    } else {
        /* Disable MDB of channel B. */
        aptx->MINDB_CFG.BIT.rg_mdb_en_b = BASE_CFG_UNSET;
    }
}

/**
  * @brief Select the MDB reference signal of the channel x.
  * @param aptx The base address of APT module.
  * @param ch Select apt channel.
  * @param refSig Reference signal.
  * @retval None.
  */
static inline void DCL_APT_SetMDBRefSig(APT_RegStruct *aptx, APT_PWMChannel ch, APT_MDBRefSig refSig)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(refSig >= APT_MDB_REF_SIGNAL_PWM_LR);
    APT_PARAM_CHECK_NO_RET(refSig <= APT_MDB_REF_SIGNAL_MDB_XBAR15);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Set MDB reference signal of channel A. */
        aptx->MINDB_CFG.BIT.rg_mdb_sel_a = refSig;
    } else {
        /* Set MDB reference signal of channel B. */
        aptx->MINDB_CFG.BIT.rg_mdb_sel_b = refSig;
    }
}

/**
  * @brief Set the polarity of the channel x.
  * @param aptx The base address of APT module.
  * @param ch Select apt channel.
  * @param pol Not invert or invert.
  * @retval None.
  */
static inline void DCL_APT_SetMDBRefSigPolarity(APT_RegStruct *aptx, APT_PWMChannel ch, APT_MDBRefSigPol pol)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(pol >= APT_MDB_REF_SIGNAL_POLARITY_NOT_INVERT);
    APT_PARAM_CHECK_NO_RET(pol <= APT_MDB_REF_SIGNAL_POLARITY_INVERT);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Set ref signal polarity of channel A. */
        aptx->MINDB_CFG.BIT.rg_mdb_inv_a = pol;
    } else {
        /* Set ref signal polarity of channel B. */
        aptx->MINDB_CFG.BIT.rg_mdb_inv_b = pol;
    }
}

/**
  * @brief Set the block signal delay mode and delay time of the channel A/B.
  * @param aptx The base address of APT module.
  * @param ch Channel A or Channel B.
  * @param dlyMode Block signal delay mode.
  * @param delayVal Delay time, unit: apt_clk (0 ~ 65535).
  * @retval None.
  */
static inline void DCL_APT_SetMDBBlkSigDlyMode(APT_RegStruct *aptx, \
                                               APT_PWMChannel ch, \
                                               APT_MDBBlkSigDelayMode dlyMode, \
                                               unsigned short delayVal)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(dlyMode >= APT_MDB_BLOCK_SIGNAL_DELAY_MODE_FALLING_EDGE);
    APT_PARAM_CHECK_NO_RET(dlyMode <= APT_MDB_BLOCK_SIGNAL_DELAY_MODE_FILTER);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Set block signal delay mode of channel A. */
        aptx->MINDB_CFG.BIT.rg_mdb_dly_mode_a = dlyMode;
        aptx->MINDB_DLY.BIT.rg_mdb_dly_a = delayVal;
    } else {
        /* Set block signal delay mode of channel A. */
        aptx->MINDB_CFG.BIT.rg_mdb_dly_mode_b = dlyMode;
        aptx->MINDB_DLY.BIT.rg_mdb_dly_b = delayVal;
    }
}

/**
  * @brief Get the block A/B signal delay time of the channel A/B.
  * @param aptx The base address of APT module.
  * @param ch Channel A or Channel B.
  * @retval Delay time of block signal A/B.
  */
static inline unsigned short DCL_APT_GetMDBBlkSigDlyMode(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_WITH_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B, false);
    return (ch == APT_PWM_CHANNEL_A)? aptx->MINDB_DLY.BIT.rg_mdb_dly_a : aptx->MINDB_DLY.BIT.rg_mdb_dly_b;
}

/**
  * @brief Select the block signal A/B and the current PWM signal A/B for processing.
  * @param aptx The base address of APT module.
  * @param ch current PWM signal A/B.
  * @param blkSig Block signal A/B.
  * @retval None.
  */
static inline void DCL_APT_SelectMDBBlkSig(APT_RegStruct *aptx, APT_PWMChannel ch, APT_MDBBlkSig blkSig)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(blkSig >= APT_MDB_BLOCK_SIGNAL_A);
    APT_PARAM_CHECK_NO_RET(blkSig <= APT_MDB_BLOCK_SIGNAL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Select block signal of channel A. */
        aptx->MINDB_CFG.BIT.rg_mdb_selblock_a = (blkSig == APT_MDB_BLOCK_SIGNAL_A) ? BASE_CFG_UNSET : BASE_CFG_SET;
    } else {
        /* Select block signal of channel B. */
        aptx->MINDB_CFG.BIT.rg_mdb_selblock_b = (blkSig == APT_MDB_BLOCK_SIGNAL_B) ? BASE_CFG_UNSET : BASE_CFG_SET;
    }
}

/**
  * @brief Set MDB output signal polarity.
  * @param aptx The base address of APT module.
  * @param ch current PWM signal A/B.
  * @param outPol Output singnal polarity.
  * @retval None.
  */
static inline void DCL_APT_SetMDBOutSigPol(APT_RegStruct *aptx, APT_PWMChannel ch, APT_MDBOutSigPol outPol)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(outPol >= APT_MDB_OUTPUT_POLARITY_NOT_BLOCK_AND_PWM);
    APT_PARAM_CHECK_NO_RET(outPol <= APT_MDB_OUTPUT_POLARITY_BLOCK);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Output polarity of channel A. */
        aptx->MINDB_CFG.BIT.rg_mdb_polsel_a = outPol;
    } else {
        /* Output polarity of channel B. */
        aptx->MINDB_CFG.BIT.rg_mdb_polsel_b = outPol;
    }
}

/* --------------------------------------------------------------------------------------------- */
/* Output Look Up Table (OLUT) submodule Direct Configuration Layer functions ------------------ */
/* --------------------------------------------------------------------------------------------- */
/**
  * @brief Enable OLUT module.
  * @param aptx The base address of APT module.
  * @param ch Select apt channel.
  * @retval None.
  */
static inline void DCL_APT_EnableOLUT(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Enable output look up table of channel A. */
        aptx->OLUTA_CFG.BIT.rg_olut_en_a = BASE_CFG_SET;
    } else {
        /* Enable output look up table of channel B. */
        aptx->OLUTB_CFG.BIT.rg_olut_en_b = BASE_CFG_SET;
    }
}

/**
  * @brief Disable OLUT module.
  * @param aptx The base address of APT module.
  * @param ch Select apt channel.
  * @retval None.
  */
static inline void DCL_APT_DisableOLUT(APT_RegStruct *aptx, APT_PWMChannel ch)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Disable output look up table of channel A. */
        aptx->OLUTA_CFG.BIT.rg_olut_en_a = BASE_CFG_UNSET;
    } else {
        /* Disable output look up table of channel B. */
        aptx->OLUTB_CFG.BIT.rg_olut_en_b = BASE_CFG_UNSET;
    }
}

/**
  * @brief Set the XBAR input signal of IN3.
  * @param aptx The base address of APT module.
  * @param ch Select apt channel.
  * @param in3Sig Input signal of input3.
  * @retval None.
  */
static inline void DCL_APT_SetOLUTInput3Src(APT_RegStruct *aptx, APT_PWMChannel ch, XBAR_OLUTXbarEvent in3Sig)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A);
    APT_PARAM_CHECK_NO_RET(ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(in3Sig >= XBAR_OLUT_EVENT_OLUTXBAR1);
    APT_PARAM_CHECK_NO_RET(in3Sig <= XBAR_OLUT_EVENT_OLUTXBAR16);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Set olut-xbar input source3 of channel A. */
        aptx->OLUTA_CFG.BIT.rg_selxbar_a = in3Sig;
    } else {
        /* Set olut-xbar input source3 of channel B. */
        aptx->OLUTB_CFG.BIT.rg_selxbar_b = in3Sig;
    }
}

/**
  * @brief Set the output table for eight combinations of (in3, in2, in1).
  *        in3(selected): select from XBAR_OLUTXbarEvent.
  *        in2(fixed): APTx_MINDB of complementary channel B/A.
  *        in1(fixed): APTx_MINDB of current channel  A/B.
  * @param aptx The base address of APT module.
  * @param ch Select apt channel.
  * @param outTable Output table of eight combinations of (in3, in2, in1).
  *        (in3<<2 + in2<<1 + in1):  000  001  010  011  100  101  110  111
  *        output table index:        0    1    2    3    4    5    6    7
  *        set output table val:     0/1  0/1  0/1  0/1  0/1  0/1  0/1  0/1
  * @param length Length of the output lookup table.
  * @retval None.
  */
static inline void DCL_APT_SetOLUT(APT_RegStruct *aptx, \
                                   APT_PWMChannel ch, \
                                   bool outTable[APT_OLUT_OUTPUT_COMBINATION_MAX_NUM], \
                                   unsigned int length)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B);
    APT_PARAM_CHECK_NO_RET(length <= APT_OLUT_OUTPUT_COMBINATION_MAX_NUM);
    unsigned char tableVal = 0;
    /* Calculate output for eight combinations. */
    for (unsigned int i = 0; i < length; i++) {
        tableVal += (outTable[i] << i);
    }
    if (ch == APT_PWM_CHANNEL_A) {
        /* Set output for eight combinations of channel A. */
        aptx->OLUTA_CFG.BIT.rg_lutdec_a = tableVal;
    } else {
        /* Set output for eight combinations of channel B. */
        aptx->OLUTB_CFG.BIT.rg_lutdec_b = tableVal;
    }
}

/* --------------------------------------------------------------------------------------------- */
/* Link (LINK) submodule Direct Configuration Layer functions ---------------------------------- */
/* --------------------------------------------------------------------------------------------- */

/**
  * @brief Set link source of TC PERIOD.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcTcPeriod(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG0.BIT.rg_tc_prd_link = aptLinkSrc;
}

/**
  * @brief Set link source of TC reference dot A.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcTcRefA(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG0.BIT.rg_tc_refa_link = aptLinkSrc;
}

/**
  * @brief Set link source of TC reference dot B.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcTcRefB(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG0.BIT.rg_tc_refb_link = aptLinkSrc;
}

/**
  * @brief Set link source of TC reference dot C.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcTcRefC(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG0.BIT.rg_tc_refc_link = aptLinkSrc;
}

/**
  * @brief Set link source of TC reference dot D.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcTcRefD(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG0.BIT.rg_tc_refd_link = aptLinkSrc;
}

/**
  * @brief Set link source of SYN_FRC register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcSynFrc(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG0.BIT.rg_syn_frc_link = aptLinkSrc;
}

/**
  * @brief Set link source of DG rising edge.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcDgRed(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG1.BIT.rg_dg_red_link = aptLinkSrc;
}

/**
  * @brief Set link source of DG falling edge.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcDgFed(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG1.BIT.rg_dg_fed_link = aptLinkSrc;
}

/**
  * @brief Set link source of DG_CFG register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcDgCfg(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG1.BIT.rg_dg_cfg_link = aptLinkSrc;
}

/**
  * @brief Set link source of PC_CFG register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcPcCfg(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG1.BIT.rg_pc_cfg_link = aptLinkSrc;
}

/**
  * @brief Set link source of PB_CFG_A register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcPbCfgA(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG1.BIT.rg_pb_cfg_a_link = aptLinkSrc;
}

/**
  * @brief Set link source of PB_CFG_B register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcPbCfgB(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG1.BIT.rg_pb_cfg_b_link = aptLinkSrc;
}

/**
  * @brief Set link source of PG_ACT_A register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcPgActA(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG2.BIT.rg_pg_act_a_link = aptLinkSrc;
}

/**
  * @brief Set link source of PG_ACT_B register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcPgActB(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG2.BIT.rg_pg_act_b_link = aptLinkSrc;
}

/**
  * @brief Set link source of PG_ACT_FRC register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcPgActFrc(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG2.BIT.rg_pg_act_frc_link = aptLinkSrc;
}

/**
  * @brief Set link source of PG_OUT_FRC register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcPgOutFrc(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG2.BIT.rg_pg_out_frc_link = aptLinkSrc;
}

/**
  * @brief Set link source of OC_FRC_EVT register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcOcFrcEvt(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG2.BIT.rg_oc_frc_evt_link = aptLinkSrc;
}

/**
  * @brief Set link source of OC_SW_CLR register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcOcSwClr(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG2.BIT.rg_oc_sw_clr_link = aptLinkSrc;
}

/**
  * @brief Set link source of GLB_LOAD register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcGlbLoad(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG3.BIT.rg_glb_load_link = aptLinkSrc;
}

/**
  * @brief Set link source of ASYNC_SWITCH register.
  * @param aptx The base address of APT module.
  * @param aptLinkSrc Link source.
  * @retval None.
  */
static inline void DCL_APT_SetLinkSrcAsyncSwitch(APT_RegStruct *aptx, APT_LinkSrc aptLinkSrc)
{
    APT_ASSERT_PARAM(IsAPTInstance(aptx));
    APT_PARAM_CHECK_NO_RET(aptLinkSrc >= APT_LINK_SOURCE_APT0);
    APT_PARAM_CHECK_NO_RET(aptLinkSrc <= APT_LINK_DISABLE);
    aptx->LINK_CFG3.BIT.rg_async_switch_link = aptLinkSrc;
}


/* --------------------------------------------------------------------------------------------- */
/* Xbar (XBAR) submodule Direct Configuration Layer functions ------------------ */
/* --------------------------------------------------------------------------------------------- */

/**
  * @brief TRIP event definition.
  * */
typedef enum {
    XBAR_TRIP_EVENT_TRIP1  = 0x00000000U,
    XBAR_TRIP_EVENT_TRIP2,
    XBAR_TRIP_EVENT_TRIP3,
    XBAR_TRIP_EVENT_TRIP4,
    XBAR_TRIP_EVENT_TRIP5,
    XBAR_TRIP_EVENT_TRIP6,
    XBAR_TRIP_EVENT_TRIP7,
    XBAR_TRIP_EVENT_TRIP8,
    XBAR_TRIP_EVENT_TRIP9,
    XBAR_TRIP_EVENT_TRIP10,
    XBAR_TRIP_EVENT_TRIP11,
    XBAR_TRIP_EVENT_TRIP12,
    XBAR_TRIP_EVENT_TRIP13,
    XBAR_TRIP_EVENT_TRIP14,
    XBAR_TRIP_EVENT_TRIP15,
    XBAR_TRIP_EVENT_MAX_NUM,
} XBAR_TripEvent;


/**
  * @brief Trip xbar input source definition.
  * */
typedef union {
    struct {
        unsigned int tripXBarIn31_0;
        unsigned int tripXBarIn63_32;
    } tripXbar;

    struct {
        struct {
            unsigned int    poe0           : 1   ; /* trip xbar input0 */
            unsigned int    poe1           : 1   ; /* trip xbar input1 */
            unsigned int    poe2           : 1   ; /* trip xbar input2 */
            unsigned int    poe3           : 1   ; /* trip xbar input3 */
            unsigned int    poe4           : 1   ; /* trip xbar input4 */
            unsigned int    poe5           : 1   ; /* trip xbar input5 */
            unsigned int    poe6           : 1   ; /* trip xbar input6 */
            unsigned int    poe7           : 1   ; /* trip xbar input7 */
            unsigned int    poe8           : 1   ; /* trip xbar input8 */
            unsigned int    poe9           : 1   ; /* trip xbar input9 */
            unsigned int    poe10          : 1   ; /* trip xbar input10 */
            unsigned int    poe11          : 1   ; /* trip xbar input11 */
            unsigned int    poe12          : 1   ; /* trip xbar input12 */
            unsigned int    poe13          : 1   ; /* trip xbar input13 */
            unsigned int    poe14          : 1   ; /* trip xbar input14 */
            unsigned int    poe15          : 1   ; /* trip xbar input15 */
            unsigned int    acmp0_trip     : 1   ; /* trip xbar input16 */
            unsigned int    acmp1_trip     : 1   ; /* trip xbar input17 */
            unsigned int    acmp2_trip     : 1   ; /* trip xbar input18 */
            unsigned int    acmp3_trip     : 1   ; /* trip xbar input19 */
            unsigned int    acmp4_trip     : 1   ; /* trip xbar input20 */
            unsigned int    acmp5_trip     : 1   ; /* trip xbar input21 */
            unsigned int    acmp6_trip     : 1   ; /* trip xbar input22 */
            unsigned int    acmp7_trip     : 1   ; /* trip xbar input23 */
            unsigned int    timer0_soc     : 1   ; /* trip xbar input24 */
            unsigned int    timer1_soc     : 1   ; /* trip xbar input25 */
            unsigned int    timer2_soc     : 1   ; /* trip xbar input26 */
            unsigned int    timer3_soc     : 1   ; /* trip xbar input27 */
            unsigned int    timer4_soc     : 1   ; /* trip xbar input28 */
            unsigned int    timer5_soc     : 1   ; /* trip xbar input29 */
            unsigned int    gpt0_soc     : 1   ; /* trip xbar input30 */
            unsigned int    gpt1_soc     : 1   ; /* trip xbar input31 */
        } BIT31_0;

        struct {
            unsigned int    adc0_trip0     : 1   ; /* trip xbar input32 */
            unsigned int    adc0_trip1     : 1   ; /* trip xbar input33 */
            unsigned int    adc0_trip2     : 1   ; /* trip xbar input34 */
            unsigned int    adc0_trip3     : 1   ; /* trip xbar input35 */
            unsigned int    adc1_trip0     : 1   ; /* trip xbar input36 */
            unsigned int    adc1_trip1     : 1   ; /* trip xbar input37 */
            unsigned int    adc1_trip2     : 1   ; /* trip xbar input38 */
            unsigned int    adc1_trip3     : 1   ; /* trip xbar input39 */
            unsigned int    adc2_trip0     : 1   ; /* trip xbar input40 */
            unsigned int    adc2_trip1     : 1   ; /* trip xbar input41 */
            unsigned int    adc2_trip2     : 1   ; /* trip xbar input42 */
            unsigned int    adc2_trip3     : 1   ; /* trip xbar input43 */
            unsigned int    adc3_trip0     : 1   ; /* trip xbar input44 */
            unsigned int    adc3_trip1     : 1   ; /* trip xbar input45 */
            unsigned int    adc3_trip2     : 1   ; /* trip xbar input46 */
            unsigned int    adc3_trip3     : 1   ; /* trip xbar input47 */
            unsigned int    reserved48     : 1   ; /* trip xbar input48 */
            unsigned int    reserved49     : 1   ; /* trip xbar input49 */
            unsigned int    reserved50     : 1   ; /* trip xbar input50 */
            unsigned int    reserved51     : 1   ; /* trip xbar input51 */
            unsigned int    reserved52     : 1   ; /* trip xbar input52 */
            unsigned int    reserved53     : 1   ; /* trip xbar input53 */
            unsigned int    reserved54     : 1   ; /* trip xbar input54 */
            unsigned int    reserved55     : 1   ; /* trip xbar input55 */
            unsigned int    reserved56     : 1   ; /* trip xbar input56 */
            unsigned int    reserved57     : 1   ; /* trip xbar input57 */
            unsigned int    reserved58     : 1   ; /* trip xbar input58 */
            unsigned int    reserved59     : 1   ; /* trip xbar input59 */
            unsigned int    reserved60     : 1   ; /* trip xbar input60 */
            unsigned int    reserved61     : 1   ; /* trip xbar input61 */
            unsigned int    reserved62     : 1   ; /* trip xbar input62 */
            unsigned int    reserved63     : 1   ; /* trip xbar input63 */
        } BIT63_32;
    } BIT;
} XBAR_TripXbarEvt;

/**
  * @brief TRIP event output polarity definition.
  * */
typedef enum {
    XBAR_INPUT_POL_NOT_INVERT    = 0x00000000U,  /* NOT_INVERT */
    XBAR_INPUT_POL_INVERT        = 0x00000001U,  /* INVERT */
} XBAR_InputPolarity;

/**
  * @brief TRIP event output polarity definition.
  * */
typedef enum {
    XBAR_TRIP_X_POL_NOT_INVERT = 0x00000000U,
    XBAR_TRIP_X_POL_INVERT     = 0x00000001U,
} XBAR_TripOutPolarity;

/* Select one as the source for the MindB-XBARx. */
typedef enum {
    XBAR_MDB_SRC_DG_NO_HR_APT0_CHA  = 0x00000000U,
    XBAR_MDB_SRC_DG_NO_HR_APT0_CHB  = 0x00000001U,
    XBAR_MDB_SRC_DG_NO_HR_APT1_CHA  = 0x00000002U,
    XBAR_MDB_SRC_DG_NO_HR_APT1_CHB  = 0x00000003U,
    XBAR_MDB_SRC_DG_NO_HR_APT2_CHA  = 0x00000004U,
    XBAR_MDB_SRC_DG_NO_HR_APT2_CHB  = 0x00000005U,
    XBAR_MDB_SRC_DG_NO_HR_APT3_CHA  = 0x00000006U,
    XBAR_MDB_SRC_DG_NO_HR_APT3_CHB  = 0x00000007U,
    XBAR_MDB_SRC_DG_NO_HR_APT4_CHA  = 0x00000008U,
    XBAR_MDB_SRC_DG_NO_HR_APT4_CHB  = 0x00000009U,
    XBAR_MDB_SRC_DG_NO_HR_APT5_CHA  = 0x0000000AU,
    XBAR_MDB_SRC_DG_NO_HR_APT5_CHB  = 0x0000000BU,
    XBAR_MDB_SRC_DG_NO_HR_APT6_CHA  = 0x0000000CU,
    XBAR_MDB_SRC_DG_NO_HR_APT6_CHB  = 0x0000000DU,
    XBAR_MDB_SRC_DG_NO_HR_APT7_CHA  = 0x0000000EU,
    XBAR_MDB_SRC_DG_NO_HR_APT7_CHB  = 0x0000000FU,
    XBAR_MDB_SRC_DG_NO_HR_APT8_CHA  = 0x00000010U,
    XBAR_MDB_SRC_DG_NO_HR_APT8_CHB  = 0x00000011U,
    XBAR_MDB_SRC_DG_NO_HR_APT9_CHA  = 0x00000012U,
    XBAR_MDB_SRC_DG_NO_HR_APT9_CHB  = 0x00000013U,
    XBAR_MDB_SRC_DG_NO_HR_APT10_CHA = 0x00000014U,
    XBAR_MDB_SRC_DG_NO_HR_APT10_CHB = 0x00000015U,
    XBAR_MDB_SRC_DG_NO_HR_APT11_CHA = 0x00000016U,
    XBAR_MDB_SRC_DG_NO_HR_APT11_CHB = 0x00000017U,
    XBAR_MDB_SRC_DG_NO_HR_APT12_CHA = 0x00000018U,
    XBAR_MDB_SRC_DG_NO_HR_APT12_CHB = 0x00000019U,
    XBAR_MDB_SRC_DG_NO_HR_APT13_CHA = 0x0000001AU,
    XBAR_MDB_SRC_DG_NO_HR_APT13_CHB = 0x0000001BU,
    XBAR_MDB_SRC_DG_NO_HR_APT14_CHA = 0x0000001CU,
    XBAR_MDB_SRC_DG_NO_HR_APT14_CHB = 0x0000001DU,
    XBAR_MDB_SRC_DG_NO_HR_APT15_CHA = 0x0000001EU,
    XBAR_MDB_SRC_DG_NO_HR_APT15_CHB = 0x0000001FU,
} XBAR_MDBSrc;

/* Select one as the source for the OLUT-XBARx. */
typedef enum {
    XBAR_OLUT_SRC_MINDB_APT0_CHA = 0x00000000U,
    XBAR_OLUT_SRC_MINDB_APT0_CHB,
    XBAR_OLUT_SRC_MINDB_APT1_CHA,
    XBAR_OLUT_SRC_MINDB_APT1_CHB,
    XBAR_OLUT_SRC_MINDB_APT2_CHA,
    XBAR_OLUT_SRC_MINDB_APT2_CHB,
    XBAR_OLUT_SRC_MINDB_APT3_CHA,
    XBAR_OLUT_SRC_MINDB_APT3_CHB,
    XBAR_OLUT_SRC_MINDB_APT4_CHA,
    XBAR_OLUT_SRC_MINDB_APT4_CHB,
    XBAR_OLUT_SRC_MINDB_APT5_CHA,
    XBAR_OLUT_SRC_MINDB_APT5_CHB,
    XBAR_OLUT_SRC_MINDB_APT6_CHA,
    XBAR_OLUT_SRC_MINDB_APT6_CHB,
    XBAR_OLUT_SRC_MINDB_APT7_CHA,
    XBAR_OLUT_SRC_MINDB_APT7_CHB,
    XBAR_OLUT_SRC_MINDB_APT8_CHA,
    XBAR_OLUT_SRC_MINDB_APT8_CHB,
    XBAR_OLUT_SRC_MINDB_APT9_CHA,
    XBAR_OLUT_SRC_MINDB_APT9_CHB,
    XBAR_OLUT_SRC_MINDB_APT10_CHA,
    XBAR_OLUT_SRC_MINDB_APT10_CHB,
    XBAR_OLUT_SRC_MINDB_APT11_CHA,
    XBAR_OLUT_SRC_MINDB_APT11_CHB,
    XBAR_OLUT_SRC_MINDB_APT12_CHA,
    XBAR_OLUT_SRC_MINDB_APT12_CHB,
    XBAR_OLUT_SRC_MINDB_APT13_CHA,
    XBAR_OLUT_SRC_MINDB_APT13_CHB,
    XBAR_OLUT_SRC_MINDB_APT14_CHA,
    XBAR_OLUT_SRC_MINDB_APT14_CHB,
    XBAR_OLUT_SRC_MINDB_APT15_CHA,
    XBAR_OLUT_SRC_MINDB_APT15_CHB,
} XBAR_OLUTSrc;

/* ----------------------------------------------------------------------------------- */
/* REGISTER DEFINITION---------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */
typedef union {
    unsigned int reg;
    struct {
        unsigned int write_protect_key : 8   ; /* [7..0] */
        unsigned int reserved0         : 24  ; /* [31..8] */
    }BIT;
} volatile XBAR_PROT_KEY_REG;

typedef struct {
    unsigned int           TRIPx_IN31_0_CFG                 ; /* 0x100 */
    unsigned int           TRIPx_IN63_32_CFG                ; /* 0x104 */
    unsigned int           reserved_0[2]                   ; /* 0x108~0x10c */
} volatile TRIP_IN_CFG_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_trip1_inv          : 1   ; /* [0] */
        unsigned int    rg_trip2_inv          : 1   ; /* [1] */
        unsigned int    rg_trip3_inv          : 1   ; /* [2] */
        unsigned int    rg_trip4_inv          : 1   ; /* [3] */
        unsigned int    rg_trip5_inv          : 1   ; /* [4] */
        unsigned int    rg_trip6_inv          : 1   ; /* [5] */
        unsigned int    rg_trip7_inv          : 1   ; /* [6] */
        unsigned int    rg_trip8_inv          : 1   ; /* [7] */
        unsigned int    rg_trip9_inv          : 1   ; /* [8] */
        unsigned int    rg_trip10_inv         : 1   ; /* [9] */
        unsigned int    rg_trip11_inv         : 1   ; /* [10] */
        unsigned int    rg_trip12_inv         : 1   ; /* [11] */
        unsigned int    rg_trip13_inv         : 1   ; /* [12] */
        unsigned int    rg_trip14_inv         : 1   ; /* [13] */
        unsigned int    rg_trip15_inv         : 1   ; /* [14] */
        unsigned int    reserved0             : 17  ; /* [31..15] */
    } BIT;
} volatile TRIP_OUT_CFG_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_trip1_flag         : 1   ; /* [0] */
        unsigned int    rg_trip2_flag         : 1   ; /* [1] */
        unsigned int    rg_trip3_flag         : 1   ; /* [2] */
        unsigned int    rg_trip4_flag         : 1   ; /* [3] */
        unsigned int    rg_trip5_flag         : 1   ; /* [4] */
        unsigned int    rg_trip6_flag         : 1   ; /* [5] */
        unsigned int    rg_trip7_flag         : 1   ; /* [6] */
        unsigned int    rg_trip8_flag         : 1   ; /* [7] */
        unsigned int    rg_trip9_flag         : 1   ; /* [8] */
        unsigned int    rg_trip10_flag        : 1   ; /* [9] */
        unsigned int    rg_trip11_flag        : 1   ; /* [10] */
        unsigned int    rg_trip12_flag        : 1   ; /* [11] */
        unsigned int    rg_trip13_flag        : 1   ; /* [12] */
        unsigned int    rg_trip14_flag        : 1   ; /* [13] */
        unsigned int    rg_trip15_flag        : 1   ; /* [14] */
        unsigned int    reserved0             : 17  ; /* [31..15] */
    } BIT;
} volatile TRIP_FLAG_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_mdb_out_sel       : 5   ; /* [4..0] */
        unsigned int    reserved0             : 27  ; /* [31..5] */
    } BIT;
} volatile MINDB_OUT_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_olut_out_sel      : 5   ; /* [4..0] */
        unsigned int    reserved0             : 27  ; /* [31..5] */
    } BIT;
} volatile OLUT_OUT_REG;


typedef struct {
    unsigned int           TRIP_XBAR_IN31_0_CFG             ; /* 0x0 */
    unsigned int           TRIP_XBAR_IN63_32_CFG            ; /* 0x4 */
    unsigned int           reserved_0[58]                   ; /* 0x8~0xec */
    XBAR_PROT_KEY_REG      XBAR_PROT_KEY                    ; /* 0xf0 */
    unsigned int           reserved_1[3]                    ; /* 0xf4~0xfc */
    TRIP_IN_CFG_REG        TRIP_IN_FCG[15]                  ; /* 0x100~0x1ec */
    unsigned int           reserved_2[68]                   ; /* 0x1f0~0x2fc */
    TRIP_OUT_CFG_REG       TRIP_OUT_CFG                     ; /* 0x300 */
    TRIP_FLAG_REG          TRIP_FLAG                        ; /* 0x304 */
    unsigned int           reserved_3[318]                  ; /* 0x308~0x7fc */
    MINDB_OUT_REG          MINDB_OUT[15]                    ; /* 0x800~0x838 */
    unsigned int           reserved_4[49]                   ; /* 0x83c~0x8fc */
    OLUT_OUT_REG           OLUT_OUT[16]                     ; /* 0x900~0x93c */
} volatile XBAR_RegStruct;

/* --------------------------------------------------------------------------------------------- */
/* Xbar (XBAR) submodule Direct Configuration Layer functions ------------------ */
/* --------------------------------------------------------------------------------------------- */
/**
  * @brief Lock XBAR register which support write protect key.
  * @param xbar XBAR register base address.
  * @retval None.
  */
static inline void DCL_XBAR_LockReg(XBAR_RegStruct *xbar)
{
    APT_ASSERT_PARAM(IsXBARInstance(xbar));
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_LOCK_REG_CMD;
}

/**
  * @brief Lock XBAR register which support write protect key.
  * @param xbar XBAR register base address.
  * @retval None.
  */
static inline void DCL_XBAR_UnLockReg(XBAR_RegStruct *xbar)
{
    APT_ASSERT_PARAM(IsXBARInstance(xbar));
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_UNLOCK_REG_CMD;
}

/**
  * @brief Set the input polarity of 64 TRIP-XBAR channels.
  * @param xbarx The base address of XBAR module.
  * @param xbarEvt Slect 64 trip-xbar event, it supports |.
  * (1) Method 1: use bits to set the polarity
  *      0: means trip input-bitx NOT_INVERT(HIGH).
  *      1: means trip input-bitx INVERT(LOW).
  * (2) Method 2: Use two registers (tripXBarIn31_0 and tripXBarIn63_32) to set the polarity.
  * @param inPol Trip xbar input polarity.
  * @retval None.
  */
static inline void DCL_XBAR_SetTripXbarInPol(XBAR_RegStruct *xbar, XBAR_TripXbarEvt xbarEvt, XBAR_InputPolarity inPol)
{
    APT_ASSERT_PARAM(IsXBARInstance(xbar));
    APT_PARAM_CHECK_NO_RET(inPol >= XBAR_INPUT_POL_NOT_INVERT && inPol <= XBAR_INPUT_POL_INVERT);
    /* Unlock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_UNLOCK_REG_CMD;
    /* Clear bits */
    xbar->TRIP_XBAR_IN31_0_CFG   &= (~xbarEvt.tripXbar.tripXBarIn31_0);
    xbar->TRIP_XBAR_IN63_32_CFG  &= (~xbarEvt.tripXbar.tripXBarIn63_32);
    /* Set bits value */
    if (inPol == XBAR_INPUT_POL_INVERT) {
        xbar->TRIP_XBAR_IN31_0_CFG   |= xbarEvt.tripXbar.tripXBarIn31_0;
        xbar->TRIP_XBAR_IN63_32_CFG  |= xbarEvt.tripXbar.tripXBarIn63_32;
    }
    /* Lock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_LOCK_REG_CMD;
}

/**
  * @brief Set the input source of TRIPx.
  * @param xbarx The base address of XBAR module.
  * @param tripx Tripx event.
  * @param xbarEvt Selection of 64-channel xbar input source, it supports |.
  *                 each bit corresponding to one trip input.
  *                 0: Disable trip input-bitx.
  *                 1: Enable trip input-bitx.
  * @retval None.
  */
static inline void DCL_XBAR_SetTripxInSrc(XBAR_RegStruct *xbar, XBAR_TripEvent tripx, XBAR_TripXbarEvt xbarEvt)
{
    APT_ASSERT_PARAM(IsXBARInstance(xbar));
    APT_PARAM_CHECK_NO_RET(tripx >= XBAR_TRIP_EVENT_TRIP1);
    APT_PARAM_CHECK_NO_RET(tripx <= XBAR_TRIP_EVENT_TRIP15);
    /* Unlock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_UNLOCK_REG_CMD;
    /* Trip1~15 use index */
    xbar->TRIP_IN_FCG[tripx].TRIPx_IN31_0_CFG |= xbarEvt.tripXbar.tripXBarIn31_0;
    xbar->TRIP_IN_FCG[tripx].TRIPx_IN63_32_CFG |= xbarEvt.tripXbar.tripXBarIn63_32;
    /* Lock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_LOCK_REG_CMD;
}

/**
  * @brief Set output polarity of TRIP1-15.
  * @param xbarx The base address of XBAR module.
  * @param tripx TRIPx event.
  * @param tripPol Output polarity.
  * @retval None.
  */
static inline void DCL_XBAR_SetTripxOutPol(XBAR_RegStruct *xbar, XBAR_TripEvent tripx, XBAR_TripOutPolarity tripPol)
{
    APT_ASSERT_PARAM(IsXBARInstance(xbar));
    APT_PARAM_CHECK_NO_RET(tripx >= XBAR_TRIP_EVENT_TRIP1 && tripx <= XBAR_TRIP_EVENT_TRIP15);
    APT_PARAM_CHECK_NO_RET(tripPol >= XBAR_TRIP_X_POL_NOT_INVERT && tripPol <= XBAR_TRIP_X_POL_INVERT);
    /* Unlock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_UNLOCK_REG_CMD;
    if (tripPol == XBAR_TRIP_X_POL_INVERT) {
        /* Set trip out events polarity INVERT. */
        xbar->TRIP_OUT_CFG.reg |= (0x1 << tripx);
    } else {
        /* Set trip out events polarity NOT_INVERT. */
        xbar->TRIP_OUT_CFG.reg &= (~(0x1 << tripx));
    }
    /* Lock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_LOCK_REG_CMD;
}

/**
  * @brief Get whether the selected trips is triggered.
  * @param xbarx The base address of XBAR module.
  * @param tripx Tripx event.
  * @retval bool.
  *         false: tripx is not triggered.
  *         true:  tripx is triggered.
  */
static inline bool DCL_XBAR_GetTripxFlag(XBAR_RegStruct *xbar, XBAR_TripEvent tripx)
{
    APT_ASSERT_PARAM(IsXBARInstance(xbar));
    APT_PARAM_CHECK_WITH_RET(tripx >= XBAR_TRIP_EVENT_TRIP1, false);
    APT_PARAM_CHECK_WITH_RET(tripx <= XBAR_TRIP_EVENT_TRIP15, false);
    return ((xbar->TRIP_FLAG.reg >> tripx) & 0x1);
}

/**
  * @brief Get whether the selected trips is triggered.
  * @param xbarx The base address of XBAR module.
  * @param tripx Tripx event.
  * @retval None.
  */
static inline void DCL_XBAR_ClearTripxFlag(XBAR_RegStruct *xbar, XBAR_TripEvent tripx)
{
    APT_ASSERT_PARAM(IsXBARInstance(xbar));
    APT_PARAM_CHECK_NO_RET(tripx >= XBAR_TRIP_EVENT_TRIP1);
    APT_PARAM_CHECK_NO_RET(tripx <= XBAR_TRIP_EVENT_TRIP15);
    /* Unlock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_UNLOCK_REG_CMD;
    xbar->TRIP_FLAG.reg &= (~(0x1 << tripx));
    /* Lock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_LOCK_REG_CMD;
}

/**
  * @brief Set the input source of mdb-xbarx.
  * @param xbarx The base address of XBAR module.
  * @param mdbXbarx MDB Xbarx event.
  * @param mdbXbarSrc Input source of minimal deadband xbar.
  * @retval None.
  */
static inline void DCL_XBAR_SetMDBXbarInSrc(XBAR_RegStruct *xbar, XBAR_MDBXbarEvent mdbXbarx, XBAR_MDBSrc mdbXbarSrc)
{
    APT_ASSERT_PARAM(IsXBARInstance(xbar));
    APT_PARAM_CHECK_NO_RET(mdbXbarx >= XBAR_MDB_EVENT_MDBXBAR1);
    APT_PARAM_CHECK_NO_RET(mdbXbarx <= XBAR_MDB_EVENT_MDBXBAR15);
    APT_PARAM_CHECK_NO_RET(mdbXbarSrc >= XBAR_MDB_SRC_DG_NO_HR_APT0_CHA);
    APT_PARAM_CHECK_NO_RET(mdbXbarSrc <= XBAR_MDB_SRC_DG_NO_HR_APT15_CHB);
    /* Unlock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_UNLOCK_REG_CMD;
    /* Set MDB Xbar source. */
    xbar->MINDB_OUT[mdbXbarx].BIT.rg_mdb_out_sel = mdbXbarSrc;
    /* Lock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_LOCK_REG_CMD;
}

/**
  * @brief Set the input source of lookup table xbar.
  * @param xbarx The base address of XBAR module.
  * @param olutXbarx OLUT Xbar event.
  * @param olutXbarSrc Source of lookup table xbarx.
  * @retval None.
  */
static inline void DCL_XBAR_SetOLUTXbarInSrc(XBAR_RegStruct *xbar, \
                                             XBAR_OLUTXbarEvent olutXbarx, \
                                             XBAR_OLUTSrc olutXbarSrc)
{
    APT_ASSERT_PARAM(IsXBARInstance(xbar));
    APT_PARAM_CHECK_NO_RET(olutXbarx >= XBAR_OLUT_EVENT_OLUTXBAR1);
    APT_PARAM_CHECK_NO_RET(olutXbarx <= XBAR_OLUT_EVENT_OLUTXBAR16);
    APT_PARAM_CHECK_NO_RET(olutXbarSrc >= XBAR_OLUT_SRC_MINDB_APT0_CHA);
    APT_PARAM_CHECK_NO_RET(olutXbarSrc <= XBAR_OLUT_SRC_MINDB_APT15_CHB);
    /* Unlock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_UNLOCK_REG_CMD;
    /* Set OLUT Xbar source */
    xbar->OLUT_OUT[olutXbarx].BIT.rg_olut_out_sel = olutXbarSrc;
    /* Lock */
    xbar->XBAR_PROT_KEY.BIT.write_protect_key = XBAR_LOCK_REG_CMD;
}

/* --------------------------------------------------------------------------------- */
/* ADDL Module --------------------------------------------------------------------- */

/* ADDLL: All digital delay lock loop, used for hrpwm */
typedef enum {
    ADDLL_CLOCK_GATE_APT0  = 0x1,
    ADDLL_CLOCK_GATE_APT1  = 0x2,
    ADDLL_CLOCK_GATE_APT2  = 0x4,
    ADDLL_CLOCK_GATE_APT3  = 0x8,
    ADDLL_CLOCK_GATE_APT4  = 0x10,
    ADDLL_CLOCK_GATE_APT5  = 0x20,
    ADDLL_CLOCK_GATE_APT6  = 0x40,
    ADDLL_CLOCK_GATE_APT7  = 0x80,
    ADDLL_CLOCK_GATE_APT8  = 0x100,
    ADDLL_CLOCK_GATE_APT9  = 0x200,
    ADDLL_CLOCK_GATE_APT10 = 0x400,
    ADDLL_CLOCK_GATE_APT11 = 0x800,
    ADDLL_CLOCK_GATE_APT12 = 0x1000,
    ADDLL_CLOCK_GATE_APT13 = 0x2000,
    ADDLL_CLOCK_GATE_APT14 = 0x4000,
    ADDLL_CLOCK_GATE_APT15 = 0x8000,
    ADDLL_CLOCK_GATE_ALL = 0xFFFF,
} ADDLL_ClockGate;

typedef enum {
    ADDLL_FILTER_COEFF_1_DIV_2 = 0x0,
    ADDLL_FILTER_COEFF_1_DIV_4 = 0x1,
    ADDLL_FILTER_COEFF_1_DIV_8 = 0x2,
} ADDLL_FilterCoeff;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    addll_resetn           : 16  ; /* [15..0] */
        unsigned int    reserved0              : 16  ; /* [31..16] */
    } BIT;
} volatile ADDLL_CRG_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    addll_enable           : 16  ; /* [15..0] */
        unsigned int    reserved0              : 16  ; /* [31..16] */
    } BIT;
} volatile ADDLL_ENABLE_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    addll_a_track          : 16  ; /* [15..0] */
        unsigned int    addll_b_track          : 16  ; /* [31..16] */
    } BIT;
} volatile ADDLL_CFG_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    addll_lock             : 16  ; /* [15..0] */
        unsigned int    reserved0              : 16  ; /* [31..16] */
    } BIT;
} volatile ADDLL_LOCK_REG;

typedef union {
    unsigned int    reg;
    struct {
        unsigned int    rg_dll_iir_coef        : 2   ; /* [1..0] */
        unsigned int    reserved0              : 30  ; /* [31..2] */
    } BIT;
} volatile ADDLL_SCALE_CFG_REG;


typedef struct {
    unsigned int                      reserved0[48]         ; /* 0x14A1 FF00 - 0x14A1 FFBC */
    ADDLL_CRG_REG                     ADDLL_CRG             ; /* 0x14A1 FFC0 */
    ADDLL_ENABLE_REG                  ADDLL_ENABLE          ; /* 0x14A1 FFC4 */
    ADDLL_CFG_REG                     ADDLL_CFG             ; /* 0x14A1 FFC8 */
    unsigned int                      reserved1             ; /* 0x14A1 FFCC */
    ADDLL_LOCK_REG                    ADDLL_LOCK            ; /* 0x14A1 FFD0 */
    ADDLL_SCALE_CFG_REG               ADDLL_SCALE_CFG       ; /* 0x14A1 FFD4 */
} volatile ADDLL_RegStruct; /* Base Address: 0x14A1_FF00 */

/**
  * @brief ResetN the ADDL(all digital delay lock loop) clock gating of the APTx.
  * @param addll The base address of ADDLL module.
  * @param aptClkGate Clock gate of aptx, it supports |.
  * @retval None.
  */
static inline void DCL_ADDLL_ResetN(ADDLL_RegStruct *addll, ADDLL_ClockGate aptClkGate)
{
    APT_ASSERT_PARAM(IsADDLLInstance(addll));
    APT_PARAM_CHECK_NO_RET(aptClkGate >= ADDLL_CLOCK_GATE_APT0 && aptClkGate <= ADDLL_CLOCK_GATE_ALL);
    addll->ADDLL_CRG.reg |= aptClkGate;
}

/**
  * @brief Reset the ADDL(all digital delay lock loop) clock gating of the APTx.
  * @param addll The base address of ADDLL module.
  * @param aptClkGate Clock gate of aptx, it supports |.
  * @retval None.
  */
static inline void DCL_ADDLL_Reset(ADDLL_RegStruct *addll, ADDLL_ClockGate aptClkGate)
{
    APT_ASSERT_PARAM(IsADDLLInstance(addll));
    APT_PARAM_CHECK_NO_RET(aptClkGate >= ADDLL_CLOCK_GATE_APT0 && aptClkGate <= ADDLL_CLOCK_GATE_ALL);
    addll->ADDLL_CRG.reg &= ~aptClkGate;
}

/**
  * @brief Enable the ADDL(all digital delay lock loop) clock gating of the APTx.
  * @param addll The base address of ADDLL module.
  * @param aptClkGate Clock gate of aptx, it supports |.
  * @retval None.
  */
static inline void DCL_ADDLL_Enable(ADDLL_RegStruct *addll, ADDLL_ClockGate aptClkGate)
{
    APT_ASSERT_PARAM(IsADDLLInstance(addll));
    APT_PARAM_CHECK_NO_RET(aptClkGate >= ADDLL_CLOCK_GATE_APT0 && aptClkGate <= ADDLL_CLOCK_GATE_ALL);
    addll->ADDLL_ENABLE.reg |= aptClkGate;
}

/**
  * @brief Disable the ADDL(all digital delay lock loop) clock gating of the APTx.
  * @param addll The base address of ADDLL module.
  * @param aptClkGate Clock gate of aptx, it supports |.
  * @retval None.
  */
static inline void DCL_ADDLL_Disable(ADDLL_RegStruct *addll, ADDLL_ClockGate aptClkGate)
{
    APT_ASSERT_PARAM(IsADDLLInstance(addll));
    APT_PARAM_CHECK_NO_RET(aptClkGate >= ADDLL_CLOCK_GATE_APT0 && aptClkGate <= ADDLL_CLOCK_GATE_ALL);
    addll->ADDLL_ENABLE.reg &= ~aptClkGate;
}

/**
  * @brief Enable the ADDL online calibration of the APTx.
  * @param addll The base address of ADDLL module.
  * @param ch APT pwm channel.
  * @param aptClkGate Clock gate of aptx, it supports |.
  * @retval None.
  */
static inline void DCL_ADDLL_OnlineCalEnable(ADDLL_RegStruct *addll, APT_PWMChannel ch, ADDLL_ClockGate aptClkGate)
{
    APT_ASSERT_PARAM(IsADDLLInstance(addll));
    APT_PARAM_CHECK_NO_RET(aptClkGate >= ADDLL_CLOCK_GATE_APT0 && aptClkGate <= ADDLL_CLOCK_GATE_ALL);
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Enable online calibration of channel A. */
        addll->ADDLL_CFG.BIT.addll_a_track |= aptClkGate;
    } else {
        /* Enable online calibration of channel B. */
        addll->ADDLL_CFG.BIT.addll_b_track |= aptClkGate;
    }
}

/**
  * @brief Disable the ADDL online calibration of the APTx.
  * @param addll The base address of ADDLL module.
  * @param ch APT pwm channel.
  * @param aptClkGate Clock gate of aptx, it supports |.
  * @retval None.
  */
static inline void DCL_ADDLL_OnlineCalDisable(ADDLL_RegStruct *addll, APT_PWMChannel ch, ADDLL_ClockGate aptClkGate)
{
    APT_ASSERT_PARAM(IsADDLLInstance(addll));
    APT_PARAM_CHECK_NO_RET(aptClkGate >= ADDLL_CLOCK_GATE_APT0 && aptClkGate <= ADDLL_CLOCK_GATE_ALL);
    APT_PARAM_CHECK_NO_RET(ch >= APT_PWM_CHANNEL_A && ch <= APT_PWM_CHANNEL_B);
    if (ch == APT_PWM_CHANNEL_A) {
        /* Disable online calibration of channel A. */
        addll->ADDLL_CFG.BIT.addll_a_track &= ~aptClkGate;
    } else {
        /* Disable online calibration of channel B. */
        addll->ADDLL_CFG.BIT.addll_b_track &= ~aptClkGate;
    }
}

/**
  * @brief Get the ADDL lock status of the APTx when online calibration is finished.
  * @param addll The base address of ADDLL module.
  * @param aptClkGate Clock gate of aptx, it supports |.
  * @retval true: locked, false: not locked.
  */
static inline bool DCL_ADDLL_GetLockStatus(ADDLL_RegStruct *addll, ADDLL_ClockGate aptClkGate)
{
    APT_ASSERT_PARAM(IsADDLLInstance(addll));
    APT_PARAM_CHECK_WITH_RET(aptClkGate >= ADDLL_CLOCK_GATE_APT0 && aptClkGate <= ADDLL_CLOCK_GATE_ALL, false);

    return ((addll->ADDLL_LOCK.BIT.addll_lock & aptClkGate) == aptClkGate);
}

/**
  * @brief Set the ADDL filter coefficient.
  * @param addll The base address of ADDLL module.
  * @param fltCoeff Filter coefficient.
  * @retval None.
  */
static inline void DCL_ADDLL_SetFilterCoeff(ADDLL_RegStruct *addll, ADDLL_FilterCoeff fltCoeff)
{
    APT_ASSERT_PARAM(IsADDLLInstance(addll));
    APT_PARAM_CHECK_NO_RET(fltCoeff >= ADDLL_FILTER_COEFF_1_DIV_2 && fltCoeff <= ADDLL_FILTER_COEFF_1_DIV_8);

    addll->ADDLL_SCALE_CFG.BIT.rg_dll_iir_coef = fltCoeff;
}

/* --------------------------------------------------------------------------------- */
/* Moved from the apt.h------------------------------------------------------------- */
#define EM_OUT_EVT_FILTER_EN    0x0f
#define EM_CMB_EVT_NUM          4

/**
  * @defgroup APT APT
  * @brief APT module.
  * @{
  */


/**
  * @defgroup APT_Common APT Common
  * @brief APT common external module.
  * @{
  */

/**
  * @defgroup APT_Handle_Definition APT Handle Definition
  * @{
  */

/*
          Basic type     AHBL         ALBH         AHBH         ALBL
                         ___       __     __       ___       __     __
          ChannelA    __|   |__      |___|      __|   |__      |___|
                      __     __       ___          ___       __     __
          ChannelB      |___|      __|   |__    __|   |__      |___|
*/
/**
  * @brief Basic PWM waveform type.
  * @details waveform type:
  *             + APT_PWM_BASIC_A_HIGH_B_LOW -- Basic PWM waveform type 1.
  *             + APT_PWM_BASIC_A_LOW_B_HIGH -- Basic PWM waveform type 2.
  *             + APT_PWM_BASIC_A_HIGH_B_HIGH -- Basic PWM waveform type 3.
  *             + APT_PWM_BASIC_A_LOW_B_LOW -- Basic PWM waveform type 4.
  */
typedef enum {
    APT_PWM_BASIC_A_HIGH_B_LOW  = 0x00000000U,
    APT_PWM_BASIC_A_LOW_B_HIGH  = 0x00000001U,
    APT_PWM_BASIC_A_HIGH_B_HIGH = 0x00000002U,
    APT_PWM_BASIC_A_LOW_B_LOW   = 0x00000003U,
} APT_PWMBasicType;

/**
  * @brief The actual outputs of PWM channelA and channelB.
  * @details Output:
  *             + APT_PWM_OUT_BASIC_TYPE = 0x00000000U -- PWM channel output the waveform according to basic PWM type.
  *             + APT_PWM_OUT_ALWAYS_LOW = 0x00000001U -- PWM channel output low level.
  *             + APT_PWM_OUT_ALWAYS_HIGH = 0x00000002U -- PWM channel output high level.
  */
typedef enum {
    APT_PWM_OUT_BASIC_TYPE  = 0x00000000U,
    APT_PWM_OUT_ALWAYS_LOW  = 0x00000001U,
    APT_PWM_OUT_ALWAYS_HIGH = 0x00000002U,
} APT_PWMChannelOutType;

/**
  * @brief PWM waveform configuration handle of APT module.
  */
typedef struct {
    APT_PWMBasicType        basicType;        /**< Basic PWM waveform type. */
    APT_PWMChannelOutType   chAOutType;       /**< Actual output of PWM channelA. */
    APT_PWMChannelOutType   chBOutType;       /**< Actual output of PWM channelB. */
    APT_CountMode           cntMode;          /**< Count mode of APT time-base counter. */
    unsigned short          dividerFactor;    /**< Divider factor. The range is 0~4095. */
    unsigned short          timerPeriod;      /**< Count period of APT time-base timer. */
    unsigned short          divInitVal;       /**< Initial value of divider. */
    unsigned short          cntInitVal;       /**< Initial value of time-base counter */
    unsigned short          cntCmpLeftEdge;   /**< Count compare point of the left edge of PWM waveform. */
    unsigned short          cntCmpRightEdge;  /**< Count compare point of the right edge of PWM waveform. */
    APT_BufferLoadMode      cntCmpLoadMode;   /**< Buffer load mode of PWM waveform count compare value. */
    APT_CmpLoadEvent        cntCmpLoadEvt;    /**< Buffer load event of PWM waveform count compare value. */
    unsigned short          deadBandCnt;      /**< Count value of dead-band counter. In units of APT clock. */
} APT_PWMWaveForm;

/**
  * @brief ADC trigger configuration handle of APT module.
  */
typedef struct {
    bool                    trgEnSOCA;        /**< Enable of ADC trigger source SOCA. */
    APT_ADCTriggerSource    trgSrcSOCA;       /**< Source of ADC trigger source SOCA. */
    unsigned short          trgScaleSOCA;     /**< Scale of ADC trigger source SOCA. */
    unsigned short          cntCmpSOCA;       /**< Count compare point of ADC trigger source SOCA when using CMPA */
    bool                    trgEnSOCB;        /**< Enable of ADC trigger source SOCB. */
    APT_ADCTriggerSource    trgSrcSOCB;       /**< Source of ADC trigger source SOCB. */
    unsigned short          trgScaleSOCB;     /**< Scale of ADC trigger source SOCB. */
    unsigned short          cntCmpSOCB;       /**< Count compare point of ADC trigger source SOCB when using CMPB */
    APT_BufferLoadMode      cntCmpLoadMode;   /**< Buffer load mode of ADC trigger count compare value. */
    APT_CmpLoadEvent        cntCmpLoadEvt;    /**< Buffer load event of ADC trigger count compare value. */
} APT_ADCTrigger;

/**
  * @brief Timer interrupt configuration handle of APT module.
  */
typedef struct {
    bool                    tmrInterruptEn;     /**< Enable of APT module timer interrupt. */
    APT_TimerInterruptSrc   tmrInterruptSrc;    /**< Source of APT module timer interrupt. */
    unsigned short          tmrInterruptScale;  /**< Scale of APT module timer interrupt. */
} APT_TimerInterrupt;

/**
  * @brief Output control protection configuration handle of APT module.
  */
typedef struct {
    bool                    ocEventEn;        /**< Enable of output control event. */
    APT_OutCtrlEvent        ocEvent;          /**< Output control event. Limited to IO events or system events. */
    APT_OutCtrlMode         ocEventMode;      /**< Output control protection mode. */
    APT_OutCtrlCbcClrMode   cbcClrMode;       /**< Event clear mode when using cycle-by-cycle mode. */
    APT_OutCtrlAction       ocAction;         /**< Output control protection action. */
    APT_EmulationMode       emMode;           /**< emulation mode */
    bool                    ocEvtInterruptEn; /**< Enable of output control event interrupt. */
} APT_OutCtrlProtect;

/**
  * @brief System protect event;
  */
typedef enum {
    APT_SYS_EVT_1       = 0x00000100U,
    APT_SYS_EVT_2       = 0x00000200U,
    APT_SYS_EVT_3       = 0x00000400U,
    APT_SYS_EVT_4       = 0x00000800U,
} APT_SysOcEvent;

/**
  * @brief Output control protection configuration handle of APT module.
  */
typedef struct {
    bool                    ocEventEnEx;        /**< Oc event enable */
    APT_OutCtrlMode         ocEventModeEx;      /**< Output control protection mode. */
    APT_OutCtrlCbcClrMode   cbcClrModeEx;       /**< Event clear mode when using cycle-by-cycle mode. */
    APT_OutCtrlAction       ocActionAEx;         /**< Output control protection channel A action. */
    APT_OutCtrlAction       ocActionBEx;        /**< Output control protection channel B action. */
    APT_SysOcEvent          ocSysEvent;			/**< System protect event */
    APT_EMTripSrc           orgTripEvents;
} APT_OutCtrlProtectEx;

/**
  * @brief struct of EM conbine event
  */
typedef struct {
    APT_EMCombineEvtSrc     emEvtSrc;         /**< Combine event selection */
    APT_EMCombineEvtMode    emEvtCombineMode; /**< Event combine mode */
    APT_EMTripSrc           emEvtOrEnSrc;
} APT_CombineEvt;

/**
 * @brief Shield window and capture configurations
 */
typedef struct {
    bool                    wdEnable;         /**< Shield windows enable bit */
    bool                    emCapEnable;      /**< Enable EM captrue functions */
    APT_EMCombineEvent      eventSel;         /**< Window source event selection */
    APT_MaskWinResetMode    wdStartAndCapClr; /**< Window's offset start count and EM capture clear condition */
    unsigned short          wdOffset;         /**< Window's offset value */
    unsigned short          wdWidth;          /**< Window's width value */
    APT_MaskWinPolarity     wdPolar;          /**< Window's polarity */
} APT_WdAndCap;


/**
 * @brief Valley switch configurations
 */
typedef struct {
    bool                        vsEnable;       /**< Valley switch enable */
    APT_EMEdgeFilterMode        vsFilerEdgeSel; /**< Filter edge selection */
    unsigned char               vsFilterCnt;    /**< Filter edge count */
    APT_ValleyCapRstType        vsClrType;      /**< Clear type */
    APT_ValleyCountEdge         vsCapEdgeSel;   /**< Capture edge selection */
    unsigned char               vsCapStartEdge; /**< Capture start edge */
    unsigned char               vsCapEndEdge;   /**< Capture end edge */
    APT_ValleyDelayMode         vsCapDelayMode; /**< Capture delay mode */
    unsigned short              vsCapSoftDelay; /**< Capture software calibrate value */
} APT_ValleySw;

/**
  * @brief Event management handle of APT module
  */
typedef struct {
    bool                    emEnable;               /**< Enable bit of event management */
    APT_CombineEvt          emEvt[EM_CMB_EVT_NUM];  /**< Combine events configuration */
    APT_WdAndCap            emWdAndCap;             /**< Shield windows and capture configuration */
    APT_ValleySw            emValleySw;             /**< Valley switch configuration */
} APT_EventManage;

/**
  * @brief Synchronization handle of slave APT module.
  */
typedef struct {
    unsigned short          divPhase;         /**< Divider phase when receiving APT synchronization pulse. */
    unsigned short          cntPhase;         /**< Counter phase when receiving APT synchronization pulse. */
    APT_SyncCountMode       syncCntMode;      /**< Count mode when receiving APT synchronization pulse. */
    APT_SyncInSrc           syncInSrc;        /**< Sync-in source of APT module */
    unsigned short          cntrSyncSrc;
    /**< Sync-in source of time-base counter synchronization
         A logical OR of valid values can be passed as the cntrSyncSrc parameter.
         Valid values for cntrSyncSrc are:
              APT_CNTR_SYNC_SRC_COMBINE_EVENT_A1  - Enable combine event A1 as the counter synchronization source.
              APT_CNTR_SYNC_SRC_COMBINE_EVENT_B1  - Enable combine event B1 as the counter synchronization source.
              APT_CNTR_SYNC_SRC_SYNCIN            - Enable Sync-In source as the counter synchronization source. */
} APT_SlaveSyncIn;

/**
  * @brief Definition of callback function type.
  */
typedef void (* APT_CallbackType)(void *aptHandle);

/**
  * @brief Definition of callback function type.
  */
typedef struct {
    void (* TmrInterruptCallBack)(void *handle);

    void (* CbcEvtInterruptCallBack)(void *handle);
    void (* OshtEvtInterruptCallBack)(void *handle);
    void (* FreeEvtInterruptCallBack)(void *handle);
    void (* SysEvtInterruptCallBack)(void *handle);
    void (* EmA1EvtInterruptCallBack)(void *handle);
    void (* MaxCbcChAEvtInterruptCallBack)(void *handle);
    void (* MaxCbcChBEvtInterruptCallBack)(void *handle);
    void (* MaxFreeChAEvtInterruptCallBack)(void *handle);
    void (* MaxFreeChBEvtInterruptCallBack)(void *handle);
} APT_UserCallBack;

/**
  * @brief Definition of callback function ID.
  */
typedef enum {
    APT_TIMER_INTERRUPT              = 0x00000000U,

    APT_EVENT_INTERRUPT_CBC          = 0x00000001U,
    APT_EVENT_INTERRUPT_OSHT         = 0x00000002U,
    APT_EVENT_INTERRUPT_FREE         = 0x00000003U,
    APT_EVENT_INTERRUPT_SYSTEM       = 0x00000004U,
    APT_EVENT_INTERRUPT_EMA1         = 0x00000005U,
    APT_EVENT_INTERRUPT_CBC_MAX_CHA  = 0x00000006U,
    APT_EVENT_INTERRUPT_CBC_MAX_CHB  = 0x00000007U,
    APT_EVENT_INTERRUPT_FREE_MAX_CHA = 0x00000008U,
    APT_EVENT_INTERRUPT_FREE_MAX_CHB = 0x00000009U,
} APT_InterruputType;

/**
  * @brief Paramater definition of reference dot.
  */
typedef struct {
    unsigned int            refDotValue;
    unsigned int            refDotValueHr;
    APT_PWMChannel          pwmChannel;       /* PWM channel selection. */
    APT_PWMActionEvent      actionEvent;      /* Point triggle action event. */
    APT_PWMAction           action;           /* Point action. */
} APT_RefDotParameters;

/**
  * @brief APT Extra Handle.
  */
typedef struct {
} APT_ExtendHandle;

#endif /* McuMagicTag_APT_IP_H */
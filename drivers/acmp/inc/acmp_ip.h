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
  * @file    acmp_ip.h
  * @author  MCU Driver Team
  * @brief   ACMP module driver.
  *          This file provides DCL functions to manage ACMP and Definitions of specific parameters.
  *           + Definition of ACMP configuration parameters.
  *           + ACMP register mapping structure.
  *           + Parameters check functions.
  *           + Direct configuration layer interface.
  */

#ifndef McuMagicTag_ACMP_IP_H
#define McuMagicTag_ACMP_IP_H

#include "baseinc.h"

#ifdef ACMP_PARAM_CHECK
#define ACMP_ASSERT_PARAM BASE_FUNC_ASSERT_PARAM
#define ACMP_PARAM_CHECK_NO_RET BASE_FUNC_PARAMCHECK_NO_RET
#define ACMP_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
#define ACMP_ASSERT_PARAM(para) ((void)0U)
#define ACMP_PARAM_CHECK_NO_RET(para) ((void)0U)
#define ACMP_PARAM_CHECK_WITH_RET(para, ret) ((void)0U)
#endif

#define ACMP_FILTER_STEP_MAX_VALUE    0x0000FFFEU
#define ACMP_FILTER_CLK_MAX_VALUE     0x0000FFFFU
#define ACMP_FILTER_SAMPWIN_MAX_VALUE 0x000000FFU
#define ACMP_FILTER_THRESH_MAX_VALUE  0x000000FFU

/**
  * @addtogroup ACMP
  * @{
  */

/**
  * @defgroup ACMP_IP ACMP_IP
  * @brief ACMP_IP: acmp_v2.
  * @{
  */

/**
 * @defgroup ACMP_Param_Def ACMP Parameters Definition
 * @brief Definition of ACMP configuration parameters
 * @{
 */

/**
  * @brief Comparator blking source type
  * @details Description:
  *          + ACMP_BLKING_SRC_SOFT    ----    The software configuration masks the window.
  *          + ACMP_BLKING_SRC_APT0    ----    APT0 output mask window.
  *          + ACMP_BLKING_SRC_APT1    ----    APT1 output mask window.
  *          + ACMP_BLKING_SRC_APT2    ----    APT2 output mask window.
  *          + ACMP_BLKING_SRC_APT3    ----    APT3 output mask window.
  *          + ACMP_BLKING_SRC_APT4    ----    APT4 output mask window.
  *          + ACMP_BLKING_SRC_APT5    ----    APT5 output mask window.
  *          + ACMP_BLKING_SRC_APT6    ----    APT6 output mask window.
  *          + ACMP_BLKING_SRC_APT7    ----    APT7 output mask window.
  *          + ACMP_BLKING_SRC_APT8    ----    APT8 output mask window.
  *          + ACMP_BLKING_SRC_APT9    ----    APT9 output mask window.
  *          + ACMP_BLKING_SRC_APT10    ----   APT10 output mask window.
  *          + ACMP_BLKING_SRC_APT11    ----   APT11 output mask window.
  *          + ACMP_BLKING_SRC_APT12    ----   APT12 output mask window.
  *          + ACMP_BLKING_SRC_APT13    ----   APT13 output mask window.
  *          + ACMP_BLKING_SRC_APT14    ----   APT14 output mask window.
  *          + ACMP_BLKING_SRC_APT15    ----   APT15 output mask window.
  */
typedef enum {
    ACMP_BLKING_SRC_SOFT = 0x00000000U,
    ACMP_BLKING_SRC_APT0 = 0x00000001U,
    ACMP_BLKING_SRC_APT1 = 0x00000002U,
    ACMP_BLKING_SRC_APT2 = 0x00000003U,
    ACMP_BLKING_SRC_APT3 = 0x00000004U,
    ACMP_BLKING_SRC_APT4 = 0x00000005U,
    ACMP_BLKING_SRC_APT5 = 0x00000006U,
    ACMP_BLKING_SRC_APT6 = 0x00000007U,
    ACMP_BLKING_SRC_APT7 = 0x00000008U,
    ACMP_BLKING_SRC_APT8 = 0x00000009U,
    ACMP_BLKING_SRC_APT9 = 0x0000000AU,
    ACMP_BLKING_SRC_APT10 = 0x0000000BU,
    ACMP_BLKING_SRC_APT11 = 0x0000000CU,
    ACMP_BLKING_SRC_APT12 = 0x0000000DU,
    ACMP_BLKING_SRC_APT13 = 0x0000000EU,
    ACMP_BLKING_SRC_APT14 = 0x0000000FU,
    ACMP_BLKING_SRC_APT15 = 0x00000010U,
} ACMP_BlkingSrcType;

/**
  * @brief Comparator hysteresis voltage
  * @details Description:
  *          + ACMP_HYS_VOL_ZERO    ----    Hysteresis voltage 0 mv.
  *          + ACMP_HYS_VOL_20MV    ----    Hysteresis voltage 20 mv.
  *          + ACMP_HYS_VOL_30MV    ----    Hysteresis voltage 30 mv.
  */
typedef enum {
    ACMP_HYS_VOL_ZERO = 0x00000000U,
    ACMP_HYS_VOL_20MV = 0x00000002U,
    ACMP_HYS_VOL_30MV = 0x00000003U,
} ACMP_HystVol;

/**
  * @brief Initial filtering value
  * @details Description:
  *          + ACMP_FILTER_INIT_VAL_LOW    ----    The initial filtering value is low level.
  *          + ACMP_FILTER_INIT_VAL_HIGH    ----   The initial filtering value is high level.
  */
typedef enum {
    ACMP_FILTER_INIT_VAL_LOW    = 0x00000000U,
    ACMP_FILTER_INIT_VAL_HIGH   = 0x00000001U,
}ACMP_FilterInitVal;

/**
  * @brief Filter Input Select
  * @details Description:
  *          + ACMP_FILTER_INPUT_INTERNAL    ----   Internal comparator.
  *          + ACMP_FILTER_INPUT_EX1         ----   External comparator 1.
  *          + ACMP_FILTER_INPUT_EX2         ----   External comparator 2.
  *          + ACMP_FILTER_INPUT_EX3         ----   External comparator 3.
  *          + ACMP_FILTER_INPUT_EX4         ----   External comparator 4.
  *          + ACMP_FILTER_INPUT_EX5         ----   External comparator 5.
  *          + ACMP_FILTER_INPUT_EX6         ----   External comparator 6.
  *          + ACMP_FILTER_INPUT_EX7         ----   External comparator 7.
  */
typedef enum {
    ACMP_FILTER_INPUT_INTERNAL    = 0x00000000U,
    ACMP_FILTER_INPUT_EX1         = 0x00000001U,
    ACMP_FILTER_INPUT_EX2         = 0x00000002U,
    ACMP_FILTER_INPUT_EX3         = 0x00000003U,
    ACMP_FILTER_INPUT_EX4         = 0x00000004U,
    ACMP_FILTER_INPUT_EX5         = 0x00000005U,
    ACMP_FILTER_INPUT_EX6         = 0x00000006U,
    ACMP_FILTER_INPUT_EX7         = 0x00000007U,
    ACMP_FILTER_INPUT_MAX         = 0x00000008U,
}ACMP_FilterInputSel;

/**
  * @brief ACMP P port input select.
  * @details Description:
  *          + ACMP_INPUT_P_SELECT0    ----    Signal source PGA0_OUT.
  *          + ACMP_INPUT_P_SELECT1    ----    Signal source PGA1_OUT.
  *          + ACMP_INPUT_P_SELECT2    ----    From pin (GPIO0_5).
  *          + ACMP_INPUT_P_SELECT3    ----    From pin (GPIO2_5).
  *          + ACMP_INPUT_P_SELECT4    ----    From pin (GPIO3_5).
  *          + ACMP_INPUT_P_SELECT5    ----    Signal source DAC_OUT.
  */
typedef enum {
    ACMP_INPUT_P_SELECT0  = 0x00000000U,
    ACMP_INPUT_P_SELECT1  = 0x00000001U,
    ACMP_INPUT_P_SELECT2  = 0x00000002U,
    ACMP_INPUT_P_SELECT3  = 0x00000003U,
    ACMP_INPUT_P_SELECT4  = 0x00000004U,
    ACMP_INPUT_P_SELECT5  = 0x00000005U,
} ACMP_InputPSel;

/**
  * @brief ACMP N port input select.
  * @details Description:
  *          + ACMP_INPUT_N_SELECT0    ----     Signal source DAC_OUT.
  *          + ACMP_INPUT_N_SELECT1    ----     None.
  *          + ACMP_INPUT_N_SELECT2    ----     From pin (GPIO0_6).
  *          + ACMP_INPUT_N_SELECT3    ----     From pin (GPIO2_6).
  *          + ACMP_INPUT_N_SELECT4    ----     From pin (GPIO3_6).
  *          + ACMP_INPUT_N_SELECT5    ----     Signal source DAC_OUT.
  */
typedef enum {
    ACMP_INPUT_N_SELECT0  = 0x00000000U,
    ACMP_INPUT_N_SELECT1  = 0x00000001U,
    ACMP_INPUT_N_SELECT2  = 0x00000002U,
    ACMP_INPUT_N_SELECT3  = 0x00000003U,
    ACMP_INPUT_N_SELECT4  = 0x00000004U,
    ACMP_INPUT_N_SELECT5  = 0x00000005U,
} ACMP_InputNSel;

/**
  * @brief Comparator output polarity
  */
typedef enum {
    ACMP_OUT_NOT_INVERT = 0x00000000U,
    ACMP_OUT_INVERT     = 0x00000001U,
} ACMP_OutputPolarity;

/**
  * @brief ACMP output selection
  * @details Description:
  *          + ACMP_RESULT_ASYNCHRONOUS   ----     Asynchronous comparison results.
  *          + ACMP_RESULT_FILTER         ----     Digital Filtering Comparison Results.
  *          + ACMP_RESULT_LATCHING       ----     Filtering result after latching.
  *          + ACMP_RESULT_SYNCHRONIZE    ----     Synchronize the comparison result.
  */
typedef enum {
    ACMP_RESULT_ASYNCHRONOUS       = 0x00000000U,
    ACMP_RESULT_FILTER             = 0x00000001U,
    ACMP_RESULT_LATCHING           = 0x00000002U,
    ACMP_RESULT_SYNCHRONIZE        = 0x00000003U,
} ACMP_ResultSelect;

/**
  * @brief ACMP trip selection
  * @details Description:
  *          + ACMP_TRIP_ASYNCHRONOUS   ----     Asynchronous comparison results.
  *          + ACMP_TRIP_FILTER         ----     Digital Filtering Comparison Results.
  *          + ACMP_TRIP_LATCHING       ----     Filtering result after latching.
  *          + ACMP_TRIP_SYNCHRONIZE    ----     Synchronize the comparison result.
  */
typedef enum {
    ACMP_TRIP_ASYNCHRONOUS       = 0x00000000U,
    ACMP_TRIP_FILTER             = 0x00000001U,
    ACMP_TRIP_LATCHING           = 0x00000002U,
    ACMP_TRIP_SYNCHRONIZE        = 0x00000003U,
} ACMP_TripSelect;

/**
  * @brief Comparator filter mode
  * @details Description:
  *          + ACMP_FILTER_NONE       ----     Raw analog comparison.
  *          + ACMP_FILTER_BLOCK      ----     Latched function.
  *          + ACMP_FILTER_FILTER     ----     Filtering funciton.
  *          + ACMP_FILTER_BOTH       ----     Filtering and Blocking function.
  */
typedef enum {
    ACMP_FILTER_NONE     =   0x00000000U,
    ACMP_FILTER_FILTER   =   0x00000001U,
    ACMP_FILTER_LATCH    =   0x00000002U,
    ACMP_FILTER_BOTH     =   0x00000003U,
} ACMP_FilterMode;


/**
  * @brief Comparator filter control structure
  */
typedef struct {
    ACMP_FilterMode      filterMode;            /**< ACMP filter mode. */
    ACMP_BlkingSrcType   blkingSrcSelect;       /**< Blocking source select.*/
    ACMP_FilterInputSel  inputSel;              /**< Filter Input Select */
    unsigned short       filterClk;             /**< Filter sampling interval */
    unsigned char        filterSampwin;         /**< Filter sampling window size */
    unsigned char        filterThresh;          /**< Filtered voting threshold */
    bool                 filterInit;           /**< Initial filtering value. */
} ACMP_FilterCtrl;

/**
  * @brief Comparator input and output configuration structure
  */
typedef struct {
    ACMP_OutputPolarity    polarity;           /**< output polarity settings */
    bool                   asynEn;             /**< Enable asynchronous path */
    ACMP_InputPSel         inputPNum;          /**< ACMP input positive number */
    ACMP_InputNSel         inputNNum;          /**< ACMP input negative number */
} ACMP_InOutConfig;

/**
  * @brief ACMP user callback function type.
  */
typedef enum {
    ACMP_POS_INT          = 0x00000000U,
    ACMP_NEG_INT          = 0x00000001U,
    ACMP_EDGE_INT         = 0x00000002U,
} ACMP_CallBackFun_Type;

/**
  * @brief ACMP user interrupt callback function.
  */
typedef struct {
    void (* AcmpPositiveCallBack)(void *handle);   /**< Rising edge interrupt callback function. */
    void (* AcmpNegativeCallBack)(void *handle);   /**< Falling edge interrupt callback function. */
    void (* AcmpEdgedCallBack)(void *handle);      /**< Flip edge interrupt callback function. */
} ACMP_UserCallBack;

/**
  * @brief ACMP extend configure.
  */
typedef struct {
} ACMP_ExtendHandle;

/**
  * @}
  */

/**
  * @defgroup ACMP_REG_Definition ACMP Register Structure.
  * @brief ACMP Register Structure Definition.
  * @{
  */

/**
  * @brief ACMP control reg 0.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  da_acmp_enh        : 1;    /**< Comparator enable signal. */
        unsigned int  reserved_0         : 31;
    } BIT;
} volatile ACMP_CTRL_REG0;

/**
  * @brief ACMP control reg 1.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  da_acmp_input_psel  : 3;   /**< Input P vin selection */
        unsigned int  da_acmp_input_nsel  : 3;   /**< Input N vin selection */
        unsigned int  reserved_0          : 26;
    } BIT;
} volatile ACMP_CTRL_REG1;

/**
  * @brief ACMP control reg 2.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  cfg_acmp_out_sel     : 2;     /**< Comparator output result selection:
                                                        0: original comparison result;
                                                        1: result after filtering;
                                                        2: masked result;
                                                        3: reversed. */
        unsigned int  cfg_acmp_out_inv     : 1;     /**< Comparator result polarity selection:
                                                        0: The result is not reversed.
                                                        1: The result is reversed. */
        unsigned int  reserved_0           : 1;
        unsigned int  cfg_acmp_trip_sel    : 2;     /**< Trip Source Selection:
                                                        0: Asynchronous comparison results;
                                                        1: Filtering result;
                                                        2: Filtering result after latching;
                                                        3: Synchronize the comparison result. */
        unsigned int  reserved_1           : 2;
        unsigned int  cfg_acmp_out_asyn_en : 1;     /**< Enable asynchronous path
                                                        0: disabled;
                                                        1: enabled. */
        unsigned int reserved_2            : 23;
    } BIT;
} volatile ACMP_CTRL_REG2;

/**
 * @brief ACMP filtering frequency divider register.
 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int cfg_acmp_filter_clk     : 16;   /**< Filter sampling interval
                                                          The filtering sampling interval is cfg_acmp_filter_clk + 1. */
        unsigned int cfg_acmp_filter_sampwin : 8;    /**<  Filter sampling window size */
        unsigned int cfg_acmp_filter_thresh  : 8;    /**<  Filter majority voting threshold. The output changes only
                                                           when at least the opposite values of cfg_acmp_filter_thresh
                                                           appear in the sampling window.
                                                           Note: The majority voting threshold needs to be greater than
                                                           half the size of the sampling window. */
    } BIT;
} volatile ACMP_CTRL_FILTER_REG;

/**
  * @brief ACMP filtering control register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  cfg_acmp_filter_en       : 1;      /**< Comparator filtering enable:
                                                             0: disabled;
                                                             1: enabled. */
        unsigned int  cfg_acmp_filter_init_val : 1;      /**< Initial filtering value:
                                                              0: Low level;
                                                              1: High level */
        unsigned int  reserved_0               : 6;
        unsigned int  cfg_acmp_filter_sel      : 3;      /**< Filter Input Select:
                                                              000: internal comparator;
                                                              001: External comparator 1;
                                                              002: External comparator 2
                                                              ...
                                                              111: External comparator 7 */
        unsigned int  reserved_1              : 21;
    } BIT;
} volatile ACMP_CTRL_REG3;

/**
  * @brief ACMP mask control register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  cfg_acmp_blk_en         : 1;      /**< Comparator mask enable:
                                                             0: disabled;
                                                             1: enabled. */
        unsigned int  reserved_0              : 7;
        unsigned int  cfg_acmp_blk_sel        : 5;     /**< Comparator Mask Window Selection:
                                                            0x0: The window is masked by software.
                                                            0x1: APT0 output mask window;
                                                            0x2: APT1 output mask window;
                                                            ...
                                                            0x10: APT15 output mask window;
                                                            else: reversed. */
        unsigned int  reserved_1              : 3;
        unsigned int  cfg_acmp_blk_win        : 1;    /**< The software configuration mask window is displayed. */
        unsigned int  reserved_2              : 15;
    } BIT;
} volatile ACMP_CTRL_REG4;


/**
  * @brief ACMP interrupt raw status register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  intr_acmp_edge         : 1;    /**< Comparison result reversal edge interrupt status. */
        unsigned int  intr_acmp_neg          : 1;    /**< Comparison result falling edge interrupt status. */
        unsigned int  intr_acmp_pos          : 1;    /**< Interrupt status on the rising edge of comparison result. */
        unsigned int  reserved               : 29;
    } BIT;
} volatile ACMP_INTR_REG;


/**
  * @brief Masked ACMP interrupt status register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  intr_acmp_edge_msk   : 1;  /**< Status of comparison result reversal edge masked interrupt. */
        unsigned int  intr_acmp_neg_msk    : 1;  /**< Int status after falling edge of comparison result is masked. */
        unsigned int  intr_acmp_pos_msk    : 1;  /**< Int status after rising edge of comparison result is masked. */
        unsigned int  reserved             : 29;
    } BIT;
} volatile ACMP_INTR_MSK_REG;


/**
  * @brief ACMP interrupt mask.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  intr_acmp_edge_mask         : 1;  /**< Comparison result reversal edge interrupt mask register:
                                                              0: mask interrupts.
                                                              1: not masked. */
        unsigned int  intr_acmp_neg_mask          : 1;  /**< Comparison result falling edge interrupt mask register:
                                                              0: mask interrupts.
                                                              1: not masked. */
        unsigned int  intr_acmp_pos_mask          : 1;  /**<  Comparison result rising edge interrupt mask register:
                                                              0: mask interrupts.
                                                              1: not masked. */
        unsigned int  reserved                    : 29;
    } BIT;
} volatile ACMP_INTR_MASK_REG;


/**
  * @brief ACMP result register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  cmp_ana_rslt             : 1;      /**< Asynchronous comparison results. */
        unsigned int  cmp_filter_rslt          : 1;      /**< Filtering result. */
        unsigned int  cmp_latch_rslt           : 1;      /**< Filtering result after latching. */
        unsigned int  cmp_sync_rslt            : 1;      /**< Synchronize the comparison result. */
        unsigned int  reserved                 : 28;
    } BIT;
} volatile ACMP_RSLT_REG;

/**
  * @brief ACMP result clear register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  cmp_latch_sw_clr          : 1;      /**< The software clears the filtering result:
                                                               0: No effect
                                                               1: Generates pulses, resets the filter result and the
                                                                  latched filter result. */
        unsigned int  cmp_latch_synp_clr        : 1;      /**< APTSYNCP Clear Filtering Result:
                                                               0: disabled
                                                               1: enabled */
        unsigned int  reserved                  : 30;
    } BIT;
} volatile ACMP_RSLT_CLR_REG;

/**
  * @brief ACMP enable delay register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  cfg_acmp_en_dly          : 8;      /**< Indicates the delay for enabling ACMP (us). */
        unsigned int  reserved                 : 24;
    } BIT;
} volatile ACMP_EN_DLY_REG;

/**
  * @brief ACMP test register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  da_acmp_test_enh          : 1;      /**< Test enable signal:
                                                               0: disabled;
                                                               1: enabled. */
        unsigned int  da_acmp_test_sel          : 4;      /**< Test signal strobe. */
        unsigned int  reserved                  : 27;
    } BIT;
} volatile ACMP_TEST_REG;


/**
  * @brief ACMP TRIM register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  da_acmp_trim              : 8;      /**< ACMP TIRM register. */
        unsigned int  reserved                  : 24;
    } BIT;
} volatile ACMP_TRIM_REG;


/**
  * @brief ACMP reserved register.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int  da_acmp_rsv               : 2;      /**< Reserved comparator register:
                                                               <1: 0>: The hysteresis voltage is selected. */
        unsigned int  reserved                  : 30;
    } BIT;
} volatile ACMP_RSV_REG;

/**
  * @brief ACMP registers definition structure.
  */
typedef struct _ACMP_RegStruct {
    ACMP_CTRL_REG0        ACMP_CTRL0;       /**< ACMP control register 0. Offset address: 0x00000000U. */
    ACMP_CTRL_REG1        ACMP_CTRL1;       /**< ACMP control register 1. Offset address: 0x00000004U. */
    ACMP_CTRL_REG2        ACMP_CTRL2;       /**< ACMP control register 2. Offset address: 0x00000008U. */
    char                  space0[48];
    ACMP_CTRL_FILTER_REG  ACMP_CTRL_FILTER; /**< ACMP filtering frequency register. Offset address: 0x0000003CU. */
    ACMP_CTRL_REG3        ACMP_CTRL3;       /**< ACMP filtering control register. Offset address: 0x00000040U. */
    ACMP_CTRL_REG4        ACMP_CTRL4;       /**< ACMP mask control register. Offset address: 0x00000044U. */
    char                  space1[8];
    ACMP_INTR_REG         ACMP_INTR;        /**< ACMP interrupt raw status register. Offset address: 0x00000050U. */
    ACMP_INTR_MSK_REG     ACMP_INTR_MSK;    /**< Masked ACMP interrupt status register. Offset address: 0x00000054U. */
    ACMP_INTR_MASK_REG    ACMP_INTR_MASK;   /**< ACMP interrupt mask register. Offset address: 0x00000058U. */
    char                  space2[20];
    ACMP_RSLT_REG         ACMP_RSLT;        /**< ACMP result register. Offset address: 0x00000070U. */
    ACMP_RSLT_CLR_REG     ACMP_RSLT_CLR;    /**< ACMP result clear register. Offset address: 0x00000074U. */
    char                  space3[8];
    ACMP_EN_DLY_REG       ACMP_EN_DLY;      /**< ACMP enable delay register. Offset address: 0x00000080U. */
    char                  space4[4];
    ACMP_TRIM_REG         ACMP_TRIM;        /**< ACMP TRIM register. Offset address: 0x00000088U. */
    ACMP_RSV_REG          ACMP_RSV;         /**< ACMP reserved register. Offset address: 0x0000008CU. */
} volatile ACMP_RegStruct;

/* Parameter Check------------------------------------------------------------------ */
/**
  * @brief Verify ACMP output polarity configuration.
  * @param polarity: ACMP output polarity
  * @retval true
  * @retval false
  */
static inline bool IsACMPOutputPolarity(ACMP_OutputPolarity polarity)
{
    return ((polarity == ACMP_OUT_NOT_INVERT) || (polarity == ACMP_OUT_INVERT));
}

/**
  * @brief Verify ACMP input P number.
  * @param pNumber: ACMP output source select
  * @retval true
  * @retval false
  */
static inline bool IsACMPInputPNumber(ACMP_InputPSel pNumber)
{
    return (pNumber <= ACMP_INPUT_P_SELECT5);
}

/**
  * @brief Verify ACMP input N number.
  * @param NNumber: ACMP output source select
  * @retval true
  * @retval false
  */
static inline bool IsACMPInputNNumber(ACMP_InputNSel NNumber)
{
    return (NNumber <= ACMP_INPUT_N_SELECT5);
}

/**
  * @brief Verify ACMP blocking source type.
  * @param BlkingSrcType: ACMP output source select
  * @retval true
  * @retval false
  */
static inline bool IsACMPBlkingSrcType(ACMP_BlkingSrcType BlkingSrcType)
{
    return (BlkingSrcType <= ACMP_BLKING_SRC_APT15);
}

/**
  * @brief Verify ACMP IO output result selection
  * @param resultSelection: ACMP output source selection.
  * @retval true
  * @retval false
  */
static inline bool IsACMPResultSeletion(ACMP_ResultSelect resultSelection)
{
    return (resultSelection <= ACMP_RESULT_SYNCHRONIZE);
}

/**
  * @brief Verify ACMP Trip output result selection
  * @param resultSelection: ACMP output source selection.
  * @retval true
  * @retval false
  */
static inline bool IsACMPTripResultSeletion(ACMP_TripSelect resultSelection)
{
    return (resultSelection <= ACMP_RESULT_SYNCHRONIZE);
}

/* Direct configuration layer ------------------------------------------------*/
/**
  * @brief Set input switch
  * @param acmpx: ACMP register base address.
  * @param inputP: ACMP inputP selection. @ref ACMP_VinSel
  * @param inputN: ACMP inputN selection. @ref ACMP_VinSel
  * @retval None.
  */
static inline void DCL_ACMP_SetInputSwith(ACMP_RegStruct *acmpx, ACMP_InputPSel inputP, ACMP_InputNSel inputN)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(inputP >= ACMP_INPUT_P_SELECT0);
    ACMP_PARAM_CHECK_NO_RET(inputP <= ACMP_INPUT_P_SELECT5);
    ACMP_PARAM_CHECK_NO_RET(inputN >= ACMP_INPUT_N_SELECT0);
    ACMP_PARAM_CHECK_NO_RET(inputN <= ACMP_INPUT_N_SELECT5);
    acmpx->ACMP_CTRL1.BIT.da_acmp_input_nsel = inputN;  /* Input port on the P side. */
    acmpx->ACMP_CTRL1.BIT.da_acmp_input_psel = inputP;  /* Input port on the N side. */
}

/**
  * @brief ACMP output(deshark and synchronize) source.
  * @param acmp: ACMP register base address.
  * @param resultSelection: config value. @ref ACMP_ResultSelect
  * @retval None.
  */
static inline void DCL_ACMP_SetCmpOutputSrc(ACMP_RegStruct *acmpx, ACMP_ResultSelect resultSelection)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(resultSelection >= ACMP_RESULT_ASYNCHRONOUS);
    ACMP_PARAM_CHECK_NO_RET(resultSelection <= ACMP_RESULT_SYNCHRONIZE);
    acmpx->ACMP_CTRL2.BIT.cfg_acmp_out_sel = resultSelection;   /* ACMP output result select. */
}

/**
  * @brief ACMP_TRI Source Selection.
  * @param acmp: ACMP register base address.
  * @param tripSelection: config value. @ref ACMP_TripSelect
  * @retval None.
  */
static inline void DCL_ACMP_SetCmpTripOutputSrc(ACMP_RegStruct *acmpx, ACMP_TripSelect tripSelection)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(tripSelection >= ACMP_TRIP_ASYNCHRONOUS);
    ACMP_PARAM_CHECK_NO_RET(tripSelection <= ACMP_TRIP_SYNCHRONIZE);
    acmpx->ACMP_CTRL2.BIT.cfg_acmp_trip_sel = tripSelection;   /* ACMP output trip select. */
}

/**
  * @brief Enable asynchronous path.
  * @param acmp: ACMP register base address.
  * @retval None.
  */
static inline void DCL_ACMP_EnableAsynOut(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_CTRL2.BIT.cfg_acmp_out_asyn_en = BASE_CFG_ENABLE;
}

/**
  * @brief Disable asynchronous path.
  * @param acmp: ACMP register base address.
  * @retval None.
  */
static inline void DCL_ACMP_DisableAsynOut(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_CTRL2.BIT.cfg_acmp_out_asyn_en = BASE_CFG_DISABLE;
}

/**
  * @brief Setting the Filtering Sampling Interval.
  * @param acmp: ACMP register base address.
  * @param  clk: Filter sampling interval.
  * @retval None.
  */
static inline void DCL_ACMP_SetFilterSamplingInterval(ACMP_RegStruct *acmpx, unsigned int clk)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(clk <= ACMP_FILTER_CLK_MAX_VALUE);
    acmpx->ACMP_CTRL_FILTER.BIT.cfg_acmp_filter_clk = clk;
}

/**
  * @brief Setting the Filtering Sampling window.
  * @param acmp: ACMP register base address.
  * @param  win: Filter sampling window.
  * @retval None.
  */
static inline void DCL_ACMP_SetFilterSamplingWin(ACMP_RegStruct *acmpx, unsigned int win)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(win <= ACMP_FILTER_SAMPWIN_MAX_VALUE);
    acmpx->ACMP_CTRL_FILTER.BIT.cfg_acmp_filter_sampwin = win;
}

/**
  * @brief Setting the Filtering Sampling thresh.
  * @param acmp: ACMP register base address.
  * @param  thresh: Filter sampling thresh.
  * @retval None.
  */
static inline void DCL_ACMP_SetFilterSamplingThresh(ACMP_RegStruct *acmpx, unsigned int thresh)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(thresh <= ACMP_FILTER_THRESH_MAX_VALUE);
    acmpx->ACMP_CTRL_FILTER.BIT.cfg_acmp_filter_thresh = thresh;
}

/**
  * @brief Enable filter.
  * @param acmp: ACMP register base address.
  * @retval None.
  */
static inline void DCL_ACMP_FilterEnable(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_CTRL3.BIT.cfg_acmp_filter_en = BASE_CFG_ENABLE;
}

/**
  * @brief Disable filter.
  * @param acmp: ACMP register base address.
  * @retval None.
  */
static inline void DCL_ACMP_FilterDisable(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_CTRL3.BIT.cfg_acmp_filter_en = BASE_CFG_DISABLE;
}

/**
  * @brief Set the filter initial value.
  * @param acmp: ACMP register base address.
  * @param val: Filter initVal.
  * @retval None.
  */
static inline void DCL_ACMP_SetFilterInitialValue(ACMP_RegStruct *acmpx, ACMP_FilterInitVal val)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(val >= ACMP_FILTER_INIT_VAL_LOW);
    ACMP_PARAM_CHECK_NO_RET(val <= ACMP_FILTER_INIT_VAL_HIGH);
    acmpx->ACMP_CTRL3.BIT.cfg_acmp_filter_init_val = val;
}

/**
  * @brief Set the Filter Input Select.
  * @param acmp: ACMP register base address.
  * @param sel: Filtered Input Comparator Select.
  * @retval None.
  */
static inline void DCL_ACMP_SetFilterInputSelect(ACMP_RegStruct *acmpx, ACMP_FilterInputSel sel)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(sel >= ACMP_FILTER_INPUT_INTERNAL);
    ACMP_PARAM_CHECK_NO_RET(sel < ACMP_FILTER_INPUT_MAX);
    acmpx->ACMP_CTRL3.BIT.cfg_acmp_filter_sel = sel;
}

/**
  * @brief  Comparator enable blking function
  * @param  acmpx: ACMP register base address.
  * @retval None.
  */
static inline void DCL_ACMP_EnableCmpBlking(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_CTRL4.BIT.cfg_acmp_blk_en = BASE_CFG_ENABLE;
}

/**
  * @brief  Comparator disable blking function
  * @param acmpx: ACMP register base address.
  * @retval None.
  */
static inline void DCL_ACMP_DisableCmpBlking(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_CTRL4.BIT.cfg_acmp_blk_en  = BASE_CFG_DISABLE;
}

/**
  * @brief Enable the software masking window.
  * @param  acmpx: ACMP register base address.
  * @retval None.
  */
static inline void DCL_ACMP_EnableSoftBlking(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_CTRL4.BIT.cfg_acmp_blk_win = BASE_CFG_DISABLE;
}

/**
  * @brief Disable the software masking window.
  * @param acmpx: ACMP register base address.
  * @retval None.
  */
static inline void DCL_ACMP_DisableSoftBlking(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_CTRL4.BIT.cfg_acmp_blk_win = BASE_CFG_ENABLE;
}

/**
  * @brief Set blking source.
  * @param acmpx: ACMP register base address.
  * @param source: Source of blking. @ref ACMP_BlkingSrcType
  * @retval None.
  */
static inline void DCL_ACMP_SetCmpBlkingSource(ACMP_RegStruct *acmpx, ACMP_BlkingSrcType source)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(source >= ACMP_BLKING_SRC_SOFT);
    ACMP_PARAM_CHECK_NO_RET(source <= ACMP_BLKING_SRC_APT15);
    acmpx->ACMP_CTRL4.BIT.cfg_acmp_blk_sel = source;
}

/**
  * @brief Set comparator hysteresis voltage.
  * @param acmpx: ACMP register base address.
  * @param volSelect: Hysteresis voltage selection. @ref ACMP_HystVol
  * @retval None.
  */
static inline void DCL_ACMP_SetCmpHysteresisVoltage(ACMP_RegStruct *acmpx, ACMP_HystVol volSelect)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(volSelect >= ACMP_HYS_VOL_ZERO);
    ACMP_PARAM_CHECK_NO_RET(volSelect <= ACMP_HYS_VOL_30MV);
    acmpx->ACMP_RSV.BIT.da_acmp_rsv = volSelect;
}

/**
  * @brief Set comparator's output polarity
  * @param acmp: ACMP register base address.
  * @param polarity: output polarity. @ref ACMP_OutputPolarity
  * @retval None.
  */
static inline void DCL_ACMP_SetCmpOutputPolarity(ACMP_RegStruct *acmpx, ACMP_OutputPolarity polarity)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    ACMP_PARAM_CHECK_NO_RET(polarity >= ACMP_OUT_NOT_INVERT);
    ACMP_PARAM_CHECK_NO_RET(polarity <= ACMP_OUT_INVERT);
    acmpx->ACMP_CTRL2.BIT.cfg_acmp_out_inv = polarity;
}

/**
  * @brief Reading Synchronize the comparison result.
  * @param acmp: ACMP register base address.
  * @retval Synchronize result.
  */
static inline unsigned int DCL_ACMP_GetCmpOutValueSynchronize(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    return acmpx->ACMP_RSLT.BIT.cmp_sync_rslt;
}

/**
  * @brief Reading Filtering result after latching.
  * @param acmp: ACMP register base address.
  * @retval Filtering result after latching.
  */
static inline unsigned int DCL_ACMP_GetCmpOutValueAfterLatching(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    return acmpx->ACMP_RSLT.BIT.cmp_latch_rslt;
}

/**
  * @brief Reading filtered result.
  * @param acmp: ACMP register base address.
  * @retval filtered result.
  */
static inline unsigned int DCL_ACMP_GetCmpOutValueFiltered(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    return acmpx->ACMP_RSLT.BIT.cmp_filter_rslt;
}

/**
  * @brief Reading Asynchronous comparison results.
  * @param acmp: ACMP register base address.
  * @retval Asynchronous comparison results.
  */
static inline unsigned int DCL_ACMP_GetCmpOutValueAsynchronous(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    return acmpx->ACMP_RSLT.BIT.cmp_ana_rslt;
}

/**
  * @brief Enable APTSYNCP Clear Filtering Result
  * @param acmp: ACMP register base address.
  * @retval none.
  */
static inline void DCL_ACMP_EnableAptSyncpCleanFilterResult(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_RSLT_CLR.BIT.cmp_latch_synp_clr = BASE_CFG_ENABLE;
}

/**
  * @brief Disable APTSYNCP Clear Filtering Result
  * @param acmp: ACMP register base address.
  * @retval none.
  */
static inline void DCL_ACMP_DisableAptSyncpCleanFilterResult(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_RSLT_CLR.BIT.cmp_latch_synp_clr = BASE_CFG_DISABLE;
}

/**
  * @brief The software clears the filtering result.
  * @param acmp: ACMP register base address.
  * @retval none.
  */
static inline void DCL_ACMP_SoftwareCleanFilterResult(ACMP_RegStruct *acmpx)
{
    ACMP_ASSERT_PARAM(IsACMPInstance(acmpx));
    acmpx->ACMP_RSLT_CLR.BIT.cmp_latch_sw_clr = BASE_CFG_ENABLE;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif

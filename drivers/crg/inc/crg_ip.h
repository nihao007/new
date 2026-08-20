/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2025. All rights reserved.
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
 * @file      crg_ip.h
 * @author    MCU Driver Team
 * @brief     TIMER module driver.
 * @details   This file provides firmware functions to manage the following
 *            functionalities of the TIMER.
 *                + CRG register mapping structure
 *                + Direct Configuration Layer functions of CRG
 */
#ifndef McuMagicTag_CRG_IP_H
#define McuMagicTag_CRG_IP_H

/* Includes ------------------------------------------------------------------*/
#include "baseinc.h"

/**
  * @addtogroup CRG
  * @{
  */

/**
  * @defgroup CRG_IP CRG_IP
  * @brief CRG_IP: crg_v1
  * @{
  */

/**
 * @defgroup CRG_Param_Def CRG Parameters Definition
 * @brief Definition of CRG configuration parameters.
 * @{
 */
#ifdef  CRG_PARAM_CHECK
#define CRG_ASSERT_PARAM          BASE_FUNC_ASSERT_PARAM
#define CRG_PARAM_CHECK_NO_RET    BASE_FUNC_PARAMCHECK_NO_RET
#define CRG_PARAM_CHECK_WITH_RET  BASE_FUNC_PARAMCHECK_WITH_RET
#else
#define CRG_ASSERT_PARAM(para)               ((void)0U)
#define CRG_PARAM_CHECK_NO_RET(para)         ((void)0U)
#define CRG_PARAM_CHECK_WITH_RET(para, ret)  ((void)0U)
#endif

#define IP_CLK_DISABLE               0x00000000U   /**< IP Clock disable bitmask */
#define IP_CLK_ENABLE                0x00000001U   /**< IP Clock disable bitmask */
#define HPM_CLK_ENABLE               0x00000001U   /**< HPM Clock enable bitmask */
#define HPM_1M_CLK_ENABLE            0x00000002U   /**< HPM 1M Clock enable bitmask */
#define IP_SYSCLK_ENABLE             0x00000002U   /**< IP SysClock disable bitmask, Only valid for ADC */

#define DAC_DIV_BITLEN               4U            /**< DIV bit length */
#define DAC_DIV_MASK ((1 << DAC_DIV_BITLEN) - 1)   /**< DAC div mask, base on the bit length */

#define ADC_DIV_FACTOR          (1 << 1)           /**< ADC div min factor */
#define CRG_1MHZ_CLK_MAX_DIV    63
#define CRG_FREQ_1MHz           (1000 * 1000)
#define CRG_PLL_REF_MIN_FREQ    (4 * CRG_FREQ_1MHz)
#define CRG_PLL_REF_MAX_FREQ    (30 * CRG_FREQ_1MHz)
#define CRG_CLK_PFD_MIN_FREQ    (4 * CRG_FREQ_1MHz)
#define CRG_CLK_PFD_MAX_FREQ    (10 * CRG_FREQ_1MHz)
#define CRG_CLK_VCO_MIN_FREQ    (200 * CRG_FREQ_1MHz)
#define CRG_CLK_VCO_MAX_FREQ    (500 * CRG_FREQ_1MHz)
#define CRG_CLK_TARGET_MAX_FREQ (200 * CRG_FREQ_1MHz)
#define CRG_CLK_PST2_MAX_FREQ   (100 * CRG_FREQ_1MHz)
#define CRG_CLK_PLL1_DEFAULT_FREQ   (100 * CRG_FREQ_1MHz)
/**
 * @brief PLL refer clock Select
 */
typedef enum {
    CRG_PLL_REF_CLK_SELECT_HOSC = 0,
    CRG_PLL_REF_CLK_SELECT_XTAL = 1,
} CRG_PllRefClkSelect;

/**
 * @brief PLL previous divsion value in register
 */
typedef enum {
    CRG_PLL_PREDIV_1 = 0,
    CRG_PLL_PREDIV_2 = 1,
    CRG_PLL_PREDIV_3 = 2,
    CRG_PLL_PREDIV_4 = 3,
    CRG_PLL_PREDIV_5 = 4,
    CRG_PLL_PREDIV_6 = 5,
    CRG_PLL_PREDIV_7 = 6,
    CRG_PLL_PREDIV_8 = 7,
} CRG_PllPreDiv;

/**
 * @brief PLL previous divison value in Calc frequency
 */
typedef enum {
    PLL_PREDIV_OUT_1 = 1,
    PLL_PREDIV_OUT_2 = 2,
    PLL_PREDIV_OUT_3 = 3,
    PLL_PREDIV_OUT_4 = 4,
    PLL_PREDIV_OUT_5 = 5,
    PLL_PREDIV_OUT_6 = 6,
    PLL_PREDIV_OUT_7 = 7,
    PLL_PREDIV_OUT_8 = 8,
} PLL_PreDivOut;

/**
 * @brief PLL post division 1 value in register
 */
typedef enum {
    CRG_PLL_POSTDIV_1 = 0,
    CRG_PLL_POSTDIV_2 = 1,
    CRG_PLL_POSTDIV_3 = 2,
    CRG_PLL_POSTDIV_4 = 3,
    CRG_PLL_POSTDIV_5 = 4,
    CRG_PLL_POSTDIV_6 = 5,
    CRG_PLL_POSTDIV_7 = 6,
    CRG_PLL_POSTDIV_8 = 7,
} CRG_PllPostDiv;

/**
 * @brief PLL post division 2 value in register
 */
typedef enum {
    CRG_PLL_POSTDIV2_1 = 0,
    CRG_PLL_POSTDIV2_2 = 1,
    CRG_PLL_POSTDIV2_3 = 2,
    CRG_PLL_POSTDIV2_4 = 3,
    CRG_PLL_POSTDIV2_5 = 4,
    CRG_PLL_POSTDIV2_6 = 5,
    CRG_PLL_POSTDIV2_7 = 6,
    CRG_PLL_POSTDIV2_8 = 7,
} CRG_PllPostDiv2;


/**
 * @brief Core clock selection
 * @note  default select HOSC
 */
typedef enum {
    CRG_CORE_CLK_SELECT_HOSC = 0,
    CRG_CORE_CLK_SELECT_TCXO = 1,
    CRG_CORE_CLK_SELECT_PLL  = 2,
} CRG_CoreClkSelect;

/**
 * @brief Core clock selection 2
 * @note  default select HOSC
 */
typedef enum {
    CRG_CORE_CLK2_SELECT_HOSC = 0,
    CRG_CORE_CLK2_SELECT_TCXO = 1,
    CRG_CORE_CLK2_SELECT_PLL  = 2,
} CRG_CoreClkSelect2;

/**
 * @brief 1M clock selection
 * @note  default select HOSC
 */
typedef enum {
    CRG_1M_CLK_SELECT_HOSC = 0,
    CRG_1M_CLK_SELECT_TCXO = 1,
} CRG_1MClkSelect;

/**
 * @brief PLL frequency multiplication range
 */
typedef enum {
    CRG_PLL_FBDIV_MIN  = 6,
    CRG_PLL_FBDIV_MAX  = 127,
} CRG_PllFbDivRange;

/**
 * @brief PLL diagnose post div selection
 */
typedef enum {
    CRG_PLL_DIG_POST_DIV_SELECT_FREF = 0,
    CRG_PLL_DIG_POST_DIV_SELECT_PLL  = 1,
} CRG_PllDigPostDivInSelect;

/**
 * @brief PLL diagnose loct detect lpsel
 */
typedef enum {
    CRG_PLL_DIG_LOCKDET_LP_SELECT_2048 = 0,
    CRG_PLL_DIG_LOCKDET_LP_SELECT_1024 = 1,
    CRG_PLL_DIG_LOCKDET_LP_SELECT_512  = 2,
    CRG_PLL_DIG_LOCKDET_LP_SELECT_256  = 3,
} CRG_PllDigLockDetLpSelect;

/**
 * @brief PLL Test selection
 */
typedef enum {
    CRG_PLL_TEST_SELECT_FPFD   = 0,
    CRG_PLL_TEST_SELECT_CKFB   = 1,
    CRG_PLL_TEST_SELECT_LOCKDET_OUTPUT   = 2,
    CRG_PLL_TEST_SELECT_FOUTPOSTDIV_128  = 3,
    CRG_PLL_TEST_SELECT_OUTPUT_0 = 4,
} CRG_PllDigTestSelect;

/**
 * @brief CRG Test Clock Select
 */
typedef enum {
    CRG_TEST_CLK_HOSC            = 0x00000001U,
    CRG_TEST_CLK_LOSC            = 0x00000002U,
    CRG_TEST_CLK_TCXO            = 0x00000003U,
    CRG_TEST_CLK_BG_CHOPPER      = 0x00000004U,
    CRG_TEST_CLK_ADC_DIV4        = 0x00000005U,
    CRG_TEST_CLK_HCLK_DIV6       = 0x00000006U,
    CRG_TEST_CLK_HOSC_DIV        = 0x00000007U,
} CRG_TestClkSel;

/**
 * @brief ADC source clock select
 */
typedef enum {
    CRG_ADC_CLK_ASYN_HOSC = 0,
    CRG_ADC_CLK_ASYN_TCXO = 1,
    CRG_ADC_CLK_ASYN_PLL_DIV = 2,
    CRG_ADC_CLK_SYN_CORE = 3,
} CRG_AdcClkSelect;

/**
 * @brief ADC synchronous and asynchronous clock source selection
 */
typedef enum {
    CRG_ADC_CLK_ASYNCHRONOUS = 0,
    CRG_ADC_CLK_SYNCHRONOUS = 1,
} CRG_AdcClkModeSelect;

/**
 * @brief ADC Div set Value
 */
typedef enum {
    CRG_ADC_DIV_1 = 0,
    CRG_ADC_DIV_2 = 1,
    CRG_ADC_DIV_3 = 2,
    CRG_ADC_DIV_4 = 3,
} CRG_AdcDiv;

/**
 * @brief ANA clock select
 */
typedef enum {
    CRG_ANA_SEL_PLL0 = 0,
    CRG_ANA_SEL_PLL1 = 1,
} CRG_AnaClkSelect;

/**
 * @brief FMC source clock select
 */
typedef enum {
    CRG_FMC_CLK_HOSC      = 0,
    CRG_FMC_CLK_TCXO      = 1,
    CRG_FMC_CLK_PLL_PST3  = 2,
    CRG_FMC_CLK_PLL_REF   = 3,
} CRG_FmcClkSelect;

/**
 * @brief FNC Div set Value
 */
typedef enum {
    CRG_FMC_DIV_1 = 0,
    CRG_FMC_DIV_2 = 1,
    CRG_FMC_DIV_3 = 2,
    CRG_FMC_DIV_4 = 3,
    CRG_FMC_DIV_5 = 4,
    CRG_FMC_DIV_6 = 5,
    CRG_FMC_DIV_7 = 6,
    CRG_FMC_DIV_8 = 7,
} CRG_FmcDiv;

/**
  * @brief CRG Extra Handle, include CRG's other config
  */
typedef struct {
    CRG_PllPostDiv2     pllPostDiv2;      /**< PLL post 2  ratio */
    CRG_1MClkSelect     clk1MSelect;      /**< 1M clock selection */
    unsigned int        clk1MDiv;         /**< 1M clock ratio */
    CRG_AnaClkSelect    anaClkSel;        /**< ANA clock selcet */
    CRG_PllPreDiv       pll1PreDiv;        /**< PLL pre division */
    unsigned int        pll1FbDiv;         /**< PLL loop divider ratio */
    CRG_PllPostDiv      pll1PostDiv;       /**< PLL post ratio */
    CRG_PllPostDiv2     pll1PostDiv2;
} CRG_ExtendHandle;

/**
 * @brief PLL Divison Config
 */
typedef struct {
    unsigned int freqErr;
    unsigned int preDiv;
    unsigned int fbDiv;
    unsigned int postDiv;
} CRG_PllDivCfg;

/**
 * @brief APB_HS_SUBSYS IP config
 */
typedef union {
    unsigned int  value;
    struct {
        unsigned int  clkEnMask    : 16;
        unsigned int  softResetReq : 16;
    } BIT;
} volatile CRG_IpWoClkSelectCfg;

/**
 * @brief CAN config
 * @see   CRG_CAN_FD_CLK_REG and CRG_CAN_FD_PCLK_REG
 */
typedef union {
    unsigned int value[2];
    struct {
        unsigned int    can_fd_clk_cken       : 1;
        unsigned int    reserved0             : 15;
        unsigned int    can_fd_clk_srst_req   : 1;
        unsigned int    reserved1             : 15;
        unsigned int    can_fd_pclk_cken      : 1;
        unsigned int    reserved2             : 15;
        unsigned int    can_fd_pclk_srst_req  : 1;
        unsigned int    reserved3             : 15;
    } BIT;
} volatile CRG_CanfdIpCfg;

/* EFLASH config */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    eflash_pclk_cken      : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_EfcIpCfg;

/**
 * @brief ADC config
 * @see   PERI_CRG41_Reg and PERI_CRG42_Reg and PERI_CRG43_Reg
 */
typedef union {
    unsigned int  value[2];
    struct {
        unsigned int    clk_adc_div0       : 2;
        unsigned int    reserved0          : 6;
        unsigned int    clk_adc_div1       : 2;
        unsigned int    reserved1          : 22;
        unsigned int    clk_adc_cken       : 1;
        unsigned int    reserved2          : 15;
        unsigned int    adc_srst_req       : 1;
        unsigned int    reserved3          : 7;
        unsigned int    cfg_adc_ckmode_sel : 1;
        unsigned int    reserved4          : 7;
    } BIT;
} volatile CRG_AdcIpCfg;

/**
 * @brief ANA config
 * @see   PERI_CRG664_Reg - PERI_CRG677_Reg
 */
typedef union {
    unsigned int  value;
    struct {
        unsigned int    reserved0    : 16;
        unsigned int    ip_srst_req  : 1;
        unsigned int    reserved1    : 15;
    } BIT;
} volatile CRG_AnaIpCfg;

/**
 * @brief FMC config
 */
typedef union {
    unsigned int  value[4];
    struct {
        unsigned int    fmc_cken           : 1;     /* CRG_FMC_CLK0 */
        unsigned int    reserved0          : 15;    /* CRG_FMC_CLK0 */
        unsigned int    fmc_phy_srst_req   : 1;     /* CRG_FMC_CLK0 */
        unsigned int    reserved1          : 15;    /* CRG_FMC_CLK0 */
        unsigned int    reserved2          : 16;    /* CRG_FMC_CLK1 */
        unsigned int    fmc_srst_req       : 1;     /* CRG_FMC_CLK1 */
        unsigned int    reserved3          : 15;    /* CRG_FMC_CLK1 */
        unsigned int    fmc_pclk_cken      : 1;     /* CRG_FMC_PCLK */
        unsigned int    reserved4          : 15;    /* CRG_FMC_PCLK */
        unsigned int    fmc_pclk_srst_req : 1;     /* CRG_FMC_PCLK */
        unsigned int    reserved5          : 15;    /* CRG_FMC_PCLK */
        unsigned int    clk_fmc_div        : 3;     /* CRG_FMC_CLK_SEL */
        unsigned int    reserved6          : 13;    /* CRG_FMC_CLK_SEL */
        unsigned int    clk_fmc_sw_sel     : 1;     /* CRG_FMC_CLK_SEL */
        unsigned int    reserved7          : 15;    /* CRG_FMC_CLK_SEL */
    } BIT;
} volatile CRG_FmcIpCfg;


/**
 * @brief IP match info for ip process
 */
typedef struct {
    void         *baseAddr;    /**< Base address of ip */
    unsigned int  offset;      /**< The offset in CRG_RegStruct */
    unsigned int  idx;         /**< index in Reg, for example: 0 -capm0_cken 1 - capm1_cken in PERI_CRG30_Reg */
} CRG_IpMatchInfo;

/**
  * @}
  */

/**
 * @brief APB_HS_SUBSYS IP config
 */
/* Define the union volatile CRG_CKREF_CKSEL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll_ref_cksel         : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_CKREF_CKSEL_REG;

/* Define the union volatile CRG_PLL0_PREDIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll0_prediv           : 4   ;
        unsigned int    reserved0             : 28  ;
    } BIT;
} volatile CRG_PLL0_PREDIV_REG;

/* Define the union volatile CRG_PLL0_FBDIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll0_fbdiv            : 8   ;
        unsigned int    reserved0             : 24  ;
    } BIT;
} volatile CRG_PLL0_FBDIV_REG;

/* Define the union volatile CRG_PLL0_PSTDIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll0_postdiv1         : 4   ;
        unsigned int    pll0_postdiv2         : 4   ;
        unsigned int    reserved0             : 24  ;
    } BIT;
} volatile CRG_PLL0_PSTDIV_REG;

/* Define the union volatile CRG_PLL0_PD */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll0_pd               : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_PLL0_PD_REG;

/* Define the union volatile CRG_PLL0_LOCK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll0_lock             : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_PLL0_LOCK_REG;

/* Define the union volatile CRG_PLL0_LOCK_DGL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll0_lock_deglitch    : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_PLL0_LOCK_DGL_REG;

/* Define the union volatile CRG_ANA_PLL_CKSEL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    ana_pll_cksel         : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_ANA_PLL_CKSEL_REG;

/* Define the union volatile CRG_PLL1_PREDIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll1_prediv           : 4   ;
        unsigned int    reserved0             : 28  ;
    } BIT;
} volatile CRG_PLL1_PREDIV_REG;

/* Define the union volatile CRG_PLL1_FBDIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll1_fbdiv            : 8   ;
        unsigned int    reserved0             : 24  ;
    } BIT;
} volatile CRG_PLL1_FBDIV_REG;

/* Define the union volatile CRG_PLL1_PSTDIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll1_postdiv1         : 4   ;
        unsigned int    pll1_postdiv2         : 4   ;
        unsigned int    reserved0             : 24  ;
    } BIT;
} volatile CRG_PLL1_PSTDIV_REG;

/* Define the union volatile CRG_PLL1_PD */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll1_pd               : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_PLL1_PD_REG;

/* Define the union volatile CRG_PLL1_LOCK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll1_lock             : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_PLL1_LOCK_REG;

/* Define the union volatile CRG_PLL1_LOCK_DGL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pll1_lock_deglitch    : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_PLL1_LOCK_DGL_REG;

/* Define the union volatile CRG_SYS_CKSEL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_pst1_sw_sel       : 2   ;
        unsigned int    reserved0             : 2   ;
        unsigned int    clk_pst2_sw_sel       : 2   ;
        unsigned int    reserved1             : 2   ;
        unsigned int    clk_pst3_sw_sel       : 2   ;
        unsigned int    reserved2             : 22  ;
    } BIT;
} volatile CRG_SYS_CKSEL_REG;

/* Define the union volatile CRG_PVD_RST_EN */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    pvd_rst_enable        : 1   ;
        unsigned int    reserved0             : 3   ;
        unsigned int    reserved1             : 28  ;
    } BIT;
} volatile CRG_PVD_RST_EN_REG;

/* Define the union volatile CRG_1M_INI_CKSEL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_1m_ini_cksel      : 1   ;
        unsigned int    reserved0             : 3   ;
        unsigned int    reserved1             : 28  ;
    } BIT;
} volatile CRG_1M_INI_CKSEL_REG;

/* Define the union volatile CRG_1M_DIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_1m_div            : 6   ;
        unsigned int    reserved0             : 2   ;
        unsigned int    reserved1             : 24  ;
    } BIT;
} volatile CRG_1M_DIV_REG;

/* Define the union volatile CRG_UART0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    uart0_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    uart0_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_UART0_REG;

/* Define the union volatile CRG_UART1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    uart1_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    uart1_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_UART1_REG;

/* Define the union volatile CRG_UART2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    uart2_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    uart2_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_UART2_REG;

/* Define the union volatile CRG_UART3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    uart3_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    uart3_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_UART3_REG;

/* Define the union volatile CRG_UART4 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    uart4_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    uart4_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_UART4_REG;

/* Define the union volatile CRG_UART5 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    uart5_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    uart5_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_UART5_REG;

/* Define the union volatile CRG_UART6 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    uart6_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    uart6_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_UART6_REG;

/* Define the union volatile CRG_UART7 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    uart7_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    uart7_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_UART7_REG;

/* Define the union volatile CRG_SPI0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    spi0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    spi0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_SPI0_REG;

/* Define the union volatile CRG_SPI1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    spi1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    spi1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_SPI1_REG;

/* Define the union volatile CRG_SPI2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    spi2_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    spi2_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_SPI2_REG;

/* Define the union volatile CRG_I2C0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    i2c0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    i2c0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_I2C0_REG;

/* Define the union volatile CRG_I2C1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    i2c1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    i2c1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_I2C1_REG;

/* Define the union volatile CRG_I2C2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    i2c2_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    i2c2_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_I2C2_REG;

/* Define the union volatile CRG_TIMER0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    timer0_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    timer0_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_TIMER0_REG;

/* Define the union volatile CRG_TIMER1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    timer1_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    timer1_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_TIMER1_REG;

/* Define the union volatile CRG_TIMER2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    timer2_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    timer2_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_TIMER2_REG;

/* Define the union volatile CRG_TIMER3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    timer3_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    timer3_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_TIMER3_REG;

/* Define the union volatile CRG_TIMER4 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    timer4_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    timer4_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_TIMER4_REG;

/* Define the union volatile CRG_TIMER5 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    timer5_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    timer5_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_TIMER5_REG;

/* Define the union volatile CRG_CAPM0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    capm0_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    capm0_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAPM0_REG;

/* Define the union volatile CRG_CAPM1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    capm1_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    capm1_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAPM1_REG;

/* Define the union volatile CRG_CAPM2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    capm2_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    capm2_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAPM2_REG;

/* Define the union volatile CRG_CAN0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    can0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    can0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAN0_REG;

/* Define the union volatile CRG_CAN1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    can1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    can1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAN1_REG;

/* Define the union volatile CRG_CAN_FD0_CLK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    can_fd0_clk_cken      : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    can_fd0_clk_srst_req  : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAN_FD0_CLK_REG;

/* Define the union volatile CRG_CAN_FD0_PCLK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    can_fd0_pclk_cken     : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    can_fd0_pclk_srst_req : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAN_FD0_PCLK_REG;

/* Define the union volatile CRG_CAN_FD1_CLK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    can_fd1_clk_cken      : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    can_fd1_clk_srst_req  : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAN_FD1_CLK_REG;

/* Define the union volatile CRG_CAN_FD1_PCLK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    can_fd1_pclk_cken     : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    can_fd1_pclk_srst_req : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAN_FD1_PCLK_REG;

/* Define the union volatile CRG_CAN_FD2_CLK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    can_fd2_clk_cken      : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    can_fd2_clk_srst_req  : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAN_FD2_CLK_REG;

/* Define the union volatile CRG_CAN_FD2_PCLK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    can_fd2_pclk_cken     : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    can_fd2_pclk_srst_req : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CAN_FD2_PCLK_REG;

/* Define the union volatile CRG_DMA0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    dma0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    dma0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_DMA0_REG;

/* Define the union volatile CRG_DMA1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    dma1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    dma1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_DMA1_REG;

/* Define the union volatile CRG_FMC_CLK0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    fmc_cken              : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    fmc_phy_srst_req      : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_FMC_CLK0_REG;

/* Define the union volatile CRG_FMC_CLK1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    fmc_srst_req          : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_FMC_CLK1_REG;

/* Define the union volatile CRG_FMC_PCLK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    fmc_pclk_cken         : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    fmc_pclk_srst_req     : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_FMC_PCLK_REG;

/* Define the union volatile CRG_FMC_CLK_SEL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_fmc_div           : 3   ;
        unsigned int    reserved0             : 13  ;
        unsigned int    clk_fmc_sw_sel        : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_FMC_CLK_SEL_REG;

/* Define the union volatile CRG_CMM0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cmm0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    cmm0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CMM0_REG;

/* Define the union volatile CRG_CMM1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cmm1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    cmm1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CMM1_REG;

/* Define the union volatile CRG_CFD */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cfd_cken              : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    cfd_srst_req          : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CFD_REG;

/* Define the union volatile CRG_CRC */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    crc_cken              : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    crc_srst_req          : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CRC_REG;

/* Define the union volatile CRG_ADDLL_CLKIN */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    addll_clkin_cken      : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_ADDLL_CLKIN_REG;

/* Define the union volatile CRG_DLL_DIG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    dll_dig_cken          : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    dll_dig_srst_req      : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_DLL_DIG_REG;

/* Define the union volatile CRG_APT_XBAR */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt_xbar_cken         : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt_xbar_srst_req     : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT_XBAR_REG;

/* Define the union volatile CRG_APT0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT0_REG;

/* Define the union volatile CRG_APT1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT1_REG;

/* Define the union volatile CRG_APT2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt2_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt2_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT2_REG;

/* Define the union volatile CRG_APT3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt3_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt3_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT3_REG;

/* Define the union volatile CRG_APT4 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt4_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt4_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT4_REG;

/* Define the union volatile CRG_APT5 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt5_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt5_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT5_REG;

/* Define the union volatile CRG_APT6 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt6_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt6_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT6_REG;

/* Define the union volatile CRG_APT7 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt7_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt7_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT7_REG;

/* Define the union volatile CRG_APT8 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt8_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt8_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT8_REG;

/* Define the union volatile CRG_APT9 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt9_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt9_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT9_REG;

/* Define the union volatile CRG_APT10 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt10_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt10_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT10_REG;

/* Define the union volatile CRG_APT11 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt11_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt11_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT11_REG;

/* Define the union volatile CRG_APT12 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt12_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt12_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT12_REG;

/* Define the union volatile CRG_APT13 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt13_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt13_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT13_REG;

/* Define the union volatile CRG_APT14 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt14_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt14_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT14_REG;

/* Define the union volatile CRG_APT15 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    apt15_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    apt15_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_APT15_REG;

/* Define the union volatile CRG_GPT0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpt0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpt0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPT0_REG;

/* Define the union volatile CRG_GPT1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpt1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpt1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPT1_REG;

/* Define the union volatile CRG_GPIO0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio0_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio0_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO0_REG;

/* Define the union volatile CRG_GPIO1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio1_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio1_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO1_REG;

/* Define the union volatile CRG_GPIO2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio2_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio2_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO2_REG;

/* Define the union volatile CRG_GPIO3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio3_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio3_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO3_REG;

/* Define the union volatile CRG_GPIO4 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio4_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio4_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO4_REG;

/* Define the union volatile CRG_GPIO5 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio5_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio5_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO5_REG;

/* Define the union volatile CRG_GPIO6 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio6_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio6_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO6_REG;

/* Define the union volatile CRG_GPIO7 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio7_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio7_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO7_REG;

/* Define the union volatile CRG_GPIO8 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio8_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio8_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO8_REG;

/* Define the union volatile CRG_GPIO9 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio9_cken            : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio9_srst_req        : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO9_REG;

/* Define the union volatile CRG_GPIO10 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio10_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio10_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO10_REG;

/* Define the union volatile CRG_GPIO11 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio11_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio11_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO11_REG;

/* Define the union volatile CRG_GPIO12 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio12_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio12_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO12_REG;

/* Define the union volatile CRG_GPIO13 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio13_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio13_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO13_REG;

/* Define the union volatile CRG_GPIO14 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio14_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio14_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO14_REG;

/* Define the union volatile CRG_GPIO15 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio15_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio15_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO15_REG;

/* Define the union volatile CRG_GPIO16 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    gpio16_cken           : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    gpio16_srst_req       : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_GPIO16_REG;

/* Define the union volatile CRG_QDM0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    qdm0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    qdm0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_QDM0_REG;

/* Define the union volatile CRG_QDM1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    qdm1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    qdm1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_QDM1_REG;

/* Define the union volatile CRG_QDM2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    qdm2_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    qdm2_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_QDM2_REG;

/* Define the union volatile CRG_QDM3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    qdm3_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    qdm3_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_QDM3_REG;

/* Define the union volatile CRG_CPU0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cpu0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    cpu0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CPU0_REG;

/* Define the union volatile CRG_CPU1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cpu1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    cpu1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CPU1_REG;

/* Define the union volatile CRG_CPU2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cpu2_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    cpu2_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_CPU2_REG;

/* Define the union volatile CRG_EFLASH */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    eflash_pclk_cken      : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_EFLASH_REG;

/* Define the union volatile CRG_IPCM */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    ipcm_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    ipcm_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_IPCM_REG;

/* Define the union volatile CRG_OSC_SAR_TRIM */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    osc_trim_cken         : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    osc_trim_srst_req     : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_OSC_SAR_TRIM_REG;

/* Define the union volatile CRG_TEST_CKSEL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    test_clk_en           : 1   ;
        unsigned int    reserved0             : 3   ;
        unsigned int    pll_test_clk_sel      : 1   ;
        unsigned int    reserved1             : 3   ;
        unsigned int    sys_clk_sel           : 1   ;
        unsigned int    reserved2             : 7   ;
        unsigned int    test_clk_sel          : 3   ;
        unsigned int    reserved3             : 13  ;
    } BIT;
} volatile CRG_TEST_CKSEL_REG;

/* Define the union volatile CRG_ADC0_DIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_adc0_div0         : 2   ;
        unsigned int    reserved0             : 6   ;
        unsigned int    clk_adc0_div1         : 2   ;
        unsigned int    reserved1             : 6   ;
        unsigned int    reserved2             : 16  ;
    } BIT;
} volatile CRG_ADC0_DIV_REG;

/* Define the union volatile CRG_ADC0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    adc0_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    adc0_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    adc0_clk_mode         : 1   ;
        unsigned int    reserved2             : 7   ;
    } BIT;
} volatile CRG_ADC0_REG;

/* Define the union volatile CRG_ADC1_DIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_adc1_div0         : 2   ;
        unsigned int    reserved0             : 6   ;
        unsigned int    clk_adc1_div1         : 2   ;
        unsigned int    reserved1             : 6   ;
        unsigned int    reserved2             : 16  ;
    } BIT;
} volatile CRG_ADC1_DIV_REG;

/* Define the union volatile CRG_ADC1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    adc1_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    adc1_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    adc1_clk_mode         : 1   ;
        unsigned int    reserved2             : 7   ;
    } BIT;
} volatile CRG_ADC1_REG;

/* Define the union volatile CRG_ADC2_DIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_adc2_div0         : 2   ;
        unsigned int    reserved0             : 6   ;
        unsigned int    clk_adc2_div1         : 2   ;
        unsigned int    reserved1             : 6   ;
        unsigned int    reserved2             : 16  ;
    } BIT;
} volatile CRG_ADC2_DIV_REG;

/* Define the union volatile CRG_ADC2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    adc2_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    adc2_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    adc2_clk_mode         : 1   ;
        unsigned int    reserved2             : 7   ;
    } BIT;
} volatile CRG_ADC2_REG;

/* Define the union volatile CRG_ADC3_DIV */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_adc3_div0         : 2   ;
        unsigned int    reserved0             : 6   ;
        unsigned int    clk_adc3_div1         : 2   ;
        unsigned int    reserved1             : 6   ;
        unsigned int    reserved2             : 16  ;
    } BIT;
} volatile CRG_ADC3_DIV_REG;

/* Define the union volatile CRG_ADC3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    adc3_cken             : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    adc3_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    adc3_clk_mode         : 1   ;
        unsigned int    reserved2             : 7   ;
    } BIT;
} volatile CRG_ADC3_REG;

/* Define the union volatile CRG_ADC0_CLK_PCTL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    adc0_CLK_PCTL         : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_ADC0_CLK_PCTL_REG;

/* Define the union volatile CRG_ADC1_CLK_PCTL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    adc1_CLK_PCTL         : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_ADC1_CLK_PCTL_REG;

/* Define the union volatile CRG_ADC2_CLK_PCTL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    adc2_CLK_PCTL         : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_ADC2_CLK_PCTL_REG;

/* Define the union volatile CRG_ADC3_CLK_PCTL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    adc3_CLK_PCTL         : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile CRG_ADC3_CLK_PCTL_REG;

/* Define the union volatile CRG_ANA */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_ana_cken          : 1   ;
        unsigned int    reserved0             : 15  ;
        unsigned int    ana_srst_req          : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_ANA_REG;

/* Define the union volatile CRG_VREF */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    vref_srst_req         : 1   ;
        unsigned int    reserved1             : 7   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile CRG_VREF_REG;

/* Define the union volatile CRG_ACMP0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    acmp0_srst_req        : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_ACMP0_REG;

/* Define the union volatile CRG_ACMP1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    acmp1_srst_req        : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_ACMP1_REG;

/* Define the union volatile CRG_ACMP2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    acmp2_srst_req        : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_ACMP2_REG;

/* Define the union volatile CRG_ACMP3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    acmp3_srst_req        : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_ACMP3_REG;

/* Define the union volatile CRG_ACMP4 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    acmp4_srst_req        : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_ACMP4_REG;

/* Define the union volatile CRG_ACMP5 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    acmp5_srst_req        : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_ACMP5_REG;

/* Define the union volatile CRG_ACMP6 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    acmp6_srst_req        : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_ACMP6_REG;

/* Define the union volatile CRG_ACMP7 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    acmp7_srst_req        : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_ACMP7_REG;

/* Define the union volatile CRG_DAC0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    dac0_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_DAC0_REG;

/* Define the union volatile CRG_DAC1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    dac1_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_DAC1_REG;

/* Define the union volatile CRG_DAC2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    dac2_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_DAC2_REG;

/* Define the union volatile CRG_DAC3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    dac3_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_DAC3_REG;

/* Define the union volatile CRG_DAC4 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    dac4_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_DAC4_REG;

/* Define the union volatile CRG_DAC5 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    dac5_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_DAC5_REG;

/* Define the union volatile CRG_DAC6 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    dac6_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_DAC6_REG;

/* Define the union volatile CRG_DAC7 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    dac7_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_DAC7_REG;

/* Define the union volatile CRG_DAC8 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    dac8_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_DAC8_REG;

/* Define the union volatile CRG_PGA0 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    pga0_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_PGA0_REG;

/* Define the union volatile CRG_PGA1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    pga1_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_PGA1_REG;

/* Define the union volatile CRG_PGA2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    pga2_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_PGA2_REG;

/* Define the union volatile CRG_PGA3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    reserved0             : 16  ;
        unsigned int    pga3_srst_req         : 1   ;
        unsigned int    reserved1             : 15  ;
    } BIT;
} volatile CRG_PGA3_REG;

/* Define the union volatile CRG_HPM */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    hpm_cken              : 1   ;
        unsigned int    reserved0             : 3   ;
        unsigned int    hpm_1m_cken           : 1   ;
        unsigned int    reserved1             : 11  ;
        unsigned int    hpm_srst_req          : 1   ;
        unsigned int    reserved2             : 3   ;
        unsigned int    hpm_1m_srst_req       : 1   ;
        unsigned int    reserved3             : 3   ;
        unsigned int    reserved4             : 8   ;
    } BIT;
} volatile CRG_HPM_REG;

/* Define the union volatile CRG_DEBUG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    clk_pst1_sw_mux_sel   : 2   ;
        unsigned int    reserved0             : 2   ;
        unsigned int    clk_pst2_sw_mux_sel   : 2   ;
        unsigned int    reserved1             : 2   ;
        unsigned int    clk_pst3_sw_mux_sel   : 2   ;
        unsigned int    reserved2             : 22  ;
    } BIT;
} volatile CRG_DEBUG_REG;

/* Define the union volatile HOSC_CTRL1 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    hosc_ctrim_value      : 10  ;
        unsigned int    reserved0             : 22  ;
    } BIT;
} volatile HOSC_CTRL1_REG;

/* Define the union volatile HOSC_CTRL2 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    hosc_rtrim_value      : 8   ;
        unsigned int    hosc_itrim_value      : 4   ;
        unsigned int    hosc_sel_vref         : 3   ;
        unsigned int    reserved0             : 1   ;
        unsigned int    hosc_kvco_sel         : 2   ;
        unsigned int    reserved1             : 2   ;
        unsigned int    hosc_lpfr_sel         : 4   ;
        unsigned int    reserved2             : 8   ;
    } BIT;
} volatile HOSC_CTRL2_REG;

/* Define the union volatile HOSC_CTRL3 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    hosc_ate_en           : 1   ;
        unsigned int    hosc_dft_en           : 1   ;
        unsigned int    reserved0             : 2   ;
        unsigned int    hosc_test             : 10  ;
        unsigned int    reserved1             : 2   ;
        unsigned int    hosc_div_sel          : 2   ;
        unsigned int    reserved2             : 6   ;
        unsigned int    hosc_start_up         : 1   ;
        unsigned int    reserved3             : 3   ;
        unsigned int    hosc_vsel_ldo_ref     : 2   ;
        unsigned int    reserved4             : 2   ;
    } BIT;
} volatile HOSC_CTRL3_REG;

/* Define the union volatile HOSC_PD */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    hosc_pd               : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile HOSC_PD_REG;

/* Define the union volatile HOSC_LOCK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    hosc_lock             : 1   ;
        unsigned int    reserved0             : 31  ;
    } BIT;
} volatile HOSC_LOCK_REG;

//==============================================================================
/* Define the global struct */
typedef struct {
    CRG_CKREF_CKSEL_REG      CRG_CKREF_CKSEL                  ; /* 0x0 */
    CRG_PLL0_PREDIV_REG      CRG_PLL0_PREDIV                  ; /* 0x4 */
    CRG_PLL0_FBDIV_REG       CRG_PLL0_FBDIV                   ; /* 0x8 */
    CRG_PLL0_PSTDIV_REG      CRG_PLL0_PSTDIV                  ; /* 0xc */
    CRG_PLL0_PD_REG          CRG_PLL0_PD                      ; /* 0x10 */
    volatile unsigned int    reserved0[2]                     ; /* 0x14~0x18 */
    CRG_PLL0_LOCK_REG        CRG_PLL0_LOCK                    ; /* 0x1c */
    CRG_PLL0_LOCK_DGL_REG    CRG_PLL0_LOCK_DGL                ; /* 0x20 */
    volatile unsigned int    reserved1[3]                     ; /* 0x24~0x2c */
    CRG_ANA_PLL_CKSEL_REG    CRG_ANA_PLL_CKSEL                ; /* 0x30 */
    CRG_PLL1_PREDIV_REG      CRG_PLL1_PREDIV                  ; /* 0x34 */
    CRG_PLL1_FBDIV_REG       CRG_PLL1_FBDIV                   ; /* 0x38 */
    CRG_PLL1_PSTDIV_REG      CRG_PLL1_PSTDIV                  ; /* 0x3c */
    CRG_PLL1_PD_REG          CRG_PLL1_PD                      ; /* 0x40 */
    volatile unsigned int    reserved2[2]                     ; /* 0x44~0x48 */
    CRG_PLL1_LOCK_REG        CRG_PLL1_LOCK                    ; /* 0x4c */
    CRG_PLL1_LOCK_DGL_REG    CRG_PLL1_LOCK_DGL                ; /* 0x50 */
    volatile unsigned int    reserved3[43]                    ; /* 0x54~0xfc */
    CRG_SYS_CKSEL_REG        CRG_SYS_CKSEL                    ; /* 0x100 */
    CRG_PVD_RST_EN_REG       CRG_PVD_RST_EN                   ; /* 0x104 */
    CRG_1M_INI_CKSEL_REG     CRG_1M_INI_CKSEL                 ; /* 0x108 */
    CRG_1M_DIV_REG           CRG_1M_DIV                       ; /* 0x10c */
    volatile unsigned int    reserved4[12]                    ; /* 0x110~0x13c */
    CRG_UART0_REG            CRG_UART0                        ; /* 0x140 */
    CRG_UART1_REG            CRG_UART1                        ; /* 0x144 */
    CRG_UART2_REG            CRG_UART2                        ; /* 0x148 */
    CRG_UART3_REG            CRG_UART3                        ; /* 0x14c */
    CRG_UART4_REG            CRG_UART4                        ; /* 0x150 */
    CRG_UART5_REG            CRG_UART5                        ; /* 0x154 */
    CRG_UART6_REG            CRG_UART6                        ; /* 0x158 */
    CRG_UART7_REG            CRG_UART7                        ; /* 0x15c */
    volatile unsigned int    reserved5[8]                     ; /* 0x160~0x17c */
    CRG_SPI0_REG             CRG_SPI0                         ; /* 0x180 */
    CRG_SPI1_REG             CRG_SPI1                         ; /* 0x184 */
    CRG_SPI2_REG             CRG_SPI2                         ; /* 0x188 */
    volatile unsigned int    reserved6[13]                    ; /* 0x18c~0x1bc */
    CRG_I2C0_REG             CRG_I2C0                         ; /* 0x1c0 */
    CRG_I2C1_REG             CRG_I2C1                         ; /* 0x1c4 */
    CRG_I2C2_REG             CRG_I2C2                         ; /* 0x1c8 */
    volatile unsigned int    reserved7[29]                    ; /* 0x1cc~0x23c */
    CRG_TIMER0_REG           CRG_TIMER0                       ; /* 0x240 */
    CRG_TIMER1_REG           CRG_TIMER1                       ; /* 0x244 */
    CRG_TIMER2_REG           CRG_TIMER2                       ; /* 0x248 */
    CRG_TIMER3_REG           CRG_TIMER3                       ; /* 0x24c */
    CRG_TIMER4_REG           CRG_TIMER4                       ; /* 0x250 */
    CRG_TIMER5_REG           CRG_TIMER5                       ; /* 0x254 */
    volatile unsigned int    reserved8[10]                    ; /* 0x258~0x27c */
    CRG_CAPM0_REG            CRG_CAPM0                        ; /* 0x280 */
    CRG_CAPM1_REG            CRG_CAPM1                        ; /* 0x284 */
    CRG_CAPM2_REG            CRG_CAPM2                        ; /* 0x288 */
    volatile unsigned int    reserved9[13]                    ; /* 0x28c~0x2bc */
    CRG_CAN_FD0_CLK_REG      CRG_CAN_FD0_CLK                  ; /* 0x2c0 */
    CRG_CAN_FD0_PCLK_REG     CRG_CAN_FD0_PCLK                 ; /* 0x2c4 */
    CRG_CAN_FD1_CLK_REG      CRG_CAN_FD1_CLK                  ; /* 0x2c8 */
    CRG_CAN_FD1_PCLK_REG     CRG_CAN_FD1_PCLK                 ; /* 0x2cc */
    CRG_CAN_FD2_CLK_REG      CRG_CAN_FD2_CLK                  ; /* 0x2d0 */
    CRG_CAN_FD2_PCLK_REG     CRG_CAN_FD2_PCLK                 ; /* 0x2d4 */
    volatile unsigned int    reserved10[10]                   ; /* 0x2d8~0x2fc */
    CRG_DMA0_REG             CRG_DMA0                         ; /* 0x300 */
    CRG_DMA1_REG             CRG_DMA1                         ; /* 0x304 */
    volatile unsigned int    reserved11[2]                    ; /* 0x308~0x30c */
    CRG_FMC_CLK0_REG         CRG_FMC_CLK0                     ; /* 0x310 */
    CRG_FMC_CLK1_REG         CRG_FMC_CLK1                     ; /* 0x314 */
    CRG_FMC_PCLK_REG         CRG_FMC_PCLK                     ; /* 0x318 */
    CRG_FMC_CLK_SEL_REG      CRG_FMC_CLK_SEL                  ; /* 0x31c */
    volatile unsigned int    reserved12[8]                    ; /* 0x320~0x33c */
    CRG_CMM0_REG             CRG_CMM0                         ; /* 0x340 */
    CRG_CMM1_REG             CRG_CMM1                         ; /* 0x344 */
    CRG_CFD_REG              CRG_CFD                          ; /* 0x348 */
    volatile unsigned int    reserved13[13]                   ; /* 0x34c~0x37c */
    CRG_CRC_REG              CRG_CRC                          ; /* 0x380 */
    volatile unsigned int    reserved14[23]                   ; /* 0x384~0x3dc */
    CRG_ADDLL_CLKIN_REG      CRG_ADDLL_CLKIN                  ; /* 0x3e0 */
    CRG_DLL_DIG_REG          CRG_DLL_DIG                      ; /* 0x3e4 */
    CRG_APT_XBAR_REG         CRG_APT_XBAR                     ; /* 0x3e8 */
    volatile unsigned int    reserved15[5]                    ; /* 0x3ec~0x3fc */
    CRG_APT0_REG             CRG_APT0                         ; /* 0x400 */
    CRG_APT1_REG             CRG_APT1                         ; /* 0x404 */
    CRG_APT2_REG             CRG_APT2                         ; /* 0x408 */
    CRG_APT3_REG             CRG_APT3                         ; /* 0x40c */
    CRG_APT4_REG             CRG_APT4                         ; /* 0x410 */
    CRG_APT5_REG             CRG_APT5                         ; /* 0x414 */
    CRG_APT6_REG             CRG_APT6                         ; /* 0x418 */
    CRG_APT7_REG             CRG_APT7                         ; /* 0x41c */
    CRG_APT8_REG             CRG_APT8                         ; /* 0x420 */
    CRG_APT9_REG             CRG_APT9                         ; /* 0x424 */
    CRG_APT10_REG            CRG_APT10                        ; /* 0x428 */
    CRG_APT11_REG            CRG_APT11                        ; /* 0x42c */
    CRG_APT12_REG            CRG_APT12                        ; /* 0x430 */
    CRG_APT13_REG            CRG_APT13                        ; /* 0x434 */
    CRG_APT14_REG            CRG_APT14                        ; /* 0x438 */
    CRG_APT15_REG            CRG_APT15                        ; /* 0x43c */
    CRG_GPT0_REG             CRG_GPT0                         ; /* 0x440 */
    CRG_GPT1_REG             CRG_GPT1                         ; /* 0x444 */
    volatile unsigned int    reserved16[14]                   ; /* 0x448~0x47c */
    CRG_GPIO0_REG            CRG_GPIO0                        ; /* 0x480 */
    CRG_GPIO1_REG            CRG_GPIO1                        ; /* 0x484 */
    CRG_GPIO2_REG            CRG_GPIO2                        ; /* 0x488 */
    CRG_GPIO3_REG            CRG_GPIO3                        ; /* 0x48c */
    CRG_GPIO4_REG            CRG_GPIO4                        ; /* 0x490 */
    CRG_GPIO5_REG            CRG_GPIO5                        ; /* 0x494 */
    CRG_GPIO6_REG            CRG_GPIO6                        ; /* 0x498 */
    CRG_GPIO7_REG            CRG_GPIO7                        ; /* 0x49c */
    CRG_GPIO8_REG            CRG_GPIO8                        ; /* 0x4a0 */
    CRG_GPIO9_REG            CRG_GPIO9                        ; /* 0x4a4 */
    CRG_GPIO10_REG           CRG_GPIO10                       ; /* 0x4a8 */
    CRG_GPIO11_REG           CRG_GPIO11                       ; /* 0x4ac */
    CRG_GPIO12_REG           CRG_GPIO12                       ; /* 0x4b0 */
    CRG_GPIO13_REG           CRG_GPIO13                       ; /* 0x4b4 */
    CRG_GPIO14_REG           CRG_GPIO14                       ; /* 0x4b8 */
    CRG_GPIO15_REG           CRG_GPIO15                       ; /* 0x4bc */
    CRG_GPIO16_REG           CRG_GPIO16                       ; /* 0x4c0 */
    volatile unsigned int    reserved17[3]                    ; /* 0x4c4~0x4cc */
    CRG_QDM0_REG             CRG_QDM0                         ; /* 0x4d0 */
    CRG_QDM1_REG             CRG_QDM1                         ; /* 0x4d4 */
    CRG_QDM2_REG             CRG_QDM2                         ; /* 0x4d8 */
    CRG_QDM3_REG             CRG_QDM3                         ; /* 0x4dc */
    volatile unsigned int    reserved18[72]                   ; /* 0x4e0~0x5fc */
    CRG_CPU0_REG             CRG_CPU0                         ; /* 0x600 */
    CRG_CPU1_REG             CRG_CPU1                         ; /* 0x604 */
    CRG_CPU2_REG             CRG_CPU2                         ; /* 0x608 */
    volatile unsigned int    reserved19[61]                   ; /* 0x60c~0x6fc */
    CRG_EFLASH_REG           CRG_EFLASH                       ; /* 0x700 */
    volatile unsigned int    reserved20[63]                   ; /* 0x704~0x7fc */
    CRG_IPCM_REG             CRG_IPCM                         ; /* 0x800 */
    volatile unsigned int    reserved21[31]                   ; /* 0x804~0x87c */
    CRG_OSC_SAR_TRIM_REG     CRG_OSC_SAR_TRIM                 ; /* 0x880 */
    volatile unsigned int    reserved22[94]                   ; /* 0x884~0x9f8 */
    CRG_TEST_CKSEL_REG       CRG_TEST_CKSEL                   ; /* 0x9fc */
    CRG_ADC0_DIV_REG         CRG_ADC0_DIV                     ; /* 0xa00 */
    CRG_ADC0_REG             CRG_ADC0                         ; /* 0xa04 */
    CRG_ADC1_DIV_REG         CRG_ADC1_DIV                     ; /* 0xa08 */
    CRG_ADC1_REG             CRG_ADC1                         ; /* 0xa0c */
    CRG_ADC2_DIV_REG         CRG_ADC2_DIV                     ; /* 0xa10 */
    CRG_ADC2_REG             CRG_ADC2                         ; /* 0xa14 */
    CRG_ADC3_DIV_REG         CRG_ADC3_DIV                     ; /* 0xa18 */
    CRG_ADC3_REG             CRG_ADC3                         ; /* 0xa1c */
    CRG_ADC0_CLK_PCTL_REG    CRG_ADC0_CLK_PCTL                ; /* 0xa20 */
    CRG_ADC1_CLK_PCTL_REG    CRG_ADC1_CLK_PCTL                ; /* 0xa24 */
    CRG_ADC2_CLK_PCTL_REG    CRG_ADC2_CLK_PCTL                ; /* 0xa28 */
    CRG_ADC3_CLK_PCTL_REG    CRG_ADC3_CLK_PCTL                ; /* 0xa2C */
    volatile unsigned int    reserved23[8]                    ; /* 0xa30~0xa4c */
    CRG_ANA_REG              CRG_ANA                          ; /* 0xa50 */
    volatile unsigned int    reserved24[3]                    ; /* 0xa54~0xa5c */
    CRG_VREF_REG             CRG_VREF                         ; /* 0xa60 */
    volatile unsigned int    reserved25[3]                    ; /* 0xa64~0xa6c */
    CRG_ACMP0_REG            CRG_ACMP0                        ; /* 0xa70 */
    CRG_ACMP1_REG            CRG_ACMP1                        ; /* 0xa74 */
    CRG_ACMP2_REG            CRG_ACMP2                        ; /* 0xa78 */
    CRG_ACMP3_REG            CRG_ACMP3                        ; /* 0xa7c */
    CRG_ACMP4_REG            CRG_ACMP4                        ; /* 0xa80 */
    CRG_ACMP5_REG            CRG_ACMP5                        ; /* 0xa84 */
    CRG_ACMP6_REG            CRG_ACMP6                        ; /* 0xa88 */
    CRG_ACMP7_REG            CRG_ACMP7                        ; /* 0xa8c */
    CRG_DAC0_REG             CRG_DAC0                         ; /* 0xa90 */
    CRG_DAC1_REG             CRG_DAC1                         ; /* 0xa94 */
    CRG_DAC2_REG             CRG_DAC2                         ; /* 0xa98 */
    CRG_DAC3_REG             CRG_DAC3                         ; /* 0xa9c */
    CRG_DAC4_REG             CRG_DAC4                         ; /* 0xaa0 */
    CRG_DAC5_REG             CRG_DAC5                         ; /* 0xaa4 */
    CRG_DAC6_REG             CRG_DAC6                         ; /* 0xaa8 */
    CRG_DAC7_REG             CRG_DAC7                         ; /* 0xaac */
    CRG_DAC8_REG             CRG_DAC8                         ; /* 0xab0 */
    CRG_PGA0_REG             CRG_PGA0                         ; /* 0xab4 */
    CRG_PGA1_REG             CRG_PGA1                         ; /* 0xab8 */
    CRG_PGA2_REG             CRG_PGA2                         ; /* 0xabc */
    CRG_PGA3_REG             CRG_PGA3                         ; /* 0xac0 */
    volatile unsigned int    reserved26[15]                   ; /* 0xac4~0xafc */
    CRG_HPM_REG              CRG_HPM                          ; /* 0xb00 */
    volatile unsigned int    reserved27[254]                  ; /* 0xb04~0xef8 */
    CRG_DEBUG_REG            CRG_DEBUG                        ; /* 0xefc */
    HOSC_CTRL1_REG           HOSC_CTRL1                       ; /* 0xf00 */
    HOSC_CTRL2_REG           HOSC_CTRL2                       ; /* 0xf04 */
    HOSC_CTRL3_REG           HOSC_CTRL3                       ; /* 0xf08 */
    HOSC_PD_REG              HOSC_PD                          ; /* 0xf0c */
    HOSC_LOCK_REG            HOSC_LOCK                        ; /* 0xf10 */
} volatile CRG_RegStruct;


/**
  * @}
  */

/* Parameter Check -----------------------------------------------------------*/
/**
  * @brief Verify pll_ref_cksel configuration
  * @param clkSelect pll_ref_cksel
  * @retval true
  * @retval false
  */
static inline bool IsCrgPllRefClkSelect(CRG_PllRefClkSelect clkSelect)
{
    return ((clkSelect == CRG_PLL_REF_CLK_SELECT_HOSC) ||
            (clkSelect == CRG_PLL_REF_CLK_SELECT_XTAL));
}

/**
  * @brief Verify Crg pll_prediv configuration
  * @param preDiv pll prediv value
  * @retval true
  * @retval false
  */
static inline bool IsCrgPllPreDiv(CRG_PllPreDiv preDiv)
{
    return ((preDiv >= CRG_PLL_PREDIV_1) &&
            (preDiv <= CRG_PLL_PREDIV_8));
}

/**
  * @brief Verify Crg pll_postdiv configuration
  * @param postDiv  pll_postdiv value
  * @retval true
  * @retval false
  */
static inline bool IsCrgPllPostDiv(CRG_PllPostDiv postDiv)
{
    return ((postDiv >= CRG_PLL_POSTDIV_1) &&
            (postDiv <= CRG_PLL_POSTDIV_8));
}

/**
  * @brief Verify Crg pll_postdiv2 configuration
  * @param postDiv  pll_postdiv2 value
  * @retval true
  * @retval false
  */
static inline bool IsCrgPllPostDiv2(CRG_PllPostDiv2 postDiv)
{
    return ((postDiv >= CRG_PLL_POSTDIV2_1) &&
            (postDiv <= CRG_PLL_POSTDIV2_8));
}

/**
  * @brief Verify Crg pll_fbdiv configuration
  * @param fbDiv  pll fbdiv value
  * @retval true
  * @retval false
  */
static inline bool IsCrgPllFbDiv(unsigned int fbDiv)
{
    return (fbDiv <= CRG_PLL_FBDIV_MAX);
}

/**
  * @brief Verify Crg pll_digpostdiv_in_sel configuration
  * @param select  pll_digpostdiv_in_sel value
  * @retval true
  * @retval false
  */
static inline bool IsCrgPllDigPostDivInSel(CRG_PllDigPostDivInSelect select)
{
    return ((select == CRG_PLL_DIG_POST_DIV_SELECT_FREF) ||
            (select == CRG_PLL_DIG_POST_DIV_SELECT_PLL));
}

/**
  * @brief Verify Crg analog pll clock configuration
  * @param select  Analog pll clock select value
  * @retval true
  * @retval false
  */
static inline bool IsAnaPllClkSel(CRG_AnaClkSelect select)
{
    return ((select == CRG_ANA_SEL_PLL0) ||
            (select == CRG_ANA_SEL_PLL1));
}

/**
  * @brief Verify Crg core_cksel configuration
  * @param select  core_cksel value
  * @retval true
  * @retval false
  */
static inline bool IsCrgCoreCkSel(CRG_CoreClkSelect select)
{
    return ((select == CRG_CORE_CLK_SELECT_HOSC) ||
            (select == CRG_CORE_CLK_SELECT_TCXO) ||
            (select == CRG_CORE_CLK_SELECT_PLL));
}

/**
  * @brief Verify Crg configuration
  * @param select  1M clock selection
  * @retval true
  * @retval false
  */
static inline bool IsCrg1MCkSel(CRG_1MClkSelect select)
{
    return ((select == CRG_1M_CLK_SELECT_HOSC) ||
            (select == CRG_1M_CLK_SELECT_TCXO));
}

/**
  * @brief Verify Crg configuration
  * @param div  1M clock ratio
  * @retval true
  * @retval false
  */
static inline bool IsCrg1MCkDiv(unsigned int div)
{
    return (div <= CRG_1MHZ_CLK_MAX_DIV);
}

/**
  * @brief Verify Crg Ip (exclude adc) clock enable configuration
  * @param enable  ip clock enable value
  * @retval true
  * @retval false
  */
static inline bool IsCrgIpClkEnable(unsigned int enable)
{
    return ((enable == IP_CLK_DISABLE) ||
            (enable == IP_CLK_ENABLE));
}

/**
  * @brief Check the PLL PreDiv is valid or not
  * @param clkPllRef PLL Refer clock
  * @param preDiv PLL Previous Divsion
  * @retval true
  * @retval false
  */
static inline bool IsCrgValidPreDiv(unsigned int pllRefFreq, unsigned int preDiv)
{
    unsigned int freq = pllRefFreq;
    if (preDiv != 0) {
        freq /= preDiv;
    }
    return (freq >= CRG_CLK_PFD_MIN_FREQ) && (freq <= CRG_CLK_PFD_MAX_FREQ);
}

/**
  * @brief Check the PLL FbDiv is valid or not
  * @param clkPfdFreq PLL PFD clock
  * @param fdDiv PLL FD Divsion
  * @retval true
  * @retval false
  */
static inline bool IsCrgValidFdDiv(unsigned int clkPfdFreq, unsigned int fdDiv)
{
    /* The maximum speed of the external clock source is CRG_CLK_PFD_MAX_FREQ. */
    if (clkPfdFreq > CRG_CLK_PFD_MAX_FREQ) {
        return false;
    } else if (fdDiv > CRG_PLL_FBDIV_MAX) {
        return false;
    }

    unsigned int freq = (fdDiv > 0x6) ? (clkPfdFreq * fdDiv) : (clkPfdFreq * 0x6); /* 0x0-0x6: divided by 0x6 */
    return (freq >= CRG_CLK_VCO_MIN_FREQ) && (freq <= CRG_CLK_VCO_MAX_FREQ);
}

/**
  * @brief Check the PLL PostDiv is valid or not
  * @param clkPllRef PLL Vco clock
  * @param postDiv PLL Post Divsion
  * @retval true
  * @retval false
  */
static inline bool IsCrgValidPostDiv(unsigned int clkVcoFreq, unsigned int postDiv)
{
    unsigned int freq = clkVcoFreq;
    if (postDiv <= CRG_PLL_POSTDIV_8) {
        freq /= (postDiv + 1);
    }
    return (freq <= CRG_CLK_TARGET_MAX_FREQ);
}

/**
  * @brief Check the PLL PostDiv is valid or not
  * @param clkPllRef PLL Vco clock
  * @param postDiv2 PLL Post Divsion2
  * @retval true
  * @retval false
  */
static inline bool IsCrgValidPostDiv2(unsigned int clkVcoFreq, unsigned int postDiv2)
{
    unsigned int freq = clkVcoFreq;
    if (postDiv2 != 0) {
        freq /= (postDiv2 + 1);
    }
    return (freq <= CRG_CLK_PST2_MAX_FREQ);
}

/**
  * @brief Check the can_fd clock select value
  * @param canFdClkSelect the value of can_fd clock select
  * @retval true
  * @retval false
  */
static inline bool IsCrgCanFdClkModeSelect(SYSCTRL_CanClkSel canFdClkSelect)
{
    return (canFdClkSelect == CLK_PLL_REF_INI || \
            canFdClkSelect == CLK_PLL1_OUT);
}

/**
  * @brief Check the adc clock select value
  * @param adcClkSelect the value of adc clock select
  * @retval true
  * @retval false
  */
static inline bool IsCrgAdcClkModeSelect(CRG_AdcClkSelect adcClkSelect)
{
    return (adcClkSelect == CRG_ADC_CLK_ASYN_HOSC || \
            adcClkSelect == CRG_ADC_CLK_ASYN_TCXO || \
            adcClkSelect == CRG_ADC_CLK_ASYN_PLL_DIV || \
            adcClkSelect == CRG_ADC_CLK_SYN_CORE);
}

/**
  * @brief Check the adc clock div value
  * @param div the value of adc clock div
  * @retval true
  * @retval false
  */
static inline bool IsCrgAdcClkDiv(CRG_AdcDiv div)
{
    return (div == CRG_ADC_DIV_1 || \
            div == CRG_ADC_DIV_2 || \
            div == CRG_ADC_DIV_3 || \
            div == CRG_ADC_DIV_4);
}

/**
  * @brief Check the FMC clock select value
  * @param fmcClkSelect the value of fmc clock select
  * @retval true
  * @retval false
  */
static inline bool IsCrgFmcClkModeSelect(CRG_FmcClkSelect fmcClkSelect)
{
    return (fmcClkSelect == CRG_FMC_CLK_HOSC || \
            fmcClkSelect == CRG_FMC_CLK_TCXO || \
            fmcClkSelect == CRG_FMC_CLK_PLL_PST3 || \
            fmcClkSelect == CRG_FMC_CLK_PLL_REF);
}

/**
  * @brief Check the FMC clock div value
  * @param div the value of FMC clock div
  * @retval true
  * @retval false
  */
static inline bool IsCrgFmcClkDiv(CRG_FmcDiv div)
{
    return (div >= CRG_FMC_DIV_1 && div <= CRG_FMC_DIV_8);
}

/**
  * @brief Set Pll Ref clock select
  * @param clk     Clock register base address
  * @param clkSel  clock source select
  * @retval None
  */
static inline void DCL_CRG_SetPllRefClkSel(CRG_RegStruct *clk, CRG_PllRefClkSelect clkSel)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgPllRefClkSelect(clkSel));
    clk->CRG_CKREF_CKSEL.BIT.pll_ref_cksel = (unsigned int)clkSel;
}

/**
  * @brief Get Pll Ref clock selection
  * @param clk             Clock register base address
  * @retval pll_ref_cksel  Ref clock selection
  */
static inline CRG_PllRefClkSelect DCL_CRG_GetPllRefClkSel(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return (CRG_PllRefClkSelect)clk->CRG_CKREF_CKSEL.BIT.pll_ref_cksel;
}

/**
  * @brief Set PLL0 prevous division ratio
  * @param clk     Clock register base address
  * @param preDiv  prevous division ratio
  * @retval None
  */
static inline void DCL_CRG_SetPll0PreDiv(CRG_RegStruct *clk, CRG_PllPreDiv preDiv)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgPllPreDiv(preDiv));
    clk->CRG_PLL0_PREDIV.BIT.pll0_prediv = (unsigned int)preDiv;
}

/**
  * @brief Set PLL1 prevous division ratio
  * @param clk     Clock register base address
  * @param preDiv  prevous division ratio
  * @retval None
  */
static inline void DCL_CRG_SetPll1PreDiv(CRG_RegStruct *clk, CRG_PllPreDiv preDiv)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgPllPreDiv(preDiv));
    clk->CRG_PLL1_PREDIV.BIT.pll1_prediv = (unsigned int)preDiv;
}

/**
  * @brief Get PLL0 prevous division ratio
  * @param clk      Clock register base address
  * @retval prediv  prevous division ratio
  */
static inline CRG_PllPreDiv DCL_CRG_GetPll0PreDiv(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return (CRG_PllPreDiv)clk->CRG_PLL0_PREDIV.BIT.pll0_prediv;
}

/**
  * @brief Get PLL1 prevous division ratio
  * @param clk      Clock register base address
  * @retval prediv  prevous division ratio
  */
static inline CRG_PllPreDiv DCL_CRG_GetPll1PreDiv(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return (CRG_PllPreDiv)clk->CRG_PLL1_PREDIV.BIT.pll1_prediv;
}

/**
  * @brief Set PLL0 frequency multiplication factor
  * @param clk    Clock register base address
  * @param fbDiv  Multiplication factor
  * @retval None
  */
static inline void DCL_CRG_SetPll0FbDiv(CRG_RegStruct *clk, unsigned int fbDiv)
{
    unsigned int div = fbDiv;
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgPllFbDiv(fbDiv));
    clk->CRG_PLL0_FBDIV.BIT.pll0_fbdiv = div;
}

/**
  * @brief Set PLL1 frequency multiplication factor
  * @param clk    Clock register base address
  * @param fbDiv  Multiplication factor
  * @retval None
  */
static inline void DCL_CRG_SetPll1FbDiv(CRG_RegStruct *clk, unsigned int fbDiv)
{
    unsigned int div = fbDiv;
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgPllFbDiv(fbDiv));
    clk->CRG_PLL1_FBDIV.BIT.pll1_fbdiv = div;
}

/**
  * @brief Get PLL0 frequency multiplication factor
  * @param clk         Clock register base address
  * @retval pll_fbdiv  Multiplication factor
  */
static inline unsigned int DCL_CRG_GetPll0FbDiv(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_PLL0_FBDIV.BIT.pll0_fbdiv;
}

/**
  * @brief Get PLL1 frequency multiplication factor
  * @param clk         Clock register base address
  * @retval pll_fbdiv  Multiplication factor
  */
static inline unsigned int DCL_CRG_GetPll1FbDiv(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_PLL1_FBDIV.BIT.pll1_fbdiv;
}

/**
  * @brief Set PLL0 post division ratio
  * @param clk     Clock register base address
  * @param postDiv Post division ratio
  * @retval None
  */
static inline void DCL_CRG_SetPll0PostDiv1(CRG_RegStruct *clk, CRG_PllPostDiv postDiv)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgPllPostDiv(postDiv));
    clk->CRG_PLL0_PSTDIV.BIT.pll0_postdiv1 = (unsigned int)postDiv;
}

/**
  * @brief Set PLL1 post division ratio
  * @param clk     Clock register base address
  * @param postDiv Post division ratio
  * @retval None
  */
static inline void DCL_CRG_SetPll1PostDiv1(CRG_RegStruct *clk, CRG_PllPostDiv postDiv)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgPllPostDiv(postDiv));
    clk->CRG_PLL1_PSTDIV.BIT.pll1_postdiv1 = (unsigned int)postDiv;
}

/**
  * @brief Get PLL0 post division ratio
  * @param clk           Clock register base address
  * @retval pll_postdiv  Post division ratio
  */
static inline CRG_PllPostDiv DCL_CRG_GetPll0PostDiv1(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return (CRG_PllPostDiv)clk->CRG_PLL0_PSTDIV.BIT.pll0_postdiv1;
}

/**
  * @brief Get PLL1 post division ratio
  * @param clk           Clock register base address
  * @retval pll_postdiv  Post division ratio
  */
static inline CRG_PllPostDiv DCL_CRG_GetPll1PostDiv1(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return (CRG_PllPostDiv)clk->CRG_PLL1_PSTDIV.BIT.pll1_postdiv1;
}

/**
  * @brief Set PLL0 post division ratio
  * @param clk     Clock register base address
  * @param postDiv Post division ratio
  * @retval None
  */
static inline void DCL_CRG_SetPll0PostDiv2(CRG_RegStruct *clk, CRG_PllPostDiv postDiv)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgPllPostDiv(postDiv));
    clk->CRG_PLL0_PSTDIV.BIT.pll0_postdiv2 = (unsigned int)postDiv;
}

/**
  * @brief Set PLL0 post division ratio
  * @param clk     Clock register base address
  * @param postDiv Post division ratio
  * @retval None
  */
static inline void DCL_CRG_SetPll1PostDiv2(CRG_RegStruct *clk, CRG_PllPostDiv postDiv)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgPllPostDiv(postDiv));
    clk->CRG_PLL1_PSTDIV.BIT.pll1_postdiv2 = (unsigned int)postDiv;
}

/**
  * @brief Get PLL1 post division ratio
  * @param clk           Clock register base address
  * @retval pll_postdiv  Post division ratio
  */
static inline CRG_PllPostDiv DCL_CRG_GetPll1PostDiv2(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return (CRG_PllPostDiv)clk->CRG_PLL1_PSTDIV.BIT.pll1_postdiv2;
}

/**
  * @brief Get PLL0 post division ratio
  * @param clk           Clock register base address
  * @retval pll_postdiv  Post division ratio
  */
static inline CRG_PllPostDiv DCL_CRG_GetPll0PostDiv2(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return (CRG_PllPostDiv)clk->CRG_PLL0_PSTDIV.BIT.pll0_postdiv2;
}

/**
  * @brief Set PLL0 Power
  * @param clk  Clock register base address
  * @param pd   pll power down or not
  * @retval None
  */
static inline void DCL_CRG_SetPll0Pd(CRG_RegStruct *clk, bool pd)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    clk->CRG_PLL0_PD.BIT.pll0_pd = (unsigned int)pd;
}

/**
  * @brief Set PLL1 Power
  * @param clk  Clock register base address
  * @param pd   pll power down or not
  * @retval None
  */
static inline void DCL_CRG_SetPll1Pd(CRG_RegStruct *clk, bool pd)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    clk->CRG_PLL1_PD.BIT.pll1_pd = (unsigned int)pd;
}

/**
  * @brief Get PLL0 power status
  * @param clk  Clock register base address
  * @retval 0: power up, 1: power down
  */
static inline bool DCL_CRG_GetPll0Pd(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_PLL0_PD.BIT.pll0_pd;
}

/**
  * @brief Get PLL1 power status
  * @param clk  Clock register base address
  * @retval 0: power up, 1: power down
  */
static inline bool DCL_CRG_GetPll1Pd(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_PLL1_PD.BIT.pll1_pd;
}

/**
  * @brief Set analog pll clock selection
  * @param clk  Clock register base address
  * @param select  Analog pll clock selection
  * @retval None
  */
static inline void DCL_CRG_SetAnaPllCkSel(CRG_RegStruct *clk, CRG_AnaClkSelect select)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsAnaPllClkSel(select));
    clk->CRG_ANA_PLL_CKSEL.BIT.ana_pll_cksel = select;
}

/**
  * @brief Get analog pll clock selection
  * @param clk  Clock register base address
  * @retval Analog pll clock selection
  */
static inline unsigned int DCL_CRG_GetAnaPllCkSel(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_ANA_PLL_CKSEL.BIT.ana_pll_cksel;
}

/**
  * @brief Set core clock selection
  * @param clk  Clock register base address
  * @param select  Core clock selection
  * @retval None
  */
static inline void DCL_CRG_SetCoreClkSel(CRG_RegStruct *clk, CRG_CoreClkSelect select)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgCoreCkSel(select));
    clk->CRG_SYS_CKSEL.BIT.clk_pst1_sw_sel = select;
}

/**
  * @brief Get core clock selection
  * @param clk  Clock register base address
  * @retval Core clock selection
  */
static inline unsigned int DCL_CRG_GetCoreClkSel(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_SYS_CKSEL.BIT.clk_pst1_sw_sel;
}

/**
  * @brief Set core clock selection
  * @param clk  Clock register base address
  * @param select  Core clock selection
  * @retval None
  */
static inline void DCL_CRG_SetAdcAsynClkSel(CRG_RegStruct *clk, CRG_CoreClkSelect select)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgCoreCkSel(select));
    clk->CRG_SYS_CKSEL.BIT.clk_pst2_sw_sel = select;
}

/**
  * @brief Get adc core clock selection
  * @param clk  Clock register base address
  * @retval Core clock selection
  */
static inline unsigned int DCL_CRG_GetAdcAsynClkSel(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_SYS_CKSEL.BIT.clk_pst2_sw_sel;
}

/**
  * @brief Set FMC core clock selection
  * @param clk  Clock register base address
  * @param select  Core clock selection
  * @retval None
  */
static inline void DCL_CRG_SetFmcAsynClkSel(CRG_RegStruct *clk, CRG_CoreClkSelect select)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrgCoreCkSel(select));
    clk->CRG_SYS_CKSEL.BIT.clk_pst3_sw_sel = select;
}

/**
  * @brief Get FMC core clock selection
  * @param clk  Clock register base address
  * @retval Core clock selection
  */
static inline unsigned int DCL_CRG_GetFmcAsynClkSel(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_SYS_CKSEL.BIT.clk_pst3_sw_sel;
}

/**
  * @brief Set 1M clock selection
  * @param clk  Clock register base address
  * @param select  Core clock selection
  * @retval None
  */
static inline void DCL_CRG_Set1MClkSel(CRG_RegStruct *clk, CRG_1MClkSelect select)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsCrg1MCkSel(select));
    clk->CRG_1M_INI_CKSEL.BIT.clk_1m_ini_cksel = select;
}

/**
  * @brief Get 1M clock selection
  * @param clk  Clock register base address
  * @retval Core clock selection
  */
static inline unsigned int DCL_CRG_Get1MClkSel(const CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_1M_INI_CKSEL.BIT.clk_1m_ini_cksel;
}

/**
  * @brief Set 1M clock division ratio
  * @param clk  Clock register base address
  * @param div  Division ratio
  * @retval None
  */
static inline void DCL_CRG_Set1MClkDiv(CRG_RegStruct *clk, unsigned int div)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(div <= CRG_1MHZ_CLK_MAX_DIV);
    clk->CRG_1M_DIV.BIT.clk_1m_div = div;
}

/**
  * @brief Get 1M clock division ratio
  * @param clk  Clock register base address
  * @retval 1M clock division ratio
  */
static inline unsigned int DCL_CRG_Get1MClkDiv(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_1M_DIV.BIT.clk_1m_div;
}

/**
  * @brief  Enable test clock function
  * @param clk  Clock register base address
  * @retval None
  */
static inline void DCL_CRG_TestClkEnable(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    clk->CRG_TEST_CKSEL.BIT.test_clk_en = BASE_CFG_ENABLE;   /* Enable the test clock. */
}

/**
  * @brief  Disable test clock function
  * @param clk  Clock register base address
  * @retval None
  */
static inline void DCL_CRG_TestClkDisable(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    clk->CRG_TEST_CKSEL.BIT.test_clk_en = BASE_CFG_DISABLE;  /* Disable the test clock. */
}

/**
  * @brief CRG test clock select.
  * @param clk  Clock register base address
  * @param clkSel  Clock select.
  * @retval None
  */
static inline void DCL_CRG_TestClkSel(CRG_RegStruct *clk, CRG_TestClkSel clkSel)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(clkSel >= CRG_TEST_CLK_HOSC);
    CRG_PARAM_CHECK_NO_RET(clkSel <= CRG_TEST_CLK_HOSC_DIV);
    clk->CRG_TEST_CKSEL.BIT.test_clk_sel = clkSel; /* Set the test clock select. */
}

/**
  * @brief Lock core in reset status.
  * @param clk  Clock register base address
  * @param chipId  core id @ref ChipCoreId
  * @retval None
  */
static inline void DCL_CRG_LockCoreResetStatus(CRG_RegStruct *clk, ChipCoreId chipId)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsChipCoreId(chipId));
    if (chipId == CHIP_CORE_0) {     /* current core id is Core 0 */
        clk->CRG_CPU0.BIT.cpu0_srst_req = BASE_CFG_SET;
    } else if (chipId == CHIP_CORE_1) {   /* current core id is Core 1 */
        clk->CRG_CPU1.BIT.cpu1_srst_req = BASE_CFG_SET;
    } else if (chipId == CHIP_CORE_2) {     /* current core id is Core 2 */
        clk->CRG_CPU2.BIT.cpu2_srst_req = BASE_CFG_SET;
    }
}

/**
  * @brief Unlock core in reset status.
  * @param clk  Clock register base address
  * @param chipId  core id @ref ChipCoreId
  * @retval None
  */
static inline void DCL_CRG_UnLockCoreResetStatus(CRG_RegStruct *clk, ChipCoreId chipId)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsChipCoreId(chipId));
    if (chipId == CHIP_CORE_0) {       /* current core id is Core 0 */
        clk->CRG_CPU0.BIT.cpu0_srst_req = BASE_CFG_UNSET;
    } else if (chipId == CHIP_CORE_1) {  /* current core id is Core 1 */
        clk->CRG_CPU1.BIT.cpu1_srst_req = BASE_CFG_UNSET;
    } else if (chipId == CHIP_CORE_2) {    /* current core id is Core 2 */
        clk->CRG_CPU2.BIT.cpu2_srst_req = BASE_CFG_UNSET;
    }
}

/**
  * @brief Enable core clock.
  * @param clk  Clock register base address
  * @param chipId  core id @ref ChipCoreId
  * @retval None
  */
static inline void DCL_CRG_EnableCoreClk(CRG_RegStruct *clk, ChipCoreId chipId)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsChipCoreId(chipId));
    if (chipId == CHIP_CORE_0) {   /* current core id is Core 0 */
        clk->CRG_CPU0.BIT.cpu0_cken = BASE_CFG_SET;
    } else if (chipId == CHIP_CORE_1) { /* current core id is Core 1 */
        clk->CRG_CPU1.BIT.cpu1_cken = BASE_CFG_SET;
    } else if (chipId == CHIP_CORE_2) {  /* current core id is Core 2 */
        clk->CRG_CPU2.BIT.cpu2_cken = BASE_CFG_SET;
    }
}

/**
  * @brief Disable core clock.
  * @param clk  Clock register base address
  * @param chipId  core id @ref ChipCoreId
  * @retval None
  */
static inline void DCL_CRG_DisableCoreClk(CRG_RegStruct *clk, ChipCoreId chipId)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    CRG_PARAM_CHECK_NO_RET(IsChipCoreId(chipId));
    if (chipId == CHIP_CORE_0) {  /* current core id is Core 0 */
        clk->CRG_CPU0.BIT.cpu0_cken = BASE_CFG_UNSET;
    } else if (chipId == CHIP_CORE_1) { /* current core id is Core 1 */
        clk->CRG_CPU1.BIT.cpu1_cken = BASE_CFG_UNSET;
    } else if (chipId == CHIP_CORE_2) {  /* current core id is Core 2 */
        clk->CRG_CPU2.BIT.cpu2_cken = BASE_CFG_UNSET;
    }
}

/**
  * @brief Clock Invert phase set.
  * @param clk  Clock register base address
  * @param invPhase  Clock Invert phase
  * @retval None
  */
static inline void DCL_CRG_SetAdc0ClkInvPhase(CRG_RegStruct *clk, bool invPhase)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    clk->CRG_ADC0_CLK_PCTL.BIT.adc0_CLK_PCTL = invPhase;
}

/**
  * @brief Clock Invert phase get.
  * @param clk  Clock register base address
  * @retval bool Invert status
  */
static inline bool DCL_CRG_GetAdc0ClkInvPhase(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_ADC0_CLK_PCTL.BIT.adc0_CLK_PCTL;
}

/**
  * @brief Clock Invert phase set.
  * @param clk  Clock register base address
  * @param invPhase  Clock Invert phase
  * @retval None
  */
static inline void DCL_CRG_SetAdc1ClkInvPhase(CRG_RegStruct *clk, bool invPhase)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    clk->CRG_ADC1_CLK_PCTL.BIT.adc1_CLK_PCTL = invPhase;
}

/**
  * @brief Clock Invert phase get.
  * @param clk  Clock register base address
  * @retval bool Invert status
  */
static inline bool DCL_CRG_GetAdc1ClkInvPhase(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_ADC1_CLK_PCTL.BIT.adc1_CLK_PCTL;
}

/**
  * @brief Clock Invert phase set.
  * @param clk  Clock register base address
  * @param invPhase  Clock Invert phase
  * @retval None
  */
static inline void DCL_CRG_SetAdc2ClkInvPhase(CRG_RegStruct *clk, bool invPhase)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    clk->CRG_ADC2_CLK_PCTL.BIT.adc2_CLK_PCTL = invPhase;
}

/**
  * @brief Clock Invert phase get.
  * @param clk  Clock register base address.
  * @retval bool Invert status.
  */
static inline bool DCL_CRG_GetAdc2ClkInvPhase(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_ADC2_CLK_PCTL.BIT.adc2_CLK_PCTL;
}

/**
  * @brief Clock Invert phase set.
  * @param clk  Clock register base address.
  * @param invPhase  Clock Invert phase.
  * @retval None.
  */
static inline void DCL_CRG_SetAdc3ClkInvPhase(CRG_RegStruct *clk, bool invPhase)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    clk->CRG_ADC3_CLK_PCTL.BIT.adc3_CLK_PCTL = invPhase;
}

/**
  * @brief Clock Invert phase get.
  * @param clk  Clock register base address
  * @retval bool Invert status
  */
static inline bool DCL_CRG_GetAdc3ClkInvPhase(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    return clk->CRG_ADC3_CLK_PCTL.BIT.adc3_CLK_PCTL;
}
/**
  * @brief ADDLL CLKIN clock enable set.
  * @param clk  Clock register base address
  * @param enable  Clock enable
  * @retval None
  */
static inline void DCL_CRG_SetAddllClkinClkEnable(CRG_RegStruct *clk, bool enable)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    if (enable & IP_CLK_ENABLE) {     /* Set enable of target ip. */
        clk->CRG_ADDLL_CLKIN.BIT.addll_clkin_cken = BASE_CFG_SET;
    } else {     /* Set disable of target ip. */
        clk->CRG_ADDLL_CLKIN.BIT.addll_clkin_cken = BASE_CFG_UNSET;
    }
}

/**
  * @brief ADDLL CLKIN clock enable get.
  * @param clk  Clock register base address
  * @retval bool, clock enbale status
  */
static inline bool DCL_CRG_GetAddllClkinClkEnable(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    /* return clock enbale status. */
    return (clk->CRG_ADDLL_CLKIN.BIT.addll_clkin_cken);
}

/**
  * @brief DLL DIG clock enable set.
  * @param clk  Clock register base address
  * @param enable  Clock enable
  * @retval None
  */
static inline void DCL_CRG_SetDllDigClkEnable(CRG_RegStruct *clk, bool enable)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    if (enable & IP_CLK_ENABLE) {     /* Set enable of target ip. */
        clk->CRG_DLL_DIG.BIT.dll_dig_cken = BASE_CFG_SET;
        clk->CRG_DLL_DIG.BIT.dll_dig_srst_req = BASE_CFG_UNSET;
    } else {     /* Set disable of target ip. */
        clk->CRG_DLL_DIG.BIT.dll_dig_cken = BASE_CFG_UNSET;
        clk->CRG_DLL_DIG.BIT.dll_dig_srst_req = BASE_CFG_SET;
    }
}

/**
  * @brief DLL DIG clock enable get.
  * @param clk  Clock register base address
  * @retval bool, clock enbale status
  */
static inline bool DCL_CRG_GetDllDigClkEnable(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    /* return clock enbale status. */
    return (clk->CRG_DLL_DIG.BIT.dll_dig_cken);
}

/**
  * @brief DLL DIG clock reset set.
  * @param clk  Clock register base address
  * @param reset  Clock reset
  * @retval None
  */
static inline void DCL_CRG_SetDllDigClkReset(CRG_RegStruct *clk, bool reset)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    if (reset & IP_CLK_ENABLE) {     /* Set reset of target ip. */
        clk->CRG_DLL_DIG.BIT.dll_dig_srst_req = BASE_CFG_SET;
    } else {     /* Unset reset of target ip. */
        clk->CRG_DLL_DIG.BIT.dll_dig_srst_req = BASE_CFG_UNSET;
    }
}

/**
  * @brief DLL DIG clock reset get.
  * @param clk  Clock register base address
  * @retval bool, clock reset status
  */
static inline bool DCL_CRG_GetDllDigClkReset(CRG_RegStruct *clk)
{
    CRG_ASSERT_PARAM(IsCRGInstance(clk));
    /* return clock reset status. */
    return (clk->CRG_DLL_DIG.BIT.dll_dig_srst_req);
}

/**
  * @}
  */

/**
 * @}
 */
#endif /* McuMagicTag_CRG_IP_H */


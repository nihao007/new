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
  * @file      sysctrl.h
  * @author    MCU Driver Team
  * @brief     This file provides firmware functions to manage the following
  *            functionalities of the system control register.
  *                + Register Struct of SYSCTRL
  */

/* Define to prevent recursive inclusion ------------------------------------- */
#ifndef McuMagicTag_SYSCTRL_H
#define McuMagicTag_SYSCTRL_H

/* Includes ------------------------------------------------------------------ */
#include "baseaddr.h"
#include "typedefs.h"
#include "info.h"

/* Macro definitions ---------------------------------------------------------*/
#define SC_LOCKEN_VALID_HIGH_BIT 0xEA510000U /**< Upper 16 active bits of the SC_LOCKEN register */
#define SC_LOW_BIT_MASK 0x0000FFFFU /**< Obtains the mask of the lower 16 bits. */
#define SC_LOCKEN_CRG_DISABLE_MASK 0x0000FFFEU /**< CRG write protection disable mask in SC_LOCKEN */
#define SC_LOCKEN_CRG_ENABLE_MASK 0x00000001U /**< CRG write protection enable mask in SC_LOCKEN */
#define SC_LOCKEN_SC_DISABLE_MASK 0x0000FFFDU /**< SC write protection disable mask in SC_LOCKEN */
#define SC_LOCKEN_SC_ENABLE_MASK 0x00000002U /**< SC write protection enbale mask in SC_LOCKEN */

#define GPIO_DFX_ENABLE_MASK    0x1FFFF
#define ACCESS_RIGHT_SADDR 0xA060
#define ACCESS_RIGHT_EADDR 0xA08C
#define ONE_PERI_OFFSET_BIT     8
#define ONE_REG_BYTE_NUM        4
#define ONE_REG_CFG_PERI_NUM    4
#define MASTER_ACCESS_PERMISSION_MASK 0x1F

#define APT_DEBUG_MODE_ADDR_OFFSET 0x8060
#define QDM_DEBUG_MODE_ADDR_OFFSET 0x80A0
#define CAPM_DEBUG_MODE_ADDR_OFFSET 0x80B0
#define DAC_DEBUG_MODE_ADDR_OFFSET 0x80BC
#define TIMER_DEBUG_MODE_ADDR_OFFSET 0x8180

/**
 * @brief Writes a value to a hardware register
 * @param addr The address of the register
 * @param val The value to be written
 */
static void WriteRegister(unsigned int addr, unsigned int val)
{
    (*(volatile unsigned int*)(void *)(uintptr_t)(addr) = (val));
}

/**
 * @brief Reads a value from a hardware register
 * @param addr The address of the register
 * @return The value read from the register
 */
static unsigned int ReadRegister(unsigned int addr)
{
    return (*(volatile unsigned int*)(void *)(uintptr_t)(addr));
}

/**
  * @brief Records the offsets of various states in the CPU status register.
  */
typedef enum {
    SYSCTRL_NMI_BIT            = 0x00000000U,
    SYSCTRL_HARD_FAULT_BIT     = 0x00000003U,
    SYSCTRL_DEBUG_BIT          = 0x00000004U,
    SYSCTRL_SLEEP_BIT          = 0x00000005U,
    SYSCTRL_CORE_BIT           = 0x00000006U,
    SYSCTRL_PC_VALID_BIT       = 0x0000001FU,
    SYSCTRL_MAX_BIT            = SYSCTRL_PC_VALID_BIT
} SYSCTRL_CpuStatus;

/**
  * @brief Peripheral access permission control.
  */
typedef enum {
    CMM0_MST_MASK_CFG       = 0,
    CMM1_MST_MASK_CFG       = 1,
    CFD_MST_MASK_CFG        = 2,
    DAC_PGA_MST_MASK_CFG    = 3,
    MTIMER0_MST_MASK_CFG    = 4,
    MTIMER1_MST_MASK_CFG    = 5,
    MTIMER2_MST_MASK_CFG    = 6,
    TIMER0_MST_MASK_CFG     = 7,
    TIMER1_MST_MASK_CFG     = 8,
    TIMER2_MST_MASK_CFG     = 9,
    TIMER3_MST_MASK_CFG     = 10,
    TIMER4_MST_MASK_CFG     = 11,
    TIMER5_MST_MASK_CFG     = 12,
    I2C0_MST_MASK_CFG       = 13,
    I2C1_MST_MASK_CFG       = 14,
    I2C2_MST_MASK_CFG       = 15,
    SPI0_MST_MASK_CFG       = 16,
    SPI1_MST_MASK_CFG       = 17,
    SPI2_MST_MASK_CFG       = 18,
    FOTPC_MST_MASK_CFG      = 19,
    CAN0_MST_MASK_CFG       = 20,
    CAN1_MST_MASK_CFG       = 21,
    CAN2_MST_MASK_CFG       = 22,
    GPT0_MST_MASK_CFG       = 23,
    APT0_5_MST_MASK_CFG     = 24,
    APT6_11_MST_MASK_CFG    = 25,
    APT12_15_MST_MASK_CFG   = 26,
    GPT1_MST_MASK_CFG       = 27,
    ADC0_MST_MASK_CFG       = 28,
    ADC1_MST_MASK_CFG       = 29,
    ADC2_MST_MASK_CFG       = 30,
    ADC3_MST_MASK_CFG       = 31,
    IWDG_MST_MASK_CFG       = 32,
    WWDG0_MST_MASK_CFG      = 33,
    WWDG1_MST_MASK_CFG      = 34,
    WWDG2_MST_MASK_CFG      = 35,
    SYSCTRl0_MST_MASK_CFG   = 36,
    SYSCTRl1_MST_MASK_CFG   = 37,
    CRG_MST_MASK_CFG        = 38,
    AON_REG_MST_MASK_CFG    = 39,
    CAPM0_2_MST_MASK_CFG    = 40,
    QDM0_MST_MASK_CFG       = 41,
    QDM1_MST_MASK_CFG       = 42,
    QDM2_MST_MASK_CFG       = 43,
    QDM3_MST_MASK_CFG       = 44,
} SYSCTRL_Peripheral;

/**
  * @brief Peripheral access permission type.
  */
typedef enum {
    CPU0_MST_ACCESS_PERMISSION_ENBALE      = 0x01,
    CPU1_MST_ACCESS_PERMISSION_ENBALE      = 0x02,
    CPU2_MST_ACCESS_PERMISSION_ENBALE      = 0x04,
    DMA0_MST_ACCESS_PERMISSION_ENBALE      = 0x08,
    DMA1_MST_ACCESS_PERMISSION_ENBALE      = 0x10,
} SYSCTRL_Permission;

/**
 * @brief Enumeration for selecting DMA0 16 request sources
 */
typedef enum {
    DMA0_REQ16_SEL_CAMP0      = 0x00,
    DMA0_REQ16_SEL_ADC1_1     = 0x01,
} SYSCTRL_Dma0Req16Sel;

/**
 * @brief Enumeration for selecting DMA0 17 request sources
 */
typedef enum {
    DMA0_REQ17_SEL_CAMP1      = 0x00,
    DMA0_REQ17_SEL_ADC0_2     = 0x01,
} SYSCTRL_Dma0Req17Sel;

/**
 * @brief Enumeration for selecting DMA0 18 request sources
 */
typedef enum {
    DMA0_REQ18_SEL_CAMP2      = 0x00,
    DMA0_REQ18_SEL_ADC1_2     = 0x01,
} SYSCTRL_Dma0Req18Sel;

/**
  * @brief CANFD_CLK_SEL register function item.
  */
typedef enum {
    CLK_PLL_REF_INI = 0x00000000U,
    CLK_PLL1_OUT    = 0x00000001U
} SYSCTRL_CanClkSel;

/**
 * @brief Enumeration for CPU_ITCM_DTCM_ERR_CFG register function item.
 */
typedef enum {
    NO_RETURN_ERROR = 0x00000000U,
    RETURN_ERROR    = 0x00000001U
} SYSCTRL_CpuItcmDtcmErrCfg;

/**
 * @brief Enumeration for debug mode source.
 */
typedef enum {
    DEBUG_MODE_SOURCE_CPU0    = 0x00000000U,
    DEBUG_MODE_SOURCE_CPU1    = 0x00000001U,
    DEBUG_MODE_SOURCE_CPU2    = 0x00000002U,
    DEBUG_MODE_SOURCE_ERROR   = 0x00000003U,
} SYSCTRL_DebugModeSource;

/**
 * @brief Enumeration for selecting the debug mode source APT index
 */
typedef enum {
    DEBUG_MODE_SOURCE_APT0     = 0,
    DEBUG_MODE_SOURCE_APT1     = 1,
    DEBUG_MODE_SOURCE_APT2     = 2,
    DEBUG_MODE_SOURCE_APT3     = 3,
    DEBUG_MODE_SOURCE_APT4     = 4,
    DEBUG_MODE_SOURCE_APT5     = 5,
    DEBUG_MODE_SOURCE_APT6     = 6,
    DEBUG_MODE_SOURCE_APT7     = 7,
    DEBUG_MODE_SOURCE_APT8     = 8,
    DEBUG_MODE_SOURCE_APT9     = 9,
    DEBUG_MODE_SOURCE_APT10    = 10,
    DEBUG_MODE_SOURCE_APT11    = 11,
    DEBUG_MODE_SOURCE_APT12    = 12,
    DEBUG_MODE_SOURCE_APT13    = 13,
    DEBUG_MODE_SOURCE_APT14    = 14,
    DEBUG_MODE_SOURCE_APT15    = 15,
    DEBUG_MODE_SOURCE_APT_MAX  = DEBUG_MODE_SOURCE_APT15,
} SYSCTRL_DebugModeAptIndex;

/**
 * @brief Enumeration for selecting the debug mode source CAPM index
 */
typedef enum {
    DEBUG_MODE_SOURCE_CAPM0     = 0,
    DEBUG_MODE_SOURCE_CAPM1     = 1,
    DEBUG_MODE_SOURCE_CAPM2     = 2,
    DEBUG_MODE_SOURCE_CAPM_MAX     = DEBUG_MODE_SOURCE_CAPM2,
} SYSCTRL_DebugModeCapmIndex;

/**
 * @brief Enumeration for selecting the debug mode source DAC index
 */
typedef enum {
    DEBUG_MODE_SOURCE_DAC0     = 0,
    DEBUG_MODE_SOURCE_DAC1     = 1,
    DEBUG_MODE_SOURCE_DAC2     = 2,
    DEBUG_MODE_SOURCE_DAC3     = 3,
    DEBUG_MODE_SOURCE_DAC4     = 4,
    DEBUG_MODE_SOURCE_DAC5     = 5,
    DEBUG_MODE_SOURCE_DAC6     = 6,
    DEBUG_MODE_SOURCE_DAC7     = 7,
    DEBUG_MODE_SOURCE_DAC8     = 8,
    DEBUG_MODE_SOURCE_DAC_MAX     = DEBUG_MODE_SOURCE_DAC8,
} SYSCTRL_DebugModeDacIndex;

/**
 * @brief Enumeration for selecting the debug mode source QDM index
 */
typedef enum {
    DEBUG_MODE_SOURCE_QDM0     = 0,
    DEBUG_MODE_SOURCE_QDM1     = 1,
    DEBUG_MODE_SOURCE_QDM2     = 2,
    DEBUG_MODE_SOURCE_QDM3     = 3,
    DEBUG_MODE_SOURCE_QDM_MAX     = DEBUG_MODE_SOURCE_QDM3,
} SYSCTRL_DebugModeQdmIndex;

/**
 * @brief Enumeration for selecting the debug mode source TIMER index
 */
typedef enum {
    DEBUG_MODE_SOURCE_TIMER0     = 0,
    DEBUG_MODE_SOURCE_TIMER1     = 1,
    DEBUG_MODE_SOURCE_TIMER2     = 2,
    DEBUG_MODE_SOURCE_TIMER3     = 3,
    DEBUG_MODE_SOURCE_TIMER4     = 4,
    DEBUG_MODE_SOURCE_TIMER5     = 5,
    DEBUG_MODE_SOURCE_TIMER_MAX     = DEBUG_MODE_SOURCE_TIMER5,
} SYSCTRL_DebugModeTimerIndex;

/**
 * @brief Enumeration for selecting the GPIO DFX (Design-for-Test) index
 */
typedef enum {
    GPIO0_DFX_EN = 0,
    GPIO1_DFX_EN = 1,
    GPIO2_DFX_EN = 2,
    GPIO3_DFX_EN = 3,
    GPIO4_DFX_EN = 4,
    GPIO5_DFX_EN = 5,
    GPIO6_DFX_EN = 6,
    GPIO7_DFX_EN = 7,
    GPIO8_DFX_EN = 8,
    GPIO9_DFX_EN = 9,
    GPIO10_DFX_EN = 10,
    GPIO11_DFX_EN = 11,
    GPIO12_DFX_EN = 12,
    GPIO13_DFX_EN = 13,
    GPIO14_DFX_EN = 14,
    GPIO15_DFX_EN = 15,
    GPIO16_DFX_EN = 16,
} SYSCTRL_GpioDfxIndex;

/**
 * @brief Enumeration for JTAG LOCK status
 */
typedef enum {
    JTAG_LOCK_STATUS_LOCK   = 0, /* The current JTAG channel is locked and cannot be accessed. */
    JTAG_LOCK_STATUS_UNLOCK = 1, /* The current JTAG channel is unlocked and can be accessed. */
} JTAG_LockStatus;

/**
 * @brief Struct for JTAG LOCK password
 */
typedef struct {
    unsigned int passWord0;
    unsigned int passWord1;
    unsigned int passWord2;
    unsigned int passWord3;
} JTAG_LockPassWord;

/**
  * @brief System status register.
  */
/* Define the union SC_STATUS */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    bootrom_finish        : 1   ;
        unsigned int    reserved_0            : 31  ;
    } BIT;
} volatile SC_STATUS_REG;

/* Define the union SC_SYS_RES */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    softresreq            : 1   ;
        unsigned int    reserved_0            : 31  ;
    } BIT;
} volatile SC_SYS_RES_REG;

/* Define the union SC_RST_CNT0 */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    soft_rst_cnt          : 16  ;
        unsigned int    ext_rst_cnt           : 16  ;
    } BIT;
} volatile SC_RST_CNT0_REG;

/* Define the union SC_RST_CNT1 */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    wdg0_rst_cnt          : 16  ;
        unsigned int    iwdg_rst_cnt          : 16  ;
    } BIT;
} volatile SC_RST_CNT1_REG;

/* Define the union SC_RST_CNT2 */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    wdg1_rst_cnt          : 16  ;
        unsigned int    wdg2_rst_cnt          : 16  ;
    } BIT;
} volatile SC_RST_CNT2_REG;

/* Define the union SC_SYS_STAT */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    update_mode_clear     : 1   ;
        unsigned int    reserved_0            : 3   ;
        unsigned int    update_mode           : 1   ;
        unsigned int    reserved_1            : 27  ;
    } BIT;
} volatile SC_SYS_STAT_REG;

/* Define the union CPU0_SC_SOFT_INT */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    cpu0_software_int     : 1   ;
        unsigned int    reserved_0            : 31  ;
    } BIT;
} volatile CPU0_SC_SOFT_INT_REG;

/* Define the union CPU1_SC_SOFT_INT */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    cpu1_software_int     : 1   ;
        unsigned int    reserved_0            : 31  ;
    } BIT;
} volatile CPU1_SC_SOFT_INT_REG;

/* Define the union CPU2_SC_SOFT_INT */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    cpu2_software_int     : 1   ;
        unsigned int    reserved_0            : 31  ;
    } BIT;
} volatile CPU2_SC_SOFT_INT_REG;

/* Define the union SC_LOCKEN */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    crg_cfg_lock          : 1   ;
        unsigned int    sc_cfg_lock           : 1   ;
        unsigned int    reserved_0            : 30  ;
    } BIT;
} volatile SC_LOCKEN_REG;

/* Define the union MEM_CTRL */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    bootram_mem_ctrl      : 16  ;
        unsigned int    bootrom_mem_ctrl      : 8   ;
        unsigned int    reserved_0            : 8   ;
    } BIT;
} volatile MEM_CTRL_REG;

/* Define the union CANFD_CLK_SEL */
typedef union {
    unsigned int reg; /* Replaced u32 with reg */
    struct {
        unsigned int    can_fd_clk_sel        : 1   ;
        unsigned int    reserved_1            : 31  ;
    } BIT;
} volatile CANFD_CLK_SEL_REG;

/**
  * @brief SYSCTRL0 register.
  */
typedef struct _SYSCTRL0_Regstruct {
    SC_STATUS_REG            SC_STATUS                        ; /* 0x0 */
    SC_SYS_RES_REG           SC_SYS_RES                       ; /* 0x4 */
    SC_RST_CNT0_REG          SC_RST_CNT0                      ; /* 0x8 */
    SC_RST_CNT1_REG          SC_RST_CNT1                      ; /* 0xc */
    SC_RST_CNT2_REG          SC_RST_CNT2                      ; /* 0x10 */
    unsigned int             reserved_0                       ; /* 0x14 */
    SC_SYS_STAT_REG          SC_SYS_STAT                      ; /* 0x18 */
    unsigned int             reserved_1                       ; /* 0x1c */
    CPU0_SC_SOFT_INT_REG     CPU0_SC_SOFT_INT                 ; /* 0x20 */
    unsigned int             CPU0_SC_SOFT_EVT_ID              ; /* 0x24 */
    CPU1_SC_SOFT_INT_REG     CPU1_SC_SOFT_INT                 ; /* 0x28 */
    unsigned int             CPU1_SC_SOFT_EVT_ID              ; /* 0x2c */
    CPU2_SC_SOFT_INT_REG     CPU2_SC_SOFT_INT                 ; /* 0x30 */
    unsigned int             CPU2_SC_SOFT_EVT_ID              ; /* 0x34 */
    unsigned int             reserved_2[3]                    ; /* 0x38~0x40 */
    SC_LOCKEN_REG            SC_LOCKEN                        ; /* 0x44 */
    unsigned int             reserved_3[46]                   ; /* 0x48~0xfc */
    unsigned int             SC_SYS_BOOT0                     ; /* 0x100 */
    unsigned int             SC_SYS_BOOT1                     ; /* 0x104 */
    unsigned int             SC_SYS_BOOT2                     ; /* 0x108 */
    unsigned int             SC_SYS_BOOT3                     ; /* 0x10c */
    unsigned int             reserved_4[60]                   ; /* 0x110~0x1fc */
    unsigned int             SC_HRST_REG0                     ; /* 0x200 */
    unsigned int             SC_HRST_REG1                     ; /* 0x204 */
    unsigned int             reserved_5[14]                   ; /* 0x208~0x23c */
    unsigned int             SC_POR_REG0                      ; /* 0x240 */
    unsigned int             SC_POR_REG1                      ; /* 0x244 */
    unsigned int             reserved_6[14]                   ; /* 0x248~0x27c */
    unsigned int             SC_REG0                          ; /* 0x280 */
    unsigned int             SC_REG1                          ; /* 0x284 */
    unsigned int             reserved_7[734]                  ; /* 0x288~0xdfc */
    unsigned int             USER_POR_REG0                    ; /* 0xe00 */
    unsigned int             USER_POR_REG1                    ; /* 0xe04 */
    unsigned int             reserved_8[14]                   ; /* 0xe08~0xe3c */
    unsigned int             USER_HRST_REG0                   ; /* 0xe40 */
    unsigned int             USER_HRST_REG1                   ; /* 0xe44 */
    unsigned int             reserved_9[14]                   ; /* 0xe48~0xe7c */
    unsigned int             USER_REG0                        ; /* 0xe80 */
    unsigned int             USER_REG1                        ; /* 0xe84 */
    unsigned int             reserved_10[7260]                ; /* 0xe88~0x7ff4 */
    MEM_CTRL_REG             MEM_CTRL                         ; /* 0x7ff8 */
    CANFD_CLK_SEL_REG        CANFD_CLK_SEL                    ; /* 0x7ffc */
} volatile SYSCTRL0_RegStruct;

/* Define the union U_APT_RUN */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    apt0_run              : 1   ; /* [0] */
        unsigned int    apt1_run              : 1   ; /* [1] */
        unsigned int    apt2_run              : 1   ; /* [2] */
        unsigned int    apt3_run              : 1   ; /* [3] */
        unsigned int    apt4_run              : 1   ; /* [4] */
        unsigned int    apt5_run              : 1   ; /* [5] */
        unsigned int    apt6_run              : 1   ; /* [6] */
        unsigned int    apt7_run              : 1   ; /* [7] */
        unsigned int    apt8_run              : 1   ; /* [8] */
        unsigned int    apt9_run              : 1   ; /* [9] */
        unsigned int    apt10_run             : 1   ; /* [10] */
        unsigned int    apt11_run             : 1   ; /* [11] */
        unsigned int    apt12_run             : 1   ; /* [12] */
        unsigned int    apt13_run             : 1   ; /* [13] */
        unsigned int    apt14_run             : 1   ; /* [14] */
        unsigned int    apt15_run             : 1   ; /* [15] */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } BIT;
} volatile APT_RUN_REG;

/* Define the union U_APT_POE_FILTER */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    poe_filter_level      : 8   ; /* [7..0] */
        unsigned int    reserved_0            : 8   ; /* [15..8] */
        unsigned int    poe_filter_en         : 1   ; /* [16] */
        unsigned int    reserved_1            : 15  ; /* [31..17] */
    } BIT;
} volatile APT_POE_FILTER_REG;

/* Define the union volatile APT_DEBUG_MODE_SEL */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    apt_debug_mode_sel_cfg : 2   ; /* [1..0] */
        unsigned int    reserved_0            : 30  ; /* [31..2] */
    } BIT;
} volatile APT_DEBUG_MODE_SEL_REG;

/* Define the union volatile QDM_DEBUG_MODE_SEL */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    qdm_debug_mode_sel_cfg : 2   ; /* [1..0] */
        unsigned int    reserved_0            : 30  ; /* [31..2] */
    } BIT;
} volatile QDM_DEBUG_MODE_SEL_REG;

/* Define the union volatile CAPM_DEBUG_MODE_SEL */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    capm_debug_mode_sel_cfg : 2   ; /* [1..0] */
        unsigned int    reserved_0            : 30  ; /* [31..2] */
    } BIT;
} volatile CAPM_DEBUG_MODE_SEL_REG;

/* Define the union volatile DAC_DEBUG_MODE_SEL */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    dac_debug_mode_sel_cfg : 2   ; /* [1..0] */
        unsigned int    reserved_0            : 30  ; /* [31..2] */
    } BIT;
} volatile DAC_DEBUG_MODE_SEL_REG;

/* Define the union volatile XTAL_CFG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    osc_ds                : 4   ; /* [3..0] */
        unsigned int    osc_e                 : 1   ; /* [4] */
        unsigned int    osc_ie                : 1   ; /* [5] */
        unsigned int    reserved_0            : 26  ; /* [31..6] */
    } BIT;
} volatile XTAL_CFG_REG;

/* Define the union volatile CPU_ITCM_DTCM_ERR_CFG_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    cpu_itcm_dtcm_err_en  : 1   ; /* [3..0] */
        unsigned int    reserved_0            : 31  ; /* [31..6] */
    } BIT;
} volatile CPU_ITCM_DTCM_ERR_CFG_REG;

/* Define the union volatile CPU_ITCM_DTCM_ERR_CFG_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    iwdg_debug_mode_sel_cfg : 2   ; /* [3..0] */
        unsigned int    reserved_0              : 30  ; /* [31..6] */
    } BIT;
} volatile IWDG_DEBUG_MODE_SEL_REG;

/* Define the union volatile TIMER_DEBUG_MODE_SEL */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    timer_debug_mode_sel_cfg : 2   ; /* [1..0] */
        unsigned int    reserved_0            : 30  ; /* [31..2] */
    } BIT;
} volatile TIMER_DEBUG_MODE_SEL_REG;

/* Define the union volatile SYSRAM_ERR */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    sysram_parity_err_clr : 1   ; /* [0] */
        unsigned int    sysram0_parity_err    : 1   ; /* [1] */
        unsigned int    sysram1_parity_err    : 1   ; /* [2] */
        unsigned int    sysram2_parity_err    : 1   ; /* [3] */
        unsigned int    sysram3_parity_err    : 1   ; /* [4] */
        unsigned int    sysram4_parity_err    : 1   ; /* [5] */
        unsigned int    sysram5_parity_err    : 1   ; /* [6] */
        unsigned int    sysram6_parity_err    : 1   ; /* [7] */
        unsigned int    sysram7_parity_err    : 1   ; /* [8] */
        unsigned int    reserved_0            : 23  ; /* [31..9] */
    } BIT;
} volatile SYSRAM_ERR_REG;

/* Define the union volatile TCM_STATUS */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    sysram_secure_access_err : 1   ; /* [0] */
        unsigned int    flash_secure_access_err : 1   ; /* [1] */
        unsigned int    reserved_0            : 6   ; /* [7..2] */
        unsigned int    sysram_secure_access_err_clr : 1   ; /* [8] */
        unsigned int    flash_secure_access_err_clr : 1   ; /* [9] */
        unsigned int    reserved_1            : 6   ; /* [15..10] */
        unsigned int    sysbus_tcm_queue_empty : 1   ; /* [16] */
        unsigned int    reserved_2            : 15  ; /* [31..17] */
    } BIT;
} volatile TCM_STATUS_REG;

/* Define the union volatile PVD_STATUS */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    pvd_toggle            : 1   ; /* [0] */
        unsigned int    reserved_0            : 31  ; /* [31..1] */
    } BIT;
} volatile PVD_STATUS_REG;

/* Define the union U_PMU_CFG0 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    reserved_0            : 8   ; /* [7..0] */
        unsigned int    pmu_top_rsv_core      : 3   ; /* [10..8] */
        unsigned int    reserved_1            : 21  ; /* [31..11] */
    } BIT;
} volatile PMU_CFG0_REG;

/* Define the union U_PMU_CFG1 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    pmu_cldo_trim         : 5   ; /* [4..0] */
        unsigned int    reserved_0            : 27  ; /* [31..5] */
    } BIT;
} volatile PMU_CFG1_REG;

/* Define the union U_PMU_CFG2 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    pmu_bg_trim           : 5   ; /* [4..0] */
        unsigned int    reserved_0            : 2   ; /* [6..5] */
        unsigned int    pmu_bg_trim_en        : 1   ; /* [7] */
        unsigned int    reserved_1            : 24  ; /* [31..8] */
    } BIT;
} volatile PMU_CFG2_REG;

/* Define the union U_CPU0_STATUS */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    cpu0_in_nmi_hdlr      : 1   ; /* [0] */
        unsigned int    reserved_0            : 2   ; /* [2..1] */
        unsigned int    cpu0_hard_fault_mode  : 1   ; /* [3] */
        unsigned int    cpu0_debug_mode       : 1   ; /* [4] */
        unsigned int    cpu0_sleep_mode       : 1   ; /* [5] */
        unsigned int    cpu0_core_mode        : 1   ; /* [6] */
        unsigned int    reserved_1            : 24  ; /* [30..7] */
        unsigned int    cpu0_pc_valid         : 1   ; /* [31] */
    } BIT;
} volatile CPU0_STATUS_REG;

/* Define the union volatile CPU1_STATUS */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    cpu1_in_nmi_hdlr      : 1   ; /* [0] */
        unsigned int    reserved_0            : 2   ; /* [2..1] */
        unsigned int    cpu1_hard_fault_mode  : 1   ; /* [3] */
        unsigned int    cpu1_debug_mode       : 1   ; /* [4] */
        unsigned int    cpu1_sleep_mode       : 1   ; /* [5] */
        unsigned int    cpu1_core_mode        : 1   ; /* [6] */
        unsigned int    reserved_1            : 24  ; /* [30..7] */
        unsigned int    cpu1_pc_valid         : 1   ; /* [31] */
    } BIT;
} volatile CPU1_STATUS_REG;

/* Define the union volatile CPU2_STATUS */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    cpu2_in_nmi_hdlr      : 1   ; /* [0] */
        unsigned int    reserved_0            : 2   ; /* [2..1] */
        unsigned int    cpu2_hard_fault_mode  : 1   ; /* [3] */
        unsigned int    cpu2_debug_mode       : 1   ; /* [4] */
        unsigned int    cpu2_sleep_mode       : 1   ; /* [5] */
        unsigned int    cpu2_core_mode        : 1   ; /* [6] */
        unsigned int    reserved_1            : 24  ; /* [30..7] */
        unsigned int    cpu2_pc_valid         : 1   ; /* [31] */
    } BIT;
} volatile CPU2_STATUS_REG;

/* Define the union volatile SRAM0_ECC_DATA_ERR_INJ1 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    sram0_ecc_data_err_inj1 : 7   ; /* [6..0] */
        unsigned int    reserved_0            : 25  ; /* [31..7] */
    } BIT;
} volatile SRAM0_ECC_DATA_ERR_INJ1_REG;

/* Define the union volatile SRAM7_ECC_DATA_ERR_INJ1 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    sram7_ecc_data_err_inj1 : 7   ; /* [6..0] */
        unsigned int    reserved_0            : 25  ; /* [31..7] */
    } BIT;
} volatile SRAM7_ECC_DATA_ERR_INJ1_REG;

/* Define the union volatile RAM0_ECC_ERR_INJ_ADDR_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    sram0_ecc_err_inj_start_addr : 8   ; /* [7..0] */
        unsigned int    sram0_ecc_err_inj_end_addr : 8   ; /* [15..8] */
        unsigned int    reserved_0            : 16  ; /* [31..7] */
    } BIT;
} volatile RAM0_ECC_ERR_INJ_ADDR_REG;

/* Define the union volatile SYS_SRAM_ECC_ERR_FLAG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    sram0_ecc_serr   : 1   ; /* [0] */
        unsigned int    sram1_ecc_serr   : 1   ; /* [1] */
        unsigned int    sram0_ecc_merr   : 1   ; /* [2] */
        unsigned int    sram1_ecc_merr   : 1   ; /* [3] */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } BIT;
} volatile SYS_SRAM_ECC_ERR_FLAG_REG;

/* Define the union volatile SYS_SRAM_ECC_ERR_INT_EN */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    sram0_ecc_serr_int_en : 1   ; /* [0] */
        unsigned int    sram1_ecc_serr_int_en : 1   ; /* [1] */
        unsigned int    sram0_ecc_merr_int_en : 1   ; /* [2] */
        unsigned int    sram1_ecc_merr_int_en : 1   ; /* [3] */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } BIT;
} volatile SYS_SRAM_ECC_ERR_INT_EN_REG;

/* Define the union volatile SYS_SRAM_ECC_ERR_INT */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    sram0_ecc_serr_int : 1   ; /* [0] */
        unsigned int    sram1_ecc_serr_int : 1   ; /* [1] */
        unsigned int    sram0_ecc_merr_int : 1   ; /* [2] */
        unsigned int    sram1_ecc_merr_int : 1   ; /* [3] */
        unsigned int    reserved_0            : 28  ; /* [31..4] */
    } BIT;
} volatile SYS_SRAM_ECC_ERR_INT_REG;

/* Define the union volatile SYS_WFI_DLY_THLD */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    cpu_wfi_dly_thld      : 8   ; /* [7..0] */
        unsigned int    reserved_0            : 24  ; /* [31..8] */
    } BIT;
} volatile SYS_WFI_DLY_THLD_REG;

/* Define the union volatile SYS_WFI_DLY_EN */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    cpu_wfi_dly_en        : 1   ; /* [0] */
        unsigned int    reserved_0            : 31  ; /* [31..1] */
    } BIT;
} volatile SYS_WFI_DLY_EN_REG;

/* Define the union volatile SYS_WFI_DLY_EN */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int reserved_0 : 16;
        unsigned int dma0_req16_sel : 1;
        unsigned int dma0_req17_sel : 1;
        unsigned int dma0_req18_sel : 1;
        unsigned int reserved_1 : 13;
    } BIT;
} volatile DMA0_REQ_SEL_REG;

/* Define the union volatile PERI_MST_MASK_CFG0 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int cmm0_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int cmm1_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int cfd_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int dac_pga_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG0_REG;

/* Define the union volatile PERI_MST_MASK_CFG1 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int mtimer0_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int mtimer1_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int mtimer2_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int timer0_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG1_REG;

/* Define the union volatile PERI_MST_MASK_CFG2 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int timer1_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int timer2_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int timer3_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int timer4_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG2_REG;

/* Define the union volatile PERI_MST_MASK_CFG3 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int timer5_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int i2c0_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int i2c1_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int i2c2_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG3_REG;

/* Define the union volatile PERI_MST_MASK_CFG4 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int spi0_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int spi1_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int spi2_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int foptc_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG4_REG;

/* Define the union volatile PERI_MST_MASK_CFG5 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int can0_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int can1_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int can2_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int gpt0_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG5_REG;

/* Define the union volatile PERI_MST_MASK_CFG6 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int apt0_5_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int apt6_11_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int apt12_15_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int gpt1_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG6_REG;

/* Define the union volatile PERI_MST_MASK_CFG7 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int adc0_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int adc1_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int adc2_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int adc3_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG7_REG;

/* Define the union volatile PERI_MST_MASK_CFG8 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int iwdg_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int wwdg0_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int wwdg1_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int wwdg2_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG8_REG;

/* Define the union volatile PERI_MST_MASK_CFG9 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int sysctrl0_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int sysctrl1_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int crg_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int aon_reg_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG9_REG;

/* Define the union volatile PERI_MST_MASK_CFG10 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int capm0_2_mst_mask_cfg : 5;
        unsigned int reserved_0 : 3;
        unsigned int qdm0_mst_mask_cfg : 5;
        unsigned int reserved_1 : 3;
        unsigned int qdm1_mst_mask_cfg : 5;
        unsigned int reserved_2 : 3;
        unsigned int qdm2_mst_mask_cfg : 5;
        unsigned int reserved_3 : 3;
    } BIT;
} volatile PERI_MST_MASK_CFG10_REG;

/* Define the union volatile PERI_MST_MASK_CFG11 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int qdm3_mst_mask_cfg : 5;
        unsigned int reserved : 27;
    } BIT;
} volatile PERI_MST_MASK_CFG11_REG;

/* Define the union volatile PERI_MST_ACCESS_EXCEPTION_INFO */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int peri_mst_access_exception_mstid : 3;
        unsigned int reserved_0 : 1;
        unsigned int peri_mst_access_exception_src_int : 1;
        unsigned int reserved_1 : 27;
    } BIT;
} volatile PERI_MST_ACCESS_EXCEPTION_INFO_REG;

/* Define the union volatile PERI_MST_ACCESS_EXCEPTION_INT_EN */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int peri_mst_access_exception_src_int_en : 1;
        unsigned int reserved : 31;
    } BIT;
} volatile PERI_MST_ACCESS_EXCEPTION_INT_EN_REG;

/* Define the union volatile PERI_MST_ACCESS_EXCEPTION_INT */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int peri_mst_access_exception_int : 1;
        unsigned int reserved : 31;
    } BIT;
} volatile PERI_MST_ACCESS_EXCEPTION_INT_REG;

/* Define the union volatile GPIO_DFX_CFG0 */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    gpio0_dfx_en          : 1   ;
        unsigned int    gpio1_dfx_en          : 1   ;
        unsigned int    gpio2_dfx_en          : 1   ;
        unsigned int    gpio3_dfx_en          : 1   ;
        unsigned int    gpio4_dfx_en          : 1   ;
        unsigned int    gpio5_dfx_en          : 1   ;
        unsigned int    gpio6_dfx_en          : 1   ;
        unsigned int    gpio7_dfx_en          : 1   ;
        unsigned int    gpio8_dfx_en          : 1   ;
        unsigned int    gpio9_dfx_en          : 1   ;
        unsigned int    gpio10_dfx_en         : 1   ;
        unsigned int    gpio11_dfx_en         : 1   ;
        unsigned int    gpio12_dfx_en         : 1   ;
        unsigned int    gpio13_dfx_en         : 1   ;
        unsigned int    gpio14_dfx_en         : 1   ;
        unsigned int    gpio15_dfx_en         : 1   ;
        unsigned int    gpio16_dfx_en         : 1   ;
        unsigned int    reserved_0            : 15  ;
    } BIT;
} volatile GPIO_DFX_CFG_REG;

/* Define the union volatile OSC_TRIM_SEL */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    losc_trim_value_sel   : 1   ; /* [0] */
        unsigned int    hosc_trim_value_sel   : 1   ; /* [1] */
        unsigned int    reserved_0            : 30  ; /* [31..2] */
    } BIT;
} volatile OSC_TRIM_SEL_REG;

/* Define the union volatile PSW_RSV */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    psw_cfg               : 7   ; /* [6..0] */
        unsigned int    reserved_0            : 25  ; /* [31..7] */
    } BIT;
} volatile PSW_RSV_REG;


/* Define the global struct */
typedef struct {
    char                                  space0[0x8000];
    APT_RUN_REG                           APT_RUN                          ; /* 0x8000 */
    unsigned int                          reserved_12[3]                   ; /* 0x8004~0x800c */
    APT_POE_FILTER_REG                    APT_POE_FILTER[16]               ; /* 0x8010~0x804c */
    unsigned int                          reserved_13[4]                   ; /* 0x8050~0x805c */
    APT_DEBUG_MODE_SEL_REG                APT_DEBUG_MODE_SEL[16]           ; /* 0x8060~0x809c */
    QDM_DEBUG_MODE_SEL_REG                QDM_DEBUG_MODE_SEL[4]            ; /* 0x80a0~0x80ac */
    CAPM_DEBUG_MODE_SEL_REG               CAPM_DEBUG_MODE_SEL[3]           ; /* 0x80b0~0x80b8 */
    DAC_DEBUG_MODE_SEL_REG                DAC_DEBUG_MODE_SEL[9]            ; /* 0x80bc~0x80dc */
    unsigned int                          reserved_14[8]                   ; /* 0x80e0~0x80fc */
    XTAL_CFG_REG                          XTAL_CFG                         ; /* 0x8100 */
    CPU_ITCM_DTCM_ERR_CFG_REG             CPU_ITCM_DTCM_ERR_CFG            ; /* 0x8104 */
    unsigned int                          reserved_15[14]                  ;
    IWDG_DEBUG_MODE_SEL_REG               IWDG_DEBUG_MODE_SEL              ; /* 0x8140 */
    unsigned int                          reserved_16[15]                  ;
    TIMER_DEBUG_MODE_SEL_REG              TIMER_DEBUG_MODE_SEL[6]          ; /* 0x8180~0x8194 */
    unsigned int                          reserved_17[27]                  ; /* 0x8198~0x8200 */
    unsigned int                          CPU1_REG_BASE                    ; /* 0x8204 */
    unsigned int                          CPU2_REG_BASE                    ; /* 0x8208 */
    unsigned int                          reserved_18[61]                  ; /* 0x820c~0x82fc */
    SYSRAM_ERR_REG                        SYSRAM_ERR                       ; /* 0x8300 */
    unsigned int                          reserved_19[2]                   ; /* 0x8304~0x8308 */
    TCM_STATUS_REG                        TCM_STATUS                       ; /* 0x830c */
    unsigned int                          reserved_20[61]                  ; /* 0x8310~0x8400 */
    PVD_STATUS_REG                        PVD_STATUS                       ; /* 0x8404 */
    PMU_CFG0_REG                          PMU_CFG0                         ; /* 0x8408 */
    PMU_CFG1_REG                          PMU_CFG1                         ; /* 0x840c */
    PMU_CFG2_REG                          PMU_CFG2                         ; /* 0x8410 */
    unsigned int                          reserved_21[763]                 ; /* 0x8414~0x8ffc */
    CPU0_STATUS_REG                       CPU0_STATUS                      ; /* 0x9000 */
    unsigned int                          CPU0_PC                          ; /* 0x9004 */
    unsigned int                          reserved_22[6]                   ; /* 0x9008~0x901c */
    CPU1_STATUS_REG                       CPU1_STATUS                      ; /* 0x9020 */
    unsigned int                          CPU1_PC                          ; /* 0x9024 */
    unsigned int                          reserved_23[6]                   ; /* 0x9028~0x903c */
    CPU2_STATUS_REG                       CPU2_STATUS                      ; /* 0x9040 */
    unsigned int                          CPU2_PC                          ; /* 0x9044 */
    unsigned int                          reserved_24[1006]                ; /* 0x9048~0x9ffc */
    unsigned int                          SRAM0_ECC_ERR_ADDR               ; /* 0xa000 */
    unsigned int                          SRAM7_ECC_ERR_ADDR               ; /* 0xa004 */
    unsigned int                          SRAM0_ECC_DATA_ERR_INJ0          ; /* 0xa008 */
    SRAM0_ECC_DATA_ERR_INJ1_REG           SRAM0_ECC_DATA_ERR_INJ1          ; /* 0xa00c */
    unsigned int                          SRAM7_ECC_DATA_ERR_INJ0          ; /* 0xa010 */
    SRAM7_ECC_DATA_ERR_INJ1_REG           SRAM7_ECC_DATA_ERR_INJ1          ; /* 0xa014 */
    RAM0_ECC_ERR_INJ_ADDR_REG             RAM0_ECC_ERR_INJ_ADDR            ; /* 0xa018 */
    unsigned int                          reserved_25[1]                   ; /* 0xa01c */
    SYS_SRAM_ECC_ERR_FLAG_REG             SYS_SRAM_ECC_ERR_FLAG            ; /* 0xa020 */
    SYS_SRAM_ECC_ERR_INT_EN_REG           SYS_SRAM_ECC_ERR_INT_EN          ; /* 0xa024 */
    SYS_SRAM_ECC_ERR_INT_REG              SYS_SRAM_ECC_ERR_INT             ; /* 0xa028 */
    unsigned int                          reserved_26[5]                   ; /* 0xa02c~0xa03c */
    SYS_WFI_DLY_THLD_REG                  SYS_WFI_DLY_THLD                 ; /* 0xa040 */
    SYS_WFI_DLY_EN_REG                    SYS_WFI_DLY_EN                   ; /* 0xa044 */
    unsigned int                          reserved_27[1]                   ;
    DMA0_REQ_SEL_REG                      DMA0_REQ_SEL                     ; /* 0xa04c */
    unsigned int                          reserved_28[4]                   ;
    PERI_MST_MASK_CFG0_REG                PERI_MST_MASK_CFG0               ; /* 0xa060 */
    PERI_MST_MASK_CFG1_REG                PERI_MST_MASK_CFG1               ; /* 0xa064 */
    PERI_MST_MASK_CFG2_REG                PERI_MST_MASK_CFG2               ; /* 0xa068 */
    PERI_MST_MASK_CFG3_REG                PERI_MST_MASK_CFG3               ; /* 0xa06C */
    PERI_MST_MASK_CFG4_REG                PERI_MST_MASK_CFG4               ; /* 0xa070 */
    PERI_MST_MASK_CFG5_REG                PERI_MST_MASK_CFG5               ; /* 0xa074 */
    PERI_MST_MASK_CFG6_REG                PERI_MST_MASK_CFG6               ; /* 0xa078 */
    PERI_MST_MASK_CFG7_REG                PERI_MST_MASK_CFG7               ; /* 0xa07C */
    PERI_MST_MASK_CFG8_REG                PERI_MST_MASK_CFG8               ; /* 0xa080 */
    PERI_MST_MASK_CFG9_REG                PERI_MST_MASK_CFG9               ; /* 0xa084 */
    PERI_MST_MASK_CFG10_REG               PERI_MST_MASK_CFG10              ; /* 0xa088 */
    PERI_MST_MASK_CFG11_REG               PERI_MST_MASK_CFG11              ; /* 0xa08C */
    unsigned int                          PERI_MST_ACCESS_EXCEPTION_ADDR   ; /* 0xa090 */
    PERI_MST_ACCESS_EXCEPTION_INFO_REG    PERI_MST_ACCESS_EXCEPTION_INFO   ; /* 0xa094 */
    PERI_MST_ACCESS_EXCEPTION_INT_EN_REG  PERI_MST_ACCESS_EXCEPTION_INT_EN ; /* 0xa098 */
    PERI_MST_ACCESS_EXCEPTION_INT_REG     PERI_MST_ACCESS_EXCEPTION_INT    ; /* 0xa09c */
    unsigned int                          reserved_29[24]                  ;
    GPIO_DFX_CFG_REG                      GPIO_DFX_CFG                     ; /* 0xa100 */
    unsigned int                          reserved_30[6075]                ;
    OSC_TRIM_SEL_REG                      OSC_TRIM_SEL                     ; /* 0xfff0 */
    unsigned int                          reserved_31[2]                   ;
    PSW_RSV_REG                           PSW_RSV                          ; /* 0xfffc */
} volatile SYSCTRL1_RegStruct;


/* Define the union volatile PSW_RSV */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    jtag_lock_sta         : 1   ;
        unsigned int    reserved_0            : 31  ;
    } BIT;
} volatile JTAG_LOCK_STA_REG;

/* Define the global struct */
typedef struct {
    unsigned int                          JTAG_PSWD_CFG0                   ; /* 0x00 */
    unsigned int                          JTAG_PSWD_CFG1                   ; /* 0x04 */
    unsigned int                          JTAG_PSWD_CFG2                   ; /* 0x08 */
    unsigned int                          JTAG_PSWD_CFG3                   ; /* 0x0c */
    unsigned int                          reserved_0[5]                    ;
    JTAG_LOCK_STA_REG                     JTAG_LOCK_STA                    ; /* 0x24 */
} volatile JTAG_LockRegStruct;

/**
  * @brief Make system soft reset.
  * @param None
  * @retval None.
  */
static inline void DCL_SYSCTRL_SoftReset(void)
{
    SYSCTRL0->SC_SYS_RES.BIT.softresreq = 1;
}

/**
  * @brief Get number of soft resets.
  * @param None
  * @retval Number of soft resets.
  */
static inline unsigned short DCL_SYSCTRL_GetSoftResetConut(void)
{
    return SYSCTRL0->SC_RST_CNT0.BIT.soft_rst_cnt;
}

/**
  * @brief Get number of reset times of the RESETN pin.
  * @param None
  * @retval Number of reset times of the RESETN pin.
  */
static inline unsigned short DCL_SYSCTRL_GetPinResetConut(void)
{
    return SYSCTRL0->SC_RST_CNT0.BIT.ext_rst_cnt;
}

/**
  * @brief Get number of IWDG resets.
  * @param None
  * @retval Number of IWDG resets.
  */
static inline unsigned short DCL_SYSCTRL_GetWdgResetConut(ChipCoreId WDGId)
{
    if (WDGId == CHIP_CORE_0) {  /* Operation WDG0. */
        return SYSCTRL0->SC_RST_CNT1.BIT.wdg0_rst_cnt;
    } else if (WDGId == CHIP_CORE_1) {  /* Operation WDG1. */
        return SYSCTRL0->SC_RST_CNT2.BIT.wdg1_rst_cnt;
    } else if (WDGId == CHIP_CORE_2) {  /* Operation WDG2. */
        return SYSCTRL0->SC_RST_CNT2.BIT.wdg2_rst_cnt;
    }
    return 0;
}

/**
  * @brief Get number of IWDG resets.
  * @param None
  * @retval Number of IWDG resets.
  */
static inline unsigned short DCL_SYSCTRL_GetIWdgResetConut(void)
{
    return SYSCTRL0->SC_RST_CNT1.BIT.iwdg_rst_cnt;
}

/**
  * @brief Set the write protection for SYSCTRL registers disable.
  * @param None
  * @retval None.
  */
static inline void DCL_SYSCTRL_ScWriteProtectionDisable(void)
{
    /* Set the corresponding bit without affecting the other bits and set the high 16 bits to EA51 to write to. */
    SYSCTRL0->SC_LOCKEN.reg = (SYSCTRL0->SC_LOCKEN.reg & SC_LOCKEN_SC_DISABLE_MASK) + SC_LOCKEN_VALID_HIGH_BIT;
}

/**
  * @brief Set the write protection for SYSCTRL registers enable.
  * @param None
  * @retval None.
  */
static inline void DCL_SYSCTRL_ScWriteProtectionEnable(void)
{
    /* Set the corresponding bit without affecting the other bits and set the high 16 bits to EA51 to write to. */
    SYSCTRL0->SC_LOCKEN.reg = ((SYSCTRL0->SC_LOCKEN.reg & SC_LOW_BIT_MASK) | SC_LOCKEN_SC_ENABLE_MASK) +
                              SC_LOCKEN_VALID_HIGH_BIT;
}

/**
  * @brief Set the write protection for CRG-related registers disable.
  * @param None
  * @retval None.
  */
static inline void DCL_SYSCTRL_CrgWriteProtectionDisable(void)
{
    /* Set the corresponding bit without affecting the other bits and set the high 16 bits to EA51 to write to. */
    SYSCTRL0->SC_LOCKEN.reg = (SYSCTRL0->SC_LOCKEN.reg & SC_LOCKEN_CRG_DISABLE_MASK) + SC_LOCKEN_VALID_HIGH_BIT;
}

/**
  * @brief Set the Set the write protection for CRG-related registers enable.
  * @param None
  * @retval None.
  */
static inline void DCL_SYSCTRL_CrgWriteProtectionEnable(void)
{
    /* Set the corresponding bit without affecting the other bits and set the high 16 bits to EA51 to write to. */
    SYSCTRL0->SC_LOCKEN.reg = ((SYSCTRL0->SC_LOCKEN.reg & SC_LOW_BIT_MASK) | SC_LOCKEN_CRG_ENABLE_MASK) +
                              SC_LOCKEN_VALID_HIGH_BIT;
}

/**
  * @brief Set software interrupt register, writing 1 generates a software interrupt.
  * @param None.
  * @retval None.
  */
static inline void DCL_SYSCTRL_GenerateSoftInterrupt(void)
{
    /* Get the baseaddress of the current chip core. */
    ChipCoreId chipId = GetChipCoreId();
    if (chipId == CHIP_CORE_0) {  /* Operation CPU0. */
        SYSCTRL0->CPU0_SC_SOFT_INT.BIT.cpu0_software_int = 1;
    } else if (chipId == CHIP_CORE_1) {  /* Operation CPU10. */
        SYSCTRL0->CPU1_SC_SOFT_INT.BIT.cpu1_software_int = 1;
    } else if (chipId == CHIP_CORE_2) {  /* Operation CPU2. */
        SYSCTRL0->CPU2_SC_SOFT_INT.BIT.cpu2_software_int = 1;
    }
}

/**
  * @brief Clear software interrupt register, writing 0 generates a software interrupt.
  * @param None.
  * @retval None.
  */
static inline void DCL_SYSCTRL_ClearSoftInterrupt(void)
{
    /* Get the baseaddress of the current chip core. */
    ChipCoreId chipId = GetChipCoreId();
    if (chipId == CHIP_CORE_0) {  /* Operation CPU0. */
        SYSCTRL0->CPU0_SC_SOFT_INT.BIT.cpu0_software_int = 0;
    } else if (chipId == CHIP_CORE_1) {  /* Operation CPU1. */
        SYSCTRL0->CPU1_SC_SOFT_INT.BIT.cpu1_software_int = 0;
    } else if (chipId == CHIP_CORE_2) {  /* Operation CPU2. */
        SYSCTRL0->CPU2_SC_SOFT_INT.BIT.cpu2_software_int = 0;
    }
}

/**
  * @brief Set Software interrupt event ID.
  * @param id the software interrupt event ID.
  * @retval None.
  */
static inline void DCL_SYSCTRL_SetSoftInterruptEventId(unsigned int id)
{
    /* Get the baseaddress of the current chip core. */
    ChipCoreId chipId = GetChipCoreId();
    if (chipId == CHIP_CORE_0) {  /* Operation CPU0. */
        SYSCTRL0->CPU0_SC_SOFT_EVT_ID = id;
    } else if (chipId == CHIP_CORE_1) {  /* Operation CPU1. */
        SYSCTRL0->CPU1_SC_SOFT_EVT_ID = id;
    } else if (chipId == CHIP_CORE_2) {  /* Operation CPU2. */
        SYSCTRL0->CPU2_SC_SOFT_EVT_ID = id;
    }
}

/**
  * @brief Get Software interrupt event ID.
  * @param None.
  * @retval The value of software interrupt event ID.
  */
static inline unsigned int DCL_SYSCTRL_GetSoftInterruptEventId(void)
{
    /* Get the baseaddress of the current chip core. */
    ChipCoreId chipId = GetChipCoreId();
    unsigned int ret = 0;
    if (chipId == CHIP_CORE_0) {  /* Operation CPU0. */
        ret = SYSCTRL0->CPU0_SC_SOFT_EVT_ID;
    } else if (chipId == CHIP_CORE_0) {  /* Operation CPU1. */
        ret = SYSCTRL0->CPU1_SC_SOFT_EVT_ID;
    } else if (chipId == CHIP_CORE_2) {  /* Operation CPU2. */
        ret = SYSCTRL0->CPU2_SC_SOFT_EVT_ID;
    }
    return ret;
}

/**
  * @brief Set can clock select.
  * @param clkSel the clock type.
  * @retval None.
  */
static inline void DCL_SYSCTRL_SetCanClkSel(SYSCTRL_CanClkSel clkSel)
{
    SYSCTRL0->CANFD_CLK_SEL.BIT.can_fd_clk_sel = (clkSel & BASE_CFG_SET);
}

/**
  * @brief Get can clock select.
  * @param None
  * @retval The select of can clock.
  */
static inline unsigned int DCL_SYSCTRL_GetCanClkSel(void)
{
    /* Get the can clock. */
    return SYSCTRL0->CANFD_CLK_SEL.BIT.can_fd_clk_sel;
}


/* The group of CPU1 Configuration Function */
/**
 * @brief Set the APT debug mode source for the specified index.
 * @param index The index of the APT debug mode register.
 * @param source The debug mode source to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetAptDebugModeSource(SYSCTRL_DebugModeAptIndex index, SYSCTRL_DebugModeSource source)
{
    if (index > DEBUG_MODE_SOURCE_APT_MAX) {
        return; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = APT_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    WriteRegister(regAddr, source & 0x3);     /* Set register. */
}

/**
 * @brief Get the APT debug mode source for the specified index.
 * @param index The index of the APT debug mode register.
 * @retval The debug mode source value read from the register.
 */
static inline SYSCTRL_DebugModeSource DCL_SYSCTRL_GetAptDebugModeSource(SYSCTRL_DebugModeAptIndex index)
{
    if (index > DEBUG_MODE_SOURCE_APT_MAX) {
        return DEBUG_MODE_SOURCE_ERROR; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = APT_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    SYSCTRL_DebugModeSource ret = (SYSCTRL_DebugModeSource)ReadRegister(regAddr);     /* Get register. */
    return ret;
}

/**
 * @brief Set the QDM debug mode source for the specified index.
 * @param index The index of the QDM debug mode register.
 * @param source The debug mode source to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetQdmDebugModeSource(SYSCTRL_DebugModeQdmIndex index, SYSCTRL_DebugModeSource source)
{
    if (index > DEBUG_MODE_SOURCE_QDM_MAX) {
        return; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = QDM_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    WriteRegister(regAddr, source & 0x3);     /* Set register. */
}

/**
 * @brief Get the QDM debug mode source for the specified index.
 * @param index The index of the QDM debug mode register.
 * @retval The debug mode source value read from the register.
 */
static inline SYSCTRL_DebugModeSource DCL_SYSCTRL_GetQdmDebugModeSource(SYSCTRL_DebugModeQdmIndex index)
{
    if (index > DEBUG_MODE_SOURCE_QDM_MAX) {
        return DEBUG_MODE_SOURCE_ERROR; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = QDM_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    SYSCTRL_DebugModeSource ret = (SYSCTRL_DebugModeSource)ReadRegister(regAddr);     /* Get register. */
    return ret;
}

/**
 * @brief Set the CAPM debug mode source for the specified index.
 * @param index The index of the CAPM debug mode register.
 * @param source The debug mode source to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetCapmDebugModeSource(SYSCTRL_DebugModeCapmIndex index, SYSCTRL_DebugModeSource source)
{
    if (index > DEBUG_MODE_SOURCE_CAPM_MAX) {
        return; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = CAPM_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    WriteRegister(regAddr, source & 0x3);     /* Set register. */
}

/**
 * @brief Get the CAPM debug mode source for the specified index.
 * @param index The index of the CAPM debug mode register.
 * @retval The debug mode source value read from the register.
 */
static inline SYSCTRL_DebugModeSource DCL_SYSCTRL_GetCapmDebugModeSource(SYSCTRL_DebugModeCapmIndex index)
{
    if (index > DEBUG_MODE_SOURCE_CAPM_MAX) {
        return DEBUG_MODE_SOURCE_ERROR; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = CAPM_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    SYSCTRL_DebugModeSource ret = (SYSCTRL_DebugModeSource)ReadRegister(regAddr);     /* Get register. */
    return ret;
}

/**
 * @brief Set the DAC debug mode source for the specified index.
 * @param index The index of the DAC debug mode register.
 * @param source The debug mode source to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetDacDebugModeSource(SYSCTRL_DebugModeDacIndex index, SYSCTRL_DebugModeSource source)
{
    if (index > DEBUG_MODE_SOURCE_DAC_MAX) {
        return; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = DAC_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    WriteRegister(regAddr, source & 0x3);     /* Set register. */
}

/**
 * @brief Get the DAC debug mode source for the specified index.
 * @param index The index of the DAC debug mode register.
 * @retval The debug mode source value read from the register.
 */
static inline SYSCTRL_DebugModeSource DCL_SYSCTRL_GetDacDebugModeSource(SYSCTRL_DebugModeDacIndex index)
{
    if (index > DEBUG_MODE_SOURCE_DAC_MAX) {
        return DEBUG_MODE_SOURCE_ERROR; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = DAC_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    SYSCTRL_DebugModeSource ret = (SYSCTRL_DebugModeSource)ReadRegister(regAddr);     /* Get register. */
    return ret;
}

/**
 * @brief Set the IWDG debug mode source.
 * @param source The debug mode source to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetIwdgebugModeSource(SYSCTRL_DebugModeSource source)
{
    SYSCTRL1->IWDG_DEBUG_MODE_SEL.BIT.iwdg_debug_mode_sel_cfg = source & 0x3; /* Set register. */
}

/**
 * @brief Get the IWDG debug mode source.
 * @details This function reads the debug mode source from the IWDG debug mode register.
 * @retval The debug mode source value read from the register.
 */
static inline SYSCTRL_DebugModeSource DCL_SYSCTRL_GetIwdgDebugModeSource(void)
{
    return (SYSCTRL_DebugModeSource)SYSCTRL1->IWDG_DEBUG_MODE_SEL.BIT.iwdg_debug_mode_sel_cfg; /* Get register. */
}

/**
 * @brief Set the Timer debug mode source for the specified index.
 * @param index The index of the Timer debug mode register.
 * @param source The debug mode source to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetTimerDebugModeSource(SYSCTRL_DebugModeTimerIndex index,
                                                       SYSCTRL_DebugModeSource source)
{
    if (index > DEBUG_MODE_SOURCE_TIMER_MAX) {
        return; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = TIMER_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    WriteRegister(regAddr, source & 0x3);     /* Set register. */
}

/**
 * @brief Get the Timer debug mode source for the specified index.
 * @param index The index of the Timer debug mode register.
 * @retval The debug mode source value read from the register.
 */
static inline SYSCTRL_DebugModeSource DCL_SYSCTRL_GetTimerDebugModeSource(SYSCTRL_DebugModeTimerIndex index)
{
    if (index > DEBUG_MODE_SOURCE_TIMER_MAX) {
        return DEBUG_MODE_SOURCE_ERROR; /* Exceeded the maximum address. */
    }
    unsigned int regAddr = TIMER_DEBUG_MODE_ADDR_OFFSET + (index * ONE_REG_BYTE_NUM) +
                           (unsigned int)SYSCTRL0_BASE;
    SYSCTRL_DebugModeSource ret = (SYSCTRL_DebugModeSource)ReadRegister(regAddr);     /* Get register. */
    return ret;
}

/**
 * @brief Set the CPU ITCM/DTCM error configuration.
 * @param cfg The error configuration to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetCpuItcmDtcmErr(SYSCTRL_CpuItcmDtcmErrCfg cfg)
{
    SYSCTRL1->CPU_ITCM_DTCM_ERR_CFG.BIT.cpu_itcm_dtcm_err_en = (cfg & RETURN_ERROR);
}

/**
 * @brief Get the CPU ITCM/DTCM error configuration.
 * @param None.
 * @retval The current error configuration value.
 */
static inline SYSCTRL_CpuItcmDtcmErrCfg DCL_SYSCTRL_GetCpuItcmDtcmErr(void)
{
    return (SYSCTRL_CpuItcmDtcmErrCfg)SYSCTRL1->CPU_ITCM_DTCM_ERR_CFG.BIT.cpu_itcm_dtcm_err_en;
}

/**
  * @brief Set Core start address.
  * @param coreId CHIP_CORE_1 and CHIP_CORE_2, CHIP_CORE_0 is main core, default auto start.
  * @param startAddr Core start address.
  * @retval None.
  */
static inline void DCL_SYSCTRL_SetCoreStartAddr(ChipCoreId coreId, unsigned int startAddr)
{
    if (coreId == CHIP_CORE_1) {
        SYSCTRL1->CPU1_REG_BASE = startAddr;
    } else if (coreId == CHIP_CORE_2) {
        SYSCTRL1->CPU2_REG_BASE = startAddr;
    }
}

/**
  * @brief Clear Core1 start address.
  * @param coreId CHIP_CORE_1 and CHIP_CORE_2, CHIP_CORE_0 is main core, default auto start.
  * @retval None.
  */
static inline void DCL_SYSCTRL_ClearCoreStartAddr(ChipCoreId coreId)
{
    if (coreId == CHIP_CORE_1) {
        SYSCTRL1->CPU1_REG_BASE = BASE_CFG_UNSET;
    } else if (coreId == CHIP_CORE_2) {
        SYSCTRL1->CPU2_REG_BASE = BASE_CFG_UNSET;
    }
}

/**
  * @brief Get SYSRAM Parity Error Status.
  * @param None.
  * @retval 0:no error, 1:error.
  */
static inline unsigned int DCL_SYSCTRL_GetSysramParityErrorStatus(void)
{
    return SYSCTRL1->SYSRAM_ERR.BIT.sysram0_parity_err;
}

/**
  * @brief Set SYSRAM parity error status clear.
  * @param None.
  * @retval None.
  */
static inline void DCL_SYSCTRL_ClearSysramParityError(void)
{
    SYSCTRL1->SYSRAM_ERR.BIT.sysram_parity_err_clr = 1; /* Write any value to clear. */
}

/**
  * @brief Get CPU status.
  * @param offset Bit offset of CPU status.
  * @retval true or false
  */
static inline bool DCL_SYSCTRL_CheckCpuStatus(SYSCTRL_CpuStatus offset)
{
    /* Get the baseaddress of the current chip core. */
    ChipCoreId chipId = GetChipCoreId();
    bool ret = false;
    if (chipId == CHIP_CORE_0) {  /* Operation CPU0. */
        ret = ((SYSCTRL1->CPU0_STATUS.reg) & (1 << offset)) == 0 ? false : true;
    } else if (chipId == CHIP_CORE_1) {  /* Operation CPU1. */
        ret = ((SYSCTRL1->CPU1_STATUS.reg) & (1 << offset)) == 0 ? false : true;
    } else if (chipId == CHIP_CORE_2) {  /* Operation CPU2. */
        ret = ((SYSCTRL1->CPU2_STATUS.reg) & (1 << offset)) == 0 ? false : true;
    }
    return ret;
}

/**
  * @brief Get SYSRAM Parity Error Status.
 * @param None.
  * @retval 0:no error, 1:error.
  */
static inline unsigned int DCL_SYSCTRL_GetCpuPcValue(void)
{
    /* Get the baseaddress of the current chip core. */
    ChipCoreId chipId = GetChipCoreId();
    unsigned int pcValue = 0;
    if (chipId == CHIP_CORE_0) {  /* Operation CPU0. */
        pcValue = SYSCTRL1->CPU0_PC;
    } else if (chipId == CHIP_CORE_1) {  /* Operation CPU1. */
        pcValue = SYSCTRL1->CPU1_PC;
    } else if (chipId == CHIP_CORE_2) {  /* Operation CPU2. */
        pcValue = SYSCTRL1->CPU2_PC;
    }
    return pcValue;
}

/**
 * @brief Get the ECC error address for SRAM0.
 * @param None.
 * @retval The ECC error address value for SRAM0.
 */
static inline unsigned int DCL_SYSCTRL_GetSram0EccErrAddr(void)
{
    return SYSCTRL1->SRAM0_ECC_ERR_ADDR;
}

/**
 * @brief Get the ECC error address for SRAM7.
 * @param None.
 * @retval The ECC error address value for SRAM7.
 */
static inline unsigned int DCL_SYSCTRL_GetSram7EccErrAddr(void)
{
    return SYSCTRL1->SRAM7_ECC_ERR_ADDR;
}

/**
 * @brief Inject a low 32-bit ECC error for SRAM0.
 * @param errMask The error mask to inject (low 32 bits).
 * @retval None.
 */
static inline void DCL_SYSCTRL_InjSram0EccErrLow32Bit(unsigned int errMask)
{
    SYSCTRL1->SRAM0_ECC_DATA_ERR_INJ0 = errMask;
}

/**
 * @brief Inject a high 7-bit ECC error for SRAM0.
 * @param errMask The error mask to inject (only the lower 7 bits are used).
 * @retval None.
 */
static inline void DCL_SYSCTRL_InjSram0EccErrhigh7Bit(unsigned int errMask)
{
    SYSCTRL1->SRAM0_ECC_DATA_ERR_INJ1.BIT.sram0_ecc_data_err_inj1 = errMask & 0x7F;
}

/**
 * @brief Inject a low 32-bit ECC error for SRAM7.
 * @param errMask The error mask to inject (low 32 bits).
 * @retval None.
 */
static inline void DCL_SYSCTRL_InjSram7EccErrLow32Bit(unsigned int errMask)
{
    SYSCTRL1->SRAM7_ECC_DATA_ERR_INJ0 = errMask;
}

/**
 * @brief Inject a high 7-bit ECC error for SRAM7.
 * @param errMask The error mask to inject (only the lower 7 bits are used).
 * @retval None.
 */
static inline void DCL_SYSCTRL_InjSram7EccErrhigh7Bit(unsigned int errMask)
{
    SYSCTRL1->SRAM7_ECC_DATA_ERR_INJ1.BIT.sram7_ecc_data_err_inj1 = errMask & 0x7F;
}

/**
 * @brief Enable or disable SRAM ECC error injection interrupts.
 * @param enableMask The enable mask to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_EnableInjSramEccErrIT(unsigned int enableMask)
{
    SYSCTRL1->SYS_SRAM_ECC_ERR_INT_EN.reg = enableMask;
}

/**
 * @brief Get the current SRAM ECC error injection interrupt mask.
 * @param None.
 * @retval The current interrupt mask value.
 */
static inline unsigned int DCL_SYSCTRL_GetInjSramEccErrITMask(void)
{
    return SYSCTRL1->SYS_SRAM_ECC_ERR_FLAG.reg;
}

/**
 * @brief Clear SRAM ECC error injection interrupts.
 * @param clearMask The clear mask to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_ClearInjSramEccErrIT(unsigned int clearMask)
{
    SYSCTRL1->SYS_SRAM_ECC_ERR_INT.reg = clearMask;
}

/**
 * @brief Configure the system WFI delay threshold.
 * @param threshold The threshold value to set (only the lower 8 bits are used).
 * @retval None.
 */
static inline void DCL_SYSCTRL_CfgSysWfiDelayThreshold(unsigned int threshold)
{
    SYSCTRL1->SYS_WFI_DLY_THLD.BIT.cpu_wfi_dly_thld = threshold & 0xFF;
}

/**
 * @brief Enable the system WFI delay function.
 * @param None.
 * @retval None.
 */
static inline void DCL_SYSCTRL_EnableSysWfiDelay(void)
{
    SYSCTRL1->SYS_WFI_DLY_EN.BIT.cpu_wfi_dly_en = BASE_CFG_ENABLE;
}

/**
 * @brief Disable the system WFI delay function.
 * @param None.
 * @retval None.
 */
static inline void DCL_SYSCTRL_DisableSysWfiDelay(void)
{
    SYSCTRL1->SYS_WFI_DLY_EN.BIT.cpu_wfi_dly_en = BASE_CFG_DISABLE;
}

/**
 * @brief Set the DMA0 request 16 source selection.
 * @param select The selection value to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetDma0Req16Sel(SYSCTRL_Dma0Req16Sel select)
{
    SYSCTRL1->DMA0_REQ_SEL.BIT.dma0_req16_sel = select & BASE_CFG_SET;
}

/**
 * @brief Get the current DMA0 request 16 source selection.
 * @param None.
 * @retval The current selection value.
 */
static inline SYSCTRL_Dma0Req16Sel DCL_SYSCTRL_GetDma0Req16Sel(void)
{
    return (SYSCTRL_Dma0Req16Sel)SYSCTRL1->DMA0_REQ_SEL.BIT.dma0_req16_sel;
}

/**
 * @brief Set the DMA0 request 17 source selection.
 * @param select The selection value to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetDma0Req17Sel(SYSCTRL_Dma0Req17Sel select)
{
    SYSCTRL1->DMA0_REQ_SEL.BIT.dma0_req17_sel = select & BASE_CFG_SET;
}

/**
 * @brief Get the current DMA0 request 17 source selection.
 * @param None.
 * @retval The current selection value.
 */
static inline SYSCTRL_Dma0Req17Sel DCL_SYSCTRL_GetDma0Req17Sel(void)
{
    return (SYSCTRL_Dma0Req17Sel)SYSCTRL1->DMA0_REQ_SEL.BIT.dma0_req17_sel;
}

/**
 * @brief Set the DMA0 request 18 source selection.
 * @param select The selection value to set.
 * @retval None.
 */
static inline void DCL_SYSCTRL_SetDma0Req18Sel(SYSCTRL_Dma0Req18Sel select)
{
    SYSCTRL1->DMA0_REQ_SEL.BIT.dma0_req18_sel = select & BASE_CFG_SET;
}

/**
 * @brief Get the current DMA0 request 18 source selection.
 * @param None.
 * @retval The current selection value.
 */
static inline SYSCTRL_Dma0Req18Sel DCL_SYSCTRL_GetDma0Req18Sel(void)
{
    return (SYSCTRL_Dma0Req18Sel)SYSCTRL1->DMA0_REQ_SEL.BIT.dma0_req18_sel;
}

/**
 * @brief Configure the access permission for a peripheral master.
 * @param index The index of the peripheral.
 * @param masterCfg The access permission configuration for the master.
 * @retval None.
 */
static inline void DCL_SYSCTRL_CfgPeriMstAccessPermission(SYSCTRL_Peripheral index, SYSCTRL_Permission masterCfg)
{
    /* Get register index. */
    unsigned int periOffset = ((index % ONE_REG_CFG_PERI_NUM) * ONE_PERI_OFFSET_BIT);
    unsigned int regAddr = ACCESS_RIGHT_SADDR + (index >> 0x2) * ONE_REG_BYTE_NUM + (unsigned int)SYSCTRL0_BASE;
    if (regAddr > (ACCESS_RIGHT_EADDR + (unsigned int)SYSCTRL0_BASE)) {
        return; /* Exceeded the maximum address. */
    }
    /* the write protection for SYSCTRL registers disable */
    DCL_SYSCTRL_ScWriteProtectionDisable();
    /* Read value and then clear bit. */
    unsigned int regValue = ReadRegister(regAddr) & (~(0x000000FF << periOffset));
    /* Config master permission */
    regValue |= (masterCfg & MASTER_ACCESS_PERMISSION_MASK) << periOffset;
    /* Set register. */
    WriteRegister(regAddr, regValue);
    DCL_SYSCTRL_ScWriteProtectionEnable();
}

/**
 * @brief Get the access permission configuration for a peripheral master.
 * @param peripheralIndex The index of the peripheral.
 * @retval The access permission configuration for the master.
 */
static inline SYSCTRL_Permission DCL_SYSCTRL_GetPeriMstAccessPermission(SYSCTRL_Peripheral peripheralIndex)
{
    /* Get register index. */
    unsigned int periOffset = ((peripheralIndex % ONE_REG_CFG_PERI_NUM) * ONE_PERI_OFFSET_BIT);
    unsigned int regAddr = ACCESS_RIGHT_SADDR + (peripheralIndex >> 0x2) * ONE_REG_BYTE_NUM +
                           (unsigned int)SYSCTRL0_BASE;
    if (regAddr > (ACCESS_RIGHT_EADDR + (unsigned int)SYSCTRL0_BASE)) {
        return 0; /* Exceeded the maximum address. */
    }
    /* the write protection for SYSCTRL registers disable */
    DCL_SYSCTRL_ScWriteProtectionDisable();
    /* Read value and then clear bit. */
    SYSCTRL_Permission regValue = (SYSCTRL_Permission)((ReadRegister(regAddr) >> periOffset) & 0xFF);
    DCL_SYSCTRL_ScWriteProtectionEnable();
    return regValue;
}

/**
 * @brief Get the exception address for peripheral master access.
 * @param None.
 * @retval The exception address for peripheral master access.
 */
static inline unsigned int DCL_SYSCTRL_GetPeriMstAccessExceptionAddr(void)
{
    return SYSCTRL1->PERI_MST_ACCESS_EXCEPTION_ADDR;
}

/**
 * @brief Get the source interrupt information from peripheral master access exception.
 * @param None.
 * @retval The source interrupt information.
 */
static inline unsigned int DCL_SYSCTRL_GetPeriMstAccessExceptionInfoSrcIT(void)
{
    return SYSCTRL1->PERI_MST_ACCESS_EXCEPTION_INFO.BIT.peri_mst_access_exception_src_int;
}

/**
 * @brief Get the master ID information from peripheral master access exception.
 * @param None.
 * @retval The master ID information.
 */
static inline unsigned int DCL_SYSCTRL_GetPeriMstAccessExceptionInfoMstId(void)
{
    return SYSCTRL1->PERI_MST_ACCESS_EXCEPTION_INFO.BIT.peri_mst_access_exception_mstid;
}

/**
 * @brief Enable the peripheral master access exception interrupt.
 * @param None.
 * @retval None.
 */
static inline void DCL_SYSCTRL_EnablePeriMstAccessExceptionInt(void)
{
    SYSCTRL1->PERI_MST_ACCESS_EXCEPTION_INT_EN.BIT.peri_mst_access_exception_src_int_en = BASE_CFG_ENABLE;
}

/**
 * @brief Disable the peripheral master access exception interrupt.
 * @param None.
 * @retval None.
 */
static inline void DCL_SYSCTRL_DisablePeriMstAccessExceptionInt(void)
{
    SYSCTRL1->PERI_MST_ACCESS_EXCEPTION_INT_EN.BIT.peri_mst_access_exception_src_int_en = BASE_CFG_DISABLE;
}

/**
 * @brief Get the status of the peripheral master access exception interrupt.
 * @param None.
 * @retval The status of the interrupt.
 */
static inline unsigned int DCL_SYSCTRL_GetPeriMstAccessExceptionInt(void)
{
    return SYSCTRL1->PERI_MST_ACCESS_EXCEPTION_INT.BIT.peri_mst_access_exception_int;
}

/**
 * @brief Enable the DFX (Design For Test) function for a GPIO.
 * @param gpioIndex The index of the GPIO.
 * @retval None.
 */
static inline void DCL_SYSCTRL_EnableGpioDfxEnable(SYSCTRL_GpioDfxIndex gpioIndex)
{
    SYSCTRL1->GPIO_DFX_CFG.reg = (BASE_CFG_SET << gpioIndex) & GPIO_DFX_ENABLE_MASK;
}

/**
 * @brief Disable the DFX (Design For Test) function for a GPIO.
 * @param gpioIndex The index of the GPIO.
 * @retval None.
 */
static inline void DCL_SYSCTRL_DisableGpioDfxEnable(SYSCTRL_GpioDfxIndex gpioIndex)
{
    SYSCTRL1->GPIO_DFX_CFG.reg = (~(BASE_CFG_SET << gpioIndex) & GPIO_DFX_ENABLE_MASK);
}

/**
 * @brief Configure the JTAG password.
 * @param passWord The password configuration containing four parts.
 * @retval None.
 */
static inline void DCL_SYSCTRL_ConfigJtagPassWord(JTAG_LockPassWord passWord)
{
    JTAG_LOCK->JTAG_PSWD_CFG0 = passWord.passWord0;
    JTAG_LOCK->JTAG_PSWD_CFG1 = passWord.passWord1;
    JTAG_LOCK->JTAG_PSWD_CFG2 = passWord.passWord2;
    JTAG_LOCK->JTAG_PSWD_CFG3 = passWord.passWord3;
}

/**
 * @brief Get the first part of the JTAG password.
 * @param None.
 * @retval The value of the first password part.
 */
static inline unsigned int DCL_SYSCTRL_GetJtagPassWord0(void)
{
    return JTAG_LOCK->JTAG_PSWD_CFG0;
}

/**
 * @brief Get the second part of the JTAG password.
 * @param None.
 * @retval The value of the second password part.
 */
static inline unsigned int DCL_SYSCTRL_GetJtagPassWord1(void)
{
    return JTAG_LOCK->JTAG_PSWD_CFG1;
}

/**
 * @brief Get the third part of the JTAG password.
 * @param None.
 * @retval The value of the third password part.
 */
static inline unsigned int DCL_SYSCTRL_GetJtagPassWord2(void)
{
    return JTAG_LOCK->JTAG_PSWD_CFG2;
}

/**
 * @brief Get the fourth part of the JTAG password.
 * @param None.
 * @retval The value of the fourth password part.
 */
static inline unsigned int DCL_SYSCTRL_GetJtagPassWord3(void)
{
    return JTAG_LOCK->JTAG_PSWD_CFG3;
}

/**
 * @brief Get the JTAG lock status.
 * @param None.
 * @retval The current lock status of the JTAG.
 */
static inline JTAG_LockStatus DCL_SYSCTRL_GetJtagLockStatus(void)
{
    /* Enable gpio dfx. */
    return (JTAG_LockStatus)JTAG_LOCK->JTAG_LOCK_STA.BIT.jtag_lock_sta;
}

#endif /* McuMagicTag_SYSCTRL_H */
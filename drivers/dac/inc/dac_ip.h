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
  * @file    dac_ip.h
  * @author  MCU Driver Team
  * @brief   DAC module driver.
  *          This file provides DCL functions to manage DAC and Definitions of specific parameters.
  *           + Definition of DAC configuration parameters.
  *           + DAC register mapping structure.
  *           + Parameters check functions.
  *           + Direct configuration layer interface.
  */

#ifndef McuMagicTag_DAC_IP_H
#define McuMagicTag_DAC_IP_H

#include "baseinc.h"

#ifdef DAC_PARAM_CHECK
#define DAC_ASSERT_PARAM BASE_FUNC_ASSERT_PARAM
#define DAC_PARAM_CHECK_NO_RET BASE_FUNC_PARAMCHECK_NO_RET
#define DAC_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
#define DAC_ASSERT_PARAM(para) ((void)0U)
#define DAC_PARAM_CHECK_NO_RET(para) ((void)0U)
#define DAC_PARAM_CHECK_WITH_RET(para, ret) ((void)0U)
#endif

#define DAC_MAX_OUT_VALUE 0xFFF

/**
  * @addtogroup DAC
  * @{
  */

/**
  * @defgroup DAC_IP DAC_IP
  * @brief DAC_IP: dac_v2.
  * @{
  */

/**
  * @defgroup DAC_REG_Definition DAC Register Structure.
  * @brief DAC Register Structure Definition.
  * @{
  */

/**
 * @brief Vref Selection Enumeration Definition
 */
typedef enum {
    DAC_VREF_SELECT_VDDA  = 0x00000000U,
    DAC_VREF_SELECT_VDAC = 0x00000001U,
    DAC_VREF_MAX
} DAC_VrefSrc;

/**
 * @brief Vset value Selection Enumeration Definition
 */
typedef enum {
    VSET_SRC_FROM_REG  = 0x00000000U,
    VSET_SRC_FROM_RAMP = 0x00000001U,
    VSET_SRC_MAX
} DAC_VsetSrc;

/**
 * @brief Vset value load mode selection Enumeration Definition
 */
typedef enum {
    VSET_LOAD_IMMEDIATE_MODE  = 0x00000000U,
    VSET_LOAD_APTSYNCP_MODE = 0x00000001U,
    VSET_LOAD_MAX
} DAC_VsetLoadMode;

/**
 * @brief Vset value load mode selection Enumeration Definition
 */
typedef enum {
    VSET_SYNCP_APT0  = 0x00000000U,
    VSET_SYNCP_APT1  = 0x00000001U,
    VSET_SYNCP_APT2  = 0x00000002U,
    VSET_SYNCP_APT3  = 0x00000003U,
    VSET_SYNCP_APT4  = 0x00000004U,
    VSET_SYNCP_APT5  = 0x00000005U,
    VSET_SYNCP_APT6  = 0x00000006U,
    VSET_SYNCP_APT7  = 0x00000007U,
    VSET_SYNCP_APT8  = 0x00000008U,
    VSET_SYNCP_APT9  = 0x00000009U,
    VSET_SYNCP_APT10  = 0x0000000AU,
    VSET_SYNCP_APT11  = 0x0000000BU,
    VSET_SYNCP_APT12  = 0x0000000CU,
    VSET_SYNCP_APT13  = 0x0000000DU,
    VSET_SYNCP_APT14  = 0x0000000EU,
    VSET_SYNCP_APT15  = 0x0000000FU,
    VSET_SYNCP_MAX
} DAC_VsetSyncpSrc;

/**
 * @brief Ramp load selection Enumeration Definition
 */
typedef enum {
    RAMP_LOAD_FROM_ACTIVE_VALUE  = 0x00000000U,
    RAMP_LOAD_FROM_BUFFER_VALUE = 0x00000001U,
    RAMP_LOAD_MAX
} DAC_RampLoadMode;

/**
 * @brief Ramp cross-connect trigger selection Enumeration Definition
 */
typedef enum {
    RAMP_XTRIG_SELECT_APTSYNCP  = 0x00000000U,
    RAMP_XTRIG_SELECT_ACMP = 0x00000001U,
    RAMP_XTRIG_MAX
} DAC_RampXtrig;

/**
 * @brief Ramp direction selection Enumeration Definition
 */
typedef enum {
    RAMP_DIR_DEC = 0x00000000U,
    RAMP_DIR_INC = 0x00000001U,
    RAMP_DIR_MAX
} DAC_RampDir;

/**
 * @brief Ramp debug mode selection Enumeration Definition
 */
typedef enum {
    RAMP_DEBUG_STOP_IMMEDIATE  = 0x00000000U,
    RAMP_DEBUG_STOP_NEXT_SYNCP = 0x00000001U,
    RAMP_DEBUG_NONE_STOP = 0x00000002U,
    RAMP_DEBUG_MAX
} DAC_RampDebugMode;

/**
  * @defgroup DAC_Ramp attribute Structure.
  * @brief DAC Register Structure Definition.
  * @{
  */
typedef struct {
    DAC_RampLoadMode           rampLoadMode;    /* DAC ramp load select. */
    DAC_RampXtrig              rampXtrig;       /* DAC ramp cross-connect trigger. */
    DAC_RampDir                rampDir;          /* DAC ramp direction. */
    unsigned short             rampStep;
    unsigned char              rampClkDiv;
    unsigned short             rampRefVal;
    unsigned short             rampDelayVal;
    DAC_RampDebugMode          rampDebugMode;   /* DAC ramp debug mode. */
} DAC_RampAttr;

/**
  * @defgroup DAC_Vset attribute Structure.
  * @brief DAC Register Structure Definition.
  * @{
  */
typedef struct {
    DAC_VsetSrc                vsetSrc;         /* DAC vset source. */
    DAC_VsetLoadMode           vsetLoadMode;    /* DAC vset load mode. */
    DAC_VsetSyncpSrc           vsetSyncpSrc;    /* DAC vset syncp src. */
} DAC_VsetAttr;

/**
  * @defgroup DAC_REG_Definition DAC Register Structure.
  * @brief DAC Register Structure Definition.
  * @{
  */
typedef struct {
    bool                    pinOutputEn;
    DAC_VrefSrc             vrefSelect;      /* DAC vref select. */
} DAC_ExtendHandle;

/**
  * @brief Define the union DAC_CTRL_REG0_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    da_dac_enh            : 1   ; /* [0] */
        unsigned int    reserved0             : 31  ; /* [31..1] */
    } BIT;
} DAC_CTRL_REG0_REG;

/**
  * @brief Define the union DAC_CTRL_REG1_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cfg_dac_vset          : 12  ; /* [11..0] */
        unsigned int    reserved0             : 20  ; /* [31..12] */
    } BIT;
} DAC_CTRL_REG1_REG;

/**
  * @brief Define the union DAC_CTRL_REG1_A_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    ro_dac_vset           : 12  ; /* [11..0] */
        unsigned int    reserved0             : 20  ; /* [31..12] */
    } BIT;
} DAC_CTRL_REG1_A_REG;

/**
  * @brief Define the union DAC_CTRL_REG2_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cfg_dac_vset_src      : 1   ; /* [0] */
        unsigned int    cfg_dac_vset_load     : 1   ; /* [1] */
        unsigned int    reserved0             : 2   ; /* [3..2] */
        unsigned int    cfg_dac_syncp         : 4   ; /* [7..4] */
        unsigned int    reserved1             : 7   ; /* [14..8] */
        unsigned int    cfg_dac_ramp_load_sel : 1   ; /* [15] */
        unsigned int    cfg_dac_ramp_xtrig    : 1   ; /* [16] */
        unsigned int    reserved2             : 3   ; /* [19..17] */
        unsigned int    cfg_dac_ramp_dir      : 1   ; /* [20] */
        unsigned int    reserved3             : 1   ; /* [21] */
        unsigned int    cfg_dac_debug_mode    : 2   ; /* [23..22] */
        unsigned int    reserved4             : 8   ; /* [31..24] */
    } BIT;
} DAC_CTRL_REG2_REG;

/**
  * @brief Define the union DAC_CTRL_REG3_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cfg_dac_ramp_start_sw : 1   ; /* [0] */
        unsigned int    reserved0             : 31  ; /* [31..1] */
    } BIT;
} DAC_CTRL_REG3_REG;

/**
  * @brief Define the union DAC_RAMP_REG0_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cfg_dac_ramp_step     : 16  ; /* [15..0] */
        unsigned int    cfg_dac_ramp_clkdiv   : 8   ; /* [23..16] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} DAC_RAMP_REG0_REG;

/**
  * @brief Define the union DAC_RAMP_REG0_A_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    ro_dac_ramp_step      : 16  ; /* [15..0] */
        unsigned int    ro_dac_ramp_clkdiv    : 8   ; /* [23..16] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} DAC_RAMP_REG0_A_REG;

/**
  * @brief Define the union DAC_RAMP_REG1_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    cfg_dac_ramp_ref      : 16  ; /* [15..0] */
        unsigned int    cfg_dac_ramp_dly      : 16  ; /* [31..16] */
    } BIT;
} DAC_RAMP_REG1_REG;

/**
  * @brief Define the union DAC_RAMP_REG1_A_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    ro_dac_ramp_ref       : 16  ; /* [15..0] */
        unsigned int    ro_dac_ramp_dly       : 16  ; /* [31..16] */
    } BIT;
} DAC_RAMP_REG1_A_REG;

/**
  * @brief Define the union DAC_RAMP_STS_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    ro_dac_ramp_sts       : 16  ; /* [15..0] */
        unsigned int    ro_dac_ramp_dly_sts   : 16  ; /* [31..16] */
    } BIT;
} DAC_RAMP_STS_REG;

/**
  * @brief Define the union DAC_TEST_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    dac_testout_en        : 1   ; /* [0] */
        unsigned int    reserved0             : 31  ; /* [31..1] */
    } BIT;
} DAC_TEST_REG;

/**
  * @brief Define the union DAC_VREF_REG.
  */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    da_dac_vref           : 1   ; /* [0] */
        unsigned int    reserved0             : 31  ; /* [31..1] */
    } BIT;
} DAC_VREF_REG;

/**
  * @brief Define the DAC reg struct.
  */
typedef struct {
    DAC_CTRL_REG0_REG        DAC_CTRL_REG0                    ; /* 0x0 */
    unsigned int             reserved0[3]                     ; /* 0x4~0xc */
    DAC_CTRL_REG1_REG        DAC_CTRL_REG1                    ; /* 0x10 */
    DAC_CTRL_REG1_A_REG      DAC_CTRL_REG1_A                  ; /* 0x14 */
    DAC_CTRL_REG2_REG        DAC_CTRL_REG2                    ; /* 0x18 */
    DAC_CTRL_REG3_REG        DAC_CTRL_REG3                    ; /* 0x1c */
    unsigned int             reserved1[4]                     ; /* 0x20~0x2c */
    DAC_RAMP_REG0_REG        DAC_RAMP_REG0                    ; /* 0x30 */
    DAC_RAMP_REG0_A_REG      DAC_RAMP_REG0_A                  ; /* 0x34 */
    DAC_RAMP_REG1_REG        DAC_RAMP_REG1                    ; /* 0x38 */
    DAC_RAMP_REG1_A_REG      DAC_RAMP_REG1_A                  ; /* 0x3c */
    DAC_RAMP_STS_REG         DAC_RAMP_STS                     ; /* 0x40 */
    unsigned int             reserved2[17]                    ; /* 0x44~0x84 */
    DAC_TEST_REG             DAC_TEST_REG                     ; /* 0x88 */
    DAC_VREF_REG             DAC_VREF_REG                     ; /* 0x8c */
} volatile DAC_RegStruct;

/**
  * @brief Verify count value of the DAC.
  * @param dacValue    Pwm number, only valid if keep equ 0
  * @retval true or false
  */
static inline bool IsDacConfigureValue(unsigned short dacValue)
{
    return ((dacValue) <= DAC_MAX_OUT_VALUE);
}

/**
  * @brief Verify vref select value of the DAC.
  * @param vrefSelect, Value of @ref DAC_VrefSrc.
  * @retval true or false
  */
static inline bool IsDacVref(DAC_VrefSrc vrefSelect)
{
    return (vrefSelect >= 0 && vrefSelect < DAC_VREF_MAX);
}

/**
  * @brief Verify vset source value of the DAC.
  * @param vsetSrc, Value of @ref DAC_VsetSrc.
  * @retval true or false
  */
static inline bool IsDacVsetSrc(DAC_VsetSrc vsetSrc)
{
    return (vsetSrc >= 0 && vsetSrc < VSET_SRC_MAX);
}

/**
  * @brief Verify vset load mode value of the DAC.
  * @param vsetLoadMode, Value of @ref DAC_VsetLoadMode.
  * @retval true or false
  */
static inline bool IsDacVsetLoadMode(DAC_VsetLoadMode vsetLoadMode)
{
    return (vsetLoadMode >= 0 && vsetLoadMode < VSET_LOAD_MAX);
}

/**
  * @brief Verify vset syncp source value of the DAC.
  * @param syncpSrc, Value of @ref DAC_VsetSyncpSrc.
  * @retval true or false
  */
static inline bool IsDacVsetSyncpSrc(DAC_VsetSyncpSrc syncpSrc)
{
    return (syncpSrc >= 0 && syncpSrc < VSET_SYNCP_MAX);
}

/**
  * @brief Verify ramp load mode value of the DAC.
  * @param select, Value of @ref DAC_RampLoadMode.
  * @retval true or false
  */
static inline bool IsDacRampLoad(DAC_RampLoadMode select)
{
    return (select >= 0 && select < RAMP_LOAD_MAX);
}

/**
  * @brief Verify cross-connect trigger mode value of the DAC.
  * @param select, Value of @ref DAC_RampXtrig.
  * @retval true or false
  */
static inline bool IsDacRampXtrig(DAC_RampXtrig select)
{
    return (select >= 0 && select < RAMP_XTRIG_MAX);
}

/**
  * @brief Verify ramP direction value of the DAC.
  * @param dir, Value of @ref DAC_RampDir.
  * @retval true or false
  */
static inline bool IsDacRampDir(DAC_RampDir dir)
{
    return (dir >= 0 && dir < RAMP_DIR_MAX);
}

/**
  * @brief Verify dac debug mode value of the DAC.
  * @param rampDebugMode, Value of @ref DAC_RampDebugMode.
  * @retval true or false
  */
static inline bool IsDacRampDebugMode(DAC_RampDebugMode rampDebugMode)
{
    return (rampDebugMode >= 0 && rampDebugMode < RAMP_DEBUG_MAX);
}

/**
  * @brief Enable DAC
  * @param dacx: DAC register base address.
  * @retval None.
  */
static inline void DCL_DAC_Enable(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    dacx->DAC_CTRL_REG0.BIT.da_dac_enh = BASE_CFG_ENABLE;
}

/**
  * @brief Disable DAC
  * @param dacx: DAC register base address.
  * @retval None.
  */
static inline void DCL_DAC_Disable(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    dacx->DAC_CTRL_REG0.BIT.da_dac_enh = BASE_CFG_DISABLE;
}

/**
  * @brief DAC Vref select VDDA or VDAC.
  * @param dacx: DAC register base address.
  * @retval None.
  */
static inline void DCL_DAC_SetVrefSrc(DAC_RegStruct *dacx, DAC_VrefSrc vrefSelct)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    DAC_PARAM_CHECK_NO_RET(IsDacVref(vrefSelct));
    dacx->DAC_VREF_REG.BIT.da_dac_vref = vrefSelct;
}

/**
  * @brief Get DAC Vref.
  * @param dacx: DAC register base address.
  * @retval DAC Vref. Value of @ref DAC_VrefSrc.
  */
static inline DAC_VrefSrc DCL_DAC_GetVrefSrc(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_VREF_REG.BIT.da_dac_vref;
}


/**
  * @brief Set DAC value
  * @param dacx: DAC register base address.
  * @param value: DAC value.
  * @retval None.
  */
static inline void DCL_DAC_SetValue(DAC_RegStruct *dacx, unsigned int value)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    DAC_PARAM_CHECK_NO_RET(value <= DAC_MAX_OUT_VALUE);
    dacx->DAC_CTRL_REG1.BIT.cfg_dac_vset = value;
}

/**
  * @brief Get Get DAC value
  * @param dacx: DAC register base address.
  * @retval the value of dac vset.
  */
static inline unsigned int DCL_DAC_GetValue(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_CTRL_REG1_A.BIT.ro_dac_vset;
}

/**
  * @brief Set DAC test mode config, pin ouput enable or diable
  * @param dacx: DAC register base address.
  * @param config: DAC output enable bit value.
  * @retval None.
  */
static inline void DCL_DAC_SetPinOutputConfig(DAC_RegStruct *dacx, bool config)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    dacx->DAC_TEST_REG.BIT.dac_testout_en = config;
}

/**
  * @brief Get DAC test mode config, pin ouput enable or diable
  * @param dacx: DAC register base address.
  * @retval enable or disable.
  */
static inline bool DCL_DAC_GetPinOutputConfig(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_TEST_REG.BIT.dac_testout_en;
}

/**
  * @brief Set Vset source.
  * @param dacx: DAC register base address.
  * @param vsetSrc Value of @ref DAC_VsetSrc.
  * @retval None.
  */
static inline void DCL_DAC_SetVsetSrc(DAC_RegStruct *dacx, DAC_VsetSrc vsetSrc)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    DAC_PARAM_CHECK_NO_RET(IsDacVsetSrc(vsetSrc));
    dacx->DAC_CTRL_REG2.BIT.cfg_dac_vset_src = vsetSrc;
}

/**
  * @brief Get Vset source.
  * @param dacx: DAC register base address.
  * @retval Vset source. Value of @ref DAC_VsetSrc.
  */
static inline DAC_VsetSrc DCL_DAC_GetVsetSrc(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_CTRL_REG2.BIT.cfg_dac_vset_src;
}

/**
  * @brief Set Vset load mode.
  * @param dacx: DAC register base address.
  * @param vsetSrc Value of @ref DAC_VsetLoadMode.
  * @retval None.
  */
static inline void DCL_DAC_SetVsetLoadMode(DAC_RegStruct *dacx, DAC_VsetLoadMode vsetLoadMode)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    DAC_PARAM_CHECK_NO_RET(IsDacVsetLoadMode(vsetLoadMode));
    dacx->DAC_CTRL_REG2.BIT.cfg_dac_vset_load = vsetLoadMode;
}

/**
  * @brief Get Vset load mode.
  * @param dacx: DAC register base address.
  * @retval Vset load mode. Value of @ref DAC_VsetLoadMode.
  */
static inline DAC_VsetLoadMode DCL_DAC_GetVsetLoadMode(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_CTRL_REG2.BIT.cfg_dac_vset_load;
}

/**
  * @brief Set Vset syncp source.
  * @param dacx: DAC register base address.
  * @param syncpSrc Value of @ref DAC_VsetSyncpSrc.
  * @retval None.
  */
static inline void DCL_DAC_SetVsetSyncpSrc(DAC_RegStruct *dacx, DAC_VsetSyncpSrc syncpSrc)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    DAC_PARAM_CHECK_NO_RET(IsDacVsetSyncpSrc(syncpSrc));
    dacx->DAC_CTRL_REG2.BIT.cfg_dac_syncp = syncpSrc;
}

/**
  * @brief Get Vset syncp source.
  * @param dacx: DAC register base address.
  * @retval Vset syncp source. Value of @ref DAC_VsetSyncpSrc.
  */
static inline DAC_VsetSyncpSrc DCL_DAC_GetVsetSyncpSrc(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_CTRL_REG2.BIT.cfg_dac_syncp;
}

/**
  * @brief Set DAC Ramp load.
  * @param dacx: DAC register base address.
  * @param rampLoad Value of @ref DAC_RampLoadMode.
  * @retval None.
  */
static inline void DCL_DAC_SetRampLoadMode(DAC_RegStruct *dacx, DAC_RampLoadMode rampLoad)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    DAC_PARAM_CHECK_NO_RET(IsDacRampLoad(rampLoad));
    dacx->DAC_CTRL_REG2.BIT.cfg_dac_ramp_load_sel = rampLoad;
}

/**
  * @brief Get DAC Ramp load.
  * @param dacx: DAC register base address.
  * @retval DAC Ramp load. Value of @ref DAC_RampLoadMode.
  */
static inline DAC_RampLoadMode DCL_DAC_GetRampLoadMode(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_CTRL_REG2.BIT.cfg_dac_ramp_load_sel;
}

/**
  * @brief Set DAC Ramp cross-connect trigger.
  * @param dacx: DAC register base address.
  * @param xtrig Value of @ref DAC_RampXtrig.
  * @retval None.
  */
static inline void DCL_DAC_SetRampXtrig(DAC_RegStruct *dacx, DAC_RampXtrig xtrig)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    DAC_PARAM_CHECK_NO_RET(IsDacRampXtrig(xtrig));
    dacx->DAC_CTRL_REG2.BIT.cfg_dac_ramp_xtrig = xtrig;
}

/**
  * @brief Get DAC Ramp cross-connect trigger.
  * @param dacx: DAC register base address.
  * @retval Ramp cross-connect trigger. Value of @ref DAC_RampXtrig.
  */
static inline DAC_RampXtrig DCL_DAC_GetRampXtrig(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_CTRL_REG2.BIT.cfg_dac_ramp_xtrig;
}

/**
  * @brief Set DAC Ramp dir.
  * @param dacx: DAC register base address.
  * @param dir Value of @ref DAC_RampDir.
  * @retval None.
  */
static inline void DCL_DAC_SetRampDir(DAC_RegStruct *dacx, DAC_RampDir dir)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    DAC_PARAM_CHECK_NO_RET(IsDacRampDir(dir));
    dacx->DAC_CTRL_REG2.BIT.cfg_dac_ramp_dir = dir;
}

/**
  * @brief Get DAC Ramp dir.
  * @param dacx: DAC register base address.
  * @retval Ramp dir. Value of @ref DAC_RampDir.
  */
static inline DAC_RampDir DCL_DAC_GetRampDir(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_CTRL_REG2.BIT.cfg_dac_ramp_dir;
}

/**
  * @brief Set DAC Debug mode.
  * @param dacx: DAC register base address.
  * @param dir Value of @ref DAC_RampDebugMode.
  * @retval None.
  */
static inline void DCL_DAC_SetRampDebugMode(DAC_RegStruct *dacx, DAC_RampDebugMode rampDebugMode)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    DAC_PARAM_CHECK_NO_RET(IsDacRampDebugMode(rampDebugMode));
    dacx->DAC_CTRL_REG2.BIT.cfg_dac_debug_mode = rampDebugMode;
}

/**
  * @brief Get DAC Debug mode.
  * @param dacx: DAC register base address.
  * @retval Debug mode. Value of @ref DAC_RampDebugMode.
  */
static inline DAC_RampDebugMode DCL_DAC_GetRampDebugMode(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_CTRL_REG2.BIT.cfg_dac_debug_mode;
}

/**
  * @brief Set DAC Debug Ramp step.
  * @param dacx: DAC register base address.
  * @param step ramp step.
  * @retval None.
  */
static inline void DCL_DAC_SetRampStep(DAC_RegStruct *dacx, unsigned short step)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    dacx->DAC_RAMP_REG0.BIT.cfg_dac_ramp_step = step;
}

/**
  * @brief Get DAC Debug Ramp step.
  * @param dacx: DAC register base address.
  * @retval Ramp step.
  */
static inline unsigned short DCL_DAC_GetRampStep(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_RAMP_REG0_A.BIT.ro_dac_ramp_step;
}

/**
  * @brief Set DAC Debug Ramp Clock Division.
  * @param dacx: DAC register base address.
  * @param div Clock Division value.
  * @retval None.
  */
static inline void DCL_DAC_SetRampClkDiv(DAC_RegStruct *dacx, unsigned char div)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    dacx->DAC_RAMP_REG0.BIT.cfg_dac_ramp_clkdiv = div;
}

/**
  * @brief Get DAC Ramp Clock Division.
  * @param dacx: DAC register base address.
  * @retval Clock Division value.
  */
static inline unsigned char DCL_DAC_GetRampClkDiv(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_RAMP_REG0_A.BIT.ro_dac_ramp_clkdiv;
}

/**
  * @brief Set DAC Ramp ref value.
  * @param dacx: DAC register base address.
  * @param div Clock Division value.
  * @retval None.
  */
static inline void DCL_DAC_SetRampRefVal(DAC_RegStruct *dacx, unsigned short val)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    dacx->DAC_RAMP_REG1.BIT.cfg_dac_ramp_ref = val;
}

/**
  * @brief Get DAC Ramp ref value.
  * @param dacx: DAC register base address.
  * @retval Ramp ref value.
  */
static inline unsigned short DCL_DAC_GetRampRefVal(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_RAMP_REG1_A.BIT.ro_dac_ramp_ref;
}

/**
  * @brief Set DAC Ramp Delay value.
  * @param dacx: DAC register base address.
  * @param div Clock Division value.
  * @retval None.
  */
static inline void DCL_DAC_SetRampDelayVal(DAC_RegStruct *dacx, unsigned short val)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    dacx->DAC_RAMP_REG1.BIT.cfg_dac_ramp_dly = val;
}

/**
  * @brief Get DAC Ramp Delay value.
  * @param dacx: DAC register base address.
  * @retval Ramp Delay value.
  */
static inline unsigned short DCL_DAC_GetRampDelayVal(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_RAMP_REG1_A.BIT.ro_dac_ramp_dly;
}

/**
  * @brief Get DAC Ramp realtime ref value.
  * @param dacx: DAC register base address.
  * @retval Ramp ref value.
  */
static inline unsigned short DCL_DAC_GetRampRealtimeRefVal(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_RAMP_STS.BIT.ro_dac_ramp_sts;
}

/**
  * @brief Get DAC Ramp realtime delay value.
  * @param dacx: DAC register base address.
  * @retval Ramp realtime delay value.
  */
static inline unsigned short DCL_DAC_GetRampRealtimeDelayVal(DAC_RegStruct *dacx)
{
    DAC_ASSERT_PARAM(IsDACInstance(dacx));
    return dacx->DAC_RAMP_STS.BIT.ro_dac_ramp_dly_sts;
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

#endif /* McuMagicTag_DAC_IP_H */

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
  * @file    srm_ip.h
  * @author  MCU Driver Team
  * @brief   SRM module driver.
  * @details This file provides firmware functions to manage the following
  *          functionalities of the SRM.
  *          + Register definition structure
  *          + Direct configuration layer interface
  *          + Basic parameter configuration macro
  */

#ifndef McuMagicTag_SRM_IP_H
#define McuMagicTag_SRM_IP_H

/* Includes ------------------------------------------------------------------*/
#include "baseinc.h"
/* Macro definition */
#ifdef SRM_PARAM_CHECK
    #define SRM_ASSERT_PARAM         BASE_FUNC_ASSERT_PARAM
    #define SRM_PARAM_CHECK_NO_RET   BASE_FUNC_PARAMCHECK_NO_RET
    #define SRM_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
    #define SRM_ASSERT_PARAM(para)                ((void)0U)
    #define SRM_PARAM_CHECK_NO_RET(para)          ((void)0U)
    #define SRM_PARAM_CHECK_WITH_RET(param, ret)  ((void)0U)
#endif

#define SRM_CH_RENAME(chId, resourceName)     (SRM_CH_ID_##chId)

typedef enum {
    SRM_CH_ID_0 = 0,
    SRM_CH_ID_1,
    SRM_CH_ID_2,
    SRM_CH_ID_3,
    SRM_CH_ID_4,
    SRM_CH_ID_5,
    SRM_CH_ID_6,
    SRM_CH_ID_7,
    SRM_CH_ID_8,
    SRM_CH_ID_9,
    SRM_CH_ID_10,
    SRM_CH_ID_11,
    SRM_CH_ID_12,
    SRM_CH_ID_13,
    SRM_CH_ID_14,
    SRM_CH_ID_15,
    SRM_CH_ID_16,
    SRM_CH_ID_17,
    SRM_CH_ID_18,
    SRM_CH_ID_19,
    SRM_CH_ID_20,
    SRM_CH_ID_21,
    SRM_CH_ID_22,
    SRM_CH_ID_23,
    SRM_CH_ID_24,
    SRM_CH_ID_25,
    SRM_CH_ID_26,
    SRM_CH_ID_27,
    SRM_CH_ID_28,
    SRM_CH_ID_29,
    SRM_CH_ID_30,
    SRM_CH_ID_31,
    SRM_CH_ID_MAX
} SRM_ChannelId;

typedef enum {
    SRM_MODE_CHECK = 0,
    SRM_MODE_WRITE,
    SRM_MODE_MAX
} SRM_Mode;

/**
  * @brief SRM extend handle.
  */
typedef struct _SRM_ExtendHandle {
} SRM_ExtendHandle;

/**
  * @brief SRM user callback.
  */
typedef struct {
    void (* FinishCallbackFunc)(void *handle);  /**< SRM callback Function */
} SRM_UserCallBack[SRM_CH_ID_MAX];

/* Define the union SRM_LOCK_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    srm_flag              : 1   ; /* [0] */
        unsigned int    srm_lock_cpu_id       : 2   ; /* [2..1] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile SRM_LOCK_REG;

/* Define the union SRM_MODE_CFG_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    srm_mode              : 1   ; /* [0] */
        unsigned int    reserved0             : 31  ; /* [31..1] */
    } BIT;
} volatile SRM_MODE_CFG_REG;

/* Define the union SRM_INT_SRC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    srm_src_int           : 1   ; /* [0] */
        unsigned int    reserved0             : 31  ; /* [31..1] */
    } BIT;
} volatile SRM_INT_SRC_REG;

/* Define the struct SRM_CH_RegStruct */
typedef struct {
    SRM_LOCK_REG             SRM_LOCK;
    SRM_MODE_CFG_REG         SRM_MODE_CFG;
    SRM_INT_SRC_REG          SRM_INT_SRC;
    unsigned int             RESERVED;
} volatile SRM_CH_RegStruct;

/* Define the struct SRM_CORE_RegStruct */
typedef struct {
    volatile unsigned int    SRM_CORE_INT_EN;
    volatile unsigned int    SRM_CORE_INT;
    unsigned int             RESERVED[2];
} volatile SRM_CORE_RegStruct;

/* Define the union SRM_WFI_CFG_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    srm_wfi_en            : 1   ; /* [0] */
        unsigned int    reserved0             : 7   ; /* [7..1] */
        unsigned int    srm_wfi_sel           : 5   ; /* [12..8] */
        unsigned int    reserved1             : 3   ; /* [15..13] */
        unsigned int    srm_wfi_cnt_thld      : 8   ; /* [23..16] */
        unsigned int    reserved2             : 8   ; /* [31..24] */
    } BIT;
} volatile SRM_WFI_CFG_REG;

/* Define the global struct */
typedef struct {
    SRM_CH_RegStruct            SRM_CH[32]                       ; /* 0x0~0x1f0 */
    unsigned int                reserved0[384]                   ; /* 0x200~0x7fc */
    SRM_CORE_RegStruct          SRM_CORE[3]                      ; /* 0x800~0x820 */
    unsigned int                reserved1[244]                   ; /* 0x830~0xbfc */
    unsigned int                SRM_LOCK_STA                     ; /* 0xc00 */
    SRM_WFI_CFG_REG             SRM_WFI_CFG                      ; /* 0xc04 */
} volatile SRM_RegStruct;

/**
  * @brief Check id whether is srm channel.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval bool true or false.
  */
static inline bool IsSrmChannelId(SRM_ChannelId srmId)
{
    return (srmId < SRM_CH_ID_MAX && srmId >= SRM_CH_ID_0);
}

/**
  * @brief Check mode whether is srm mode.
  * @param srmMode SRM mode @ref SRM_Mode.
  * @retval bool true or false.
  */
static inline bool IsSrmMode(SRM_Mode srmMode)
{
    return (srmMode < SRM_MODE_MAX && srmMode >= SRM_MODE_CHECK);
}

/**
  * @brief Set srm mode.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @param srmMode SRM mode @ref SRM_Mode.
  * @retval bool true means has been config success, false means has been config fail.
  */
static inline bool DCL_SRM_SetMode(SRM_RegStruct *srm, SRM_ChannelId srmId, SRM_Mode srmMode)
{
    SRM_ASSERT_PARAM(srm != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmId), false);
    SRM_PARAM_CHECK_WITH_RET(IsSrmMode(srmMode), false);
    srm->SRM_CH[srmId].SRM_MODE_CFG.BIT.srm_mode = srmMode; /* Check or write mode */
    return (srm->SRM_CH[srmId].SRM_MODE_CFG.BIT.srm_mode == srmMode) ? true : false;
}

/**
  * @brief Lock srm channel with id.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval None.
  */
static inline void DCL_SRM_LockChannel(SRM_RegStruct *srm, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    BASE_FUNC_PARAMCHECK_NO_RET(IsSrmChannelId(srmId));
    unsigned int srmMode = srm->SRM_CH[srmId].SRM_MODE_CFG.BIT.srm_mode;
    if (srmMode == SRM_MODE_WRITE) {
        srm->SRM_CH[srmId].SRM_LOCK.BIT.srm_flag = BASE_CFG_ENABLE;
    } else if (srmMode == SRM_MODE_CHECK) {
        unsigned int flag = srm->SRM_CH[srmId].SRM_LOCK.BIT.srm_flag; /* lock by read */
        BASE_FUNC_UNUSED(flag);
    }
}

/**
  * @brief UnLock srm channel with id.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval None.
  */
static inline void DCL_SRM_UnLockChannel(SRM_RegStruct *srm, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    BASE_FUNC_PARAMCHECK_NO_RET(IsSrmChannelId(srmId));
    srm->SRM_CH[srmId].SRM_LOCK.BIT.srm_flag = BASE_CFG_DISABLE;
}

/**
  * @brief Get srm channel share resource id.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval bool true means locked, false means unlocked.
  */
static inline ChipCoreId DCL_SRM_GetChannelCoreId(SRM_RegStruct *srm, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmId), false);
    return (1 << srm->SRM_CH[srmId].SRM_LOCK.BIT.srm_lock_cpu_id); /* 1 : cpu id convert to chip core id */
}

/**
  * @brief Get srm channel lock status with id.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval bool true means locked, false means unlocked.
  */
static inline bool DCL_SRM_GetChannelLockStatus(SRM_RegStruct *srm, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmId), false);
    return (((srm->SRM_LOCK_STA >> srmId) & BASE_CFG_SET) == BASE_CFG_SET) ? true : false;
}

/**
  * @brief Get srm channel int status with id.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval bool true means has interrupt, false means no interrupt.
  */
static inline bool DCL_SRM_GetChannelIntStatus(SRM_RegStruct *srm, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmId), false);
    return (srm->SRM_CH[srmId].SRM_INT_SRC.BIT.srm_src_int == BASE_CFG_SET) ? true : false;
}

/**
  * @brief Clear srm channel int status with id.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval bool true means has clear interrupt, false means has not clear interrupt.
  */
static inline bool DCL_SRM_ClearChannelInt(SRM_RegStruct *srm, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmId), false);
    srm->SRM_CH[srmId].SRM_INT_SRC.BIT.srm_src_int = BASE_CFG_SET;
    return (srm->SRM_CH[srmId].SRM_INT_SRC.BIT.srm_src_int == BASE_CFG_UNSET) ? true : false;
}

/**
  * @brief Enable core srm channel interrupt with id.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param coreId chip core id @ref ChipCoreId.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval bool true means has enable interrupt, false means has not enable interrupt.
  */
static inline bool DCL_SRM_EnableCoreChannelInt(SRM_RegStruct *srm, ChipCoreId coreId, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsChipCoreId(coreId), false);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmId), false);
    unsigned int cpuCoreId = coreId >> 1; /* chip core id convert to cpu core id */
    srm->SRM_CORE[cpuCoreId].SRM_CORE_INT_EN |= (BASE_CFG_SET << srmId);
    return ((srm->SRM_CORE[cpuCoreId].SRM_CORE_INT_EN >> srmId) & BASE_CFG_SET) ? true : false;
}

/**
  * @brief Disable core srm channel interrupt with id.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param coreId chip core id @ref ChipCoreId.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval bool true means has disable interrupt, false means has not disable interrupt.
  */
static inline bool DCL_SRM_DisableCoreChannelInt(SRM_RegStruct *srm, ChipCoreId coreId, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsChipCoreId(coreId), false);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmId), false);
    unsigned int cpuCoreId = coreId >> 1; /* chip core id convert to cpu core id */
    srm->SRM_CORE[cpuCoreId].SRM_CORE_INT_EN &= ~(BASE_CFG_SET << srmId);
    return (((srm->SRM_CORE[cpuCoreId].SRM_CORE_INT_EN >> srmId) & BASE_CFG_SET) == BASE_CFG_UNSET) ? true : false;
}

/**
  * @brief Get core srm channel interrupt status with id.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param coreId chip core id @ref ChipCoreId.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval bool true means has core channel interrupt, false means has no core channel interrupt.
  */
static inline bool DCL_SRM_GetCoreChannelIntStatus(SRM_RegStruct *srm, ChipCoreId coreId, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    SRM_PARAM_CHECK_WITH_RET(IsChipCoreId(coreId), false);
    SRM_PARAM_CHECK_WITH_RET(IsSrmChannelId(srmId), false);
    unsigned int cpuCoreId = coreId >> 1; /* chip core id convert to cpu core id */
    return ((srm->SRM_CORE[cpuCoreId].SRM_CORE_INT >> srmId) & BASE_CFG_SET) ? true : false;
}

/**
  * @brief Set core into wfi state time interval, one by one, unit cycle.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param timeCycle time unit cycle.
  * @retval bool true means has been config success, false means has been config fail.
  */
static inline bool DCL_SRM_SetWfiTime(SRM_RegStruct *srm, unsigned char timeCycle)
{
    SRM_ASSERT_PARAM(srm != NULL);
    srm->SRM_WFI_CFG.BIT.srm_wfi_cnt_thld = timeCycle;
    return (srm->SRM_WFI_CFG.BIT.srm_wfi_cnt_thld == timeCycle) ? true : false;
}

/**
  * @brief Get core into wfi state time interval, one by one, unit cycle.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @retval unsigned char time unit cycle.
  */
static inline unsigned char DCL_SRM_GetWfiTime(SRM_RegStruct *srm)
{
    SRM_ASSERT_PARAM(srm != NULL);
    return srm->SRM_WFI_CFG.BIT.srm_wfi_cnt_thld;
}

/**
  * @brief Set srm channel for core into wfi state.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @param srmId SRM channel id @ref SRM_ChannelId.
  * @retval bool true means has been config success, false means has been config fail.
  */
static inline bool DCL_SRM_SetWfiChannel(SRM_RegStruct *srm, SRM_ChannelId srmId)
{
    SRM_ASSERT_PARAM(srm != NULL);
    srm->SRM_WFI_CFG.BIT.srm_wfi_sel = srmId;
    return (srm->SRM_WFI_CFG.BIT.srm_wfi_sel == srmId) ? true : false;
}

/**
  * @brief Get srm channel for core into wfi state.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @retval SRM_ChannelId SRM channel id.
  */
static inline SRM_ChannelId DCL_SRM_GetWfiChannel(SRM_RegStruct *srm)
{
    SRM_ASSERT_PARAM(srm != NULL);
    return srm->SRM_WFI_CFG.BIT.srm_wfi_sel;
}

/**
  * @brief Enable core into wfi state one by one.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @retval bool true means has been config success, false means has been config fail.
  */
static inline bool DCL_SRM_EnableWfi(SRM_RegStruct *srm)
{
    SRM_ASSERT_PARAM(srm != NULL);
    srm->SRM_WFI_CFG.BIT.srm_wfi_en = BASE_CFG_ENABLE;
    return (srm->SRM_WFI_CFG.BIT.srm_wfi_en == BASE_CFG_ENABLE) ? true : false;
}

/**
  * @brief Disable core into wfi state one by one.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @retval bool true means has been config success, false means has been config fail.
  */
static inline bool DCL_SRM_DisableWfi(SRM_RegStruct *srm)
{
    SRM_ASSERT_PARAM(srm != NULL);
    srm->SRM_WFI_CFG.BIT.srm_wfi_en = BASE_CFG_DISABLE;
    return (srm->SRM_WFI_CFG.BIT.srm_wfi_en == BASE_CFG_DISABLE) ? true : false;
}

/**
  * @brief Get srm wfi state.
  * @param srm baseaddress @ref SRM_RegStruct.
  * @retval bool true means enbale wfi, false means disable wfi.
  */
static inline bool DCL_SRM_GetWfiStatus(SRM_RegStruct *srm)
{
    SRM_ASSERT_PARAM(srm != NULL);
    return (srm->SRM_WFI_CFG.BIT.srm_wfi_en == BASE_CFG_ENABLE) ? true : false;
}

#endif /* McuMagicTag_SRM_IP_H */

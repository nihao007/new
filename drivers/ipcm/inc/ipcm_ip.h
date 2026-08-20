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
  * @file    ipcm_ip.h
  * @author  MCU Driver Team
  * @brief   IPCM module driver.
  * @details This file provides firmware functions to manage the following
  *          functionalities of the IPCM.
  *          + Register definition structure
  *          + Direct configuration layer interface
  *          + Basic parameter configuration macro
  */

#ifndef McuMagicTag_IPCM_IP_H
#define McuMagicTag_IPCM_IP_H

/* Includes ------------------------------------------------------------------*/
#include "baseinc.h"
/* Macro definition */
#ifdef IPCM_PARAM_CHECK
    #define IPCM_ASSERT_PARAM         BASE_FUNC_ASSERT_PARAM
    #define IPCM_PARAM_CHECK_NO_RET   BASE_FUNC_PARAMCHECK_NO_RET
    #define IPCM_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
    #define IPCM_ASSERT_PARAM(para)                ((void)0U)
    #define IPCM_PARAM_CHECK_NO_RET(para)          ((void)0U)
    #define IPCM_PARAM_CHECK_WITH_RET(param, ret)  ((void)0U)
#endif

#define IPCM_UNLOCK_CMD       0x5A5A5A5AU
#define IPCM_LOCK_CMD         0xADDADAADU
#define MBX_MODE_MASK         0x00000003U

typedef enum {
    MBX_CH_0 = 0x00000000U,
    MBX_CH_1 = 0x00000001U,
    MBX_CH_2 = 0x00000002U,
    MBX_CH_3 = 0x00000003U,
    MBX_CH_4 = 0x00000004U,
    MBX_CH_5 = 0x00000005U,
    MBX_CH_MAX
} IPCM_MbxChId;

typedef enum {
    MBX_NORMAL_MODE                  = 0x00000000U,
    MBX_AUTO_REPLY_MODE              = 0x00000001U,
    MBX_AUTO_LINK_MODE               = 0x00000002U,
    MBX_AUTO_REPLY_LINK_MODE         = 0x00000003U,
    MBX_MODE_MAX
} IPCM_MbxChMode;

typedef enum {
    MBX_STATUS_FREE                  = 0x00000001U,
    MBX_STATUS_SOURCE                = 0x00000002U,
    MBX_STATUS_DEST                  = 0x00000004U,
    MBX_STATUS_REPLY                 = 0x00000008U,
    MBX_STATUS_MAX
} IPCM_MbxChStatus;

typedef enum {
    MBX_DATA_INDEX_0                 = 0x00000000U,
    MBX_DATA_INDEX_1                 = 0x00000001U,
    MBX_DATA_INDEX_2                 = 0x00000002U,
    MBX_DATA_INDEX_3                 = 0x00000003U,
    MBX_DATA_INDEX_MAX
} IPCM_MbxChDataIndex;

typedef enum {
    WRITE_FINISH_TYPE                = 0x00000000U,
    READ_FINISH_TYPE                 = 0x00000001U,
    TYPE_CALLBACK_MAX
} IPCM_CallbackType;

/**
  * @brief IPCM handle structure definition.
  */
typedef enum {
    TYPE_ENABLE_SRC_CORE_ID,
    TYPE_ENABLE_DEST_CORE_ID,
    TYPE_ENABLE_MBX_CH_MODE,
    TYPE_ENABLE_INT_NOTIFY_CORE,
    TYPE_DISABLE_INT_NOTIFY_CORE,
    TYPE_CH_WRITE_FINISH_CALLBACK,
    TYPE_CH_READ_FINISH_CALLBACK,
    TYPE_CONFIG_MAX
} IPCM_ConfigType;

/**
  * @brief IPCM extend handle.
  */
typedef struct _IPCM_ExtendHandle {
} IPCM_ExtendHandle;

/**
  * @brief IPCM user callback.
  */
typedef struct {
    void (* WriteItFinishCallback)(void *handle);  /**< IPCM callback Function */
    void (* ReadItFinishCallback)(void *handle);  /**< IPCM callback Function */
} IPCM_UserCallBack[MBX_CH_MAX];

/* Define the union MBX_SOURCE_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    source                : 3   ; /* [2..0] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile MBX_SOURCE_REG;

/* Define the union MBX_DSET_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    dset                  : 3   ; /* [2..0] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile MBX_DSET_REG;

/* Define the union MBX_DCLEAR_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    dclear                : 3   ; /* [2..0] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile MBX_DCLEAR_REG;

/* Define the union MBX_DSTATUS_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    dstatus               : 3   ; /* [2..0] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile MBX_DSTATUS_REG;

/* Define the union MBX_MODE_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    auto_answer           : 1   ; /* [0] */
        unsigned int    auto_link             : 1   ; /* [1] */
        unsigned int    reserved0             : 2   ; /* [3..2] */
        unsigned int    state_status          : 4   ; /* [7..4] */
        unsigned int    reserved1             : 24  ; /* [31..8] */
    } BIT;
} volatile MBX_MODE_REG;

/* Define the union MBX_IMASK_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    int_mask              : 3   ; /* [2..0] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile MBX_IMASK_REG;

/* Define the union MBX_ICLR_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    int_clear             : 3   ; /* [2..0] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile MBX_ICLR_REG;

/* Define the union MBX_SEND_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    send                  : 3   ; /* [2..0] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile MBX_SEND_REG;

/* Define the union CPU_IMST */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    int_mask_status       : 6   ; /* [5..0] */
        unsigned int    reserved0             : 26  ; /* [31..6] */
    } BIT;
} volatile CPU_IMST_REG;

/* Define the union CPU_IRST */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    int_raw_status        : 6   ; /* [5..0] */
        unsigned int    reserved0             : 26  ; /* [31..6] */
    } BIT;
} volatile CPU_IRST_REG;

/* Define the union MBX_ACTIVE */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    mbx_active_status     : 6   ; /* [5..0] */
        unsigned int    reserved0             : 26  ; /* [31..6] */
    } BIT;
} volatile IPC_MBX_ACTIVE_REG;

/* Define the union IPC_LP_STATE */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    lp_state              : 3   ; /* [2..0] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile IPC_LP_STATE_REG;

/* Define the union IPC_VERSION */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    ipc_ver               : 8   ; /* [7..0] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile IPC_VERSION_REG;

/* Define the union IPC_LOCK */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    ipc_lock              : 1   ; /* [0] */
        unsigned int    reserved0             : 31  ; /* [31..1] */
    } BIT;
} volatile IPC_LOCK_REG;

typedef struct {
    MBX_SOURCE_REG       MBX_SOURCE;
    MBX_DSET_REG         MBX_DSET;
    MBX_DCLEAR_REG       MBX_DCLEAR;
    MBX_DSTATUS_REG      MBX_DSTATUS;
    MBX_MODE_REG         MBX_MODE;
    MBX_IMASK_REG        MBX_IMASK;
    MBX_ICLR_REG         MBX_ICLR;
    MBX_SEND_REG         MBX_SEND;
    unsigned int         MBX_DATA[4];
    unsigned int         RESERVED[4];
} volatile IPC_MBX_RegStruct;

typedef struct {
    CPU_IMST_REG         CPU_IMST;
    CPU_IRST_REG         CPU_IRST;
} volatile IPC_CPU_RegStruct;

/* Define the global struct */
typedef struct {
    IPC_MBX_RegStruct       IPC_MBX[6]                       ; /* 0x0~0x140 */
    unsigned int            reserved0[416]                   ; /* 0x180~0x7fc */
    IPC_CPU_RegStruct       IPC_CPU[3]                       ; /* 0x800~0x810 */
    unsigned int            reserved1[58]                    ; /* 0x818~0x8fc */
    IPC_MBX_ACTIVE_REG      IPC_MBX_ACTIVE                   ; /* 0x900 */
    IPC_LP_STATE_REG        IPC_LP_STATE                     ; /* 0x904 */
    unsigned int            reserved2[38]                    ; /* 0x908~0x99c */
    IPC_VERSION_REG         IPC_VERSION                      ; /* 0x9a0 */
    unsigned int            IPC_GP_REG[2]                    ; /* 0x9a4~0x9a8 */
    unsigned int            reserved3[21]                    ; /* 0x9ac~0x9fc */
    IPC_LOCK_REG            IPC_LOCK                         ; /* 0xa00 */
} volatile IPCM_RegStruct;

/**
  * @brief Check mailbox id whether is right.
  * @param mbxId mailbox channel.
  * @retval bool True or false.
  */
static inline bool IsMbxChannelId(IPCM_MbxChId mbxId)
{
    return (mbxId < MBX_CH_MAX && mbxId >= MBX_CH_0);
}

/**
  * @brief Check mailbox channel mode whether is right.
  * @param mbxMode mailbox channel mode.
  * @retval bool True or false.
  */
static inline bool IsMbxChannelMode(IPCM_MbxChMode mbxMode)
{
    return (mbxMode < MBX_MODE_MAX && mbxMode >= MBX_NORMAL_MODE);
}

/**
  * @brief Check mailbox channel status whether is right.
  * @param mbxStatus mailbox channel status.
  * @retval bool True or false.
  */
static inline bool IsMbxChannelStatus(IPCM_MbxChStatus mbxStatus)
{
    return (mbxStatus < MBX_STATUS_MAX && mbxStatus >= MBX_STATUS_FREE);
}

/**
  * @brief Check mailbox channel data index whether is right.
  * @param mbxDataIndex mailbox channel data index.
  * @retval bool True or false.
  */
static inline bool IsMbxChannelDataIndex(IPCM_MbxChDataIndex mbxDataIndex)
{
    return (mbxDataIndex < MBX_DATA_INDEX_MAX && mbxDataIndex >= MBX_DATA_INDEX_0);
}

/**
  * @brief Check mailbox channel interrupt type index whether is right.
  * @param mbxDataIndex mailbox channel interrupt type.
  * @retval bool True or false.
  */
static inline bool IsMbxCallbackType(IPCM_CallbackType type)
{
    return (type < TYPE_CALLBACK_MAX && type >= WRITE_FINISH_TYPE);
}

/**
  * @brief DCL lock IPCM regs.
  * @param ipcm IPCM register base address.
  * @retval None.
  */
static inline void DCL_IPCM_LockRegs(IPCM_RegStruct *ipcm)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    ipcm->IPC_LOCK.reg = IPCM_LOCK_CMD;
}

/**
  * @brief DCL unlock IPCM regs.
  * @param ipcm IPCM register base address.
  * @retval None.
  */
static inline void DCL_IPCM_UnLockRegs(IPCM_RegStruct *ipcm)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    ipcm->IPC_LOCK.reg = IPCM_UNLOCK_CMD;
}

/**
  * @brief DCL Enable IPCM Mailbox Source core ID.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param coreId chip core id.
  * @retval None.
  */
static inline void DCL_IPCM_EnableMbxChannelSrc(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_PARAM_CHECK_NO_RET(IsMbxChannelId(mbxId));     /* param check */
    IPCM_PARAM_CHECK_NO_RET(IsChipCoreId(coreId));
    ipcm->IPC_MBX[mbxId].MBX_SOURCE.BIT.source |= coreId;
}

/**
  * @brief DCL Disable IPCM Mailbox Source core ID.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @retval None.
  */
static inline void DCL_IPCM_DisableMbxChannelSrc(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_PARAM_CHECK_NO_RET(IsMbxChannelId(mbxId));    /* param check */
    ChipCoreId coreId = ipcm->IPC_MBX[mbxId].MBX_SOURCE.BIT.source;
    ipcm->IPC_MBX[mbxId].MBX_SOURCE.BIT.source = coreId;
}

/**
  * @brief DCL Get IPCM Mailbox Source core ID.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @retval unsigned int the source core id.
  */
static inline unsigned int DCL_IPCM_GetMbxChannelSrc(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));     /* param check */
    return ipcm->IPC_MBX[mbxId].MBX_SOURCE.BIT.source;
}

/**
  * @brief DCL Enable IPCM Mailbox Destination core ID.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param coreId chip core id.
  * @retval None.
  */
static inline void DCL_IPCM_EnableMbxChannelDest(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_PARAM_CHECK_NO_RET(IsMbxChannelId(mbxId));
    IPCM_PARAM_CHECK_NO_RET(IsChipCoreId(coreId));     /* param check */
    ipcm->IPC_MBX[mbxId].MBX_DSET.BIT.dset |= coreId;
}

/**
  * @brief DCL Disable IPCM Mailbox Destination core ID.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @retval None.
  */
static inline void DCL_IPCM_DisableMbxChannelDest(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_PARAM_CHECK_NO_RET(IsMbxChannelId(mbxId));    /* param check */
    ChipCoreId coreId = ipcm->IPC_MBX[mbxId].MBX_DSTATUS.BIT.dstatus;
    ipcm->IPC_MBX[mbxId].MBX_DCLEAR.BIT.dclear = coreId;
}

/**
  * @brief DCL Get IPCM Mailbox Destination core ID.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @retval unsigned int the destination core id.
  */
static inline unsigned int DCL_IPCM_GetMbxChannelDest(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));     /* param check */
    return ipcm->IPC_MBX[mbxId].MBX_DSTATUS.BIT.dstatus;
}

/**
  * @brief DCL Set IPCM Mailbox Channel mode.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param coreId chip core id.
  * @retval None.
  */
static inline void DCL_IPCM_SetMbxChannelMode(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, IPCM_MbxChMode mode)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_PARAM_CHECK_NO_RET(IsMbxChannelId(mbxId));    /* param check */
    ipcm->IPC_MBX[mbxId].MBX_MODE.BIT.auto_answer = mode & 0x01;  /* 0x01 : auto_answer bit 0 */
    ipcm->IPC_MBX[mbxId].MBX_MODE.BIT.auto_link = (mode & 0x02) >> 1;  /* 0x02 : auto_link bit 1 */
}

/**
  * @brief DCL Get IPCM Mailbox Channel mode.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @retval IPCM_MbxChMode mailbox channel mode.
  */
static inline IPCM_MbxChMode DCL_IPCM_GetMbxChannelMode(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));    /* param check */
    return (IPCM_MbxChMode)(ipcm->IPC_MBX[mbxId].MBX_MODE.reg & 0x03); /* 0x03 : auto_link | auto_answer */
}

/**
  * @brief DCL Get IPCM Mailbox Channel status.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @retval IPCM_MbxChStatus mailbox channel status.
  */
static inline IPCM_MbxChStatus DCL_IPCM_GetMbxChannelStatus(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));     /* param check */
    return ipcm->IPC_MBX[mbxId].MBX_MODE.BIT.state_status;
}


/**
  * @brief DCL Enable IPCM Mailbox channel interruppt for destination core ID.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param coreId chip core id.
  * @retval None.
  */
static inline void DCL_IPCM_EnableMbxChannelInterrupt(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_PARAM_CHECK_NO_RET(IsMbxChannelId(mbxId));
    IPCM_PARAM_CHECK_NO_RET(IsChipCoreId(coreId));    /* param check */
    ipcm->IPC_MBX[mbxId].MBX_IMASK.reg &= ~coreId;
}

/**
  * @brief DCL Disable IPCM Mailbox channel interruppt for destination core ID.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param coreId chip core id.
  * @retval None.
  */
static inline void DCL_IPCM_DisableMbxChannelInterrupt(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_PARAM_CHECK_NO_RET(IsMbxChannelId(mbxId));
    IPCM_PARAM_CHECK_NO_RET(IsChipCoreId(coreId));     /* param check */
    ipcm->IPC_MBX[mbxId].MBX_IMASK.reg |= coreId;
}

/**
  * @brief DCL Get IPCM Mailbox channel interruppt Enable or disable by coreID.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param coreId chip core id.
  * @retval true means Enable or false means disable.
  */
static inline bool DCL_IPCM_GetMbxChannelCoreIntCfg(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_PARAM_CHECK_WITH_RET(IsMbxChannelId(mbxId), false);
    IPCM_PARAM_CHECK_WITH_RET(IsChipCoreId(coreId), false);     /* param check */
    return ((ipcm->IPC_MBX[mbxId].MBX_IMASK.reg & coreId) != coreId);
}

/**
  * @brief DCL Get IPCM Mailbox Channel interrupt status from which core.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @retval unsigned int mailbox channel interrupt status.
  */
static inline unsigned int DCL_IPCM_GetMbxChannelIntStatus(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));    /* param check */
    return ipcm->IPC_MBX[mbxId].MBX_ICLR.BIT.int_clear;
}

/**
  * @brief DCL Clear IPCM Mailbox Channel interrupt status.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param coreId chip core id.
  * @retval None.
  */
static inline void DCL_IPCM_ClearMbxChannelIntStatus(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));
    IPCM_PARAM_CHECK_NO_RET(IsChipCoreId(coreId));    /* param check */
    ipcm->IPC_MBX[mbxId].MBX_ICLR.BIT.int_clear = coreId;
}

/**
  * @brief DCL IPCM Enable Mailbox Channel source send interrupt to transmit message function.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param coreId chip core id.
  * @retval bool true means send success, false means send fail.
  */
static inline bool DCL_IPCM_SrcStartSend(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));
    IPCM_PARAM_CHECK_WITH_RET(IsChipCoreId(coreId), false);  /* param check */
    ipcm->IPC_MBX[mbxId].MBX_SEND.BIT.send = coreId;
    return ((ipcm->IPC_MBX[mbxId].MBX_SOURCE.reg == coreId) ? true : false);
}

/**
  * @brief DCL IPCM Enable Mailbox Channel destination send interrupt to transmit message function.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param coreId chip core id.
  * @retval bool true means send success, false means send fail.
  */
static inline bool DCL_IPCM_DestStartSend(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));
    IPCM_PARAM_CHECK_WITH_RET(IsChipCoreId(coreId), false);     /* param check */
    ipcm->IPC_MBX[mbxId].MBX_SEND.BIT.send = coreId;
    return ((ipcm->IPC_MBX[mbxId].MBX_DSTATUS.reg == coreId) ? true : false);
}

/**
  * @brief DCL IPCM Get which core send data.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @retval ChipCoreId the core send mailbox interrupt.
  */
static inline ChipCoreId DCL_IPCM_GetCoreIdSendData(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));     /* param check */
    return ipcm->IPC_MBX[mbxId].MBX_SEND.BIT.send;
}

/**
  * @brief DCL IPCM Set Mailbox Channel Data.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param dataIndex IPCM mailbox channel data index.
  * @param dataVal IPCM mailbox channel data.
  * @retval None.
  */
static inline void DCL_IPCM_SetMbxData(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId,
                                       IPCM_MbxChDataIndex dataIndex, unsigned int dataVal)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));     /* param check */
    IPCM_ASSERT_PARAM(IsMbxChannelDataIndex(dataIndex));
    ipcm->IPC_MBX[mbxId].MBX_DATA[dataIndex] = dataVal;
}

/**
  * @brief DCL IPCM Get Mailbox Channel Data by index.
  * @param ipcm IPCM register base address.
  * @param mbxId IPCM mailbox channel.
  * @param dataIndex IPCM mailbox channel data index.
  * @retval unsigned int data IPCM mailbox channel data.
  */
static inline unsigned int DCL_IPCM_GetMbxData(IPCM_RegStruct *ipcm, IPCM_MbxChId mbxId, IPCM_MbxChDataIndex dataIndex)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsMbxChannelId(mbxId));
    IPCM_ASSERT_PARAM(IsMbxChannelDataIndex(dataIndex));     /* param check */
    return ipcm->IPC_MBX[mbxId].MBX_DATA[dataIndex];
}

/**
  * @brief DCL IPCM Get core mask mailbox channel interrupt status.
  * @param ipcm IPCM register base address.
  * @param coreId chip core id.
  * @retval unsigned int core mask interrupt.
  */
static inline unsigned int DCL_IPCM_GetCoreMaskIntStatus(IPCM_RegStruct *ipcm, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsChipCoreId(coreId));     /* param check */
    return ipcm->IPC_CPU[coreId >> 1].CPU_IMST.BIT.int_mask_status;
}

/**
  * @brief DCL IPCM Get core origin mailbox channel interrupt status.
  * @param ipcm IPCM register base address.
  * @param coreId chip core id.
  * @retval unsigned int core origin interrupt.
  */
static inline unsigned int DCL_IPCM_GetCoreOriginIntStatus(IPCM_RegStruct *ipcm, ChipCoreId coreId)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    IPCM_ASSERT_PARAM(IsChipCoreId(coreId));      /* param check */
    return ipcm->IPC_CPU[coreId >> 1].CPU_IRST.BIT.int_raw_status;
}

/**
  * @brief DCL IPCM Get Mailbox Channel active status.
  * @param ipcm IPCM register base address.
  * @retval unsigned int the status of mailbox that is actived.
  */
static inline unsigned int DCL_IPCM_GetMbxChActiveStatus(IPCM_RegStruct *ipcm)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    return ipcm->IPC_MBX_ACTIVE.BIT.mbx_active_status;
}

/**
  * @brief DCL IPCM Get ip design version.
  * @param ipcm IPCM register base address.
  * @retval unsigned int the version of ip.
  */
static inline unsigned int DCL_IPCM_GetVersion(IPCM_RegStruct *ipcm)
{
    IPCM_ASSERT_PARAM(IsIPCMInstance(ipcm));
    return ipcm->IPC_VERSION.reg;
}

#endif /* McuMagicTag_IPCM_IP_H */

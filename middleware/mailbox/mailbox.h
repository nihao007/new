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
  * @file      mainbox.h
  * @author    MCU Driver Team
  * @brief     Mailbox module driver
  * @details   The header file contains the following declaration:
  *             + Mailbox handle structure definition.
  *             + Initialization functions.
  *             + Mailbox Set And Get Functions.
  *             + Interrupt Handler Functions.
  */

#ifndef McuMagicTag_MAILBOX_H
#define McuMagicTag_MAILBOX_H

/* Includes ------------------------------------------------------------------*/
#include "baseinc.h"
#include "ipcm.h"
/**
  * @defgroup Mailbox Mailbox
  * @brief Mailbox module.
  * @{
  */

/**
  * @defgroup MAILBOX_Common Mailbox Common
  * @brief Mailbox common external module.
  * @{
  */

/**
  * @defgroup MAILBOX_MailboxHandle Mailbox Handle Definition
  * @{
  */

/* Typedef definitions -------------------------------------------------------*/
#ifdef MBX_PARAM_CHECK
    #define MBX_ASSERT_PARAM         BASE_FUNC_ASSERT_PARAM
    #define MBX_PARAM_CHECK_NO_RET   BASE_FUNC_PARAMCHECK_NO_RET
    #define MBX_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
    #define MBX_ASSERT_PARAM(para)                ((void)0U)
    #define MBX_PARAM_CHECK_NO_RET(para)          ((void)0U)
    #define MBX_PARAM_CHECK_WITH_RET(param, ret)  ((void)0U)
#endif

/**
  * @}
  */

/**
  * @brief Mailbox Connection relationship Groups definition.
  */
typedef enum {
    CONNECT_GROUP_CORE0_CORE1 = 0,      /**< core0 connect core1. */
    CONNECT_GROUP_CORE0_CORE2 = 1,      /**< core0 connect core2. */
    CONNECT_GROUP_CORE1_CORE2 = 2,      /**< core1 connect core2. */
    CONNECT_GROUP_MAX
} MBX_ConnectGroup;

/**
 * @brief Get read or write mailbox channel information.
 */
typedef enum {
    MODE_WRITE,      /**< Use read mailbox. */
    MODE_READ,       /**< Use write mailbox. */
    MODE_INFO_MAX    /**< Maximum value of the enumeration. */
} MBX_GetInfoMode;

/**
 * @brief Extent handle definition of Mailbox
 */
typedef struct {
    MBX_GetInfoMode getInfoMode;
} MBX_ExtendHandle;

/**
  * @brief Mailbox share memory attribute structure definition.
  */
typedef struct {
    bool                enableConfig;    /**< share memory enable config. */
    unsigned int        startAddr;       /**< share memory start address. */
    unsigned int        lenth;           /**< share memory lenth. */
} MBX_ShmAttr;

/**
  * @brief Mailbox config attribute structure definition.
  */
typedef struct {
    IPCM_MbxChAttr       chCfg;               /**< attribute of mailbox channel config. */
    IPCM_MbxMsgMode      msgMode;             /**< attribute of mailbox message transmit mode. */
    IPCM_CallbackFunc    writeFinishCallback;  /**< attribute of mailbox source core send finish callback. */
    IPCM_CallbackFunc    readFinishCallback;   /**< attribute of mailbox dest core receive finish callback. */
} MBX_ConfigAttr;

/**
  * @brief Mailbox message transmit attribute structure definition.
  */
typedef struct {
    volatile unsigned char      *dataAddr;        /**< message data address. */
    volatile unsigned int        dataRemainSize;  /**< message data remain size to be tansmit. */
    volatile unsigned int        dataIndex;       /**< current message data to be tansmit. */
    volatile IPCM_MbxChStatus    status;          /**< message status. */
} MBX_Msg;

/**
  * @brief Mailbox handle structure definition. one mailbox handle consist of two ipcm channel.
  */
typedef struct {
    IPCM_Handle        *ipcmHandle;           /**< ipcm handle point. */
    IPCM_MbxChId        srcToDestChId;        /**< source transmit message to dest channel. */
    IPCM_MbxChId        destToSrcChId;        /**< dest transmit message to source channel. */
    MBX_ShmAttr         srcToDestShmAttr;     /**< source transmit message to dest channel share memory attribute. */
    MBX_ShmAttr         destToSrcShmAttr;     /**< dest transmit message to source channel share memory attribute. */
    MBX_Msg             srcToDestMsg;         /**< message of source transmit to dest. */
    MBX_Msg             destToSrcMsg;         /**< message of dest transmit to source. */
    MBX_ExtendHandle    handleEx;
} MBX_Handle;

/**
  * @brief Mailbox attribute structure definition.
  */
typedef struct {
    MBX_Handle*          mbxHandles[CONNECT_GROUP_MAX]; /**< groups of mailbox handle point. */
} MBX_Groups;

/**
  * @brief Check whether is maibox connect group.
  * @param connectGroup Value of @ref MBX_ConnectGroup
  * @retval true or false.
  */
static inline bool IsConnectGroup(MBX_ConnectGroup connectGroup)
{
    return (connectGroup == CONNECT_GROUP_CORE0_CORE1 ||
            connectGroup == CONNECT_GROUP_CORE0_CORE2 ||
            connectGroup == CONNECT_GROUP_CORE1_CORE2);
}

/**
  * @defgroup MBX_API_Declaration Mailbox HAL API
  * @{
  */
BASE_StatusType MBX_Init(MBX_Handle* handle, MBX_ConfigAttr srcToDestCfg, MBX_ConfigAttr destToSrcCfg);
BASE_StatusType MBX_DeInit(MBX_Handle* handle);
BASE_StatusType MBX_WriteBlocking(MBX_Handle* handle, unsigned char *wdata, unsigned int size, unsigned int timeoutMs);
BASE_StatusType MBX_WriteIT(MBX_Handle* handle, unsigned char *wdata, unsigned int size);
BASE_StatusType MBX_ReadBlocking(MBX_Handle* handle, unsigned char *rdata, unsigned int size, unsigned int timeoutMs);
BASE_StatusType MBX_ReadIT(MBX_Handle* handle, unsigned char *rdata, unsigned int size);
BASE_StatusType MBX_WriteReadBlocking(MBX_Handle* handle, unsigned char *wdata, unsigned int wsize,
                                      unsigned char *rdata, unsigned int rsize, unsigned int timeoutMs);
BASE_StatusType MBX_WriteReadIT(MBX_Handle* handle, unsigned char *wdata, unsigned int wsize,
                                unsigned char *rdata, unsigned int rsize);
BASE_StatusType MBX_RegisterCallback(MBX_Handle* handle, IPCM_CallbackType type, IPCM_CallbackFunc callBackFunc);
BASE_StatusType MBX_RegisterHandleToGroups(MBX_Groups* mbxGroups, MBX_ConnectGroup connectNum, MBX_Handle* handle);
void MBX_IrqHandler(void* mbxGropus);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* McuMagicTag_MAILBOX_H */
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
  * @file      ipcm.h
  * @author    MCU Driver Team
  * @brief     IPCM module driver
  * @details   The header file contains the following declaration:
  *             + IPCM handle structure definition.
  *             + Initialization functions.
  *             + IPCM Set And Get Functions.
  *             + Interrupt Handler Functions.
  */

#ifndef McuMagicTag_IPCM_H
#define McuMagicTag_IPCM_H

/* Includes ------------------------------------------------------------------*/
#include "ipcm_ip.h"
/**
  * @defgroup IPCM IPCM
  * @brief IPCM module.
  * @{
  */

/**
  * @defgroup IPCM_Common IPCM Common
  * @brief IPCM common external module.
  * @{
  */

/**
  * @defgroup IPCM_Handle_Definition IPCM Handle Definition
  * @{
  */

/* Typedef definitions -------------------------------------------------------*/
typedef void (* IPCM_CallbackFunc)(void *handle);

/**
  * @brief Mailbox attribute structure definition.
  */
typedef enum {
    IPCM_MBX_MODE_BLOCKING,       /**< transmit by blocking cpu. */
    IPCM_MBX_MODE_IT,             /**< transmit by blocking it. */
    IPCM_MBX_MODE_MAX
} IPCM_MbxMsgMode;

/**
  * @brief IPCM mailbox channel attribute structure definition.
  */
typedef struct {
    ChipCoreId          srcCoreId;       /**< source core id. */
    ChipCoreId          destCoreId;      /**< dest core id. */
    IPCM_MbxChMode      mbxChMode;       /**< mailbox channal mode */
} IPCM_MbxChAttr;

/**
  * @brief IPCM handle structure definition.
  */
typedef struct _IPCM_Handle {
    IPCM_RegStruct     *baseAddress;     /**< IPCM base address. */
    IPCM_UserCallBack   userCallBack;    /**< User callback */
    IPCM_ExtendHandle   handleEx;        /**< IPCM extend parameter */
} IPCM_Handle;

/**
  * @}
  */
 
/**
  * @defgroup IPCM_API_Declaration IPCM HAL API
  * @{
  */
BASE_StatusType HAL_IPCM_Init(IPCM_Handle *handle, IPCM_MbxChId mbxChId, IPCM_MbxChAttr chCfg);
BASE_StatusType HAL_IPCM_DeInit(IPCM_Handle *handle, IPCM_MbxChId mbxChId);
void HAL_IPCM_Config(IPCM_Handle *handle, IPCM_MbxChId mbxChId, IPCM_ConfigType type, unsigned int value);
BASE_StatusType HAL_IPCM_NotifyCore(IPCM_Handle *handle, IPCM_MbxChId mbxChId, bool enableConfig);
IPCM_MbxChStatus HAL_IPCM_GetStatus(IPCM_Handle *handle, IPCM_MbxChId mbxChId);
BASE_StatusType HAL_IPCM_SendData(IPCM_Handle *handle, IPCM_MbxChId mbxChId);
BASE_StatusType HAL_IPCM_SetData(IPCM_Handle *handle, IPCM_MbxChId mbxChId,
                                 IPCM_MbxChDataIndex dataIndex, unsigned int dataVal);
unsigned int HAL_IPCM_GetData(IPCM_Handle *handle, IPCM_MbxChId mbxChId, IPCM_MbxChDataIndex dataIndex);
void HAL_IPCM_RegisterCallback(IPCM_Handle *handle, IPCM_MbxChId mbxChId,
                               IPCM_CallbackType type, IPCM_CallbackFunc callBackFunc);
void HAL_IPCM_IrqHandler(void *handle);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* McuMagicTag_IPCM_H */
/**
  * @copyright Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2024-2025. All rights reserved.
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
  * @file      can_ex.h
  * @author    MCU Driver Team
  * @brief     CAN module driver
  * @details   The header file contains the following declaration:
  *             + Setting the Special Function Configuration.
  */

#ifndef McuMagicTag_CAN_EX_H
#define McuMagicTag_CAN_EX_H

/* Includes ------------------------------------------------------------------*/
#include "can.h"
/* Macro definitions ---------------------------------------------------------*/
/**
  * @addtogroup CAN_IP
  * @{
  */

/**
  * @defgroup CAN_EX_API_Declaration CAN HAL API EX
  * @{
  */
BASE_StatusType HAL_CAN_CancelTxRequestEx(CAN_Handle *handle, unsigned int index);
#ifdef CANFD_ENABLE
BASE_StatusType HAL_CAN_ReadFdFrameEx(CAN_Handle *handle, CAN_RxLocation rxLocation, CAN_FdRxMessage *rxHeader);
BASE_StatusType HAL_CAN_WriteFdFrameEx(CAN_Handle *handle, CAN_SendMsgMode sendMode, CAN_FdTxMessage *txHeader);
BASE_StatusType HAL_CAN_ReadTxEventFdFrameEx(CAN_Handle *handle, CAN_FdTxEventHeader *txEvt);
#endif
/**
  * @}
  */

/**
  * @}
  */

#endif /* McuMagicTag_CAN_H */
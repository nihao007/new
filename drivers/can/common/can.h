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
  * @file    can.h
  * @author  MCU Driver Team
  * @brief   CAN module driver.
  * @details This file provides firmware functions to manage the following
  *          functionalities of the CAN.
  *           + Definition of the CAN handle structure.
  *           + Initialization and de-initialization functions.
  *           + Sending and receiving CAN data frames functions.
  *           + Interrupt handler function and user registration callback function.
  */

/* Includes ------------------------------------------------------------------*/
#ifndef McuMagicTag_CAN_H
#define McuMagicTag_CAN_H

#include "can_ip.h"


/**
  * @defgroup CAN CAN
  * @brief CAN module.
  * @{
  */

/**
  * @defgroup CAN_Common CAN Common
  * @brief CAN common external module.
  * @{
  */

/**
  * @defgroup CAN_Handle_Definition CAN Handle Definition
  * @{
  */

/**
  * @brief Definition of the CAN handle structure.
  */
typedef struct _CAN_Handle {
    CAN_RegStruct                  *baseAddress;        /**< CAN registers base address */
    CAN_RamParam                    ramParam;

    CAN_WorkMode                    workMode;           /**< Working mode */
    CAN_FrameFormat                 frameFormat;        /**< Frame format */
    CAN_NominalBitTiming            nomBitTiming;       /**< Nominal bit timing (frame header data rate) */

    CAN_GlobalFilterConfig          globalFilterCfg;    /**< Global filter configuration */
    CAN_AutoRetransmission          autoRetransmission; /**< Auto retransmission configuration */
    CAN_TransmitPause               transmitPause;      /**< Transmission pause configuration */
    CAN_ProtocolException           protocolException;  /**< Protocol exception configuration */
    
    unsigned int                    txEvtFifoWatermark; /**< Transmit event FIFO watermark level */
    unsigned int                    rxFifo0Watermark;   /**< Receive FIFO 0 watermark level */
    unsigned int                    rxFifo1Watermark;   /**< Receive FIFO 1 watermark level */
    CAN_WideMessageMarker           txMesMarker;        /**< Transmit message width marker */
    CAN_StateType                   state;              /**< Transmit status of the CAN */
    CAN_UserCallBack                userCallBack;       /**< User callback function for CAN */
    CAN_ExtendHandle                handleEx;           /**< CAN extended handle */
} CAN_Handle;

/**
 * @brief Callback Function Type Definition.
 */
typedef void (*CAN_CallbackType)(void *handle, unsigned int param);
/**
  * @}
  */

/**
  * @defgroup CAN_API_Declaration CAN HAL API
  * @{
  */
BASE_StatusType HAL_CAN_Init(CAN_Handle *handle);
BASE_StatusType HAL_CAN_DeInit(CAN_Handle *handle);

/* CAN receive and send data functions. */
BASE_StatusType HAL_CAN_Read(CAN_Handle *handle, CAN_RxLocation rxLocation, CAN_Message *rxHeader);
BASE_StatusType HAL_CAN_Write(CAN_Handle *handle, CAN_SendMsgMode sendMode, CAN_Message *txHeader);
BASE_StatusType HAL_CAN_Start(CAN_Handle *handle);
BASE_StatusType HAL_CAN_Stop(CAN_Handle *handle);

/* CAN status acquisition function. */
BASE_StatusType HAL_CAN_GetHighPriorityMsg(CAN_Handle *handle, CAN_HpMsgStatus *hpMsgStatus);
BASE_StatusType HAL_CAN_GetProtocolStatus(CAN_Handle *handle, CAN_ProtocolStatus *status);
BASE_StatusType HAL_CAN_GetErrorCounters(CAN_Handle *handle, CAN_ErrorCounters *counters);
unsigned int HAL_CAN_GetFifoStatus(CAN_Handle *handle, CAN_FifoType fifoType);

/* CAN special function configuration interface. */
BASE_StatusType HAL_CAN_ConfigFilter(CAN_Handle *handle, CAN_IDType frameType, CAN_FilterCfg *filterCfg);
BASE_StatusType HAL_CAN_ConfigTimestamp(CAN_Handle *handle, CAN_TimestampMode mode, unsigned int counterDiv);
unsigned int HAL_CAN_GetTimestampCounter(CAN_Handle *handle);
BASE_StatusType HAL_CAN_ConfigTimeout(CAN_Handle *handle, CAN_TimeoutMode mode, unsigned int counterDiv);
unsigned int HAL_CAN_GetTimeoutCounter(CAN_Handle *handle);
BASE_StatusType HAL_CAN_ConfigTxDelayCompensation(CAN_Handle *handle, unsigned int offset, unsigned int filterWinLen);

/* CAN interrupt service funciton. */
BASE_StatusType HAL_CAN_ConfigInterrupt(CAN_Handle *handle, bool enable, unsigned int mask);
BASE_StatusType HAL_CAN_RegisterCallBack(CAN_Handle *handle, CAN_CallBackFunType typeID,
                                         CAN_CallbackType pCallback);
void HAL_CAN_IrqHandler(void *handle);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif  /* McuMagicTag_CAN_H */
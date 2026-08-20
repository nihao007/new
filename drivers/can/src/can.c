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
  * @file    can.c
  * @author  MCU Driver Team
  * @brief   CAN module driver.
  * @details This file provides firmware functions to manage the following
  *          functionalities of the CAN.
  *           + Initialization and de-initialization functions
  *           + Sending and receiving CAN data frames functions
  *           + Interrupt handling function and user registration callback function
  *           + CAN data frame filtering function
  */

/* Includes ------------------------------------------------------------------*/
#include "can.h"
#include "interrupt.h"

#define GET_INDEX_DATD_SIZE        32
#define GET_INDEX_DS_LOW_32_OFFSET 8
#define GET_INDEX_DS_LOW_32_DIV    4
#define GET_INDEX_DS_UP_32_BASE    5
#define GET_INDEX_DS_UP_32_DIV     16

#define SMID_FILTER_SFT_OFFSET                    30
#define SMID_FILTER_SFT_MASK                      0x03
#define SMID_FILTER_SFEC_OFFSET                   27
#define SMID_FILTER_SFEC_MASK                     0x7
#define SMID_FILTER_SFID1_OFFSET                  16
#define SMID_FILTER_SFID1_MASK                    0x7FF
#define SMID_FILTER_SSYNC_OFFSET                  15
#define SMID_FILTER_SSYNC_MASK                    0x1
#define SMID_FILTER_SFID2_OFFSET                  0
#define SMID_FILTER_SFID2_MASK                    0x7FF

#define EMID_FILTER_EFEC_OFFSET                   29
#define EMID_FILTER_EFEC_MASK                     0x07
#define EMID_FILTER_EFID1_OFFSET                  0x00
#define EMID_FILTER_EFID1_MASK                    0x1FFFFFFF
#define EMID_FILTER_EFT_OFFSET                    31
#define EMID_FILTER_EFT_MASK                      0x01
#define EMID_FILTER_EFID2_OFFSET                  0x00
#define EMID_FILTER_EFID2_MASK                    0x1FFFFFFF

/**
 * @brief Check the every items SRAM parameters for the CAN module are valid.
 * @param ramParam Pointer to the CAN_RamParam structure containing configuration parameters.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
static BASE_StatusType CheckItemsSize(CAN_RamParam *ramParam)
{
    /* Check if any single region exceeds its maximum allowed size */
    if ((ramParam->stdFltNum > SIDFC_RAM_SIZE_MAX) || (ramParam->extFltNum > XIDFC_RAM_SIZE_MAX) || \
        (ramParam->rxFifo0Num > RX_FIFO_0_SIZE_MAX) || (ramParam->rxFifo1Num > RX_FIFO_1_SIZE_MAX) || \
        (ramParam->rxBufNum > RX_BUFFERS_SIZE_MAX) || (ramParam->txEvtFifoNum > TX_EVENT_FIFO_SIZE_MAX) || \
        (ramParam->txBufNum > TX_BUFFERS_SIZE_MAX) || (ramParam->txFifoQNum > TX_BUFFERS_SIZE_MAX) || \
        ((ramParam->txBufNum + ramParam->txFifoQNum > TX_BUFFERS_SIZE_MAX))) {
        return BASE_STATUS_ERROR;  /* Invalid, return error. */
    }
    return BASE_STATUS_OK;
}

/**
 * @brief Check if the SRAM parameters for the CAN module are valid.
 * @param ramParam Pointer to the CAN_RamParam structure containing configuration parameters.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
static BASE_StatusType CheckSramparameters(CAN_RamParam *ramParam)
{
    unsigned int totalRamSize = 0;
    BASE_StatusType ret;
    /* Check if data length exceeds the maximum allowed size of 64 bytes */
    if (ramParam->dataSize >= DATA_SIZE_64_BYTE) {
        return BASE_STATUS_ERROR;
    }

    /* Check if any single region exceeds its maximum allowed size */
    ret = CheckItemsSize(ramParam);
    if (ret != BASE_STATUS_OK) {
        return ret;
    }

    /* Calculate memory used by standard filters */
    totalRamSize += (ramParam->stdFltNum * STANDARD_MESSAGE_ID_FILTER_ELEMENT_SIZE);
    /* Calculate memory used by extended filters */
    totalRamSize += (ramParam->extFltNum * EXTENDED_MESSAGE_ID_FILTER_ELEMENT_SIZE);
    /* Calculate memory used by receive buffers and FIFOs */
    totalRamSize += ((ramParam->rxFifo0Num + ramParam->rxFifo1Num + ramParam->rxBufNum) * \
                    (RX_BUFFER_FIFO_ELEMENT_HEADER_SIZE + ramParam->dataSize));
    /* Calculate memory used by transmit buffers and FIFOs */
    totalRamSize += (ramParam->txBufNum + ramParam->txFifoQNum) * (TX_BUFFER_ELEMENT_HEADER_SIZE + ramParam->dataSize);
    /* Calculate memory used by transmit event FIFOs */
    totalRamSize += (ramParam->txEvtFifoNum * TX_EVENT_FIFO_ELEMENT_SIZE);

    /* Check if total memory usage exceeds the configured total RAM size */
    if (totalRamSize > ramParam->ramTotalSize) {
        return BASE_STATUS_ERROR;
    }

    return BASE_STATUS_OK;
}

/**
 * @brief Convert the data size to an index value.
 * @param dataSize The data size to be converted.
 * @retval The calculated index value.
 */
static unsigned int DataSizeToIndex(unsigned int dataSize)
{
    /* Data length is less than 32 bytes. */
    if (dataSize < GET_INDEX_DATD_SIZE) {
        return (dataSize - GET_INDEX_DS_LOW_32_OFFSET) / GET_INDEX_DS_LOW_32_DIV;
    }
    /* Data length is greater than 32 bytes. */
    return GET_INDEX_DS_UP_32_BASE + (dataSize - GET_INDEX_DATD_SIZE) / GET_INDEX_DS_UP_32_DIV;
}

/**
 * @brief Configure the Rx SRAM address for the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing configuration parameters.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
static void ConfigRxSramAddress(CAN_Handle *handle, unsigned int *startAddr)
{
    /* Configure the address space for receive FIFO 0 */
    *startAddr += handle->ramParam.extFltNum * EXTENDED_MESSAGE_ID_FILTER_ELEMENT_SIZE;
    DCL_CAN_SetFIFO0StartAddress(handle->baseAddress, (*startAddr >> RAM_ADDR_RESERVE_BIT));
    DCL_CAN_SetFIFO0Size(handle->baseAddress, handle->ramParam.rxFifo0Num);
    DCL_CAN_SetFIFO0OperationMode(handle->baseAddress, handle->ramParam.rxFifo0OperationMode);
    DCL_CAN_SetFIFO0Watermark(handle->baseAddress, handle->rxFifo0Watermark);

    /* Configure the address space for receive FIFO 1 */
    *startAddr += handle->ramParam.rxFifo0Num * (RX_BUFFER_FIFO_ELEMENT_HEADER_SIZE + handle->ramParam.dataSize);
    DCL_CAN_SetFIFO1StartAddress(handle->baseAddress, (*startAddr >> RAM_ADDR_RESERVE_BIT));
    DCL_CAN_SetFIFO1Size(handle->baseAddress, handle->ramParam.rxFifo1Num);
    DCL_CAN_SetFIFO1OperationMode(handle->baseAddress, handle->ramParam.rxFifo1OperationMode);
    DCL_CAN_SetFIFO1Watermark(handle->baseAddress, handle->rxFifo1Watermark);

    /* Configure the address space for receive buffers */
    *startAddr += handle->ramParam.rxFifo1Num * (RX_BUFFER_FIFO_ELEMENT_HEADER_SIZE + handle->ramParam.dataSize);
    DCL_CAN_SetRxBufferStartAddress(handle->baseAddress, (*startAddr >> RAM_ADDR_RESERVE_BIT));
}

/**
 * @brief Configure the Tx SRAM address for the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing configuration parameters.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
static void ConfigTxSramAddress(CAN_Handle *handle, unsigned int *startAddr)
{
    /* Configure the address space for transmit event FIFO */
    *startAddr += handle->ramParam.rxBufNum * (RX_BUFFER_FIFO_ELEMENT_HEADER_SIZE + handle->ramParam.dataSize);
    DCL_CAN_SetEventFifoStartAddress(handle->baseAddress, (*startAddr >> RAM_ADDR_RESERVE_BIT));
    DCL_CAN_SetEventFifoSize(handle->baseAddress, handle->ramParam.txEvtFifoNum);
    DCL_CAN_SetEventFifoWatermark(handle->baseAddress, handle->txEvtFifoWatermark);

    /* Configure the address space for transmit buffers and FIFO queue */
    *startAddr += handle->ramParam.txEvtFifoNum * TX_EVENT_FIFO_ELEMENT_SIZE;
    DCL_CAN_SetTxBuffersStartAddress(handle->baseAddress, (*startAddr >> RAM_ADDR_RESERVE_BIT));
    DCL_CAN_SetDedicatedTransmitBuffers(handle->baseAddress, handle->ramParam.txBufNum);
    DCL_CAN_SetTransmitFIFOQueueSize(handle->baseAddress, handle->ramParam.txFifoQNum);
    DCL_CAN_SetTransmitFIFOQueueMode(handle->baseAddress, handle->ramParam.txfqMode);
}

/**
 * @brief Configure the SRAM address for the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing configuration parameters.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
static BASE_StatusType ConfigSramAddress(CAN_Handle *handle)
{
    unsigned int startAddr;
    unsigned int dataFieldSizeCfg;

    /* Validate SRAM parameters before proceeding */
    if (CheckSramparameters(&handle->ramParam) == BASE_STATUS_ERROR) {
        return BASE_STATUS_ERROR;
    }

    /* Set the base address of the SRAM based on the CAN instance */
    unsigned int ramAddrBase = CAN0_RAM_START_ADDR;
    if (handle->baseAddress == CAN1) {
        ramAddrBase = CAN1_RAM_START_ADDR;
    } else if (handle->baseAddress == CAN2) {
        ramAddrBase = CAN2_RAM_START_ADDR;
    }
    unsigned int *ramAddr = (unsigned int *)(void *)(uintptr_t)ramAddrBase;
    unsigned int *ramAddrEnd = (unsigned int *)(void *)(uintptr_t)(ramAddrBase + handle->ramParam.ramTotalSize);

    /* Reset the SRAM memory by clearing all addresses */
    while (ramAddr < ramAddrEnd) {
        *ramAddr = 0x00000000; // Reset SRAM data
        ramAddr++;
    }

    /* Configure the address space for standard filters */
    ramAddrBase = 0;
    if (handle->baseAddress == CAN1) {
        ramAddrBase = 0x1000;
    } else if (handle->baseAddress == CAN2) {
        ramAddrBase = 0x2000;
    }
    startAddr = ramAddrBase;
    DCL_CAN_SidfcRegSetFlssa(handle->baseAddress, (startAddr >> RAM_ADDR_RESERVE_BIT));
    DCL_CAN_SetListSizeStandard(handle->baseAddress, handle->ramParam.stdFltNum);

    /* Configure the address space for extended filters */
    startAddr += handle->ramParam.stdFltNum * STANDARD_MESSAGE_ID_FILTER_ELEMENT_SIZE;
    DCL_CAN_SetFilterListExtendedStartAddress(handle->baseAddress, (startAddr >> RAM_ADDR_RESERVE_BIT));
    DCL_CAN_SetListSizeExtended(handle->baseAddress, handle->ramParam.extFltNum);
    
    /* Configure the address space for Rx and Tx FIFO */
    ConfigRxSramAddress(handle, &startAddr);
    ConfigTxSramAddress(handle, &startAddr);

    /* Configure data field size for all relevant modules */
    dataFieldSizeCfg = DataSizeToIndex(handle->ramParam.dataSize);
    DCL_CAN_SetFIFO0DataFieldSize(handle->baseAddress, dataFieldSizeCfg);
    DCL_CAN_SetFIFO1DataFieldSize(handle->baseAddress, dataFieldSizeCfg);
    DCL_CAN_SetBufferDataFieldSize(handle->baseAddress, dataFieldSizeCfg);
    DCL_CAN_SetTxBufferDataSize(handle->baseAddress, dataFieldSizeCfg);

    return BASE_STATUS_OK;
}

/**
 * @brief Configure the global filter parameters for the CAN module.
 * @param canx Pointer to the CAN_RegStruct structure containing register addresses.
 * @param param Pointer to the CAN_GlobalFilterConfig structure containing global filter configuration parameters.
 */
static void GlobalFilterConfig(CAN_RegStruct *canx, CAN_GlobalFilterConfig *param)
{
    DCL_CAN_SetRejectRemoteFramesExtended(canx, param->rejectRemoteFramesExtendedCfg);
    DCL_CAN_SetRejectRemoteFramesStandard(canx, param->rejectRemoteFramesStandardCfg);
    DCL_CAN_SetAcceptNonMatchingFramesExtended(canx, param->nonMatchExtendedCfg);
    DCL_CAN_SetAcceptNonMatchingFramesStandard(canx, param->nonMatchStandardCfg);
}

/**
 * @brief Find the position of the first set bit (1) in an unsigned integer.
 * @param data The unsigned integer to search for the first set bit.
 * @return The position of the first set bit (0-based index). Returns 0 if no bits are set.
 */
static unsigned int FindFirstOne(unsigned int data)
{
    for (unsigned int i = 0; i < UNSIGNED_INT_BIT_WIDTH; i++) {
        if (data & (0x1 << i)) {
            return i;
        }
    }
    return 0;
}

/**
 * @brief Initialize the CAN module operation mode.
 * @param handle Pointer to the CAN_Handle structure containing initialization parameters.
 * @retval None.
 */
static void ConfigOperationMode(CAN_Handle *handle)
{
    /* Reset all register. */
    DCL_CAN_EnableRestrictedMode(handle->baseAddress, BASE_CFG_UNSET);
    DCL_CAN_EnableBusMonitoringMode(handle->baseAddress, BASE_CFG_UNSET);
    DCL_CAN_EnableTestMode(handle->baseAddress, BASE_CFG_UNSET);
    DCL_CAN_EnableLoopbackMode(handle->baseAddress, BASE_CFG_UNSET);

    if (handle->workMode == WORK_MODE_RESTRICTED_OPERATION) {
        /* Config restricted operation. */
        DCL_CAN_EnableRestrictedMode(handle->baseAddress, BASE_CFG_SET);
    } else if (handle->workMode == WORK_MODE_BUS_MONITORING) {
        /* Config bus monitoring mode. */
        DCL_CAN_EnableBusMonitoringMode(handle->baseAddress, BASE_CFG_SET);
    } else if (handle->workMode == WORK_MODE_INTERNAL_LOOPBACK_TEST) {
        /* Config internal loopback test. */
        DCL_CAN_EnableTestMode(handle->baseAddress, BASE_CFG_SET);
        DCL_CAN_EnableBusMonitoringMode(handle->baseAddress, BASE_CFG_SET);
        DCL_CAN_EnableLoopbackMode(handle->baseAddress, BASE_CFG_SET);
    } else if (handle->workMode == WORK_MODE_EXTERNAL_LOOPBACK_TEST) {
        /* Config external loopback test. */
        DCL_CAN_EnableTestMode(handle->baseAddress, BASE_CFG_SET);
        DCL_CAN_EnableLoopbackMode(handle->baseAddress, BASE_CFG_SET);
    }
}

/**
 * @brief Initialize the CAN module bit timing.
 * @param handle Pointer to the CAN_Handle structure containing initialization parameters.
 * @retval None.
 */
static void ConfigBitTiming(CAN_Handle *handle)
{
    DCL_CAN_SetNominalBitTiming(handle->baseAddress, handle->nomBitTiming);
#ifdef CANFD_ENABLE
    if (handle->frameFormat == CAN_FRAME_CLASSIC) {
        DCL_CAN_EnableFDOperation(handle->baseAddress, BASE_CFG_UNSET);
    } else {
        DCL_CAN_EnableFDOperation(handle->baseAddress, BASE_CFG_SET);
    }
    if (handle->frameFormat == CAN_FRAME_FD_BRS) { // Variable bit rate
        DCL_CAN_SetDataBitTiming(handle->baseAddress, handle->handleEx.dataBitTiming);
        DCL_CAN_EnableBitRateSwitch(handle->baseAddress, BASE_CFG_SET);
    }
#endif
}

/**
 * @brief Config interrupt line parameters.
 * @param handle Pointer to the CAN_Handle structure.
 * @retval None.
 */
static void ConfigInterrupt(CAN_Handle *handle)
{
    /* Bit 0 selects line 0, bit 1 selects line 1. */
    DCL_CAN_InterruptLineSelectSet(handle->baseAddress, handle->handleEx.itLineSelect);
    if (handle->handleEx.itLineEnable == BOTH_LINE_INTERRUPT_ENABLE) {
        DCL_CAN_InterruptLine0Enable(handle->baseAddress); /* Enable interrupt line0 and line1. */
        DCL_CAN_InterruptLine1Enable(handle->baseAddress);
    } else if (handle->handleEx.itLineEnable == LINE1_INTERRUPT_ENABLE) {
        DCL_CAN_InterruptLine1Enable(handle->baseAddress); /* Enable interrupt line1. */
    } else if (handle->handleEx.itLineEnable == LINE0_INTERRUPT_ENABLE) {
        DCL_CAN_InterruptLine0Enable(handle->baseAddress); /* Enable interrupt line0. */
    }
}

/**
 * @brief Initialize the CAN module with the specified configuration parameters.
 * @param handle Pointer to the CAN_Handle structure containing initialization parameters.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_Init(CAN_Handle *handle)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_PARAM_CHECK_WITH_RET(IsWorkMode(handle->workMode), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsFrameFormat(handle->frameFormat), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsRejectRemoteFrames(handle->globalFilterCfg.rejectRemoteFramesExtendedCfg),
                             BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsRejectRemoteFrames(handle->globalFilterCfg.rejectRemoteFramesStandardCfg),
                             BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsAcpNonMatchFrameMode(handle->globalFilterCfg.nonMatchExtendedCfg), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsAcpNonMatchFrameMode(handle->globalFilterCfg.nonMatchStandardCfg), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsAutoRetransmission(handle->autoRetransmission), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsTransmitPause(handle->transmitPause), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsProtocolException(handle->protocolException), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET((handle->txEvtFifoWatermark <= TX_EVENT_FIFO_WATERMARK), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET((handle->rxFifo0Watermark <= RX_FIFO0_WATERMARK), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET((handle->rxFifo1Watermark <= RX_FIFO1_WATERMARK), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsWideMessageMarker(handle->txMesMarker), BASE_STATUS_ERROR);

    BASE_StatusType ret;

    /* Step 0: Exit sleep mode */
    DCL_CAN_EnableClockStopAcknowledge(handle->baseAddress, BASE_CFG_UNSET);
    while (DCL_CAN_GetClockStopAcknowledge(handle->baseAddress) != BASE_CFG_UNSET) {
        ;
    }
    /* Step 1: Configure to enter initialization mode */
    DCL_CAN_EnableInit(handle->baseAddress, BASE_CFG_SET);
    while (DCL_CAN_GetInitialized(handle->baseAddress) != BASE_CFG_SET) {
        ;
    }

    /* Step 2: Configure to remove write protection */
    DCL_CAN_EnableCfgChange(handle->baseAddress, BASE_CFG_SET);
    /* Step 3: Configure SRAM address space */
    ret = ConfigSramAddress(handle);
    if (ret != BASE_STATUS_OK) {
        handle->state = CAN_STATE_ERROR;
        return ret;
    }

    /* Step 4: Configure operating mode */
    ConfigOperationMode(handle);
    /* Step 5: Configure bit timing */
    ConfigBitTiming(handle);
    /* Step 6: Configure global filter */
    GlobalFilterConfig(handle->baseAddress, &handle->globalFilterCfg);
    /* Step 7: Configure automatic retransmission */
    DCL_CAN_DisableAutoRetransmission(handle->baseAddress, handle->autoRetransmission);
    /* Step 8: Configure transmission pause */
    DCL_CAN_EnableTransmitPause(handle->baseAddress, handle->transmitPause);
    /* Step 9: Configure exception handling */
    DCL_CAN_DisableProtocolExceptionHandling(handle->baseAddress, handle->protocolException);
    /* Step 10: Configure message width */
    DCL_CAN_EnableWideMessageMarker(handle->baseAddress, handle->txMesMarker);
    /* Step 11: Config interrupt. */
    ConfigInterrupt(handle);
    handle->state = CAN_STATE_READY;
    return BASE_STATUS_OK;
}

/**
 * @brief Deinitialize the CAN module and reset its configuration.
 * @param handle Pointer to the CAN_Handle structure containing configuration parameters.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_DeInit(CAN_Handle *handle)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));

    /* Stop the CAN module */
    HAL_CAN_Stop(handle);

    /* Disable all interrupts */
    DCL_CAN_InterruptEnableRegSet(handle->baseAddress, BASE_CFG_DISABLE);
    DCL_CAN_InterruptLine0Disable(handle->baseAddress);
    DCL_CAN_InterruptLine1Disable(handle->baseAddress);

    /* Clear all callback functions */
    handle->userCallBack.RxFifo0Callback = NULL;
    handle->userCallBack.RxFifo1Callback = NULL;
    handle->userCallBack.RxBufferCallback = NULL;
    handle->userCallBack.TxFifoEmptyCallback = NULL;
    handle->userCallBack.TxCompleteCallback = NULL;
    handle->userCallBack.TxCanacelCallback = NULL;
    handle->userCallBack.HighPriorityMsgCallback = NULL;
    handle->userCallBack.TxEventFifoCallback = NULL;
    handle->userCallBack.TimestampCallback = NULL;
    handle->userCallBack.TimeoutCallback = NULL;
    handle->userCallBack.ErrorCallback = NULL;

    /* Reset state. */
    handle->state = CAN_STATE_NONE_INIT;

    return BASE_STATUS_OK;
}

/**
 * @brief Config message header first word data.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param targetAddr The address of  tx message header in sram.
 * @param txHeader Pointer to the CAN_TxMessage structure containing message details to be sent.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
static BASE_StatusType ConfigMsgHeaderFirstW(CAN_Handle *handle, unsigned int *targetAddr, CAN_Message *txHeader)
{
    unsigned int txElementWord = 0;

    txElementWord |= (txHeader->idType & TX_ELEMENT_XTD_MASK) << TX_ELEMENT_XTD_OFFSET;
    txElementWord |= (txHeader->frameType & TX_ELEMENT_RTR_MASK) << TX_ELEMENT_RTR_OFFSET;

    if (txHeader->idType == CAN_ID_STANDARD) {
        /* Standard frame id masker offset 18. */
        txElementWord |= (txHeader->identifier << TX_ELEMENT_FIFO_IDS_OFFSET) & TX_ELEMENT_FIFO_IDS_MASK;
    } else if (txHeader->idType == CAN_ID_EXTENDED) {
        /* Extended frame id masker offset 0. */
        txElementWord |= (txHeader->identifier & TX_ELEMENT_FIFO_IDE_MASK) << TX_ELEMENT_FIFO_IDE_OFFSET;
    } else {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }
    *targetAddr = txElementWord; /* Write data to sram. */

    return BASE_STATUS_OK;
}

/**
 * @brief Config message header second word data.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param targetAddr The address of  tx message header in sram.
 * @param txHeader Pointer to the CAN_TxMessage structure containing message details to be sent.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
static BASE_StatusType ConfigMsgHeaderSecW(unsigned int *targetAddr, CAN_Message *txHeader)
{
    unsigned int txElementWord = 0;
    txElementWord |= (txHeader->dataLength & TX_ELEMENT_DCL_TSCE_MASK) << TX_ELEMENT_DCL_TSCE_OFFSET;
    *targetAddr = txElementWord; /* Write data to sram. */
    return BASE_STATUS_OK;
}

/**
 * @brief Copy the message data to the target address.
 * @param targetAddr the address of message in sram.
 * @param txHeader Pointer to the CAN_TxMessage structure containing message details to be sent.
 * @retval None.
 */
static void CopyMessage(unsigned int *targetAddr, CAN_Message *txHeader)
{
    unsigned char *srcAddr = (unsigned char *)(void *)txHeader->data; /* the source data pointer */
    unsigned char *destAddr = (unsigned char *)(void *)targetAddr; /* the target address */
    for (unsigned int i = 0; i < txHeader->dataLength; i++) {
        *destAddr++ = *srcAddr++;
    }
    asm("fence");
}

/**
 * @brief Send a message through the CAN module using the specified transmission mode.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param sendMode The transmission mode: either through a transmission buffer or FIFO queue.
 * @param txHeader Pointer to the CAN_TxMessage structure containing message details to be sent.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_Write(CAN_Handle *handle, CAN_SendMsgMode sendMode, CAN_Message *txHeader)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_PARAM_CHECK_WITH_RET(IsSendMsgMode(sendMode), BASE_STATUS_ERROR);
    CAN_ASSERT_PARAM(txHeader != NULL);
    CAN_PARAM_CHECK_WITH_RET((txHeader->dataLength <= COM_CAN_DLC_BYTES_8), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsFrameType(txHeader->frameType), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsIDType(txHeader->idType), BASE_STATUS_ERROR);

    /* Check if the module is in a busy state */
    if (handle->state != CAN_STATE_BUSY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }
    
    unsigned int regAddr;
    unsigned int index = 0;
    unsigned int *targetAddr;

    /* Calculate the base address for transmission */
    regAddr = (DCL_CAN_GetTxBuffersStartAddress(handle->baseAddress) << RAM_ADDR_RESERVE_BIT) + CAN_COM_RAM_ADDR;

    /* Determine the index based on the transmission mode */
    if (sendMode == CAN_SEND_MSG_BY_TX_BUFFER) {
        unsigned int trbrpReg = DCL_CAN_TxbrpRegGet(handle->baseAddress);
        for (unsigned int i = 0; i < handle->ramParam.txBufNum; i++) {
            if (trbrpReg & (0x1 << i)) {
                continue;
            }
            index = i;
            break;
        }
        if (index >= handle->ramParam.txBufNum) {
            handle->state = CAN_STATE_ERROR;
            return BASE_STATUS_ERROR;
        }
    } else if (sendMode == CAN_SEND_MSG_BY_TX_FIFOQ) {
        if (DCL_CAN_GetTxFifoFull(handle->baseAddress) == BASE_CFG_SET) {
            handle->state = CAN_STATE_ERROR;
            return BASE_STATUS_ERROR;
        }
        index = DCL_CAN_GetTxFifoQueuePutIndex(handle->baseAddress);
    } else {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Calculate the target address for message storage */
    regAddr = regAddr + (index * (TX_BUFFER_ELEMENT_HEADER_SIZE + handle->ramParam.dataSize));
    targetAddr = (unsigned int *)(void *)(uintptr_t)regAddr;

    /* Construct and write the message header */
    ConfigMsgHeaderFirstW(handle, targetAddr, txHeader);
    targetAddr++;
    /* Construct and write the message control word */
    ConfigMsgHeaderSecW(targetAddr, txHeader);
    targetAddr++;
    /* Copy the message data to the target address */
    CopyMessage(targetAddr, txHeader);
    /* Trigger the transmission */
    DCL_CAN_TxbarRegSet(handle->baseAddress, (BASE_CFG_SET << index));
    return BASE_STATUS_OK;
}

/**
 * @brief Start the CAN module to begin message transmission and reception.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_Start(CAN_Handle *handle)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));

    /* Check if the module is in the ready state */
    if (handle->state != CAN_STATE_BUSY && handle->state != CAN_STATE_READY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Enter normal operating mode */
    DCL_CAN_EnableInit(handle->baseAddress, BASE_CFG_UNSET);
    while (DCL_CAN_GetInitialized(handle->baseAddress) != BASE_CFG_UNSET) {
        ;
    }

    /* Update module state to busy */
    handle->state = CAN_STATE_BUSY;
    return BASE_STATUS_OK;
}

/**
 * @brief Stop the CAN module to halt message transmission and reception.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_Stop(CAN_Handle *handle)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));

    /* Check if the module is in the busy state */
    if (handle->state != CAN_STATE_BUSY && handle->state != CAN_STATE_READY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Enter initialization mode */
    DCL_CAN_EnableInit(handle->baseAddress, BASE_CFG_SET);
    while (DCL_CAN_GetInitialized(handle->baseAddress) != BASE_CFG_SET) {
        ;
    }

    /* Update module state to ready */
    handle->state = CAN_STATE_READY;
    return BASE_STATUS_OK;
}

/**
 * @brief Read message header and data.
 * @param targetAddr the address of message in sram.
 * @param rxHeader Pointer to the CAN_RxMessage structure to store the received message details.
 * @retval None.
 */
static void ReadMessageHeaderAndData(unsigned int *targetAddr, CAN_Message *rxHeader)
{
    unsigned int rxElementW;
    
    /* Read and parse the message header */
    rxElementW = *targetAddr;
    rxHeader->idType = (rxElementW >> RX_ELEMENT_XTD_OFFSET) & RX_ELEMENT_XTD_MASK;
    rxHeader->frameType = (rxElementW >> RX_ELEMENT_RTR_OFFSET) & RX_ELEMENT_RTR_MASK;
    if (rxHeader->idType == RX_ELEMENT_XTD_MASK) {
        rxHeader->identifier = (rxElementW >> RX_ELEMENT_EID_OFFSET) & RX_ELEMENT_EID_MASK;
    } else {
        rxHeader->identifier = (rxElementW >> RX_ELEMENT_SID_OFFSET) & RX_ELEMENT_SID_MASK;
    }
    
    targetAddr++;

    /* Read and parse the message header */
    rxElementW = *targetAddr;
    rxHeader->dataLength = (rxElementW >> RX_ELEMENT_DLC_OFFSET) & RX_ELEMENT_DLC_MASK;
    /* The maximum data length of a CAN protocol frame is 8 bytes */
    if (rxHeader->dataLength > CAN_RX_BUFFERS_SIZE_MAX) {
        rxHeader->dataLength = CAN_RX_BUFFERS_SIZE_MAX;
    }
    targetAddr++;

    /* Copy the message data to the provided buffer */
    unsigned char *srcAddr = (unsigned char *)targetAddr;
    unsigned char *destAddr = (unsigned char *)rxHeader->data;
    /* Clear data. */
    for (unsigned int i = 0; i < CAN_RX_BUFFERS_SIZE_MAX; i++) {
        rxHeader->data[i] = 0x0;
    }
    /* Read data form sram. */
    for (unsigned int i = 0; i < rxHeader->dataLength; i++) {
        *destAddr = *srcAddr;
        *srcAddr = 0x00000000; /* Clear src data. */
        srcAddr++;
        destAddr++;
    }
}

/**
 * @brief Clear read index in rx fifo or rx buffer.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param rxLocation The location from which to read the message: RX FIFO0, RX FIFO1, or RX buffer.
 * @param getIndex index of fist word in rx message header.
 * @param getIndexE index of second word in rx message header.
 * @retval None.
 */
static void ClearReadIndex(CAN_Handle *handle, CAN_RxLocation rxLocation, unsigned int getIndex, unsigned int getIndexE)
{
    /* Clear the message reception flag based on the location */
    if (rxLocation == READ_FROM_RX_FIFO0) {
        DCL_CAN_SetFIFO0AcknowledgeIndex(handle->baseAddress, getIndex);
    } else if (rxLocation == READ_FROM_RX_FIFO1) {
        /* Clear Rx fifo1 index data. */
        DCL_CAN_SetFIFO1AcknowledgeIndex(handle->baseAddress, getIndex);
    } else if (rxLocation == READ_FROM_RX_BUFFER) {
        DCL_CAN_Ndat1RegSet(handle->baseAddress, (BASE_CFG_SET << getIndex));
        DCL_CAN_Ndat2RegSet(handle->baseAddress, (BASE_CFG_SET << getIndexE));
    }
}

/**
 * @brief Read a received message from the specified location in the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param rxLocation The location from which to read the message: RX FIFO0, RX FIFO1, or RX buffer.
 * @param rxHeader Pointer to the CAN_RxMessage structure to store the received message details.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_Read(CAN_Handle *handle, CAN_RxLocation rxLocation, CAN_Message *rxHeader)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_PARAM_CHECK_WITH_RET(IsRxLocation(rxLocation), BASE_STATUS_ERROR);
    CAN_ASSERT_PARAM(rxHeader != NULL);

    unsigned int *targetAddr;
    unsigned int regAddr;
    unsigned int getIndex = 0;
    unsigned int getIndexE = 0;

    /* Check if the module is in a busy state */
    if (handle->state != CAN_STATE_BUSY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Determine the message location and retrieve index */
    if (rxLocation == READ_FROM_RX_FIFO0) {
        if (DCL_CAN_GetFIFO0FillLevel(handle->baseAddress) == 0) {
            handle->state = CAN_STATE_ERROR;
            return BASE_STATUS_ERROR;
        }
        /* if read from rx fifo0, get the message location and retrieve index */
        regAddr = (DCL_CAN_GetFIFO0StartAddress(handle->baseAddress) << RAM_ADDR_RESERVE_BIT) + CAN_COM_RAM_ADDR;
        getIndex = DCL_CAN_GetFIFO0GetIndex(handle->baseAddress);
    } else if (rxLocation == READ_FROM_RX_FIFO1) {
        if (DCL_CAN_GetFIFO1FillLevel(handle->baseAddress) == 0) {
            handle->state = CAN_STATE_ERROR;
            return BASE_STATUS_ERROR;
        }
        /* if read from rx fifo1, get the message location and retrieve index */
        regAddr = (DCL_CAN_GetFIFO1StartAddress(handle->baseAddress) << RAM_ADDR_RESERVE_BIT) + CAN_COM_RAM_ADDR;
        getIndex = DCL_CAN_GetFIFO1GetIndex(handle->baseAddress);
    } else if (rxLocation == READ_FROM_RX_BUFFER) {
        if (DCL_CAN_Ndat1RegGet(handle->baseAddress) == 0 || DCL_CAN_Ndat2RegGet(handle->baseAddress) == 0) {
            handle->state = CAN_STATE_ERROR;
            return BASE_STATUS_ERROR;
        }
        /* if read from rx buffer, get the message location and retrieve index */
        regAddr = (DCL_CAN_GetRxBufferStartAddress(handle->baseAddress) << RAM_ADDR_RESERVE_BIT) + CAN_COM_RAM_ADDR;
        getIndex = FindFirstOne(DCL_CAN_Ndat1RegGet(handle->baseAddress));
        getIndexE = FindFirstOne(DCL_CAN_Ndat2RegGet(handle->baseAddress));
    } else {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Calculate the target address for message retrieval */
    targetAddr = (unsigned int *)(void *)(uintptr_t)(regAddr + getIndex * (handle->ramParam.dataSize + \
                 RX_BUFFER_FIFO_ELEMENT_HEADER_SIZE));

    /* Read and parse the message header */
    ReadMessageHeaderAndData(targetAddr, rxHeader);
    /* Clear the message reception flag based on the location */
    ClearReadIndex(handle, rxLocation, getIndex, getIndexE);

    return BASE_STATUS_OK;
}

/**
 * @brief Retrieve the status information of high-priority messages in the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param hpMsgStatus Pointer to the CAN_HpMsgStatus structure to store high-priority message status.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_GetHighPriorityMsg(CAN_Handle *handle, CAN_HpMsgStatus *hpMsgStatus)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_ASSERT_PARAM(hpMsgStatus != NULL);

    // Read high-priority message status from registers
    hpMsgStatus->msgStorage = DCL_CAN_GetMessageStorageIndicator(handle->baseAddress);
    hpMsgStatus->msgIndex = DCL_CAN_GetBufferIndex(handle->baseAddress);
    hpMsgStatus->filterList = DCL_CAN_GetFilterList(handle->baseAddress);
    hpMsgStatus->filterIndex = DCL_CAN_GetFilterIndex(handle->baseAddress);

    return BASE_STATUS_OK;
}

/**
 * @brief Retrieve the protocol status information from the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param status Pointer to the CAN_ProtocolStatus structure to store protocol status information.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_GetProtocolStatus(CAN_Handle *handle, CAN_ProtocolStatus *status)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_ASSERT_PARAM(status != NULL);

    /* Read protocol status from registers */
    status->lastErrorCode = DCL_CAN_GetLastError(handle->baseAddress);
    status->activity = DCL_CAN_GetActivity(handle->baseAddress);
    status->errorPassive = DCL_CAN_GetErrorPassive(handle->baseAddress);
    status->warning = DCL_CAN_GetWarningStatus(handle->baseAddress);
    status->busOff = DCL_CAN_GetBusOffStatus(handle->baseAddress);
    /* Get Data Phase Last Error Code */
    status->dataLastErrorCode = DCL_CAN_GetDataPhaseLastError(handle->baseAddress);
    status->rxESIflag = DCL_CAN_GetEsiFlag(handle->baseAddress);
    status->rxBRSflag = DCL_CAN_GetBrsFlag(handle->baseAddress);
    status->rxFDFflag = DCL_CAN_GetReceivedMessageFlag(handle->baseAddress);
    status->protocolException = DCL_CAN_GetProtocolExceptionEvent(handle->baseAddress);
    /* Get Transmitter Delay Compensation Value */
    status->tdcValue = DCL_CAN_GetTransmitterDelayCompensation(handle->baseAddress);

    return BASE_STATUS_OK;
}

/**
 * @brief Retrieve the error counters information from the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param counters Pointer to the CAN_ErrorCounters structure to store error counters information.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_GetErrorCounters(CAN_Handle *handle, CAN_ErrorCounters *counters)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_ASSERT_PARAM(counters != NULL);

    // Read error counters from registers
    counters->txErrorCnt = DCL_CAN_GetTransmitErrorCounter(handle->baseAddress);
    counters->rxErrorCnt = DCL_CAN_GetReceiveErrorCounter(handle->baseAddress);
    counters->rxErrorPassive = DCL_CAN_GetReceiveErrorPassive(handle->baseAddress);
    counters->errorLogging = DCL_CAN_GetCanErrorLogging(handle->baseAddress);

    return BASE_STATUS_OK;  // Return success status
}

/**
 * @brief Retrieve the status of a specified FIFO in the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param fifoType The type of FIFO to query (e.g., RX_FIFO0_FULL, TX_EFIFO_FREE_LEVEL).
 * @return The status of the specified FIFO.
 */
unsigned int HAL_CAN_GetFifoStatus(CAN_Handle *handle, CAN_FifoType fifoType)
{
    CAN_ASSERT_PARAM(handle != NULL);  // Check if handle is not null
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));  // Verify CAN instance

    unsigned int ret = 0;
    switch (fifoType) {
        case RX_FIFO0_FULL :
            /* Get rx fifo0 full status */
            ret = DCL_CAN_GetFIFO0Full(handle->baseAddress);
            break;
        case RX_FIFO0_FILL_LEVEL :
            /* Get rx fifo0 fill level status */
            ret = DCL_CAN_GetFIFO0FillLevel(handle->baseAddress);
            break;
        case RX_FIFO1_FULL :
            /* Get rx fifo1 full status */
            ret = DCL_CAN_GetFIFO1Full(handle->baseAddress);
            break;
        case RX_FIFO1_FILL_LEVEL :
            /* Get rx fifo1 fill level status */
            ret = DCL_CAN_GetFIFO1FillLevel(handle->baseAddress);
            break;
        case TX_FIFOQ_FULL :
            /* Get tx fifoQ full status */
            ret = DCL_CAN_GetTxFifoFull(handle->baseAddress);
            break;
        case TX_FIFOQ_FREE_LEVEL :
            /* Get tx fifoQ free level status */
            ret = DCL_CAN_GetTxFifoFreeLevel(handle->baseAddress);
            break;
        case TX_EFIFO_FULL :
            /* Get tx event fifoQ full status */
            ret = DCL_CAN_GetEventFifoFull(handle->baseAddress);
            break;
        case TX_EFIFO_FREE_LEVEL :
            /* Get tx event fifoQ fill level status */
            ret = DCL_CAN_GetEventFifoFillLevel(handle->baseAddress);
            break;
        default:
            break;
    }
    return ret;
}

/**
 * @brief Configure the message filters in the CAN module based on the specified frame type and filter configuration.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param frameType The type of CAN frame to configure (standard or extended).
 * @param filterCfg Pointer to the CAN_FilterCfg structure containing filter configuration parameters.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_ConfigFilter(CAN_Handle *handle, CAN_IDType frameType, CAN_FilterCfg *filterCfg)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_PARAM_CHECK_WITH_RET(IsIDType(frameType), BASE_STATUS_ERROR);
    CAN_ASSERT_PARAM(filterCfg != NULL);

    /* Check if the module is in the ready state */
    if (handle->state != CAN_STATE_BUSY && handle->state != CAN_STATE_READY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    unsigned int filterElementW1 = 0;  // Initialize filter element word 1
    unsigned int filterElementW2 = 0;  // Initialize filter element word 2
    unsigned int idFilterAddr;  // Address of the filter element in RAM
    unsigned int offset;  // Offset within the filter element
    unsigned int *targetAddr;  // Pointer to the target address in RAM

    /* Configure standard message ID filter */
    if (frameType == CAN_ID_STANDARD) {
        filterElementW1 |= (filterCfg->filterType & SMID_FILTER_SFT_MASK) << SMID_FILTER_SFT_OFFSET;
        filterElementW1 |= (filterCfg->filterConfig & SMID_FILTER_SFEC_MASK) << SMID_FILTER_SFEC_OFFSET;
        filterElementW1 |= (filterCfg->filterID1 & SMID_FILTER_SFID1_MASK) << SMID_FILTER_SFID1_OFFSET;
        filterElementW1 |= (filterCfg->filterID2 & SMID_FILTER_SFID2_MASK) << SMID_FILTER_SFID2_OFFSET;

        /* Calculate the address and offset for the standard message ID filter */
        idFilterAddr = (DCL_CAN_SidfcRegGetFlssa(handle->baseAddress) << RAM_ADDR_RESERVE_BIT) + CAN_COM_RAM_ADDR;
        offset = filterCfg->index * STANDARD_MESSAGE_ID_FILTER_ELEMENT_SIZE;
        targetAddr = (unsigned int *)(void *)(uintptr_t)(idFilterAddr + offset);
        *targetAddr = filterElementW1;
    } else if (frameType == CAN_ID_EXTENDED) {
        /* Configure extended message ID filter */
        filterElementW1 |= (filterCfg->filterConfig & EMID_FILTER_EFEC_MASK) << EMID_FILTER_EFEC_OFFSET;
        filterElementW1 |= (filterCfg->filterID1 & EMID_FILTER_EFID1_MASK) << EMID_FILTER_EFID1_OFFSET;
        filterElementW2 |= (filterCfg->filterType & EMID_FILTER_EFT_MASK) << EMID_FILTER_EFT_OFFSET;
        filterElementW2 |= (filterCfg->filterID2 & EMID_FILTER_EFID2_MASK) << EMID_FILTER_EFID2_OFFSET;

        /* Calculate the address and offset for the extended message ID filter */
        idFilterAddr = (DCL_CAN_GetFilterListExtendedStartAddress(handle->baseAddress) << RAM_ADDR_RESERVE_BIT) +
                       CAN_COM_RAM_ADDR;
        offset = filterCfg->index * EXTENDED_MESSAGE_ID_FILTER_ELEMENT_SIZE;
        targetAddr = (unsigned int *)(void *)(uintptr_t)(idFilterAddr + offset);
        *targetAddr = filterElementW1;
        targetAddr++;
        *targetAddr = filterElementW2;
    } else {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;  // Invalid frame type
    }

    return BASE_STATUS_OK;  // Return success status
}

/**
 * @brief Configure the timestamp mode and counter divider for the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param mode The timestamp mode to configure (e.g., free-running or event-based).
 * @param counterDiv The divider value for the timestamp counter.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_ConfigTimestamp(CAN_Handle *handle, CAN_TimestampMode mode, unsigned int counterDiv)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_PARAM_CHECK_WITH_RET(IsTimestampMode(mode), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET((counterDiv <= TSCC_BIT_TCP_MASK), BASE_STATUS_ERROR);

    /* Check if the module is in the ready state */
    if (handle->state != CAN_STATE_BUSY && handle->state != CAN_STATE_READY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Enable configuration access */
    DCL_CAN_EnableCfgChange(handle->baseAddress, BASE_CFG_SET);

    /* Configure timestamp mode and counter divider */
    DCL_CAN_SetTimestampSelect(handle->baseAddress, mode);
    DCL_CAN_SetTimestampPrescaler(handle->baseAddress, counterDiv);

    /* Disable configuration access */
    DCL_CAN_EnableCfgChange(handle->baseAddress, BASE_CFG_UNSET);

    return BASE_STATUS_OK;  // Return success status
}

/**
 * @brief Get the current value of the timestamp counter from the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @return The current value of the timestamp counter.
 */
unsigned int HAL_CAN_GetTimestampCounter(CAN_Handle *handle)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));

    /* Read the timestamp counter value */
    return DCL_CAN_GetTimestampCounter(handle->baseAddress);
}

/**
 * @brief Configure the timeout mode and counter divider for the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param mode The timeout mode to configure (e.g., fixed or variable).
 * @param counterDiv The divider value for the timeout counter.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_ConfigTimeout(CAN_Handle *handle, CAN_TimeoutMode mode, unsigned int counterDiv)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_PARAM_CHECK_WITH_RET(IsTimeoutMode(mode), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET((counterDiv <= 0xFFFF), BASE_STATUS_ERROR);

    /* Check if the module is in the ready state */
    if (handle->state != CAN_STATE_READY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Enable configuration access */
    DCL_CAN_EnableCfgChange(handle->baseAddress, BASE_CFG_SET);

    /* Configure timeout mode, counter divider, and enable timeout */
    DCL_CAN_SetTimeoutSelect(handle->baseAddress, mode);
    DCL_CAN_SetTimeoutPeriod(handle->baseAddress, counterDiv);
    DCL_CAN_SetEnableTimeoutCounter(handle->baseAddress, BASE_CFG_ENABLE);

    /* Disable configuration access */
    DCL_CAN_EnableCfgChange(handle->baseAddress, BASE_CFG_UNSET);

    return BASE_STATUS_OK;  // Return success status
}

/**
 * @brief Get the current value of the timeout counter from the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @return The current value of the timeout counter.
 */
unsigned int HAL_CAN_GetTimeoutCounter(CAN_Handle *handle)
{
    CAN_ASSERT_PARAM(handle != NULL);  // Check if handle is not null
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));  // Verify CAN instance

    /* Read the timeout counter value */
    return DCL_CAN_GetTimeoutCounter(handle->baseAddress);
}

/**
 * @brief Configure the transmission delay compensation parameters for the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param offset The delay compensation offset value.
 * @param filterWinLen The filter window length for delay compensation.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_ConfigTxDelayCompensation(CAN_Handle *handle, unsigned int offset, unsigned int filterWinLen)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_PARAM_CHECK_WITH_RET((offset <= 0x7F), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET((filterWinLen <= 0x7F), BASE_STATUS_ERROR);

    /* Check if the module is in the ready state */
    if (handle->state != CAN_STATE_BUSY && handle->state != CAN_STATE_READY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Enable configuration access */
    DCL_CAN_EnableCfgChange(handle->baseAddress, BASE_CFG_SET);

    /* Configure delay compensation offset and filter window length */
    DCL_CAN_SetTxDelayCompensationSSPOffset(handle->baseAddress, offset);
    DCL_CAN_SetTxdelatCompensationFilterWinLength(handle->baseAddress, filterWinLen);
    DCL_CAN_EnableTransmitterDelayCompensation(handle->baseAddress);
    /* Disable configuration access */
    DCL_CAN_EnableCfgChange(handle->baseAddress, BASE_CFG_UNSET);

    return BASE_STATUS_OK;  // Return success status
}

static BASE_StatusType RegisterRxCallback(CAN_Handle *handle, CAN_CallBackFunType typeID,
                                          CAN_CallbackType pCallback)
{
    if (typeID == CAN_RX_FIFO_0) {
        /* Register the rx fifo0 callback function based on the event type */
        handle->userCallBack.RxFifo0Callback = pCallback;
    } else if (typeID == CAN_RX_FIFO_1) {
        /* Register the rx fifo1 callback function based on the event type */
        handle->userCallBack.RxFifo1Callback = pCallback;
    } else if (typeID == CAN_RX_BUFFER)  {
        /* Register the rx buffer callback function based on the event type */
        handle->userCallBack.RxBufferCallback = pCallback;
    } else {
        return BASE_STATUS_ERROR;
    }
    return BASE_STATUS_OK;
}

static BASE_StatusType RegisterTxCallback(CAN_Handle *handle, CAN_CallBackFunType typeID,
                                          CAN_CallbackType pCallback)
{
    if (typeID == CAN_TX_FIFO_EMPTY) {
        /* Register the tx fifo empty callback function based on the event type */
        handle->userCallBack.TxFifoEmptyCallback = pCallback;
    } else if (typeID == CAN_TX_COMPLETE) {
        /* Register the tx complete callback function based on the event type */
        handle->userCallBack.TxCompleteCallback = pCallback;
    } else if (typeID == CAN_TX_CANCEL) {
        /* Register the tx canacel callback function based on the event type */
        handle->userCallBack.TxCanacelCallback = pCallback;
    }  else {
        return BASE_STATUS_ERROR;
    }
    return BASE_STATUS_OK;
}

static BASE_StatusType RegisterSpecialFuncCallback(CAN_Handle *handle, CAN_CallBackFunType typeID,
                                                   CAN_CallbackType pCallback)
{
    if (typeID == CAN_HIGH_PRIORITY_MSG) {
        /* Register the high priority callback function based on the event type */
        handle->userCallBack.HighPriorityMsgCallback = pCallback;
    } else if (typeID == CAN_TX_EVENT_FIFO) {
        /* Register the tx event fifo callback function based on the event type */
        handle->userCallBack.TxEventFifoCallback = pCallback;
    } else if (typeID == CAN_TIMESTAMP) {
        /* Register the timestamp callback function based on the event type */
        handle->userCallBack.TimestampCallback = pCallback;
    }  else if (typeID == CAN_TIMEOUT) {
        /* Register the error callback function based on the event type */
        handle->userCallBack.TimeoutCallback = pCallback;
    }  else if (typeID == CAN_ERROR) {
        /* Register the error callback function based on the event type */
        handle->userCallBack.ErrorCallback = pCallback;
    }  else {
        return BASE_STATUS_ERROR;
    }
    return BASE_STATUS_OK;
}

/**
 * @brief Configure CAN module interrupt enable mask.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param enable True or false corresponding bit.
 * @param mask Interrupt bit mask.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_ConfigInterrupt(CAN_Handle *handle, bool enable, unsigned int mask)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));

    /* Check if the module is in a busy state */
    if (handle->state != CAN_STATE_BUSY && handle->state != CAN_STATE_READY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }
    unsigned int enableReg = DCL_CAN_InterruptEnableRegGet(handle->baseAddress);
    if (enable) {
        enableReg |= mask; /* Enable Control */
    } else {
        enableReg &= ~mask; /* Disable Control */
    }
    
    DCL_CAN_InterruptEnableRegSet(handle->baseAddress, enableReg); /* Enable all interrupt. */
    return BASE_STATUS_OK;
}

/**
 * @brief Register a callback function for specific CAN events.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param typeID The type of event to register the callback for.
 * @param pCallback Pointer to the callback function to be registered.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_RegisterCallBack(CAN_Handle *handle, CAN_CallBackFunType typeID,
                                         CAN_CallbackType pCallback)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));

    BASE_StatusType ret = BASE_STATUS_ERROR;
    /* Register the Rx callback function based on the event type */
    ret = RegisterRxCallback(handle, typeID, pCallback);
    if (ret == BASE_STATUS_OK) {
        return ret;
    }
    /* Register the Tx callback function based on the event type */
    ret = RegisterTxCallback(handle, typeID, pCallback);
    if (ret == BASE_STATUS_OK) {
        return ret;
    }
    /* Register the Special func callback function based on the event type */
    ret = RegisterSpecialFuncCallback(handle, typeID, pCallback);
    return ret;
}

static void RxMessageHandler(CAN_Handle *handle, unsigned int irReg)
{
    unsigned int mask = 0;
    mask = irReg & RX_FIFO0_IT_MASK;
    if (mask && (handle->userCallBack.RxFifo0Callback != NULL)) {
        handle->userCallBack.RxFifo0Callback(handle, mask);  /* Handle RX FIFO0 interrupt */
    }

    mask = irReg & RX_FIFO1_IT_MASK;
    if (mask && (handle->userCallBack.RxFifo1Callback != NULL)) {
        handle->userCallBack.RxFifo1Callback(handle, mask);  /* Handle RX FIFO1 interrupt */
    }

    mask = irReg & RX_BUFFER_IT_MASK;
    if (mask && (handle->userCallBack.RxBufferCallback != NULL)) {
        handle->userCallBack.RxBufferCallback(handle, mask);  /* Handle RX buffer interrupt */
    }
}

static void TxMessageHandler(CAN_Handle *handle, unsigned int irReg)
{
    unsigned int mask = 0;
    mask = irReg & TX_FIFO_EMPTY_IT_MASK;
    if (mask && (handle->userCallBack.TxFifoEmptyCallback != NULL)) {
        handle->userCallBack.TxFifoEmptyCallback(handle, mask);  /* Handle TX FIFO empty interrupt */
    }

    mask = irReg & TX_COMPLETE_IT_MASK;
    if (mask && (handle->userCallBack.TxCompleteCallback != NULL)) {
        unsigned int indexes = DCL_CAN_TxbtoRegGet(handle->baseAddress);  /* Get completed transmission indexes */
        indexes &= DCL_CAN_TxbtieRegGet(handle->baseAddress);  /* Mask with enabled indexes */
        handle->userCallBack.TxCompleteCallback(handle, indexes);  /* Handle TX complete interrupt */
    }

    mask = irReg & TX_CANCEL_IT_MASK;
    if (mask && (handle->userCallBack.TxCanacelCallback != NULL)) {
        unsigned int indexes = DCL_CAN_TxbcfRegGet(handle->baseAddress);  /* Get canceled transmission indexes */
        indexes &= DCL_CAN_TxbcieRegGet(handle->baseAddress);  /* Mask with enabled indexes */
        handle->userCallBack.TxCanacelCallback(handle, indexes);  /* Handle TX cancel interrupt */
    }

    mask = irReg & TX_EVENT_FIFO_IT_MASK;
    if (mask && (handle->userCallBack.TxEventFifoCallback != NULL)) {
        handle->userCallBack.TxEventFifoCallback(handle, mask);  /* Handle TX event FIFO interrupt */
    }
}

/**
 * @brief Interrupt handler for the CAN module to process various interrupt events.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 */
void HAL_CAN_IrqHandler(void *handle)
{
    CAN_ASSERT_PARAM(handle != NULL);  /* Check if handle is not null */
    CAN_Handle *canHandle = (CAN_Handle *)handle;
    CAN_ASSERT_PARAM(IsCANInstance(canHandle->baseAddress));  /* Verify CAN instance */

    /* Check if the module is in the ready state */
    if (canHandle->state == CAN_STATE_ERROR) {
        return;
    }

    unsigned int mask = 0;
    unsigned int irReg = DCL_CAN_InterruptStateGet(canHandle->baseAddress);  /* Read the interrupt status register */
    DCL_CAN_InterruptStateClear(canHandle->baseAddress, irReg);  /* Clear the interrupt flags */
    /* Read the interrupt enable register */
    unsigned int ieReg = DCL_CAN_InterruptEnableRegGet(canHandle->baseAddress);
    irReg &= ieReg;  /* Mask the interrupt status with the enable register to get active interrupts */

    /* Process each interrupt type */
    mask = irReg & HIGH_PRIORITY_IT_MASK;
    if (mask && (canHandle->userCallBack.HighPriorityMsgCallback != NULL)) {
        canHandle->userCallBack.HighPriorityMsgCallback(canHandle, mask);  /* Handle high-priority message interrupt */
    }

    RxMessageHandler(canHandle, irReg);
    TxMessageHandler(canHandle, irReg);

    mask = irReg & TIMESTAMP_IT_MASK;
    if (mask && (canHandle->userCallBack.TimestampCallback != NULL)) {
        canHandle->userCallBack.TimestampCallback(canHandle, mask);  /* Handle timestamp interrupt */
    }

    mask = irReg & TIMEOUT_IT_MASK;
    if (mask && (canHandle->userCallBack.TimeoutCallback != NULL)) {
        canHandle->userCallBack.TimeoutCallback(canHandle, mask);  /* Handle timeout interrupt */
    }

    mask = irReg & ERROR_IT_MASK;
    if (mask && (canHandle->userCallBack.ErrorCallback != NULL)) {
        canHandle->userCallBack.ErrorCallback(canHandle, mask);  /* Handle error interrupt */
    }
}
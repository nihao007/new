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
  * @file      can_ex.c
  * @author    MCU Driver Team
  * @brief     CAN module driver
  * @details   The header file contains the following declaration:
  *             + Setting the Special Function Configuration.
  */

/* Includes ------------------------------------------------------------------*/
#include "can_ex.h"

/* Macro definitions ---------------------------------------------------------*/

/**
 * @brief Cancel a transmission request in the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param index Index of the transmission request to cancel, @arg CAN_CancelTxRequestIndices.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_CancelTxRequestEx(CAN_Handle *handle, unsigned int index)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    
    /* Check if the module is in a busy state */
    if (handle->state != CAN_STATE_BUSY && handle->state != CAN_STATE_READY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Cancel the transmission request at the specified index */
    DCL_CAN_TxbcrRegSet(handle->baseAddress, index);
    return BASE_STATUS_OK;
}

#ifdef CANFD_ENABLE

/**
 * @brief Config message header first word data.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param targetAddr The address of  tx message header in sram.
 * @param txHeader Pointer to the CAN_TxMessage structure containing message details to be sent.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
static BASE_StatusType ConfigMsgHeaderFirstW(CAN_Handle *handle, unsigned int *targetAddr, CAN_FdTxMessage *txHeader)
{
    unsigned int txElementWord = 0;
 
    txElementWord |= (txHeader->idType & TX_ELEMENT_XTD_MASK) << TX_ELEMENT_XTD_OFFSET;
    txElementWord |= (txHeader->txFrameType & TX_ELEMENT_RTR_MASK) << TX_ELEMENT_RTR_OFFSET;
 
    if (txHeader->idType == CAN_ID_STANDARD) {
        /* Standard frame id masker offset 18. */
        txElementWord |= (txHeader->identifier << TX_ELEMENT_FIFO_IDS_OFFSET) & TX_ELEMENT_FIFO_IDS_MASK;
    } else if (txHeader->idType == CAN_ID_EXTENDED) {
        /* Extended frame id masker offset 0 */
        txElementWord |= (txHeader->identifier & TX_ELEMENT_FIFO_IDE_MASK) << TX_ELEMENT_FIFO_IDE_OFFSET;
    } else {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }
    *targetAddr = txElementWord; /* Write data to sram. */
 
    return BASE_STATUS_OK;
}

/**
 * @brief Convert CANFD data length enum to dlc bit representation.
 * @param dataLength The CANFD data length enum value.
 * @retval The dlc bit representation of the data length.
 *         Returns 0-8 for values 0-8, and 0x9-0xf for values 12,16,20,24,32,48,64 respectively.
 */
static unsigned int LengthToBit(CAN_DlcBytes dataLength)
{
    /* for data lengths 0-8, return the enum value directly */
    if (dataLength < COM_CAN_DLC_BYTES_12) {
        return (unsigned int)dataLength;
    }
    /* for data lengths 12,16,20,24,32,48,64, return corresponding bit values */
    switch (dataLength) {
        case COM_CAN_DLC_BYTES_12:
            return 0x9;  /* representation for 12 bytes */
        case COM_CAN_DLC_BYTES_16:
            return 0xa;  /* representation for 16 bytes */
        case COM_CAN_DLC_BYTES_20:
            return 0xb;  /* representation for 20 bytes */
        case COM_CAN_DLC_BYTES_24:
            return 0xc;  /* representation for 24 bytes */
        case COM_CAN_DLC_BYTES_32:
            return 0xd;  /* representation for 32 bytes */
        case COM_CAN_DLC_BYTES_48:
            return 0xe;  /* representation for 48 bytes */
        case COM_CAN_DLC_BYTES_64:
            return 0xf;  /* representation for 64 bytes */
        default:
            return 0;
    }
}

/**
 * @brief Convert CANFD dlc bit representation to data length enum.
 * @param dataDlc The dlc bit representation of the data length.
 * @retval The CANFD data length enum value.
 *         Returns 0-8 for values 0-8, and COM_CAN_DLC_BYTES_12-64 for values 0x9-0xf respectively.
 */
static CAN_DlcBytes BitToLength(unsigned int dataDlc)
{
    /* for bit values 0-8, return the enum value directly */
    if (dataDlc <= COM_CAN_DLC_BYTES_8) {
        return (CAN_DlcBytes)dataDlc;
    }
    /* for bit values 0x9-0xf, return corresponding enum values */
    switch (dataDlc) {
        case 0x9:
            return COM_CAN_DLC_BYTES_12;  /* convert bit 0x9 to 12 bytes */
        case 0xa:
            return COM_CAN_DLC_BYTES_16;  /* convert bit 0xa to 16 bytes */
        case 0xb:
            return COM_CAN_DLC_BYTES_20;  /* convert bit 0xb to 20 bytes */
        case 0xc:
            return COM_CAN_DLC_BYTES_24;  /* convert bit 0xc to 24 bytes */
        case 0xd:
            return COM_CAN_DLC_BYTES_32;  /* convert bit 0xd to 32 bytes */
        case 0xe:
            return COM_CAN_DLC_BYTES_48;  /* convert bit 0xe to 48 bytes */
        case 0xf:
            return COM_CAN_DLC_BYTES_64;  /* convert bit 0xf to 64 bytes */
        default:
            return COM_CAN_DLC_BYTES_0;
    }
}

/**
 * @brief Config message header second word data.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param targetAddr The address of  tx message header in sram.
 * @param txHeader Pointer to the CAN_FdMessage structure containing message details to be sent.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
static BASE_StatusType ConfigMsgHeaderSecW(CAN_Handle *handle, unsigned int *targetAddr, CAN_FdTxMessage *txHeader)
{
    unsigned int txElementWord = 0;
 
    txElementWord |= (txHeader->msgMarker & TX_ELEMENT_FIFO_MML_MASK) << TX_ELEMENT_FIFO_MML_OFFSET;
    if (DCL_CAN_GetWideMessageMarker(handle->baseAddress) == BASE_CFG_SET) {
        /* If set wide message marker, do this things. */
        txElementWord |= (txHeader->msgMarker & TX_ELEMENT_FIFO_MMH_MASK);
    }
 
    txElementWord |= (txHeader->efcEn & TX_ELEMENT_FIFO_EFC_MASK) << TX_ELEMENT_FIFO_EFC_OFFSET;
    txElementWord |= (txHeader->tscEn & TX_ELEMENT_FIFO_TSCE_MASK) << TX_ELEMENT_FIFO_TSCE_OFFSET;
    txElementWord |= (txHeader->format & TX_ELEMENT_FDF_TSCE_MASK) << TX_ELEMENT_FDF_TSCE_OFFSET;
    txElementWord |= (txHeader->brsState & TX_ELEMENT_BRS_TSCE_MASK) << TX_ELEMENT_BRS_TSCE_OFFSET;
    txElementWord |= (LengthToBit(txHeader->dataLength) & TX_ELEMENT_DCL_TSCE_MASK) << TX_ELEMENT_DCL_TSCE_OFFSET;
 
    *targetAddr = txElementWord; /* Write data to sram. */
 
    return BASE_STATUS_OK;
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
 * @brief Read message header and data.
 * @param targetAddr the address of message in sram.
 * @param rxHeader Pointer to the CAN_RxMessage structure to store the received message details.
 * @retval None.
 */
static void ReadMessageHeaderAndData(unsigned int *targetAddr, CAN_FdRxMessage *rxHeader)
{
    unsigned int rxElementW;
    
    /* Read and parse the message header */
    rxElementW = *targetAddr;
    rxHeader->idType = (rxElementW >> RX_ELEMENT_XTD_OFFSET) & RX_ELEMENT_XTD_MASK;
    rxHeader->rxFrameType = (rxElementW >> RX_ELEMENT_RTR_OFFSET) & RX_ELEMENT_RTR_MASK;
    if (rxHeader->idType == RX_ELEMENT_XTD_MASK) {
        rxHeader->identifier = (rxElementW >> RX_ELEMENT_EID_OFFSET) & RX_ELEMENT_EID_MASK;
    } else {
        rxHeader->identifier = (rxElementW >> RX_ELEMENT_SID_OFFSET) & RX_ELEMENT_SID_MASK;
    }
    targetAddr++;
 
    /* Read and parse the message header */
    rxElementW = *targetAddr;
    rxHeader->rxFliterMatch = (rxElementW >> RX_ELEMENT_ANMF_OFFSET) & RX_ELEMENT_ANMF_MASK;
    rxHeader->rxFliterIndex = (rxElementW >> RX_ELEMENT_FIDX_OFFSET) & RX_ELEMENT_FIDX_MASK;
    rxHeader->format = (rxElementW >> RX_ELEMENT_FDF_OFFSET) & RX_ELEMENT_FDF_MASK;
    rxHeader->brsState = (rxElementW >> RX_ELEMENT_BRS_OFFSET) & RX_ELEMENT_BRS_MASK;
    rxHeader->dataLength = BitToLength((rxElementW >> RX_ELEMENT_DLC_OFFSET) & RX_ELEMENT_DLC_MASK);
    rxHeader->rxTimeStamp = (rxElementW >> RX_ELEMENT_RXTS_OFFSET) & RX_ELEMENT_RXTS_MASK;
    targetAddr++;
 
    /* Copy the message data to the provided buffer */
    unsigned char *srcAddr = (unsigned char *)targetAddr;
    unsigned char *destAddr = (unsigned char *)rxHeader->data;
    /* Clear data. */
    for (unsigned int i = 0; i < RX_BUFFERS_SIZE_MAX; i++) {
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
 * @brief Read a received message from the specified location in the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param rxLocation The location from which to read the message: RX FIFO0, RX FIFO1, or RX buffer.
 * @param rxHeader Pointer to the CAN_FdMessage structure to store the received message details.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_ReadFdFrameEx(CAN_Handle *handle, CAN_RxLocation rxLocation, CAN_FdRxMessage *rxHeader)
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
 * @brief Copy the message data to the target address.
 * @param targetAddr the address of message in sram.
 * @param txHeader Pointer to the CAN_TxMessage structure containing message details to be sent.
 * @retval None.
 */
static void CopyMessage(unsigned int *targetAddr, CAN_FdTxMessage *txHeader)
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
BASE_StatusType HAL_CAN_WriteFdFrameEx(CAN_Handle *handle, CAN_SendMsgMode sendMode, CAN_FdTxMessage *txHeader)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_PARAM_CHECK_WITH_RET(IsSendMsgMode(sendMode), BASE_STATUS_ERROR);
    CAN_ASSERT_PARAM(txHeader != NULL);
    CAN_PARAM_CHECK_WITH_RET((txHeader->dataLength <= COM_CAN_DLC_BYTES_64), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsFrameType(txHeader->txFrameType), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsIDType(txHeader->idType), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsFormat(txHeader->format), BASE_STATUS_ERROR);
    CAN_PARAM_CHECK_WITH_RET(IsBrsState(txHeader->brsState), BASE_STATUS_ERROR);

    /* Check if the module is in a busy state */
    if (handle->state != CAN_STATE_BUSY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }
    
    unsigned int regAddr, index = 0;
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
        index = DCL_CAN_GetTxFifoGetIndex(handle->baseAddress);
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
    ConfigMsgHeaderSecW(handle, targetAddr, txHeader);
    targetAddr++;
    /* Copy the message data to the target address */
    CopyMessage(targetAddr, txHeader);
    /* Trigger the transmission */
    DCL_CAN_TxbarRegSet(handle->baseAddress, (BASE_CFG_SET << index));
    return BASE_STATUS_OK;
}

/**
 * @brief Read a transmission event from the TX event FIFO in the CAN module.
 * @param handle Pointer to the CAN_Handle structure containing module configuration and state.
 * @param txEvt Pointer to the CAN_FdTxEventHeader structure to store the transmission event details.
 * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT, NOT SUPPORT.
 */
BASE_StatusType HAL_CAN_ReadTxEventFdFrameEx(CAN_Handle *handle, CAN_FdTxEventHeader *txEvt)
{
    CAN_ASSERT_PARAM(handle != NULL);
    CAN_ASSERT_PARAM(IsCANInstance(handle->baseAddress));
    CAN_ASSERT_PARAM(txEvt != NULL);

    unsigned int txEfifoAddr;
    unsigned int offset;
    unsigned int *targetAddr;
    unsigned int txEventElement;

    /* Check if the module is in a busy state */
    if (handle->state != CAN_STATE_BUSY) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }

    /* Check if there are events in the TX event FIFO */
    if (DCL_CAN_GetEventFifoFillLevel(handle->baseAddress) == 0) {
        handle->state = CAN_STATE_ERROR;
        return BASE_STATUS_ERROR;
    }
    
    /* Get the base address and offset for the TX event FIFO */
    txEfifoAddr = (DCL_CAN_GetEventFifoStartAddress(handle->baseAddress) << RAM_ADDR_RESERVE_BIT) + CAN_COM_RAM_ADDR;
    offset = DCL_CAN_GetEventFifoGetIndex(handle->baseAddress);
    offset = offset * TX_EVENT_FIFO_ELEMENT_SIZE;
    targetAddr = (unsigned int *)(void *)(uintptr_t)(txEfifoAddr + offset);

    /* Read and parse the event element data */
    txEventElement = *targetAddr;
    txEvt->idType = (txEventElement >> TX_EVENT_FIFO_XTD_OFFSET) & TX_EVENT_FIFO_XTD_MASK;
    txEvt->txFrameType = (txEventElement >> TX_EVENT_FIFO_RTR_OFFSET) & TX_EVENT_FIFO_RTR_MASK;
    if (txEvt->idType == CAN_ID_STANDARD) {
        txEvt->identifier = (txEventElement >> TX_EVENT_FIFO_IDS_OFFSET) & TX_EVENT_FIFO_IDS_MASK;
    } else {
        /* If idtpye is extended. */
        txEvt->identifier = (txEventElement >> TX_EVENT_FIFO_IDE_OFFSET) & TX_EVENT_FIFO_IDE_MASK;
    }
    targetAddr++;

    /* Read and parse the event element data */
    txEventElement = *targetAddr;
    txEvt->eventType = (txEventElement >> TX_EVENT_FIFO_ET_OFFSET) & TX_EVENT_FIFO_ET_MASK;
    txEvt->format = (txEventElement >> TX_EVENT_FIFO_FD_OFFSET) & TX_EVENT_FIFO_FD_MASK;
    txEvt->brsState = (txEventElement >> TX_EVENT_FIFO_BRS_OFFSET) & TX_EVENT_FIFO_BRS_MASK;
    txEvt->dataLength = BitToLength((txEventElement >> TX_EVENT_FIFO_DLC_OFFSET) & TX_EVENT_FIFO_DLC_MASK);
    
    /* Parse message marker and timestamp based on configuration */
    if (DCL_CAN_GetWideMessageMarker(handle->baseAddress) == BASE_CFG_SET) {
        txEvt->msgMarker = (txEventElement >> TX_EVENT_FIFO_MML_OFFSET) & TX_EVENT_FIFO_MML_MASK;
        txEvt->msgMarker += (txEventElement & (TX_EVENT_FIFO_MML_MASK << TX_EVENT_FIFO_MMH_OFFSET)) >> \
                            TX_EVENT_FIFO_MMH_OFFSET;
        if ((txEventElement >> TX_EVENT_FIFO_TXTSC_OFFSET) & TX_EVENT_FIFO_TXTSC_MASK) {
            txEvt->txTimestamp = (txEventElement >> TX_EVENT_FIFO_TXTSP_OFFSET) & TX_EVENT_FIFO_TXTSP_MASK;
        } else {
            txEvt->txTimestamp = 0;
        }
    } else {
        /* Not wide message marker. */
        txEvt->msgMarker = (txEventElement >> TX_EVENT_FIFO_MML_OFFSET) & TX_EVENT_FIFO_MML_MASK;
        txEvt->txTimestamp = (txEventElement >> TX_EVENT_FIFO_TXTS_OFFSET) & TX_EVENT_FIFO_TXTS_MASK;
    }

    return BASE_STATUS_OK;
}
#endif
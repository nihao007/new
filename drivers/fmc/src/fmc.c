/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2025. All rights reserved.
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
  * @file    fmc.c
  * @author  MCU Driver Team
  * @brief   FMC module driver
  * @details This file provides firmware functions to manage the following
  *          functionalities of the FMC.
  *          + Initialization and de-initialization functions
  *          + Peripheral Control functions
  */
#include "interrupt.h"
#include "systickinit.h"
#include "fmc.h"

#define FMC_WRITE_ENABLE_CMD       (0x06) /* Enable Flash write function */
#define FMC_PAGE_PROGRAM_CMD       (0x02) /* Page programming commands */
#define FMC_READ_DATA_CMD          (0x03) /* Read data command */
#define FMC_READ_STATUS_CMD        (0x05) /* Read status register command */

#define FMC_CHIP_ERASE_CMD         (0x60) /* Chip erase command */
#define FMC_CHIP_ERASE_CMD2        (0xC7) /* Backup chip erase command */

#define FMC_INT_ENABLE             (0xA3)     /* Enbale all interrupt */
#define FMC_ALL_INT_CLEAR          (0xFFFFFF) /* Clear all interrupt flags */

#define FMC_OP_SEND_CMD            (0x81) /* Send command operation */
#define FMC_OP_SEND_CMD_ADDR       (0xC1) /* Send command with address */
#define FMC_OP_SEND_CMD_WRITE      (0xA1) /* Send read command with data reading */
#define FMC_OP_SEND_CMD_READ       (0x85) /* Send read command with data writing */
#define FMC_OP_SEND_CMD_ADDR_WREN  (0xE1) /* Send write command with address */
#define FMC_OP_SEND_CMD_ADDR_READ  (0xC5) /* Send read command with address */

#define FMC_OP_READ_STATUS         (0x03) /* Read status operation */

#define FMC_FLASH_ENABLE_WRITE     (0x02) /* Enable Flash write function */
#define FMC_FLASH_BUSY             (0x01) /* Flash busy status flag */
#define FMC_DUMMY_SEND             (0x100) /* Enable dummy send */

#define FMC_TICK_MS_DIV            (1000) /* The divisor for converting milliseconds to clock cycles */

static void WaitWriteEnableCmplt(FMC_Handle *handle);
static bool WriteEnableCmplt(FMC_Handle *handle);

/**
  * @brief FMC init .
  * @param handle FMC handle.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_Init(FMC_Handle *handle)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    /* Init FMC state. */
    handle->state = FMC_STATE_READY;
    return BASE_STATUS_OK;
}

/**
  * @brief FMC deinit .
  * @param handle FMC handle.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_DeInit(FMC_Handle *handle)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    /* Deinit FMC state. */
    handle->state = FMC_STATE_RESET;
    return BASE_STATUS_OK;
}

/**
  * @brief FMC timing init .
  * @param handle FMC handle.
  * @param spiTime FMC timing config parameter struct.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_TimingInit(FMC_Handle *handle, FMC_TimingInit spiTime)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    FMC_ASSERT_PARAM(IsFMCCsSetupTime(spiTime.csSetupTime));
    FMC_ASSERT_PARAM(IsFMCCsDeselectTime(spiTime.csDeselTime));
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }
    /* Config cs setup time */
    handle->baseAddress->TIMING_SPI_CFG.BIT.tcss = spiTime.csSetupTime;
    /* Config cs deselect time */
    handle->baseAddress->TIMING_SPI_CFG.BIT.tshsl = spiTime.csDeselTime;
    return BASE_STATUS_OK;
}

/**
  * @brief FMC base parameter config .
  * @param handle FMC handle.
  * @param baseParam FMC base parameter config struct.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_BaseParamConfig(FMC_Handle *handle, FMC_BaseParameter baseParam)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    FMC_ASSERT_PARAM(IsFMCAddrType(baseParam.addrType));
    FMC_ASSERT_PARAM(IsFMCOpMode(baseParam.opMode));
    FMC_ASSERT_PARAM(IsFMCBootReadSpeedMode(baseParam.bootRdSpdMode));
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }
    handle->baseAddress->FMC_CFG.BIT.spi_nor_addr_mode = baseParam.addrType; /* Set address type */
    handle->baseAddress->FMC_CFG.BIT.op_mode = baseParam.opMode; /* Set operation mode */
    handle->baseAddress->FMC_BOOT_CMD.BIT.rd_cmd = baseParam.bootRdSpdMode;  /* Set boot read speed mode. */
    return BASE_STATUS_OK;
}

/**
  * @brief FMC operation config.
  * @param handle FMC handle.
  * @param opCfg FMC peration config struct.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_OpConfig(FMC_Handle *handle, FMC_OpConfig opCfg)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    FMC_ASSERT_PARAM(IsFMCCsSelect(opCfg.csSel));
    FMC_ASSERT_PARAM(IsFMCSPIifType(opCfg.ifType));
    FMC_ASSERT_PARAM(IsFMCSPINorAddrNum(opCfg.addrByteNum));
    FMC_ASSERT_PARAM(IsFMCDummyNum(opCfg.dummyNum));
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }
    handle->baseAddress->FMC_OP_CFG.BIT.fm_cs = opCfg.csSel; /* Set cs select */
    handle->baseAddress->FMC_OP_CFG.BIT.mem_if_type = opCfg.ifType; /* Set flash interface type */

    handle->baseAddress->FMC_OP_CFG.BIT.addr_num = opCfg.addrByteNum; /* Set flash address bytes num */
    handle->baseAddress->FMC_OP_CFG.BIT.dummy_num = opCfg.dummyNum; /* Set dummy num */
    if (handle->baseAddress->FMC_BOOT_CMD.BIT.rd_cmd == FMC_BOOT_READ_FAST_MODE) {
        handle->baseAddress->FMC_OP_CFG.BIT.dummy_num = 1; /* The dummy num should be set to 1 in fast mode  */
        handle->baseAddress->FMC_OP_CFG.BIT.oen_multicycle = 1; /* The oen_multicycle should be enable in fast mode */
    }
    
    return BASE_STATUS_OK;
}

/**
  * @brief FMC register callback function.
  * @param handle FMC handle.
  * @param event FMC callback event type.
  * @param pCallback FMC callback function.
  * @retval None.
  */
void HAL_FMC_RegisterCallback(FMC_Handle *handle, FMC_CallBackEvent event, FMC_CallbackFunType pCallback)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    /* Register the callback function */
    switch (event) {
        case FMC_WRITE_EVENT_DONE:
            handle->userCallBack.WriteCpltCallback = pCallback; /* Write complete */
            break;
        case FMC_READ_EVENT_DONE:
            handle->userCallBack.ReadCpltCallback = pCallback; /* Read complete */
            break;
        case FMC_DMA_WRITE_EVENT_DONE:
            handle->userCallBack.DmaWriteCpltCallback = pCallback; /* DMA write complete */
            break;
        case FMC_DMA_READ_EVENT_DONE:
            handle->userCallBack.DmaReadCpltCallback = pCallback; /* DMA read complete */
            break;
        case FMC_DMA_ERR:
        case FMC_ERR:
            handle->userCallBack.ErrorCallback = pCallback; /* error callback */
            break;
        default:
            return;
    }
}

/**
  * @brief Enable fma write, wait for fmc writing complete.
  * @param handle FMC handle.
  * @retval true is complete.
  */
static bool WriteEnableCmplt(FMC_Handle *handle)
{
    /* Writing enable commond. */
    handle->baseAddress->FMC_CMD.BIT.cmd1 = FMC_WRITE_ENABLE_CMD;
    /* Sending commond to flash. */
    handle->baseAddress->FMC_OP.reg = FMC_OP_SEND_CMD;
    /* Waiting for the operation to complete. */
    while (handle->baseAddress->FMC_OP.BIT.reg_op_start) {
        ; /* Wait FMC ready. */
    }
    return true;
}

/**
  * @brief Enable fma read, wait for fmc reading complete.
  * @param handle FMC handle.
  * @retval true is complete.
  */
static bool ReadStatusCmplt(FMC_Handle *handle)
{
    /* Read flash status commond. */
    handle->baseAddress->FMC_CMD.BIT.cmd1 = FMC_READ_STATUS_CMD;
    /* Sending commond to flash. */
    handle->baseAddress->FMC_OP.reg = FMC_OP_READ_STATUS;
    /* Waiting for the operation to complete. */
    while (handle->baseAddress->FMC_OP.BIT.reg_op_start) {
        ; /* Wait FMC ready. */
    }
    return true;
}

/**
  * @brief Waite write enable complete.
  * @param handle FMC handle.
  * @retval NoneT.
  */
static void WaitWriteEnableCmplt(FMC_Handle *handle)
{
    do {
        WriteEnableCmplt(handle);
        ReadStatusCmplt(handle);
        /* Wait until the flash is ready. */
    } while ((handle->baseAddress->FMC_FLASH_INFO.BIT.flash_status & FMC_FLASH_ENABLE_WRITE) == 0);
}

/**
  * @brief Waite spi nor flash status ready.
  * @param handle FMC handle.
  * @retval NoneT.
  */
static void WaitSpiNorFlashStatusReady(FMC_Handle *handle)
{
    do {
        ReadStatusCmplt(handle);
        /* Wait until the flash is ready. */
    } while ((handle->baseAddress->FMC_FLASH_INFO.BIT.flash_status & FMC_FLASH_BUSY) != 0);
}

/**
  * @brief FMC erase.
  * @param handle FMC handle.
  * @param eraseCmd Flash erase commond.
  * @param startAddr Flash erase start address.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_Erase(FMC_Handle *handle, unsigned int eraseCmd, unsigned int startAddr)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }

    handle->state = FMC_STATE_BUSY_ERASE;

    WaitWriteEnableCmplt(handle);
    handle->baseAddress->FMC_CFG.BIT.op_mode = 0x1; /* Enter normal mode. */
    /* Writing the erase commond. */
    handle->baseAddress->FMC_CMD.BIT.cmd1 = eraseCmd;
    /* Write the address. */
    handle->baseAddress->FMC_ADDRL = startAddr;
    /* Sending the commond and address to flash. */
    handle->baseAddress->FMC_OP.reg = (eraseCmd == FMC_CHIP_ERASE_CMD || eraseCmd == FMC_CHIP_ERASE_CMD2) ?
                                      FMC_OP_SEND_CMD : FMC_OP_SEND_CMD_ADDR;
    while (handle->baseAddress->FMC_OP.BIT.reg_op_start) {
        ; /* Wait FMC ready. */
    }
    WaitSpiNorFlashStatusReady(handle);

    handle->state = FMC_STATE_READY;
    return BASE_STATUS_OK;
}

/**
  * @brief Write data to fmc buffer.
  * @param srcAddr source address.
  * @param writeLen Write data length.
  * @retval None.
  */
static void WriteDataToBuffer(unsigned int srcAddr, unsigned int writeLen)
{
    unsigned char *buffer = FMC_BUFFER_ADDR;
    unsigned char *srcBuf = (unsigned char *)(uintptr_t)srcAddr;
    /* Write data to buffer */
    for (unsigned int i = 0; i < writeLen; i++) {
        *buffer = srcBuf[i];
        buffer++;
    }
}

/**
  * @brief Read data from fmc buffer.
  * @param destAddr destination address.
  * @param readLen Read data length.
  * @retval None.
  */
static void ReadDataFromBuffer(unsigned int destAddr, unsigned int readLen)
{
    unsigned char *buffer = FMC_BUFFER_ADDR;
    unsigned char *destBuf = (unsigned char *)(uintptr_t)destAddr;
    /* Read data from buffer */
    for (unsigned int i = 0; i < readLen; i++) {
        destBuf[i] = *buffer;
        buffer++;
    }
}

/**
  * @brief Write data config.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param writeLen Transfer data length.
  * @retval None.
  */
static void WriteData(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr, unsigned int writeLen)
{
    WaitWriteEnableCmplt(handle);

    WriteDataToBuffer(srcAddr, writeLen);
    /* Writing enable commond. */
    handle->baseAddress->FMC_CMD.BIT.cmd1 = handle->writeCmd;
    handle->baseAddress->FMC_ADDRL = destAddr;
    handle->baseAddress->FMC_DATA_NUM.BIT.op_data_num = writeLen;
    /* Sending commond/address/write enable to flash. */
    unsigned int fmcOpReg = (handle->baseAddress->FMC_OP_CFG.BIT.dummy_num == FMC_DUMMY_0BYTE) ?
                            FMC_OP_SEND_CMD_ADDR_WREN : (FMC_OP_SEND_CMD_ADDR_WREN | FMC_DUMMY_SEND);
    handle->baseAddress->FMC_OP.reg = fmcOpReg;
}

/**
  * @brief Read data config.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param readLen Transfer data length.
  * @retval None.
  */
static void ReadData(FMC_Handle *handle, unsigned int srcAddr, unsigned int readLen)
{
    /* Writing enable commond. */
    handle->baseAddress->FMC_CMD.BIT.cmd1 = handle->readCmd;
    handle->baseAddress->FMC_ADDRL = srcAddr;
    handle->baseAddress->FMC_DATA_NUM.BIT.op_data_num = readLen;
    /* Sending commond/address/write enable to flash. */
    unsigned int fmcOpReg = (handle->baseAddress->FMC_OP_CFG.BIT.dummy_num == FMC_DUMMY_0BYTE) ?
                            FMC_OP_SEND_CMD_ADDR_READ : (FMC_OP_SEND_CMD_ADDR_READ | FMC_DUMMY_SEND);
    handle->baseAddress->FMC_OP.reg = fmcOpReg;
}

/**
  * @brief Write data with DMA.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param writeLen Transfer data length.
  * @retval None.
  */
static void WriteDataDma(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr, unsigned int writeLen)
{
    WaitWriteEnableCmplt(handle);
    handle->baseAddress->FMC_DMA_SADDR_D0 = srcAddr; /* Config the source address. */
    handle->baseAddress->FMC_ADDRL = destAddr;      /* Config the destination address. */
    handle->baseAddress->FMC_DMA_LEN.BIT.dma_len = writeLen; /* Config the data length. */
    handle->baseAddress->FMC_OP_CTRL.BIT.rw_op = 1;  /* DMA write */
    handle->baseAddress->FMC_OP_CTRL.BIT.wr_opcode = handle->writeCmd; /* Write enable commond */
    handle->baseAddress->FMC_INT_EN.reg = FMC_INT_ENABLE; /* Enable interrupt */
    handle->baseAddress->FMC_OP_CTRL.BIT.dma_op_ready = 1; /* Start DMA. */
}

/**
  * @brief Config the address and transfer data size.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param transferSize Transfer data total length.
  * @retval None.
  */
static void ConfigParameters(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                             unsigned int transferSize)
{
    /* Config address */
    handle->srcAddr = srcAddr;
    handle->destAddr = destAddr;
    handle->transferSize = transferSize; /* Config total bytes number to transfer. */
    handle->txCount = 0;
    handle->rxCount = 0;
}

/**
  * @brief Update address and transfer length.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param transLen Transfer data length.
  * @param limitLen The maximum value to transfer data.
  * @retval None.
  */
static void UpdateNewAddrAndTranLen(FMC_Handle *handle, unsigned int *srcAddr, unsigned int *destAddr,
                                    unsigned int *transLen, unsigned int limitLen)
{
    unsigned int count = 0;
    if (handle->state == FMC_STATE_BUSY_WRITE) {
        count = handle->txCount;
    } else if (handle->state == FMC_STATE_BUSY_READ) {
        count = handle->rxCount;
    }
    /* Update address */
    *srcAddr = handle->srcAddr + count;
    *destAddr = handle->destAddr + count;
    /* Remain data length */
    unsigned int remainLen = handle->transferSize - count;
    *transLen = remainLen < limitLen ? remainLen : limitLen;
}

/**
  * @brief Fmc operate, surrport commond write, commond and address write,
  *        commond and data write or read, commond and address and data write or read .
  * @param handle FMC handle.
  * @param cmd SPI nor flash operation commond.
  * @param opType operation type.
  * @param wrRd write or read mode.
  * @param inputParam input parameter, include address, data length, timout.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_Operate(FMC_Handle *handle, unsigned char cmd, FMC_OpType opType,
                                FMC_DataWriteReadMode wrRd, FMC_InputParam inputParam)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    FMC_ASSERT_PARAM(IsFMCOpType(opType));
    FMC_ASSERT_PARAM(IsFMCWriteReadMode(wrRd));
    FMC_ASSERT_PARAM(inputParam.dataLen < FMC_PAGE_BYTES); /* Check the maximum data length */
    unsigned int preTick = 0;
    unsigned int curTick = 0;
    unsigned long long delta = 0;
    /* Target time difference for timeout */
    unsigned long long targetDelta = (unsigned long long)SYSTICK_GetCRGHZ() / FMC_TICK_MS_DIV * inputParam.timeOut;
    
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }

    handle->state = FMC_STATE_BUSY;
    handle->transferMode = FMC_BLOCKING_MODE;
    handle->baseAddress->FMC_CFG.BIT.op_mode = 0x1; /* Enter normal mode. */

    /* Init operation code */
    unsigned char opCode[FMC_OP_MAX] = {FMC_OP_SEND_CMD, FMC_OP_SEND_CMD_ADDR,
                                        FMC_OP_SEND_CMD_WRITE, FMC_OP_SEND_CMD_ADDR_WREN};
    if (wrRd == FMC_WRITE) {
        if (opType >= FMC_OP_CMD_DATA) {
            WaitWriteEnableCmplt(handle);
            WriteDataToBuffer(inputParam.srcAddr, inputParam.dataLen); /* Write data to buffer */
        }
        handle->baseAddress->FMC_ADDRL = inputParam.destAddr; /* Config address */
    } else {
        handle->baseAddress->FMC_ADDRL = inputParam.srcAddr; /* Config address */
        opCode[FMC_OP_CMD_DATA] = FMC_OP_SEND_CMD_READ; /* Update read operation code */
        opCode[FMC_OP_CMD_ADDR_DATA] = FMC_OP_SEND_CMD_ADDR_READ; /* Update read operation code */
    }
    /* Set current transfer length */
    handle->currTransLen = inputParam.dataLen;

    /* Writing enable commond. */
    handle->baseAddress->FMC_CMD.BIT.cmd1 = cmd;
    handle->baseAddress->FMC_DATA_NUM.BIT.op_data_num = inputParam.dataLen;
    /* Sending commond/address/write enable to flash. */
    handle->baseAddress->FMC_OP.reg = opCode[opType];

    while (handle->baseAddress->FMC_OP.BIT.reg_op_start) {
        curTick = DCL_SYSTICK_GetTick();
        delta = curTick > preTick ? curTick - preTick : SYSTICK_MAX_VALUE - preTick + curTick;
        if (delta >= targetDelta) { /* The configured timeout period is exceeded. */
            return BASE_STATUS_TIMEOUT;
        }
    }
    /* Read data from buffer to destination address */
    if (wrRd == FMC_READ && opType >= FMC_OP_CMD_DATA) {
        ReadDataFromBuffer(inputParam.destAddr, handle->currTransLen);
    }
    /* Wait spi nor flash status ready */
    WaitSpiNorFlashStatusReady(handle);
    handle->state = FMC_STATE_READY;
    return BASE_STATUS_OK;
}

/**
  * @brief Writing data in blocking mode.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param writeLen Number of the data to write.
  * @param timeout Timeout period,unit: ms.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_WriteBlocking(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                      unsigned int writeLen, unsigned int timeout)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));

    unsigned int preTick = 0;
    unsigned int curTick = 0;
    unsigned long long delta = 0;
    /* Target time difference for timeout */
    unsigned long long targetDelta = (unsigned long long)SYSTICK_GetCRGHZ() / FMC_TICK_MS_DIV * timeout;

    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }

    handle->state = FMC_STATE_BUSY_WRITE;
    handle->transferMode = FMC_BLOCKING_MODE;
    handle->baseAddress->FMC_CFG.BIT.op_mode = 0x1; /* Enter normal mode. */

    /* Calculate the number of bytes remaining on the current page */
    ConfigParameters(handle, srcAddr, destAddr, writeLen);
    /* Calculate remaining bytes in the current page */
    unsigned int remainBytes = FMC_PAGE_BYTES - destAddr % FMC_PAGE_BYTES;
    /* Determine the number of bytes to write in the first page */
    unsigned int fstPageBytes = remainBytes < writeLen ? remainBytes : writeLen;
    handle->currTransLen = fstPageBytes;
    unsigned int newSrcAddr = srcAddr;
    unsigned int newDestAddr = destAddr;
    /* Main transfer loop */
    while (handle->txCount < handle->transferSize) {
        /* Write data to the Flash memory */
        WriteData(handle, newSrcAddr, newDestAddr, handle->currTransLen);

        preTick = DCL_SYSTICK_GetTick();
        curTick = preTick;
        /* Waiting for the operation to complete. */
        while (handle->baseAddress->FMC_OP.BIT.reg_op_start) {
            curTick = DCL_SYSTICK_GetTick();
            delta = curTick > preTick ? curTick - preTick : SYSTICK_MAX_VALUE - preTick + curTick;
            if (delta >= targetDelta) { /* The configured timeout period is exceeded. */
                return BASE_STATUS_TIMEOUT;
            }
        }
        handle->txCount += handle->currTransLen;
        /* Update addresses and transfer length for the next page */
        UpdateNewAddrAndTranLen(handle, &newSrcAddr, &newDestAddr, &handle->currTransLen, FMC_PAGE_BYTES);
        if (handle->currTransLen == 0) {
            break;
        }
    }
    /* Wait for the Flash memory to be ready */
    WaitSpiNorFlashStatusReady(handle);
    handle->state = FMC_STATE_READY;

    return BASE_STATUS_OK;
}

/**
  * @brief Reading data in blocking mode.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param readLen Number of the data to read.
  * @param timeout Timeout period,unit: ms.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_ReadBlocking(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                     unsigned int readLen, unsigned int timeout)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    unsigned int preTick, curTick = 0;
    unsigned long long delta = 0;
    /* Target time difference for timeout */
    unsigned long long targetDelta = (unsigned long long)SYSTICK_GetCRGHZ() / FMC_TICK_MS_DIV * timeout;
    
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }

    handle->state = FMC_STATE_BUSY_READ;
    handle->transferMode = FMC_BLOCKING_MODE;
    handle->baseAddress->FMC_CFG.BIT.op_mode = 0x1; /* Enter normal mode. */
    /* Configure parameters for the read operation */
    ConfigParameters(handle, srcAddr, destAddr, readLen);
    /* Set current transfer length */
    handle->currTransLen = readLen < FMC_BUFFER_LEN ? readLen : FMC_BUFFER_LEN;
    unsigned int newSrcAddr = srcAddr;
    unsigned int newDestAddr = destAddr;
    /* Main transfer loop */
    while (handle->rxCount < handle->transferSize) {
        /* Read data from Flash memory */
        ReadData(handle, newSrcAddr, handle->currTransLen);
        preTick = DCL_SYSTICK_GetTick();
        curTick = preTick;
        /* Waiting for the operation to complete. */
        while (handle->baseAddress->FMC_OP.BIT.reg_op_start) {
            curTick = DCL_SYSTICK_GetTick();
            delta = curTick > preTick ? curTick - preTick : SYSTICK_MAX_VALUE - preTick + curTick;
            if (delta >= targetDelta) { /* The configured timeout period is exceeded. */
                return BASE_STATUS_TIMEOUT;
            }
        }
        /* Read data from buffer to destination address */
        ReadDataFromBuffer(newDestAddr, handle->currTransLen);
        
        handle->rxCount += handle->currTransLen;
        /* Update addresses and transfer length for the next transfer */
        UpdateNewAddrAndTranLen(handle, &newSrcAddr, &newDestAddr, &handle->currTransLen, FMC_BUFFER_LEN);
        if (handle->currTransLen == 0) {
            break;
        }
    }
    
    handle->state = FMC_STATE_READY;

    return BASE_STATUS_OK;
}

/**
  * @brief Writing data in interrupt mode.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param writeLen Number of the data to write.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_WriteIT(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                unsigned int writeLen)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }
    handle->state = FMC_STATE_BUSY_WRITE;
    handle->transferMode = FMC_INTERRUPT_MODE;
    handle->baseAddress->FMC_CFG.BIT.op_mode = 0x1; /* Enter normal mode. */
    /* Calculate the number of bytes remaining on the current page */
    unsigned int remainBytes = FMC_PAGE_BYTES - destAddr % FMC_PAGE_BYTES;
    unsigned int fstPageBytes = remainBytes < writeLen ? remainBytes : writeLen;
    handle->currTransLen = fstPageBytes;
    ConfigParameters(handle, srcAddr, destAddr, writeLen);
    handle->baseAddress->FMC_INT_EN.reg = FMC_INT_ENABLE;
    WriteData(handle, srcAddr, destAddr, handle->currTransLen);
    
    return BASE_STATUS_OK;
}

/**
  * @brief Reading data in interrupt mode.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param readLen Number of the data to read.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_ReadIT(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                               unsigned int readLen)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }
    handle->state = FMC_STATE_BUSY_READ;
    handle->transferMode = FMC_INTERRUPT_MODE;
    handle->baseAddress->FMC_CFG.BIT.op_mode = 0x1; /* Enter normal mode. */
    /* Calculate the number of bytes remaining on the current page */
    handle->currTransLen = readLen < FMC_BUFFER_LEN ? readLen : FMC_BUFFER_LEN;
    ConfigParameters(handle, srcAddr, destAddr, readLen);
    handle->baseAddress->FMC_INT_EN.reg = FMC_INT_ENABLE;
    ReadData(handle, srcAddr, handle->currTransLen);

    return BASE_STATUS_OK;
}

/**
  * @brief Writing data in DMA mode.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param writeLen Number of the data to write.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_WriteDMA(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                 unsigned int writeLen)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP_CTRL.BIT.dma_op_ready == 1) {
        return BASE_STATUS_BUSY;
    }
    handle->state = FMC_STATE_BUSY_WRITE;
    handle->transferMode = FMC_DMA_MODE;
    handle->baseAddress->FMC_CFG.BIT.op_mode = 0x1; /* Enter normal mode. */
    /* Calculate the number of bytes remaining on the current page */
    unsigned int remainBytes = FMC_PAGE_BYTES - destAddr % FMC_PAGE_BYTES;
    unsigned int fstPageBytes = remainBytes < writeLen ? remainBytes : writeLen;
    handle->currTransLen = fstPageBytes;
    ConfigParameters(handle, srcAddr, destAddr, writeLen);
    WriteDataDma(handle, srcAddr, destAddr, handle->currTransLen);
    return BASE_STATUS_OK;
}

/**
  * @brief Reading data in DMA mode.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param readLen Number of the data to read.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_ReadDMA(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                unsigned int readLen)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP_CTRL.BIT.dma_op_ready == 1) {
        return BASE_STATUS_BUSY;
    }

    handle->state = FMC_STATE_BUSY_READ;
    handle->transferMode = FMC_DMA_MODE;
    handle->baseAddress->FMC_CFG.BIT.op_mode = 0x1; /* Enter normal mode. */
    
    handle->baseAddress->FMC_DMA_SADDR_D0 = destAddr; /* Config the source address. */
    handle->baseAddress->FMC_ADDRL = srcAddr;      /* Config the destination address. */
    handle->baseAddress->FMC_DMA_LEN.BIT.dma_len = readLen; /* Config the data length. */
    handle->baseAddress->FMC_OP_CTRL.BIT.rw_op = 0;  /* DMA read */
    handle->baseAddress->FMC_OP_CTRL.BIT.rd_opcode = FMC_READ_DATA_CMD; /* Read enable commond */
    handle->baseAddress->FMC_INT_EN.reg = FMC_INT_ENABLE; /* Enable interrupt */
    handle->baseAddress->FMC_OP_CTRL.BIT.dma_op_ready = 1; /* Start DMA. */
    return BASE_STATUS_OK;
}

/**
  * @brief Reading data in boot mode.
  * @param handle FMC handle.
  * @param srcAddr source address.
  * @param destAddr destination address.
  * @param readLen Bytes of the data to read.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_BootRead(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                 unsigned int readLen)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    FMC_ASSERT_PARAM((srcAddr + readLen + (unsigned int)FMC_BUFFER_ADDR) < (unsigned int)FMC_BUFFER_ADDR_MAX);

    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }
    handle->baseAddress->FMC_CFG.BIT.op_mode = 0x0; /* Enter boot mode. */
    unsigned char *srcBuf = (unsigned char *)(void *)(uintptr_t)(srcAddr + ((unsigned int)FMC_BUFFER_ADDR));
    unsigned char *destBuf = (unsigned char *)(void *)(uintptr_t)destAddr;
    /* Read data */
    for (unsigned int i = 0; i < readLen; i++) {
        destBuf[i] = srcBuf[i];
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Reading data in boot mode.
  * @param handle FMC handle.
  * @param enable true is enable low power mode, false is close low power mode.
  * @retval BASE status type: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_FMC_LowPowerModeCtrl(FMC_Handle *handle, bool enable)
{
    FMC_ASSERT_PARAM(handle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(handle->baseAddress));
    /* Check FMC state */
    if (handle->baseAddress->FMC_OP.BIT.reg_op_start == 1) {
        return BASE_STATUS_BUSY;
    }
    DCL_FMC_SetLowPowerMode(handle->baseAddress, enable);
    return BASE_STATUS_OK;
}

/**
  * @brief Interrupt mode process.
  * @param handle FMC Handle.
  * @retval None.
  */
static void FMC_IntModeProcess(FMC_Handle *handle)
{
    unsigned int srcAddr = 0;
    unsigned int destAddr = 0;
    if (handle->state == FMC_STATE_BUSY_READ) {
        /* Read data */
        ReadDataFromBuffer(handle->destAddr, handle->currTransLen);
        handle->rxCount += handle->currTransLen;
        if (handle->rxCount < handle->transferSize) {
            UpdateNewAddrAndTranLen(handle, &srcAddr, &destAddr, &handle->currTransLen, FMC_BUFFER_LEN);
            ReadData(handle, srcAddr, handle->currTransLen);
            handle->baseAddress->FMC_INT_EN.reg = FMC_INT_ENABLE;
        } else {
            handle->state = FMC_STATE_READY;
            handle->baseAddress->FMC_INT_EN.reg = 0x0; /* Disable interrupt */
            handle->userCallBack.ReadCpltCallback(handle);  /* Read complete callback */
        }
    } else if (handle->state == FMC_STATE_BUSY_WRITE) {
        handle->txCount += handle->currTransLen;
        if (handle->txCount < handle->transferSize) {
            UpdateNewAddrAndTranLen(handle, &srcAddr, &destAddr, &handle->currTransLen, FMC_PAGE_BYTES);
            WriteData(handle, srcAddr, destAddr, handle->currTransLen);
            handle->baseAddress->FMC_INT_EN.reg = FMC_INT_ENABLE;
        } else {
            handle->state = FMC_STATE_READY;
            handle->baseAddress->FMC_INT_EN.reg = 0x0; /* Disable interrupt */
            handle->userCallBack.WriteCpltCallback(handle);  /* Write complete callback */
        }
    }
}

/**
  * @brief DMA mode process.
  * @param handle FMC Handle.
  * @retval None.
  */
static void FMC_DmaModeProcess(FMC_Handle *handle)
{
    unsigned int srcAddr = 0;
    unsigned int destAddr = 0;
    if (handle->state == FMC_STATE_BUSY_READ) {
        handle->state = FMC_STATE_READY;
        handle->baseAddress->FMC_INT_EN.reg = 0x0; /* Disable interrupt */
        handle->userCallBack.DmaReadCpltCallback(handle);  /* DMA read complete callback */
    } else if (handle->state == FMC_STATE_BUSY_WRITE) {
        handle->txCount += handle->currTransLen;
        if (handle->txCount < handle->transferSize) {
            UpdateNewAddrAndTranLen(handle, &srcAddr, &destAddr, &handle->currTransLen, FMC_PAGE_BYTES);
            WriteDataDma(handle, srcAddr, destAddr, handle->currTransLen);
        } else {
            handle->state = FMC_STATE_READY;
            handle->baseAddress->FMC_INT_EN.reg = 0x0; /* Disable interrupt */
            handle->userCallBack.DmaWriteCpltCallback(handle);  /* DMA write complete callback */
        }
    }
}

/**
  * @brief Interrupt Handling Function.
  * @param handle FMC Handle.
  * @retval None.
  */
void HAL_FMC_IrqHandler(void *handle)
{
    FMC_Handle *fmcHandle = (FMC_Handle *)handle;
    FMC_ASSERT_PARAM(fmcHandle != NULL);
    FMC_ASSERT_PARAM(IsFMCInstance(fmcHandle->baseAddress));

    /* Check FMC state */
    if (fmcHandle->baseAddress->FMC_OP.BIT.reg_op_start == 1 ||
        fmcHandle->baseAddress->FMC_OP_CTRL.BIT.dma_op_ready == 1) {
        return;
    }
    /* Error interrupt process */
    if (fmcHandle->baseAddress->FMC_INT.BIT.dma_err_int == 1 ||
        fmcHandle->baseAddress->FMC_INT.BIT.op_fail_int == 1 ||
        fmcHandle->baseAddress->FMC_INT.BIT.ahb_op_int == 1) {
        fmcHandle->userCallBack.ErrorCallback(fmcHandle);
    }
    if (fmcHandle->baseAddress->FMC_INT.BIT.op_done_int) {
        /* Interrupt mode */
        if (fmcHandle->transferMode == FMC_INTERRUPT_MODE) {
            FMC_IntModeProcess(fmcHandle);
        } else if (fmcHandle->transferMode == FMC_DMA_MODE) { /* DMA mode */
            FMC_DmaModeProcess(fmcHandle);
        }
    }
    fmcHandle->baseAddress->FMC_INT_CLR.reg = FMC_ALL_INT_CLEAR; /* Clear interrupt. */
}

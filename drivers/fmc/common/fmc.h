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
  * @file    fmc.h
  * @author  MCU Driver Team
  * @brief   FMC module driver.
  * @details This file provides firmware functions to manage the following
  *          functionalities of the FMC.
  *           + The definition of the FMC handle structure.
  *           + Initialization and de-initialization functions
  *           + Peripheral querying the transmission functions.
  *           + Peripheral interrupt handler and callback registration functions.
  */

/* Includes ------------------------------------------------------------------*/
#ifndef McuMagicTag_FMC_H
#define McuMagicTag_FMC_H
#include "fmc_ip.h"

/**
  * @defgroup FMC
  * @brief FMC module.
  * @{
  */

/**
  * @defgroup FMC_Common FMC Common
  * @brief FMC common external module.
  * @{
  */
/**
  * @brief Module Status Enumeration Definition
  */
typedef enum {
    FMC_STATE_RESET              = 0x00000000U,
    FMC_STATE_READY              = 0x00000001U,
    FMC_STATE_BUSY               = 0x00000002U,
    FMC_STATE_BUSY_ERASE         = 0x00000003U,
    FMC_STATE_BUSY_WRITE         = 0x00000004U,
    FMC_STATE_BUSY_READ          = 0x00000005U,
    FMC_STATE_ERROR              = 0x00000006U
} FMC_StateType;

/**
  * @brief Operation parameter configuration Definition
  */
typedef struct {
    FMC_CsSelect           csSel;           /**< CS select */
    FMC_SpiIfType          ifType;          /**< Interface type */
    FMC_SPINorAddrByteNum  addrByteNum;     /**< Address bytes number */
    FMC_DummyNum           dummyNum;        /**< Dummy number */
} FMC_OpConfig;

/**
  * @brief Timing Definition
  */
typedef struct {
    FMC_CsSetupTime        csSetupTime;     /**< CS setup delay configuration */
    FMC_CsDeselectTime     csDeselTime;     /**< CS deselect time configuration */
} FMC_TimingInit;

/**
  * @brief Base parameter configuration Definition
  */
typedef struct {
    FMC_SPINorAddrType     addrType;        /**< SPI NOR flash address type configuration */
    FMC_OpMode             opMode;          /**< Operation mode configuration. */
    FMC_BootReadSpeedMode  bootRdSpdMode;   /**< Configuring the Boot Read Speed Mode. */
} FMC_BaseParameter;

/**
  * @brief Input parameter definition
  */
typedef struct {
    unsigned int           srcAddr;         /**< Source address */
    unsigned int           destAddr;        /**< Destination address */
    unsigned int           dataLen;         /**< Data length */
    unsigned int           timeOut;         /**< Timeout time, unit is: ms */
} volatile FMC_InputParam;

/**
  * @defgroup FMC_Handle_Definition FMC Handle Definition
  * @{
  */
typedef struct _FMC_Handle {
    FMC_RegStruct         *baseAddress;     /**< Register base address. */
    FMC_TransferMode       transferMode;    /**< FMC transfer mode. */
    FMC_StateType          state;           /**< Running status of the FMC module. For details, see FMC_StateType. */
    unsigned int           srcAddr;         /**< Source address. */
    unsigned int           destAddr;        /**< Destination address. */
    unsigned int           transferSize;    /**< Total length of transmitted data. */
    unsigned int           currTransLen;    /**< Current transmission length. */
    unsigned int           txCount;         /**< Tx Length of data transferred. */
    unsigned int           rxCount;         /**< Rx Length of data transferred. */
    unsigned char          writeCmd;        /**< Read spi nor flash commond. */
    unsigned char          readCmd;         /**< Write spi nor flash commond. */
    FMC_UserCallBcak       userCallBack;    /**< User-defined callback function. */
    FMC_ExtendHandle       handleEx;        /**< Extend handle,  configuring some special parameters. */
} FMC_Handle;

/**
 * @brief Callback Function Type Definition.
 */
typedef void (*FMC_CallbackFunType)(void *handle);
/**
  * @}
  */

/**
  * @defgroup FMC_API_Declaration FMC HAL API
  * @brief Hardware abstraction layer.
  * @{
  */
BASE_StatusType HAL_FMC_Init(FMC_Handle *handle);
BASE_StatusType HAL_FMC_DeInit(FMC_Handle *handle);
BASE_StatusType HAL_FMC_TimingInit(FMC_Handle *handle, FMC_TimingInit spiTime);
BASE_StatusType HAL_FMC_BaseParamConfig(FMC_Handle *handle, FMC_BaseParameter baseParam);
BASE_StatusType HAL_FMC_OpConfig(FMC_Handle *handle, FMC_OpConfig opCfg);

void HAL_FMC_RegisterCallback(FMC_Handle *handle, FMC_CallBackEvent event, FMC_CallbackFunType pCallback);

BASE_StatusType HAL_FMC_Operate(FMC_Handle *handle, unsigned char cmd, FMC_OpType opType,
                                FMC_DataWriteReadMode wrRd, FMC_InputParam inputParam);
BASE_StatusType HAL_FMC_Erase(FMC_Handle *handle, unsigned int eraseCmd, unsigned int startAddr);
BASE_StatusType HAL_FMC_WriteBlocking(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                      unsigned int writeLen, unsigned int timeout);
BASE_StatusType HAL_FMC_ReadBlocking(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                     unsigned int readLen, unsigned int timeout);
BASE_StatusType HAL_FMC_WriteIT(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                unsigned int writeLen);
BASE_StatusType HAL_FMC_ReadIT(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                               unsigned int readLen);
BASE_StatusType HAL_FMC_WriteDMA(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                 unsigned int writeLen);
BASE_StatusType HAL_FMC_ReadDMA(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                unsigned int readLen);
BASE_StatusType HAL_FMC_BootRead(FMC_Handle *handle, unsigned int srcAddr, unsigned int destAddr,
                                 unsigned int readLen);
BASE_StatusType HAL_FMC_LowPowerModeCtrl(FMC_Handle *handle, bool enable);
void HAL_FMC_IrqHandler(void *handle);
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif  /* McuMagicTag_DMA_H */
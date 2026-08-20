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
  * @file    fmc_ip.h
  * @author  MCU Driver Team
  * @brief   FMC module driver
  * @details This file provides firmware functions to manage the following
  *          functionalities of the FMC.
  *          + Register definition structure
  *          + Direct configuration layer interface
  *          + Basic parameter configuration macro
  */

#ifndef McuMagicTag_FMC_IP_H
#define McuMagicTag_FMC_IP_H
#include "baseinc.h"

#define FMC_BUFFER_ADDR      (void *)(0x0F000000)
#define FMC_BUFFER_ADDR_MAX  (void *)(0x0F3FFFFF)
#define FMC_PAGE_BYTES          (0x100)       /* 256 bytes */
#define FMC_SECTOR_BYTES        (0x100)       /* 256 bytes */
#define FMC_BUFFER_LEN          (0x100)       /* 256 bytes */
#define FMC_TRANS_DATA_MAX_NUM  0x3FFF        /* 14 bits */
#define FMC_DMA_MAX_LENGTH      0xFFFFFFF     /* 27 bits */
#define FMC_OP_MAX_TIMEOUT      0xFFFFFF      /* 24 bits */

#ifdef FMC_PARAM_CHECK
#define FMC_ASSERT_PARAM  BASE_FUNC_ASSERT_PARAM
#define FMC_PARAM_CHECK_NO_RET BASE_FUNC_PARAMCHECK_NO_RET
#define FMC_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
#define FMC_ASSERT_PARAM(para) ((void)0U)
#define FMC_PARAM_CHECK_NO_RET(para) ((void)0U)
#define FMC_PARAM_CHECK_WITH_RET(para, ret) ((void)0U)
#endif

#define FMC_WAIT_MAX                (0xFFFF)
#define FMC_INT_MASK                (0x1FFFF)

/**
  * @brief SPI NOR flash address type.
  */
typedef enum {
    FMC_SPI_NOR_ADDR_3BYTES = 0x00000000U,
    FMC_SPI_NOR_ADDR_4BYTES = 0x00000001U
} volatile FMC_SPINorAddrType;

/**
  * @brief SPI NOR flash address bytes.
  */
typedef enum {
    FMC_SPI_NOR_ADDR_BYTES_NUM_3 = 0x00000003U,
    FMC_SPI_NOR_ADDR_BYTES_NUM_4 = 0x00000004U
} volatile FMC_SPINorAddrByteNum;

/**
  * @brief FMC operate mode.
  */
typedef enum {
    FMC_BOOT_MODE = 0x00000000U,
    FMC_NORMAL_MODE = 0x00000001U,
} volatile FMC_OpMode;

/**
  * @brief FMC read delay clock.
  */
typedef enum {
    FMC_READ_DELAY_ZERO_CLK                 = 0x00000000U,
    FMC_READ_DELAY_ZERO_PORINT_FIVE_CLK     = 0x00000001U,
    FMC_READ_DELAY_ONE_CLK                  = 0x00000002U,
    FMC_READ_DELAY_ONE_PORINT_FIVE_CLK      = 0x00000003U,
    FMC_READ_DELAY_TWO_CLK                  = 0x00000004U,
    FMC_READ_DELAY_TWO_PORINT_FIVE_CLK      = 0x00000005U,
    FMC_READ_DELAY_THREE_CLK                = 0x00000006U,
    FMC_READ_DELAY_THREE_PORINT_FIVE_CLK    = 0x00000007U,
} volatile FMC_ReadDelay;

/**
  * @brief FMC cs setup wait clock.
  */
typedef enum {
    FMC_CS_SETUP_TIME_1_CLK   = 0x00000000U,
    FMC_CS_SETUP_TIME_2_CLK   = 0x00000001U,
    FMC_CS_SETUP_TIME_3_CLK   = 0x00000002U,
    FMC_CS_SETUP_TIME_4_CLK   = 0x00000003U,
    FMC_CS_SETUP_TIME_5_CLK   = 0x00000004U,
    FMC_CS_SETUP_TIME_6_CLK   = 0x00000005U,
    FMC_CS_SETUP_TIME_7_CLK   = 0x00000006U,
    FMC_CS_SETUP_TIME_8_CLK   = 0x00000007U,
} volatile FMC_CsSetupTime;

/**
  * @brief FMC cs deselect wait clock.
  */
typedef enum {
    FMC_CS_DESELECT_TIME_1_CLK   = 0x00000000U,
    FMC_CS_DESELECT_TIME_2_CLK   = 0x00000001U,
    FMC_CS_DESELECT_TIME_3_CLK   = 0x00000002U,
    FMC_CS_DESELECT_TIME_4_CLK   = 0x00000003U,
    FMC_CS_DESELECT_TIME_5_CLK   = 0x00000004U,
    FMC_CS_DESELECT_TIME_6_CLK   = 0x00000005U,
    FMC_CS_DESELECT_TIME_7_CLK   = 0x00000006U,
    FMC_CS_DESELECT_TIME_8_CLK   = 0x00000007U,
    FMC_CS_DESELECT_TIME_9_CLK   = 0x00000008U,
    FMC_CS_DESELECT_TIME_10_CLK  = 0x00000009U,
    FMC_CS_DESELECT_TIME_11_CLK  = 0x0000000AU,
    FMC_CS_DESELECT_TIME_12_CLK  = 0x0000000BU,
    FMC_CS_DESELECT_TIME_13_CLK  = 0x0000000CU,
    FMC_CS_DESELECT_TIME_14_CLK  = 0x0000000DU,
    FMC_CS_DESELECT_TIME_15_CLK  = 0x0000000EU,
    FMC_CS_DESELECT_TIME_16_CLK  = 0x0000000FU,
} volatile FMC_CsDeselectTime;

/**
  * @brief FMC cs.
  */
typedef enum {
    FMC_SELECT_CS_0    = 0x00000000U,
    FMC_SELECT_CS_1    = 0x00000001U
} volatile FMC_CsSelect;

/**
  * @brief FMC interface type.
  */
typedef enum {
    FMC_STANDARD_SPI                    = 0x00000000U,
    FMC_DUAL_INPUT_OR_OUTPUT_SPI        = 0x00000001U,
    FMC_DUAL_IO_SPI                     = 0x00000002U,
    FMC_QUAD_INPUT_OR_OUTPUT_SPI        = 0x00000003U,
    FMC_QUAD_IO_SPI                     = 0x00000004U,
} volatile FMC_SpiIfType;

/**
  * @brief FMC dummy bytes.
  */
typedef enum {
    FMC_DUMMY_0BYTE  = 0x00000000U,
    FMC_DUMMY_1BYTE  = 0x00000001U,
    FMC_DUMMY_2BYTE  = 0x00000002U,
    FMC_DUMMY_3BYTE  = 0x00000003U,
    FMC_DUMMY_4BYTE  = 0x00000004U,
    FMC_DUMMY_5BYTE  = 0x00000005U,
    FMC_DUMMY_6BYTE  = 0x00000006U,
    FMC_DUMMY_7BYTE  = 0x00000007U,
    FMC_DUMMY_8BYTE  = 0x00000008U,
    FMC_DUMMY_9BYTE  = 0x00000009U,
    FMC_DUMMY_10BYTE = 0x0000000AU,
    FMC_DUMMY_11BYTE = 0x0000000BU,
    FMC_DUMMY_12BYTE = 0x0000000CU,
    FMC_DUMMY_13BYTE = 0x0000000DU,
    FMC_DUMMY_14BYTE = 0x0000000EU,
    FMC_DUMMY_15BYTE = 0x0000000FU,
} volatile FMC_DummyNum;

/**
  * @brief Callback Triggering Event Enumeration Definition
  */
typedef enum {
    FMC_WRITE_EVENT_DONE,
    FMC_READ_EVENT_DONE,
    FMC_DMA_WRITE_EVENT_DONE,
    FMC_DMA_READ_EVENT_DONE,
    FMC_DMA_ERR,
    FMC_ERR,
} volatile FMC_CallBackEvent;

/**
  * @brief FMC transfer mode.
  */
typedef enum {
    FMC_BLOCKING_MODE   = 0x00000000U,
    FMC_INTERRUPT_MODE  = 0x00000001U,
    FMC_DMA_MODE        = 0x00000002U,
} volatile FMC_TransferMode;

/**
  * @brief FMC boot mode tpye.
  */
typedef enum {
    FMC_BOOT_READ_NORMAL_MODE     = 0x03,
    FMC_BOOT_READ_FAST_MODE       = 0x0B,
} volatile FMC_BootReadSpeedMode;

/**
  * @brief FMC operate type.
  */
typedef enum {
    FMC_OP_CMD_ONLY = 0,
    FMC_OP_CMD_ADDR = 1,
    FMC_OP_CMD_DATA = 2,
    FMC_OP_CMD_ADDR_DATA = 3,
    FMC_OP_MAX,
} volatile FMC_OpType;

/**
  * @brief FMC write or read mode.
  */
typedef enum {
    FMC_WRITE = 0,
    FMC_READ  = 1,
} volatile FMC_DataWriteReadMode;

/**
  * @brief FMC extend handle, configuring some special parameters.
  */
typedef struct {
} volatile FMC_ExtendHandle;

/**
  * @brief User-defined callback function.
  */
typedef struct {
    /* Reading completion callback function */
    void (* ReadCpltCallback)(void *handle);
    /* Writing completion callback function */
    void (* WriteCpltCallback)(void *handle);
    /* Dma reading completion callback function */
    void (* DmaReadCpltCallback)(void *handle);
    /* DMA writing completion callback function */
    void (* DmaWriteCpltCallback)(void *handle);
    /* Error callback function */
    void (* ErrorCallback)(void *handle);
} volatile FMC_UserCallBcak;


/* Define the union FMC_CFG_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    op_mode               : 1   ; /* [0] */
        unsigned int    flash_sel             : 2   ; /* [2..1] */
        unsigned int    reserved0             : 7   ; /* [9..3] */
        unsigned int    spi_nor_addr_mode     : 1   ; /* [10] */
        unsigned int    reserved1             : 21  ; /* [31..11] */
    } BIT;
} volatile FMC_CFG_REG;

/* Define the union GLOBAL_CFG_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    reserved0             : 3   ; /* [2..0] */
        unsigned int    rd_delay              : 3   ; /* [5..3] */
        unsigned int    wp_en                 : 1   ; /* [6] */
        unsigned int    reserved1             : 25  ; /* [31..7] */
    } BIT;
} volatile GLOBAL_CFG_REG;

/* Define the union TIMING_SPI_CFG_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    tshsl                 : 4   ; /* [3..0] */
        unsigned int    tcss                  : 4   ; /* [7..4] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile TIMING_SPI_CFG_REG;

/* Define the union FMC_INT_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    op_done_int           : 1   ; /* [0] */
        unsigned int    op_fail_int           : 1   ; /* [1] */
        unsigned int    reserved0             : 3   ; /* [4..2] */
        unsigned int    dma_err_int           : 1   ; /* [5] */
        unsigned int    reserved1             : 1   ; /* [6] */
        unsigned int    ahb_op_int            : 1   ; /* [7] */
        unsigned int    reserved2             : 24  ; /* [31..8] */
    } BIT;
} volatile FMC_INT_REG;

/* Define the union FMC_INT_EN_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    op_done_int_en        : 1   ; /* [0] */
        unsigned int    op_fail_int_en        : 1   ; /* [1] */
        unsigned int    reserved0             : 3   ; /* [4..2] */
        unsigned int    dma_err_int_en        : 1   ; /* [5] */
        unsigned int    reserved1             : 1   ; /* [6] */
        unsigned int    ahb_op_int_en         : 1   ; /* [7] */
        unsigned int    reserved2             : 24  ; /* [31..8] */
    } BIT;
} volatile FMC_INT_EN_REG;

/* Define the union FMC_INT_CLR_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    op_done_int_clr       : 1   ; /* [0] */
        unsigned int    op_fail_int_clr       : 1   ; /* [1] */
        unsigned int    reserved0             : 3   ; /* [4..2] */
        unsigned int    dma_err_int_clr       : 1   ; /* [5] */
        unsigned int    reserved1             : 1   ; /* [6] */
        unsigned int    ahb_op_int_clr        : 1   ; /* [7] */
        unsigned int    reserved2             : 24  ; /* [31..8] */
    } BIT;
} volatile FMC_INT_CLR_REG;

/* Define the union FMC_CMD_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    cmd1                  : 8   ; /* [7..0] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile FMC_CMD_REG;

/* Define the union FMC_OP_CFG_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    dummy_num             : 4   ; /* [3..0] */
        unsigned int    addr_num              : 3   ; /* [6..4] */
        unsigned int    mem_if_type           : 3   ; /* [9..7] */
        unsigned int    force_cs_en           : 1   ; /* [10] */
        unsigned int    fm_cs                 : 2   ; /* [12..11] */
        unsigned int    oen_multicycle        : 1   ; /* [13] */
        unsigned int    reserved0             : 18  ; /* [31..14] */
    } BIT;
} volatile FMC_OP_CFG_REG;

/* Define the union FMC_DATA_NUM_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    op_data_num           : 14  ; /* [13..0] */
        unsigned int    reserved0             : 18  ; /* [31..14] */
    } BIT;
} volatile FMC_DATA_NUM_REG;

/* Define the union FMC_OP_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    reg_op_start          : 1   ; /* [0] */
        unsigned int    read_status_en        : 1   ; /* [1] */
        unsigned int    read_data_en          : 1   ; /* [2] */
        unsigned int    reserved0             : 2   ; /* [4..3] */
        unsigned int    write_data_en         : 1   ; /* [5] */
        unsigned int    addr_en               : 1   ; /* [6] */
        unsigned int    cmd1_en               : 1   ; /* [7] */
        unsigned int    dummy_en              : 1   ; /* [8] */
        unsigned int    reserved1             : 23  ; /* [31..9] */
    } BIT;
} volatile FMC_OP_REG;

/* Define the union FMC_DMA_LEN_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    dma_len               : 28  ; /* [27..0] */
        unsigned int    reserved0             : 4   ; /* [31..28] */
    } BIT;
} volatile FMC_DMA_LEN_REG;

/* Define the union FMC_DMA_AHB_CTRL_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    burst4_en             : 1   ; /* [0] */
        unsigned int    burst8_en             : 1   ; /* [1] */
        unsigned int    burst16_en            : 1   ; /* [2] */
        unsigned int    reserved0             : 29  ; /* [31..3] */
    } BIT;
} volatile FMC_DMA_AHB_CTRL_REG;


/* Define the union FMC_OP_CTRL_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    dma_op_ready          : 1   ; /* [0] */
        unsigned int    rw_op                 : 1   ; /* [1] */
        unsigned int    reserved0             : 6   ; /* [7..2] */
        unsigned int    wr_opcode             : 8   ; /* [15..8] */
        unsigned int    rd_opcode             : 8   ; /* [23..16] */
        unsigned int    reserved1             : 8   ; /* [31..24] */
    } BIT;
} volatile FMC_OP_CTRL_REG;

/* Define the union FMC_TIMEOUT_WR_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    timeout_wr            : 24  ; /* [23..0] */
        unsigned int    reserved0             : 8   ; /* [31..24] */
    } BIT;
} volatile FMC_TIMEOUT_WR_REG;

/* Define the union FMC_LP_CTRL_REG */
typedef union {
    unsigned int    reg;
    struct {
        unsigned int    clk_gate_en           : 1   ; /* [0] */
        unsigned int    mem_clk_en            : 1   ; /* [1] */
        unsigned int    reserved0             : 30  ; /* [31..2] */
    } BIT;
} volatile FMC_LP_CTRL_REG;

/* Define the union FMC_FLASH_INFO_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    flash_status          : 8   ; /* [7..0] */
        unsigned int    exp_flash_status      : 8   ; /* [15..8] */
        unsigned int    bb_flag               : 8   ; /* [23..16] */
        unsigned int    exp_bb_flag           : 8   ; /* [31..24] */
    } BIT;
} volatile FMC_FLASH_INFO_REG;

/* Define the union FMC_BOOT_CMD_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int    rd_cmd                : 8   ; /* [7..0] */
        unsigned int    reserved0             : 24  ; /* [31..8] */
    } BIT;
} volatile FMC_BOOT_CMD_REG;

typedef enum {
    DMA_READ,
    DMA_WRITE,
} volatile FMC_Dma_Op_Type;

//==============================================================================
/* Define the global struct */
typedef struct {
    FMC_CFG_REG                FMC_CFG                         ; /* 0x0 */
    GLOBAL_CFG_REG             GLOBAL_CFG                      ; /* 0x4 */
    TIMING_SPI_CFG_REG         TIMING_SPI_CFG                  ; /* 0x8 */
    unsigned int               reserved0[3]                    ; /* 0xc~0x14 */
    FMC_INT_REG                FMC_INT                         ; /* 0x18 */
    FMC_INT_EN_REG             FMC_INT_EN                      ; /* 0x1c */
    FMC_INT_CLR_REG            FMC_INT_CLR                     ; /* 0x20 */
    FMC_CMD_REG                FMC_CMD                         ; /* 0x24 */
    unsigned int               reserved1                       ; /* 0x28 */
    unsigned int               FMC_ADDRL                       ; /* 0x2c */
    FMC_OP_CFG_REG             FMC_OP_CFG                      ; /* 0x30 */
    unsigned int               SPI_OP_ADDR                     ; /* 0x34 */
    FMC_DATA_NUM_REG           FMC_DATA_NUM                    ; /* 0x38 */
    FMC_OP_REG                 FMC_OP                          ; /* 0x3c */
    FMC_DMA_LEN_REG            FMC_DMA_LEN                     ; /* 0x40 */
    unsigned int               reserved2                       ; /* 0x44~0x47 */
    FMC_DMA_AHB_CTRL_REG       FMC_DMA_AHB_CTRL                ; /* 0x48 */
    unsigned int               FMC_DMA_SADDR_D0                ; /* 0x4c */
    unsigned int               reserved3[6]                    ; /* 0x50~0x64 */
    FMC_OP_CTRL_REG            FMC_OP_CTRL                     ; /* 0x68 */
    FMC_TIMEOUT_WR_REG         FMC_TIMEOUT_WR                  ; /* 0x6c */
    unsigned int               reserved4[2]                    ; /* 0x70~0x74 */
    FMC_LP_CTRL_REG            FMC_LP_CTRL                     ; /* 0x78 */
    unsigned int               reserved5[12]                   ; /* 0x7c~0xa8 */
    FMC_FLASH_INFO_REG         FMC_FLASH_INFO                  ; /* 0xac */
    unsigned int               reserved6[199]                  ; /* 0xb0~0x3c8 */
    FMC_BOOT_CMD_REG           FMC_BOOT_CMD                    ; /* 0x3cc */
    unsigned int               reserved7[24]                   ; /* 0x3d0~0x45c */
} volatile FMC_RegStruct;


/* Declare the functions that set the member value */

/**
  * @brief Checking FMC cs setup time Parameters.
  * @param eccType Flash ecc type.
  * @retval Bool
  */
static inline bool IsFMCCsSetupTime(FMC_CsSetupTime csSetupTime)
{
    return (csSetupTime >= FMC_CS_SETUP_TIME_1_CLK && csSetupTime <= FMC_CS_SETUP_TIME_8_CLK);
}

/**
  * @brief Checking FMC cs deselect time Parameters.
  * @param eccType Flash ecc type.
  * @retval Bool
  */
static inline bool IsFMCCsDeselectTime(FMC_CsDeselectTime csDeselTime)
{
    return (csDeselTime >= FMC_CS_DESELECT_TIME_1_CLK && csDeselTime <= FMC_CS_DESELECT_TIME_16_CLK);
}

/**
  * @brief Checking FMC address type Parameters.
  * @param eccType Flash ecc type.
  * @retval Bool
  */
static inline bool IsFMCAddrType(FMC_SPINorAddrType addrType)
{
    return (addrType == FMC_SPI_NOR_ADDR_3BYTES || addrType == FMC_SPI_NOR_ADDR_4BYTES);
}

/**
  * @brief Checking FMC operation mode Parameters.
  * @param eccType Flash ecc type.
  * @retval Bool
  */
static inline bool IsFMCOpMode(FMC_OpMode opMode)
{
    return (opMode == FMC_BOOT_MODE || opMode == FMC_NORMAL_MODE);
}

/**
  * @brief Checking FMC boot read speed Parameters.
  * @param eccType Flash ecc type.
  * @retval Bool
  */
static inline bool IsFMCBootReadSpeedMode(FMC_BootReadSpeedMode bootRdSpdMode)
{
    return (bootRdSpdMode == FMC_BOOT_READ_NORMAL_MODE || bootRdSpdMode == FMC_BOOT_READ_FAST_MODE);
}

/**
  * @brief Checking FMC cs select Parameters.
  * @param eccType Flash ecc type.
  * @retval Bool
  */
static inline bool IsFMCCsSelect(FMC_CsSelect csSele)
{
    return (csSele == FMC_SELECT_CS_0 || csSele == FMC_SELECT_CS_1);
}

/**
  * @brief Checking FMC spi interface type Parameters.
  * @param eccType Flash ecc type.
  * @retval Bool
  */
static inline bool IsFMCSPIifType(FMC_SpiIfType ifType)
{
    /* Check FMC spi interface type */
    return (ifType == FMC_STANDARD_SPI || \
            ifType == FMC_DUAL_INPUT_OR_OUTPUT_SPI || \
            ifType == FMC_DUAL_IO_SPI || \
            ifType == FMC_QUAD_INPUT_OR_OUTPUT_SPI || \
            ifType == FMC_QUAD_IO_SPI);
}

/**
  * @brief Checking FMC spi nor flash address number Parameters.
  * @param eccType Flash ecc type.
  * @retval Bool
  */
static inline bool IsFMCSPINorAddrNum(FMC_SPINorAddrByteNum addrByteNum)
{
    return (addrByteNum == FMC_SPI_NOR_ADDR_BYTES_NUM_3 || addrByteNum == FMC_SPI_NOR_ADDR_BYTES_NUM_4);
}

/**
  * @brief Checking FMC dummy num Parameters.
  * @param eccType Flash ecc type.
  * @retval Bool
  */
static inline bool IsFMCDummyNum(FMC_DummyNum dummyNum)
{
    return (dummyNum >= FMC_DUMMY_0BYTE && dummyNum <= FMC_DUMMY_15BYTE);
}

/**
  * @brief Checking FMC read delay clock.
  * @param eccType Flash ecc type.
  * @retval true
  * @retval false
  */
static inline bool IsFMCReadDelayClk(FMC_ReadDelay clkNum)
{
    return (clkNum >= FMC_READ_DELAY_ZERO_CLK && clkNum <= FMC_READ_DELAY_THREE_PORINT_FIVE_CLK);
}

/**
  * @brief Checking FMC read delay clock.
  * @param eccType Flash ecc type.
  * @retval true
  * @retval false
  */
static inline bool IsFMCIntType(unsigned int intType)
{
    return ((intType & FMC_INT_MASK) != BASE_CFG_UNSET) && ((intType & ~FMC_INT_MASK) == BASE_CFG_UNSET);
}

/**
  * @brief Checking FMC DMA operation type.
  * @param dmaOpType Dma operation type.
  * @retval true
  * @retval false
  */
static inline bool IsFMCDmaOpType(FMC_Dma_Op_Type dmaOpType)
{
    return ((dmaOpType == DMA_WRITE) || (dmaOpType == DMA_READ));
}

/**
  * @brief Checking FMC write or read mode.
  * @param wrRd Write or read mode.
  * @retval true
  * @retval false
  */
static inline bool IsFMCWriteReadMode(FMC_DataWriteReadMode wrRd)
{
    return ((wrRd == FMC_WRITE) || (wrRd == FMC_READ));
}

/**
  * @brief Checking FMC operation type.
  * @param opType operation type.
  * @retval true
  * @retval false
  */
static inline bool IsFMCOpType(FMC_OpType opType)
{
    return ((opType >= FMC_OP_CMD_ONLY) && (opType < FMC_OP_MAX));
}

/* Direct configuration layer interface----------------------------------*/
/**
  * @brief Set flash address type.
  * @param fmc FMC register base address.
  * @param addrType Flash address type.
  * @retval None.
  */
static inline void DCL_FMC_SetAddrType(FMC_RegStruct *fmc, FMC_SPINorAddrType addrType)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCAddrType(addrType));
    fmc->FMC_CFG.BIT.spi_nor_addr_mode = addrType;
}

/**
  * @brief Get flash address type.
  * @param fmc FMC register base address.
  * @retval unsigned char, address type.
  */
static inline unsigned char DCL_FMC_GetAddrType(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_CFG.BIT.spi_nor_addr_mode);
}

/**
  * @brief Set operate mode.
  * @param fmc FMC register base address.
  * @param opMode Operate mode, boot/normal.
  * @retval None.
  */
static inline void DCL_FMC_SetOpMode(FMC_RegStruct *fmc, FMC_OpMode opMode)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCOpMode(opMode));
    fmc->FMC_CFG.BIT.op_mode = opMode;
}

/**
  * @brief Get operate mode.
  * @param fmc FMC register base address.
  * @retval Operate mode, boot/normal.
  */
static inline unsigned char DCL_FMC_GetOpMode(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_CFG.BIT.op_mode);
}

/**
  * @brief Set write protect.
  * @param fmc FMC register base address.
  * @param enable true or false.
  * @retval None.
  */
static inline void DCL_FMC_SetWriteProtect(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->GLOBAL_CFG.BIT.wp_en = enable;
}

/**
  * @brief Get write protect.
  * @param fmc FMC register base address.
  * @retval true or false.
  */
static inline bool DCL_FMC_GetWriteProtect(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->GLOBAL_CFG.BIT.wp_en);
}

/**
  * @brief Set read delay clock.
  * @param fmc FMC register base address.
  * @param enable true or false.
  * @retval None.
  */
static inline void DCL_FMC_SetReadDelayClk(FMC_RegStruct *fmc, FMC_ReadDelay clkNum)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCReadDelayClk(clkNum));
    fmc->GLOBAL_CFG.BIT.rd_delay = clkNum;
}

/**
  * @brief Get read delay clock.
  * @param fmc FMC register base address.
  * @retval read delay clock configuration,see registers description.
  */
static inline unsigned char DCL_FMC_GetReadDelayClk(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->GLOBAL_CFG.BIT.rd_delay);
}

/**
  * @brief Set cs setup time.
  * @param fmc FMC register base address.
  * @param setupTime cs setup time.
  * @retval None.
  */
static inline void DCL_FMC_SetCsSetupTime(FMC_RegStruct *fmc, FMC_CsSetupTime setupTime)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCCsSetupTime(setupTime));
    fmc->TIMING_SPI_CFG.BIT.tcss = setupTime;
}

/**
  * @brief Get cs setup time.
  * @param fmc FMC register base address.
  * @retval cs setup time configuration,see registers description.
  */
static inline unsigned char DCL_FMC_GetCsSetupTime(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return(fmc->TIMING_SPI_CFG.BIT.tcss);
}

/**
  * @brief Set cs deselect time.
  * @param fmc FMC register base address.
  * @param csDeselTime  cs deselect time.
  * @retval None.
  */
static inline void DCL_FMC_SetCsDeselectTime(FMC_RegStruct *fmc, FMC_CsDeselectTime csDeselTime)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCCsDeselectTime(csDeselTime));
    fmc->TIMING_SPI_CFG.BIT.tshsl = csDeselTime;
}

/**
  * @brief Get cs deselect time.
  * @param fmc FMC register base address.
  * @retval cs deselect time configuration,see registers description.
  */
static inline unsigned char DCL_FMC_GetCsDeselectTime(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->TIMING_SPI_CFG.BIT.tshsl);
}

/**
  * @brief Get FMC interrupt status.
  * @param fmc FMC register base address.
  * @retval Interrupt status.
  */
static inline unsigned int DCL_FMC_GetIntStatus(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_INT.reg);
}

/**
  * @brief Set FMC interrupt enable.
  * @param fmc FMC register base address.
  * @param intType interrupt type mask .
  * @retval None.
  */
static inline void DCL_FMC_SetIntEnable(FMC_RegStruct *fmc, unsigned int intType)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCIntType(intType));
    fmc->FMC_INT_EN.reg = intType;
}

/**
  * @brief Get FMC interrupt enable.
  * @param fmc FMC register base address.
  * @retval FMC_INT register configuration.
  */
static inline unsigned int  DCL_FMC_GetIntEnable(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_INT_EN.reg);
}

/**
  * @brief Clear FMC interrupt.
  * @param fmc FMC register base address.
  * @retval None.
  */
static inline void DCL_FMC_IntClear(FMC_RegStruct *fmc, unsigned int intType)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCIntType(intType));
    fmc->FMC_INT_CLR.reg = intType;
}

/**
  * @brief Set FMC commond1.
  * @param fmc FMC register base address.
  * @param cmd1 The commond to operate flash.
  * @retval None.
  */
static inline void DCL_FMC_SetCmd1(FMC_RegStruct *fmc, unsigned char cmd1)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_CMD.BIT.cmd1 = cmd1;
}

/**
  * @brief Get FMC commond1.
  * @param fmc FMC register base address.
  * @retval The commond to operate flash.
  */
static inline unsigned char DCL_FMC_GetCmd1(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_CMD.BIT.cmd1);
}

/**
  * @brief Set flash address low byte.
  * @param fmc FMC register base address.
  * @param addrLowByte Address low byte.
  * @retval None.
  */
static inline void DCL_FMC_SetAddrLowByte(FMC_RegStruct *fmc, unsigned int addrLowByte)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_ADDRL = addrLowByte;
}

/**
  * @brief Get flash address low byte.
  * @param fmc FMC register base address.
  * @retval Address low byte.
  */
static inline unsigned int DCL_FMC_GetAddrLowByte(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_ADDRL);
}

/**
  * @brief Set dummy bytes.
  * @param fmc FMC register base address.
  * @param dummyBytes Dummy bytes.
  * @retval None.
  */
static inline void DCL_FMC_SetDummyBytes(FMC_RegStruct *fmc, FMC_DummyNum dummyBytes)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCDummyNum(dummyBytes));
    fmc->FMC_OP_CFG.BIT.dummy_num = dummyBytes;
}

/**
  * @brief Get dummy bytes.
  * @param fmc FMC register base address.
  * @param dummyBytes Dummy bytes.
  * @retval None.
  */
static inline unsigned char DCL_FMC_GetDummyBytes(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CFG.BIT.dummy_num);
}

/**
  * @brief Set flash address byte num.
  * @param fmc FMC register base address.
  * @param addrByteNum Address bytes num.
  * @retval None.
  */
static inline void DCL_FMC_SetAddrBytesNum(FMC_RegStruct *fmc, FMC_SPINorAddrByteNum  addrByteNum)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCSPINorAddrNum(addrByteNum));
    fmc->FMC_OP_CFG.BIT.addr_num = addrByteNum;
}

/**
  * @brief Get flash address byte num.
  * @param fmc FMC register base address.
  * @retval Address bytes num.
  */
static inline unsigned char DCL_FMC_GetAddrBytesNum(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CFG.BIT.addr_num);
}

/**
  * @brief Set flash interface type.
  * @param fmc FMC register base address.
  * @param ifType SPI flash interface type.
  * @retval None.
  */
static inline void DCL_FMC_SetMemIfType(FMC_RegStruct *fmc, FMC_SpiIfType ifType)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCSPIifType(ifType));
    fmc->FMC_OP_CFG.BIT.mem_if_type= ifType;
}

/**
  * @brief Get flash interface type.
  * @param fmc FMC register base address.
  * @retval SPI flash interface type.
  */
static inline unsigned char DCL_FMC_GetMemIfType(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CFG.BIT.mem_if_type);
}

/**
  * @brief Set cs force enable.
  * @param fmc FMC register base address.
  * @param enable ture is enable, false is disable.
  * @retval None.
  */
static inline void DCL_FMC_SetCsForceEnable(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP_CFG.BIT.force_cs_en = enable;
}

/**
  * @brief Get cs force enable state.
  * @param fmc FMC register base address.
  * @retval ture is enable, false is disable.
  */
static inline bool DCL_FMC_GetCsForceEnable(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CFG.BIT.force_cs_en);
}

/**
  * @brief Set cs select.
  * @param fmc FMC register base address.
  * @param csSel Selected cs.
  * @retval None.
  */
static inline void DCL_FMC_SetCsSelect(FMC_RegStruct *fmc, FMC_CsSelect csSel)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCCsSelect(csSel));
    fmc->FMC_OP_CFG.BIT.fm_cs = csSel;
}

/**
  * @brief Get cs selected.
  * @param fmc FMC register base address.
  * @retval SPI flash cs selected.
  */
static inline unsigned char DCL_FMC_GetCsSelected(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CFG.BIT.fm_cs);
}

/**
  * @brief Set oen multicycle enable.
  * @param fmc FMC register base address.
  * @param enable ture is enable, false is disable.
  * @retval None.
  */
static inline void DCL_FMC_SetOenMulticycle(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP_CFG.BIT.oen_multicycle = enable;
}

/**
  * @brief Get oen multicycle enable.
  * @param fmc FMC register base address.
  * @retval ture is enable, false is disable.
  */
static inline bool DCL_FMC_GetOenMulticycle(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CFG.BIT.oen_multicycle);
}

/**
  * @brief Set operation address of spi flash.
  * @param fmc FMC register base address.
  * @param addr The operation address of spi flash.
  * @retval None.
  */
static inline void DCL_FMC_SetSpiFlashAddr(FMC_RegStruct *fmc, unsigned int addr)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->SPI_OP_ADDR = addr;
}

/**
  * @brief Get operation address of spi flash.
  * @param fmc FMC register base address.
  * @retval address.
  */
static inline unsigned int DCL_FMC_GetSpiFlashAddr(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->SPI_OP_ADDR);
}

/**
  * @brief Set operation data num.
  * @param fmc FMC register base address.
  * @param dataNum The data number to operation.
  * @retval None.
  */
static inline void DCL_FMC_SetOpDataNum(FMC_RegStruct *fmc, unsigned short dataNum)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(dataNum <= FMC_TRANS_DATA_MAX_NUM);
    fmc->FMC_DATA_NUM.BIT.op_data_num = dataNum;
}

/**
  * @brief Get operation data num.
  * @param fmc FMC register base address.
  * @retval The operation data num.
  */
static inline unsigned short DCL_FMC_GetOpDataNum(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_DATA_NUM.BIT.op_data_num);
}

/**
  * @brief Set operation start.
  * @param fmc FMC register base address.
  * @retval None.
  */
static inline void DCL_FMC_SetOpStart(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP.BIT.reg_op_start = 0x1;
}

/**
  * @brief Get operation state.
  * @param fmc FMC register base address.
  * @retval 1 is busy, 0 is idle.
  */
static inline unsigned char DCL_FMC_GetOpStartState(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP.BIT.reg_op_start);
}

/**
  * @brief Set reading spi flash status enable.
  * @param fmc FMC register base address.
  * @param enable ture is enable, false is disable.
  * @retval None.
  */
static inline void DCL_FMC_SetReadSpiFlashStatusEnable(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP.BIT.read_status_en = enable;
}

/**
  * @brief Get the state of reading spi flash status enable.
  * @param fmc FMC register base address.
  * @retval ture is enable, false is disable.
  */
static inline bool DCL_FMC_GetReadSpiFlashStatusEnableState(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP.BIT.read_status_en);
}

/**
  * @brief Set reading data enable.
  * @param fmc FMC register base address.
  * @param enable ture is enable, false is disable.
  * @retval None.
  */
static inline void DCL_FMC_SetReadDataEnable(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP.BIT.read_data_en = enable;
}

/**
  * @brief Get the state of reading data enable.
  * @param fmc FMC register base address.
  * @retval ture is enable, false is disable.
  */
static inline bool DCL_FMC_GetReadDataEnableState(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP.BIT.read_data_en);
}

/**
  * @brief Set writing data enable.
  * @param fmc FMC register base address.
  * @param enable ture is enable, false is disable.
  * @retval None.
  */
static inline void DCL_FMC_SetWriteDataEnable(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP.BIT.write_data_en = enable;
}

/**
  * @brief Get the state of writing data enable.
  * @param fmc FMC register base address.
  * @retval ture is enable, false is disable.
  */
static inline bool DCL_FMC_GetWriteDataEnableState(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP.BIT.write_data_en);
}

/**
  * @brief Set writing address to flash enable.
  * @param fmc FMC register base address.
  * @param enable ture is enable, false is disable.
  * @retval None.
  */
static inline void DCL_FMC_SetWriteAddrEnable(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP.BIT.addr_en = enable;
}

/**
  * @brief Get the state of writing address to flash enable.
  * @param fmc FMC register base address.
  * @retval ture is enable, false is disable.
  */
static inline bool DCL_FMC_GetWriteAddrEnableState(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP.BIT.addr_en);
}

/**
  * @brief Set writing commond1 to flash enable.
  * @param fmc FMC register base address.
  * @param enable ture is enable, false is disable.
  * @retval None.
  */
static inline void DCL_FMC_SetWriteCmd1Enable(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP.BIT.cmd1_en = enable;
}

/**
  * @brief Get the state of writing commond1 to flash enable.
  * @param fmc FMC register base address.
  * @retval ture is enable, false is disable.
  */
static inline bool DCL_FMC_GetWriteCmd1EnableState(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP.BIT.cmd1_en);
}

/**
  * @brief Set transferring dummy bytes after wrote address enable.
  * @param fmc FMC register base address.
  * @param enable ture is enable, false is disable.
  * @retval None.
  */
static inline void DCL_FMC_SetTransDummyEnable(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP.BIT.dummy_en = enable;
}

/**
  * @brief Get the state of transferring dummy bytes after wrote address enable.
  * @param fmc FMC register base address.
  * @retval ture is enable, false is disable.
  */
static inline bool DCL_FMC_GetTransDummyEnableState(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP.BIT.dummy_en);
}

/**
  * @brief Set dma handling length.
  * @param fmc FMC register base address.
  * @param dataNum Dma handling length.
  * @retval None.
  */
static inline void DCL_FMC_SetDmaLength(FMC_RegStruct *fmc, unsigned int dataNum)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(dataNum <= FMC_DMA_MAX_LENGTH);
    fmc->FMC_DMA_LEN.BIT.dma_len = dataNum;
}

/**
  * @brief Get dma handling length.
  * @param fmc FMC register base address.
  * @retval Dma handling length.
  */
static inline unsigned int DCL_FMC_GetDmaLength(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_DMA_LEN.BIT.dma_len);
}

/**
  * @brief Set dma start address.
  * @param fmc FMC register base address.
  * @param addr The start address of dma handling.
  * @retval None.
  */
static inline void DCL_FMC_SetDmaStartAddr(FMC_RegStruct *fmc, unsigned int addr)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_DMA_SADDR_D0 = addr;
}

/**
  * @brief Get dma start address.
  * @param fmc FMC register base address.
  * @retval The start address of dma handling.
  */
static inline unsigned int DCL_FMC_GetDmaStartAddr(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_DMA_SADDR_D0);
}

/**
  * @brief Set dma operation start.
  * @param fmc FMC register base address.
  * @retval None.
  */
static inline void DCL_FMC_SetDmaOpStart(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP_CTRL.BIT.dma_op_ready = 0x1;
}

/**
  * @brief Get dma operation state.
  * @param fmc FMC register base address.
  * @retval 1 is busy, 0 is idle.
  */
static inline unsigned char DCL_FMC_GetDmaOpStartState(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CTRL.BIT.dma_op_ready);
}

/**
  * @brief Set dma operation type.
  * @param fmc FMC register base address.
  * @param dmaOpType dma operation type, write or read.
  * @retval None.
  */
static inline void DCL_FMC_SetDmaOpType(FMC_RegStruct *fmc, FMC_Dma_Op_Type dmaOpType)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(IsFMCDmaOpType(dmaOpType));
    fmc->FMC_OP_CTRL.BIT.rw_op = dmaOpType;
}

/**
  * @brief Get dma operation type.
  * @param fmc FMC register base address.
  * @retval dma operation type, write or read.
  */
static inline FMC_Dma_Op_Type DCL_FMC_GetDmaOpType(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CTRL.BIT.rw_op);
}

/**
  * @brief Set dma write code.
  * @param fmc FMC register base address.
  * @param code dma write code.
  * @retval None.
  */
static inline void DCL_FMC_SetDmaWriteCode(FMC_RegStruct *fmc, unsigned char code)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP_CTRL.BIT.wr_opcode = code;
}

/**
  * @brief Get dma write code.
  * @param fmc FMC register base address.
  * @retval dma write code.
  */
static inline unsigned char DCL_FMC_GetDmaWriteCode(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CTRL.BIT.wr_opcode);
}

/**
  * @brief Set dma read code.
  * @param fmc FMC register base address.
  * @param code dma read code.
  * @retval None.
  */
static inline void DCL_FMC_SetDmaReadCode(FMC_RegStruct *fmc, unsigned char code)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_OP_CTRL.BIT.rd_opcode = code;
}

/**
  * @brief Get dma read code.
  * @param fmc FMC register base address.
  * @retval dma read code.
  */
static inline unsigned char DCL_FMC_GetDmaReadCode(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_OP_CTRL.BIT.rd_opcode);
}

/**
  * @brief Set boot mode.
  * @param fmc FMC register base address.
  * @param code operation timeout.
  * @retval None.
  */
static inline void DCL_FMC_SetOpTimeout(FMC_RegStruct *fmc, unsigned int timeout)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    FMC_ASSERT_PARAM(timeout <= FMC_OP_MAX_TIMEOUT);
    fmc->FMC_TIMEOUT_WR.BIT.timeout_wr = timeout;
}

/**
  * @brief Get operation timeout.
  * @param fmc FMC register base address.
  * @retval operation timeout.
  */
static inline unsigned int DCL_FMC_GetOpTimeout(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_TIMEOUT_WR.BIT.timeout_wr);
}

/**
  * @brief Set low power mode enable.
  * @param fmc FMC register base address.
  * @param enable,ture is enable low power mode, false is close low power mode.
  * @retval None.
  */
static inline void DCL_FMC_SetLowPowerMode(FMC_RegStruct *fmc, bool enable)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    fmc->FMC_LP_CTRL.BIT.clk_gate_en = enable;
}

/**
  * @brief Get the stage of low power mode enable.
  * @param fmc FMC register base address.
  * @retval ture is enable low power mode, false is close low power mode.
  */
static inline bool DCL_FMC_GetLowPowerModeState(FMC_RegStruct *fmc)
{
    FMC_ASSERT_PARAM(IsFMCInstance(fmc));
    return (fmc->FMC_LP_CTRL.BIT.clk_gate_en);
}

/**
  * @}
  */

/**
  * @}
  */
#endif /* #ifndef McuMagicTag_FMC_IP_H */

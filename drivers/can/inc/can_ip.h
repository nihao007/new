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
  * @file    can_ip.h
  * @author  MCU Driver Team
  * @brief   CAN module driver.
  * @details This file provides DCL functions to manage CAN and Definition of
  *          specific parameters
  *           + Definition of CAN configuration parameters.
  *           + CAN register mapping structure.
  *           + Parameters check functions.
  *           + Direct configuration layer interface
  */

#ifndef McuMagicTag_CAN_IP_H
#define McuMagicTag_CAN_IP_H

#include "baseinc.h"

#ifdef CAN_PARAM_CHECK
#define CAN_ASSERT_PARAM BASE_FUNC_ASSERT_PARAM
#define CAN_PARAM_CHECK_NO_RET BASE_FUNC_PARAMCHECK_NO_RET
#define CAN_PARAM_CHECK_WITH_RET BASE_FUNC_PARAMCHECK_WITH_RET
#else
#define CAN_ASSERT_PARAM(para) ((void)0U)
#define CAN_PARAM_CHECK_NO_RET(para) ((void)0U)
#define CAN_PARAM_CHECK_WITH_RET(para, ret) ((void)0U)
#endif

/**
  * @addtogroup CAN
  * @{
  */

/**
  * @defgroup CAN_IP CAN_IP
  * @brief CAN_IP: can_v1.
  * @{
  */
/* Compatible with multiple chip models. */
#if defined (CHIP_3066HAPIDM) || defined (CHIP_3066HAPIVM) || defined (CHIP_3066HAPIMM)
#define CANFD_ENABLE
#endif

/**
  * @defgroup CAN_Param_Def CAN Parameters Definition
  * @brief Definition of CAN configuration parameters.
  * @{
  */
#define CAN_COM_RAM_ADDR       0x0403D000  /**< CAN controller common RAM base address */
#define CAN0_RAM_START_ADDR    0x0403D000  /**< CAN0 controller RAM start address */
#define CAN1_RAM_START_ADDR    0x0403E000  /**< CAN1 controller RAM start address */
#define CAN2_RAM_START_ADDR    0x0403F000  /**< CAN2 controller RAM start address */

/* Interrupt enable related mask */
#define RX_FIFO0_IT_MASK                          0xF
#define RX_FIFO1_IT_MASK                          0xF0
#define RX_BUFFER_IT_MASK                         0x80000
#define TX_FIFO_EMPTY_IT_MASK                     0x800
#define TX_COMPLETE_IT_MASK                       0x200
#define TX_CANCEL_IT_MASK                         0x400
#define HIGH_PRIORITY_IT_MASK                     0x100
#define TX_EVENT_FIFO_IT_MASK                     0xF000
#define TIMESTAMP_IT_MASK                         0x10000
#define TIMEOUT_IT_MASK                           0x40000
#define ERROR_IT_MASK                             0x3FF20000

/* Interrupt status mask */
#define RX_FIFO0_NEW_MESSAGE_IT_MASK                 0x1
#define RX_FIFO0_WATERMARK_REACHED_IT_MASK           0x2
#define RX_FIFO0_FULL_IT_MASK                        0x4
#define RX_FIFO0_MESSAGE_LOST_IT_MASK                0x8
#define RX_FIFO1_NEW_MESSAGE_IT_MASK                 0x10
#define RX_FIFO1_WATERMARK_REACHED_IT_MASK           0x20
#define RX_FIFO1_FULL_IT_MASK                        0x40
#define RX_FIFO1_MESSAGE_LOST_IT_MASK                0x80
#define HIGH_PRIORITY_MESSAGE_IT_MASK                0x100
#define TRANSMISSION_COMPLETED_IT_MASK               0x200
#define TRANSMISSION_CANCELLATION_FINISHED_IT_MASK   0x400
#define TX_FIFO_EMPTY_IT_MASK                        0x800
#define TX_FIFO_NEW_ENTRY_IT_MASK                    0x1000
#define TX_EVENT_FIFO_WATERMARK_REACHED_IT_MASK      0x2000
#define TX_EVENT_FIFO_FULL_IT_MASK                   0x4000
#define TX_EVENT_FIFO_ELEMENT_LOST_IT_MASK           0x8000
#define TIMESTAMP_WRAPAROUND_IT_MASK                 0x10000
#define MESSAGE_RAM_ACCESS_FAILURE_IT_MASK           0x20000
#define TIMEOUT_OCCURRED_IT_MASK                     0x40000
#define MESSAGE_STORE_TO_DEDICATED_RX_BUFFER_IT_MASK 0x80000
#define BIT_ERROR_CORRECTED_IT_MASK                  0x100000
#define BIT_ERROR_UNCORRECTED_IT_MASK                0x200000
#define ERROR_LOGGING_OVERFLOW_IT_MASK               0x400000
#define ERROR_PASSIVE_IT_MASK                        0x800000
#define WARNING_STATUS_IT_MASK                       0x1000000
#define BUS_OFF_IT_MASK                              0x2000000
#define WATCHDOG_INTERRUPT_IT_MASK                   0x4000000
#define PROTOCOL_ERROR_IN_ARBITRATION_PHASE_IT_MASK  0x8000000
#define PROTOCOL_ERROR_IN_DATA_PHASE_IT_MASK         0x10000000
#define ACCESS_TO_RESERVED_ADDRESS_IT_MASK           0x20000000

#define CAN_IR_RF0N_MASK       0x01  /**< Receive FIFO 0 empty interrupt mask */
#define CAN_IR_RF0N_POS        0     /**< Position of Receive FIFO 0 empty interrupt bit */
#define CAN_IR_RF0W_MASK       0x02  /**< Receive FIFO 0 warning interrupt mask */
#define CAN_IR_RF0W_POS        1     /**< Position of Receive FIFO 0 warning interrupt bit */
#define CAN_IR_RF0F_MASK       0x04  /**< Receive FIFO 0 full interrupt mask */
#define CAN_IR_RF0F_POS        2     /**< Position of Receive FIFO 0 full interrupt bit */
#define CAN_IR_RF0L_MASK       0x08  /**< Receive FIFO 0 message lost interrupt mask */
#define CAN_IR_RF0L_POS        3     /**< Position of Receive FIFO 0 message lost interrupt bit */

#define CAN_IR_RF1N_MASK       0x10  /**< Receive FIFO 1 empty interrupt mask */
#define CAN_IR_RF1N_POS        4     /**< Position of Receive FIFO 1 empty interrupt bit */
#define CAN_IR_RF1W_MASK       0x20  /**< Receive FIFO 1 warning interrupt mask */
#define CAN_IR_RF1W_POS        5     /**< Position of Receive FIFO 1 warning interrupt bit */
#define CAN_IR_RF1F_MASK       0x40  /**< Receive FIFO 1 full interrupt mask */
#define CAN_IR_RF1F_POS        6     /**< Position of Receive FIFO 1 full interrupt bit */
#define CAN_IR_RF1L_MASK       0x80  /**< Receive FIFO 1 message lost interrupt mask */
#define CAN_IR_RF1L_POS        7     /**< Position of Receive FIFO 1 message lost interrupt bit */

#define CAN_IR_HPM_MASK        0x100  /**< High-priority message interrupt mask */
#define CAN_IR_HPM_POS         8     /**< Position of High-priority message interrupt bit */
#define CAN_IR_TC_MASK         0x200  /**< Transmission completed interrupt mask */
#define CAN_IR_TC_POS          9     /**< Position of Transmission completed interrupt bit */
#define CAN_IR_TCF_MASK        0x400  /**< Transmission completed frame interrupt mask */
#define CAN_IR_TCF_POS         10    /**< Position of Transmission completed frame interrupt bit */
#define CAN_IR_TFE_MASK        0x800  /**< Transmit FIFO empty interrupt mask */
#define CAN_IR_TFE_POS         11    /**< Position of Transmit FIFO empty interrupt bit */

#define CAN_IR_TEFN_MASK       0x1000  /**< Transmit FIFO empty new message interrupt mask */
#define CAN_IR_TEFN_POS        12     /**< Position of Transmit FIFO empty new message interrupt bit */
#define CAN_IR_TEFW_MASK       0x2000  /**< Transmit FIFO warning interrupt mask */
#define CAN_IR_TEFW_POS        13     /**< Position of Transmit FIFO warning interrupt bit */
#define CAN_IR_TEFF_MASK       0x4000  /**< Transmit FIFO full interrupt mask */
#define CAN_IR_TEFF_POS        14     /**< Position of Transmit FIFO full interrupt bit */
#define CAN_IR_TEFL_MASK       0x8000  /**< Transmit FIFO message lost interrupt mask */
#define CAN_IR_TEFL_POS        15     /**< Position of Transmit FIFO message lost interrupt bit */

#define CAN_IR_TSW_MASK        0x10000  /**< Transmit pause warning interrupt mask */
#define CAN_IR_TSW_POS         16     /**< Position of Transmit pause warning interrupt bit */
#define CAN_IR_MRAF_MASK       0x20000  /**< Message RAM access failure interrupt mask */
#define CAN_IR_MRAF_POS        17     /**< Position of Message RAM access failure interrupt bit */
#define CAN_IR_TOO_MASK        0x40000  /**< Timeout occurred interrupt mask */
#define CAN_IR_TOO_POS         18     /**< Position of Timeout occurred interrupt bit */
#define CAN_IR_DRX_MASK        0x80000  /**< Data transmission conflict interrupt mask */
#define CAN_IR_DRX_POS         19     /**< Position of Data transmission conflict interrupt bit */

#define CAN_IR_BEC_MASK        0x100000  /**< Bus error corrupt interrupt mask */
#define CAN_IR_BEC_POS         20     /**< Position of Bus error corrupt interrupt bit */
#define CAN_IR_BEU_MASK        0x200000  /**< Bus error uncorrectable interrupt mask */
#define CAN_IR_BEU_POS         21     /**< Position of Bus error uncorrectable interrupt bit */
#define CAN_IR_ELO_MASK        0x400000  /**< Error logging overflow interrupt mask */
#define CAN_IR_ELO_POS         22     /**< Position of Error logging overflow interrupt bit */
#define CAN_IR_EP_MASK         0x800000  /**< Error passive interrupt mask */
#define CAN_IR_EP_POS          23     /**< Position of Error passive interrupt bit */

#define CAN_IR_EW_MASK         0x1000000  /**< Error warning interrupt mask */
#define CAN_IR_EW_POS          24     /**< Position of Error warning interrupt bit */
#define CAN_IR_BO_MASK         0x2000000  /**< Bus off interrupt mask */
#define CAN_IR_BO_POS          25     /**< Position of Bus off interrupt bit */
#define CAN_IR_WDI_MASK        0x4000000  /**< Watchdog interrupt mask */
#define CAN_IR_WDI_POS         26     /**< Position of Watchdog interrupt bit */
#define CAN_IR_PEA_MASK        0x8000000  /**< Protocol error active interrupt mask */
#define CAN_IR_PEA_POS         27     /**< Position of Protocol error active interrupt bit */

#define CAN_IR_PED_MASK        0x10000000  /**< Protocol error detected interrupt mask */
#define CAN_IR_PED_POS         28     /**< Position of Protocol error detected interrupt bit */
#define CAN_IR_ARA_MASK        0x20000000  /**< Arbitration lost interrupt mask */
#define CAN_IR_ARA_POS         29     /**< Position of Arbitration lost interrupt bit */

#define BIT_WDC_MASK             0xF
#define BIT_TXBNS_MASK           0x1F
#define TEST_TX_BIT_MASK         0x03
#define TEST_TXBNP_BIT_MASK      0x1F
#define RWD_BIT_WDV_MASK         0xFF
#define TSCC_BIT_TSS_MASK        0x03
#define TSCC_BIT_TCP_MASK        0x0F

#define DATA_TIMING_SEG2_OFFSET  4
#define DATA_TIMING_SEG1_OFFSET  8
#define DATA_TIMING_BRP_OFFSET   16
#define DATA_TIMING_TDC_MASK     0x800000

#define NOMINAL_TIMING_SEG1_OFFSET   8
#define NOMINAL_TIMING_BRP_OFFSET    16
#define NOMINAL_TIMING_SJW_OFFSET    25

#define TX_EVENT_FIFO_XTD_OFFSET                  30
#define TX_EVENT_FIFO_XTD_MASK                    0x01
#define TX_EVENT_FIFO_RTR_OFFSET                  29
#define TX_EVENT_FIFO_RTR_MASK                    0x01
#define TX_EVENT_FIFO_IDS_OFFSET                  18
#define TX_EVENT_FIFO_IDS_MASK                    0x1FFC0000
#define TX_EVENT_FIFO_IDE_OFFSET                  0
#define TX_EVENT_FIFO_IDE_MASK                    0x1FFFFFFF

#define TX_EVENT_FIFO_MML_OFFSET                  24
#define TX_EVENT_FIFO_MML_MASK                    0xFF
#define TX_EVENT_FIFO_MMH_OFFSET                  8
#define TX_EVENT_FIFO_MMH_MASK                    0xFF
#define TX_EVENT_FIFO_ET_OFFSET                   22
#define TX_EVENT_FIFO_ET_MASK                     0x03
#define TX_EVENT_FIFO_FD_OFFSET                   21
#define TX_EVENT_FIFO_FD_MASK                     0x1
#define TX_EVENT_FIFO_BRS_OFFSET                  20
#define TX_EVENT_FIFO_BRS_MASK                    0x1
#define TX_EVENT_FIFO_DLC_OFFSET                  16
#define TX_EVENT_FIFO_DLC_MASK                    0x0F
#define TX_EVENT_FIFO_TXTS_OFFSET                 0
#define TX_EVENT_FIFO_TXTS_MASK                   0xFFFF
#define TX_EVENT_FIFO_TXTSC_OFFSET                4
#define TX_EVENT_FIFO_TXTSC_MASK                  0x1
#define TX_EVENT_FIFO_TXTSP_OFFSET                0
#define TX_EVENT_FIFO_TXTSP_MASK                  0xF

#define RX_ELEMENT_XTD_OFFSET                     30
#define RX_ELEMENT_XTD_MASK                       0x01
#define RX_ELEMENT_RTR_OFFSET                     29
#define RX_ELEMENT_RTR_MASK                       0x01
#define RX_ELEMENT_EID_OFFSET                      0
#define RX_ELEMENT_EID_MASK                        0x1FFFFFFF
#define RX_ELEMENT_SID_OFFSET                      18
#define RX_ELEMENT_SID_MASK                        0x7FF

#define RX_ELEMENT_ANMF_OFFSET                    31
#define RX_ELEMENT_ANMF_MASK                      0x01
#define RX_ELEMENT_FIDX_OFFSET                    24
#define RX_ELEMENT_FIDX_MASK                      0x7F
#define RX_ELEMENT_FDF_OFFSET                     21
#define RX_ELEMENT_FDF_MASK                       0x1
#define RX_ELEMENT_BRS_OFFSET                     20
#define RX_ELEMENT_BRS_MASK                       0x1
#define RX_ELEMENT_DLC_OFFSET                     16
#define RX_ELEMENT_DLC_MASK                       0xF
#define RX_ELEMENT_RXTS_OFFSET                    0
#define RX_ELEMENT_RXTS_MASK                      0xFFFF
#define RX_ELEMENT_TSC_OFFSET                     4
#define RX_ELEMENT_TSC_MASK                       0x01
#define RX_ELEMENT_RXTSP_OFFSET                   0
#define RX_ELEMENT_RXTSP_MASK                     0xF

#define TX_ELEMENT_XTD_OFFSET                     30
#define TX_ELEMENT_XTD_MASK                       0x01
#define TX_ELEMENT_RTR_OFFSET                     29
#define TX_ELEMENT_RTR_MASK                       0x01
#define TX_ELEMENT_FIFO_IDS_OFFSET                18
#define TX_ELEMENT_FIFO_IDS_MASK                  0x1FFC0000
#define TX_ELEMENT_FIFO_IDE_OFFSET                0
#define TX_ELEMENT_FIFO_IDE_MASK                  0x1FFFFFFF

#define TX_ELEMENT_FIFO_MML_OFFSET                24
#define TX_ELEMENT_FIFO_MML_MASK                  0xFF
#define TX_ELEMENT_FIFO_MMH_OFFSET                8
#define TX_ELEMENT_FIFO_MMH_MASK                  0xFF00
#define TX_ELEMENT_FIFO_EFC_OFFSET                23
#define TX_ELEMENT_FIFO_EFC_MASK                  0x01
#define TX_ELEMENT_FIFO_TSCE_OFFSET               22
#define TX_ELEMENT_FIFO_TSCE_MASK                 0x01
#define TX_ELEMENT_FDF_TSCE_OFFSET                21
#define TX_ELEMENT_FDF_TSCE_MASK                  0x01
#define TX_ELEMENT_BRS_TSCE_OFFSET                20
#define TX_ELEMENT_BRS_TSCE_MASK                  0x01
#define TX_ELEMENT_DCL_TSCE_OFFSET                16
#define TX_ELEMENT_DCL_TSCE_MASK                  0xF

#define RAM_ADDR_RESERVE_BIT                      2

#define TX_EVENT_FIFO_WATERMARK                   32
#define RX_FIFO0_WATERMARK                        64
#define RX_FIFO1_WATERMARK                        64

#define RX_BUFFER_FIFO_ELEMENT_HEADER_SIZE        8
#define TX_BUFFER_ELEMENT_HEADER_SIZE             8
#define TX_EVENT_FIFO_ELEMENT_SIZE                8
#define STANDARD_MESSAGE_ID_FILTER_ELEMENT_SIZE   4
#define EXTENDED_MESSAGE_ID_FILTER_ELEMENT_SIZE   8

#define UNSIGNED_INT_BIT_WIDTH     32
#define CAN_RX_BUFFERS_SIZE_MAX    8

#define SIDFC_RAM_SIZE_MAX         128
#define XIDFC_RAM_SIZE_MAX         64
#define RX_FIFO_0_SIZE_MAX         64
#define RX_FIFO_1_SIZE_MAX         64
#define RX_BUFFERS_SIZE_MAX        64
#define TX_EVENT_FIFO_SIZE_MAX     32
#define TX_BUFFERS_SIZE_MAX        32

#define CANFD_ENABLE
/**
  * @brief Type ID of the callback function registered by the user.
  */
typedef enum {
    CAN_RX_FIFO_0             = 0x00000000U,
    CAN_RX_FIFO_1             = 0x00000001U,
    CAN_RX_BUFFER             = 0x00000002U,
    CAN_TX_FIFO_EMPTY         = 0x00000003U,
    CAN_TX_COMPLETE           = 0x00000004U,
    CAN_TX_CANCEL             = 0x00000005U,
    CAN_HIGH_PRIORITY_MSG     = 0x00000006U,
    CAN_TX_EVENT_FIFO         = 0x00000007U,
    CAN_TIMESTAMP             = 0x00000008U,
    CAN_TIMEOUT               = 0x00000009U,
    CAN_ERROR                 = 0x0000000AU,
} CAN_CallBackFunType;

/**
  * @brief CAN state type.
  */
typedef enum {
    CAN_STATE_NONE_INIT = 0x00000000U,  /**< CAN state: Not initialized */
    CAN_STATE_READY     = 0x00000001U,  /**< CAN state: Ready to operate */
    CAN_STATE_BUSY      = 0x00000002U,  /**< CAN state: Busy (processing) */
    CAN_STATE_ERROR     = 0x00000003U,  /**< CAN state: Error occurred */
} CAN_StateType;

/**
  * @brief CAN frame format type.
  */
typedef enum {
    CAN_FRAME_CLASSIC         = 0x00000000U,  /**< Classic CAN frame format */
#ifdef CANFD_ENABLE
    CAN_FRAME_FD_NO_BRS       = 0x00000001U,  /**< CAN FD frame format without BRS (Bit Rate Switching) */
    CAN_FRAME_FD_BRS          = 0x00000010U,  /**< CAN FD frame format with BRS (Bit Rate Switching) */
#endif
} CAN_FrameFormat;

/**
  * @brief CAN work mode type.
  */
typedef enum {
    WORK_MODE_NORMAL                     = 0x00000000U,  /**< Normal operation mode */
    WORK_MODE_RESTRICTED_OPERATION       = 0x00000001U,  /**< Restricted operation mode */
    WORK_MODE_BUS_MONITORING             = 0x00000002U,  /**< Bus monitoring mode */
    WORK_MODE_INTERNAL_LOOPBACK_TEST     = 0x00000003U,  /**< Internal loopback test mode */
    WORK_MODE_EXTERNAL_LOOPBACK_TEST     = 0x00000004U,  /**< External loopback test mode */
} CAN_WorkMode;

/**
  * @brief CAN auto retransmission configuration type.
  */
typedef enum {
    AUTO_RETRANSMISSION_ENABLE        = 0x00000000U,  /**< Auto retransmission is enabled */
    AUTO_RETRANSMISSION_DISABLE       = 0x00000001U,  /**< Auto retransmission is disabled */
} CAN_AutoRetransmission;

/**
  * @brief CAN transmit pause configuration type.
  */
typedef enum {
    TRANSMIT_PAUSE_DISABLE      = 0x00000000U,  /**< Transmit pause is disabled */
    TRANSMIT_PAUSE_ENABLE       = 0x00000001U,  /**< Transmit pause is enabled */
} CAN_TransmitPause;

/**
  * @brief CAN protocol exception configuration type.
  */
typedef enum {
    PROTOCOL_EXECPTION_DISABLE      = 0x00000000U,  /**< Protocol exception is disabled (no exception handling) */
    PROTOCOL_EXECPTION_ENABLE       = 0x00000001U,  /**< Protocol exception is enabled (handle protocol errors) */
} CAN_ProtocolException;

/**
  * @brief CAN timestamp mode type.
  */
typedef enum {
    TIMESTAMP_COUNT_ALWAYS_0              = 0x00000000U,  /**< Timestamp counter is always 0 (disabled) */
    TIMESTAMP_COUNT_INCREMENT_ACCORDE_TCP = 0x00000001U,  /**< Timestamp counter increments according to TCP */
    TIMESTAMP_COUNT_USE_EXTERNAL          = 0x00000003U,  /**< Timestamp counter uses external time source */
} CAN_TimestampMode;

/**
  * @brief CAN wide message marker type.
  */
typedef enum {
    WIDE_MESSAGE_MARKER_8BIT              = 0x00000000U,  /**< Wide message marker is 8-bit wide */
    WIDE_MESSAGE_MARKER_16BIT             = 0x00000001U,  /**< Wide message marker is 16-bit wide */
} CAN_WideMessageMarker;

/**
  * @brief CAN frame format type.
  */
typedef enum {
    CAN_FRAME_FORMAT_ISO               = 0x00000000U,  /**< Frame format follows ISO standard */
    CAN_FRAME_FORMAT_BOSCH             = 0x00000001U,
} CAN_NonISOOperation;

/**
  * @brief CAN edge filter mode type.
  */
typedef enum {
    CAN_EDGE_FILTER_DISABLE               = 0x00000000U,  /**< Edge filter is disabled */
    CAN_EDGE_FILTER_ENABLE             = 0x00000001U,  /**< Edge filter is enabled */
} CAN_EdgeFilterMode;

/**
  * @brief CAN timeout mode type.
  */
typedef enum {
    TIMEOUT_CONTINUOUS_OPERATION              = 0x00000000U,  /**< Timeout is in continuous operation mode */
    TIMEOUT_CONTROLLED_BY_TX_EVENT_FIFO       = 0x00000001U,  /**< Timeout is controlled by the transmit event FIFO */
    TIMEOUT_CONTROLLED_BY_RX_FIFO_0           = 0x00000002U,  /**< Timeout is controlled by the receive FIFO 0 */
    TIMEOUT_CONTROLLED_BY_RX_FIFO_1           = 0x00000003U,  /**< Timeout is controlled by the receive FIFO 1 */
} CAN_TimeoutMode;

/**
  * @brief CAN reject remote frames with ID configuration type.
  */
typedef enum {
    FILTER_FRAME_WITH_ID           = 0x00000000U,  /**< Accept frames with ID */
    REJECY_ALL_FRAME_WITH_ID       = 0x00000001U,  /**< Reject all frames with ID */
} CAN_RejectRemoteFrames;

/**
  * @brief CAN accept non-matching frames mode type.
  */
typedef enum {
    ACCEPT_IN_RX_FIFO_0           = 0x00000000U,  /**< Accept non-matching frames in receive FIFO 0 */
    ACCEPT_IN_RX_FIFO_1           = 0x00000001U,  /**< Accept non-matching frames in receive FIFO 1 */
    RETECT_FRAME                  = 0x00000002U,  /**< Reject non-matching frames (frames are not accepted) */
} CAN_AcpNonMatchFrameMode;

/**
  * @brief CAN transmit FIFO or queue mode type.
  */
typedef enum {
    TX_FIFO_MODE = 0x00000000U,  /**< Transmit using FIFO mode (FIFO operation) */
    TX_QUEUE_MODE = 0x00000001U,  /**< Transmit using queue mode (queue operation) */
} CAN_TxFifoQueueMode;

/**
  * @brief CAN receive FIFO operation mode type.
  */
typedef enum {
    RX_FIFO_BLOCKING_MODE  = 0x00000000U,  /**< Receive FIFO operates in blocking mode */
    RX_FIFO_OVERWRITE_MODE = 0x00000001U,  /**< Receive FIFO operates in overwrite mode */
} CAN_RxFifoOperationMode;

/**
  * @brief CAN filter type configuration.
  */
typedef enum {
    CAN_FILTER_RANGE         = 0x00000000U,  /**< Range filter type */
    CAN_FILTER_DUAL          = 0x00000001U,  /**< Dual filter type */
    CAN_FILTER_MASK          = 0x00000002U,  /**< Mask filter type */
    CAN_FILTER_RANGE_NO_EIDM = 0x00000003U,  /**< Range filter type without extended ID masking */
} CAN_FilterType;

/**
  * @brief CAN filter configuration type.
  */
typedef enum {
    CAN_FILTER_DISABLE       = 0x00000000U,  /**< Filter is disabled */
    CAN_FILTER_TO_RXFIFO0    = 0x00000001U,  /**< Filter directs messages to Receive FIFO 0 */
    CAN_FILTER_TO_RXFIFO1    = 0x00000002U,  /**< Filter directs messages to Receive FIFO 1 */
    CAN_FILTER_REJECT        = 0x00000003U,  /**< Filter rejects messages */
    CAN_FILTER_HP            = 0x00000004U,  /**< High-priority filter */
    CAN_FILTER_TO_RXFIFO0_HP = 0x00000005U,  /**< High-priority filter directs messages to Receive FIFO 0 */
    CAN_FILTER_TO_RXFIFO1_HP = 0x00000006U,  /**< High-priority filter directs messages to Receive FIFO 1 */
} CAN_FilterConfig;

/**
  * @brief CAN identifier type.
  */
typedef enum {
    CAN_ID_STANDARD       = 0x00000000U,  /**< Standard CAN identifier (11-bit) */
    CAN_ID_EXTENDED       = 0x00000001U,  /**< Extended CAN identifier (29-bit) */
} CAN_IDType;

/**
  * @brief CAN message transmission mode.
  */
typedef enum {
    CAN_SEND_MSG_BY_TX_FIFOQ       = 0x00000000U,  /**< Send message using transmit FIFO/Queue */
    CAN_SEND_MSG_BY_TX_BUFFER      = 0x00000001U,  /**< Send message using transmit buffer */
} CAN_SendMsgMode;

/**
  * @brief CAN receive message location.
  */
typedef enum {
    READ_FROM_RX_FIFO0       = 0x00000000U,  /**< Read message from Receive FIFO 0 */
    READ_FROM_RX_FIFO1       = 0x00000001U,  /**< Read message from Receive FIFO 1 */
    READ_FROM_RX_BUFFER      = 0x00000002U,  /**< Read message from Receive Buffer */
} CAN_RxLocation;

/**
  * @brief CAN frame type enumeration.
  */
typedef enum {
    CAN_FRAME_DATA         = 0x00000000U,  /**< Data frame */
    CAN_FRAME_REMOTE       = 0x00000001U,  /**< Remote frame */
} CAN_FrameTypeEnum;

/**
  * @brief CAN event FIFO control configuration.
  */
typedef enum {
    STORE_TX_EVENTS_DISABLE = 0x00000000U,  /**< Disable storing of transmit events */
    STORE_TX_EVENTS_ENABLE  = 0x00000001U,  /**< Enable storing of transmit events */
} CAN_EventFIFOControl;

/**
  * @brief CAN timestamp capture configuration.
  */
typedef enum {
    TIME_STAMP_CAPTURE_DISABLE = 0x00000000U,  /**< Disable timestamp capture */
    TIME_STAMP_CAPTURE_ENABLE  = 0x00000001U,  /**< Enable timestamp capture */
} CAN_TimeStampCaptureControl;

/**
  * @brief CAN frame format type.
  */
typedef enum {
    CLASSIC_CAN_FORMAT               = 0x00000000U,  /**< Classic CAN frame format */
    NO_CLASSIC_CAN_FORMAT            = 0x00000001U,  /**< No Classic CAN frame format */
} CAN_Format;

/**
  * @brief CAN bit rate switching state.
  */
typedef enum {
    BIT_RATE_SWITCHING_DISABLE = 0x00000000U,  /**< Bit rate switching is disabled */
    BIT_RATE_SWITCHING_ENABLE  = 0x00000001U,  /**< Bit rate switching is enabled */
} CAN_BrsState;

/**
  * @brief CAN DLC (Data Length Code) bytes enumeration.
  */
typedef enum {
    COM_CAN_DLC_BYTES_0 = 0,  /**< DLC = 0 bytes */
    COM_CAN_DLC_BYTES_1 = 1,  /**< DLC = 1 byte */
    COM_CAN_DLC_BYTES_2 = 2,  /**< DLC = 2 bytes */
    COM_CAN_DLC_BYTES_3 = 3,  /**< DLC = 3 bytes */
    COM_CAN_DLC_BYTES_4 = 4,  /**< DLC = 4 bytes */
    COM_CAN_DLC_BYTES_5 = 5,  /**< DLC = 5 bytes */
    COM_CAN_DLC_BYTES_6 = 6,  /**< DLC = 6 bytes */
    COM_CAN_DLC_BYTES_7 = 7,  /**< DLC = 7 bytes */
    COM_CAN_DLC_BYTES_8  = 8,    /**< DLC = 8 bytes */
    COM_CAN_DLC_BYTES_12 = 12,   /**< DLC = 12 bytes */
    COM_CAN_DLC_BYTES_16 = 16,   /**< DLC = 16 bytes */
    COM_CAN_DLC_BYTES_20 = 20,   /**< DLC = 20 bytes */
    COM_CAN_DLC_BYTES_24 = 24,   /**< DLC = 24 bytes */
    COM_CAN_DLC_BYTES_32 = 32,   /**< DLC = 32 bytes */
    COM_CAN_DLC_BYTES_48 = 48,   /**< DLC = 48 bytes */
    COM_CAN_DLC_BYTES_64 = 64,   /**< DLC = 64 bytes */
} CAN_DlcBytes;

/**
  * @brief CAN transmit event type enumeration.
  */
typedef enum {
    TX_EVENT_TYPE       = 0x00000001U,  /**< Normal transmit event type */
    TX_EVENT_IN_CANCEL  = 0x00000002U,  /**< Transmit event type during cancellation */
} CAN_TxEventType;

/**
  * @brief CAN high-priority message storage configuration.
  */
typedef enum {
    HP_MSG_STORE_NO_FIFO_SELECT       = 0x00000000U,  /**< No FIFO selected for high-priority messages */
    HP_MSG_STORE_FIFO_MSG_LOST        = 0x00000001U,  /**< High-priority message storage results in message loss */
    HP_MSG_STORE_FIFO_1               = 0x00000002U,  /**< High-priority messages stored in FIFO 1 */
    HP_MSG_STORE_FIFO_0               = 0x00000003U,  /**< High-priority messages stored in FIFO 0 */
} CAN_HpMsgStorage;

/**
  * @brief CAN communication state enumeration.
  */
typedef enum {
    CAN_STATE_SYNC = 0x00000000U,  /**< Communication state: Synchronized */
    CAN_STATE_IDLE = 0x00000001U,  /**< Communication state: Idle */
    CAN_STATE_RX   = 0x00000002U,  /**< Communication state: Receiving */
    CAN_STATE_TX   = 0x00000003U,  /**< Communication state: Transmitting */
} CAN_CommunicationState;

/**
  * @brief CAN communication error code enumeration.
  */
typedef enum {
    CAN_PROTOCOL_ERROR_NONE    = 0x00000000U,  /**< No protocol error */
    CAN_PROTOCOL_ERROR_STUFF   = 0x00000001U,  /**< Stuff error */
    CAN_PROTOCOL_ERROR_FORM    = 0x00000002U,  /**< Form error */
    CAN_PROTOCOL_ERROR_ACK     = 0x00000003U,  /**< Acknowledgment error */
    CAN_PROTOCOL_ERROR_BIT1    = 0x00000004U,  /**< Bit 1 error */
    CAN_PROTOCOL_ERROR_BIT0    = 0x00000005U,  /**< Bit 0 error */
    CAN_PROTOCOL_ERROR_CRC     = 0x00000006U,  /**< CRC error */
    CAN_PROTOCOL_ERROR_NO_CHANGE = 0x00000007U,  /**< No change error */
} CAN_CommunicationErrorCode;

/**
  * @brief CAN FIFO type enumeration.
  */
typedef enum {
    RX_FIFO0_FULL           = 0x00,  /**< Receive FIFO 0 is full */
    RX_FIFO0_FILL_LEVEL     = 0x01,  /**< Receive FIFO 0 fill level */
    RX_FIFO1_FULL           = 0x02,  /**< Receive FIFO 1 is full */
    RX_FIFO1_FILL_LEVEL     = 0x03,  /**< Receive FIFO 1 fill level */
    TX_FIFOQ_FULL           = 0x04,  /**< Transmit FIFO/Queue is full */
    TX_FIFOQ_FREE_LEVEL     = 0x05,  /**< Transmit FIFO/Queue free level */
    TX_EFIFO_FULL           = 0x06,  /**< Transmit event FIFO is full */
    TX_EFIFO_FREE_LEVEL     = 0x07,  /**< Transmit event FIFO free level */
} CAN_FifoType;

/**
  * @brief CAN data size enumeration.
  */
typedef enum {
    DATA_SIZE_8_BYTE     = 8,   /**< Data size of 8 bytes */
    DATA_SIZE_12_BYTE    = 12,  /**< Data size of 12 bytes */
    DATA_SIZE_16_BYTE    = 16,  /**< Data size of 16 bytes */
    DATA_SIZE_20_BYTE    = 20,  /**< Data size of 20 bytes */
    DATA_SIZE_24_BYTE    = 24,  /**< Data size of 24 bytes */
    DATA_SIZE_32_BYTE    = 32,  /**< Data size of 32 bytes */
    DATA_SIZE_48_BYTE    = 48,  /**< Data size of 48 bytes */
    DATA_SIZE_64_BYTE    = 64,  /**< Data size of 64 bytes */
} CAN_DataSize;

/**
  * @brief CAN Rx fifo mode enumeration.
  */
typedef enum {
    FIFO_BLOCKING_MODE   = 0,
    FIFO_OVERWRITE_MODE  = 1,
} CAN_RxFifoMode;

/**
  * @brief CAN IT line enable enumeration.
  */
typedef enum {
    LINE0_INTERRUPT_ENABLE   = 1,
    LINE1_INTERRUPT_ENABLE   = 2,
    BOTH_LINE_INTERRUPT_ENABLE   = 3,
} CAN_LineItEnable;

/**
  * @brief CAN cancel tx request index enumeration.
  */
typedef enum {
    CANCEL_TX_REQUEST_INVALID = 0x00000000,
    CANCEL_TX_REQUEST_INDEX_0  = 0x00000001,
    CANCEL_TX_REQUEST_INDEX_1  = 0x00000002,
    CANCEL_TX_REQUEST_INDEX_2  = 0x00000004,
    CANCEL_TX_REQUEST_INDEX_3  = 0x00000008,
    CANCEL_TX_REQUEST_INDEX_4  = 0x00000010,
    CANCEL_TX_REQUEST_INDEX_5  = 0x00000020,
    CANCEL_TX_REQUEST_INDEX_6  = 0x00000040,
    CANCEL_TX_REQUEST_INDEX_7  = 0x00000080,
    CANCEL_TX_REQUEST_INDEX_8  = 0x00000100,
    CANCEL_TX_REQUEST_INDEX_9  = 0x00000200,
    CANCEL_TX_REQUEST_INDEX_10 = 0x00000400,
    CANCEL_TX_REQUEST_INDEX_11 = 0x00000800,
    CANCEL_TX_REQUEST_INDEX_12 = 0x00001000,
    CANCEL_TX_REQUEST_INDEX_13 = 0x00002000,
    CANCEL_TX_REQUEST_INDEX_14 = 0x00004000,
    CANCEL_TX_REQUEST_INDEX_15 = 0x00008000,
    CANCEL_TX_REQUEST_INDEX_16 = 0x00010000,
    CANCEL_TX_REQUEST_INDEX_17 = 0x00020000,
    CANCEL_TX_REQUEST_INDEX_18 = 0x00040000,
    CANCEL_TX_REQUEST_INDEX_19 = 0x00080000,
    CANCEL_TX_REQUEST_INDEX_20 = 0x00100000,
    CANCEL_TX_REQUEST_INDEX_21 = 0x00200000,
    CANCEL_TX_REQUEST_INDEX_22 = 0x00400000,
    CANCEL_TX_REQUEST_INDEX_23 = 0x00800000,
    CANCEL_TX_REQUEST_INDEX_24 = 0x01000000,
    CANCEL_TX_REQUEST_INDEX_25 = 0x02000000,
    CANCEL_TX_REQUEST_INDEX_26 = 0x04000000,
    CANCEL_TX_REQUEST_INDEX_27 = 0x08000000,
    CANCEL_TX_REQUEST_INDEX_28 = 0x10000000,
    CANCEL_TX_REQUEST_INDEX_29 = 0x20000000,
    CANCEL_TX_REQUEST_INDEX_30 = 0x40000000,
    CANCEL_TX_REQUEST_INDEX_31 = 0x80000000
} CAN_CancelTxRequestIndices;

#ifdef CANFD_ENABLE
/**
  * @brief Bit timing parameters for data phase in CAN communication.
  */
typedef struct {
    unsigned int dsjw : 4;    /**< Data phase synchronization jump width (4 bits) */
    unsigned int dtseg1 : 5;  /**< Data phase segment 1 (Tseg1) timing (5 bits) */
    unsigned int dtseg2 : 4;  /**< Data phase segment 2 (Tseg2) timing (4 bits) */
    unsigned int dbrp : 5;    /**< Data phase bit rate prescaler (5 bits) */
} CAN_DataBitTiming;
#endif

/**
  * @brief Bit timing parameters for nominal phase in CAN communication.
  */
typedef struct {
    unsigned int ntseg2 : 7;  /**< Nominal phase segment 2 (Tseg2) timing (7 bits) */
    unsigned int ntseg1 : 8;  /**< Nominal phase segment 1 (Tseg1) timing (8 bits) */
    unsigned int nbrp : 9;    /**< Nominal phase bit rate prescaler (9 bits) */
    unsigned int nsjw : 7;    /**< Nominal phase synchronization jump width (7 bits) */
} CAN_NominalBitTiming;

/**
  * @brief CAN global filter configuration structure.
  */
typedef struct {
    CAN_RejectRemoteFrames     rejectRemoteFramesExtendedCfg;  /**< Rejecting remote frames with extended ID */
    CAN_RejectRemoteFrames     rejectRemoteFramesStandardCfg;   /**< Rejecting remote frames with standard ID */
    CAN_AcpNonMatchFrameMode   nonMatchExtendedCfg;           /**< Non-matching frames with extended ID */
    CAN_AcpNonMatchFrameMode   nonMatchStandardCfg;           /**< Non-matching frames with standard ID */
} CAN_GlobalFilterConfig;

/**
  * @brief CAN timestamp configuration structure.
  */
typedef struct {
    CAN_TimestampMode            timestampSelect;  /**< Timestamp mode selection */
    unsigned int                   counterPrescaler;  /**< Timestamp counter prescaler value */
} CAN_TimestampConfig;

/**
  * @brief CAN timeout configuration structure.
  */
typedef struct {
    unsigned int                   enable;           /**< Timeout enable flag */
    CAN_TimeoutMode                timeoutSelect;    /**< Timeout mode selection */
    unsigned int                   timeoutPeriod;    /**< Timeout period value */
} CAN_TimeoutConfig;

/**
  * @brief CAN transmit message configuration structure.
  */
typedef struct {
    CAN_FrameTypeEnum             frameType;     /**< Type of transmit frame (data or remote) */
    CAN_IDType                    idType;          /**< Identifier type (standard or extended) */
    unsigned int                  identifier;      /**< Message identifier */
    CAN_DlcBytes                  dataLength;      /**< Data length code (DLC) */
    unsigned char                 data[8];        /**< Message data buffer */
} CAN_Message;

#ifdef CANFD_ENABLE
/**
  * @brief CANFD transmit message configuration structure.
  */
typedef struct {
    CAN_FrameTypeEnum             txFrameType;     /**< Type of transmit frame (data or remote) */
    CAN_IDType                    idType;          /**< Identifier type (standard or extended) */
    unsigned int                  identifier;      /**< Message identifier */
    unsigned short                msgMarker;       /**< Message marker for identification */
    CAN_EventFIFOControl          efcEn;           /**< Event FIFO control enable */
    CAN_TimeStampCaptureControl   tscEn;           /**< Timestamp capture enable */
    CAN_Format                    format;           /**< CAN format enable */
    CAN_BrsState                  brsState;        /**< Bit rate switching state */
    CAN_DlcBytes                  dataLength;      /**< Data length code (DLC) */
    unsigned char                 data[64];        /**< Message data buffer (maximum 64 bytes) */
} CAN_FdTxMessage;


/**
  * @brief CAN receive message structure.
  */
typedef struct {
    CAN_IDType                    idType;          /**< Identifier type (standard or extended) */
    CAN_FrameTypeEnum             rxFrameType;     /**< Type of received frame (data or remote) */
    unsigned int                  identifier;      /**< Message identifier */
    unsigned int                  rxFliterMatch;   /**< Filter match result */
    unsigned int                  rxFliterIndex;   /**< Filter index that matched the message */
    CAN_Format                    format;           /**< CAN format enable */
    CAN_BrsState                  brsState;        /**< Bit rate switching state */
    CAN_DlcBytes                  dataLength;      /**< Data length code (DLC) */
    unsigned int                  rxTimeStamp;     /**< Timestamp of message reception */
    unsigned char                 data[64];        /**< Received message data buffer (maximum 64 bytes) */
} CAN_FdRxMessage;

/**
  * @brief CAN transmit event header structure.
  */
typedef struct {
    CAN_IDType                    idType;          /**< Identifier type (standard or extended) */
    CAN_FrameTypeEnum             txFrameType;     /**< Type of transmit frame (data or remote) */
    unsigned int                  identifier;      /**< Message identifier */
    unsigned int                  msgMarker;       /**< Message marker for identification */
    CAN_TxEventType               eventType;       /**< Type of transmit event */
    CAN_Format                    format;           /**< CAN format enable */
    CAN_BrsState                  brsState;        /**< Bit rate switching state */
    CAN_DlcBytes                  dataLength;      /**< Data length code (DLC) */
    unsigned int                  txTimestamp;     /**< Timestamp of message transmission */
} CAN_FdTxEventHeader;
#endif

/**
  * @brief CAN high-priority message status structure.
  */
typedef struct {
    unsigned int             filterList;  /**< Filter list identifier */
    unsigned int             filterIndex;  /**< Filter index */
    CAN_HpMsgStorage         msgStorage;   /**< High-priority message storage configuration */
    unsigned int             msgIndex;     /**< Message index */
} CAN_HpMsgStatus;

/**
  * @brief CAN protocol status structure.
  */
typedef struct {
    CAN_CommunicationErrorCode     lastErrorCode;  /**< Last communication error code */
    CAN_CommunicationState         activity;       /**< Current communication state */
    unsigned int                     errorPassive;   /**< Error passive status */
    unsigned int                     warning;        /**< Error warning status */
    unsigned int                     busOff;         /**< Bus off status */
    unsigned int                     dataLastErrorCode;  /**< Last data-related error code */
    unsigned int                     rxESIflag;      /**< Receive ESI flag */
    unsigned int                     rxBRSflag;      /**< Receive BRS flag */
    unsigned int                     rxFDFflag;      /**< Receive FDF flag */
    unsigned int                     protocolException;  /**< Protocol exception status */
    unsigned int                     tdcValue;       /**< Time delta counter (TDC) value */
} CAN_ProtocolStatus;

/**
  * @brief CAN error counters structure.
  */
typedef struct {
    unsigned int txErrorCnt;  /**< Transmit error counter */
    unsigned int rxErrorCnt;  /**< Receive error counter */
    unsigned int rxErrorPassive;  /**< Receive error passive counter */
    unsigned int errorLogging;    /**< Error logging counter */
} CAN_ErrorCounters;
/**
  * @brief CAN filter configuration structure.
  */
typedef struct {
    unsigned int index;              /**< Filter index or identifier */
    CAN_FilterType filterType;     /**< Type of filter (e.g., range, dual, mask) */
    CAN_FilterConfig filterConfig; /**< Filter configuration (e.g., disable, direct to FIFO0, etc.) */
    unsigned int filterID1;          /**< First filter identifier (ID1) */
    unsigned int filterID2;          /**< Second filter identifier (ID2) */
} CAN_FilterCfg;

/**
  * @brief CAN RAM parameter configuration structure.
  */
typedef struct {
    unsigned int ramTotalSize;       /**< Total RAM size allocated for CAN module, unit: byte. */
    unsigned int stdFltNum;          /**< Number of standard filters, unit: 4byte. */
    unsigned int extFltNum;          /**< Number of extended filters, unit: 8byte. */
    unsigned int rxFifo0Num;         /**< Number of receive FIFO 0 slots, unit: 8Byte + dataSize. */
    unsigned int rxFifo1Num;         /**< Number of receive FIFO 1 slots, unit: 8Byte + dataSize. */
    unsigned int rxBufNum;           /**< Number of receive buffer slots, unit: 8Byte + dataSize. */
    unsigned int txEvtFifoNum;       /**< Number of transmit event FIFO slots, unit: 8Byte. */
    unsigned int txBufNum;           /**< Number of transmit buffer slots, unit: 8Byte + dataSize. */
    unsigned int txFifoQNum;         /**< Number of transmit FIFO/Queue slots, unit: 8Byte + dataSize. */
    CAN_DataSize dataSize;         /**< Data size configuration */
    CAN_RxFifoMode rxFifo0OperationMode;  /**< Receive FIFO 0 operation mode */
    CAN_RxFifoMode rxFifo1OperationMode;  /**< Receive FIFO 1 operation mode */
    CAN_TxFifoQueueMode txfqMode;  /**< Transmit FIFO/Queue operation mode */
} CAN_RamParam;

/**
 * @brief Type define of user callback function
 */
typedef struct {
    void (*RxFifo0Callback)(void *handle, unsigned int rxFifo0ItState);
    void (*RxFifo1Callback)(void *handle, unsigned int rxFifo1ItState);
    void (*RxBufferCallback)(void *handle, unsigned int rxBufferItState);
    void (*TxFifoEmptyCallback)(void *handle, unsigned int emptyFlag);
    void (*TxCompleteCallback)(void *handle, unsigned int indexes);
    void (*TxCanacelCallback)(void *handle, unsigned int indexes);
    void (*HighPriorityMsgCallback)(void *handle, unsigned int highPriortyMsgFlag);
    void (*TxEventFifoCallback)(void *handle, unsigned int txEventFifoItState);
    void (*TimestampCallback)(void *handle, unsigned int wraparoundFlag);
    void (*TimeoutCallback)(void *handle, unsigned int timeoutFlag);
    void (*ErrorCallback)(void *handle, unsigned int errorItState);
} CAN_UserCallBack;

/**
 * @brief Extent handle definition of CAN
 */
typedef struct {
    unsigned int itLineSelect;
    CAN_LineItEnable itLineEnable;
#ifdef CANFD_ENABLE
    CAN_DataBitTiming               dataBitTiming;      /**< Data bit timing (data transmission rate) */
#endif
} CAN_ExtendHandle;
/**
  * @}
  */

/**
  * @defgroup CAN_Reg_Def CAN Register Definition
  * @brief CAN register mapping structure.
  * @{
  */

/* Define the union U_CREL */
typedef union {
    unsigned int reg;
    struct {
        unsigned int day : 8;
        unsigned int mon : 8;
        unsigned int year : 4;
        unsigned int substep : 4;
        unsigned int step : 4;
        unsigned int rel : 4;
    } BIT;
} volatile CREL_REG;

/* Define the union DBTP_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int dsjw : 4;
        unsigned int dtseg2 : 4;
        unsigned int dtseg1 : 5;
        unsigned int reserved_0 : 3;
        unsigned int dbrp : 5;
        unsigned int reserved_1 : 2;
        unsigned int tdc : 1;
        unsigned int reserved_2 : 8;
    } BIT;
} volatile DBTP_REG;

/* Define the union TEST_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int reserved_0 : 4;
        unsigned int lbck : 1;  /* Loop Back Mode. */
        unsigned int tx : 2;    /* Control of Transmit Pin. */
        unsigned int rx : 1;    /* Receive Pin. */
        unsigned int txbnp : 5; /* Tx Buffer Number Prepared. */
        unsigned int pval : 1;  /* Prepared Valid. */
        unsigned int reserved_1 : 2;
        unsigned int txbns : 5; /* Tx Buffer Number Started. */
        unsigned int sval : 1;  /* Started Valid. */
        unsigned int reserved_2 : 10;
    } BIT;
} volatile TEST_REG;

/* Define the union RWD_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int wdc : 8; /* Watchdog Value. */
        unsigned int wdv : 8; /* Watchdog Configuration. */
        unsigned int reserved_0 : 16;
    } BIT;
} volatile RWD_REG;

/* Define the union CCCR_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int init : 1;
        unsigned int cce : 1;
        unsigned int asm_u : 1;  // Asm is the keyword.
        unsigned int csa : 1;
        unsigned int csr : 1;
        unsigned int bmon : 1;
        unsigned int dar : 1;
        unsigned int test : 1;
#ifdef CANFD_ENABLE
        unsigned int fdoe : 1;
        unsigned int brse : 1;
#else
        unsigned int reserved_0 : 2;
#endif
        unsigned int reserved_1 : 1;
        unsigned int wmm : 1;
        unsigned int pxhd : 1;
        unsigned int efbi : 1;
        unsigned int txp : 1;
        unsigned int niso : 1;
        unsigned int reserved_2 : 16;
    } BIT;
} volatile CCCR_REG;

/* Define the union NBTP_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int ntseg2 : 7;
        unsigned int reserved_0 : 1;
        unsigned int ntseg : 8;
        unsigned int nbrp : 9;
        unsigned int nsjw : 7;
    } BIT;
} volatile NBTP_REG;

/* Define the union TSCC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int tss : 2; /* Timestamp Select. */
        unsigned int reserved_0 : 14;
        unsigned int tcp : 4; /* Timestamp Counter Prescaler. */
        unsigned int reserved_1 : 12;
    } BIT;
} volatile TSCC_REG;

/* Define the union TSCV_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int tsc : 16;
        unsigned int reserved_0 : 16;
    } BIT;
} volatile TSCV_REG;

/* Define the union TOCC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int etoc : 1; /* Enable Timeout Counter. */
        unsigned int tos : 2;  /* Timeout Select. */
        unsigned int reserved_0 : 13;
        unsigned int top : 16; /* Timeout Period. */
    } BIT;
} volatile TOCC_REG;

/* Define the union TOCV_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int toc : 16;
        unsigned int reserved_0 : 16;
    } BIT;
} volatile TOCV_REG;

/* Define the union ECR_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int tec : 8; /* Transmit Error Counter. */
        unsigned int rec : 7; /* Receive Error Counter. */
        unsigned int rp : 1;  /* Receive Error Passive. */
        unsigned int cel : 8; /* CAN Error Logging. */
        unsigned int reserved_0 : 8;
    } BIT;
} volatile ECR_REG;

/* Define the union PSR_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int lec : 3;  /* Last Error Code. */
        unsigned int act : 2;  /* Activity. */
        unsigned int eps : 1;  /* Error Passive. */
        unsigned int ews : 1;  /* Warning Status. */
        unsigned int bos : 1;  /* Bus_Off Status. */
        unsigned int dlec : 3; /* Data Phase Last Error Code. */
        unsigned int res : 1;  /* ESI flag of last received Message. */
        unsigned int rbrs : 1; /* BRS flag of last received CAN Message. */
        unsigned int rfdf : 1; /* BRS flag of last received CAN Message. */
        unsigned int pxe : 1;  /* BRS flag of last received CAN Message. */
        unsigned int reserved_0 : 1;
        unsigned int tdcv : 7; /* BRS flag of last received CAN Message. */
        unsigned int reserved_1 : 9;
    } BIT;
} volatile PSR_REG;

/* Define the union TDCR_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int tdcf : 7; /* Transmitter Delay Compensation SSP Offset. */
        unsigned int reserved_0 : 1;
        unsigned int tdco : 7; /* Transmitter Delay Compensation Filter Window Length. */
        unsigned int reserved_1 : 17;
    } BIT;
} volatile TDCR_REG;

/* Define the union IR_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int rf0n : 1;
        unsigned int rf0w : 1;
        unsigned int rf0f : 1;
        unsigned int rf0l : 1;
        unsigned int rf1n : 1;
        unsigned int rf1w : 1;
        unsigned int rf1f : 1;
        unsigned int rf1l : 1;
        unsigned int hpm : 1;
        unsigned int tc : 1;
        unsigned int tcf : 1;
        unsigned int tfe : 1;
        unsigned int tefn : 1;
        unsigned int tefw : 1;
        unsigned int teff : 1;
        unsigned int tefl : 1;
        unsigned int tsw : 1;
        unsigned int mraf : 1;
        unsigned int too : 1;
        unsigned int drx : 1;
        unsigned int bec : 1;
        unsigned int beu : 1;
        unsigned int elo : 1;
        unsigned int ep : 1;
        unsigned int ew : 1;
        unsigned int bo : 1;
        unsigned int wdi : 1;
        unsigned int pea : 1;
        unsigned int ped : 1;
        unsigned int ara : 1;
        unsigned int reserved_0 : 2;
    } BIT;
} volatile IR_REG;

/* Define the union IE_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int rf0ne : 1;
        unsigned int rf0we : 1;
        unsigned int rf0fe : 1;
        unsigned int rf0le : 1;
        unsigned int rf1ne : 1;
        unsigned int rf1we : 1;
        unsigned int rf1fe : 1;
        unsigned int rf1le : 1;
        unsigned int hpme : 1;
        unsigned int tce : 1;
        unsigned int tcfe : 1;
        unsigned int tfee : 1;
        unsigned int tefne : 1;
        unsigned int tefwe : 1;
        unsigned int teffe : 1;
        unsigned int tefle : 1;
        unsigned int tswe : 1;
        unsigned int mrafe : 1;
        unsigned int tooe : 1;
        unsigned int drxe : 1;
        unsigned int bece : 1;
        unsigned int beue : 1;
        unsigned int eloe : 1;
        unsigned int epe : 1;
        unsigned int ewe : 1;
        unsigned int boe : 1;
        unsigned int wdie : 1;
        unsigned int peae : 1;
        unsigned int pede : 1;
        unsigned int arae : 1;
        unsigned int reserved_0 : 2;
    } BIT;
} volatile IE_REG;

/* Define the union ILS_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int rf0nl : 1;
        unsigned int rf0wl : 1;
        unsigned int rf0fl : 1;
        unsigned int rf0ll : 1;
        unsigned int rf1nl : 1;
        unsigned int rf1wl : 1;
        unsigned int rf1fl : 1;
        unsigned int rf1ll : 1;
        unsigned int hpml : 1;
        unsigned int tcl : 1;
        unsigned int tcfl : 1;
        unsigned int tfel : 1;
        unsigned int tefnl : 1;
        unsigned int tefwl : 1;
        unsigned int teffl : 1;
        unsigned int tefll : 1;
        unsigned int tswl : 1;
        unsigned int mrafl : 1;
        unsigned int tool : 1;
        unsigned int drxl : 1;
        unsigned int becl : 1;
        unsigned int beul : 1;
        unsigned int elol : 1;
        unsigned int epl : 1;
        unsigned int ewl : 1;
        unsigned int bol : 1;
        unsigned int wdil : 1;
        unsigned int peal : 1;
        unsigned int pedl : 1;
        unsigned int aral : 1;
        unsigned int reserved_0 : 2;
    } BIT;
} volatile ILS_REG;

/* Define the union ILE_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int eint0 : 1;
        unsigned int eint1 : 1;
        unsigned int reserved_0 : 30;
    } BIT;
} volatile ILE_REG;

/* Define the union GFC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int rrfe : 1; /* Reject Remote Frames Extended. */
        unsigned int rrfs : 1; /* Reject Remote Frames Standard. */
        unsigned int anfe : 2; /* Accept Non-matching Frames Extended. */
        unsigned int anfs : 2; /* Accept Non-matching Frames Standard. */
        unsigned int reserved_0 : 26;
    } BIT;
} volatile GFC_REG;

/* Define the union SIDFC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int reserved_0 : 2;
        unsigned int flssa : 14; /* Filter List Standard Start Address. */
        unsigned int lss : 8;    /* List Size Standard. */
        unsigned int reserved_1 : 8;
    } BIT;
} volatile SIDFC_REG;

/* Define the union XIDFC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int reserved_0 : 2;
        unsigned int flesa : 14; /* Filter List Extended Start Address. */
        unsigned int lse : 7;    /* List Size Extended. */
        unsigned int reserved_1 : 9;
    } BIT;
} volatile XIDFC_REG;

/* Define the union XIDAM_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int eidm : 29; /* Extended ID Mask. */
        unsigned int reserved_0 : 3;
    } BIT;
} volatile XIDAM_REG;

/* Define the union HPMS_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int bidx : 6; /* Buffer Index. */
        unsigned int msi : 2;  /* Message Storage Indicator. */
        unsigned int fidx : 7; /* Filter Index. */
        unsigned int flst : 1; /* Filter List. */
        unsigned int reserved_0 : 16;
    } BIT;
} volatile HPMS_REG;

/* Define the union NDAT1_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int nd0 : 1;
        unsigned int nd1 : 1;
        unsigned int nd2 : 1;
        unsigned int nd3 : 1;
        unsigned int nd4 : 1;
        unsigned int nd5 : 1;
        unsigned int nd6 : 1;
        unsigned int nd7 : 1;
        unsigned int nd8 : 1;
        unsigned int nd9 : 1;
        unsigned int nd10 : 1;
        unsigned int nd11 : 1;
        unsigned int nd12 : 1;
        unsigned int nd13 : 1;
        unsigned int nd14 : 1;
        unsigned int nd15 : 1;
        unsigned int nd16 : 1;
        unsigned int nd17 : 1;
        unsigned int nd18 : 1;
        unsigned int nd19 : 1;
        unsigned int nd20 : 1;
        unsigned int nd21 : 1;
        unsigned int nd22 : 1;
        unsigned int nd23 : 1;
        unsigned int nd24 : 1;
        unsigned int nd25 : 1;
        unsigned int nd26 : 1;
        unsigned int nd27 : 1;
        unsigned int nd28 : 1;
        unsigned int nd29 : 1;
        unsigned int nd30 : 1;
        unsigned int nd31 : 1;
    } BIT;
} volatile NDAT1_REG;

/* Define the union NDAT2_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int nd32 : 1;
        unsigned int nd33 : 1;
        unsigned int nd34 : 1;
        unsigned int nd35 : 1;
        unsigned int nd36 : 1;
        unsigned int nd37 : 1;
        unsigned int nd38 : 1;
        unsigned int nd39 : 1;
        unsigned int nd40 : 1;
        unsigned int nd41 : 1;
        unsigned int nd42 : 1;
        unsigned int nd43 : 1;
        unsigned int nd44 : 1;
        unsigned int nd45 : 1;
        unsigned int nd46 : 1;
        unsigned int nd47 : 1;
        unsigned int nd48 : 1;
        unsigned int nd49 : 1;
        unsigned int nd50 : 1;
        unsigned int nd51 : 1;
        unsigned int nd52 : 1;
        unsigned int nd53 : 1;
        unsigned int nd54 : 1;
        unsigned int nd55 : 1;
        unsigned int nd56 : 1;
        unsigned int nd57 : 1;
        unsigned int nd58 : 1;
        unsigned int nd59 : 1;
        unsigned int nd60 : 1;
        unsigned int nd61 : 1;
        unsigned int nd62 : 1;
        unsigned int nd63 : 1;
    } BIT;
} volatile NDAT2_REG;

/* Define the union RXF0C_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int reserved_0 : 2;
        unsigned int f0sa : 14; /* Rx FIFO 0 Start Address. */
        unsigned int f0s : 7;   /* Rx FIFO 0 Size. */
        unsigned int reserved_1 : 1;
        unsigned int f0wm : 7; /* Rx FIFO 0 Watermark. */
        unsigned int f0om : 1; /* FIFO 0 Operation Mode. */
    } BIT;
} volatile RXF0C_REG;

/* Define the union RXF0S_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int f0fl : 7; /* Rx FIFO 0 Fill Level. */
        unsigned int reserved_0 : 1;
        unsigned int f0gi : 6; /* Rx FIFO 0 Get Index. */
        unsigned int reserved_1 : 2;
        unsigned int f0pi : 6; /* Rx FIFO 0 Put Index. */
        unsigned int reserved_2 : 2;
        unsigned int f0f : 1;   /* Rx FIFO 0 Full. */
        unsigned int rf0ml : 1; /* Rx FIFO 0 Message Lost. */
        unsigned int reserved_3 : 6;
    } BIT;
} volatile RXF0S_REG;

/* Define the union RXF0A_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int f0ai : 6; /* Rx FIFO 0 Acknowledge Index. */
        unsigned int reserved_0 : 26;
    } BIT;
} volatile RXF0A_REG;

/* Define the union RXBC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int reserved_0 : 2;
        unsigned int rbsa : 14; /* Rx Buffer Start Address. */
        unsigned int reserved_1 : 16;
    } BIT;
} volatile RXBC_REG;

/* Define the union RXF1C_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int reserved_0 : 2;
        unsigned int f1sa : 14; /* Rx FIFO 1 Start Address. */
        unsigned int f1s : 7;   /* Rx FIFO 1 Size. */
        unsigned int reserved_1 : 1;
        unsigned int f1wm : 7; /* Rx FIFO 1 Watermark. */
        unsigned int f1om : 1; /* FIFO 1 Operation Mode. */
    } BIT;
} volatile RXF1C_REG;

/* Define the union RXF1S_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int f1fl : 7; /* Rx FIFO 1 Fill Level. */
        unsigned int reserved_0 : 1;
        unsigned int f1gi : 6; /* Rx FIFO 1 Get Index. */
        unsigned int reserved_1 : 2;
        unsigned int f1pi : 6; /* Rx FIFO 1 Put Index. */
        unsigned int reserved_2 : 2;
        unsigned int f1f : 1;   /* Rx FIFO 1 Full. */
        unsigned int rf1ml : 1; /* Rx FIFO 1 Message Lost. */
        unsigned int reserved_3 : 4;
        unsigned int dms : 2; /* Debug Message Status. */
    } BIT;
} volatile RXF1S_REG;

/* Define the union RXF1A_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int f1a : 6; /* Rx FIFO 1 Acknowledge Index. */
        unsigned int reserved_0 : 26;
    } BIT;
} volatile RXF1A_REG;

/* Define the union RXESC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int f0ds : 3; /* Rx FIFO 0 Data Field Size. */
        unsigned int reserved_0 : 1;
        unsigned int f1ds : 3; /* Rx FIFO 1 Data Field Size. */
        unsigned int reserved_1 : 1;
        unsigned int rbds : 3; /* Rx Buffer Data Field Size. */
        unsigned int reserved_2 : 21;
    } BIT;
} volatile RXESC_REG;

/* Define the union TXBC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int reserved_0 : 2;
        unsigned int tbsa : 14; /* Tx Buffers Start Address. */
        unsigned int ndtb : 6;  /* Number of Dedicated Transmit Buffers. */
        unsigned int reserved_1 : 2;
        unsigned int tfqs : 6; /* Transmit FIFO/Queue Size. */
        unsigned int tfqm : 1; /* Tx FIFO/Queue Mode. */
        unsigned int reserved_2 : 1;
    } BIT;
} volatile TXBC_REG;

/* Define the union TXFQS_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int tffl : 6; /* Tx FIFO Free Level. */
        unsigned int reserved_0 : 2;
        unsigned int tfgi : 5; /* Tx FIFO Get Index. */
        unsigned int reserved_1 : 3;
        unsigned int tfqp : 5; /* Tx FIFO/Queue Put Index. */
        unsigned int tfqf : 1; /* Tx FIFO/Queue Full. */
        unsigned int reserved_2 : 10;
    } BIT;
} volatile TXFQS_REG;

/* Define the union TXESC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int tbds : 3; /* Tx Buffer Data Field Size. */
        unsigned int reserved_0 : 29;
    } BIT;
} volatile TXESC_REG;

/* Define the union TXBRP_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int trp0 : 1;
        unsigned int trp1 : 1;
        unsigned int trp2 : 1;
        unsigned int trp3 : 1;
        unsigned int trp4 : 1;
        unsigned int trp5 : 1;
        unsigned int trp6 : 1;
        unsigned int trp7 : 1;
        unsigned int trp8 : 1;
        unsigned int trp9 : 1;
        unsigned int trp10 : 1;
        unsigned int trp11 : 1;
        unsigned int trp12 : 1;
        unsigned int trp13 : 1;
        unsigned int trp14 : 1;
        unsigned int trp15 : 1;
        unsigned int trp16 : 1;
        unsigned int trp17 : 1;
        unsigned int trp18 : 1;
        unsigned int trp19 : 1;
        unsigned int trp20 : 1;
        unsigned int trp21 : 1;
        unsigned int trp22 : 1;
        unsigned int trp23 : 1;
        unsigned int trp24 : 1;
        unsigned int trp25 : 1;
        unsigned int trp26 : 1;
        unsigned int trp27 : 1;
        unsigned int trp28 : 1;
        unsigned int trp29 : 1;
        unsigned int trp30 : 1;
        unsigned int trp31 : 1;
    } BIT;
} volatile TXBRP_REG;

/* Define the union TXBAR_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int ar0 : 1;
        unsigned int ar1 : 1;
        unsigned int ar2 : 1;
        unsigned int ar3 : 1;
        unsigned int ar4 : 1;
        unsigned int ar5 : 1;
        unsigned int ar6 : 1;
        unsigned int ar7 : 1;
        unsigned int ar8 : 1;
        unsigned int ar9 : 1;
        unsigned int ar10 : 1;
        unsigned int ar11 : 1;
        unsigned int ar12 : 1;
        unsigned int ar13 : 1;
        unsigned int ar14 : 1;
        unsigned int ar15 : 1;
        unsigned int ar16 : 1;
        unsigned int ar17 : 1;
        unsigned int ar18 : 1;
        unsigned int ar19 : 1;
        unsigned int ar20 : 1;
        unsigned int ar21 : 1;
        unsigned int ar22 : 1;
        unsigned int ar23 : 1;
        unsigned int ar24 : 1;
        unsigned int ar25 : 1;
        unsigned int ar26 : 1;
        unsigned int ar27 : 1;
        unsigned int ar28 : 1;
        unsigned int ar29 : 1;
        unsigned int ar30 : 1;
        unsigned int ar31 : 1;
    } BIT;
} volatile TXBAR_REG;

/* Define the union TXBCR_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int cr0 : 1;
        unsigned int cr1 : 1;
        unsigned int cr2 : 1;
        unsigned int cr3 : 1;
        unsigned int cr4 : 1;
        unsigned int cr5 : 1;
        unsigned int cr6 : 1;
        unsigned int cr7 : 1;
        unsigned int cr8 : 1;
        unsigned int cr9 : 1;
        unsigned int cr10 : 1;
        unsigned int cr11 : 1;
        unsigned int cr12 : 1;
        unsigned int cr13 : 1;
        unsigned int cr14 : 1;
        unsigned int cr15 : 1;
        unsigned int cr16 : 1;
        unsigned int cr17 : 1;
        unsigned int cr18 : 1;
        unsigned int cr19 : 1;
        unsigned int cr20 : 1;
        unsigned int cr21 : 1;
        unsigned int cr22 : 1;
        unsigned int cr23 : 1;
        unsigned int cr24 : 1;
        unsigned int cr25 : 1;
        unsigned int cr26 : 1;
        unsigned int cr27 : 1;
        unsigned int cr28 : 1;
        unsigned int cr29 : 1;
        unsigned int cr30 : 1;
        unsigned int cr31 : 1;
    } BIT;
} volatile TXBCR_REG;

/* Define the union TXBTO_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int to0 : 1;
        unsigned int to1 : 1;
        unsigned int to2 : 1;
        unsigned int to3 : 1;
        unsigned int to4 : 1;
        unsigned int to5 : 1;
        unsigned int to6 : 1;
        unsigned int to7 : 1;
        unsigned int to8 : 1;
        unsigned int to9 : 1;
        unsigned int to10 : 1;
        unsigned int to11 : 1;
        unsigned int to12 : 1;
        unsigned int to13 : 1;
        unsigned int to14 : 1;
        unsigned int to15 : 1;
        unsigned int to16 : 1;
        unsigned int to17 : 1;
        unsigned int to18 : 1;
        unsigned int to19 : 1;
        unsigned int to20 : 1;
        unsigned int to21 : 1;
        unsigned int to22 : 1;
        unsigned int to23 : 1;
        unsigned int to24 : 1;
        unsigned int to25 : 1;
        unsigned int to26 : 1;
        unsigned int to27 : 1;
        unsigned int to28 : 1;
        unsigned int to29 : 1;
        unsigned int to30 : 1;
        unsigned int to31 : 1;
    } BIT;
} volatile TXBTO_REG;

/* Define the union TXBCF_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int cf0 : 1;
        unsigned int cf1 : 1;
        unsigned int cf2 : 1;
        unsigned int cf3 : 1;
        unsigned int cf4 : 1;
        unsigned int cf5 : 1;
        unsigned int cf6 : 1;
        unsigned int cf7 : 1;
        unsigned int cf8 : 1;
        unsigned int cf9 : 1;
        unsigned int cf10 : 1;
        unsigned int cf11 : 1;
        unsigned int cf12 : 1;
        unsigned int cf13 : 1;
        unsigned int cf14 : 1;
        unsigned int cf15 : 1;
        unsigned int cf16 : 1;
        unsigned int cf17 : 1;
        unsigned int cf18 : 1;
        unsigned int cf19 : 1;
        unsigned int cf20 : 1;
        unsigned int cf21 : 1;
        unsigned int cf22 : 1;
        unsigned int cf23 : 1;
        unsigned int cf24 : 1;
        unsigned int cf25 : 1;
        unsigned int cf26 : 1;
        unsigned int cf27 : 1;
        unsigned int cf28 : 1;
        unsigned int cf29 : 1;
        unsigned int cf30 : 1;
        unsigned int cf31 : 1;
    } BIT;
} volatile TXBCF_REG;

/* Define the union TXBTIE_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int tie0 : 1;
        unsigned int tie1 : 1;
        unsigned int tie2 : 1;
        unsigned int tie3 : 1;
        unsigned int tie4 : 1;
        unsigned int tie5 : 1;
        unsigned int tie6 : 1;
        unsigned int tie7 : 1;
        unsigned int tie8 : 1;
        unsigned int tie9 : 1;
        unsigned int tie10 : 1;
        unsigned int tie11 : 1;
        unsigned int tie12 : 1;
        unsigned int tie13 : 1;
        unsigned int tie14 : 1;
        unsigned int tie15 : 1;
        unsigned int tie16 : 1;
        unsigned int tie17 : 1;
        unsigned int tie18 : 1;
        unsigned int tie19 : 1;
        unsigned int tie20 : 1;
        unsigned int tie21 : 1;
        unsigned int tie22 : 1;
        unsigned int tie23 : 1;
        unsigned int tie24 : 1;
        unsigned int tie25 : 1;
        unsigned int tie26 : 1;
        unsigned int tie27 : 1;
        unsigned int tie28 : 1;
        unsigned int tie29 : 1;
        unsigned int tie30 : 1;
        unsigned int tie31 : 1;
    } BIT;
} volatile TXBTIE_REG;

/* Define the union TXBCIE_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int cfie0 : 1;
        unsigned int cfie1 : 1;
        unsigned int cfie2 : 1;
        unsigned int cfie3 : 1;
        unsigned int cfie4 : 1;
        unsigned int cfie5 : 1;
        unsigned int cfie6 : 1;
        unsigned int cfie7 : 1;
        unsigned int cfie8 : 1;
        unsigned int cfie9 : 1;
        unsigned int cfie10 : 1;
        unsigned int cfie11 : 1;
        unsigned int cfie12 : 1;
        unsigned int cfie13 : 1;
        unsigned int cfie14 : 1;
        unsigned int cfie15 : 1;
        unsigned int cfie16 : 1;
        unsigned int cfie17 : 1;
        unsigned int cfie18 : 1;
        unsigned int cfie19 : 1;
        unsigned int cfie20 : 1;
        unsigned int cfie21 : 1;
        unsigned int cfie22 : 1;
        unsigned int cfie23 : 1;
        unsigned int cfie24 : 1;
        unsigned int cfie25 : 1;
        unsigned int cfie26 : 1;
        unsigned int cfie27 : 1;
        unsigned int cfie28 : 1;
        unsigned int cfie29 : 1;
        unsigned int cfie30 : 1;
        unsigned int cfie31 : 1;
    } BIT;
} volatile TXBCIE_REG;

/* Define the union TXEFC_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int reserved_0 : 2;
        unsigned int efsa : 14; /* Event FIFO Start Address. */
        unsigned int efs : 6;   /* Event FIFO Size. */
        unsigned int reserved_1 : 2;
        unsigned int efwm : 6; /* Event FIFO Watermark. */
        unsigned int reserved_2 : 2;
    } BIT;
} volatile TXEFC_REG;

/* Define the union TXEFS_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int effl : 6; /* Event FIFO Fill Level. */
        unsigned int reserved_0 : 2;
        unsigned int efgi : 5; /* Event FIFO Get Index. */
        unsigned int reserved_1 : 3;
        unsigned int efpi : 5; /* Event FIFO Put Index. */
        unsigned int reserved_2 : 3;
        unsigned int eff : 1;   /* Event FIFO Full. */
        unsigned int tefel : 1; /* Tx Event FIFO Element Lost. */
        unsigned int reserved_3 : 6;
    } BIT;
} volatile TXEFS_REG;

/* Define the union TXEFA_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int EFA : 5; /* Event FIFO Acknowledge Index. */
        unsigned int reserved_0 : 27;
    } BIT;
} volatile TXEFA_REG;

/* Define the union WRAPPER_VER_REG */
typedef union {
    unsigned int reg;
    struct {
        unsigned int month_day : 16;
        unsigned int year : 8;
        unsigned int release_substep : 1;
        unsigned int release_step : 1;
        unsigned int release_ver : 1;
        unsigned int reserved_0 : 5;
    } BIT;
} volatile WRAPPER_VER_REG;

/**
  * @brief Register mapping structure.
  */
typedef struct {
    CREL_REG                        CREL;                     /* 0x0 */
    unsigned int                    ENDN;                     /* 0x4 */
#ifdef CANFD_ENABLE
    unsigned int                    CUST;                     /* 0x8 */
    DBTP_REG                        DBTP;                     /* 0xc */
#else
    unsigned int                    reserved_0[2];            /* 0x30~0x3c */
#endif

    TEST_REG                        TEST;                     /* 0x10 */
    RWD_REG                         RWD;                      /* 0x14 */
    CCCR_REG                        CCCR;                     /* 0x18 */
    NBTP_REG                        NBTP;                     /* 0x1c */
    TSCC_REG                        TSCC;                     /* 0x20 */
    TSCV_REG                        TSCV;                     /* 0x24 */
    TOCC_REG                        TOCC;                     /* 0x28 */
    TOCV_REG                        TOCV;                     /* 0x2c */
    unsigned int                    reserved_1[4];            /* 0x30~0x3c */
    ECR_REG                         ECR;                      /* 0x40 */
    PSR_REG                         PSR;                      /* 0x44 */
    TDCR_REG                        TDCR;                     /* 0x48 */
    unsigned int                    reserved_2;               /* 0x4c */
    IR_REG                          IR;                       /* 0x50 */
    IE_REG                          IE;                       /* 0x54 */
    ILS_REG                         ILS;                      /* 0x58 */
    ILE_REG                         ILE;                      /* 0x5c */
    unsigned int                    reserved_3[8];            /* 0x60~0x7c */
    GFC_REG                         GFC;                      /* 0x80 */
    SIDFC_REG                       SIDFC;                    /* 0x84 */
    XIDFC_REG                       XIDFC;                    /* 0x88 */
    unsigned int                    reserved_4;               /* 0x8c */
    XIDAM_REG                       XIDAM;                    /* 0x90 */
    HPMS_REG                        HPMS;                     /* 0x94 */
    NDAT1_REG                       NDAT1;                    /* 0x98 */
    NDAT2_REG                       NDAT2;                    /* 0x9c */
    RXF0C_REG                       RXF0C;                    /* 0xa0 */
    RXF0S_REG                       RXF0S;                    /* 0xa4 */
    RXF0A_REG                       RXF0A;                    /* 0xa8 */
    RXBC_REG                        RXBC;                     /* 0xac */
    RXF1C_REG                       RXF1C;                    /* 0xb0 */
    RXF1S_REG                       RXF1S;                    /* 0xb4 */
    RXF1A_REG                       RXF1A;                    /* 0xb8 */
    RXESC_REG                       RXESC;                    /* 0xbc */
    TXBC_REG                        TXBC;                     /* 0xc0 */
    TXFQS_REG                       TXFQS;                    /* 0xc4 */
    TXESC_REG                       TXESC;                    /* 0xc8 */
    TXBRP_REG                       TXBRP;                    /* 0xcc */
    TXBAR_REG                       TXBAR;                    /* 0xd0 */
    TXBCR_REG                       TXBCR;                    /* 0xd4 */
    TXBTO_REG                       TXBTO;                    /* 0xd8 */
    TXBCF_REG                       TXBCF;                    /* 0xdc */
    TXBTIE_REG                      TXBTIE;                   /* 0xe0 */
    TXBCIE_REG                      TXBCIE;                   /* 0xe4 */
    unsigned int                    reserved_5[2];            /* 0xe8~0xec */
    TXEFC_REG                       TXEFC;                    /* 0xf0 */
    TXEFS_REG                       TXEFS;                    /* 0xf4 */
    TXEFA_REG                       TXEFA;                    /* 0xf8 */
    unsigned int                    reserved_6[449];          /* 0xfc~0x7fc */
    WRAPPER_VER_REG                 WRAPPER_VER;              /* 0x800 */
} volatile CAN_RegStruct;
/**
  * @}
  */

/**
  * @brief Check one bit value.
  * @param value the value of one bit.
  * @retval true
  * @retval false
  */
static inline bool IsOneBitCfgValue(unsigned int value)
{
    return (value == 0x0 || value == 0x01);
}

/**
  * @brief Check work mode.
  * @param value the value of work mode.
  * @retval true
  * @retval false
  */
static inline bool IsWorkMode(unsigned int value)
{
    return (value == WORK_MODE_NORMAL || value == WORK_MODE_RESTRICTED_OPERATION || \
            value == WORK_MODE_BUS_MONITORING || value == WORK_MODE_INTERNAL_LOOPBACK_TEST || \
            value == WORK_MODE_EXTERNAL_LOOPBACK_TEST);
}

/**
  * @brief Check auto retransmission.
  * @param value the value of auto retransmission.
  * @retval true
  * @retval false
  */
static inline bool IsAutoRetransmission(unsigned int value)
{
    return (value == AUTO_RETRANSMISSION_ENABLE || value == AUTO_RETRANSMISSION_DISABLE);
}


/**
  * @brief Check transmit pause configuration.
  * @param value the value of transmit pause configuration.
  * @retval true
  * @retval false
  */
static inline bool IsTransmitPause(unsigned int value)
{
    return (value == TRANSMIT_PAUSE_DISABLE || value == TRANSMIT_PAUSE_ENABLE);
}

/**
  * @brief Check frame format.
  * @param value the value of frame format.
  * @retval true
  * @retval false
  */
static inline bool IsFrameFormat(unsigned int value)
{
#ifdef CANFD_ENABLE
    return (value == CAN_FRAME_CLASSIC || value == CAN_FRAME_FD_NO_BRS || value == CAN_FRAME_FD_BRS);
#endif
    return (value == CAN_FRAME_CLASSIC);
}

/**
  * @brief Check reject remote frames with extended ID.
  * @param value the value of reject remote frames with extended ID.
  * @retval true
  * @retval false
  */
static inline bool IsRejectRemoteFrames(unsigned int value)
{
    return (value == FILTER_FRAME_WITH_ID || value == REJECY_ALL_FRAME_WITH_ID);
}

/**
  * @brief Check accept non-matching frames mode.
  * @param value the value of accept non-matching frames mode.
  * @retval true
  * @retval false
  */
static inline bool IsAcpNonMatchFrameMode(unsigned int value)
{
    return (value == ACCEPT_IN_RX_FIFO_0 || value == ACCEPT_IN_RX_FIFO_1 || \
            value == RETECT_FRAME);
}

/**
  * @brief Check protocol exception configuration.
  * @param value the value of protocol exception configuration.
  * @retval true
  * @retval false
  */
static inline bool IsProtocolException(unsigned int value)
{
    return (value == PROTOCOL_EXECPTION_DISABLE || value == PROTOCOL_EXECPTION_ENABLE);
}

/**
  * @brief Check wide message marker.
  * @param value the value of wide message marker.
  * @retval true
  * @retval false
  */
static inline bool IsWideMessageMarker(unsigned int value)
{
    return (value == WIDE_MESSAGE_MARKER_8BIT || value == WIDE_MESSAGE_MARKER_16BIT);
}

/**
  * @brief Check receive message location.
  * @param value the value of receive message location.
  * @retval true
  * @retval false
  */
static inline bool IsRxLocation(unsigned int value)
{
    return (value == READ_FROM_RX_FIFO0 || value == READ_FROM_RX_FIFO1 || \
            value == READ_FROM_RX_BUFFER);
}

/**
  * @brief Check identifier type.
  * @param value the value of identifier type.
  * @retval true
  * @retval false
  */
static inline bool IsIDType(unsigned int value)
{
    return (value == CAN_ID_STANDARD || value == CAN_ID_EXTENDED);
}

/**
  * @brief Check frame type.
  * @param value the value of frame type.
  * @retval true
  * @retval false
  */
static inline bool IsFrameType(unsigned int value)
{
    return (value == CAN_FRAME_DATA || value == CAN_FRAME_REMOTE);
}

/**
  * @brief Check message transmission mode.
  * @param value the value of message transmission mode.
  * @retval true
  * @retval false
  */
static inline bool IsSendMsgMode(unsigned int value)
{
    return (value == CAN_SEND_MSG_BY_TX_FIFOQ || value == CAN_SEND_MSG_BY_TX_BUFFER);
}

/**
  * @brief Check timestamp mode type.
  * @param value the value of timestamp mode type.
  * @retval true
  * @retval false
  */
static inline bool IsTimestampMode(unsigned int value)
{
    return (value == TIMESTAMP_COUNT_ALWAYS_0 || value == TIMESTAMP_COUNT_INCREMENT_ACCORDE_TCP || \
            value == TIMESTAMP_COUNT_USE_EXTERNAL);
}

/**
  * @brief Check timeout mode type.
  * @param value the value of timeout mode type.
  * @retval true
  * @retval false
  */
static inline bool IsTimeoutMode(unsigned int value)
{
    return (value == TIMEOUT_CONTINUOUS_OPERATION || value == TIMEOUT_CONTROLLED_BY_TX_EVENT_FIFO || \
            value == TIMEOUT_CONTROLLED_BY_RX_FIFO_0 || value == TIMEOUT_CONTROLLED_BY_RX_FIFO_1);
}

/**
  * @brief Check format type.
  * @param value the value of format type.
  * @retval true
  * @retval false
  */
static inline bool IsFormat(unsigned int value)
{
    return (value == CLASSIC_CAN_FORMAT || value == NO_CLASSIC_CAN_FORMAT);
}

/**
  * @brief Check bit rate switching state type.
  * @param value the value of bit rate switching state type.
  * @retval true
  * @retval false
  */
static inline bool IsBrsState(unsigned int value)
{
    return (value == BIT_RATE_SWITCHING_DISABLE || value == BIT_RATE_SWITCHING_ENABLE);
}

/**
  * @brief Check transmit event type.
  * @param value the value of transmit event type.
  * @retval true
  * @retval false
  */
static inline bool IsTxEventType(unsigned int value)
{
    return (value == TX_EVENT_TYPE || value == TX_EVENT_IN_CANCEL);
}

/**
 * @brief Enable or disable the initialization of the CAN module.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'init' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableInit(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.init = value;
}

/**
 * @brief Get the current initialization status of the CAN module.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'init' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetInitialized(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.init;
}

/**
 * @brief Enable or disable the configuration change detection.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'cce' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableCfgChange(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.cce = value;
}

/**
 * @brief Get the current status of the configuration change detection.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'cce' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetCfgChange(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.cce;
}

/**
 * @brief Enable or disable the restricted mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'asm_u' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableRestrictedMode(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.asm_u = value;
}

/**
 * @brief Get the current status of the restricted mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'asm_u' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetRestrictedMode(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.asm_u;
}

/**
 * @brief Enable or disable the clock stop acknowledge functionality.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'csa' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableClockStopAcknowledge(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.csa = value;
}

/**
 * @brief Get the current status of the clock stop acknowledge functionality.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'csa' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetClockStopAcknowledge(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.csa;
}

/**
 * @brief Enable or disable the clock stop request functionality.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'csr' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableClockStopRequest(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.csr = value;
}

/**
 * @brief Get the current status of the clock stop request functionality.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'csr' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetClockStopRequest(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.csr;
}

/**
 * @brief Enable or disable the bus monitoring mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'bmon' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableBusMonitoringMode(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.bmon = value;
}

/**
 * @brief Get the current status of the bus monitoring mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'bmon' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetBusMonitoringMode(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.bmon;
}

/**
 * @brief Enable or disable automatic retransmission.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'dar' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_DisableAutoRetransmission(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.dar = value;
}

/**
 * @brief Get the current status of automatic retransmission.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'dar' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetAutoRetransmission(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.dar;
}

/**
 * @brief Enable or disable the test mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'test' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableTestMode(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.test = value;
}

/**
 * @brief Get the current status of the test mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'test' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetTestMode(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.test;
}

/**
 * @brief Enable or disable the wide message marker.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'wmm' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableWideMessageMarker(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.wmm = value;
}

/**
 * @brief Get the current status of the wide message marker.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'wmm' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetWideMessageMarker(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.wmm;
}

/**
 * @brief Enable or disable protocol exception handling.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'pxhd' bit, 0: enable, 1: disable.
 * @return None.
 */
static inline void DCL_CAN_DisableProtocolExceptionHandling(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.pxhd = value;
}

/**
 * @brief Get the current status of protocol exception handling.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'pxhd' bit, 0: enable, 1: disable.
 */
static inline unsigned int DCL_CAN_GetProtocolExceptionHandling(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.pxhd;
}

/**
 * @brief Enable or disable edge filtering.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'efbi' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableEdgeFiltering(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.efbi = value;
}

/**
 * @brief Get the current status of edge filtering.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'efbi' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetEdgeFiltering(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.efbi;
}

/**
 * @brief Enable or disable the transmit pause functionality.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'txp' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableTransmitPause(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.txp = value;
}

/**
 * @brief Get the current status of the transmit pause functionality.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'txp' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetTransmitPause(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.txp;
}

/**
 * @brief Enable or disable non-ISO operation.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'niso' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableNonISOOperation(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.niso = value;
}

/**
 * @brief Get the current status of non-ISO operation.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'niso' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetNonISOOperation(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.niso;
}

/**
 * @brief Enable or disable the loop-back mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'lbck' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableLoopbackMode(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->TEST.BIT.lbck = value;
}

/**
 * @brief Get the current status of the loop-back mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'lbck' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetLoopbackMode(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TEST.BIT.lbck;
}

/**
 * @brief Set the transmit pin control value.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tx' field.
 * @return None.
 */
static inline void DCL_CAN_SetTransmitPinControl(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= TEST_TX_BIT_MASK);
    canx->TEST.BIT.tx = value;
}

/**
 * @brief Get the current value of the transmit pin control.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tx' field.
 */
static inline unsigned int DCL_CAN_GetTransmitPinControl(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TEST.BIT.tx;
}

/**
 * @brief Set the receive pin state.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rx' bit, 0: inactive, 1: active.
 * @return None.
 */
static inline void DCL_CAN_SetReceivePinState(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->TEST.BIT.rx = value;
}

/**
 * @brief Get the current state of the receive pin.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rx' bit, 0: inactive, 1: active.
 */
static inline unsigned int DCL_CAN_GetReceivePinState(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TEST.BIT.rx;
}

/**
 * @brief Set the prepared transmit buffer number.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'txbnp' field.
 * @return None.
 */
static inline void DCL_CAN_SetPreparedTxBufferNumber(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= TEST_TXBNP_BIT_MASK);
    canx->TEST.BIT.txbnp = value;
}

/**
 * @brief Get the current number of prepared transmit buffers.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'txbnp' field.
 */
static inline unsigned int DCL_CAN_GetPreparedTxBufferNumber(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TEST.BIT.txbnp;
}

/**
 * @brief Set the 'pval' bit in the TEST_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'pval' bit, 0: invalid, 1: valid.
 * @return None.
 */
static inline void DCL_CAN_SetPreparedValidFlag(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->TEST.BIT.pval = value;
}

/**
 * @brief Get the current status of the prepared valid flag.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'pval' bit, 0: invalid, 1: valid.
 */
static inline unsigned int DCL_CAN_GetPreparedValidFlag(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TEST.BIT.pval;
}

/**
 * @brief Set the started transmit buffer number.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'txbns' field.
 * @return None.
 */
static inline void DCL_CAN_SetStartedTxBufferNumber(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= BIT_TXBNS_MASK);
    canx->TEST.BIT.txbns = value;
}

/**
 * @brief Get the current number of started transmit buffers.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'txbns' field.
 */
static inline unsigned int DCL_CAN_GetStartedTxBufferNumber(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TEST.BIT.txbns;
}

/**
 * @brief Set the started valid flag.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'sval' bit, 0: invalid, 1:valid.
 * @return None.
 */
static inline void DCL_CAN_SetStartedValidFlag(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->TEST.BIT.sval = value;
}

/**
 * @brief Get the current status of the started valid flag.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'sval' bit, 0: invalid, 1:valid.
 */
static inline unsigned int DCL_CAN_GetStartedValidFlag(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TEST.BIT.sval;
}

/**
 * @brief Set the watchdog timer value.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'wdc' field.
 * @return None.
 */
static inline void DCL_CAN_SetWatchdogValue(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= BIT_WDC_MASK);
    canx->RWD.BIT.wdc = value;
}

/**
 * @brief Get the current watchdog timer value.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'wdc' field.
 */
static inline unsigned int DCL_CAN_GetWatchdogValue(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RWD.BIT.wdc;
}

/**
 * @brief Set the watchdog configuration.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'wdv' field.
 * @return None.
 */
static inline void DCL_CAN_SetWatchdogConfig(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= RWD_BIT_WDV_MASK);
    canx->RWD.BIT.wdv = value;
}

/**
 * @brief Get the current watchdog configuration.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'wdv' field.
 */
static inline unsigned int DCL_CAN_GetWatchdogConfig(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RWD.BIT.wdv;
}

/**
 * @brief Set the Timestamp Select in the TSCC register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tss' field.
 * @return None.
 */
static inline void DCL_CAN_SetTimestampSelect(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= TSCC_BIT_TSS_MASK);
    canx->TSCC.BIT.tss = value;
}

/**
 * @brief Get the current value of the Timestamp Select.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tss' field.
 */
static inline unsigned int DCL_CAN_GetTimestampSelect(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TSCC.BIT.tss;
}

/**
 * @brief Set the Timestamp Counter Prescaler.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tcp' field.
 * @return None.
 */
static inline void DCL_CAN_SetTimestampPrescaler(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= TSCC_BIT_TCP_MASK);
    canx->TSCC.BIT.tcp = value;
}

/**
 * @brief Get the current value of the Timestamp Counter Prescaler.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tcp' field.
 */
static inline unsigned int DCL_CAN_GetTimestampPrescaler(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TSCC.BIT.tcp;
}

/**
 * @brief Set the Timestamp Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tsc' field.
 * @return None.
 */
static inline void DCL_CAN_SetTimestampCounter(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0xFFFF);
    canx->TSCV.BIT.tsc = value;
}

/**
 * @brief Get the Timestamp Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tsc' field.
 */
static inline unsigned int DCL_CAN_GetTimestampCounter(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TSCV.BIT.tsc;
}

/**
 * @brief Set the Enable Timeout Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'etoc' bit, 0: disable, 1:enable.
 * @return None.
 */
static inline void DCL_CAN_SetEnableTimeoutCounter(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->TOCC.BIT.etoc = value;
}

/**
 * @brief Get the current value of the Enable Timeout Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'etoc' bit.
 */
static inline unsigned int DCL_CAN_GetEnableTimeoutCounter(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TOCC.BIT.etoc;
}

/**
 * @brief Set the Timeout Select.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tos' field.
 * @return None.
 */
static inline void DCL_CAN_SetTimeoutSelect(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x03);
    canx->TOCC.BIT.tos = value;
}

/**
 * @brief Get the current value of the Timeout Select.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tos' field.
 */
static inline unsigned int DCL_CAN_GetTimeoutSelect(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TOCC.BIT.tos;
}

/**
 * @brief Set the Timeout Period.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'top' field.
 * @return None.
 */
static inline void DCL_CAN_SetTimeoutPeriod(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0xFFFF);
    canx->TOCC.BIT.top = value;
}

/**
 * @brief Get the current value of the Timeout Period.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'top' field.
 */
static inline unsigned int DCL_CAN_GetTimeoutPeriod(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TOCC.BIT.top;
}

/**
 * @brief Set the Timeout Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'toc' field.
 * @return None.
 */
static inline void DCL_CAN_SetTimeoutCounter(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0xFFFF);
    canx->TOCV.BIT.toc = value;
}

/**
 * @brief Get the Timeout Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'toc' field.
 */
static inline unsigned int DCL_CAN_GetTimeoutCounter(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TOCV.BIT.toc;
}

/**
 * @brief Set the Transmit Error Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tec' field.
 * @return None.
 */
static inline void DCL_CAN_SetTransmitErrorCounter(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0xFF);
    canx->ECR.BIT.tec = value;
}

/**
 * @brief Get the current value of the Transmit Error Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tec' field.
 */
static inline unsigned int DCL_CAN_GetTransmitErrorCounter(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->ECR.BIT.tec;
}

/**
 * @brief Set the Receive Error Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rec' field.
 * @return None.
 */
static inline void DCL_CAN_SetReceiveErrorCounter(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->ECR.BIT.rec = value;
}

/**
 * @brief Get the current value of the Receive Error Counter.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rec' field.
 */
static inline unsigned int DCL_CAN_GetReceiveErrorCounter(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->ECR.BIT.rec;
}

/**
 * @brief Set the Receive Error Passive.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rp' bit.
 * @return None.
 */
static inline void DCL_CAN_SetReceiveErrorPassive(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->ECR.BIT.rp = value;
}

/**
 * @brief Get the current value of the Receive Error Passive.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rp' bit.
 */
static inline unsigned int DCL_CAN_GetReceiveErrorPassive(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->ECR.BIT.rp;
}

/**
 * @brief Set the CAN Error Logging.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'cel' field
 * @return None.
 */
static inline void DCL_CAN_SetCanErrorLogging(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->ECR.BIT.cel = value;
}

/**
 * @brief Get the current value of the CAN Error Logging.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'cel' field.
 */
static inline unsigned int DCL_CAN_GetCanErrorLogging(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->ECR.BIT.cel;
}

/**
 * @brief Set the current value of the Last Error Code.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'lec' field.
 * @return None.
 */
static inline void DCL_CAN_PsrRegSetLec(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x07);
    canx->PSR.BIT.lec = value;
}

/**
 * @brief Get the current value of the Last Error Code.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'lec' field.
 */
static inline unsigned int DCL_CAN_GetLastError(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.lec;
}

/**
 * @brief Set the Activity in the PSR_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'act' field.
 * @return None.
 */
static inline void DCL_CAN_SetActivity(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x03);
    canx->PSR.BIT.act = value;
}

/**
 * @brief Get the Activity in the PSR_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'act' field.
 */
static inline unsigned int DCL_CAN_GetActivity(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.act;
}

/**
 * @brief Set the Error Passive.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'eps' bit.
 * @return None.
 */
static inline void DCL_CAN_SetErrorPassive(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->PSR.BIT.eps = value;
}

/**
 * @brief Get the current value of the Error Passive.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'eps' bit.
 */
static inline unsigned int DCL_CAN_GetErrorPassive(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.eps;
}

/**
 * @brief Set the current value of the Warning Status.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'ews' bit.
 * @return None.
 */
static inline void DCL_CAN_SetWarningStatus(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->PSR.BIT.ews = value;
}

/**
 * @brief Get the current value of the Warning Status.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'ews'.
 */
static inline unsigned int DCL_CAN_GetWarningStatus(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.ews;
}

/**
 * @brief Set the Bus_Off Status.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'bos' bit.
 * @return None.
 */
static inline void DCL_CAN_SetBusOffStatus(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->PSR.BIT.bos = value;
}

/**
 * @brief Get the current value of the Bus_Off Status.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'bos'.
 */
static inline unsigned int DCL_CAN_GetBusOffStatus(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.bos;
}

/**
 * @brief Set the Data Phase Last Error Code.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'dlec' field.
 * @return None.
 */
static inline void DCL_CAN_SetDataPhaseLastError(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x07);
    canx->PSR.BIT.dlec = value;
}

/**
 * @brief Get the current value of the Data Phase Last Error Code.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'dlec' field.
 */
static inline unsigned int DCL_CAN_GetDataPhaseLastError(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.dlec;
}

/**
 * @brief Set the current value of the ESI Flag.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'res' bit.
 * @return None.
 */
static inline void DCL_CAN_SetEsiFlag(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->PSR.BIT.res = value;
}

/**
 * @brief Get the current value of the ESI Flag.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'res' bit.
 */
static inline unsigned int DCL_CAN_GetEsiFlag(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.res;
}

/**
 * @brief Set the BRS flag of last received CAN Message.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rbrs' bit.
 * @return None.
 */
static inline void DCL_CAN_SetBrsFlag(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->PSR.BIT.rbrs = value;
}

/**
 * @brief Get the BRS flag of last received CAN Message.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rbrs' bit.
 */
static inline unsigned int DCL_CAN_GetBrsFlag(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.rbrs;
}

/**
 * @brief Set the Received a CAN Message.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rfdf' bit.
 * @return None.
 */
static inline void DCL_CAN_SetReceivedMessageFlag(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->PSR.BIT.rfdf = value;
}

/**
 * @brief Get the Received a CAN Message.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rfdf' bit.
 */
static inline unsigned int DCL_CAN_GetReceivedMessageFlag(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.rfdf;
}

/**
 * @brief Set the protocol exception event.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'pxe' bit.
 * @return None.
 */
static inline void DCL_CAN_SetProtocolExceptionEvent(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->PSR.BIT.pxe = value;
}

/**
 * @brief Get the protocol exception event.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'pxe' bit.
 */
static inline unsigned int DCL_CAN_GetProtocolExceptionEvent(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.pxe;
}

/**
 * @brief Set the transmitter delay compensation value.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tdcv' field.
 * @return None.
 */
static inline void DCL_CAN_SetTransmitterDelayCompensation(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->PSR.BIT.tdcv = value;
}

/**
 * @brief Get the transmitter delay compensation value.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tdcv' field.
 */
static inline unsigned int DCL_CAN_GetTransmitterDelayCompensation(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->PSR.BIT.tdcv;
}

/**
 * @brief Set the transmitter delay compensation filter window length.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tdcf' field.
 * @return None.
 */
static inline void DCL_CAN_SetTxdelatCompensationFilterWinLength(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->TDCR.BIT.tdcf = value;
}

/**
 * @brief Get the transmitter delay compensation filter window length.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tdcf' field.
 */
static inline unsigned int DCL_CAN_GetTxdelatCompensationFilterWinLength(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TDCR.BIT.tdcf;
}

/**
 * @brief Set the transmitter delay compensation SSP offset.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tdco' field.
 * @return None.
 */
static inline void DCL_CAN_SetTxDelayCompensationSSPOffset(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->TDCR.BIT.tdco = value;
}

/**
 * @brief Get the transmitter delay compensation SSP offset.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tdco' field.
 */
static inline unsigned int DCL_CAN_GetTxDelayCompensationSSPOffset(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TDCR.BIT.tdco;
}

/**
 * @brief Get the current value of the interrupt state register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the interrupt state register.
 */
static inline unsigned int DCL_CAN_InterruptStateGet(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->IR.reg;
}

/**
 * @brief Clear the specified bits in the interrupt state register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param clearBit Bitmask of the bits to clear.
 * @return None.
 */
static inline void DCL_CAN_InterruptStateClear(CAN_RegStruct *canx, unsigned int clearBit)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->IR.reg = clearBit;
}

/**
 * @brief Set the value of the interrupt enable register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param regValue Value to set the interrupt enable register to.
 * @return None.
 */
static inline void DCL_CAN_InterruptEnableRegSet(CAN_RegStruct *canx, unsigned int regValue)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->IE.reg = regValue;
}

/**
 * @brief Get the current value of the interrupt enable register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the interrupt enable register.
 */
static inline unsigned int DCL_CAN_InterruptEnableRegGet(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->IE.reg;
}

/**
 * @brief Set the value of the interrupt line select register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param lineSelect Value to set the interrupt line select register to.
 * @return None.
 */
static inline void DCL_CAN_InterruptLineSelectSet(CAN_RegStruct *canx, unsigned int lineSelect)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->ILS.reg = lineSelect;
}

/**
 * @brief Get the current value of the interrupt line select register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the interrupt line select register.
 */
static inline unsigned int DCL_CAN_InterruptLineSelectGet(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->ILS.reg;
}

/**
 * @brief Enable interrupt line 0.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return None.
 */
static inline void DCL_CAN_InterruptLine0Enable(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->ILE.BIT.eint0 = BASE_CFG_ENABLE;
}

/**
 * @brief Disable interrupt line 0.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return None.
 */
static inline void DCL_CAN_InterruptLine0Disable(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->ILE.BIT.eint0 = BASE_CFG_DISABLE;
}

/**
 * @brief Enable interrupt line 1.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return None.
 */
static inline void DCL_CAN_InterruptLine1Enable(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->ILE.BIT.eint1 = BASE_CFG_ENABLE;
}

/**
 * @brief Disable interrupt line 1.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return None.
 */
static inline void DCL_CAN_InterruptLine1Disable(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->ILE.BIT.eint1 = BASE_CFG_DISABLE;
}

/**
 * @brief Set the Reject Remote Frames Extended.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rrfe' bit.
 * @return None.
 */
static inline void DCL_CAN_SetRejectRemoteFramesExtended(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->GFC.BIT.rrfe = value;
}

/**
 * @brief Get the current value of the Reject Remote Frames Extended.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rrfe' bit.
 */
static inline unsigned int DCL_CAN_GetRejectRemoteFramesExtended(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->GFC.BIT.rrfe;
}

/**
 * @brief Set the Reject Remote Frames Standard.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rrfs' bit.
 * @return None.
 */
static inline void DCL_CAN_SetRejectRemoteFramesStandard(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->GFC.BIT.rrfs = value;
}

/**
 * @brief Get the current value of the Reject Remote Frames Standard.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rrfs' bit.
 */
static inline unsigned int DCL_CAN_GetRejectRemoteFramesStandard(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->GFC.BIT.rrfs;
}

/**
 * @brief Set the Accept Non-matching Frames Extended.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'anfe' field.
 * @return None.
 */
static inline void DCL_CAN_SetAcceptNonMatchingFramesExtended(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x03);
    canx->GFC.BIT.anfe = value;
}

/**
 * @brief Get the current value of the Accept Non-matching Frames Extended.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'anfe' field.
 */
static inline unsigned int DCL_CAN_GetAcceptNonMatchingFramesExtended(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->GFC.BIT.anfe;
}

/**
 * @brief Set the Accept Non-matching Frames Standard.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'anfs' field.
 * @return None.
 */
static inline void DCL_CAN_SetAcceptNonMatchingFramesStandard(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x03);
    canx->GFC.BIT.anfs = value;
}

/**
 * @brief Get the current value of the Accept Non-matching Frames Standard.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'anfs' field.
 */
static inline unsigned int DCL_CAN_GetAcceptNonMatchingFramesStandard(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->GFC.BIT.anfs;
}

/**
 * @brief Set the Filter List Standard Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'flssa' field.
 * @return None.
 */
static inline void DCL_CAN_SidfcRegSetFlssa(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3FFF);
    canx->SIDFC.BIT.flssa = value;
}

/**
 * @brief Get the Filter List Standard Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'flssa' field.
 */
static inline unsigned int DCL_CAN_SidfcRegGetFlssa(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->SIDFC.BIT.flssa;
}

/**
 * @brief Set the Filter List Standard Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'lss' field.
 * @return None.
 */
static inline void DCL_CAN_SetListSizeStandard(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0xFF);
    canx->SIDFC.BIT.lss = value;
}

/**
 * @brief Get the current value of the List Size Standard.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'lss' field.
 */
static inline unsigned int DCL_CAN_GetListSizeStandard(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->SIDFC.BIT.lss;
}

/**
 * @brief Set the Filter List Extended Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'flesa' field.
 * @return None.
 */
static inline void DCL_CAN_SetFilterListExtendedStartAddress(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3FFF);
    canx->XIDFC.BIT.flesa = value;
}

/**
 * @brief Get the current value of the Filter List Extended Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'flesa' field.
 */
static inline unsigned int DCL_CAN_GetFilterListExtendedStartAddress(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->XIDFC.BIT.flesa;
}

/**
 * @brief Set the List Size Extended.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'lse' field.
 * @return None.
 */
static inline void DCL_CAN_SetListSizeExtended(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->XIDFC.BIT.lse = value;
}

/**
 * @brief Get the List Size Extended.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'lse' field.
 */
static inline unsigned int DCL_CAN_GetListSizeExtended(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->XIDFC.BIT.lse;
}

/**
 * @brief Set the Extended ID Mask.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'eidm' field.
 * @return None.
 */
static inline void DCL_CAN_SetExtendedIdMask(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x1FFFFFFF);
    canx->XIDAM.BIT.eidm = value;
}

/**
 * @brief Get the current value of the Extended ID Mask.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'eidm' field.
 */
static inline unsigned int DCL_CAN_GetExtendedIdMask(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->XIDAM.BIT.eidm;
}

/**
 * @brief Set the Buffer Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'bidx' field.
 * @return None.
 */
static inline void DCL_CAN_SetBufferIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->HPMS.BIT.bidx = value;
}

/**
 * @brief Get the current value of the Buffer Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'bidx' field.
 */
static inline unsigned int DCL_CAN_GetBufferIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->HPMS.BIT.bidx;
}

/**
 * @brief Set the Message Storage Indicator.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'msi' field.
 * @return None.
 */
static inline void DCL_CAN_SetMessageStorageIndicator(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3);
    canx->HPMS.BIT.msi = value;
}

/**
 * @brief Get the current value of the 'msi' field in the HPMS_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'msi' field.
 */
static inline unsigned int DCL_CAN_GetMessageStorageIndicator(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->HPMS.BIT.msi;
}

/**
 * @brief Set the Filter Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'fidx' field.
 * @return None.
 */
static inline void DCL_CAN_SetFilterIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->HPMS.BIT.fidx = value;
}

/**
 * @brief Get the current value of the Filter Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'fidx' field.
 */
static inline unsigned int DCL_CAN_GetFilterIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->HPMS.BIT.fidx;
}

/**
 * @brief Set the Filter List.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'flst' field.
 * @return None.
 */
static inline void DCL_CAN_SetFilterList(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->HPMS.BIT.flst = value;
}

/**
 * @brief Get the current value of the Filter List.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'flst' field.
 */
static inline unsigned int DCL_CAN_GetFilterList(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->HPMS.BIT.flst;
}

/**
 * @brief Set the NDAT1_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the reg.
 * @return None.
 */
static inline void DCL_CAN_Ndat1RegSet(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->NDAT1.reg = value;
}

/**
 * @brief Get the current value of the 'nd6' field in the NDAT1_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'nd6' field.
 */
static inline unsigned int DCL_CAN_Ndat1RegGet(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->NDAT1.reg;
}

/**
 * @brief Set the NDAT2_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the reg.
 * @return None.
 */
static inline void DCL_CAN_Ndat2RegSet(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->NDAT2.reg = value;
}

/**
 * @brief Get the current value of the 'nd6' field in the NDAT1_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'nd6' field.
 */
static inline unsigned int DCL_CAN_Ndat2RegGet(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->NDAT2.reg;
}

/**
 * @brief Set the Rx FIFO 0 Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0sa' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0StartAddress(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3FFF);
    canx->RXF0C.BIT.f0sa = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0sa' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0StartAddress(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0C.BIT.f0sa;
}

/**
 * @brief Set the Rx FIFO 0 Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0s' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0Size(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->RXF0C.BIT.f0s = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0s' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0Size(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0C.BIT.f0s;
}

/**
 * @brief Set the Rx FIFO 0 Watermark.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0wm' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0Watermark(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->RXF0C.BIT.f0wm = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Watermark.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0wm' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0Watermark(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0C.BIT.f0wm;
}

/**
 * @brief Set the Rx FIFO 0 Operation Mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0om' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0OperationMode(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->RXF0C.BIT.f0om = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Operation Mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0om' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0OperationMode(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0C.BIT.f0om;
}

/**
 * @brief Set the Rx FIFO 0 Fill Level.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0fl' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0FillLevel(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->RXF0S.BIT.f0fl = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Fill Level.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0fl' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0FillLevel(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0S.BIT.f0fl;
}

/**
 * @brief Set the Rx FIFO 0 Get Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0gi' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0GetIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->RXF0S.BIT.f0gi = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Get Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0gi' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0GetIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0S.BIT.f0gi;
}

/**
 * @brief Set the Rx FIFO 0 Put Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0pi' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0PutIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->RXF0S.BIT.f0pi = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Put Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0pi' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0PutIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0S.BIT.f0pi;
}

/**
 * @brief Set the Rx FIFO 0 Full.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0f' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0Full(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->RXF0S.BIT.f0f = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Full.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0f' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0Full(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0S.BIT.f0f;
}

/**
 * @brief Set the Rx FIFO 0 Message Lost.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rf0ml' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0MessageLost(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->RXF0S.BIT.rf0ml = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Message Lost.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rf0ml' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0MessageLost(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0S.BIT.rf0ml;
}

/**
 * @brief Set the Rx FIFO 0 Acknowledge Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0ai' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0AcknowledgeIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->RXF0A.BIT.f0ai = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Acknowledge Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0ai' field (0 to 63).
 */
static inline unsigned int DCL_CAN_GetFIFO0AcknowledgeIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF0A.BIT.f0ai;
}

/**
 * @brief Set the Rx Buffer Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rbsa' field.
 * @return None.
 */
static inline void DCL_CAN_SetRxBufferStartAddress(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3FFF);
    canx->RXBC.BIT.rbsa = value;
}

/**
 * @brief Get the current value of the Rx Buffer Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rbsa' field.
 */
static inline unsigned int DCL_CAN_GetRxBufferStartAddress(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXBC.BIT.rbsa;
}

/**
 * @brief Set the Rx FIFO 1 Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1sa' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1StartAddress(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3FFF);
    canx->RXF1C.BIT.f1sa = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1sa' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1StartAddress(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1C.BIT.f1sa;
}

/**
 * @brief Set the Rx FIFO 1 Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1s' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1Size(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->RXF1C.BIT.f1s = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1s' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1Size(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1C.BIT.f1s;
}

/**
 * @brief Set the Rx FIFO 1 Watermark.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1wm' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1Watermark(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->RXF1C.BIT.f1wm = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Watermark.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1wm' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1Watermark(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1C.BIT.f1wm;
}

/**
 * @brief Set the Rx FIFO 1 Operation Mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1om' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1OperationMode(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->RXF1C.BIT.f1om = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Operation Mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1om' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1OperationMode(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1C.BIT.f1om;
}

/**
 * @brief Set the Rx FIFO 1 Fill Level.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1fl' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1FillLevel(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7F);
    canx->RXF1S.BIT.f1fl = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Fill Level.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1fl' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1FillLevel(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1S.BIT.f1fl;
}

/**
 * @brief Set the Rx FIFO 1 Get Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1gi' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1GetIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->RXF1S.BIT.f1gi = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Get Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1gi' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1GetIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1S.BIT.f1gi;
}

/**
 * @brief Set the Rx FIFO 1 Put Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1pi' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1PutIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->RXF1S.BIT.f1pi = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Put Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1pi' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1PutIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1S.BIT.f1pi;
}

/**
 * @brief Set the Rx FIFO 1 Full.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1f' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1Full(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->RXF1S.BIT.f1f = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Full.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1f' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1Full(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1S.BIT.f1f;
}

/**
 * @brief Set the Rx FIFO 1 Message Lost.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rf1ml' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1MessageLost(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->RXF1S.BIT.rf1ml = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Message Lost.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rf1ml' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1MessageLost(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1S.BIT.rf1ml;
}

/**
 * @brief Set the Debug Message Status.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'dms' field.
 * @return None.
 */
static inline void DCL_CAN_SetDebugMessageStatus(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x03);
    canx->RXF1S.BIT.dms = value;
}

/**
 * @brief Get the current value of the Debug Message Status.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'dms' field.
 */
static inline unsigned int DCL_CAN_GetDebugMessageStatus(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1S.BIT.dms;
}

/**
 * @brief Set the Rx FIFO 1 Acknowledge Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1a' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1AcknowledgeIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->RXF1A.BIT.f1a = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Acknowledge Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1a' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1AcknowledgeIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXF1A.BIT.f1a;
}

/**
 * @brief Set the Rx FIFO 0 Data Field Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f0ds' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO0DataFieldSize(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7);
    canx->RXESC.BIT.f0ds = value;
}

/**
 * @brief Get the current value of the Rx FIFO 0 Data Field Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f0ds' field.
 */
static inline unsigned int DCL_CAN_GetFIFO0DataFieldSize(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXESC.BIT.f0ds;
}

/**
 * @brief Set the Rx FIFO 1 Data Field Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'f1ds' field.
 * @return None.
 */
static inline void DCL_CAN_SetFIFO1DataFieldSize(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7);
    canx->RXESC.BIT.f1ds = value;
}

/**
 * @brief Get the current value of the Rx FIFO 1 Data Field Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'f1ds' field.
 */
static inline unsigned int DCL_CAN_GetFIFO1DataFieldSize(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXESC.BIT.f1ds;
}

/**
 * @brief Set the Rx Buffer Data Field Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'rbds' field.
 * @return None.
 */
static inline void DCL_CAN_SetBufferDataFieldSize(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7);
    canx->RXESC.BIT.rbds = value;
}

/**
 * @brief Get the current value of the Rx Buffer Data Field Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'rbds' field.
 */
static inline unsigned int DCL_CAN_GetBufferDataFieldSize(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->RXESC.BIT.rbds;
}

/**
 * @brief Set the Tx Buffers Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tbsa' field.
 * @return None.
 */
static inline void DCL_CAN_SetTxBuffersStartAddress(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3FFF);
    canx->TXBC.BIT.tbsa = value;
}

/**
 * @brief Get the current value of the Tx Buffers Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tbsa' field.
 */
static inline unsigned int DCL_CAN_GetTxBuffersStartAddress(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBC.BIT.tbsa;
}

/**
 * @brief Set the Number of Dedicated Transmit Buffers.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'ndtb' field.
 * @return None.
 */
static inline void DCL_CAN_SetDedicatedTransmitBuffers(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->TXBC.BIT.ndtb = value;
}

/**
 * @brief Get the current value of the Number of Dedicated Transmit Buffers.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'ndtb' field.
 */
static inline unsigned int DCL_CAN_GetDedicatedTransmitBuffers(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBC.BIT.ndtb;
}

/**
 * @brief Set the Transmit FIFO/Queue Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tfqs' field.
 * @return None.
 */
static inline void DCL_CAN_SetTransmitFIFOQueueSize(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->TXBC.BIT.tfqs = value;
}

/**
 * @brief Get the current value of the Transmit FIFO/Queue Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tfqs' field.
 */
static inline unsigned int DCL_CAN_GetTransmitFIFOQueueSize(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBC.BIT.tfqs;
}

/**
 * @brief Set the Tx FIFO/Queue Mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tfqm' field.
 * @return None.
 */
static inline void DCL_CAN_SetTransmitFIFOQueueMode(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->TXBC.BIT.tfqm = value;
}

/**
 * @brief Get the current value of the Tx FIFO/Queue Mode.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tfqm' field.
 */
static inline unsigned int DCL_CAN_GetTransmitFIFOQueueMode(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBC.BIT.tfqm;
}

/**
 * @brief Set the Tx FIFO Free Level.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tffl' field.
 * @return None.
 */
static inline void DCL_CAN_SetTxFifoFreeLevel(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->TXFQS.BIT.tffl = value;
}

/**
 * @brief Get the current Tx FIFO Free Level.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tffl' field.
 */
static inline unsigned int DCL_CAN_GetTxFifoFreeLevel(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXFQS.BIT.tffl;
}

/**
 * @brief Set the Tx FIFO Get Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tfgi' field.
 * @return None.
 */
static inline void DCL_CAN_SetTxFifoGetIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x1F);
    canx->TXFQS.BIT.tfgi = value;
}

/**
 * @brief Get the current Tx FIFO Get Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tfgi' field.
 */
static inline unsigned int DCL_CAN_GetTxFifoGetIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXFQS.BIT.tfgi;
}

/**
 * @brief Set the Tx FIFO/Queue Put Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tfqp' field.
 * @return None.
 */
static inline void DCL_CAN_SetTxFifoQueuePutIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x1F);
    canx->TXFQS.BIT.tfqp = value;
}

/**
 * @brief Get the current Tx FIFO/Queue Put Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tfqp' field.
 */
static inline unsigned int DCL_CAN_GetTxFifoQueuePutIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXFQS.BIT.tfqp;
}

/**
 * @brief Set the Tx FIFO/Queue Full flag.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tfqf' field.
 * @return None.
 */
static inline void DCL_CAN_SetTxFifoFull(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->TXFQS.BIT.tfqf = value;
}

/**
 * @brief Get the current Tx FIFO/Queue Full status.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tfqf' field.
 */
static inline unsigned int DCL_CAN_GetTxFifoFull(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXFQS.BIT.tfqf;
}

/**
 * @brief Set the Tx Buffer Data Field Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tbds' field.
 * @return None.
 */
static inline void DCL_CAN_SetTxBufferDataSize(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x7);
    canx->TXESC.BIT.tbds = value;
}

/**
 * @brief Get the current Tx Buffer Data Field Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tbds' field.
 */
static inline unsigned int DCL_CAN_GetTxBufferDataSize(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXESC.BIT.tbds;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 */
static inline unsigned int DCL_CAN_TxbrpRegGet(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBRP.reg;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 */
static inline unsigned int DCL_CAN_TxbarRegGet(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBAR.reg;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 * @return None.
 */
static inline void DCL_CAN_TxbarRegSet(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->TXBAR.reg = value;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 */
static inline unsigned int DCL_CAN_TxbcrRegGet(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBCR.reg;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 * @return None.
 */
static inline void DCL_CAN_TxbcrRegSet(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->TXBCR.reg = value;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 */
static inline unsigned int DCL_CAN_TxbtoRegGet(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBTO.reg;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 */
static inline unsigned int DCL_CAN_TxbcfRegGet(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBCF.reg;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 * @return None.
 */
static inline void DCL_CAN_TxbtieRegSet(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->TXBTIE.reg = value;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 */
static inline unsigned int DCL_CAN_TxbtieRegGet(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBTIE.reg;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 * @return None.
 */
static inline void DCL_CAN_TxbcieRegSet(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->TXBCIE.reg = value;
}

/**
 * @brief Set the 'trp0' field in the TXBRP_REG register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'trp0' field.
 */
static inline unsigned int DCL_CAN_TxbcieRegGet(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXBCIE.reg;
}

/**
 * @brief Set the Event FIFO Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'efsa' field.
 * @return None.
 */
static inline void DCL_CAN_SetEventFifoStartAddress(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3FFF);
    canx->TXEFC.BIT.efsa = value;
}

/**
 * @brief Get the current Event FIFO Start Address.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'efsa' field.
 */
static inline unsigned int DCL_CAN_GetEventFifoStartAddress(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXEFC.BIT.efsa;
}

/**
 * @brief Set the Event FIFO Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'efs' field.
 * @return None.
 */
static inline void DCL_CAN_SetEventFifoSize(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->TXEFC.BIT.efs = value;
}

/**
 * @brief Get the current Event FIFO Size.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'efs' field.
 */
static inline unsigned int DCL_CAN_GetEventFifoSize(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXEFC.BIT.efs;
}

/**
 * @brief Set the Event FIFO Watermark.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'efwm' field.
 * @return None.
 */
static inline void DCL_CAN_SetEventFifoWatermark(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->TXEFC.BIT.efwm = value;
}

/**
 * @brief Get the current Event FIFO Watermark.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'efwm' field.
 */
static inline unsigned int DCL_CAN_GetEventFifoWatermark(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXEFC.BIT.efwm;
}

/**
 * @brief Set the Event FIFO Fill Level.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'effl' field.
 * @return None.
 */
static inline void DCL_CAN_SetEventFifoFillLevel(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x3F);
    canx->TXEFS.BIT.effl = value;
}

/**
 * @brief Get the current Event FIFO Fill Level.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'effl' field.
 */
static inline unsigned int DCL_CAN_GetEventFifoFillLevel(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXEFS.BIT.effl;
}

/**
 * @brief Set the Event FIFO Get Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'efgi' field.
 * @return None.
 */
static inline void DCL_CAN_SetEventFifoGetIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x1F);
    canx->TXEFS.BIT.efgi = value;
}

/**
 * @brief Get the current Event FIFO Get Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'efgi' field.
 */
static inline unsigned int DCL_CAN_GetEventFifoGetIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXEFS.BIT.efgi;
}

/**
 * @brief Set the Event FIFO Put Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'efpi' field.
 * @return None.
 */
static inline void DCL_CAN_SetEventFifoPutIndex(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(value <= 0x1F);
    canx->TXEFS.BIT.efpi = value;
}

/**
 * @brief Get the current Event FIFO Put Index.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'efpi' field.
 */
static inline unsigned int DCL_CAN_GetEventFifoPutIndex(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXEFS.BIT.efpi;
}

/**
 * @brief Set the Event FIFO Full flag.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'eff' field.
 * @return None.
 */
static inline void DCL_CAN_SetEventFifoFull(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->TXEFS.BIT.eff = value;
}

/**
 * @brief Get the current Event FIFO Full status.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'eff' field.
 */
static inline unsigned int DCL_CAN_GetEventFifoFull(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXEFS.BIT.eff;
}

/**
 * @brief Set the Tx Event FIFO Element Lost flag.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'tefel' field.
 * @return None.
 */
static inline void DCL_CAN_SetTxEventFifoElementLost(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->TXEFS.BIT.tefel = value;
}

/**
 * @brief Get the current Tx Event FIFO Element Lost status.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'tefel' field.
 */
static inline unsigned int DCL_CAN_GetTxEventFifoElementLost(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->TXEFS.BIT.tefel;
}


#ifdef CANFD_ENABLE
/**
 * @brief Set the data bit timing parameters in the DBTP register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param dBitTiming Structure containing data bit timing parameters (DSJW, DTseg2, DTseg1, DBRP).
 * @return None.
 */
static inline void DCL_CAN_SetDataBitTiming(CAN_RegStruct *canx, CAN_DataBitTiming dBitTiming)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));

    unsigned int val = canx->DBTP.reg & DATA_TIMING_TDC_MASK;  // save TDC mode
    val |= dBitTiming.dsjw;
    val |= (dBitTiming.dtseg2) << DATA_TIMING_SEG2_OFFSET;
    val |= (dBitTiming.dtseg1) << DATA_TIMING_SEG1_OFFSET;
    val |= (dBitTiming.dbrp) << DATA_TIMING_BRP_OFFSET;
    canx->DBTP.reg = val;
}

/**
 * @brief Get the current data bit timing parameters from the DBTP register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Structure containing data bit timing parameters (DSJW, DTseg2, DTseg1, DBRP).
 */
static inline void DCL_CAN_GetDataBitTiming(CAN_RegStruct *canx, CAN_DataBitTiming *dBitTiming)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_ASSERT_PARAM(dBitTiming != NULL);

    /* Read the value from the Data Bit Timing Prescaler (DBTP) register */
    unsigned int dbtpReg = canx->DBTP.reg;
    dBitTiming->dsjw = dbtpReg;
    dBitTiming->dtseg2 = dbtpReg >> DATA_TIMING_SEG2_OFFSET;
    dBitTiming->dtseg1 = dbtpReg >> DATA_TIMING_SEG1_OFFSET;
    /* Extract the Data Bit Rate Prescaler (DBRP) by shifting the DBTP register. */
    dBitTiming->dbrp = dbtpReg >> DATA_TIMING_BRP_OFFSET;
}

/**
 * @brief Enable transmitter delay compensation (TDC) in the DBTP register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return None.
 */
static inline void DCL_CAN_EnableTransmitterDelayCompensation(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->DBTP.BIT.tdc = BASE_CFG_SET;
}

/**
 * @brief Disable transmitter delay compensation (TDC) in the DBTP register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return None.
 */
static inline void DCL_CAN_DisableTransmitterDelayCompensation(CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    canx->DBTP.BIT.tdc = BASE_CFG_UNSET;
}

/**
 * @brief Set the 'fdoe' bit in the CCCR register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'fdoe' bit (0 or 1).
 * @return None.
 */
static inline void DCL_CAN_EnableFDOperation(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.fdoe = value;
}

/**
 * @brief Get the current status of CAN FD operation.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'fdoe' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetFDOperation(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.fdoe;
}

/**
 * @brief Enable bit rate switching functionality.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param value Value to set the 'brse' bit, 0: disable, 1: enable.
 * @return None.
 */
static inline void DCL_CAN_EnableBitRateSwitch(CAN_RegStruct *canx, unsigned int value)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_PARAM_CHECK_NO_RET(IsOneBitCfgValue(value));
    canx->CCCR.BIT.brse = value;
}

/**
 * @brief Get the current status of bit rate switching.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @return Current value of the 'brse' bit, 0: disable, 1: enable.
 */
static inline unsigned int DCL_CAN_GetBitRateSwitch(const CAN_RegStruct *canx)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    return canx->CCCR.BIT.brse;
}

#endif

/**
 * @brief Set the nominal bit timing parameters in the NBTP register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param nBitTiming Structure containing nominal bit timing parameters (NTseg2, NTseg1, NBRP, NSJW).
 * @return None.
 */
static inline void DCL_CAN_SetNominalBitTiming(CAN_RegStruct *canx, CAN_NominalBitTiming nBitTiming)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));

    unsigned int val = nBitTiming.ntseg2;
    val |= (nBitTiming.ntseg1) << NOMINAL_TIMING_SEG1_OFFSET;
    val |= (nBitTiming.nbrp) << NOMINAL_TIMING_BRP_OFFSET;
    val |= (nBitTiming.nsjw) << NOMINAL_TIMING_SJW_OFFSET;
    /* Write the combined timing value to the Nominal Bit Timing Prescaler register */
    canx->NBTP.reg = val;
}

/**
 * @brief Get the current nominal bit timing parameters from the NBTP register.
 * @param canx Pointer to the CAN_RegStruct instance.
 * @param nBitTiming Structure containing nominal bit timing parameters (NTseg2, NTseg1, NBRP, NSJW).
 */
static inline void DCL_CAN_GetNominalBitTiming(CAN_RegStruct *canx, CAN_NominalBitTiming *nBitTiming)
{
    CAN_ASSERT_PARAM(IsCANInstance(canx));
    CAN_ASSERT_PARAM(nBitTiming != NULL);

    /* Read the value from the Nominal Bit Timing Prescaler register */
    unsigned int nbtpReg = canx->NBTP.reg;
    nBitTiming->ntseg2 = nbtpReg;
    nBitTiming->ntseg1 = nbtpReg >> NOMINAL_TIMING_SEG1_OFFSET;
    nBitTiming->nbrp = nbtpReg >> NOMINAL_TIMING_BRP_OFFSET;
    /* Extract the Nominal Synchronization Jump Width by shifting the NBTP register right. */
    nBitTiming->nsjw = nbtpReg >> NOMINAL_TIMING_SJW_OFFSET;
}
#endif
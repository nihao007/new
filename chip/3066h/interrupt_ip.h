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
  * @file    interrupt_ip.h
  * @author  MCU Driver Team
  * @brief   interrupt module driver.
  *          This file define the interrupt number
  */

#ifndef MCUMagicTag_INTERRUPT_IP_H
#define MCUMagicTag_INTERRUPT_IP_H

/* Typedef definitions -------------------------------------------------------*/
#define MSTATUS_MIE            0x00000008U       /**< mie in mstatus */
#define MSTATUS_MPIE           0x00000080U       /**< mpie in mstatus */
#define UINT32_CUT_MASK        0xFFFFFFFFU

#define IRQ_PRIO_HIGHEST 15   /**< Highest priority of a hardware interrupt. */
#define IRQ_PRIO_LOWEST  0    /**< Lowest priority of a hardware interrupt. */

/**
 * @brief Count of system interrupt vector.
 *        The number of standard interrupts inside the CPU. The interrupt number
 *        is 0~15. The software interrupt nesting scheme cannot use standard
 *        interrupts, which means that external system integration will ensure
 *        that no standard interrupts will be triggered.
 */
#define IRQ_VECTOR_CNT               16

/**
 * @brief Count of local interrupt vector 0 - 5, enabled by CSR mie 26 -31 bit.
 */
#define IRQ_MIE_VECTOR_CNT            6

/**
 * @brief Count of IRQ controlled by CSR mie
 */
#define IRQ_MIE_TOTAL_CNT        (IRQ_VECTOR_CNT + IRQ_MIE_VECTOR_CNT)
#define IRQ_LOCIEN1_OFFSET       64
#define IRQ_LOCIEN2_OFFSET       96
#define IRQ_LOCIEN3_OFFSET       128

/**
  * @brief rv_custom_csr
  *        locipri0~15 are registers that control the priority of interrupts,
  *        and every 4 bits control the priority of an interrupt
  */
#define LOCIPRI0                0xBC0
#define LOCIPRI1                0xBC1
#define LOCIPRI2                0xBC2
#define LOCIPRI3                0xBC3
#define LOCIPRI4                0xBC4
#define LOCIPRI5                0xBC5
#define LOCIPRI6                0xBC6
#define LOCIPRI7                0xBC7
#define LOCIPRI8                0xBC8
#define LOCIPRI9                0xBC9
#define LOCIPRI10               0xBCA
#define LOCIPRI11               0xBCB
#define LOCIPRI12               0xBCC
#define LOCIPRI13               0xBCD
#define LOCIPRI14               0xBCE
#define LOCIPRI15               0xBCF

#define LOCIPRI(x)              LOCIPRI##x

/**
  * @brief locien0~3 are registers that control interrupt enable
  */
#define LOCIEN0                 0xBE0
#define LOCIEN1                 0xBE1
#define LOCIEN2                 0xBE2
#define LOCIEN3                 0xBE3

/**
  * @brief locipd0~3 are registers that control the interrupt flag bit. Each bit
  *        controls an interrupt. If the corresponding bit bit is 1, it means the
  *        corresponding interrupt is triggered.
  */
#define LOCIPD0                 0xBE8
#define LOCIPD1                 0xBE9
#define LOCIPD2                 0xBEA
#define LOCIPD3                 0xBEB

/**
  * @brief Locipclr is the register that clears the interrupt flag bit, and the
  *        corresponding interrupt number is assigned to the locipclr register,
  *        and the hardware will clear the corresponding interrupt flag bit, that
  *        is, the corresponding locipd bit is set
  */
#define LOCIPCLR                0xBF0

/**
  * @brief The maximum number of interrupts supported, excluding 26 internal standard
  *          interrupts, up to 230 external non-standard interrupts can be supported
  */
#define IRQ_NUM                 256

/* ---------- Interrupt Number Definition ----------------------------------- */
typedef enum {
    IRQ_SOFTWARE             = 16,   /* The first 0~15 interrupts are the internal standard interrupts of the CPU,
	                                      and the customizable external non-standard interrupts start from 16 */
    IRQ_WWDG1                = 27,

    IRQ_UART0                = 18,
    IRQ_UART1                = 19,
    IRQ_UART2                = 20,
    IRQ_MTIMER               = 21,    /* IRQ_MTIMER : 21, cpu0 is mtimer0, cpu1 is mtimer1, cpu2 is mtimer2. */
    IRQ_TIMER0               = 22,
    IRQ_TIMER1               = 23,
    IRQ_TIMER2               = 24,
    IRQ_TIMER3               = 25,
    IRQ_GPT0_INT             = 26,
    IRQ_GPT0_PRD_INT         = 27,
    IRQ_GPT1_INT             = 28,
    IRQ_GPT1_PRD_INT         = 29,
    IRQ_WWDG                 = 30,    /* IRQ_WWDG : 30, cpu0 is wwdg0, cpu1 is wwdg1, cpu2 is wwdg2. */
    IRQ_IWDG                 = 31,
    IRQ_I2C0                 = 32,
    IRQ_I2C1                 = 33,
    IRQ_SPI0                 = 34,
    IRQ_SPI1                 = 35,
    IRQ_CAN1_INT0            = 36,
    IRQ_CAN1_INT1            = 37,
    IRQ_APT0_EVT             = 38,
    IRQ_APT0_TMR             = 39,
    IRQ_APT1_EVT             = 40,
    IRQ_APT1_TMR             = 41,
    IRQ_APT2_EVT             = 42,
    IRQ_APT2_TMR             = 43,
    IRQ_APT3_EVT             = 44,
    IRQ_APT3_TMR             = 45,
    IRQ_APT4_EVT             = 46,
    IRQ_APT4_TMR             = 47,
    IRQ_APT5_EVT             = 48,
    IRQ_APT5_TMR             = 49,
    IRQ_APT6_EVT             = 50,
    IRQ_APT6_TMR             = 51,
    IRQ_APT7_EVT             = 52,
    IRQ_APT7_TMR             = 53,
    IRQ_APT8_EVT             = 54,
    IRQ_APT8_TMR             = 55,
    IRQ_UART3                = 56,
    IRQ_UART4                = 57,
    IRQ_CMM                  = 58,
    IRQ_CFD                  = 59,
    IRQ_CAPM0                = 60,
    IRQ_CAPM1                = 61,
    IRQ_CAPM2                = 62,
    IRQ_QDM0                 = 63,
    IRQ_QDM1                 = 64,
    IRQ_QDM2                 = 65,
    IRQ_QDM3                 = 66,
    IRQ_DMA_TC               = 67,
    IRQ_DMA_ERR              = 68,
    IRQ_SYSRAM_PARITY_ERR    = 69,
    IRQ_UART5                = 70,
    IRQ_EFC                  = 71,
    IRQ_EFC_ERR              = 72,
    IRQ_DMA1_TC              = 73,
    IRQ_DMA1_ERR             = 74,
    IRQ_PVD                  = 75,
    IRQ_ACMP0_INT            = 76,
    IRQ_ACMP1_INT            = 77,
    IRQ_ACMP2_INT            = 78,
    IRQ_UART6                = 79,
    IRQ_UART7                = 80,
    IRQ_ADC0_EVENT           = 81,     /* IRQ_ADC0_EVENT : 81, cpu0 is EVENT[0], cpu1 is EVENT[1], cpu2 is EVENT[2]. */
    IRQ_ADC0_ERR             = 82,     /* IRQ_ADC0_ERR : 82, cpu0 is ERR[0], cpu1 is ERR[1], cpu2 is ERR[2]. */
    IRQ_ADC0_INT0            = 83,
    IRQ_ADC0_INT1            = 84,
    IRQ_ADC0_INT2            = 85,
    IRQ_ADC0_INT3            = 86,
    IRQ_ADC1_EVENT           = 87,     /* IRQ_ADC1_EVENT : 87, cpu0 is EVENT[0], cpu1 is EVENT[1], cpu2 is EVENT[2]. */
    IRQ_ADC1_ERR             = 88,     /* IRQ_ADC1_ERR : 88, cpu0 is ERR[0], cpu1 is ERR[1], cpu2 is ERR[2]. */
    IRQ_ADC1_INT0            = 89,
    IRQ_ADC1_INT1            = 90,
    IRQ_ADC1_INT2            = 91,
    IRQ_ADC1_INT3            = 92,
    IRQ_ADC2_EVENT           = 93,     /* IRQ_ADC2_EVENT : 93, cpu0 is EVENT[0], cpu1 is EVENT[1], cpu2 is EVENT[2]. */
    IRQ_ADC2_ERR             = 94,     /* IRQ_ADC2_ERR : 94, cpu0 is ERR[0], cpu1 is ERR[1], cpu2 is ERR[2]. */
    IRQ_ADC2_INT0            = 95,
    IRQ_ADC2_INT1            = 96,
    IRQ_ADC2_INT2            = 97,
    IRQ_ADC2_INT3            = 98,
    IRQ_GPIO0                = 99,
    IRQ_GPIO1                = 100,
    IRQ_GPIO2                = 101,
    IRQ_GPIO3                = 102,
    IRQ_GPIO4                = 103,
    IRQ_GPIO5                = 104,
    IRQ_GPIO6                = 105,
    IRQ_GPIO7                = 106,
    IRQ_GPIO8                = 107,
    IRQ_GPIO9                = 108,
    IRQ_GPIO10               = 109,
    IRQ_GPIO11               = 110,
    IRQ_GPIO12               = 111,
    IRQ_GPIO13               = 112,
    IRQ_GPIO14               = 113,
    IRQ_GPIO15               = 114,
    IRQ_GPIO16               = 115,
    IRQ_I2C2                 = 117,
    IRQ_SPI2                 = 118,
    IRQ_SRM                  = 119,
    IRQ_TIMER4               = 120,
    IRQ_TIMER5               = 121,
    IRQ_APT9_EVT             = 122,
    IRQ_APT9_TMR             = 123,
    IRQ_APT10_EVT            = 124,
    IRQ_APT10_TMR            = 125,
    IRQ_APT11_EVT            = 126,
    IRQ_APT11_TMR            = 127,
    IRQ_CMM1                 = 128,
    IRQ_FMC                  = 129,
    IRQ_SYSRAM_ECC_ERR       = 130,
    IRQ_IPC                  = 131,
    IRQ_ADC3_EVENT           = 132,  /* IRQ_ADC3_EVENT : 132, cpu0 is EVENT[0], cpu1 is EVENT[1], cpu2 is EVENT[2]. */
    IRQ_ADC3_ERR             = 133,  /* IRQ_ADC3_ERR : 133, cpu0 is ERR[0], cpu1 is ERR[1], cpu2 is ERR[2]. */
    IRQ_ADC3_INT0            = 134,
    IRQ_ADC3_INT1            = 135,
    IRQ_ADC3_INT2            = 136,
    IRQ_ADC3_INT3            = 137,
    IRQ_ACMP3_INT            = 138,
    IRQ_ACMP4_INT            = 139,
    IRQ_ACMP5_INT            = 140,
    IRQ_ACMP6_INT            = 141,
    IRQ_ACMP7_INT            = 142,

    IRQ_CAN0_INT0            = 144,
    IRQ_CAN0_INT1            = 145,
    IRQ_APT12_EVT            = 146,
    IRQ_APT12_TMR            = 147,
    IRQ_APT13_EVT            = 148,
    IRQ_APT13_TMR            = 149,
    IRQ_APT14_EVT            = 150,
    IRQ_APT14_TMR            = 151,
    IRQ_APT15_EVT            = 152,
    IRQ_APT15_TMR            = 153,
    IRQ_CAN2_INT0            = 154,
    IRQ_CAN2_INT1            = 155,
    IRQ_PERI_ACCESS_ERR      = 154,
    IRQ_MAX,                 /**< The maximum number of interrupts currently supported */
} IRQ_ID;

#endif   /* MCUMagicTag_INTERRUPT_IP_H */
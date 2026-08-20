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
  * @file      baseaddr.h
  * @author    MCU Driver Team
  * @brief     Definition of MCU register baseaddress
  */

/* Define to prevent recursive inclusion ------------------------------------- */
#ifndef McuMagicTag_BASEADDR_H
#define McuMagicTag_BASEADDR_H

#define CRG_BASE          (void *)0x10000000
#define CMM_BASE          (void *)0x10010000
#define CMM1_BASE         (void *)0x10011000
#define CFD_BASE          (void *)0x10020000
#define SYSCTRL0_BASE     (void *)0x10100000
#define SYSCTRL1_BASE     (void *)0x10100000 /* The base address offset is configured in the register. */
#define UART0_BASE        (void *)0x14000000
#define UART1_BASE        (void *)0x14001000
#define UART2_BASE        (void *)0x14002000
#define UART3_BASE        (void *)0x14003000
#define UART4_BASE        (void *)0x14004000
#define UART5_BASE        (void *)0x14005000
#define UART6_BASE        (void *)0x14006000
#define UART7_BASE        (void *)0x14007000

#define I2C0_BASE         (void *)0x14100000
#define I2C1_BASE         (void *)0x14101000
#define I2C2_BASE         (void *)0x14102000
#define SPI0_BASE         (void *)0x14200000
#define SPI1_BASE         (void *)0x14201000
#define SPI2_BASE         (void *)0x14202000
#define TIMER0_BASE       (void *)0x14300000
#define TIMER1_BASE       (void *)0x14301000
#define TIMER2_BASE       (void *)0x14302000
#define TIMER3_BASE       (void *)0x14303000
#define TIMER4_BASE       (void *)0x14304000
#define TIMER5_BASE       (void *)0x14305000

#define SYSTICK_BASE      (void *)0x14380000
#define SYSTICK1_BASE     (void *)0x14381000
#define SYSTICK2_BASE     (void *)0x14382000
#define JTAG_LOCK_BASE    (void *)0x18010000

#define WWDG0_BASE         (void *)0x14400000
#define IWDG_BASE         (void *)0x14401000
#define WWDG1_BASE        (void *)0x14402000
#define WWDG2_BASE        (void *)0x14403000
#define GPIO0_BASE        (void *)0x14500000
#define GPIO1_BASE        (void *)0x14501000
#define GPIO2_BASE        (void *)0x14502000
#define GPIO3_BASE        (void *)0x14503000
#define GPIO4_BASE        (void *)0x14504000
#define GPIO5_BASE        (void *)0x14505000
#define GPIO6_BASE        (void *)0x14506000
#define GPIO7_BASE        (void *)0x14507000
#define GPIO8_BASE        (void *)0x14508000
#define GPIO9_BASE        (void *)0x14509000
#define GPIO10_BASE       (void *)0x1450A000
#define GPIO11_BASE       (void *)0x1450B000
#define GPIO12_BASE       (void *)0x1450C000
#define GPIO13_BASE       (void *)0x1450D000
#define GPIO14_BASE       (void *)0x1450E000
#define GPIO15_BASE       (void *)0x1450F000
#define GPIO16_BASE       (void *)0x14510000

#define CAN0_BASE         (void *)0x14680000
#define CAN1_BASE         (void *)0x14684000
#define CAN2_BASE         (void *)0x14688000

#define GPT0_BASE         (void *)0x14700000
#define GPT1_BASE         (void *)0x14701000

#define EFC_BASE          (void *)0x14710000
#define FOTPC_BASE        (void *)0x14720000

#define PMC_BASE          (void *)0x147E0000
#define IOCMG_BASE        (void *)0x147F0000
#define CRC_BASE          (void *)0x14800000
#define OSC_SAR_TRIM_BASE (void *)0x14810000
#define IPCM_BASE         (void *)0x14820000
#define APT0_BASE         (void *)0x14A00000
#define APT1_BASE         (void *)0x14A01000
#define APT2_BASE         (void *)0x14A02000
#define APT3_BASE         (void *)0x14A03000
#define APT4_BASE         (void *)0x14A04000
#define APT5_BASE         (void *)0x14A05000
#define APT6_BASE         (void *)0x14A06000
#define APT7_BASE         (void *)0x14A07000
#define APT8_BASE         (void *)0x14A08000
#define APT9_BASE         (void *)0x14A09000
#define APT10_BASE        (void *)0x14A0A000
#define APT11_BASE        (void *)0x14A0B000
#define APT12_BASE        (void *)0x14A0C000
#define APT13_BASE        (void *)0x14A0D000
#define APT14_BASE        (void *)0x14A0E000
#define APT15_BASE        (void *)0x14A0F000

#define XBAR_BASE         (void *)0x14A1F000
#define ADDLL_BASE        (void *)0x14A1FF00

#define SRM_BASE          (void *)0x14A20000
#define CAPM0_BASE        (void *)0x14B00000
#define CAPM1_BASE        (void *)0x14B01000
#define CAPM2_BASE        (void *)0x14B02000
#define CAPM_COMM_BASE    (void *)0x14B03000

#define QDM0_BASE         (void *)0x14C00000
#define QDM1_BASE         (void *)0x14C01000
#define QDM2_BASE         (void *)0x14C02000
#define QDM3_BASE         (void *)0x14C03000
#define ADC0_BASE         (void *)0x18000000
#define ADC1_BASE         (void *)0x18001000
#define ADC2_BASE         (void *)0x18002000
#define ADC3_BASE         (void *)0x18003000
#define VREF_BASE         (void *)0x18100000
#define PGA0_BASE         (void *)0x18200000
#define PGA1_BASE         (void *)0x18201000
#define PGA2_BASE         (void *)0x18202000
#define PGA3_BASE         (void *)0x18203000
#define ACMP0_BASE        (void *)0x18300000
#define ACMP1_BASE        (void *)0x18301000
#define ACMP2_BASE        (void *)0x18302000
#define ACMP3_BASE        (void *)0x18303000
#define ACMP4_BASE        (void *)0x18304000
#define ACMP5_BASE        (void *)0x18305000
#define ACMP6_BASE        (void *)0x18306000
#define ACMP7_BASE        (void *)0x18307000
#define DAC0_BASE         (void *)0x18400000
#define DAC1_BASE         (void *)0x18401000
#define DAC2_BASE         (void *)0x18402000
#define DAC3_BASE         (void *)0x18403000
#define DAC4_BASE         (void *)0x18404000
#define DAC5_BASE         (void *)0x18405000
#define DAC6_BASE         (void *)0x18406000
#define DAC7_BASE         (void *)0x18407000
#define DACBUFF_BASE      (void *)0x18408000

#define TSENSOR_BASE      (void *)0x18500000
#define ANA_CTRL_TOP_BASE (void *)0x18600000

#define DMA_BASE          (void *)0x1C000000
#define DMA_CHANNEL0_BASE (void *)0x1C000100
#define DMA_CHANNEL1_BASE (void *)0x1C000120
#define DMA_CHANNEL2_BASE (void *)0x1C000140
#define DMA_CHANNEL3_BASE (void *)0x1C000160
#define DMA_CHANNEL4_BASE (void *)0x1C000180
#define DMA_CHANNEL5_BASE (void *)0x1C0001A0
#define DMA1_BASE          (void *)0x1C001000
#define DMA1_CHANNEL0_BASE (void *)0x1C001100
#define DMA1_CHANNEL1_BASE (void *)0x1C001120
#define DMA1_CHANNEL2_BASE (void *)0x1C001140
#define DMA1_CHANNEL3_BASE (void *)0x1C001160
#define DMA1_CHANNEL4_BASE (void *)0x1C001180
#define DMA1_CHANNEL5_BASE (void *)0x1C0011A0
#define FMC_BASE          (void *)0x1C0E0000

#define CRG ((CRG_RegStruct *)CRG_BASE)
#define CMM ((CMM_RegStruct *)CMM_BASE)
#define CMM1 ((CMM_RegStruct *)CMM1_BASE)
#define CFD ((CFD_RegStruct *)CFD_BASE)
#define SYSCTRL0 ((SYSCTRL0_RegStruct *)SYSCTRL0_BASE)
#define SYSCTRL1 ((SYSCTRL1_RegStruct *)SYSCTRL1_BASE)
#define JTAG_LOCK ((JTAG_LockRegStruct *)JTAG_LOCK_BASE)
#define UART0 ((UART_RegStruct *)UART0_BASE)
#define UART1 ((UART_RegStruct *)UART1_BASE)
#define UART2 ((UART_RegStruct *)UART2_BASE)
#define UART3 ((UART_RegStruct *)UART3_BASE)
#define UART4 ((UART_RegStruct *)UART4_BASE)
#define UART5 ((UART_RegStruct *)UART5_BASE)
#define UART6 ((UART_RegStruct *)UART6_BASE)
#define UART7 ((UART_RegStruct *)UART7_BASE)
#define I2C0 ((I2C_RegStruct *)I2C0_BASE)
#define I2C1 ((I2C_RegStruct *)I2C1_BASE)
#define I2C2 ((I2C_RegStruct *)I2C2_BASE)
#define SPI0 ((SPI_RegStruct *)SPI0_BASE)
#define SPI1 ((SPI_RegStruct *)SPI1_BASE)
#define SPI2 ((SPI_RegStruct *)SPI2_BASE)
#define TIMER0 ((TIMER_RegStruct *)TIMER0_BASE)
#define TIMER1 ((TIMER_RegStruct *)TIMER1_BASE)
#define TIMER2 ((TIMER_RegStruct *)TIMER2_BASE)
#define TIMER3 ((TIMER_RegStruct *)TIMER3_BASE)
#define TIMER4 ((TIMER_RegStruct *)TIMER4_BASE)
#define TIMER5 ((TIMER_RegStruct *)TIMER5_BASE)
#define SYSTICK ((SYSTICK_RegStruct *)SYSTICK_BASE)
#define SYSTICK1 ((SYSTICK_RegStruct *)SYSTICK1_BASE)
#define SYSTICK2 ((SYSTICK_RegStruct *)SYSTICK2_BASE)
#define WWDG0 ((WWDG_RegStruct *)WWDG0_BASE)
#define IWDG ((IWDG_RegStruct *)IWDG_BASE)
#define WWDG1 ((WWDG_RegStruct *)WWDG1_BASE)
#define WWDG2 ((WWDG_RegStruct *)WWDG2_BASE)
#define GPIO0 ((GPIO_RegStruct *)GPIO0_BASE)
#define GPIO1 ((GPIO_RegStruct *)GPIO1_BASE)
#define GPIO2 ((GPIO_RegStruct *)GPIO2_BASE)
#define GPIO3 ((GPIO_RegStruct *)GPIO3_BASE)
#define GPIO4 ((GPIO_RegStruct *)GPIO4_BASE)
#define GPIO5 ((GPIO_RegStruct *)GPIO5_BASE)
#define GPIO6 ((GPIO_RegStruct *)GPIO6_BASE)
#define GPIO7 ((GPIO_RegStruct *)GPIO7_BASE)
#define GPIO8 ((GPIO_RegStruct *)GPIO8_BASE)
#define GPIO9 ((GPIO_RegStruct *)GPIO9_BASE)
#define GPIO10 ((GPIO_RegStruct *)GPIO10_BASE)
#define GPIO11 ((GPIO_RegStruct *)GPIO11_BASE)
#define GPIO12 ((GPIO_RegStruct *)GPIO12_BASE)
#define GPIO13 ((GPIO_RegStruct *)GPIO13_BASE)
#define GPIO14 ((GPIO_RegStruct *)GPIO14_BASE)
#define GPIO15 ((GPIO_RegStruct *)GPIO15_BASE)
#define GPIO16 ((GPIO_RegStruct *)GPIO16_BASE)

#define CAN0 ((CAN_RegStruct *)CAN0_BASE)
#define CAN1 ((CAN_RegStruct *)CAN1_BASE)
#define CAN2 ((CAN_RegStruct *)CAN2_BASE)

#define GPT0 ((GPT_RegStruct *)GPT0_BASE)
#define GPT1 ((GPT_RegStruct *)GPT1_BASE)
#define EFC ((EFC_RegStruct *)EFC_BASE)
#define PMC ((PMC_RegStruct *)PMC_BASE)
#define IOCMG ((IOConfig_RegStruct *)IOCMG_BASE)
#define CRC ((CRC_RegStruct *)CRC_BASE)
#define IPCM ((IPCM_RegStruct *)IPCM_BASE)
#define APT0 ((APT_RegStruct *)APT0_BASE)
#define APT1 ((APT_RegStruct *)APT1_BASE)
#define APT2 ((APT_RegStruct *)APT2_BASE)
#define APT3 ((APT_RegStruct *)APT3_BASE)
#define APT4 ((APT_RegStruct *)APT4_BASE)
#define APT5 ((APT_RegStruct *)APT5_BASE)
#define APT6 ((APT_RegStruct *)APT6_BASE)
#define APT7 ((APT_RegStruct *)APT7_BASE)
#define APT8 ((APT_RegStruct *)APT8_BASE)
#define APT9 ((APT_RegStruct *)APT9_BASE)
#define APT10 ((APT_RegStruct *)APT10_BASE)
#define APT11 ((APT_RegStruct *)APT11_BASE)
#define APT12 ((APT_RegStruct *)APT12_BASE)
#define APT13 ((APT_RegStruct *)APT13_BASE)
#define APT14 ((APT_RegStruct *)APT14_BASE)
#define APT15 ((APT_RegStruct *)APT15_BASE)
#define XBAR  ((XBAR_RegStruct *)XBAR_BASE)
#define ADDLL ((ADDLL_RegStruct *)ADDLL_BASE)
#define SRM ((SRM_RegStruct *)SRM_BASE)
#define CAPM0 ((CAPM_RegStruct *)CAPM0_BASE)
#define CAPM1 ((CAPM_RegStruct *)CAPM1_BASE)
#define CAPM2 ((CAPM_RegStruct *)CAPM2_BASE)
#define CAPM_COMM ((CAPM_COMM_RegStruct *)CAPM_COMM_BASE)
#define QDM0 ((QDM_RegStruct *)QDM0_BASE)
#define QDM1 ((QDM_RegStruct *)QDM1_BASE)
#define QDM2 ((QDM_RegStruct *)QDM2_BASE)
#define QDM3 ((QDM_RegStruct *)QDM3_BASE)
#define ADC0 ((ADC_RegStruct *)ADC0_BASE)
#define ADC1 ((ADC_RegStruct *)ADC1_BASE)
#define ADC2 ((ADC_RegStruct *)ADC2_BASE)
#define ADC3 ((ADC_RegStruct *)ADC3_BASE)
#define VREF ((VREF_RegStruct *)VREF_BASE)
#define PGA0 ((PGA_RegStruct *)PGA0_BASE)
#define PGA1 ((PGA_RegStruct *)PGA1_BASE)
#define PGA2 ((PGA_RegStruct *)PGA2_BASE)
#define PGA3 ((PGA_RegStruct *)PGA3_BASE)
#define ACMP0 ((ACMP_RegStruct *)ACMP0_BASE)
#define ACMP1 ((ACMP_RegStruct *)ACMP1_BASE)
#define ACMP2 ((ACMP_RegStruct *)ACMP2_BASE)
#define ACMP3 ((ACMP_RegStruct *)ACMP3_BASE)
#define ACMP4 ((ACMP_RegStruct *)ACMP4_BASE)
#define ACMP5 ((ACMP_RegStruct *)ACMP5_BASE)
#define ACMP6 ((ACMP_RegStruct *)ACMP6_BASE)
#define ACMP7 ((ACMP_RegStruct *)ACMP7_BASE)
#define DAC0 ((DAC_RegStruct *)DAC0_BASE)
#define DAC1 ((DAC_RegStruct *)DAC1_BASE)
#define DAC2 ((DAC_RegStruct *)DAC2_BASE)
#define DAC3 ((DAC_RegStruct *)DAC3_BASE)
#define DAC4 ((DAC_RegStruct *)DAC4_BASE)
#define DAC5 ((DAC_RegStruct *)DAC5_BASE)
#define DAC6 ((DAC_RegStruct *)DAC6_BASE)
#define DAC7 ((DAC_RegStruct *)DAC7_BASE)
#define DACBUFF ((DAC_RegStruct *)DACBUFF_BASE)
#define TSENSOR ((TSENSOR_RegStruct *)TSENSOR_BASE)
#define DMA ((DMA_RegStruct *)DMA_BASE)
#define DMA_CHANNEL0 ((DMA_ChannelRegStruct *)DMA_CHANNEL0_BASE)
#define DMA_CHANNEL1 ((DMA_ChannelRegStruct *)DMA_CHANNEL1_BASE)
#define DMA_CHANNEL2 ((DMA_ChannelRegStruct *)DMA_CHANNEL2_BASE)
#define DMA_CHANNEL3 ((DMA_ChannelRegStruct *)DMA_CHANNEL3_BASE)
#define DMA_CHANNEL4 ((DMA_ChannelRegStruct *)DMA_CHANNEL4_BASE)
#define DMA_CHANNEL5 ((DMA_ChannelRegStruct *)DMA_CHANNEL5_BASE)
#define DMA1 ((DMA_RegStruct *)DMA1_BASE)
#define DMA1_CHANNEL0 ((DMA_ChannelRegStruct *)DMA1_CHANNEL0_BASE)
#define DMA1_CHANNEL1 ((DMA_ChannelRegStruct *)DMA1_CHANNEL1_BASE)
#define DMA1_CHANNEL2 ((DMA_ChannelRegStruct *)DMA1_CHANNEL2_BASE)
#define DMA1_CHANNEL3 ((DMA_ChannelRegStruct *)DMA1_CHANNEL3_BASE)
#define DMA1_CHANNEL4 ((DMA_ChannelRegStruct *)DMA1_CHANNEL4_BASE)
#define DMA1_CHANNEL5 ((DMA_ChannelRegStruct *)DMA1_CHANNEL5_BASE)
#define FMC ((FMC_RegStruct*)FMC_BASE)

#define IsCRGInstance(instance) ((instance) == CRG)
#define IsCMMInstance(instance) (((instance) == CMM) || ((instance) == CMM1))
#define IsCFDInstance(instance) ((instance) == CFD)
#define IsSYSCTRLInstance(instance) (((instance) == SYSCTRL0) || ((instance) == SYSCTRL1))
#define IsUARTInstance(instance) (((instance) == UART0) || ((instance) == UART1) || \
                                  ((instance) == UART2) || ((instance) == UART3) || ((instance) == UART4) || \
                                  ((instance) == UART5) || ((instance) == UART6) || ((instance) == UART7))
#define IsI2CInstance(instance) ((instance) == I2C0 || (instance) == I2C1 || (instance) == I2C2)
#define IsSPIInstance(instance) (((instance) == SPI0) || ((instance) == SPI1) || ((instance) == SPI2))
#define IsTIMERInstance(instance) (((instance) == TIMER0) || ((instance) == TIMER1) || \
                                   ((instance) == TIMER2) || ((instance) == TIMER3) || \
                                   ((instance) == TIMER4) || ((instance) == TIMER5))
#define IsSYSTICKInstance(instance)  ((instance) == SYSTICK || (instance) == SYSTICK1 || (instance) == SYSTICK2)
#define IsWWDGInstance(instance) ((instance) == WWDG0 || (instance) == WWDG1 || (instance) == WWDG2)
#define IsIWDGInstance(instance) ((instance) == IWDG)
#define IsGPIOInstance(instance) (((instance) == GPIO0) || ((instance) == GPIO1) || \
                                  ((instance) == GPIO2) || ((instance) == GPIO3) || \
                                  ((instance) == GPIO4) || ((instance) == GPIO5) || \
                                  ((instance) == GPIO6) || ((instance) == GPIO7) || \
                                  ((instance) == GPIO8) || ((instance) == GPIO9) || \
                                  ((instance) == GPIO10) || ((instance) == GPIO11) || \
                                  ((instance) == GPIO12) || ((instance) == GPIO13) || \
                                  ((instance) == GPIO14) || ((instance) == GPIO15) || \
                                  ((instance) == GPIO16))
#define IsCANInstance(instance) ((instance) == CAN0 || ((instance) == CAN1) || ((instance) == CAN2))
#define IsGPTInstance(instance) (((instance) == GPT0) || ((instance) == GPT1))
#define IsEFCInstance(instance) ((instance) == EFC)
#define IsPMCInstance(instance) ((instance) == PMC)
#define IsIOCMGInstance(instance) ((instance) == IOCMG)
#define IsCRCInstance(instance) ((instance) == CRC)
#define IsIPCMInstance(instance) ((instance) == IPCM)
#define IsAPTInstance(instance) (((instance) == APT0) || ((instance) == APT1) || \
                                 ((instance) == APT2) || ((instance) == APT3) || \
                                 ((instance) == APT4) || ((instance) == APT5) || \
                                 ((instance) == APT6) || ((instance) == APT7) || \
                                 ((instance) == APT8) || ((instance) == APT9) || \
                                 ((instance) == APT10) || ((instance) == APT11) || \
                                 ((instance) == APT12) || ((instance) == APT13) || \
                                 ((instance) == APT14) || ((instance) == APT15))
#define IsXBARInstance(instance) ((instance) == XBAR)
#define IsADDLLInstance(instance) ((instance) == ADDLL)
#define IsSRMInstance(instance) ((instance) == SRM)
#define IsCAPMInstance(instance) (((instance) == CAPM0) || ((instance) == CAPM1) || ((instance) == CAPM2))
#define IsCAPMCOMMInstance(instance) ((instance) == CAPM_COMM)
#define IsQDMInstance(instance) (((instance) == QDM0) || ((instance) == QDM1) || \
                                 ((instance) == QDM2) || ((instance) == QDM3))
#define IsADCInstance(instance) (((instance) == ADC0) || ((instance) == ADC1) || \
                                 ((instance) == ADC2) || ((instance) == ADC3))
#define IsPGAInstance(instance) (((instance) == PGA0) || ((instance) == PGA1) || \
                                 ((instance) == PGA2) || ((instance) == PGA3))
#define IsDACInstance(instance) (((instance) == DAC0) || ((instance) == DAC1) || ((instance) == DAC2) || \
                                 ((instance) == DAC3) || ((instance) == DAC4) || ((instance) == DAC5) || \
                                 ((instance) == DAC6) || ((instance) == DAC7) || ((instance) == DACBUFF))
#define IsACMPInstance(instance) (((instance) == ACMP0) || ((instance) == ACMP1) || \
                                  ((instance) == ACMP2) || ((instance) == ACMP3) || \
                                  ((instance) == ACMP4) || ((instance) == ACMP5) || \
                                  ((instance) == ACMP6) || ((instance) == ACMP7))
#define IsDMAInstance(instance) ((instance) == DMA || (instance) == DMA1)
#define IsDMACHXInstance(instance) (((instance) == DMA_CHANNEL0) || ((instance) == DMA_CHANNEL1) || \
                                    ((instance) == DMA_CHANNEL2) || ((instance) == DMA_CHANNEL3) || \
                                    ((instance) == DMA_CHANNEL4) || ((instance) == DMA_CHANNEL5) || \
                                    ((instance) == DMA1_CHANNEL0) || ((instance) == DMA1_CHANNEL1) || \
                                    ((instance) == DMA1_CHANNEL2) || ((instance) == DMA1_CHANNEL3) || \
                                    ((instance) == DMA1_CHANNEL4) || ((instance) == DMA1_CHANNEL5))
#define IsFMCInstance(instance) ((instance) == FMC)
#define SRAM_START 0x03FC0000
#define SRAM_END 0x0403FFFF
#define REGISTER_START 0x10000000
#define REGISTER_END 0x1C0E0FFF

#endif /* McuMagicTag_BASEADDR_H */
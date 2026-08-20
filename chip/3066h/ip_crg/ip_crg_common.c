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
  * @file      ip_crg_common.c
  * @author    MCU Driver Team
  * @brief     Contains ip crg common header files.
  */

/* Includes ----------------------------------------------------------------- */
#include "baseaddr.h"
#include "ip_crg_common.h"

/**
 * @brief Get IP frequency by ip register base address
 * @param ipBaseAddr The ip base address
 * @retval The bus frequency where the IP is located
 */
#ifdef FPGA
unsigned int CHIP_GetIpFreqHz(const void *ipBaseAddr)
{
    if (ipBaseAddr == IWDG_BASE) { /* The IWDG working clock is LOSC clock. */
        return LOSC_FREQ;
    } else {
        return HOSC_FREQ; /* The base address does not match, return LOSC freq. */
    }
}
#endif

static const CHIP_CrgIpMatchInfo g_crgIpMatch[] = {
    {UART0_BASE, CRG_IP_LS_CLK_SEL,  0x140, 0},
    {UART1_BASE, CRG_IP_LS_CLK_SEL,  0x144, 0},
    {UART2_BASE, CRG_IP_LS_CLK_SEL,  0x148, 0},
    {UART3_BASE, CRG_IP_LS_CLK_SEL,  0x14C, 0},
    {UART4_BASE, CRG_IP_LS_CLK_SEL,  0x150, 0},
    {UART5_BASE, CRG_IP_LS_CLK_SEL,  0x154, 0},
    {UART6_BASE, CRG_IP_LS_CLK_SEL,  0x158, 0},
    {UART7_BASE, CRG_IP_LS_CLK_SEL,  0x15C, 0},
    {SPI0_BASE, CRG_IP_LS_CLK_SEL, 0x180, 0},
    {SPI1_BASE, CRG_IP_LS_CLK_SEL, 0x184, 0},
    {SPI2_BASE, CRG_IP_LS_CLK_SEL, 0x188, 0},
    {I2C0_BASE, CRG_IP_LS_CLK_SEL, 0x1C0, 0},
    {I2C1_BASE, CRG_IP_LS_CLK_SEL, 0x1C4, 0},
    {I2C2_BASE, CRG_IP_LS_CLK_SEL, 0x1C8, 0},
    {TIMER0_BASE, CRG_IP_HS_CLK_SEL, 0x240, 0},
    {TIMER1_BASE, CRG_IP_HS_CLK_SEL, 0x244, 0},
    {TIMER2_BASE, CRG_IP_HS_CLK_SEL, 0x248, 0},
    {TIMER3_BASE, CRG_IP_HS_CLK_SEL, 0x24C, 0},
    {TIMER4_BASE, CRG_IP_HS_CLK_SEL, 0x250, 0},
    {TIMER5_BASE, CRG_IP_HS_CLK_SEL, 0x254, 0},
    {SYSTICK_BASE, CRG_IP_HS_CLK_SEL, 0x40, 0},
    {SYSTICK1_BASE, CRG_IP_HS_CLK_SEL, 0x40, 0},
    {SYSTICK2_BASE, CRG_IP_HS_CLK_SEL, 0x40, 0},
    {CAPM0_BASE, CRG_IP_HS_CLK_SEL, 0x280, 0},
    {CAPM1_BASE, CRG_IP_HS_CLK_SEL, 0x284, 0},
    {CAPM2_BASE, CRG_IP_HS_CLK_SEL, 0x288, 0},
    {CAN0_BASE, CRG_IP_CAN, 0x2C0, 0},
    {CAN1_BASE, CRG_IP_CAN, 0x2C8, 0},
    {CAN2_BASE, CRG_IP_CAN, 0x2D0, 0},

    {DMA_BASE,  CRG_IP_HS_CLK_SEL, 0x300, 0},
    {DMA1_BASE, CRG_IP_HS_CLK_SEL, 0x304, 0},
    {FMC_BASE,  CRG_IP_FMC, 0x310, 0},

    {CMM_BASE,  CRG_IP_LS_CLK_SEL, 0x0340, 0},
    {CMM1_BASE, CRG_IP_LS_CLK_SEL, 0x0344, 0},
    {CFD_BASE,  CRG_IP_LS_CLK_SEL, 0x0348, 0},
    {CRC_BASE,  CRG_IP_HS_CLK_SEL, 0x380, 0},

    {XBAR_BASE,  CRG_IP_HS_CLK_SEL, 0x3E8, 0},
    {APT0_BASE,  CRG_IP_HS_CLK_SEL, 0x400, 0},
    {APT1_BASE,  CRG_IP_HS_CLK_SEL, 0x404, 0},
    {APT2_BASE,  CRG_IP_HS_CLK_SEL, 0x408, 0},
    {APT3_BASE,  CRG_IP_HS_CLK_SEL, 0x40C, 0},
    {APT4_BASE,  CRG_IP_HS_CLK_SEL, 0x410, 0},
    {APT5_BASE,  CRG_IP_HS_CLK_SEL, 0x414, 0},
    {APT6_BASE,  CRG_IP_HS_CLK_SEL, 0x418, 0},
    {APT7_BASE,  CRG_IP_HS_CLK_SEL, 0x41C, 0},
    {APT8_BASE,  CRG_IP_HS_CLK_SEL, 0x420, 0},
    {APT9_BASE,  CRG_IP_HS_CLK_SEL, 0x424, 0},
    {APT10_BASE, CRG_IP_HS_CLK_SEL, 0x428, 0},
    {APT11_BASE, CRG_IP_HS_CLK_SEL, 0x42C, 0},
    {APT12_BASE, CRG_IP_HS_CLK_SEL, 0x430, 0},
    {APT13_BASE, CRG_IP_HS_CLK_SEL, 0x434, 0},
    {APT14_BASE, CRG_IP_HS_CLK_SEL, 0x438, 0},
    {APT15_BASE, CRG_IP_HS_CLK_SEL, 0x43C, 0},
    {GPT0_BASE,  CRG_IP_HS_CLK_SEL, 0x440, 0},
    {GPT1_BASE,  CRG_IP_HS_CLK_SEL, 0x444, 0},
    {GPIO0_BASE,  CRG_IP_LS_CLK_SEL, 0x480, 0},
    {GPIO1_BASE,  CRG_IP_LS_CLK_SEL, 0x484, 0},
    {GPIO2_BASE,  CRG_IP_LS_CLK_SEL, 0x488, 0},
    {GPIO3_BASE,  CRG_IP_LS_CLK_SEL, 0x48C, 0},
    {GPIO4_BASE,  CRG_IP_LS_CLK_SEL, 0x490, 0},
    {GPIO5_BASE,  CRG_IP_LS_CLK_SEL, 0x494, 0},
    {GPIO6_BASE,  CRG_IP_LS_CLK_SEL, 0x498, 0},
    {GPIO7_BASE,  CRG_IP_LS_CLK_SEL, 0x49C, 0},
    {GPIO8_BASE,  CRG_IP_LS_CLK_SEL, 0x4A0, 0},
    {GPIO9_BASE,  CRG_IP_HS_CLK_SEL, 0x4A4, 0},
    {GPIO10_BASE, CRG_IP_HS_CLK_SEL, 0x4A8, 0},
    {GPIO11_BASE, CRG_IP_HS_CLK_SEL, 0x4AC, 0},
    {GPIO12_BASE, CRG_IP_HS_CLK_SEL, 0x4B0, 0},
    {GPIO13_BASE, CRG_IP_HS_CLK_SEL, 0x4B4, 0},
    {GPIO14_BASE, CRG_IP_HS_CLK_SEL, 0x4B8, 0},
    {GPIO15_BASE, CRG_IP_HS_CLK_SEL, 0x4BC, 0},
    {GPIO16_BASE, CRG_IP_HS_CLK_SEL, 0x4C0, 0},
    {QDM0_BASE, CRG_IP_HS_CLK_SEL, 0x4D0, 0},
    {QDM1_BASE, CRG_IP_HS_CLK_SEL, 0x4D4, 0},
    {QDM2_BASE, CRG_IP_HS_CLK_SEL, 0x4D8, 0},
    {QDM3_BASE, CRG_IP_HS_CLK_SEL, 0x4DC, 0},

    {EFC_BASE,  CRG_IP_EFC, 0x700, 0},
    {IPCM_BASE, CRG_IP_HS_CLK_SEL, 0x800, 0},
    {ADC0_BASE, CRG_IP_ADC, 0xA00, 0},
    {ADC1_BASE, CRG_IP_ADC, 0xA08, 0},
    {ADC2_BASE, CRG_IP_ADC, 0xA10, 0},
    {ADC3_BASE, CRG_IP_ADC, 0xA18, 0},
    {VREF_BASE, CRG_IP_ANA, 0xA60, 0},
    {ACMP0_BASE, CRG_IP_ANA, 0xA70, 0},
    {ACMP1_BASE, CRG_IP_ANA, 0xA74, 0},
    {ACMP2_BASE, CRG_IP_ANA, 0xA78, 0},
    {ACMP3_BASE, CRG_IP_ANA, 0xA7C, 0},
    {ACMP4_BASE, CRG_IP_ANA, 0xA80, 0},
    {ACMP5_BASE, CRG_IP_ANA, 0xA84, 0},
    {ACMP6_BASE, CRG_IP_ANA, 0xA88, 0},
    {ACMP7_BASE, CRG_IP_ANA, 0xA8C, 0},
    {DAC0_BASE, CRG_IP_ANA, 0xA90, 0},
    {DAC1_BASE, CRG_IP_ANA, 0xA94, 0},
    {DAC2_BASE, CRG_IP_ANA, 0xA98, 0},
    {DAC3_BASE, CRG_IP_ANA, 0xA9C, 0},
    {DAC4_BASE, CRG_IP_ANA, 0xAA0, 0},
    {DAC5_BASE, CRG_IP_ANA, 0xAA4, 0},
    {DAC6_BASE, CRG_IP_ANA, 0xAA8, 0},
    {DAC7_BASE, CRG_IP_ANA, 0xAAC, 0},
    {DACBUFF_BASE, CRG_IP_ANA, 0xAB0, 0},

    {PGA0_BASE, CRG_IP_ANA, 0xAB4, 0},
    {PGA1_BASE, CRG_IP_ANA, 0xAB8, 0},
    {PGA2_BASE, CRG_IP_ANA, 0xABC, 0},
    {PGA3_BASE, CRG_IP_ANA, 0xAC0, 0},

    {WWDG0_BASE, CRG_IP_WWDG, 0x0, 0},
    {IWDG_BASE,  CRG_IP_IWDG, 0x0, 0},
    {WWDG1_BASE, CRG_IP_WWDG, 0x0, 0},
    {WWDG2_BASE, CRG_IP_WWDG, 0x0, 0},
};

/**
  * @brief Get IP Match Info, @see g_crgIpMatch
  * @param baseAddr The ip base address
  * @retval The Address(offset) in g_crgIpMatch if match success
  * @retval 0 if match fail
  */
CHIP_CrgIpMatchInfo *GetCrgIpMatchInfo(const void *baseAddr)
{
    unsigned int i;
    for (i = 0; i < sizeof(g_crgIpMatch) / sizeof(g_crgIpMatch[0]); ++i) {
        if (baseAddr == g_crgIpMatch[i].ipBaseAddr) {
            return (CHIP_CrgIpMatchInfo *)&g_crgIpMatch[i];
        }
    }
    return (CHIP_CrgIpMatchInfo *)0; /* The base address does not match, return 0. */
}

/**
  * @copyright Copyright (c) 2022, HiSilicon (Shanghai) Technologies Co., Ltd. All rights reserved.
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
  * @file      anatrim.c
  * @author    MCU Driver Team
  * @brief     Chip Init modlue.
  * @details   Calibration of analog module parameters.
  */
#include "anatrim.h"

float g_tsensorGain = 0.00418f;

#ifdef FPGA
#else

/**
 * @brief Obtains the chip ID.
 * @param None
 * @retval None
 */
static bool CHIP_GetInfo(void)
{
    FOTP_INFO_RGN0_NUMBER_1 pvdData1;
    FOTP_INFO_RGN0_NUMBER_4 emptyData;
    FOTP_INFO_RGN0_NUMBER_2 idData;
    FOTP_InfoGet(FOTP_INFO_RNG0, 4U, (void *)&emptyData.comData);   /* 4 is the number of fotp_empty_flag in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 2U, (void *)&idData.comData);      /* 2 is the number of idData in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 1U, (void *)&pvdData1.comData);      /* 1 is the number of pvdData1 in otp */
    PMC->PMU_CTRL0.BIT.pvd_trim = pvdData1.REG.data3.PVD_TRIM_1V1;
    PMC->PMU_CTRL0.BIT.pmu_top_rsv_aon = pvdData1.REG.data3.CORE_POR_TRIM_1V1_HYS;
    PMC->PMU_VSENOR.BIT.pmu_core_vsensor_thld = pvdData1.REG.data3.CORE_POR_TRIM_1V1; /* PMC register config */
    if (emptyData.REG.fotp_empty_flag != 0x5AA59669 || idData.REG.chip_id == 0xFFFFFFFF) {
        return false;
    }
    return true;
}

/**
 * @brief VREF trim config.
 * @param None
 * @retval None
 */
static void VREF_Trim(void)
{
    FOTP_INFO_RGN0_NUMBER_20 trimData20;
    FOTP_InfoGet(FOTP_INFO_RNG0, 20U, (void *)&trimData20.comData); /* 20 is the number of trim data in otp */
    /* VREF */
    VREF->VREF_TRIM0.BIT.da_iref_trim = trimData20.REG.data0.da_iref_trim;
    VREF->VREF_TRIM0.BIT.da_ref_vref_trim = trimData20.REG.data0.da_ref_vref_trim;
    VREF->VREF_TRIM0.BIT.da_ref_vbg_trim = trimData20.REG.data0.da_ref_vbg_trim;
    unsigned int value = trimData20.REG.data1.da_ref_temp_trim3;
    value |= (trimData20.REG.data1.da_ref_temp_trim2 << 8U);    /* Shift left by 8 bits */
    value |= (trimData20.REG.data1.da_ref_temp_trim1 << 16U);   /* Shift left by 16 bits */
    value |= (trimData20.REG.data0.da_ref_temp_trim0 << 24U);   /* Shift left by 24 bits */
    VREF->VREF_TRIM1.reg = value;
}

/**
 * @brief TSENSOR trim config.
 * @param None
 * @retval None
 */
static void TSENSOR_Trim(void)
{
    HAL_CRG_IpEnableSet((void *)ADC1, IP_CLK_ENABLE); /* Enable the clock for calibration */
    HAL_CRG_IpEnableSet((void *)ADC0, IP_CLK_ENABLE); /* Enable the clock for calibration */
    FOTP_INFO_RGN0_NUMBER_20 trimData20;
    FOTP_INFO_RGN0_NUMBER_38 trimData38;
    FOTP_InfoGet(FOTP_INFO_RNG0, 20U, (void *)&trimData20.comData); /* 20 is the number of trim data in otp */
    /* TSENSOR */
    TSENSOR->TSENSOR_TRIM.reg = trimData20.REG.data1.da_ref_vptat0_trim;
    ADC0->ADC_TSENSOR_TRIM.BIT.cfg_tsensor_ofst_trim = trimData38.REG.data2.ts0_offset;
    ADC1->ADC_TSENSOR_TRIM.BIT.cfg_tsensor_ofst_trim = trimData38.REG.data2.ts1_offset;
    HAL_CRG_IpEnableSet((void *)ADC1, IP_CLK_DISABLE); /* The clock is disabled after calibration */
}

/**
 * @brief PGA trim config.
 * @param None
 * @retval None
 */
static void PGA_Trim(void)
{
    HAL_CRG_IpEnableSet((void *)PGA0, IP_CLK_ENABLE);
    HAL_CRG_IpEnableSet((void *)PGA1, IP_CLK_ENABLE);
    HAL_CRG_IpEnableSet((void *)PGA2, IP_CLK_ENABLE);
    FOTP_INFO_RGN0_NUMBER_21 trimData21;
    FOTP_InfoGet(FOTP_INFO_RNG0, 21U, (void *)&trimData21.comData); /* 21 is the number of trim data in otp */
    /* PGA */
    PGA0->PGA_TRIM.BIT.da_pga_vos_trim = trimData21.REG.data0.da_pga0_vos_trim;
    PGA1->PGA_TRIM.BIT.da_pga_vos_trim = trimData21.REG.data0.da_pga1_vos_trim;
    PGA2->PGA_TRIM.BIT.da_pga_vos_trim = trimData21.REG.data1.da_pga2_vos_trim;
    HAL_CRG_IpEnableSet((void *)PGA0, IP_CLK_DISABLE);
    HAL_CRG_IpEnableSet((void *)PGA1, IP_CLK_DISABLE);
    HAL_CRG_IpEnableSet((void *)PGA2, IP_CLK_DISABLE);
}

/**
 * @brief ADC0 trim config.
 * @param None
 * @retval None
 */
static void ADC0_Trim(void)
{
    HAL_CRG_IpEnableSet((void *)ADC0, IP_CLK_ENABLE); /* Enable the clock for calibration */

    FOTP_INFO_RGN0_NUMBER_21 trimData21;
    FOTP_INFO_RGN0_NUMBER_22 trimData22;
    FOTP_INFO_RGN0_NUMBER_23 trimData23;
    FOTP_INFO_RGN0_NUMBER_24 trimData24;
    FOTP_INFO_RGN0_NUMBER_25 trimData25;
    FOTP_INFO_RGN0_NUMBER_26 trimData26;
    FOTP_InfoGet(FOTP_INFO_RNG0, 21U, (void *)&trimData21.comData); /* 21 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 22U, (void *)&trimData22.comData); /* 22 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 23U, (void *)&trimData23.comData); /* 23 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 24U, (void *)&trimData24.comData); /* 24 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 25U, (void *)&trimData25.comData); /* 25 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 26U, (void *)&trimData26.comData); /* 26 is the number of trim data in otp */

    ADC0->ADC_OEGE_TRIM.BIT.cfg_gain_cali_trim = trimData21.REG.data2.saradc0_gain;
    ADC0->ADC_OEGE_TRIM.BIT.cfg_ofst_cali_trim = trimData21.REG.data2.saradc0_offset;
    /* Trim adc0 gain & offset value */
    ADC0->ADC_AIN0_OEGE_TRIM0.BIT.cfg_ain0_gain_trim2   = trimData22.REG.data2.saradc0_ain0_gain2;
    ADC0->ADC_AIN0_OEGE_TRIM0.BIT.cfg_ain0_ofst_trim2   = trimData22.REG.data2.saradc0_ain0_offset2;
    ADC0->ADC_AIN0_OEGE_TRIM1.BIT.cfg_ain0_gain_trim3   = trimData22.REG.data3.saradc0_ain0_gain3;
    ADC0->ADC_AIN0_OEGE_TRIM1.BIT.cfg_ain0_ofst_trim3   = trimData22.REG.data3.saradc0_ain0_offset3;
    /* Trim adc0 gain & offset value */
    ADC0->ADC_AIN0_OEGE_TRIM2.BIT.cfg_ain0_gain_trim5   = trimData23.REG.data0.saradc0_ain0_gain5;
    ADC0->ADC_AIN0_OEGE_TRIM2.BIT.cfg_ain0_ofst_trim5   = trimData23.REG.data0.saradc0_ain0_offset5;
    ADC0->ADC_AIN0_OEGE_TRIM3.BIT.cfg_ain0_gain_trim6   = trimData23.REG.data1.saradc0_ain0_gain6;
    ADC0->ADC_AIN0_OEGE_TRIM3.BIT.cfg_ain0_ofst_trim6   = trimData23.REG.data1.saradc0_ain0_offset6;
    /* Trim adc0 gain & offset value */
    ADC0->ADC_AIN0_OEGE_TRIM4.BIT.cfg_ain0_gain_trim7p5 = trimData23.REG.data2.saradc0_ain0_gain7p5;
    ADC0->ADC_AIN0_OEGE_TRIM4.BIT.cfg_ain0_ofst_trim7p5 = trimData23.REG.data2.saradc0_ain0_offset7p5;
    ADC0->ADC_AIN0_OEGE_TRIM5.BIT.cfg_ain0_gain_trim10  = trimData23.REG.data3.saradc0_ain0_gain10;
    ADC0->ADC_AIN0_OEGE_TRIM5.BIT.cfg_ain0_ofst_trim10  = trimData23.REG.data3.saradc0_ain0_offset10;
    /* Trim adc0 gain & offset value */
    ADC0->ADC_AIN0_OEGE_TRIM6.BIT.cfg_ain0_gain_trim12  = trimData24.REG.data0.saradc0_ain0_gain12 ;
    ADC0->ADC_AIN0_OEGE_TRIM6.BIT.cfg_ain0_ofst_trim12  = trimData24.REG.data0.saradc0_ain0_offset12;
    ADC0->ADC_AIN0_OEGE_TRIM7.BIT.cfg_ain0_gain_trim15  = trimData24.REG.data1.saradc0_ain0_gain15;
    ADC0->ADC_AIN0_OEGE_TRIM7.BIT.cfg_ain0_ofst_trim15  = trimData24.REG.data1.saradc0_ain0_offset15;
    /* Trim adc0 gain & offset value */
    ADC0->ADC_AIN1_OEGE_TRIM0.BIT.cfg_ain1_gain_trim2   = trimData24.REG.data2.saradc0_ain1_gain2;
    ADC0->ADC_AIN1_OEGE_TRIM0.BIT.cfg_ain1_ofst_trim2   = trimData24.REG.data2.saradc0_ain1_offset2;
    ADC0->ADC_AIN1_OEGE_TRIM1.BIT.cfg_ain1_gain_trim3   = trimData24.REG.data3.saradc0_ain1_gain3;
    ADC0->ADC_AIN1_OEGE_TRIM1.BIT.cfg_ain1_ofst_trim3   = trimData24.REG.data3.saradc0_ain1_offset3;
    /* Trim adc0 gain & offset value */
    ADC0->ADC_AIN1_OEGE_TRIM2.BIT.cfg_ain1_gain_trim5   = trimData25.REG.data0.saradc0_ain1_gain5;
    ADC0->ADC_AIN1_OEGE_TRIM2.BIT.cfg_ain1_ofst_trim5   = trimData25.REG.data0.saradc0_ain1_offset5;
    ADC0->ADC_AIN1_OEGE_TRIM3.BIT.cfg_ain1_gain_trim6   = trimData25.REG.data1.saradc0_ain1_gain6;
    ADC0->ADC_AIN1_OEGE_TRIM3.BIT.cfg_ain1_ofst_trim6   = trimData25.REG.data1.saradc0_ain1_offset6;
    /* Trim adc0 gain & offset value */
    ADC0->ADC_AIN1_OEGE_TRIM4.BIT.cfg_ain1_gain_trim7p5 = trimData25.REG.data2.saradc0_ain1_gain7p5;;
    ADC0->ADC_AIN1_OEGE_TRIM4.BIT.cfg_ain1_ofst_trim7p5 = trimData25.REG.data2.saradc0_ain1_offset7p5;;
    ADC0->ADC_AIN1_OEGE_TRIM5.BIT.cfg_ain1_gain_trim10 = trimData25.REG.data3.saradc0_ain1_gain10;
    ADC0->ADC_AIN1_OEGE_TRIM5.BIT.cfg_ain1_ofst_trim10 = trimData25.REG.data3.saradc0_ain1_offset10;
    /* Trim adc0 gain & offset value */
    ADC0->ADC_AIN1_OEGE_TRIM6.BIT.cfg_ain1_gain_trim12 = trimData26.REG.data0.saradc0_ain1_gain12;
    ADC0->ADC_AIN1_OEGE_TRIM6.BIT.cfg_ain1_ofst_trim12 = trimData26.REG.data0.saradc0_ain1_offset12;
    ADC0->ADC_AIN1_OEGE_TRIM7.BIT.cfg_ain1_gain_trim15 = trimData26.REG.data1.saradc0_ain1_gain15;
    ADC0->ADC_AIN1_OEGE_TRIM7.BIT.cfg_ain1_ofst_trim15 = trimData26.REG.data1.saradc0_ain1_offset15;

    HAL_CRG_IpEnableSet((void *)ADC0, IP_CLK_DISABLE); /* The clock is disabled after calibration */
}

/**
 * @brief ADC1 trim config.
 * @param None
 * @retval None
 */
static void ADC1_Trim(void)
{
    HAL_CRG_IpEnableSet((void *)ADC1, IP_CLK_ENABLE); /* Enable the clock for calibration */

    FOTP_INFO_RGN0_NUMBER_21 trimData21;
    FOTP_INFO_RGN0_NUMBER_26 trimData26;
    FOTP_INFO_RGN0_NUMBER_27 trimData27;
    FOTP_INFO_RGN0_NUMBER_28 trimData28;
    FOTP_INFO_RGN0_NUMBER_29 trimData29;
    FOTP_INFO_RGN0_NUMBER_30 trimData30;

    FOTP_InfoGet(FOTP_INFO_RNG0, 21U, (void *)&trimData21.comData); /* 21 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 26U, (void *)&trimData26.comData); /* 26 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 27U, (void *)&trimData27.comData); /* 27 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 28U, (void *)&trimData28.comData); /* 28 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 29U, (void *)&trimData29.comData); /* 29 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 30U, (void *)&trimData30.comData); /* 30 is the number of trim data in otp */

    ADC1->ADC_OEGE_TRIM.BIT.cfg_gain_cali_trim = trimData21.REG.data3.saradc1_gain;
    ADC1->ADC_OEGE_TRIM.BIT.cfg_ofst_cali_trim = trimData21.REG.data3.saradc1_offset;
    /* Trim adc1 gain & offset value */
    ADC1->ADC_AIN0_OEGE_TRIM0.BIT.cfg_ain0_gain_trim2   = trimData26.REG.data2.saradc1_ain0_gain2;
    ADC1->ADC_AIN0_OEGE_TRIM0.BIT.cfg_ain0_ofst_trim2   = trimData26.REG.data2.saradc1_ain0_offset2;
    ADC1->ADC_AIN0_OEGE_TRIM1.BIT.cfg_ain0_gain_trim3   = trimData26.REG.data3.saradc1_ain0_gain3;
    ADC1->ADC_AIN0_OEGE_TRIM1.BIT.cfg_ain0_ofst_trim3   = trimData26.REG.data3.saradc1_ain0_offset3;
    /* Trim adc1 gain & offset value */
    ADC1->ADC_AIN0_OEGE_TRIM2.BIT.cfg_ain0_gain_trim5   = trimData27.REG.data0.saradc1_ain0_gain5;
    ADC1->ADC_AIN0_OEGE_TRIM2.BIT.cfg_ain0_ofst_trim5   = trimData27.REG.data0.saradc1_ain0_offset5;
    ADC1->ADC_AIN0_OEGE_TRIM3.BIT.cfg_ain0_gain_trim6   = trimData27.REG.data1.saradc1_ain0_gain6;
    ADC1->ADC_AIN0_OEGE_TRIM3.BIT.cfg_ain0_ofst_trim6   = trimData27.REG.data1.saradc1_ain0_offset6;
    /* Trim adc1 gain & offset value */
    ADC1->ADC_AIN0_OEGE_TRIM4.BIT.cfg_ain0_gain_trim7p5 = trimData27.REG.data2.saradc1_ain0_gain7p5;
    ADC1->ADC_AIN0_OEGE_TRIM4.BIT.cfg_ain0_ofst_trim7p5 = trimData27.REG.data2.saradc1_ain0_offset7p5;
    ADC1->ADC_AIN0_OEGE_TRIM5.BIT.cfg_ain0_gain_trim10  = trimData27.REG.data3.saradc1_ain0_gain10;
    ADC1->ADC_AIN0_OEGE_TRIM5.BIT.cfg_ain0_ofst_trim10  = trimData27.REG.data3.saradc1_ain0_offset10;
    /* Trim adc1 gain & offset value */
    ADC1->ADC_AIN0_OEGE_TRIM6.BIT.cfg_ain0_gain_trim12  = trimData28.REG.data0.saradc1_ain0_gain12;
    ADC1->ADC_AIN0_OEGE_TRIM6.BIT.cfg_ain0_ofst_trim12  = trimData28.REG.data0.saradc1_ain0_offset12;
    ADC1->ADC_AIN0_OEGE_TRIM7.BIT.cfg_ain0_gain_trim15  = trimData28.REG.data1.saradc1_ain0_gain15;
    ADC1->ADC_AIN0_OEGE_TRIM7.BIT.cfg_ain0_ofst_trim15  = trimData28.REG.data1.saradc1_ain0_offset15;
    /* Trim adc1 gain & offset value */
    ADC1->ADC_AIN1_OEGE_TRIM0.BIT.cfg_ain1_gain_trim2   = trimData28.REG.data2.saradc1_ain1_gain2;
    ADC1->ADC_AIN1_OEGE_TRIM0.BIT.cfg_ain1_ofst_trim2   = trimData28.REG.data2.saradc1_ain1_offset2;
    ADC1->ADC_AIN1_OEGE_TRIM1.BIT.cfg_ain1_gain_trim3   = trimData28.REG.data3.saradc1_ain1_gain3;
    ADC1->ADC_AIN1_OEGE_TRIM1.BIT.cfg_ain1_ofst_trim3   = trimData28.REG.data3.saradc1_ain1_offset3;
    /* Trim adc1 gain & offset value */
    ADC1->ADC_AIN1_OEGE_TRIM2.BIT.cfg_ain1_gain_trim5   = trimData29.REG.data0.saradc1_ain1_gain5;
    ADC1->ADC_AIN1_OEGE_TRIM2.BIT.cfg_ain1_ofst_trim5   = trimData29.REG.data0.saradc1_ain1_offset5;
    ADC1->ADC_AIN1_OEGE_TRIM3.BIT.cfg_ain1_gain_trim6   = trimData29.REG.data1.saradc1_ain1_gain6;
    ADC1->ADC_AIN1_OEGE_TRIM3.BIT.cfg_ain1_ofst_trim6   = trimData29.REG.data1.saradc1_ain1_offset6;
    /* Trim adc1 gain & offset value */
    ADC1->ADC_AIN1_OEGE_TRIM4.BIT.cfg_ain1_gain_trim7p5 = trimData29.REG.data2.saradc1_ain1_gain7p5;
    ADC1->ADC_AIN1_OEGE_TRIM4.BIT.cfg_ain1_ofst_trim7p5 = trimData29.REG.data2.saradc1_ain1_offset7p5;
    ADC1->ADC_AIN1_OEGE_TRIM5.BIT.cfg_ain1_gain_trim10  = trimData29.REG.data3.saradc1_ain1_gain10;
    ADC1->ADC_AIN1_OEGE_TRIM5.BIT.cfg_ain1_ofst_trim10  = trimData29.REG.data3.saradc1_ain1_offset10;
    /* Trim adc1 gain & offset value */
    ADC1->ADC_AIN1_OEGE_TRIM6.BIT.cfg_ain1_gain_trim12 = trimData30.REG.data0.saradc1_ain1_gain12;
    ADC1->ADC_AIN1_OEGE_TRIM6.BIT.cfg_ain1_ofst_trim12 = trimData30.REG.data0.saradc1_ain1_offset12;
    ADC1->ADC_AIN1_OEGE_TRIM7.BIT.cfg_ain1_gain_trim15 = trimData30.REG.data1.saradc1_ain1_gain15;
    ADC1->ADC_AIN1_OEGE_TRIM7.BIT.cfg_ain1_ofst_trim15 = trimData30.REG.data1.saradc1_ain1_offset15;

    HAL_CRG_IpEnableSet((void *)ADC1, IP_CLK_DISABLE); /* The clock is disabled after calibration */
}

/**
 * @brief ADC2 trim config.
 * @param None
 * @retval None
 */
static void ADC2_Trim(void)
{
    HAL_CRG_IpEnableSet((void *)ADC2, IP_CLK_ENABLE); /* Enable the clock for calibration */

    FOTP_INFO_RGN0_NUMBER_22 trimData22;
    FOTP_INFO_RGN0_NUMBER_30 trimData30;
    FOTP_INFO_RGN0_NUMBER_31 trimData31;
    FOTP_INFO_RGN0_NUMBER_32 trimData32;
    FOTP_INFO_RGN0_NUMBER_33 trimData33;
    FOTP_INFO_RGN0_NUMBER_34 trimData34;
    FOTP_InfoGet(FOTP_INFO_RNG0, 22U, (void *)&trimData22.comData); /* 22 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 30U, (void *)&trimData30.comData); /* 30 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 31U, (void *)&trimData31.comData); /* 31 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 32U, (void *)&trimData32.comData); /* 32 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 33U, (void *)&trimData33.comData); /* 33 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 34U, (void *)&trimData33.comData); /* 34 is the number of trim data in otp */

    ADC2->ADC_OEGE_TRIM.BIT.cfg_gain_cali_trim = trimData22.REG.data0.saradc2_gain;
    ADC2->ADC_OEGE_TRIM.BIT.cfg_ofst_cali_trim = trimData22.REG.data0.saradc2_offset;
    /* Trim adc2 gain & offset value */
    ADC2->ADC_AIN0_OEGE_TRIM0.BIT.cfg_ain0_gain_trim2   = trimData30.REG.data2.saradc2_ain0_gain2;
    ADC2->ADC_AIN0_OEGE_TRIM0.BIT.cfg_ain0_ofst_trim2   = trimData30.REG.data2.saradc2_ain0_offset2;
    ADC2->ADC_AIN0_OEGE_TRIM1.BIT.cfg_ain0_gain_trim3   = trimData30.REG.data3.saradc2_ain0_gain3;
    ADC2->ADC_AIN0_OEGE_TRIM1.BIT.cfg_ain0_ofst_trim3   = trimData30.REG.data3.saradc2_ain0_offset3;
    /* Trim adc2 gain & offset value */
    ADC2->ADC_AIN0_OEGE_TRIM2.BIT.cfg_ain0_gain_trim5   = trimData31.REG.data0.saradc2_ain0_gain5;
    ADC2->ADC_AIN0_OEGE_TRIM2.BIT.cfg_ain0_ofst_trim5   = trimData31.REG.data0.saradc2_ain0_offset5;
    ADC2->ADC_AIN0_OEGE_TRIM3.BIT.cfg_ain0_gain_trim6   = trimData31.REG.data1.saradc2_ain0_gain6;
    ADC2->ADC_AIN0_OEGE_TRIM3.BIT.cfg_ain0_ofst_trim6   = trimData31.REG.data1.saradc2_ain0_offset6;
    /* Trim adc2 gain & offset value */
    ADC2->ADC_AIN0_OEGE_TRIM4.BIT.cfg_ain0_gain_trim7p5 = trimData31.REG.data2.saradc2_ain0_gain7p5;
    ADC2->ADC_AIN0_OEGE_TRIM4.BIT.cfg_ain0_ofst_trim7p5 = trimData31.REG.data2.saradc2_ain0_offset7p5;
    ADC2->ADC_AIN0_OEGE_TRIM5.BIT.cfg_ain0_gain_trim10  = trimData31.REG.data3.saradc2_ain0_gain10;
    ADC2->ADC_AIN0_OEGE_TRIM5.BIT.cfg_ain0_ofst_trim10  = trimData31.REG.data3.saradc2_ain0_offset10;
    /* Trim adc2 gain & offset value */
    ADC2->ADC_AIN0_OEGE_TRIM6.BIT.cfg_ain0_gain_trim12  = trimData32.REG.data0.saradc2_ain0_gain12;
    ADC2->ADC_AIN0_OEGE_TRIM6.BIT.cfg_ain0_ofst_trim12  = trimData32.REG.data0.saradc2_ain0_offset12;
    ADC2->ADC_AIN0_OEGE_TRIM7.BIT.cfg_ain0_gain_trim15  = trimData32.REG.data1.saradc2_ain0_gain15;
    ADC2->ADC_AIN0_OEGE_TRIM7.BIT.cfg_ain0_ofst_trim15  = trimData32.REG.data1.saradc2_ain0_offset15;
    /* Trim adc2 gain & offset value */
    ADC2->ADC_AIN1_OEGE_TRIM0.BIT.cfg_ain1_gain_trim2 = trimData32.REG.data2.saradc2_ain1_gain2;
    ADC2->ADC_AIN1_OEGE_TRIM0.BIT.cfg_ain1_ofst_trim2 = trimData32.REG.data2.saradc2_ain1_offset2;
    ADC2->ADC_AIN1_OEGE_TRIM1.BIT.cfg_ain1_gain_trim3 = trimData32.REG.data3.saradc2_ain1_gain3;
    ADC2->ADC_AIN1_OEGE_TRIM1.BIT.cfg_ain1_ofst_trim3 = trimData32.REG.data3.saradc2_ain1_offset3;
    /* Trim adc2 gain & offset value */
    ADC2->ADC_AIN1_OEGE_TRIM2.BIT.cfg_ain1_gain_trim5   = trimData33.REG.data0.saradc2_ain1_gain5;
    ADC2->ADC_AIN1_OEGE_TRIM2.BIT.cfg_ain1_ofst_trim5   = trimData33.REG.data0.saradc2_ain1_offset5;
    ADC2->ADC_AIN1_OEGE_TRIM3.BIT.cfg_ain1_gain_trim6   = trimData33.REG.data1.saradc2_ain1_gain6;
    ADC2->ADC_AIN1_OEGE_TRIM3.BIT.cfg_ain1_ofst_trim6   = trimData33.REG.data1.saradc2_ain1_offset6;
    /* Trim adc2 gain & offset value */
    ADC2->ADC_AIN1_OEGE_TRIM4.BIT.cfg_ain1_gain_trim7p5 = trimData33.REG.data2.saradc2_ain1_gain7p5;
    ADC2->ADC_AIN1_OEGE_TRIM4.BIT.cfg_ain1_ofst_trim7p5 = trimData33.REG.data2.saradc2_ain1_offset7p5;
    ADC2->ADC_AIN1_OEGE_TRIM5.BIT.cfg_ain1_gain_trim10  = trimData33.REG.data3.saradc2_ain1_gain10;
    ADC2->ADC_AIN1_OEGE_TRIM5.BIT.cfg_ain1_ofst_trim10  = trimData33.REG.data3.saradc2_ain1_offset10;
    /* Trim adc2 gain & offset value */
    ADC2->ADC_AIN1_OEGE_TRIM6.BIT.cfg_ain1_gain_trim12 = trimData34.REG.data0.saradc2_ain1_gain12;
    ADC2->ADC_AIN1_OEGE_TRIM6.BIT.cfg_ain1_ofst_trim12 = trimData34.REG.data0.saradc2_ain1_offset12;
    ADC2->ADC_AIN1_OEGE_TRIM7.BIT.cfg_ain1_gain_trim15 = trimData34.REG.data1.saradc2_ain1_gain15;
    ADC2->ADC_AIN1_OEGE_TRIM7.BIT.cfg_ain1_ofst_trim15 = trimData34.REG.data1.saradc2_ain1_offset15;

    HAL_CRG_IpEnableSet((void *)ADC2, IP_CLK_DISABLE); /* The clock is disabled after calibration */
}

/**
 * @brief ADC2 trim config.
 * @param None
 * @retval None
 */
static void ADC3_Trim(void)
{
    HAL_CRG_IpEnableSet((void *)ADC3, IP_CLK_ENABLE); /* Enable the clock for calibration */

    FOTP_INFO_RGN0_NUMBER_22 trimData22;
    FOTP_INFO_RGN0_NUMBER_34 trimData34;
    FOTP_INFO_RGN0_NUMBER_35 trimData35;
    FOTP_INFO_RGN0_NUMBER_36 trimData36;
    FOTP_INFO_RGN0_NUMBER_37 trimData37;
    FOTP_INFO_RGN0_NUMBER_38 trimData38;
    FOTP_InfoGet(FOTP_INFO_RNG0, 22U, (void *)&trimData22.comData); /* 22 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 34U, (void *)&trimData34.comData); /* 34 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 35U, (void *)&trimData35.comData); /* 35 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 36U, (void *)&trimData36.comData); /* 36 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 37U, (void *)&trimData37.comData); /* 37 is the number of trim data in otp */
    FOTP_InfoGet(FOTP_INFO_RNG0, 38U, (void *)&trimData38.comData); /* 38 is the number of trim data in otp */

    ADC3->ADC_OEGE_TRIM.BIT.cfg_gain_cali_trim = trimData22.REG.data1.saradc3_gain;
    ADC3->ADC_OEGE_TRIM.BIT.cfg_ofst_cali_trim = trimData22.REG.data1.saradc3_offset;
    /* Trim adc3 gain & offset value */
    ADC3->ADC_AIN0_OEGE_TRIM0.BIT.cfg_ain0_gain_trim2   = trimData34.REG.data2.saradc3_ain0_gain2;
    ADC3->ADC_AIN0_OEGE_TRIM0.BIT.cfg_ain0_ofst_trim2   = trimData34.REG.data2.saradc3_ain0_offset2;
    ADC3->ADC_AIN0_OEGE_TRIM1.BIT.cfg_ain0_gain_trim3   = trimData34.REG.data3.saradc3_ain0_gain3;
    ADC3->ADC_AIN0_OEGE_TRIM1.BIT.cfg_ain0_ofst_trim3   = trimData34.REG.data3.saradc3_ain0_offset3;
    /* Trim adc3 gain & offset value */
    ADC3->ADC_AIN0_OEGE_TRIM2.BIT.cfg_ain0_gain_trim5   = trimData35.REG.data0.saradc3_ain0_gain5;
    ADC3->ADC_AIN0_OEGE_TRIM2.BIT.cfg_ain0_ofst_trim5   = trimData35.REG.data0.saradc3_ain0_offset5;
    ADC3->ADC_AIN0_OEGE_TRIM3.BIT.cfg_ain0_gain_trim6   = trimData35.REG.data1.saradc3_ain0_gain6;
    ADC3->ADC_AIN0_OEGE_TRIM3.BIT.cfg_ain0_ofst_trim6   = trimData35.REG.data1.saradc3_ain0_offset6;
    /* Trim adc3 gain & offset value */
    ADC3->ADC_AIN0_OEGE_TRIM4.BIT.cfg_ain0_gain_trim7p5 = trimData35.REG.data2.saradc3_ain0_gain7p5;
    ADC3->ADC_AIN0_OEGE_TRIM4.BIT.cfg_ain0_ofst_trim7p5 = trimData35.REG.data2.saradc3_ain0_offset7p5;
    ADC3->ADC_AIN0_OEGE_TRIM5.BIT.cfg_ain0_gain_trim10  = trimData35.REG.data3.saradc3_ain0_gain10;
    ADC3->ADC_AIN0_OEGE_TRIM5.BIT.cfg_ain0_ofst_trim10  = trimData35.REG.data3.saradc3_ain0_offset10;
    /* Trim adc3 gain & offset value */
    ADC3->ADC_AIN0_OEGE_TRIM6.BIT.cfg_ain0_gain_trim12  = trimData36.REG.data0.saradc3_ain0_gain12;
    ADC3->ADC_AIN0_OEGE_TRIM6.BIT.cfg_ain0_ofst_trim12  = trimData36.REG.data0.saradc3_ain0_offset12;
    ADC3->ADC_AIN0_OEGE_TRIM7.BIT.cfg_ain0_gain_trim15  = trimData36.REG.data1.saradc3_ain0_gain15;
    ADC3->ADC_AIN0_OEGE_TRIM7.BIT.cfg_ain0_ofst_trim15  = trimData36.REG.data1.saradc3_ain0_offset15;
    /* Trim adc3 gain & offset value */
    ADC3->ADC_AIN1_OEGE_TRIM0.BIT.cfg_ain1_gain_trim2   = trimData36.REG.data2.saradc3_ain1_gain2;
    ADC3->ADC_AIN1_OEGE_TRIM0.BIT.cfg_ain1_ofst_trim2   = trimData36.REG.data2.saradc3_ain1_offset2;
    ADC3->ADC_AIN1_OEGE_TRIM1.BIT.cfg_ain1_gain_trim3   = trimData36.REG.data3.saradc3_ain1_gain3;
    ADC3->ADC_AIN1_OEGE_TRIM1.BIT.cfg_ain1_ofst_trim3   = trimData36.REG.data3.saradc3_ain1_offset3;
    /* Trim adc3 gain & offset value */
    ADC3->ADC_AIN1_OEGE_TRIM2.BIT.cfg_ain1_gain_trim5   = trimData37.REG.data0.saradc3_ain1_gain5;
    ADC3->ADC_AIN1_OEGE_TRIM2.BIT.cfg_ain1_ofst_trim5   = trimData37.REG.data0.saradc3_ain1_offset5;
    ADC3->ADC_AIN1_OEGE_TRIM3.BIT.cfg_ain1_gain_trim6   = trimData37.REG.data1.saradc3_ain1_gain6;
    ADC3->ADC_AIN1_OEGE_TRIM3.BIT.cfg_ain1_ofst_trim6   = trimData37.REG.data1.saradc3_ain1_offset6;
    /* Trim adc3 gain & offset value */
    ADC3->ADC_AIN1_OEGE_TRIM4.BIT.cfg_ain1_gain_trim7p5 = trimData37.REG.data2.saradc3_ain1_gain7p5;
    ADC3->ADC_AIN1_OEGE_TRIM4.BIT.cfg_ain1_ofst_trim7p5 = trimData37.REG.data2.saradc3_ain1_offset7p5;
    ADC3->ADC_AIN1_OEGE_TRIM5.BIT.cfg_ain1_gain_trim10  = trimData37.REG.data3.saradc3_ain1_gain10;
    ADC3->ADC_AIN1_OEGE_TRIM5.BIT.cfg_ain1_ofst_trim10  = trimData37.REG.data3.saradc3_ain1_offset10;
    /* Trim adc3 gain & offset value */
    ADC3->ADC_AIN1_OEGE_TRIM6.BIT.cfg_ain1_gain_trim12 = trimData38.REG.data0.saradc3_ain1_gain12;
    ADC3->ADC_AIN1_OEGE_TRIM6.BIT.cfg_ain1_ofst_trim12 = trimData38.REG.data0.saradc3_ain1_offset12;
    ADC3->ADC_AIN1_OEGE_TRIM7.BIT.cfg_ain1_gain_trim15 = trimData38.REG.data1.saradc3_ain1_gain15;
    ADC3->ADC_AIN1_OEGE_TRIM7.BIT.cfg_ain1_ofst_trim15 = trimData38.REG.data1.saradc3_ain1_offset15;

    HAL_CRG_IpEnableSet((void *)ADC3, IP_CLK_DISABLE); /* The clock is disabled after calibration */
}

/**
 * @brief Analog module trim.
 * @param None
 * @retval None
 */
static void CHIP_AnalogTrim(void)
{
    VREF_Trim();   /* VREF trim config */
    PGA_Trim();    /* PGA trim config */
    TSENSOR_Trim(); /* TSENSOR trim config */
    ADC0_Trim(); /* ADC0 trim config */
    ADC1_Trim(); /* ADC1 trim config */
    ADC2_Trim(); /* ADC2 trim config */
    ADC3_Trim(); /* ADC3 trim config */
}

/**
 * @brief Parameter calibration entry of the analog module.
 * @param None
 * @retval None
 */
void ANATRIM_Entry(void)
{
    if (CHIP_GetInfo() == false) { /* If the chip information is incorrect, calibration is not performed */
        return;
    }
    CHIP_AnalogTrim();   /* Analog trim config */
}
#endif
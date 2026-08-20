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
  * @file      fotp.h
  * @author    MCU Driver Team
  * @brief     This file provides firmware functions to manage the following
  *            functionalities of the system control register.
  *                + Register Struct of FOTP RNG0 and FOTP RNG1
  */
#ifndef McuMagicTag_FOTP_H
#define McuMagicTag_FOTP_H

#define FOTP_INFO_REG_MAX_ID   59 /* Max index of fotp info */

typedef enum {
    FOTP_INFO_RNG0 = 0,
    FOTP_INFO_RNG1 = 1,
    FOTP_INFO_RNG2 = 2,
    FOTP_INFO_RNG3 = 3,
    FOTP_INFO_RNG4 = 4,
    FOTP_INFO_RNG5 = 20,
    FOTP_INFO_RNG6 = 24,
    FOTP_INFO_RNG7 = 29,
    FOTP_INFO_RNG8 = 30,
    FOTP_INFO_RNG9 = 31,
    FOTP_INFO_MAXTYPE = FOTP_INFO_RNG9,
} FOTP_InfoRngType;

typedef struct {
    unsigned int data[4];
} FOTP_CommonData;

/*
 * FOTP INFO RNG0
 */
typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    DIEID_STD_VER : 3;
            unsigned int    MR_FLAG       : 1;
            unsigned int    LOTID0        : 6;
            unsigned int    LOTID1        : 6;
            unsigned int    LOTID2        : 6;
            unsigned int    LOTID3        : 6;
            unsigned int    LOTID4        : 4;
        } data0;
        struct {
            unsigned int    LOTID4         : 2;
            unsigned int    LOTID5         : 6;
            unsigned int    WAFERID        : 5;
            unsigned int    DIEX           : 8;
            unsigned int    DIEY           : 8;
            unsigned int    PASSFLAG_RT_CP : 1;
            unsigned int    reserved0      : 2;
        } data1;
        unsigned int        reserved_0[2];
    } REG;
} FOTP_INFO_RGN0_NUMBER_0;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    YEAR : 6;
            unsigned int    MON  : 4;
            unsigned int    DAY  : 5;
            unsigned int    HOUR : 5;
            unsigned int    MIN  : 6;
            unsigned int    SEC  : 6;
        } data0;
        struct {
            unsigned int    LOSC_CTRIM    : 9;
            unsigned int    HOSC_CTRM     : 10;
            unsigned int    reserved0      : 1;
            unsigned int    PMU_BG_TRIM   : 5;
            unsigned int    reserved1     : 1;
            unsigned int    PMU_CLDO_TRIM : 5;
            unsigned int    reserved2     : 1;
        } data1;
        struct {
            unsigned int    PMU_CLDO_PCURRENT_TRIM : 3;
            unsigned int    reserved0               : 3;
            unsigned int    PMU_ALDO_TRIM          : 5;
            unsigned int    reserved1              : 1;
            unsigned int    IPADDR0                : 8;
            unsigned int    IPADDR1                : 8;
            unsigned int    IPADDR2                : 4;
        } data2;
        struct {
            unsigned int    IPADDR2               : 4;
            unsigned int    IPADDR3               : 8;
            unsigned int    SITE_NUM_FT           : 5;
            unsigned int    reserved0              : 2;
            unsigned int    PVD_TRIM_1V1          : 3;
            unsigned int    CORE_POR_TRIM_1V1_HYS : 5;
            unsigned int    CORE_POR_TRIM_1V1     : 5;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_1;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        chip_id;
        unsigned int        reserved_0;
        struct {
            unsigned int    version_id : 8;
            unsigned int    reserved0   : 24;
        } data2;
        unsigned int        customer_id;
    } REG;
} FOTP_INFO_RGN0_NUMBER_2;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        fotp_empty_flag;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN0_NUMBER_4;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn0_unlock : 8;
            unsigned int    reserved0         : 24;
        } data0;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn4_unlock : 8;
            unsigned int    reserved0         : 24;
        } data2;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN0_NUMBER_5;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    bootrom_debug_enable : 8;
            unsigned int    reserved0             : 24;
        } data0;
        struct {
            unsigned int    bootrom_hide_disable : 1;
            unsigned int    reserved0             : 31;
        } data1;
        struct {
            unsigned int    ef_bist_intf_enable : 1;
            unsigned int    reserved0            : 31;
        } data2;
        struct {
            unsigned int    dft_jtag_enable : 1;
            unsigned int    reserved0        : 31;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_6;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    cpu_fpu_enable  : 1;
            unsigned int    reserved0        : 7;
            unsigned int    sysram_size_cfg : 3;
            unsigned int    reserved1       : 5;
            unsigned int    eflash_size_cfg : 11;
            unsigned int    reserved2       : 4;
            unsigned int    cpu_maxfreq_cfg : 1;
        } data0;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN0_NUMBER_7;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    adc0_enable  : 1;
            unsigned int    adc1_enable  : 1;
            unsigned int    adc2_enable  : 1;
            unsigned int    dac0_enable  : 1;
            unsigned int    dac1_enable  : 1;
            unsigned int    dac2_enable  : 1;
            unsigned int    pga0_enable  : 1;
            unsigned int    pga1_enable  : 1;
            unsigned int    pga2_enable  : 1;
            unsigned int    acmp0_enable : 1;
            unsigned int    acmp1_enable : 1;
            unsigned int    acmp2_enable : 1;
            unsigned int    acmp3_enable : 1;
            unsigned int    acmp4_enable : 1;
            unsigned int    acmp5_enable : 1;
            unsigned int    acmp6_enable : 1;
            unsigned int    acmp7_enable : 1;
            unsigned int    adc3_enable  : 1;
            unsigned int    dac3_enable  : 1;
            unsigned int    dac4_enable  : 1;
            unsigned int    dac5_enable  : 1;
            unsigned int    dac6_enable  : 1;
            unsigned int    dac7_enable  : 1;
            unsigned int    pga3_enable  : 1;
            unsigned int    reserved0     : 8;
        } data0;
        unsigned int        reserved_0;
        struct {
            unsigned int    apt0_hr_enable : 1;
            unsigned int    apt1_hr_enable : 1;
            unsigned int    apt2_hr_enable : 1;
            unsigned int    apt3_hr_enable : 1;
            unsigned int    apt4_hr_enable : 1;
            unsigned int    apt5_hr_enable : 1;
            unsigned int    reserved0       : 26;
        } data2;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN0_NUMBER_8;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    apt0_enable    : 1;
            unsigned int    apt1_enable    : 1;
            unsigned int    apt2_enable    : 1;
            unsigned int    apt3_enable    : 1;
            unsigned int    apt4_enable    : 1;
            unsigned int    apt5_enable    : 1;
            unsigned int    apt6_enable    : 1;
            unsigned int    apt7_enable    : 1;
            unsigned int    apt8_enable    : 1;
            unsigned int    apt9_enable    : 1;
            unsigned int    apt10_enable   : 1;
            unsigned int    apt11_enable   : 1;
            unsigned int    apt12_enable   : 1;
            unsigned int    apt13_enable   : 1;
            unsigned int    apt14_enable   : 1;
            unsigned int    apt15_enable   : 1;
            unsigned int    can0_enable    : 1;
            unsigned int    can1_enable    : 1;
            unsigned int    can2_enable    : 1;
            unsigned int    can_fd0_enable : 1;
            unsigned int    can_fd1_enable : 1;
            unsigned int    can_fd2_enable : 1;
            unsigned int    reserved0       : 10;
        } data0;
        unsigned int        reserved_0;
        struct {
            unsigned int    eflash_double_bank_hisi : 1;
            unsigned int    reserved0                : 15;
            unsigned int    cpu_refine_dfx          : 1;
            unsigned int    reserved1               : 15;
        } data2;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN0_NUMBER_9;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    IDDQ_DVDD : 8;
            unsigned int    IDDQ_AVDD : 8;
            unsigned int    reserved0  : 16;
        } data0;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN0_NUMBER_10;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    hpm_core : 16;
            unsigned int    reserved0 : 16;
        } data0;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN0_NUMBER_11;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    PASSFLAG_CP_RT : 1;
            unsigned int    reserved0       : 31;
        } data0;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN0_NUMBER_13;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    PASSFLAG_FT_HT : 1;
            unsigned int    reserved0       : 31;
        } data0;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN0_NUMBER_16;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    DVS_FLOW_FLAG : 1;
            unsigned int    DVS_PASS_FLAG : 1;
            unsigned int    reserved0      : 30;
        } data0;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN0_NUMBER_17;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    FAILFLAG_ALL : 2;
            unsigned int    reserved0     : 30;
        } data0;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN0_NUMBER_18;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    PASSFLAG_FT_RT : 1;
            unsigned int    reserved0       : 31;
        } data0;
        unsigned int        reserved_0;
        struct {
            unsigned int    da_pga3_vos_trim : 9;
            unsigned int    reserved0         : 7;
            unsigned int    reserved1        : 16;
        } data2;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN0_NUMBER_19;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    da_ref_vbg_trim   : 8;
            unsigned int    da_ref_vref_trim  : 8;
            unsigned int    da_iref_trim      : 8;
            unsigned int    da_ref_temp_trim0 : 8;
        } data0;
        struct {
            unsigned int    da_ref_temp_trim1  : 8;
            unsigned int    da_ref_temp_trim2  : 8;
            unsigned int    da_ref_temp_trim3  : 8;
            unsigned int    da_ref_vptat0_trim : 8;
        } data1;
        struct {
            unsigned int    da_ref_buf_trim    : 8;
            unsigned int    da_ref_vptat1_trim : 8;
            unsigned int    reserved0           : 8;
            unsigned int    reserved1          : 8;
        } data2;
        struct {
            unsigned int    reserved0         : 8;
            unsigned int    da_ana_top_trim0 : 8;
            unsigned int    da_ana_top_trim1 : 8;
            unsigned int    reserved1        : 8;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_20;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    da_pga0_vos_trim : 9;
            unsigned int    reserved0         : 7;
            unsigned int    da_pga1_vos_trim : 9;
            unsigned int    reserved1        : 7;
        } data0;
        struct {
            unsigned int    da_pga2_vos_trim : 9;
            unsigned int    reserved0         : 7;
            unsigned int    reserved1        : 12;
            unsigned int    reserved2        : 4;
        } data1;
        struct {
            unsigned int    saradc0_gain   : 13;
            unsigned int    reserved0       : 3;
            unsigned int    saradc0_offset : 12;
            unsigned int    reserved1      : 4;
        } data2;
        struct {
            unsigned int    saradc1_gain   : 13;
            unsigned int    reserved0       : 3;
            unsigned int    saradc1_offset : 12;
            unsigned int    reserved1      : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_21;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc2_gain   : 13;
            unsigned int    reserved0       : 3;
            unsigned int    saradc2_offset : 12;
            unsigned int    reserved1      : 4;
        } data0;
        struct {
            unsigned int    saradc3_gain   : 13;
            unsigned int    reserved0       : 3;
            unsigned int    saradc3_offset : 12;
            unsigned int    reserved1      : 4;
        } data1;
        struct {
            unsigned int    saradc0_ain0_gain2   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc0_ain0_offset2 : 12;
            unsigned int    reserved1            : 4;
        } data2;
        struct {
            unsigned int    saradc0_ain0_gain3   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc0_ain0_offset3 : 12;
            unsigned int    reserved1            : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_22;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc0_ain0_gain5   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc0_ain0_offset5 : 12;
            unsigned int    reserved1            : 4;
        } data0;
        struct {
            unsigned int    saradc0_ain0_gain6   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc0_ain0_offset6 : 12;
            unsigned int    reserved1            : 4;
        } data1;
        struct {
            unsigned int    saradc0_ain0_gain7p5   : 13;
            unsigned int    reserved0               : 3;
            unsigned int    saradc0_ain0_offset7p5 : 12;
            unsigned int    reserved1              : 4;
        } data2;
        struct {
            unsigned int    saradc0_ain0_gain10   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc0_ain0_offset10 : 12;
            unsigned int    reserved1             : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_23;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc0_ain0_gain12   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc0_ain0_offset12 : 12;
            unsigned int    reserved1             : 4;
        } data0;
        struct {
            unsigned int    saradc0_ain0_gain15   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc0_ain0_offset15 : 12;
            unsigned int    reserved1             : 4;
        } data1;
        struct {
            unsigned int    saradc0_ain1_gain2   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc0_ain1_offset2 : 12;
            unsigned int    reserved1            : 4;
        } data2;
        struct {
            unsigned int    saradc0_ain1_gain3   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc0_ain1_offset3 : 12;
            unsigned int    reserved1            : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_24;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc0_ain1_gain5   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc0_ain1_offset5 : 12;
            unsigned int    reserved1            : 4;
        } data0;
        struct {
            unsigned int    saradc0_ain1_gain6   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc0_ain1_offset6 : 12;
            unsigned int    reserved1            : 4;
        } data1;
        struct {
            unsigned int    saradc0_ain1_gain7p5   : 13;
            unsigned int    reserved0               : 3;
            unsigned int    saradc0_ain1_offset7p5 : 12;
            unsigned int    reserved1              : 4;
        } data2;
        struct {
            unsigned int    saradc0_ain1_gain10   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc0_ain1_offset10 : 12;
            unsigned int    reserved1             : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_25;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc0_ain1_gain12   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc0_ain1_offset12 : 12;
            unsigned int    reserved1             : 4;
        } data0;
        struct {
            unsigned int    saradc0_ain1_gain15   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc0_ain1_offset15 : 12;
            unsigned int    reserved1             : 4;
        } data1;
        struct {
            unsigned int    saradc1_ain0_gain2   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc1_ain0_offset2 : 12;
            unsigned int    reserved1            : 4;
        } data2;
        struct {
            unsigned int    saradc1_ain0_gain3   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc1_ain0_offset3 : 12;
            unsigned int    reserved1            : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_26;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc1_ain0_gain5   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc1_ain0_offset5 : 12;
            unsigned int    reserved1            : 4;
        } data0;
        struct {
            unsigned int    saradc1_ain0_gain6   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc1_ain0_offset6 : 12;
            unsigned int    reserved1            : 4;
        } data1;
        struct {
            unsigned int    saradc1_ain0_gain7p5   : 13;
            unsigned int    reserved0               : 3;
            unsigned int    saradc1_ain0_offset7p5 : 12;
            unsigned int    reserved1              : 4;
        } data2;
        struct {
            unsigned int    saradc1_ain0_gain10   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc1_ain0_offset10 : 12;
            unsigned int    reserved1             : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_27;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc1_ain0_gain12   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc1_ain0_offset12 : 12;
            unsigned int    reserved1             : 4;
        } data0;
        struct {
            unsigned int    saradc1_ain0_gain15   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc1_ain0_offset15 : 12;
            unsigned int    reserved1             : 4;
        } data1;
        struct {
            unsigned int    saradc1_ain1_gain2   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc1_ain1_offset2 : 12;
            unsigned int    reserved1            : 4;
        } data2;
        struct {
            unsigned int    saradc1_ain1_gain3   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc1_ain1_offset3 : 12;
            unsigned int    reserved1            : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_28;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc1_ain1_gain5   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc1_ain1_offset5 : 12;
            unsigned int    reserved1            : 4;
        } data0;
        struct {
            unsigned int    saradc1_ain1_gain6   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc1_ain1_offset6 : 12;
            unsigned int    reserved1            : 4;
        } data1;
        struct {
            unsigned int    saradc1_ain1_gain7p5   : 13;
            unsigned int    reserved0               : 3;
            unsigned int    saradc1_ain1_offset7p5 : 12;
            unsigned int    reserved1              : 4;
        } data2;
        struct {
            unsigned int    saradc1_ain1_gain10   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc1_ain1_offset10 : 12;
            unsigned int    reserved1             : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_29;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc1_ain1_gain12   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc1_ain1_offset12 : 12;
            unsigned int    reserved1             : 4;
        } data0;
        struct {
            unsigned int    saradc1_ain1_gain15   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc1_ain1_offset15 : 12;
            unsigned int    reserved1             : 4;
        } data1;
        struct {
            unsigned int    saradc2_ain0_gain2   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc2_ain0_offset2 : 12;
            unsigned int    reserved1            : 4;
        } data2;
        struct {
            unsigned int    saradc2_ain0_gain3   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc2_ain0_offset3 : 12;
            unsigned int    reserved1            : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_30;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc2_ain0_gain5   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc2_ain0_offset5 : 12;
            unsigned int    reserved1            : 4;
        } data0;
        struct {
            unsigned int    saradc2_ain0_gain6   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc2_ain0_offset6 : 12;
            unsigned int    reserved1            : 4;
        } data1;
        struct {
            unsigned int    saradc2_ain0_gain7p5   : 13;
            unsigned int    reserved0               : 3;
            unsigned int    saradc2_ain0_offset7p5 : 12;
            unsigned int    reserved1              : 4;
        } data2;
        struct {
            unsigned int    saradc2_ain0_gain10   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc2_ain0_offset10 : 12;
            unsigned int    reserved1             : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_31;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc2_ain0_gain12   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc2_ain0_offset12 : 12;
            unsigned int    reserved1             : 4;
        } data0;
        struct {
            unsigned int    saradc2_ain0_gain15   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc2_ain0_offset15 : 12;
            unsigned int    reserved1             : 4;
        } data1;
        struct {
            unsigned int    saradc2_ain1_gain2   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc2_ain1_offset2 : 12;
            unsigned int    reserved1            : 4;
        } data2;
        struct {
            unsigned int    saradc2_ain1_gain3   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc2_ain1_offset3 : 12;
            unsigned int    reserved1            : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_32;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc2_ain1_gain5   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc2_ain1_offset5 : 12;
            unsigned int    reserved1            : 4;
        } data0;
        struct {
            unsigned int    saradc2_ain1_gain6   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc2_ain1_offset6 : 12;
            unsigned int    reserved1            : 4;
        } data1;
        struct {
            unsigned int    saradc2_ain1_gain7p5   : 13;
            unsigned int    reserved0               : 3;
            unsigned int    saradc2_ain1_offset7p5 : 12;
            unsigned int    reserved1              : 4;
        } data2;
        struct {
            unsigned int    saradc2_ain1_gain10   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc2_ain1_offset10 : 12;
            unsigned int    reserved1             : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_33;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc2_ain1_gain12   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc2_ain1_offset12 : 12;
            unsigned int    reserved1             : 4;
        } data0;
        struct {
            unsigned int    saradc2_ain1_gain15   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc2_ain1_offset15 : 12;
            unsigned int    reserved1             : 4;
        } data1;
        struct {
            unsigned int    saradc3_ain0_gain2   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc3_ain0_offset2 : 12;
            unsigned int    reserved1            : 4;
        } data2;
        struct {
            unsigned int    saradc3_ain0_gain3   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc3_ain0_offset3 : 12;
            unsigned int    reserved1            : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_34;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc3_ain0_gain5   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc3_ain0_offset5 : 12;
            unsigned int    reserved1            : 4;
        } data0;
        struct {
            unsigned int    saradc3_ain0_gain6   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc3_ain0_offset6 : 12;
            unsigned int    reserved1            : 4;
        } data1;
        struct {
            unsigned int    saradc3_ain0_gain7p5   : 13;
            unsigned int    reserved0               : 3;
            unsigned int    saradc3_ain0_offset7p5 : 12;
            unsigned int    reserved1              : 4;
        } data2;
        struct {
            unsigned int    saradc3_ain0_gain10   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc3_ain0_offset10 : 12;
            unsigned int    reserved1             : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_35;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc3_ain0_gain12   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc3_ain0_offset12 : 12;
            unsigned int    reserved1             : 4;
        } data0;
        struct {
            unsigned int    saradc3_ain0_gain15   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc3_ain0_offset15 : 12;
            unsigned int    reserved1             : 4;
        } data1;
        struct {
            unsigned int    saradc3_ain1_gain2   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc3_ain1_offset2 : 12;
            unsigned int    reserved1            : 4;
        } data2;
        struct {
            unsigned int    saradc3_ain1_gain3   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc3_ain1_offset3 : 12;
            unsigned int    reserved1            : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_36;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc3_ain1_gain5   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc3_ain1_offset5 : 12;
            unsigned int    reserved1            : 4;
        } data0;
        struct {
            unsigned int    saradc3_ain1_gain6   : 13;
            unsigned int    reserved0             : 3;
            unsigned int    saradc3_ain1_offset6 : 12;
            unsigned int    reserved1            : 4;
        } data1;
        struct {
            unsigned int    saradc3_ain1_gain7p5   : 13;
            unsigned int    reserved0               : 3;
            unsigned int    saradc3_ain1_offset7p5 : 12;
            unsigned int    reserved1              : 4;
        } data2;
        struct {
            unsigned int    saradc3_ain1_gain10   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc3_ain1_offset10 : 12;
            unsigned int    reserved1             : 4;
        } data3;
    } REG;
} FOTP_INFO_RGN0_NUMBER_37;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    saradc3_ain1_gain12   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc3_ain1_offset12 : 12;
            unsigned int    reserved1             : 4;
        } data0;
        struct {
            unsigned int    saradc3_ain1_gain15   : 13;
            unsigned int    reserved0              : 3;
            unsigned int    saradc3_ain1_offset15 : 12;
            unsigned int    reserved1             : 4;
        } data1;
        struct {
            unsigned int    ts0_offset : 12;
            unsigned int    reserved0   : 4;
            unsigned int    ts1_offset : 12;
            unsigned int    reserved1  : 4;
        } data2;
        unsigned int        reserved_0;
    } REG;
} FOTP_INFO_RGN0_NUMBER_38;

/*
 * FOTP INFO RNG1
 */
typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    main_rgn0_0_size : 11;
            unsigned int    reserved0         : 21;
        } data0;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN1_NUMBER_0;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    uart0_enable   : 1;
            unsigned int    uart1_enable   : 1;
            unsigned int    uart2_enable   : 1;
            unsigned int    uart3_enable   : 1;
            unsigned int    uart4_enable   : 1;
            unsigned int    uart5_enable   : 1;
            unsigned int    uart6_enable   : 1;
            unsigned int    uart7_enable   : 1;
            unsigned int    IWDG_DEEPSLEEP : 1;
            unsigned int    reserved0       : 1;
            unsigned int    IWDG_SW        : 1;
            unsigned int    reserved1      : 13;
            unsigned int    verify_disable : 1;
            unsigned int    reserved2      : 7;
        } data0;
        struct {
            unsigned int    sysram_parity_disable : 1;
            unsigned int    sysram_ecc_disable    : 4;
            unsigned int    sysram_init_disable   : 1;
            unsigned int    reserved              : 26;
        } data1;
        struct {
            unsigned int    uart_boot_enable  : 1;
            unsigned int    spi_boot_enable   : 1;
            unsigned int    i2c_boot_enable   : 1;
            unsigned int    can_boot_enable   : 1;
            unsigned int    smbus_boot_enable : 1;
            unsigned int    reserved          : 27;
        } data2;
        unsigned int        reserved;
    } REG;
} FOTP_INFO_RGN1_NUMBER_8;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    eflash_double_bank_user : 1;
            unsigned int    reserved                : 31;
        } data0;
        unsigned int        reserved_0;
        unsigned int        user_id;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN1_NUMBER_16;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    jtag_pswd_rden : 4;
            unsigned int    reserved       : 28;
        } data2;
        unsigned int        reserved_0;
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN1_NUMBER_24;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        reserved_0[2];
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN1_NUMBER_25;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        reserved_0[2];
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN1_NUMBER_32;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        reserved_0[2];
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN1_NUMBER_33;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        reserved_0[2];
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN1_NUMBER_40;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        reserved_0[2];
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN1_NUMBER_41;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        reserved_0[2];
        struct {
            unsigned int    main_rgn0_0_protection_level : 8;
            unsigned int    reserved                     : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN1_NUMBER_48;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        reserved_0[2];
        unsigned int        reserved_1[2];
    } REG;
} FOTP_INFO_RGN1_NUMBER_52;

typedef union {
    FOTP_CommonData    comData;
    struct {
        unsigned int        reserved_0[2];
        struct {
            unsigned int    info_rgn1_0_unlock : 8;
            unsigned int    reserved0           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN1_NUMBER_56;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn1_1_unlock : 8;
            unsigned int    reserved0           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn1_2_unlock : 8;
            unsigned int    reserved0           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN1_NUMBER_57;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn1_3_unlock : 8;
            unsigned int    reserved0           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn1_4_unlock : 8;
            unsigned int    reserved0           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN1_NUMBER_58;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn1_5_unlock : 8;
            unsigned int    reserved0           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn1_6_unlock : 8;
            unsigned int    reserved0           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN1_NUMBER_59;

/*
 * FOTP INFO RNG2
 */
typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    main_rgn0_1_start_addr : 10;
            unsigned int    reserved               : 6;
            unsigned int    main_rgn0_1_end_addr   : 10;
            unsigned int    reserved1              : 6;
        } data0;
        struct {
            unsigned int    main_rgn0_1_code_end : 10;
            unsigned int    reserved             : 22;
        } data1;
        struct {
            unsigned int    sram_rgn0_1_start_addr : 10;
            unsigned int    reserved               : 6;
            unsigned int    sram_rgn0_1_end_addr   : 10;
            unsigned int    reserved1              : 6;
        } data2;
        struct {
            unsigned int    sram_rgn0_1_cpsrc_addr : 10;
            unsigned int    reserved               : 14;
            unsigned int    sram_rgn0_1_disable    : 8;
        } data3;
    } REG;
} FOTP_INFO_RGN2_NUMBER_0;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    main_rgn0_1_protection_level : 8;
            unsigned int    reserved                     : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn2_0_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN2_NUMBER_56;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn2_1_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn2_2_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN2_NUMBER_57;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn2_3_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn2_4_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN2_NUMBER_58;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn2_5_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn2_6_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN2_NUMBER_59;

/*
 * FOTP INFO RNG3
 */
typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    main_rgn0_1_start_addr : 10;
            unsigned int    reserved               : 6;
            unsigned int    main_rgn0_1_end_addr   : 10;
            unsigned int    reserved1              : 6;
        } data0;
        struct {
            unsigned int    main_rgn0_1_code_end : 10;
            unsigned int    reserved             : 22;
        } data1;
        struct {
            unsigned int    sram_rgn0_1_start_addr : 10;
            unsigned int    reserved               : 6;
            unsigned int    sram_rgn0_1_end_addr   : 10;
            unsigned int    reserved1              : 6;
        } data2;
        struct {
            unsigned int    sram_rgn0_1_cpsrc_addr : 10;
            unsigned int    reserved               : 14;
            unsigned int    sram_rgn0_1_disable    : 8;
        } data3;
    } REG;
} FOTP_INFO_RGN3_NUMBER_0;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    main_rgn0_1_protection_level : 8;
            unsigned int    reserved                     : 24;
        } data0;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn2_0_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN3_NUMBER_56;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn2_1_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn2_2_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN3_NUMBER_57;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn2_3_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn2_4_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN3_NUMBER_58;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn2_5_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn2_6_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN3_NUMBER_59;

/*
 * FOTP INFO RNG5
 */
typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn5_unlock   : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        unsigned int        reserved_1;
        unsigned int        reserved_2;
    } REG;
} FOTP_INFO_RGN5_NUMBER_0;


/*
 * FOTP INFO RNG6
 */
typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn6_unlock   : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        unsigned int        reserved_1;
        unsigned int        reserved_2;
    } REG;
} FOTP_INFO_RGN6_NUMBER_0;


/*
 * FOTP INFO RNG7
 */
typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    main_rgn0_3_start_addr : 10;
            unsigned int    reserved               : 6;
            unsigned int    main_rgn0_3_end_addr   : 10;
            unsigned int    reserved1              : 6;
        } data0;
        struct {
            unsigned int    main_rgn0_3_code_end : 10;
            unsigned int    reserved             : 22;
        } data1;
        struct {
            unsigned int    sram_rgn0_3_start_addr : 10;
            unsigned int    reserved               : 6;
            unsigned int    sram_rgn0_3_end_addr   : 10;
            unsigned int    reserved1              : 6;
        } data2;
        struct {
            unsigned int    sram_rgn0_3_cpsrc_addr : 10;
            unsigned int    reserved               : 14;
            unsigned int    sram_rgn0_3_disable    : 8;
        } data3;
    } REG;
} FOTP_INFO_RGN7_NUMBER_0;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    main_rgn0_3_protection_level : 8;
            unsigned int    reserved                     : 24;
        } data0;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn7_0_unlock : 8;
            unsigned int    reserved           : 24;
        } data1;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN7_NUMBER_56;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn7_1_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn7_2_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN7_NUMBER_57;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn7_3_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn7_4_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN7_NUMBER_58;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn7_5_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn7_6_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN7_NUMBER_59;


/*
 * FOTP INFO RNG8
 */
typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    main_rgn0_4_start_addr : 10;
            unsigned int    reserved               : 6;
            unsigned int    main_rgn0_4_end_addr   : 10;
            unsigned int    reserved1              : 6;
        } data0;
        struct {
            unsigned int    main_rgn0_4_code_end : 10;
            unsigned int    reserved             : 22;
        } data1;
        struct {
            unsigned int    sram_rgn0_4_start_addr : 10;
            unsigned int    reserved               : 6;
            unsigned int    sram_rgn0_4_end_addr   : 10;
            unsigned int    reserved1              : 6;
        } data2;
        struct {
            unsigned int    sram_rgn0_4_cpsrc_addr : 10;
            unsigned int    reserved               : 14;
            unsigned int    sram_rgn0_4_disable    : 8;
        } data3;
    } REG;
} FOTP_INFO_RGN8_NUMBER_0;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    main_rgn0_4_protection_level : 8;
            unsigned int    reserved                     : 24;
        } data0;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn8_0_unlock : 8;
            unsigned int    reserved           : 24;
        } data1;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN8_NUMBER_56;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn8_1_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn8_2_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN8_NUMBER_57;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn8_3_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn8_4_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN8_NUMBER_58;

typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    info_rgn8_5_unlock : 8;
            unsigned int    reserved           : 24;
        } data2;
        unsigned int        reserved_0;
        struct {
            unsigned int    info_rgn8_6_unlock : 8;
            unsigned int    reserved           : 24;
        } data0;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN8_NUMBER_59;

/*
 * FOTP INFO RNG9
 */
typedef union {
    FOTP_CommonData    comData;
    struct {
        struct {
            unsigned int    eflash_bank_remap : 8;
            unsigned int    reserved          : 24;
        } data0;
        unsigned int        reserved_0;
        struct {
            unsigned int    info9_unlock : 8;
            unsigned int    reserved     : 24;
        } data2;
        unsigned int        reserved_1;
    } REG;
} FOTP_INFO_RGN9_NUMBER_0;
#endif
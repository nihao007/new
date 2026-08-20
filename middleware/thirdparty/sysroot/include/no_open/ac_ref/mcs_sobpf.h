/**
  * @ Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2025. All rights reserved.
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
  * @file     mcs_trq_comp_adpt.h
  * @author   MCU Algorithm Team
  * @brief    This file provides functions declaration of the Second-order BPF filter module.
*/

#ifndef McuMagicTag_SOBPF_H
#define McuMagicTag_SOBPF_H

#include "mcs_filter.h"
/**
  * @brief Structure of the transfer function numerator numerator denominator polynomial coefficients.
  */
typedef struct {
    float num[3];
    float den[3];
} TF_PARAM_Handle;

/**
  * @brief Structure of the second-order bandpass filter.
  */
typedef struct {
    float zNum[3];
    float zDen[3];
    float uLast;
    float uLastLast;
    float yLast;
    float yLastLast;
    float ts;
} SOBPF_Handle;

void SOBPF_Clear(SOBPF_Handle *bpfHandle);
float SOBPF_Exec(SOBPF_Handle *bpfHandle, float fDataIn);
void SOBPF_Init(SOBPF_Handle *bpfHandle, float freq, float ts);

#endif
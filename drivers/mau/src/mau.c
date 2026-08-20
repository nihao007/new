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
 * @file    mau.c
 * @author  MCU Driver Team.
 * @brief   Programmable Gain Amplifier HAL level module driver.
 *          This file provides firmware functions to manage the following
 *          functionalities of the amplifier
 *           + Programmable Gain Amplifier's Initialization and de-initialization functions
 *           + Set amplifier's gain value
 */
#include "mau.h"
#include "HiMCUGenIntrinsics.h"
#define MAU_PI (3.141593f)
#define MAU_HALFPI (1.5707963f)
#define MAU_2DIVPI (0.6366198f)
#define BSPUB_RISCV 1
/**
  * @brief Calculate Sin Values.
  * @brief y=sin(x*pi/2).
  * @param val Angle value to be calculated.
  * @retval float Calculated sin value.
  */
float HAL_MAU_Sin(float val)
{
    /* y=sin(x*pi/2 ) -> y=sin(x*pi/2 *2/pi) ==> y=sinx */
    return RISCV_UCI_SP_1R1W(val * MAU_2DIVPI, COM_MAU_FUNC_SIN_PI_2_X);
}

/**
  * @brief Calculate Cos Values.
  * @brief y=cos(x*pi/2).
  * @param val Angle value to be calculated.
  * @retval float Calculated cos value.
  */
float HAL_MAU_Cos(float val)
{
    /* y=cos(x*pi/2 ) -> y=cos(x*pi/2 *2/pi) ==> y=cosx */
    return RISCV_UCI_SP_1R1W(val * MAU_2DIVPI, COM_MAU_FUNC_COS_PI_2_X);
}

/**
  * @brief Fast sqrt calculation using MAU.
  * @param val Float val.
  * @retval Sqrt result.
  */
float HAL_MAU_Sqrt(float val)
{
    MAU_ASSERT_PARAM(val >= 0.0f);
    return RISCV_UCI_SP_1R1W(val, COM_MAU_FUNC_SQRT_X);
}

/**
  * @brief Calculating arc tangent.
  * @brief y=arctan(x) * 2 / PI.
  * @param val: Target Value.
  * @retval Arctangent value.
  */
float HAL_MAU_ATan(float val)
{
    /* NOTICE : the result = arctan(val) */
    return RISCV_UCI_SP_1R1W(val, COM_MAU_FUNC_ATAN_X_2_PI) * MAU_HALFPI;
}
/**
  * @brief Reciprocal calculation.
  * @brief y=1/x.
  * @param val The value to be Rcp.
  * @retval Rcp results.
  */
float HAL_MAU_Rcp(float val)
{
    MAU_ASSERT_PARAM(val > FLT_EPSILON || val < -FLT_EPSILON);
    return RISCV_UCI_SP_1R1W(val, COM_MAU_FUNC_RCP_X);
}

/**
  * @brief Reciprocal square root calculation.
  * @brief y=1/√x.
  * @param val The value to be Rsq.
  * @retval Rsq results.
  */
float HAL_MAU_Rsq(float val)
{
    MAU_ASSERT_PARAM(val > 0.0f);
    return RISCV_UCI_SP_1R1W(val, COM_MAU_FUNC_RSQ_X);
}

/**
  * @brief Power exponent calculation.
  * @brief y=2^x.
  * @param val The value to be Rsq.
  * @retval Exp2 results.
  */
float HAL_MAU_Exp2(float val)
{
    return RISCV_UCI_SP_1R1W(val, COM_MAU_FUNC_EXP2_X);
}

/**
  * @brief Log2 calculation.
  * @brief y=log2^x.
  * @param val The value to be Log2.
  * @retval Log2 results.
  */
float HAL_MAU_Log2(float val)
{
    MAU_ASSERT_PARAM(val > 0.0f);
    return RISCV_UCI_SP_1R1W(val, COM_MAU_FUNC_LOG2_X);
}

/**
  * @brief Ln calculation.
  * @brief y=lnx.
  * @param val The value to be Ln.
  * @retval Ln results.
  */
float HAL_MAU_Ln(float val)
{
    MAU_ASSERT_PARAM(val > 0.0f);
    return RISCV_UCI_SP_1R1W(val, COM_MAU_FUNC_LN_X);
}
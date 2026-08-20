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
 * @file    dac_ex.c
 * @author  MCU Driver Team.
 * @brief   DAC HAL level module driver.
 *          This file provides firmware functions to manage the following
 *          functionalities of the DAC and Comparator.
 *           + DAC's Initialization and de-initialization functions
 *           + Set DAC value function
 */
#include "dac_ex.h"

/**
  * @brief Init DAC Vset attribute.
  * @param dacHandle, Value of @ref DAC handle.
  * @param vsetAttr, Value of @ref DAC_VsetAttr.
  * @retval None.
  */
BASE_StatusType HAL_DAC_InitVsetAttrEx(DAC_Handle *dacHandle, DAC_VsetAttr vsetAttr)
{
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    /* Set Vset source, load mode, syncp source. */
    DCL_DAC_SetVsetSrc(dacHandle->baseAddress, vsetAttr.vsetSrc);
    DCL_DAC_SetVsetLoadMode(dacHandle->baseAddress, vsetAttr.vsetLoadMode);
    if (vsetAttr.vsetLoadMode == VSET_LOAD_APTSYNCP_MODE) { /* Load vset value from apt syncp signal. */
        DCL_DAC_SetVsetSyncpSrc(dacHandle->baseAddress, vsetAttr.vsetSyncpSrc);
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Init DAC ramp attribute.
  * @param dacHandle, Value of @ref DAC handle.
  * @param rampAttr, Value of @ref DAC_RampAttr.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_DAC_InitRampAttrEx(DAC_Handle *dacHandle, DAC_RampAttr rampAttr)
{
    /* Repeat config stable time */
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    if (DCL_DAC_GetVsetSrc(dacHandle->baseAddress) == VSET_SRC_FROM_RAMP) {  /* Vset source from ramp */
        DCL_DAC_SetRampLoadMode(dacHandle->baseAddress, rampAttr.rampLoadMode);  /* ramp load mode */
        DCL_DAC_SetRampXtrig(dacHandle->baseAddress, rampAttr.rampXtrig);  /* ramp cross-connection trigger */
        DCL_DAC_SetRampDir(dacHandle->baseAddress, rampAttr.rampDir);   /* ramp direction */
        DCL_DAC_SetRampStep(dacHandle->baseAddress, rampAttr.rampStep);  /* ramp step */
        DCL_DAC_SetRampClkDiv(dacHandle->baseAddress, rampAttr.rampClkDiv);    /* ramp clock division */
        DCL_DAC_SetRampRefVal(dacHandle->baseAddress, rampAttr.rampRefVal);  /* ramp ref value */
        DCL_DAC_SetRampDelayVal(dacHandle->baseAddress, rampAttr.rampDelayVal);  /* ramp delay value */
        DCL_DAC_SetRampDebugMode(dacHandle->baseAddress, rampAttr.rampDebugMode);  /* ramp debug mode */
    }
    return BASE_STATUS_OK;
}

/**
  * @brief Set DAC basic attribute config by type.
  * @param dacHandle, Value of @ref DAC handle.
  * @param basicType, Value of @ref DAC_BasicType.
  * @param value, value to be config.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_DAC_SetBasicConfigEx(DAC_Handle *dacHandle, DAC_BasicType basicType, unsigned int value)
{
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    BASE_StatusType status = BASE_STATUS_OK;
    switch (basicType) {
        case TYPE_VREF_SRC:    /* vref source */
            DAC_PARAM_CHECK_WITH_RET(value < DAC_VREF_MAX, BASE_STATUS_ERROR);
            DCL_DAC_SetVrefSrc(dacHandle->baseAddress, (DAC_VrefSrc)value);
            break;
        case TYPE_VSET_SRC:   /* vset source */
            DAC_PARAM_CHECK_WITH_RET(value < VSET_SRC_MAX, BASE_STATUS_ERROR);
            DCL_DAC_SetVsetSrc(dacHandle->baseAddress, (DAC_VsetSrc)value);
            break;
        case TYPE_VSET_LOAD_MODE:  /* vset load mode */
            DAC_PARAM_CHECK_WITH_RET(value < VSET_LOAD_MAX, BASE_STATUS_ERROR);
            DCL_DAC_SetVsetLoadMode(dacHandle->baseAddress, (DAC_VsetLoadMode)value);
            break;
        case TYPE_VSET_SYNCP_SRC:    /* vset syncp source */
            DAC_PARAM_CHECK_WITH_RET(value < VSET_SYNCP_MAX, BASE_STATUS_ERROR);
            DCL_DAC_SetVsetSyncpSrc(dacHandle->baseAddress, (DAC_VsetSyncpSrc)value);
            break;
        case TYPE_VSET_VALUE:    /* vset value */
            HAL_DAC_SetValue(dacHandle, value);
            break;
        default:
            status = BASE_STATUS_ERROR;
            break;
    }
    return status;
}

/**
  * @brief Set DAC ramp attribute config by type.
  * @param dacHandle, Value of @ref DAC handle.
  * @param rampType, Value of @ref DAC_RampType.
  * @param value, value to be config.
  * @retval BASE_StatusType: OK, ERROR, BUSY, TIMEOUT.
  */
BASE_StatusType HAL_DAC_SetRampConfigEx(DAC_Handle *dacHandle, DAC_RampType rampType, unsigned int value)
{
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    BASE_StatusType status = BASE_STATUS_OK;
    switch (rampType) {
        case TYPE_RAMP_LOAD_MODE:    /* ramp load mode */
            DAC_PARAM_CHECK_WITH_RET(value < RAMP_LOAD_MAX, BASE_STATUS_ERROR);
            DCL_DAC_SetRampLoadMode(dacHandle->baseAddress, (DAC_RampLoadMode)value);
            break;
        case TYPE_RAMP_XTRIG:    /* ramp cross-connect trigger */
            DAC_PARAM_CHECK_WITH_RET(value < RAMP_XTRIG_MAX, BASE_STATUS_ERROR);
            DCL_DAC_SetRampXtrig(dacHandle->baseAddress, (DAC_RampXtrig)value);
            break;
        case TYPE_RAMP_DIR:    /* ramp direction */
            DAC_PARAM_CHECK_WITH_RET(value < RAMP_DIR_MAX, BASE_STATUS_ERROR);
            DCL_DAC_SetRampDir(dacHandle->baseAddress, (DAC_RampDir)value);
            break;
        case TYPE_RAMP_STEP:    /* ramp step */
            DAC_PARAM_CHECK_WITH_RET(value < 0xFFFF, BASE_STATUS_ERROR);  /* 0xFFFF : step max */
            DCL_DAC_SetRampStep(dacHandle->baseAddress, (unsigned short)value);
            break;
        case TYPE_RAMP_CLK_DIV:
            DAC_PARAM_CHECK_WITH_RET(value < 0xFF, BASE_STATUS_ERROR);    /* 0xFF : clk div max */
            DCL_DAC_SetRampClkDiv(dacHandle->baseAddress, (unsigned char)value);
            break;
        case TYPE_RAMP_REF_VAL:    /* ramp ref value */
            DAC_PARAM_CHECK_WITH_RET(value < 0xFFFF, BASE_STATUS_ERROR);   /* 0xFFFF : ref value max */
            DCL_DAC_SetRampRefVal(dacHandle->baseAddress, (unsigned short)value);
            break;
        case TYPE_RAMP_DELAY_VAL:    /* ramp delay value */
            DAC_PARAM_CHECK_WITH_RET(value < 0xFFFF, BASE_STATUS_ERROR);     /* 0xFFFF : delay value max */
            DCL_DAC_SetRampDelayVal(dacHandle->baseAddress, (unsigned short)value);
            break;
        case TYPE_RAMP_DEBUG_MODE:    /* ramp debug mode */
            DAC_PARAM_CHECK_WITH_RET(value < RAMP_DEBUG_MAX, BASE_STATUS_ERROR);
            DCL_DAC_SetRampDebugMode(dacHandle->baseAddress, (DAC_RampDebugMode)value);
            break;
        default:
            status = BASE_STATUS_ERROR;
            break;
    }
    return status;
}

/**
  * @brief Get DAC basic config by basic type.
  * @param dacHandle, Value of @ref DAC handle.
  * @param basicType, Value of @ref DAC_BasicType.
  * @retval value of basic config.
  */
unsigned int HAL_DAC_GetBasicConfigEx(DAC_Handle *dacHandle, DAC_BasicType basicType)
{
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    unsigned int ret = UINT_MAX;   /* UINT_MAX means error */
    switch (basicType) {
        case TYPE_VREF_SRC:  /* vref source */
            ret = DCL_DAC_GetVrefSrc(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < DAC_VREF_MAX, UINT_MAX);
            break;
        case TYPE_VSET_SRC:  /* vset source */
            ret = DCL_DAC_GetVsetSrc(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < VSET_SRC_MAX, UINT_MAX);
            break;
        case TYPE_VSET_LOAD_MODE:    /* vset load mode */
            ret = DCL_DAC_GetVsetLoadMode(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < VSET_LOAD_MAX, UINT_MAX);
            break;
        case TYPE_VSET_SYNCP_SRC:   /* vset syncp src */
            ret = DCL_DAC_GetVsetSyncpSrc(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < VSET_SYNCP_MAX, UINT_MAX);
            break;
        case TYPE_VSET_VALUE:     /* vset value */
            if (DCL_DAC_GetVsetSrc(dacHandle->baseAddress) == VSET_SRC_FROM_REG) {
                ret = DCL_DAC_GetValue(dacHandle->baseAddress);
                DAC_PARAM_CHECK_WITH_RET(ret < DAC_MAX_OUT_VALUE, UINT_MAX);
            }
            break;
        default:
            break;
    }
    return ret;
}

/**
  * @brief Get DAC Ramp real time value by ramp type.
  * @param dacHandle, Value of @ref DAC handle.
  * @param rampType, Value of @ref DAC_RampType.
  * @retval value of real time. ref or delay
  */
static unsigned int GetDacRampRealtimeValueEx(DAC_Handle *dacHandle, DAC_RampType rampType)
{
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    unsigned int ret = UINT_MAX;  /* UINT_MAX means error */
    switch (rampType) {
        case TYPE_RAMP_RT_REF_VAL:   /* ramp real time ref value */
            ret = DCL_DAC_GetRampRealtimeRefVal(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < 0xFFFF, UINT_MAX);     /* 0xFFFF : ref value max */
            break;
        case TYPE_RAMP_RT_DELAY_VAL:   /* ramp real time delay value */
            ret = DCL_DAC_GetRampRealtimeDelayVal(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < 0xFFFF, UINT_MAX);     /* 0xFFFF : delay value max */
            break;
        default:
            break;
    }
    return ret;
}

/**
  * @brief Get DAC Ramp real time value by ramp type.
  * @param dacHandle, Value of @ref DAC handle.
  * @param rampType, Value of @ref DAC_RampType.
  * @retval value of ram cofig value.
  */
unsigned int HAL_DAC_GetRampConfigEx(DAC_Handle *dacHandle, DAC_RampType rampType)
{
    DAC_ASSERT_PARAM(dacHandle != NULL);
    DAC_ASSERT_PARAM(IsDACInstance(dacHandle->baseAddress));
    unsigned int ret = UINT_MAX;  /* UINT_MAX means error */
    switch (rampType) {
        case TYPE_RAMP_LOAD_MODE: /* ramp load mode */
            ret = DCL_DAC_GetRampLoadMode(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < RAMP_LOAD_MAX, UINT_MAX);
            break;
        case TYPE_RAMP_XTRIG:    /* ramp cross-connect trigger */
            ret = DCL_DAC_GetRampXtrig(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < RAMP_XTRIG_MAX, UINT_MAX);
            break;
        case TYPE_RAMP_DIR:   /* ramp direction */
            ret = DCL_DAC_GetRampDir(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < RAMP_DIR_MAX, UINT_MAX);
            break;
        case TYPE_RAMP_STEP:   /* ramp step */
            ret = DCL_DAC_GetRampStep(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < 0xFFFF, UINT_MAX);  /* 0xFFFF : step max */
            break;
        case TYPE_RAMP_CLK_DIV:   /* ramp clock division */
            ret = DCL_DAC_GetRampClkDiv(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < 0xFF, UINT_MAX);    /* 0xFF : clk div max */
            break;
        case TYPE_RAMP_REF_VAL:   /* ramp ref value */
            ret = DCL_DAC_GetRampRefVal(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < 0xFFFF, UINT_MAX);   /* 0xFFFF : ref value max */
            break;
        case TYPE_RAMP_DELAY_VAL:   /* ramp delay value */
            ret = DCL_DAC_GetRampDelayVal(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < 0xFFFF, UINT_MAX);     /* 0xFFFF : delay value max */
            break;
        case TYPE_RAMP_DEBUG_MODE:   /* ramp debug mode */
            ret = DCL_DAC_GetRampDebugMode(dacHandle->baseAddress);
            DAC_PARAM_CHECK_WITH_RET(ret < RAMP_DEBUG_MAX, UINT_MAX);
            break;
        default:
            ret = GetDacRampRealtimeValueEx(dacHandle, rampType); /* get real time config */
            break;
    }
    return ret;
}
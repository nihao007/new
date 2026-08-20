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
  * @file    ai_common.h
  * @author  MCU Driver Team
  * @brief   Public internal processing handle.
  */

#ifndef AI_COMMON_H
#define AI_COMMON_H
#include "typedefs.h"
/* ----------- Refrigerator System Type ---------------  */
#define  SINGLE_SYSTEM_DUAL_COMPARTMENT    0                    // Single-system dual-compartment refrigerator
#define  TRIPLE_SYSTEM_TRIPLE_COMPARTMENT  1                    // Triple System Triple Chamber Refrigerator

#define  FRIDGE_TOPOLOGY_TYPE   SINGLE_SYSTEM_DUAL_COMPARTMENT  // Refrigerator topology type
#if (FRIDGE_TOPOLOGY_TYPE == TRIPLE_SYSTEM_TRIPLE_COMPARTMENT)
/* ------------- Temp Setting ----------- */
#define   LD_TEMP_TARGET    (-17.44f)
#define   LC_TEMP_TARGET    (4.90f)
#define   BW_TEMP_TARGET    (-3.2f)

/* ------------- Weight Setting ----------- */
#define   LD_WEIGHT_VALUE     (404)
#define   LC_WEIGHT_VALUE     (748)
#define   BW_WEIGHT_VALUE     (544)

/* Weight Adjustment Rate. */
#define LC_WEIGHT_ADJUST_RATE  (84.0)
#define LD_WEIGHT_ADJUST_RATE  (50.0)
#define BW_WEIGHT_ADJUST_RATE  (50.0)

/* Weight upper and lower limits */
#define LD_WEIGHT_UPPER_VALUE  (1444.0)
#define LD_WEIGHT_LOWER_VALUE  (-200.0)

#define LC_WEIGHT_UPPER_VALUE  (1844.0)
#define LC_WEIGHT_LOWER_VALUE  (450.0)

#define BW_WEIGHT_UPPER_VALUE  (848.0)
#define BW_WEIGHT_LOWER_VALUE  (-200.0)

/* ------------- Model Update Setting ---------- */
#define   MODEL_UPDATE_FLAG    (0)

/* ---------- Maximum Refrigeration Request Time: 30 mins ----------- */
#define   LC_REQUEST_MAX_TIME  (30)

/* ------------- Compression speed amplitude setting. ----------- */
#define  COMPRESSOR_MAX_SPEED   (1480.0f)
#define  COMPRESSOR_MIN_SPEED   (900.0f)

/* -------------Fan speed level amplitude setting. ----------- */
#define LC_FAN_UPPER_GEAR      (18)
#define LC_FAN_LOWER_GEAR      (4)

#define LD_FAN_UPPER_GEAR      (8)
#define LD_FAN_LOWER_GEAR      (2)

#define BW_FAN_UPPER_GEAR      (2)
#define BW_FAN_LOWER_GEAR      (0)

#define LN_FAN_COMPRESSOR432       (4320)
#define LN_FAN_GEAR432             (16)

#define LN_FAN_COMPRESSOR320       (3200)
#define LN_FAN_GEAR3200            (14)

#define LN_FAN_COMPRESSOR240       (2400)
#define LN_FAN_GEAR240             (12)

#define LN_FAN_COMPRESSOR170       (1700)
#define LN_FAN_GEAR170             (9)

#define LN_FAN_COMPRESSOR100       (1000)
#define LN_FAN_GEAR100             (7)

#define LN_FAN_COMPRESSOR80        (800)
#define LN_FAN_GEAR80              (0)

#elif (FRIDGE_TOPOLOGY_TYPE == SINGLE_SYSTEM_DUAL_COMPARTMENT)
/**
 * ATTENTION: The refrigerator has two compartments: LC compartment and LD compartment.
              The setting of the BW compartment does not work.
 */

/* ------------- Temp Setting ----------- */
#define   LD_TEMP_TARGET      (-19.3f)
#define   LC_TEMP_TARGET      (6.80f)
#define   BW_TEMP_TARGET      (-5.5f)

/* ------------- Weight Setting ----------- */
#define   LD_WEIGHT_VALUE     (7900)
#define   LC_WEIGHT_VALUE     (5725)
#define   BW_WEIGHT_VALUE     (465)

/* Weight Adjustment Rate. */
#define LC_WEIGHT_ADJUST_RATE  (-180.0)
#define LD_WEIGHT_ADJUST_RATE  (340.0)
#define BW_WEIGHT_ADJUST_RATE  (0.0)

/* Weight upper and lower limits */
#define LD_WEIGHT_UPPER_VALUE  (9264.0)
#define LD_WEIGHT_LOWER_VALUE  (-200.0)

#define LC_WEIGHT_UPPER_VALUE  (9854.0)
#define LC_WEIGHT_LOWER_VALUE  (450.0)

#define BW_WEIGHT_UPPER_VALUE  (848.0)
#define BW_WEIGHT_LOWER_VALUE  (-200.0)
/* ------------- Model Update Setting ---------- */
#define   MODEL_UPDATE_FLAG    (0)

/* ---------- Maximum Refrigeration Request Time: 30 mins ----------- */
#define   LC_REQUEST_MAX_TIME  (30)

/* ------------- Recommended compressor speed range. ----------- */
#define  COMPRESSOR_MAX_SPEED   (1680.0f)
#define  COMPRESSOR_MIN_SPEED   (1280.0f)

/* -------------Fan speed level amplitude setting. ----------- */
#define LC_FAN_UPPER_GEAR      (18)
#define LC_FAN_LOWER_GEAR      (4)

#define LD_FAN_UPPER_GEAR      (12)
#define LD_FAN_LOWER_GEAR      (2)

#define BW_FAN_UPPER_GEAR      (0)
#define BW_FAN_LOWER_GEAR      (0)

#define LN_FAN_COMPRESSOR432       (4320)
#define LN_FAN_GEAR432             (16)

#define LN_FAN_COMPRESSOR320       (3200)
#define LN_FAN_GEAR3200            (14)

#define LN_FAN_COMPRESSOR240       (2400)
#define LN_FAN_GEAR240             (12)

#define LN_FAN_COMPRESSOR170       (1700)
#define LN_FAN_GEAR170             (9)

#define LN_FAN_COMPRESSOR100       (1000)
#define LN_FAN_GEAR100             (8)

#define LN_FAN_COMPRESSOR80        (800)
#define LN_FAN_GEAR80              (0)

# endif

/* Flash AI Model Data Backup Area 1 */
#define LC_STATE_DATA_ADDR1     (0x40000)
#define LC_ACTION_DATA_ADDR1    (0x40800)

#define LD_STATE_DATA_ADDR1     (0x41000)
#define LD_ACTION_DATA_ADDR1    (0x41800)

#define BW_STATE_DATA_ADDR1     (0x42000)
#define BW_ACTION_DATA_ADDR1    (0x42800)

/* Flash AI Model Data Backup Area 2 */
#define LC_STATE_DATA_ADDR2     (0x43000)
#define LC_ACTION_DATA_ADDR2    (0x43800)

#define LD_STATE_DATA_ADDR2     (0x44000)
#define LD_ACTION_DATA_ADDR2    (0x44800)

#define BW_STATE_DATA_ADDR2     (0x45000)
#define BW_ACTION_DATA_ADDR2    (0x45800)

/* Flash Storage cycle */
#define STORAGE_DATA_PERIOD    (240)           // 240 mins

/**
 * @brief Observation Parameters Structure.
 */
typedef struct {
    double ldTemp;               /**< Current temperature for LD. */
    double lcTemp;               /**< Current temperature for LC. */
    double bwTemp;               /**< Current temperature for BW. */

    double ldHSTemp;             /**< Target temperature for LD. */
    double lcHSTemp;             /**< Target temperature for LC. */
    double bwHSTemp;             /**< Target temperature for BW. */

    unsigned int compFeedSpeed;  /**< Compressor feed speed. */
    unsigned int compFeedPower;  /**< Compressor feed power. */
    unsigned int powerSummary;   /**< Summary of power usage. */

    unsigned int ambientHum;     /**< Ambient humidity. */

    float lcSettingTemp;         /**< Setting temperature for LC. */
    float ldSettingTemp;         /**< Setting temperature for LD. */
    float bwSettingTemp;         /**< Setting temperature for BW. */
    float ambientTemp;           /**< Ambient temperature. */

    unsigned char lcFanGear;     /**< Fan gear setting for LC. */
    unsigned char ldFanGear;     /**< Fan gear setting for LD. */
    unsigned char bwFanGear;     /**< Fan gear setting for BW. */
    unsigned char lnFanGear;     /**< Fan gear setting for LN. */

    bool dhsStatus;              /**< Status of the DHS system. */
    bool zrhsStatus;             /**< Status of the ZRHS system. */

    bool lcDoorStatus;           /**< Door status for LC. */
    bool ldDoorStatus;           /**< Door status for LD. */
    bool bwDoorStatus;           /**< Door status for BW. */

    bool lcRequestStatus;        /**< Request status for LC. */
    bool ldRequestStatus;        /**< Request status for LD. */
    bool bwRequestStatus;        /**< Request status for BW. */
    unsigned int reserved1;      /**< Reserved field for future parameters. */
} ObsParams;

/**
 * @brief Control Variables Structure
 * @details This structure manages cooling requests, fan gear settings, and control
 *          durations for different modules, along with compressor speed settings.
 */
typedef struct {
    bool lcCoolRequest;            /**< Cooling request for LC. */
    bool ldCoolRequest;            /**< Cooling request for LD. */
    bool bwCoolRequest;            /**< Cooling request for BW. */

    unsigned char lcFanSetGear;    /**< Fan gear setting for LC. */
    unsigned char ldFanSetGear;    /**< Fan gear setting for LD. */
    unsigned char bwFanSetGear;    /**< Fan gear setting for BW. */
    unsigned char lnFanSetGear;    /**< Fan gear setting for LN. */
    unsigned char reserved1;       /**< Reserved field for future use. */
    unsigned int compSetSpeed;     /**< Compressor set speed. */
    unsigned int controlDuration;  /**< Duration of the control action. */
} ObsControl;


/**
 * @brief Temperature Target.
 */
typedef struct {
    double ldTemp;  /**< Temperature target for LD. */
    double lcTemp;  /**< Temperature target for LC. */
    double bwTemp;  /**< Temperature target for BW. */
} TempTarget;

/**
 * @brief Weight Values.
 */
typedef struct {
    double ldWeight;  /**< Weight value for LD. */
    double lcWeight;  /**< Weight value for LC. */
    double bwWeight;  /**< Weight value for BW. */
} WeightValue;

extern ObsParams  g_obsParams;
extern ObsControl g_obsControl;

/* External API */
unsigned int AI_ControllerInit(ObsParams *obsParams, TempTarget *tempTarget, WeightValue *weightValue);
unsigned int AI_ControllerExec(ObsParams *obsParams, ObsControl *controlRes);
unsigned int AI_ControllerStop(void);
unsigned int AI_ControllerStart(void);
#endif

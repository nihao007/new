/**
 * @copyright Copyright (c)  HiSilicon (Shanghai) Technologies  Co., Ltd. 2025-2025. All rights reserved.
 * @file    app_entry.h
 * @author  MCU Driver Team
 * @brief   Hearder file code main entry of AI model control
 */
#ifndef APP_ENTRY_H
#define APP_ENTRY_H
#include <stdbool.h>
#include <stdint.h>
#include "app_config.h"

typedef int (*ClipCompFreqSc)(const float scaleFreq, float* denormalizedFreq,\
                              float outdoorTemperature, bool isUp, int mode);
typedef struct {
    float *obsAct;
    uint32_t obsActLen;
    float *nextObs;
    uint32_t nextObsLen;
} ConfigData;

typedef struct {
    float freqWeight;    /* net frequency weight */
    float tempWeight;    /* net temperature weight */
    float extFanWeight;  /* net external fan weight */
} WeightConfig;

typedef struct {
    uint32_t obsLen;    /* obsLen */
    float inputObs[GET_STATUS_NUM];    /* input Obs array */
    uint32_t resLen;    /* obsLen */
    float outputRes[SET_ACTION_NUM];  /* act output result array */
} InputDataConfig;

typedef struct {
    float coolMaxRange[DIM_ACT];
    float coolMinRange[DIM_ACT];
    float heatMaxRange[DIM_ACT];
    float heatMinRange[DIM_ACT];
    int32_t coolRangeSize;
    int32_t heatRangeSize;;
} ControlRange;

// output API
void EntryEnable(bool enable);   // Energy saving control
void ModelControlEntry(void);     // model control main interface
/* Build model need cool&heat initial dataset */
int ModelBuild(ConfigData *inputCoolConfig, ConfigData *inputHeatConfig);   // model build
int ConfigInit(ControlRange *inputRange);  // control action range init
int ModelLibInit(void);     // model lib init and check

// dynamic update control action range
void UpdateActionRange(float* maxRange, float* minRange, unsigned int rangeSize);
void SetDefaultMode(void);       // set default control
void SetModelCtrlMode(void);        // set model control
void SetCoolWork(int mode);      // set the cool work mode
void SetHeatWork(int mode);      // set the heat work mode

// frequency constraint curve init
void GlobalRegInit(ClipCompFreqSc comFreqScale);

void SetInferInterval(uint32_t timeMs);        // set infer interval [60000,5*60000]
void SetFlushFlashInterval(uint32_t timeMs);   // set ram data flush to flash interval [1h, 5h]
void SetTurnOffThreshold(float threshold);     // set turn off air-conditioner temperature threshold[-0.8,-0.3]
// range [0.3,0.8]
void SetTurnOnThreshold(float threshold);      // set turn on air-conditioner temperature threshold after turn-off
void SetMaxRetryTime(unsigned int times);      // set max retry time when the model-build init fail(max times 100)
/**
 * @brief Check model infer using the special obs-array
 * @details This interface is used for the detection of control board before delivery, just to judge the result of
 *          the interface.
 */
int ModelInferCheck(InputDataConfig *inputData);
/**
 * @brief Update the control model net weight
 * @details When use this interface to update the net weight, there are some comments on the weightConfig struct, when:
 * weight < 0: is invalid, return; weight = 0 :keep last; weight > 0:update to the net weight. defaultWeightConfig = {}
 */
void UpdateModelWeight(WeightConfig *netWeight);
void EnableModelDataCollection(bool enable); // Start Obtain Initial Model Data
/* Prints the initial model data and displays the model data collection completion. */
void ModelDataCollectionState(void);

#endif
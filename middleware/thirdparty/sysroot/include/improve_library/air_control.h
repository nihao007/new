/**
 * @copyright Copyright (c)  HiSilicon (Shanghai) Technologies  Co., Ltd. 2025-2025. All rights reserved.
 * @file    air_control.h
 * @author  MCU Driver Team
 * @brief   header file of control air-condition interface
 *
 */
#ifndef AIR_CONTROL_H
#define AIR_CONTROL_H

typedef struct {
    int compressorFreq;
    int externalFanSpeed;
    int expansionValve;
}AirActionT;

int AirTransactionSetAction(AirActionT *action, unsigned int actionLen);
int AirTransactionObsGet(float *obs, unsigned int obsLen);
int AirTransactionSetActionWithError(AirActionT *action, unsigned int actionLen, int errorCode);

#endif /* #ifndef AIR_CONTROL_H */
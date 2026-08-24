#ifndef MOTOR_STEPPER_H
#define MOTOR_STEPPER_H

#include "gpio.h"
#include "adc.h" /* SDK: ADC_Handle */
#include "apt.h" /* SDK: APT_Handle */
#include <stdint.h>

typedef struct {
    /* — IO 句柄 — */
    APT_Handle *apt;    /* 硬件脉冲发生器 (APT0, 唯一模式) */
    GPIO_Handle *dirGpio;
    uint16_t dirPin;
    GPIO_Handle *enGpio;
    uint16_t enPin;
    GPIO_Handle *sleepGpio;
    uint16_t sleepPin;
    GPIO_Handle *resetGpio;
    uint16_t resetPin;
    GPIO_Handle *ms1Gpio;
    uint16_t ms1Pin;
    GPIO_Handle *ms2Gpio;
    uint16_t ms2Pin;
    GPIO_Handle *ms3Gpio;
    uint16_t ms3Pin;
    GPIO_Handle *refGpio;
    uint16_t refPin; /* 可选, 不用则设 NULL */

    /* — ADC 反馈标定 — */
    ADC_Handle *adc;
    uint16_t adc0deg;   /* 0° 时 ADC 读数 */
    uint16_t adc180deg; /* 180° 时 ADC 读数 */
    float degPerStep;   /* 每步对应角度 */

    uint8_t id;
} Motor_Stepper;

void Stepper_Run(Motor_Stepper *m, unsigned int steps, unsigned int dir);

void Stepper_Stop(Motor_Stepper *m);

/* Non-blocking status updated by the APT pulse callback. */
unsigned int Stepper_IsBusy(void);
unsigned int Stepper_GetDoneSeq(void);
unsigned int Stepper_GetLastCompletedSteps(void);

/* 高级接口: 跑到目标角度 (内部读 ADC → 算步数 → 调 Stepper_Run) */
void Stepper_RunToAngle(Motor_Stepper *m, float targetDeg);

#endif

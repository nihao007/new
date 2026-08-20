#ifndef MOTOR_BLDC_H
#define MOTOR_BLDC_H

#include "gpio.h"
#include "gpt.h"
#include "adc.h"
#include "../pid/pid.h"
#include <stdint.h>
#include <stdbool.h>

#define FG_REV 6 /* FG 脉冲数/圈 */
#define DUTY_MAX 9500.0f
#define STARTUP_DUTY 5000.0f
#define STARTUP_INTEGRAL 1000.0f

/* ========== 子结构体 ========== */

typedef struct {
    uint32_t last, prev;
    float rpm;
    uint8_t initialized, updated, firstEdge, rejectCnt;
    float fc;         /* 低通截止频率 Hz */
    uint32_t timerHz; /* 时基频率 */
} BldcFb_State;

typedef struct {
    PIDCtrl pid;        /* PI 控制器 (kp/ki/integral) */
    float gravBase;     /* 重力补偿基数 */
    float startupDuty;  /* 开环起步 duty (0=用宏 STARTUP_DUTY; 重负载电机可单独加大) */
    float rampRate;     /* actRpm 斜坡率 (RPM/s) */
    float cmdRate;      /* cmdRpm 斜坡率 (RPM/s), 用于减速平滑 */
    float holdIntegral; /* 启动预置积分 (duty≈holdIntegral/9500) */
    float cmdTarget;    /* 位置环原始目标 RPM */
    float cmdRpm;       /* 斜坡滤波后目标 RPM */
    float actRpm;       /* 斜坡处理后实际 RPM */
    uint16_t duty;      /* 当前占空比输出 */
    uint8_t dir;        /* 当前方向 */
    uint8_t enabled;
} BldcSpd_State;

typedef struct {
    PIDCtrl pid;     /* P 控制器 (只用 kp) */
    float deadband;  /* 位置死区 (°) */
    float holdKp;    /* 死区内小 P 增益 (RPM/°) */
    float maxSpeed;  /* 最大输出速度 (RPM) */
    float decelRate; /* 减速度 (RPM/s), 用于速度前瞻 */
    float target;    /* 目标角度 (°) */
    uint8_t enabled;
    /* 前馈控制 */
    float ffUp;      /* 上坡(往90°走)前馈 duty */
    float ffDown;    /* 下坡(离90°走)刹车 duty */
} BldcPos_State;

/* ========== BLDC 电机主结构体 ========== */

typedef struct {
    /* — IO — */
    GPT_Handle *pwm;
    GPIO_Handle *frGpio;
    uint16_t frPin;
    GPIO_Handle *bkGpio;
    uint16_t bkPin;

    /* — ADC — */
    ADC_Handle *adc;
    uint16_t adc0deg;   /* 0° 时 ADC 读数 */
    uint16_t adc180deg; /* 180° 时 ADC 读数 */
    float degRange;     /* ADC 全程对应角度 (默认180) */

    /* — 子模块 — */
    BldcFb_State fb;
    BldcSpd_State spd;
    BldcPos_State pos;

    uint8_t id;
} Motor_Bldc;

/* ========== API ========== */

/* 硬件操作 */
void Bldc_SetOutput(Motor_Bldc *m, uint16_t duty, uint8_t dir);
void Bldc_Stop(Motor_Bldc *m);
void Bldc_FaultClear(Motor_Bldc *m);

/* FG 测速 */
void BldcFb_Isr(Motor_Bldc *m, uint32_t ticks);
void BldcFb_Update(Motor_Bldc *m);
float BldcFb_GetRpm(Motor_Bldc *m);

/* 速度 PI */
void BldcSpd_SetTarget(Motor_Bldc *m, float rpm);
void BldcSpd_Run(Motor_Bldc *m, float ts, float thetaRad);

/* 位置环 P */
void BldcPos_SetTarget(Motor_Bldc *m, float deg);
float BldcPos_Exec(Motor_Bldc *m, float curDeg, bool fgActive, bool running, uint8_t *dir);

/* 高级接口: 设目标角度 (复位速度环, 使能) */
void Bldc_SetTargetDeg(Motor_Bldc *m, float deg);

/* ADC 工具 (BLDC 角度传感器标定换算) */
float BldcAdcToDeg(Motor_Bldc *m, uint16_t adcVal);

#endif

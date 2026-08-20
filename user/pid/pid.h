#ifndef PID_H
#define PID_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float kp, ki, kd;
    float integral, prevErr;
    float outMin, outMax;
    float integralMin, integralMax;
    float dtMin; /* 最小 dt 保护 (us) */
} PIDCtrl;

/* 用于 C99 初始化: .pid = PIDCTRL_INIT(0.5, 0.2, 0, 0, 9500, -3000, 3000) */
#define PIDCTRL_INIT(kp_, ki_, kd_, omin_, omax_, imin_, imax_) \
    {.kp = (kp_),                                               \
     .ki = (ki_),                                               \
     .kd = (kd_),                                               \
     .integral = 0,                                             \
     .prevErr = 0,                                              \
     .outMin = (omin_),                                         \
     .outMax = (omax_),                                         \
     .integralMin = (imin_),                                    \
     .integralMax = (imax_),                                    \
     .dtMin = 0.0001f}

void PIDCtrl_Reset(PIDCtrl *p);
void PIDCtrl_SetOutputLimits(PIDCtrl *p, float min, float max);
void PIDCtrl_SetIntegralLimits(PIDCtrl *p, float min, float max);
void PIDCtrl_SetIntegral(PIDCtrl *p, float val);
float PIDCtrl_Exec(PIDCtrl *p, float setpoint, float feedback, float dt);

#endif

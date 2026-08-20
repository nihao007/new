#include "pid.h"
#include <float.h>

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

void PIDCtrl_Reset(PIDCtrl *p)
{
    p->integral = 0;
    p->prevErr = 0;
}

void PIDCtrl_SetOutputLimits(PIDCtrl *p, float min, float max)
{
    p->outMin = min;
    p->outMax = max;
    p->integralMin = min;
    p->integralMax = max;
}

void PIDCtrl_SetIntegralLimits(PIDCtrl *p, float min, float max)
{
    p->integralMin = min;
    p->integralMax = max;
}

void PIDCtrl_SetIntegral(PIDCtrl *p, float val)
{
    p->integral = CLAMP(val, p->integralMin, p->integralMax);
}

float PIDCtrl_Exec(PIDCtrl *p, float setpoint, float feedback, float dt)
{
    float err, pOut, iOut, dOut, out;

    if (dt < p->dtMin)
        dt = p->dtMin;

    err = setpoint - feedback;

    /* 比例 */
    pOut = p->kp * err;

    /* 积分 — 带抗饱和钳位 */
    p->integral += p->ki * dt * err;
    p->integral = CLAMP(p->integral, p->integralMin, p->integralMax);
    iOut = p->integral;

    /* 微分 */
    if (p->kd != 0.0f) {
        dOut = p->kd * (err - p->prevErr) / dt;
    } else {
        dOut = 0.0f;
    }
    p->prevErr = err;

    out = pOut + iOut + dOut;
    out = CLAMP(out, p->outMin, p->outMax);

    return out;
}

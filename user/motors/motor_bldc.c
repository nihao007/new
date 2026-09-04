#include "motor_bldc.h"
#include <math.h>

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))
#define REJECT_THRESH 1500.0f
#define RPM_MAX 5000.0f
#define RPM_MIN 10.0f
#define INTEGRAL_LIMIT 3000.0f

/* BLDC 角度传感器: 标定值来自 Motor_Bldc.adc0deg / .adc180deg */

/* ================================================================
 *  硬件操作
 * ================================================================ */

void Bldc_SetOutput(Motor_Bldc *m, uint16_t duty, uint8_t dir)
{
    GPT_ReferCfg ref;

    if (m == NULL || m->pwm == NULL)
        return;
    /* 方向 */
    if (m->frGpio)
        HAL_GPIO_SetValue(m->frGpio, m->frPin, dir ? GPIO_HIGH_LEVEL : GPIO_LOW_LEVEL);
    /* 刹车: duty>0 → 释放(HIGH), duty=0 → 锁止(LOW) */
    if (m->bkGpio)
        HAL_GPIO_SetValue(m->bkGpio, m->bkPin,
            duty > 0 ? GPIO_HIGH_LEVEL : GPIO_LOW_LEVEL);
    if (duty > DUTY_MAX)
        duty = DUTY_MAX;

    /* PWM 占空比 */
    if (duty == 0) {
        ref.refA0.refdot = 0;
        ref.refA0.refAction = GPT_ACTION_OUTPUT_HIGH;
        ref.refB0.refdot = 0;
        ref.refB0.refAction = GPT_ACTION_NO_ACTION;
    } else if (duty >= 9999) {
        ref.refA0.refdot = 0;
        ref.refA0.refAction = GPT_ACTION_OUTPUT_LOW;
        ref.refB0.refdot = 0;
        ref.refB0.refAction = GPT_ACTION_NO_ACTION;
    } else {
        ref.refA0.refdot = 0;
        ref.refA0.refAction = GPT_ACTION_OUTPUT_LOW;
        ref.refB0.refdot = duty;
        ref.refB0.refAction = GPT_ACTION_OUTPUT_HIGH;
    }
    HAL_GPT_SetReferCounterAndAction(m->pwm, &ref);
}

void Bldc_Stop(Motor_Bldc *m)
{
    if (m == NULL)
        return;

    /* PWM 归零 */
    GPT_ReferCfg ref = {.refA0 = {.refdot = 0, .refAction = GPT_ACTION_OUTPUT_HIGH},
                        .refB0 = {.refdot = 0, .refAction = GPT_ACTION_NO_ACTION}};
    if (m->pwm)
        HAL_GPT_SetReferCounterAndAction(m->pwm, &ref);

    /* BRAKE LOW → 驱动 IC 停止输出 */
    if (m->bkGpio)
        HAL_GPIO_SetValue(m->bkGpio, m->bkPin, GPIO_LOW_LEVEL);

    /* 状态复位 */
    m->spd.enabled = false;
    m->spd.duty = 0;
    PIDCtrl_Reset(&m->spd.pid);
}

void Bldc_FaultClear(Motor_Bldc *m)
{
    /* FR 反转 → 清除驱动 IC 堵转保护锁死 */
    if (m->frGpio == NULL)
        return;

    HAL_GPIO_SetValue(m->frGpio, m->frPin, GPIO_HIGH_LEVEL);
    BASE_FUNC_DELAY_MS(10);
    HAL_GPIO_SetValue(m->frGpio, m->frPin, GPIO_LOW_LEVEL);
}

/* ================================================================
 *  FG 测速 (T 法)
 * ================================================================ */

void BldcFb_Isr(Motor_Bldc *m, uint32_t ticks)
{
    BldcFb_State *f = &m->fb;

    if (f->firstEdge) {
        f->prev = ticks;
        f->last = ticks;
        f->firstEdge = false;
        return;
    }
    f->prev = f->last;
    f->last = ticks;
    f->updated = true;
}

void BldcFb_Update(Motor_Bldc *m)
{
    BldcFb_State *f = &m->fb;
    uint32_t dt;
    float dtSec, raw, jump, alpha;

    if (!f->updated)
        return;
    f->updated = false;

    dt = f->last - f->prev;
    if (dt == 0)
        return;

    dtSec = dt / (float)f->timerHz;
    raw = 60.0f / (FG_REV * dtSec);
    raw = CLAMP(raw, RPM_MIN, RPM_MAX);

    /* 首样本 */
    if (!f->initialized) {
        f->rpm = raw;
        f->initialized = true;
        return;
    }

    /* 野值剔除 */
    jump = raw - f->rpm;
    if (fabsf(jump) > REJECT_THRESH) {
        f->rejectCnt++;
        if (f->rejectCnt >= 3) {
            f->rpm = raw;
            f->rejectCnt = 0;
        }
        return;
    }
    f->rejectCnt = 0;

    /* EMA 低通 */
    alpha = dtSec / (dtSec + 1.0f / (2.0f * 3.14159f * f->fc));
    f->rpm += alpha * jump;
}

float BldcFb_GetRpm(Motor_Bldc *m)
{
    return m->fb.rpm;
}

/* ================================================================
 *  速度 PI
 * ================================================================ */

void BldcSpd_SetTarget(Motor_Bldc *m, float rpm)
{
    /* cmdRpm 大幅下降 → 积分等比下降, 配合提前减速 */
    if (rpm < m->spd.cmdTarget * 0.8f && rpm > 1.0f) {
        float ratio = rpm / m->spd.cmdTarget;
        m->spd.pid.integral *= ratio;
    }
    m->spd.cmdTarget = rpm;
}

void BldcSpd_Run(Motor_Bldc *m, float ts, float thetaRad)
{
    BldcSpd_State *s = &m->spd;
    float grav, err, out,angle;

    if (!s->enabled) {
        s->duty = 0;
        return;
    }

     //修改：去掉重力补偿
    // grav = s->gravBase * cosf(thetaRad);
    if(thetaRad>=90) angle=180.0-thetaRad;
    else angle=thetaRad;
    grav = s->gravBase * cosf(angle);

    /* — cmdRpm 斜坡滤波: 限制位置环指令跳变 — */
    if (s->cmdRpm < s->cmdTarget - 0.5f)
        s->cmdRpm = fminf(s->cmdRpm + s->cmdRate * ts, s->cmdTarget);
    else if (s->cmdRpm > s->cmdTarget + 0.5f)
        s->cmdRpm = fmaxf(s->cmdRpm - s->cmdRate * ts, s->cmdTarget);
    else
        s->cmdRpm = s->cmdTarget;

    /* — Hold 模式: 目标≈0 — */
     if (fabsf(s->cmdRpm) < 1.0f) {
        //修改：去掉重力补偿
        // if (s->pid.integral < grav)
        //     s->pid.integral = grav;      /* 积分锁底=重力 — 保持力=重力补偿 */
        out = s->pid.integral;
        s->duty = (uint16_t)CLAMP(out, 0, DUTY_MAX);
        return;
    }

    /* — 启动: FB 未就绪时开环起步 (per-motor startupDuty, 0=用宏) — */
    if (!m->fb.initialized) {
        s->duty = (s->startupDuty > 0.0f) ? (uint16_t)s->startupDuty : STARTUP_DUTY;
        s->dir = (s->cmdRpm >= 0) ? 0 : 1;
        return;
    }

    /* — actRpm 速度斜坡 — */
    if (s->actRpm < s->cmdRpm - 0.5f)
        s->actRpm = fminf(s->actRpm + s->rampRate * ts, s->cmdRpm);
    else if (s->actRpm > s->cmdRpm + 0.5f)
        s->actRpm = fmaxf(s->actRpm - s->rampRate * ts, s->cmdRpm);
    else
        s->actRpm = s->cmdRpm;

    /* — 下坡超速反向制动: 重力加速 (cmd 与 to90 异号) 且转速超目标 → FR 翻转主动刹车.
     * 下降时 grav 应托举 (增角) 防掉 — userPID完成 原样无此机制, PI 用 |actRpm|
     * 刹不住 → 下降抖动 (速度环与重力前馈打架) — */
    // if (s->cmdRpm * (1.5707963f - thetaRad) < 0.0f
    //     && BldcFb_GetRpm(m) > fabsf(s->actRpm) + 300.0f) {   /* 阈值 300: 只在明显超速时制动, 防低速爬升误触发 */
    //     out = (BldcFb_GetRpm(m) - fabsf(s->actRpm)) * 1.0f;
    //     if (out > DUTY_MAX) out = DUTY_MAX;
    //     s->dir = (s->cmdRpm >= 0) ? 1 : 0;   /* 反向 (main.c 取反: 下降→增角托举制动) */
    //     s->duty = (uint16_t)CLAMP(out, 0, DUTY_MAX);
    //     return;
    // }

    /* — PI 控制: 重力作为积分基底 (积分锁底=grav, 量级一致; 1 平衡点 cos 外推标定) — */
    err = fabsf(s->actRpm) - BldcFb_GetRpm(m);
    if (err < -150.0f) err = -150.0f;   /* 超速泄压限幅: 防 kp·err 大负抵消重力/翻转方向 (转一下停一下振荡) */
    float piOut = PIDCtrl_Exec(&s->pid, err, 0, ts);   /* 速度误差积分 + 限幅 */
    //修改：去掉重力补偿
    // if (s->pid.integral < grav)
    //     s->pid.integral = grav;      /* exec 后锁底=重力 (托力基底, 爬升够力) */
    out = s->pid.kp * err + s->pid.integral;
    
    /* 去掉输出托底: 速度环减速 (err 负 → kp·err 负 → out < grav) 不被重力托底
     * 挡住 — 否则到位减速失效 → 来回冲 (速度环与重力前馈打架) */
    s->dir = (s->cmdRpm >= 0) ? 0 : 1;
    // if(!s->dir) {
    //     if(thetaRad<=90.0)   
    //     {
    //         out=out+grav;  
    //         // DBG_PRINTF(" \r\n 1111111 out=out+grav\r\n");
    //     }
    //     else  
    //     {
    //         out=out-grav;
    //         // DBG_PRINTF(" \r\n 222222 out=out-grav\r\n");
    //     } 
    // }
    // else {
    //     if(thetaRad<=90.0) 
    //     {
    //         // DBG_PRINTF(" \r\n 33333 out=out-grav\r\n");
    //         out=out-grav;
    //     } 
    //     else 
    //     {
    //         out=out+grav; 
    //         // DBG_PRINTF(" \r\n 44444 out=out+grav\r\n");
    //     }
        
    // }
    if (s->dir == 0) {
    // 上升：增加出力
    out = out + s->gravBase;
    } else {
        // 下降：减少出力
        out = out - s->gravBase;
    }
    // if (out < 900) out = 900;   /* 运动输出 ≥ 0 (方向由 cmdRpm 定, 减速靠 out 减小) */

    s->duty = (uint16_t)CLAMP(out, 0, DUTY_MAX);
    // s->duty = 900;
}

/* ================================================================
 *  位置环 P
 * ================================================================ */

void BldcPos_SetTarget(Motor_Bldc *m, float deg)
{
    if (deg < 0)
        deg = 0;
    if (deg > 180)
        deg = 180;
    m->pos.target = deg;
    m->pos.enabled = true;
}

float BldcPos_Exec(Motor_Bldc *m, float curDeg, bool fgActive, bool running, uint8_t *dir)
{
    BldcPos_State *p = &m->pos;
    float pe, spd;

    if (!p->enabled) {
        *dir = 0;
        return 0;
    }

    pe = p->target - curDeg;

    /* 在死区内 → 小 P 保持, 不推飞也不掉落 */
    if (fabsf(pe) < p->deadband) {
        spd = pe * p->holdKp;
        *dir = (spd >= 0) ? 0 : 1;
        return spd;
    }

    /* 速度前瞻: 限制P输出不超过剩余角度能刹停的最大速度 */
    float maxFeedrate = sqrtf(fabsf(pe) * p->decelRate / 3.0f);

    spd = p->pid.kp * pe;
    spd = CLAMP(spd, -maxFeedrate, maxFeedrate);
    *dir = (spd >= 0) ? 0 : 1;
    return spd;
}

/* ================================================================
 *  高级接口
 * ================================================================ */

void Bldc_SetTargetDeg(Motor_Bldc *m, float deg)
{
    BldcPos_SetTarget(m, deg);
    m->pos.enabled = true;
    /* 新目标: 放宽积分限幅让holdIntegral不被钳位, 位置环40ms后自动收窄 */
    float maxLimit = m->spd.holdIntegral + 1000.0f;
    PIDCtrl_SetIntegralLimits(&m->spd.pid, -maxLimit, maxLimit);
    PIDCtrl_SetIntegral(&m->spd.pid, m->spd.holdIntegral);
    m->spd.actRpm = 0;
    m->spd.enabled = true;
}

/* ================================================================
 *  ADC 工具 (BLDC 角度传感器分段线性)
 * ================================================================ */

float BldcAdcToDeg(Motor_Bldc *m, uint16_t adcVal)
{
    float range = (float)((int)m->adc180deg - (int)m->adc0deg);
    if (range == 0.0f) return 0.0f; /* 防除零 */
    float span = (m->degRange > 0.0f) ? m->degRange : 180.0f;
    return (float)((int)adcVal - (int)m->adc0deg) * span / range;
}

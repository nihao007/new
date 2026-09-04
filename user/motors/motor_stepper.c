#include "motor_stepper.h"
#include "../adc/adc_user.h"
#include "debug.h"
#include <math.h>

/* ========== APT 硬件脉冲 (非阻塞): 步进计数状态 ========== */
static volatile unsigned int g_stepTarget = 0;
static volatile unsigned int g_stepCount = 0;
static volatile unsigned char g_stepBusy = 0;
static volatile unsigned int g_stepDoneSeq = 0;
static volatile unsigned int g_stepLastCompletedSteps = 0;

#define STEPPER_APT_COUNTER_HZ 1000000.0f
#define STEPPER_DEFAULT_SPEED_DPS 20U
#define STEPPER_MIN_SPEED_DPS 5U
#define STEPPER_MAX_SPEED_DPS 60U

/* APT0 周期归零中断回调 (每脉冲计一次, 数满自动停 APT) */
void APT0_StepperCallback(void *param)
{
    (void)param;
    if (g_stepBusy) {
        g_stepCount++;
        if (g_stepCount >= g_stepTarget) {
            HAL_APT_StopModule(RUN_APT0);
            g_stepLastCompletedSteps = g_stepCount;
            g_stepBusy = 0;
            g_stepDoneSeq++;
        }
    }
}

uint8_t Stepper_SetSpeedDps(Motor_Stepper *m, uint8_t speedDps)
{
    unsigned int period;
    unsigned short compare;

    if (m == NULL || m->apt == NULL || m->degPerStep <= 0.0f)
        return 0U;

    if (speedDps == 0U)
        speedDps = STEPPER_DEFAULT_SPEED_DPS;
    if (speedDps < STEPPER_MIN_SPEED_DPS)
        speedDps = STEPPER_MIN_SPEED_DPS;
    if (speedDps > STEPPER_MAX_SPEED_DPS)
        speedDps = STEPPER_MAX_SPEED_DPS;

    /* UP_DOWN mode: pulse frequency = 1MHz / (2 * period). */
    period = (unsigned int)(STEPPER_APT_COUNTER_HZ * m->degPerStep /
                            (2.0f * (float)speedDps) + 0.5f);
    if (period < 2U)
        period = 2U;
    if (period > 65535U)
        period = 65535U;

    compare = (unsigned short)(period / 2U);
    (void)HAL_APT_SetTimerPeriod(m->apt, (unsigned short)period,
                                APT_BUFFER_INDEPENDENT_LOAD,
                                APT_PERIOD_LOAD_EVENT_ZERO);
    (void)HAL_APT_SetPWMDuty(m->apt, compare, compare);
    return speedDps;
}

void Stepper_Run(Motor_Stepper *m, unsigned int steps, unsigned int dir)
{
    /* 防御: 本板无步进硬件 (enGpio 未初始化) 时直接返回, 不崩 (非步进板急停也会调本函数) */
    if (m == NULL || m->enGpio == NULL || steps == 0U)
        return;

    /* 唤醒 */
    HAL_GPIO_SetValue(m->sleepGpio, m->sleepPin, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetValue(m->resetGpio, m->resetPin, GPIO_HIGH_LEVEL);

    /* 整步模式 */
    HAL_GPIO_SetValue(m->ms3Gpio, m->ms3Pin, GPIO_LOW_LEVEL);
    HAL_GPIO_SetValue(m->ms2Gpio, m->ms2Pin, GPIO_LOW_LEVEL);
    HAL_GPIO_SetValue(m->ms1Gpio, m->ms1Pin, GPIO_LOW_LEVEL);

    /* 可选 REF 引脚 */
    if (m->refGpio)
        HAL_GPIO_SetValue(m->refGpio, m->refPin, GPIO_HIGH_LEVEL);

    /* 使能 + 方向 */
    HAL_GPIO_SetValue(m->enGpio, m->enPin, GPIO_LOW_LEVEL);
    HAL_GPIO_SetValue(m->dirGpio, m->dirPin, dir ? GPIO_HIGH_LEVEL : GPIO_LOW_LEVEL);

    /* ===== APT 硬件脉冲 (非阻塞, 完成由 APT0_StepperCallback 自动停) ===== */
    g_stepTarget = steps;
    g_stepCount = 0;
    g_stepBusy = 1;
    HAL_APT_StartModule(RUN_APT0);
}

void Stepper_Stop(Motor_Stepper *m)
{
    if (m == NULL || m->enGpio == NULL)
        return;
    g_stepBusy = 0;
    HAL_APT_StopModule(RUN_APT0);
    g_stepTarget = 0;
    g_stepCount = 0;
    HAL_GPIO_SetValue(m->enGpio, m->enPin, GPIO_HIGH_LEVEL);      /* 关断 */
    HAL_GPIO_SetValue(m->sleepGpio, m->sleepPin, GPIO_LOW_LEVEL); /* 休眠 */
}

unsigned int Stepper_IsBusy(void)
{
    return (unsigned int)g_stepBusy;
}

unsigned int Stepper_GetDoneSeq(void)
{
    return g_stepDoneSeq;
}

unsigned int Stepper_GetLastCompletedSteps(void)
{
    return g_stepLastCompletedSteps;
}

void Stepper_RunToAngle(Motor_Stepper *m, float targetDeg)
{
    uint16_t val;
    float cur, d;
    unsigned int steps;

    ADC_Read(m->adc, &val);

    cur = (float)((int)val - (int)m->adc0deg) * 180.0f / (float)((int)m->adc180deg - (int)m->adc0deg);

    d = fabsf(targetDeg - cur);
    steps = (unsigned int)(d / m->degPerStep + 0.5f);
    if (steps) {
        Stepper_Run(m, steps, targetDeg > cur ? 1 : 0);
    }
    DBG_PRINTF("step adc=%u cur=%.1f target=%.1f diff=%.1f steps=%u\r\n",
           (unsigned int)val,
           cur,
           targetDeg,
           d,
           (unsigned int)steps);
}

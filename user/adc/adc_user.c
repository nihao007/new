#include "adc_user.h"
#include "main.h" /* g_adc 句柄 */

void ADC_Read(ADC_Handle *adc, uint16_t *val)
{
    ADC_ReadSoc(adc, ADC_SOC_NUM0, val);
}

void ADC_ReadSoc(ADC_Handle *adc, uint32_t soc, uint16_t *val)
{
    uint32_t sum = 0;
    for (int i = 0; i < 4; i++) {
        HAL_ADC_SoftTrigSample(adc, soc);
        while (HAL_ADC_CheckSocFinish(adc, soc) == 0) ;
        sum += HAL_ADC_GetConvResult(adc, soc);
    }
    *val = (uint16_t)(sum >> 2);
}

/* ================================================================
 * 总线标定参数: 4 套, 按 ID 匹配 — 偏移基准 (全程 0~180, 无负数)
 * 标定流程: 手摆 0° 记录 ADC → 手摆 180° 记录 ADC → 手摆顺齐读 deg 得偏移
 * ================================================================ */
static const Bus_Calib g_busCalib[4] = {
    [BUS_MOTOR_JOINT1] = { 1420, 480, 119 },  /* ID00: 0°=1420, 180°=480, 顺齐偏移=119° (实测推导) */
    [BUS_MOTOR_JOINT2] = { 0, 4095, 90 },     /* TODO: 按偏移基准实测 */
    [BUS_MOTOR_JOINT3] = { 1000, 500, 90 },   /* TODO: 按偏移基准实测 */
    [BUS_MOTOR_JOINT4] = { 1000, 500, 90 },   /* TODO: 按偏移基准实测 */
};

void ADC_GetCalib(uint8_t id, uint16_t *adc0deg, uint16_t *adc180deg, uint8_t *alignedDeg)
{
    const Bus_Calib *c = &g_busCalib[id & 3];
    *adc0deg = c->adc0deg;
    *adc180deg = c->adc180deg;
    *alignedDeg = c->alignedDeg;
}

/* ADC → 局部角 (0~180 空间, 反向电位器自动处理) */
static float AdcToDeg(const Bus_Calib *c, uint16_t adcVal)
{
    float range = (float)((int)c->adc180deg - (int)c->adc0deg);
    if (range == 0.0f)
        return 0.0f;   /* 防除零 */
    return (float)((int)adcVal - (int)c->adc0deg) * 180.0f / range;
}

/* 按 ID 读本板电位器 → 局部角 (4 块板同一 IO, 统一走唯一 ADC) */
float ADC_ReadAngle(uint8_t id)
{
    uint16_t val;
    ADC_Read(&g_adc2, &val);
    return AdcToDeg(&g_busCalib[id & 3], val);
}

/* 世界角 = 父臂角 + 局部角 − 顺齐偏移 (重力模型/读应答用) */
float ADC_WorldAngle(uint8_t id, float parentWorld)
{
    uint16_t a0, a180;
    uint8_t aligned;
    ADC_GetCalib(id, &a0, &a180, &aligned);
    return parentWorld + ADC_ReadAngle(id) - (float)aligned;
}

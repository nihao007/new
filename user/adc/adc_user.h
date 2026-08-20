#ifndef ADC_USER_H
#define ADC_USER_H

#include "main.h" /* SDK: ADC_Handle, HAL_ADC API */
#include <stdint.h>

void ADC_Read(ADC_Handle *adc, uint16_t *val);

/* 指定 SOC 通道采样 (4次平均): 双BLDC版 SOC0=A2(电机3), SOC1=A3(电机2) */
void ADC_ReadSoc(ADC_Handle *adc, uint32_t soc, uint16_t *val);

/* 总线电机 ID (跳线 00/01/10/11 决定, 与总线帧 ID 字节一致) */
typedef enum {
    BUS_MOTOR_JOINT1 = 0,  /* ID00: 无刷关节1 */
    BUS_MOTOR_JOINT2 = 1,  /* ID01: 无刷关节2 */
    BUS_MOTOR_JOINT3 = 2,  /* ID02: 无刷关节3 */
    BUS_MOTOR_JOINT4 = 3,  /* ID03: 无刷关节4 */
} Bus_MotorId;

/* 标定参数 (手摆实测), 按总线 ID 匹配 — 偏移基准 (全程 0~180, 无负数):
 *   adc0deg    = 0° 位置 ADC (电位器一端)
 *   adc180deg  = 180° 位置 ADC (另一端)
 *   alignedDeg = 臂与父臂顺齐时的角度 (偏移量, 0~180 空间)
 * 反向电位器自动处理 (adc180deg < adc0deg 即可) */
typedef struct {
    uint16_t adc0deg;
    uint16_t adc180deg;
    uint8_t alignedDeg;   /* 顺齐偏移 (0~180 空间) */
} Bus_Calib;

/* 取指定 ID 的标定参数 */
void ADC_GetCalib(uint8_t id, uint16_t *adc0deg, uint16_t *adc180deg, uint8_t *alignedDeg);

/* 按 ID 读本板电位器 → 局部角 (0~180°, 偏移基准)
 * 唯一 ADC (硬件 ADC2, GPIO6_0), 4 块板同一 IO, 标定按 ID 匹配 */
float ADC_ReadAngle(uint8_t id);

/* 世界角 = 父臂角 + 局部角 − 顺齐偏移 (重力模型/读应答用) */
float ADC_WorldAngle(uint8_t id, float parentWorld);

#endif

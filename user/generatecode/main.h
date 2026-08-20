#ifndef McuMagicTag_SYSTEM_INIT_H
#define McuMagicTag_SYSTEM_INIT_H

#include "uart.h"
#include "uart_ex.h"
#include "gpt.h"
#include "gpt_ex.h"
#include "gpio.h"
#include "gpio_ex.h"
#include "timer.h"
#include "timer_ex.h"
#include "crg.h"
#include "iocmg.h"
#include "iocmg_ex.h"
#include "adc.h"
#include "apt.h"
#include <stdint.h>

#define IO_SPEED_FAST 0x00U
#define IO_SPEED_SLOW 0x01U
#define IO_DRV_LEVEL4 0x00U
#define IO_DRV_LEVEL3 0x01U
#define IO_DRV_LEVEL2 0x02U
#define IO_DRV_LEVEL1 0x03U
#define XTAL_DRV_LEVEL4 0x03U
#define XTAL_DRV_LEVEL3 0x02U
#define XTAL_DRV_LEVEL2 0x01U
#define XTAL_DRV_LEVEL1 0x00U

extern GPT_Handle g_gptHandle0;
extern GPT_Handle g_gptHandle1;
extern UART_Handle g_uart1;

extern GPIO_Handle g_gpio2_2;
extern GPIO_Handle g_gpio4_6;
extern GPIO_Handle g_gpio4_7;
extern GPIO_Handle g_gpio5_3;
extern GPIO_Handle g_gpio5_4;
extern GPIO_Handle g_gpio6_2;
extern GPIO_Handle g_gpio6_7;
/* 步进电机1 IO */
extern GPIO_Handle g_gpio7_5;
extern GPIO_Handle g_gpio7_6;
extern GPIO_Handle g_gpio6_5;
extern GPIO_Handle g_gpio4_0;
extern GPIO_Handle g_gpio4_1;
extern GPIO_Handle g_gpio5_6;
extern GPIO_Handle g_gpio5_7;
extern GPIO_Handle g_gpio3_5;

extern TIMER_Handle g_timerCtrl;
extern TIMER_Handle g_timerFg;

extern ADC_Handle g_adc0;   /* 电机1 (步进) 电位器: ADC0 A5 / GPIO3_2 */
extern ADC_Handle g_adc1;   /* 诊断: ADC1 A2 / GPIO2_0 */
extern ADC_Handle g_adc2;   /* 电机3 角度: ADC2 A2 / GPIO6_0 */
extern ADC_Handle g_adc3;   /* 电机2/4 电位器: ADC3 A15 / GPIO16_5 — 实际接线 */

extern APT_Handle g_aptStepper;   /* 电机1 步进: APT0 硬件脉冲 (定义在 main.c) */

BASE_StatusType CRG_Config(CRG_CoreClkSelect *coreClkSelect);
void SystemInit(void);

extern void GPIO5_3_IrqCallback(void *param);
extern void GPIO6_2_IrqCallback(void *param);
extern void APT0_StepperCallback(void *param);   /* APT0 步进计步中断回调 (motor_stepper.c) */
extern void SpeedLoop_ISR(void *handle);

/* UART1 callbacks */
extern void UART1_ReadItFinishCallBack(void *handle);
extern void UART1_TransmitItErrorCallBack(void *handle);

#endif

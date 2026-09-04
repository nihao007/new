/**
 * @brief     System init — 单板双BLDC关节伺服: 电机2(肩, 新增) + 电机3(肘) + 步进(保留)
 * 电机2: GPT0/GPIO7_0 PWM, GPIO2_2 FR, GPIO4_6 BK, GPIO5_3 FG(IRQ_GPIO5), ADC3_A15/GPIO16_5
 * 电机3: GPT1/GPIO7_1 PWM, GPIO4_7 FR, GPIO6_7 BK, GPIO6_2 FG(IRQ_GPIO6), ADC2_A2/GPIO6_0
 * 控制时基: TIMER1 = 5ms (ISR只置标志), 速度环/位置环(20ms=4×5ms)均在主循环执行
 */

#include "main.h"
#include "ioconfig.h"
#include "iocmg_ip.h"
#include "iocmg_ex.h"
#include "interrupt.h"
#include "timer.h"
#include "timer_ex.h"

BASE_StatusType CRG_Config(CRG_CoreClkSelect *coreClkSelect)
{
    CRG_Handle crg;
    crg.baseAddress = CRG;
    crg.pllRefClkSelect = CRG_PLL_REF_CLK_SELECT_HOSC;
    crg.pllFbDiv = 48;
    crg.pllPreDiv = CRG_PLL_PREDIV_3;
    crg.pllPostDiv = CRG_PLL_POSTDIV_2;
    crg.coreClkSelect = CRG_CORE_CLK_SELECT_PLL;
    crg.handleEx.pllPostDiv2 = CRG_PLL_POSTDIV2_4;
    crg.handleEx.pll1PreDiv = CRG_PLL_PREDIV_3;
    crg.handleEx.pll1FbDiv = 48;
    crg.handleEx.pll1PostDiv = CRG_PLL_POSTDIV_4;
    crg.handleEx.pll1PostDiv2 = CRG_PLL_POSTDIV2_4;
    crg.handleEx.anaClkSel = CRG_ANA_SEL_PLL0;
    crg.handleEx.clk1MSelect = CRG_1M_CLK_SELECT_HOSC;
    crg.handleEx.clk1MDiv = (25 - 1);
    if (HAL_CRG_Init(&crg) != BASE_STATUS_OK)
        return BASE_STATUS_ERROR;
    *coreClkSelect = crg.coreClkSelect;
    return BASE_STATUS_OK;
}

static void GPIO_Init(void)
{
    /* ========== 电机2 (肩, 新增): FR=GPIO2_2, BK=GPIO4_6, FG=GPIO5_3 ========== */
    /* 电机2 FR: GPIO2_2 (默认低) */
    HAL_CRG_IpEnableSet(GPIO2_BASE, IP_CLK_ENABLE);
    g_gpio2_2.baseAddress = GPIO2;
    g_gpio2_2.pins = GPIO_PIN_2;
    HAL_GPIO_Init(&g_gpio2_2);
    HAL_GPIO_SetDirection(&g_gpio2_2, g_gpio2_2.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio2_2, g_gpio2_2.pins, GPIO_LOW_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio2_2, g_gpio2_2.pins, GPIO_INT_TYPE_NONE);

    /* 电机2 BK: GPIO4_6 (上电默认低=抱闸; 状态机运行后按控制律释放/抱闸) */
    HAL_CRG_IpEnableSet(GPIO4_BASE, IP_CLK_ENABLE);
    g_gpio4_6.baseAddress = GPIO4;
    g_gpio4_6.pins = GPIO_PIN_6;
    HAL_GPIO_Init(&g_gpio4_6);
    HAL_GPIO_SetDirection(&g_gpio4_6, g_gpio4_6.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio4_6, g_gpio4_6.pins, GPIO_LOW_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio4_6, g_gpio4_6.pins, GPIO_INT_TYPE_NONE);

    /* 电机2 FG: GPIO5_3 (开漏上拉, 下降沿中断, 200MHz时间戳在回调里取) */
    HAL_CRG_IpEnableSet(GPIO5_BASE, IP_CLK_ENABLE);
    g_gpio5_3.baseAddress = GPIO5;
    g_gpio5_3.pins = GPIO_PIN_3;
    HAL_GPIO_Init(&g_gpio5_3);
    HAL_GPIO_SetDirection(&g_gpio5_3, g_gpio5_3.pins, GPIO_INPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio5_3, g_gpio5_3.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio5_3, g_gpio5_3.pins, GPIO_INT_TYPE_FALL_EDGE);
    HAL_GPIO_SetPinsIrqCpuIDEx(&g_gpio5_3, g_gpio5_3.pins, CHIP_CORE_0);
    HAL_GPIO_RegisterCallBack(&g_gpio5_3, GPIO_PIN_3, GPIO5_3_IrqCallback);
    IRQ_Register(IRQ_GPIO5, HAL_GPIO_IrqHandler, &g_gpio5_3);
    IRQ_SetPriority(IRQ_GPIO5, 0);
    IRQ_EnableN(IRQ_GPIO5);

    /* ========== 电机3 (肘, 原有): FR=GPIO4_7, BK=GPIO6_7, FG=GPIO6_2 ========== */
    /* 电机3 FR: GPIO4_7 */
    HAL_CRG_IpEnableSet(GPIO4_BASE, IP_CLK_ENABLE);
    g_gpio4_7.baseAddress = GPIO4;
    g_gpio4_7.pins = GPIO_PIN_7;
    HAL_GPIO_Init(&g_gpio4_7);
    HAL_GPIO_SetDirection(&g_gpio4_7, g_gpio4_7.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio4_7, g_gpio4_7.pins, GPIO_LOW_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio4_7, g_gpio4_7.pins, GPIO_INT_TYPE_NONE);

    /* 电机3 BK: GPIO6_7 (上电默认低=抱闸) */
    HAL_CRG_IpEnableSet(GPIO6_BASE, IP_CLK_ENABLE);
    g_gpio6_7.baseAddress = GPIO6;
    g_gpio6_7.pins = GPIO_PIN_7;
    HAL_GPIO_Init(&g_gpio6_7);
    HAL_GPIO_SetDirection(&g_gpio6_7, g_gpio6_7.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio6_7, g_gpio6_7.pins, GPIO_LOW_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio6_7, g_gpio6_7.pins, GPIO_INT_TYPE_NONE);

    /* 电机3 FG: GPIO6_2 (开漏上拉, 下降沿中断) */
    HAL_CRG_IpEnableSet(GPIO6_BASE, IP_CLK_ENABLE);
    g_gpio6_2.baseAddress = GPIO6;
    g_gpio6_2.pins = GPIO_PIN_2;
    HAL_GPIO_Init(&g_gpio6_2);
    HAL_GPIO_SetDirection(&g_gpio6_2, g_gpio6_2.pins, GPIO_INPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio6_2, g_gpio6_2.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio6_2, g_gpio6_2.pins, GPIO_INT_TYPE_FALL_EDGE);
    HAL_GPIO_SetPinsIrqCpuIDEx(&g_gpio6_2, g_gpio6_2.pins, CHIP_CORE_0);
    HAL_GPIO_RegisterCallBack(&g_gpio6_2, GPIO_PIN_2, GPIO6_2_IrqCallback);
    IRQ_Register(IRQ_GPIO6, HAL_GPIO_IrqHandler, &g_gpio6_2);
    IRQ_SetPriority(IRQ_GPIO6, 0);
    IRQ_EnableN(IRQ_GPIO6);

    /* UART1 TX/RX uses GPIO1_2/GPIO1_3, need GPIO1 clock */
    HAL_CRG_IpEnableSet(GPIO1_BASE, IP_CLK_ENABLE);

    /* GPIO5_4: output high */
    HAL_CRG_IpEnableSet(GPIO5_BASE, IP_CLK_ENABLE);
    g_gpio5_4.baseAddress = GPIO5;
    g_gpio5_4.pins = GPIO_PIN_4;
    HAL_GPIO_Init(&g_gpio5_4);
    HAL_GPIO_SetDirection(&g_gpio5_4, g_gpio5_4.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio5_4, g_gpio5_4.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio5_4, g_gpio5_4.pins, GPIO_INT_TYPE_NONE);

    /* ========== 步进电机1 (ID01, APT0 硬件脉冲) ========== */
    /* 步进 DIR: GPIO7_5 (初始高) */
    HAL_CRG_IpEnableSet(GPIO7_BASE, IP_CLK_ENABLE);
    g_gpio7_5.baseAddress = GPIO7;
    g_gpio7_5.pins = GPIO_PIN_5;
    HAL_GPIO_Init(&g_gpio7_5);
    HAL_GPIO_SetDirection(&g_gpio7_5, g_gpio7_5.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio7_5, g_gpio7_5.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio7_5, g_gpio7_5.pins, GPIO_INT_TYPE_NONE);

    /* 步进 REF: GPIO7_6 (可选, 初始高) */
    HAL_CRG_IpEnableSet(GPIO7_BASE, IP_CLK_ENABLE);
    g_gpio7_6.baseAddress = GPIO7;
    g_gpio7_6.pins = GPIO_PIN_6;
    HAL_GPIO_Init(&g_gpio7_6);
    HAL_GPIO_SetDirection(&g_gpio7_6, g_gpio7_6.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio7_6, g_gpio7_6.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio7_6, g_gpio7_6.pins, GPIO_INT_TYPE_NONE);

    /* 步进 MS3: GPIO3_5 (初始高) */
    HAL_CRG_IpEnableSet(GPIO3_BASE, IP_CLK_ENABLE);
    g_gpio3_5.baseAddress = GPIO3;
    g_gpio3_5.pins = GPIO_PIN_5;
    HAL_GPIO_Init(&g_gpio3_5);
    HAL_GPIO_SetDirection(&g_gpio3_5, g_gpio3_5.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio3_5, g_gpio3_5.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio3_5, g_gpio3_5.pins, GPIO_INT_TYPE_NONE);

    /* 步进 SLEEP: GPIO4_0 (高=正常工作) */
    HAL_CRG_IpEnableSet(GPIO4_BASE, IP_CLK_ENABLE);
    g_gpio4_0.baseAddress = GPIO4;
    g_gpio4_0.pins = GPIO_PIN_0;
    HAL_GPIO_Init(&g_gpio4_0);
    HAL_GPIO_SetDirection(&g_gpio4_0, g_gpio4_0.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio4_0, g_gpio4_0.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio4_0, g_gpio4_0.pins, GPIO_INT_TYPE_NONE);

    /* 步进 RESET: GPIO4_1 (高=正常运行) */
    HAL_CRG_IpEnableSet(GPIO4_BASE, IP_CLK_ENABLE);
    g_gpio4_1.baseAddress = GPIO4;
    g_gpio4_1.pins = GPIO_PIN_1;
    HAL_GPIO_Init(&g_gpio4_1);
    HAL_GPIO_SetDirection(&g_gpio4_1, g_gpio4_1.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio4_1, g_gpio4_1.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio4_1, g_gpio4_1.pins, GPIO_INT_TYPE_NONE);

    /* 步进 MS2: GPIO5_6 (初始低) */
    HAL_CRG_IpEnableSet(GPIO5_BASE, IP_CLK_ENABLE);
    g_gpio5_6.baseAddress = GPIO5;
    g_gpio5_6.pins = GPIO_PIN_6;
    HAL_GPIO_Init(&g_gpio5_6);
    HAL_GPIO_SetDirection(&g_gpio5_6, g_gpio5_6.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio5_6, g_gpio5_6.pins, GPIO_LOW_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio5_6, g_gpio5_6.pins, GPIO_INT_TYPE_NONE);

    /* 步进 MS1: GPIO5_7 (初始高) */
    HAL_CRG_IpEnableSet(GPIO5_BASE, IP_CLK_ENABLE);
    g_gpio5_7.baseAddress = GPIO5;
    g_gpio5_7.pins = GPIO_PIN_7;
    HAL_GPIO_Init(&g_gpio5_7);
    HAL_GPIO_SetDirection(&g_gpio5_7, g_gpio5_7.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio5_7, g_gpio5_7.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio5_7, g_gpio5_7.pins, GPIO_INT_TYPE_NONE);

    /* 步进 EN: GPIO6_5 (高=关断, 默认安全) */
    HAL_CRG_IpEnableSet(GPIO6_BASE, IP_CLK_ENABLE);
    g_gpio6_5.baseAddress = GPIO6;
    g_gpio6_5.pins = GPIO_PIN_5;
    HAL_GPIO_Init(&g_gpio6_5);
    HAL_GPIO_SetDirection(&g_gpio6_5, g_gpio6_5.pins, GPIO_OUTPUT_MODE);
    HAL_GPIO_SetValue(&g_gpio6_5, g_gpio6_5.pins, GPIO_HIGH_LEVEL);
    HAL_GPIO_SetIrqType(&g_gpio6_5, g_gpio6_5.pins, GPIO_INT_TYPE_NONE);

}

static void GPT0_Init(void)
{
    /* 电机2 PWM: GPT0 → GPIO7_0, period 9999 (0..9999 = ON占空比, 低电平=ON) */
    HAL_CRG_IpEnableSet(GPT0_BASE, IP_CLK_ENABLE);
    g_gptHandle0.baseAddress = GPT0;
    g_gptHandle0.clockDiv = 0;
    g_gptHandle0.period = 9999;
    g_gptHandle0.refA0.refdot = 0;
    g_gptHandle0.refA0.refAction = GPT_ACTION_OUTPUT_HIGH;   /* 上电默认高=PWM关(OFF) */
    g_gptHandle0.refB0.refdot = 0;
    g_gptHandle0.refB0.refAction = GPT_ACTION_NO_ACTION;
    g_gptHandle0.bufLoad = BASE_CFG_ENABLE;
    g_gptHandle0.pwmKeep = BASE_CFG_ENABLE;
    g_gptHandle0.handleEx.periodIntEnable = BASE_CFG_DISABLE;
    g_gptHandle0.handleEx.outputFinIntEnable = BASE_CFG_DISABLE;
    g_gptHandle0.triggleAdcOutFinish = BASE_CFG_DISABLE;
    g_gptHandle0.triggleAdcPeriod = BASE_CFG_DISABLE;
    HAL_GPT_Init(&g_gptHandle0);
}

static void GPT1_Init(void)
{
    /* 电机3 PWM: GPT1 → GPIO7_1 */
    HAL_CRG_IpEnableSet(GPT1_BASE, IP_CLK_ENABLE);
    g_gptHandle1.baseAddress = GPT1;
    g_gptHandle1.clockDiv = 0;
    g_gptHandle1.period = 9999;
    g_gptHandle1.refA0.refdot = 0;
    g_gptHandle1.refA0.refAction = GPT_ACTION_OUTPUT_HIGH;
    g_gptHandle1.refB0.refdot = 0;
    g_gptHandle1.refB0.refAction = GPT_ACTION_NO_ACTION;
    g_gptHandle1.bufLoad = BASE_CFG_ENABLE;
    g_gptHandle1.pwmKeep = BASE_CFG_ENABLE;
    g_gptHandle1.handleEx.periodIntEnable = BASE_CFG_DISABLE;
    g_gptHandle1.handleEx.outputFinIntEnable = BASE_CFG_DISABLE;
    g_gptHandle1.triggleAdcOutFinish = BASE_CFG_DISABLE;
    g_gptHandle1.triggleAdcPeriod = BASE_CFG_DISABLE;
    HAL_GPT_Init(&g_gptHandle1);
}

#define UART1_BAUD_RATE 115200

static void UART1_Init(void)
{
    HAL_CRG_IpEnableSet(UART1_BASE, IP_CLK_ENABLE);
    g_uart1.baseAddress = UART1;
    g_uart1.baudRate = UART1_BAUD_RATE;
    g_uart1.dataLength = UART_DATALENGTH_8BIT;
    g_uart1.stopBits = UART_STOPBITS_ONE;
    g_uart1.parity = UART_PARITY_NONE;
    g_uart1.txMode = UART_MODE_INTERRUPT;
    g_uart1.rxMode = UART_MODE_INTERRUPT;
    g_uart1.fifoMode = BASE_CFG_ENABLE;
    g_uart1.fifoTxThr = UART_FIFODEPTH_SIZE4;
    g_uart1.fifoRxThr = UART_FIFODEPTH_SIZE1;
    g_uart1.hwFlowCtr = BASE_CFG_DISABLE;
    g_uart1.handleEx.overSampleMultiple = UART_OVERSAMPLING_16X;
    g_uart1.handleEx.msbFirst = BASE_CFG_DISABLE;
    HAL_UART_Init(&g_uart1);

    /* Register callbacks */
    HAL_UART_RegisterCallBack(&g_uart1, UART_READ_IT_FINISH, UART1_ReadItFinishCallBack);
    HAL_UART_RegisterCallBack(&g_uart1, UART_TRNS_IT_ERROR, UART1_TransmitItErrorCallBack);

    /* Register IRQ handler */
    IRQ_Register(IRQ_UART1, HAL_UART_IrqHandler, &g_uart1);
    IRQ_SetPriority(IRQ_UART1, 1);
    IRQ_EnableN(IRQ_UART1);
}

static void IOConfig(void)
{
    /* JTAG */
    HAL_IOCMG_SetPinAltFuncMode(GPIO0_0_AS_JTAG_TCK);
    HAL_IOCMG_SetPinPullMode(GPIO0_0_AS_JTAG_TCK, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO0_0_AS_JTAG_TCK, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO0_0_AS_JTAG_TCK, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO0_0_AS_JTAG_TCK, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO0_0_AS_JTAG_TCK, OPEN_DRAIN_DISABLE);
    HAL_IOCMG_SetPinAltFuncMode(GPIO0_1_AS_JTAG_TMS);
    HAL_IOCMG_SetPinPullMode(GPIO0_1_AS_JTAG_TMS, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO0_1_AS_JTAG_TMS, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO0_1_AS_JTAG_TMS, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO0_1_AS_JTAG_TMS, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO0_1_AS_JTAG_TMS, OPEN_DRAIN_DISABLE);
    /* GPT0 PWM (电机2) */
    HAL_IOCMG_SetPinAltFuncMode(GPIO7_0_AS_GPT0_PWM);
    HAL_IOCMG_SetPinPullMode(GPIO7_0_AS_GPT0_PWM, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO7_0_AS_GPT0_PWM, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO7_0_AS_GPT0_PWM, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO7_0_AS_GPT0_PWM, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO7_0_AS_GPT0_PWM, OPEN_DRAIN_DISABLE);
    /* 电机2 FR: GPIO2_2 */
    HAL_IOCMG_SetPinAltFuncMode(GPIO2_2_AS_GPIO2_2);
    HAL_IOCMG_SetPinPullMode(GPIO2_2_AS_GPIO2_2, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO2_2_AS_GPIO2_2, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO2_2_AS_GPIO2_2, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO2_2_AS_GPIO2_2, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO2_2_AS_GPIO2_2, OPEN_DRAIN_DISABLE);
    /* 电机2 BK: GPIO4_6 */
    HAL_IOCMG_SetPinAltFuncMode(GPIO4_6_AS_GPIO4_6);
    HAL_IOCMG_SetPinPullMode(GPIO4_6_AS_GPIO4_6, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO4_6_AS_GPIO4_6, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO4_6_AS_GPIO4_6, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO4_6_AS_GPIO4_6, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO4_6_AS_GPIO4_6, OPEN_DRAIN_DISABLE);
    /* 电机2 FG: GPIO5_3 (开漏上拉) */
    HAL_IOCMG_SetPinAltFuncMode(GPIO5_3_AS_GPIO5_3);
    HAL_IOCMG_SetPinPullMode(GPIO5_3_AS_GPIO5_3, PULL_UP);
    HAL_IOCMG_SetPinSchmidtMode(GPIO5_3_AS_GPIO5_3, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO5_3_AS_GPIO5_3, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO5_3_AS_GPIO5_3, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO5_3_AS_GPIO5_3, OPEN_DRAIN_DISABLE);
    /* GPT1 PWM (电机3) */
    HAL_IOCMG_SetPinAltFuncMode(GPIO7_1_AS_GPT1_PWM);
    HAL_IOCMG_SetPinPullMode(GPIO7_1_AS_GPT1_PWM, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO7_1_AS_GPT1_PWM, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO7_1_AS_GPT1_PWM, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO7_1_AS_GPT1_PWM, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO7_1_AS_GPT1_PWM, OPEN_DRAIN_DISABLE);
    /* 电机3 FR */
    HAL_IOCMG_SetPinAltFuncMode(GPIO4_7_AS_GPIO4_7);
    HAL_IOCMG_SetPinPullMode(GPIO4_7_AS_GPIO4_7, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO4_7_AS_GPIO4_7, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO4_7_AS_GPIO4_7, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO4_7_AS_GPIO4_7, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO4_7_AS_GPIO4_7, OPEN_DRAIN_DISABLE);
    /* 电机3 BK */
    HAL_IOCMG_SetPinAltFuncMode(GPIO6_7_AS_GPIO6_7);
    HAL_IOCMG_SetPinPullMode(GPIO6_7_AS_GPIO6_7, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO6_7_AS_GPIO6_7, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO6_7_AS_GPIO6_7, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO6_7_AS_GPIO6_7, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO6_7_AS_GPIO6_7, OPEN_DRAIN_DISABLE);
    /* 电机3 FG: GPIO6_2 */
    HAL_IOCMG_SetPinAltFuncMode(GPIO6_2_AS_GPIO6_2);
    HAL_IOCMG_SetPinPullMode(GPIO6_2_AS_GPIO6_2, PULL_UP);
    HAL_IOCMG_SetPinSchmidtMode(GPIO6_2_AS_GPIO6_2, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO6_2_AS_GPIO6_2, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO6_2_AS_GPIO6_2, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO6_2_AS_GPIO6_2, OPEN_DRAIN_DISABLE);
    /* UART1: GPIO1_2 TX, GPIO1_3 RX */
    HAL_IOCMG_SetPinAltFuncMode(GPIO1_2_AS_UART1_TXD);
    HAL_IOCMG_SetPinPullMode(GPIO1_2_AS_UART1_TXD, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO1_2_AS_UART1_TXD, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO1_2_AS_UART1_TXD, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO1_2_AS_UART1_TXD, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO1_2_AS_UART1_TXD, OPEN_DRAIN_DISABLE);
    HAL_IOCMG_SetPinAltFuncMode(GPIO1_3_AS_UART1_RXD);
    HAL_IOCMG_SetPinPullMode(GPIO1_3_AS_UART1_RXD, PULL_UP);
    HAL_IOCMG_SetPinSchmidtMode(GPIO1_3_AS_UART1_RXD, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO1_3_AS_UART1_RXD, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO1_3_AS_UART1_RXD, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO1_3_AS_UART1_RXD, OPEN_DRAIN_DISABLE);
    /* GPIO5_4: output high */
    HAL_IOCMG_SetPinAltFuncMode(GPIO5_4_AS_GPIO5_4);
    HAL_IOCMG_SetPinPullMode(GPIO5_4_AS_GPIO5_4, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO5_4_AS_GPIO5_4, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO5_4_AS_GPIO5_4, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO5_4_AS_GPIO5_4, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO5_4_AS_GPIO5_4, OPEN_DRAIN_DISABLE);
    /* ========== 步进电机1 IO (ID01) ========== */
    HAL_IOCMG_SetPinAltFuncMode(GPIO7_5_AS_GPIO7_5);
    HAL_IOCMG_SetPinPullMode(GPIO7_5_AS_GPIO7_5, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO7_5_AS_GPIO7_5, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO7_5_AS_GPIO7_5, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO7_5_AS_GPIO7_5, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO7_5_AS_GPIO7_5, OPEN_DRAIN_DISABLE);

    HAL_IOCMG_SetPinAltFuncMode(GPIO7_6_AS_GPIO7_6);
    HAL_IOCMG_SetPinPullMode(GPIO7_6_AS_GPIO7_6, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO7_6_AS_GPIO7_6, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO7_6_AS_GPIO7_6, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO7_6_AS_GPIO7_6, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO7_6_AS_GPIO7_6, OPEN_DRAIN_DISABLE);

    HAL_IOCMG_SetPinAltFuncMode(GPIO3_5_AS_GPIO3_5);
    HAL_IOCMG_SetPinPullMode(GPIO3_5_AS_GPIO3_5, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO3_5_AS_GPIO3_5, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO3_5_AS_GPIO3_5, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO3_5_AS_GPIO3_5, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO3_5_AS_GPIO3_5, OPEN_DRAIN_DISABLE);

    HAL_IOCMG_SetPinAltFuncMode(GPIO4_0_AS_GPIO4_0);
    HAL_IOCMG_SetPinPullMode(GPIO4_0_AS_GPIO4_0, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO4_0_AS_GPIO4_0, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO4_0_AS_GPIO4_0, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO4_0_AS_GPIO4_0, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO4_0_AS_GPIO4_0, OPEN_DRAIN_DISABLE);

    HAL_IOCMG_SetPinAltFuncMode(GPIO4_1_AS_GPIO4_1);
    HAL_IOCMG_SetPinPullMode(GPIO4_1_AS_GPIO4_1, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO4_1_AS_GPIO4_1, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO4_1_AS_GPIO4_1, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO4_1_AS_GPIO4_1, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO4_1_AS_GPIO4_1, OPEN_DRAIN_DISABLE);

    HAL_IOCMG_SetPinAltFuncMode(GPIO5_6_AS_GPIO5_6);
    HAL_IOCMG_SetPinPullMode(GPIO5_6_AS_GPIO5_6, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO5_6_AS_GPIO5_6, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO5_6_AS_GPIO5_6, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO5_6_AS_GPIO5_6, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO5_6_AS_GPIO5_6, OPEN_DRAIN_DISABLE);

    HAL_IOCMG_SetPinAltFuncMode(GPIO5_7_AS_GPIO5_7);
    HAL_IOCMG_SetPinPullMode(GPIO5_7_AS_GPIO5_7, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO5_7_AS_GPIO5_7, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO5_7_AS_GPIO5_7, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO5_7_AS_GPIO5_7, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO5_7_AS_GPIO5_7, OPEN_DRAIN_DISABLE);

    HAL_IOCMG_SetPinAltFuncMode(GPIO6_5_AS_GPIO6_5);
    HAL_IOCMG_SetPinPullMode(GPIO6_5_AS_GPIO6_5, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO6_5_AS_GPIO6_5, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO6_5_AS_GPIO6_5, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO6_5_AS_GPIO6_5, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO6_5_AS_GPIO6_5, OPEN_DRAIN_DISABLE);

    /* ADC: GPIO3_2 → ADC0_ANA_A5, GPIO2_0 → ADC1_ANA_A2, GPIO6_0 → ADC2_ANA_A2
     * (ADC0=电机1/2/4 电位器跳线分板; ADC1=userPID完成 的电机2 接线; ADC2=电机3) */
    HAL_IOCMG_SetPinAltFuncMode(GPIO3_2_AS_ADC0_ANA_A5);
    HAL_IOCMG_SetPinPullMode(GPIO3_2_AS_ADC0_ANA_A5, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO3_2_AS_ADC0_ANA_A5, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinAltFuncMode(GPIO2_0_AS_ADC1_ANA_A2);
    HAL_IOCMG_SetPinPullMode(GPIO2_0_AS_ADC1_ANA_A2, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO2_0_AS_ADC1_ANA_A2, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinAltFuncMode(GPIO6_0_AS_ADC2_ANA_A2);
    HAL_IOCMG_SetPinPullMode(GPIO6_0_AS_ADC2_ANA_A2, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO6_0_AS_ADC2_ANA_A2, SCHMIDT_DISABLE);
    /* ADC3: GPIO16_5 → ADC3_ANA_A15 (电机2 电位器实际接线) */
    HAL_IOCMG_SetPinAltFuncMode(GPIO16_5_AS_ADC3_ANA_A15);
    HAL_IOCMG_SetPinPullMode(GPIO16_5_AS_ADC3_ANA_A15, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO16_5_AS_ADC3_ANA_A15, SCHMIDT_DISABLE);
    /* APT0 PWM (电机1 步进 STEP): GPIO3_4 = APT0_PWMA (硬件脉冲) */
    HAL_IOCMG_SetPinAltFuncMode(GPIO3_4_AS_APT0_PWMA);
    HAL_IOCMG_SetPinPullMode(GPIO3_4_AS_APT0_PWMA, PULL_NONE);
    HAL_IOCMG_SetPinSchmidtMode(GPIO3_4_AS_APT0_PWMA, SCHMIDT_DISABLE);
    HAL_IOCMG_SetPinLevelShiftRate(GPIO3_4_AS_APT0_PWMA, LEVEL_SHIFT_RATE_SLOW);
    HAL_IOCMG_SetPinDriveRate(GPIO3_4_AS_APT0_PWMA, DRIVER_RATE_2);
    HAL_IOCMG_SetPinOpenDrainModeEx(GPIO3_4_AS_APT0_PWMA, OPEN_DRAIN_DISABLE);
}

static void TIMER1_Init(void)
{
    /* 控制时基 5ms (load=999999 @200MHz): ISR只置标志, 速度环在主循环执行(用户改) */
    HAL_CRG_IpEnableSet(TIMER1_BASE, IP_CLK_ENABLE);
    g_timerCtrl.baseAddress = TIMER1;
    g_timerCtrl.load = 999999;
    g_timerCtrl.bgLoad = 999999; /* 5ms @ 200MHz */
    g_timerCtrl.mode = TIMER_MODE_RUN_PERIODIC;
    g_timerCtrl.prescaler = TIMERPRESCALER_NO_DIV;
    g_timerCtrl.size = TIMER_SIZE_32BIT;
    g_timerCtrl.interruptEn = BASE_CFG_ENABLE;
    g_timerCtrl.adcSocReqEnable = BASE_CFG_DISABLE;
    g_timerCtrl.dmaReqEnable = BASE_CFG_DISABLE;
    HAL_TIMER_Init(&g_timerCtrl);
    IRQ_Register(IRQ_TIMER1, HAL_TIMER_IrqHandler, &g_timerCtrl);
    HAL_TIMER_RegisterCallback(&g_timerCtrl, TIMER_PERIOD_FIN, SpeedLoop_ISR);
    IRQ_SetPriority(IRQ_TIMER1, 1);
    IRQ_EnableN(IRQ_TIMER1);
    HAL_TIMER_Start(&g_timerCtrl);
}

static void TIMER2_Init(void)
{
    HAL_CRG_IpEnableSet(TIMER2_BASE, IP_CLK_ENABLE);
    g_timerFg.baseAddress = TIMER2;
    g_timerFg.cntMode = TIMER_COUNT_UP;
    g_timerFg.load = 0xFFFFFFFF;
    g_timerFg.bgLoad = 0;
    g_timerFg.mode = TIMER_MODE_RUN_FREE;
    g_timerFg.prescaler = TIMERPRESCALER_NO_DIV;
    g_timerFg.size = TIMER_SIZE_32BIT;
    g_timerFg.interruptEn = BASE_CFG_DISABLE;
    g_timerFg.adcSocReqEnable = BASE_CFG_DISABLE;
    g_timerFg.dmaReqEnable = BASE_CFG_DISABLE;
    HAL_TIMER_Init(&g_timerFg);
    HAL_TIMER_Start(&g_timerFg);
}

/* ================================================================
 *  APT0: 电机1 步进 硬件脉冲发生器 (100Hz, 50%占空比)
 *  GPIO3_4 = APT0_PWMA → STEP; 每周期归零中断计步, 数满由回调停
 * ================================================================ */
static void APT0_Init(void)
{
    unsigned int aptClk;

    HAL_CRG_IpEnableSet(APT0_BASE, IP_CLK_ENABLE);
    aptClk = HAL_CRG_GetIpFreq((void *)APT0);

    g_aptStepper.baseAddress = APT0;

    /* 分频到 1MHz → UP_DOWN 周期=2×5000 → 100Hz */
    g_aptStepper.waveform.dividerFactor = aptClk / 1000000U - 1;
    // g_aptStepper.waveform.timerPeriod = 5000;       /* 2×5000/1MHz = 10ms = 100Hz */
    g_aptStepper.waveform.timerPeriod =2500;
    g_aptStepper.waveform.cntMode = APT_COUNT_MODE_UP_DOWN;
    g_aptStepper.waveform.basicType = APT_PWM_BASIC_A_HIGH_B_LOW;
    g_aptStepper.waveform.chAOutType = APT_PWM_OUT_BASIC_TYPE;
    g_aptStepper.waveform.chBOutType = APT_PWM_OUT_BASIC_TYPE;
    g_aptStepper.waveform.divInitVal = 0;
    g_aptStepper.waveform.cntInitVal = 0;
    g_aptStepper.waveform.cntCmpLeftEdge = 1250;    /* 50% 占空比 */
    g_aptStepper.waveform.cntCmpRightEdge = 1250;
    g_aptStepper.waveform.cntCmpLoadMode = APT_BUFFER_INDEPENDENT_LOAD;
    g_aptStepper.waveform.cntCmpLoadEvt = APT_COMPARE_LOAD_EVENT_ZERO;
    g_aptStepper.waveform.deadBandCnt = 0;

    /* 每周期归零中断 → 计步 */
    g_aptStepper.tmrInterrupt.tmrInterruptEn = BASE_CFG_ENABLE;
    g_aptStepper.tmrInterrupt.tmrInterruptSrc = APT_TIMER_INTERRUPT_SRC_CNTR_ZERO;
    g_aptStepper.tmrInterrupt.tmrInterruptScale = 1;

    HAL_APT_PWMInit(&g_aptStepper);
    HAL_APT_RegisterCallBack(&g_aptStepper, APT_TIMER_INTERRUPT, APT0_StepperCallback);
    IRQ_SetPriority(IRQ_APT0_TMR, 5);
    IRQ_Register(IRQ_APT0_TMR, HAL_APT_TimerIrqHandler, &g_aptStepper);
    IRQ_EnableN(IRQ_APT0_TMR);
}

/* ADC 初始化: 两个外设, 各配一个通道
 *   ADC0: SOC0 = A5 (GPIO3_2, 电机1/2/4 电位器 — 跳线分板共用)
 *   ADC2: SOC0 = A2 (GPIO6_0, 电机3 角度) */
static void ADC_Init(void)
{
    SOC_Param socParam = {0};
    socParam.sampleTotalTime = ADC_SOCSAMPLE_5CLK;
    socParam.trigSource = ADC_TRIGSOC_SOFT;
    socParam.continueMode = BASE_CFG_DISABLE;
    socParam.finishMode = ADC_SOCFINISH_NONE;

    /* ADC0: 电机1/2/4 电位器 (通道A5, GPIO3_2) */
    HAL_CRG_IpClkSelectSet(ADC0_BASE, CRG_ADC_CLK_ASYN_PLL_DIV);
    HAL_CRG_IpClkDivSet(ADC0_BASE, CRG_ADC_DIV_1);
    HAL_CRG_IpEnableSet(ADC0_BASE, IP_CLK_ENABLE);
    g_adc0.baseAddress = ADC0;
    g_adc0.socPriority = ADC_PRIMODE_ALL_ROUND;
    HAL_ADC_Init(&g_adc0);
    socParam.adcInput = ADC_CH_ADCINA5;
    HAL_ADC_ConfigureSoc(&g_adc0, ADC_SOC_NUM0, &socParam);

    /* ADC1: 诊断通道 (A2, GPIO2_0) — userPID完成 的电机2 接线 */
    HAL_CRG_IpClkSelectSet(ADC1_BASE, CRG_ADC_CLK_ASYN_PLL_DIV);
    HAL_CRG_IpClkDivSet(ADC1_BASE, CRG_ADC_DIV_1);
    HAL_CRG_IpEnableSet(ADC1_BASE, IP_CLK_ENABLE);
    g_adc1.baseAddress = ADC1;
    g_adc1.socPriority = ADC_PRIMODE_ALL_ROUND;
    HAL_ADC_Init(&g_adc1);
    socParam.adcInput = ADC_CH_ADCINA2;
    HAL_ADC_ConfigureSoc(&g_adc1, ADC_SOC_NUM0, &socParam);

    /* ADC2: 电机3 角度 (通道A2, GPIO6_0) */
    HAL_CRG_IpClkSelectSet(ADC2_BASE, CRG_ADC_CLK_ASYN_PLL_DIV);
    HAL_CRG_IpClkDivSet(ADC2_BASE, CRG_ADC_DIV_1);
    HAL_CRG_IpEnableSet(ADC2_BASE, IP_CLK_ENABLE);
    g_adc2.baseAddress = ADC2;
    g_adc2.socPriority = ADC_PRIMODE_ALL_ROUND;
    HAL_ADC_Init(&g_adc2);
    socParam.adcInput = ADC_CH_ADCINA2;
    HAL_ADC_ConfigureSoc(&g_adc2, ADC_SOC_NUM0, &socParam);

    /* ADC3: 电机2 电位器 (通道A15, GPIO16_5) — 实际接线 */
    HAL_CRG_IpClkSelectSet(ADC3_BASE, CRG_ADC_CLK_ASYN_PLL_DIV);
    HAL_CRG_IpClkDivSet(ADC3_BASE, CRG_ADC_DIV_1);
    HAL_CRG_IpEnableSet(ADC3_BASE, IP_CLK_ENABLE);
    g_adc3.baseAddress = ADC3;
    g_adc3.socPriority = ADC_PRIMODE_ALL_ROUND;
    HAL_ADC_Init(&g_adc3);
    socParam.adcInput = ADC_CH_ADCINA15;
    HAL_ADC_ConfigureSoc(&g_adc3, ADC_SOC_NUM0, &socParam);
}

void SystemInit(void)
{
    IOConfig();
    UART1_Init();
    GPT0_Init();
    GPT1_Init();
    GPIO_Init();
    APT0_Init();
    ADC_Init();
    TIMER1_Init();
    TIMER2_Init();
}

#include "uart_bus.h"
#include "main.h"  /* SDK HAL: gpio / iocmg / 引脚复用宏 */

/* ============================================================
 * 本板地址: 2 位拨码/跳线 (高=1, 低=0)
 *   bit0 = GPIO3_0, bit1 = GPIO2_7
 *   若硬件反接(下拉=1), 把下面的 GPIO_HIGH_LEVEL 换成 GPIO_LOW_LEVEL
 * ============================================================ */
#define ADDR_PIN_BIT0   GPIO_PIN_0   /* GPIO3_0 → 地址 bit0 */
#define ADDR_PIN_BIT1   GPIO_PIN_7   /* GPIO2_7 → 地址 bit1 */

static GPIO_Handle s_addrBit0;   /* GPIO3 */
static GPIO_Handle s_addrBit1;   /* GPIO2 */
static GPIO_Handle s_txPin;      /* GPIO1_2 (仅方向控制) */

void UartBus_Init(void)
{
    /* 地址脚时钟 (system_init 已不再初始化 GPIO3, 这里自给自足) */
    HAL_CRG_IpEnableSet(GPIO2_BASE, IP_CLK_ENABLE);
    HAL_CRG_IpEnableSet(GPIO3_BASE, IP_CLK_ENABLE);

    /* 地址脚: GPIO3_0 输入 */
    s_addrBit0.baseAddress = GPIO3;
    s_addrBit0.pins = ADDR_PIN_BIT0;
    HAL_GPIO_Init(&s_addrBit0);
    HAL_GPIO_SetDirection(&s_addrBit0, ADDR_PIN_BIT0, GPIO_INPUT_MODE);
    HAL_GPIO_SetIrqType(&s_addrBit0, ADDR_PIN_BIT0, GPIO_INT_TYPE_NONE);

    /* 地址脚: GPIO2_7 输入 */
    s_addrBit1.baseAddress = GPIO2;
    s_addrBit1.pins = ADDR_PIN_BIT1;
    HAL_GPIO_Init(&s_addrBit1);
    HAL_GPIO_SetDirection(&s_addrBit1, ADDR_PIN_BIT1, GPIO_INPUT_MODE);
    HAL_GPIO_SetIrqType(&s_addrBit1, ADDR_PIN_BIT1, GPIO_INT_TYPE_NONE);

    /* TX 脚句柄 (不 Init, 避免破坏 UART1 已配好的外设) */
    s_txPin.baseAddress = GPIO1;
    s_txPin.pins = GPIO_PIN_2;

    /* 上电默认: TX 不驱动总线 */
    UartBus_TxMuxToGpio();
}

uint8_t UartBus_GetAddr(void)
{
    uint8_t addr = 0;
    if (HAL_GPIO_GetPinValue(&s_addrBit0, ADDR_PIN_BIT0) == GPIO_HIGH_LEVEL)
        addr |= 0x01U;
    if (HAL_GPIO_GetPinValue(&s_addrBit1, ADDR_PIN_BIT1) == GPIO_HIGH_LEVEL)
        addr |= 0x02U;
    return addr;
}

bool UartBus_IsMyMessage(uint8_t msgAddr)
{
    return (UartBus_GetAddr() == msgAddr);
}

void UartBus_TxMuxToUart(void)
{
    HAL_IOCMG_SetPinAltFuncMode(GPIO1_2_AS_UART1_TXD);
    HAL_IOCMG_SetPinPullMode(GPIO1_2_AS_UART1_TXD, PULL_NONE);
}

void UartBus_TxMuxToGpio(void)
{
#if (UART_BUS_ENABLE == 1)
    HAL_IOCMG_SetPinAltFuncMode(GPIO1_2_AS_GPIO1_2);
    HAL_IOCMG_SetPinPullMode(GPIO1_2_AS_GPIO1_2, PULL_NONE);
    HAL_GPIO_SetDirection(&s_txPin, GPIO_PIN_2, GPIO_INPUT_MODE); /* 高阻输入 */
#endif
    /* 单板调试模式 (UART_BUS_ENABLE=0): 不做任何事, TX 保持 UART1 常开 */
}

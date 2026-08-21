#ifndef UART_BUS_H
#define UART_BUS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief  UART1 总线复用接口 (多板共用一条串口线)
 *
 *  设计: UART1 TX (GPIO1_2) 平常保持 GPIO 输入 (高阻, 不驱动总线);
 *        收到消息后读本板地址脚 (GPIO3_1 / GPIO3_0) 电平, 与消息地址比对;
 *        匹配才把 TX 复用为 UART1_TXD 发应答, 发完切回 GPIO 输入。
 *
 *  总线模式开关:
 *    1 = 总线模式 — TX 常态 GPIO 高阻 (多板共线)  ← 4块板部署用这个
 *    0 = 单板调试  — TX 保持 UART1 常开, 串口日志/回显可用  ← 当前调试用
 */
#define UART_BUS_ENABLE 0

/* 初始化: 地址脚 GPIO 输入 + TX 脚默认切回 GPIO (不复用) */
void UartBus_Init(void);

/* 读本板地址: bit1=GPIO3_1(PIN76), bit0=GPIO3_0(PIN75), 高=1 低=0 → 0~3 */
uint8_t UartBus_GetAddr(void);

/* 消息地址是否本板该响应: 电平地址 == 消息地址 */
bool UartBus_IsMyMessage(uint8_t msgAddr);

/* 复用: GPIO1_2 → UART1_TXD (准备发应答) */
void UartBus_TxMuxToUart(void);

/* 不复用: GPIO1_2 → GPIO 输入 (高阻, 不驱动总线) */
void UartBus_TxMuxToGpio(void);

#endif

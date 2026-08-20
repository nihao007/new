#ifndef UART_RING_H
#define UART_RING_H

#include <stdint.h>

/* 接收字节回显开关: 1=每收到一字节 DBG_PRINTF("%02X ") (单板调试用)
 * 0=关闭 — 多板共线时必须关: 板与板互相回显会死循环刷爆总线 */
#define UART_RING_ECHO 0

void UART_Ring_Init(void);
uint8_t UART_Ring_HasData(void);
uint8_t UART_Ring_ReadByte(void);

#endif

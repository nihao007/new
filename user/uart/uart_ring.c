#include "uart_ring.h"
#include "main.h"  /* SDK UART handle, g_uart1, UART1_ReadItFinishCallBack prototype */
#include "debug.h" /* DBG_PRINTF */

#define RX_BUF 256

static struct {
    UART_Handle *handle;
    uint8_t buf[RX_BUF];
    volatile uint16_t head, tail;
    volatile uint8_t byte;
} g_uart;

void UART_Ring_Init(void)
{
    g_uart.handle = &g_uart1;
    g_uart.head = 0;
    g_uart.tail = 0;
    HAL_UART_ReadIT(&g_uart1, (unsigned char *)&g_uart.byte, 1);
}

uint8_t UART_Ring_HasData(void)
{
    return (g_uart.head != g_uart.tail) ? 1 : 0;
}

uint8_t UART_Ring_ReadByte(void)
{
    uint8_t b;
    if (g_uart.head == g_uart.tail)
        return 0;
    b = g_uart.buf[g_uart.tail];
    g_uart.tail = (g_uart.tail + 1) % RX_BUF;
    return b;
}

/* SDK UART1 中断回调 — 弱函数覆写 */
void UART1_ReadItFinishCallBack(void *handle)
{
    uint8_t b = g_uart.byte;

#if (UART_RING_ECHO == 1)
    DBG_PRINTF("%02X ", b);   /* 调试回显 (单板日志用; 多板总线时改 UART_RING_ECHO=0) */
#endif

    uint16_t n = (g_uart.head + 1) % RX_BUF;
    if (n != g_uart.tail) {
        g_uart.buf[g_uart.head] = b;
        g_uart.head = n;
    }
    HAL_UART_ReadIT((UART_Handle *)handle, (unsigned char *)&g_uart.byte, 1);
}

/* SDK UART1 发送错误回调 — 必填弱函数 */
void UART1_TransmitItErrorCallBack(void *handle)
{
    HAL_UART_ReadIT((UART_Handle *)handle, (unsigned char *)&g_uart.byte, 1);
}

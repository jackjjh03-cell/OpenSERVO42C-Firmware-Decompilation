#ifndef OS_UART_TRANSPORT_H
#define OS_UART_TRANSPORT_H
#include "board.h"
#include "stream.h"
typedef struct {
    os_stream rx;
    uint8_t tx[OS_STREAM_CAPACITY];
    uint8_t ready,tx_len,tx_next;
    uint32_t dropped;
} os_uart;
/* Caller owns NVIC setup and serializes take/send with the UART IRQ.
 * ISR only copies bytes; dispatch and configuration remain foreground work. */
bool os_uart_init(os_uart *u,const os_board_io *io,uint32_t peripheral_hz,uint32_t baud);
void os_uart_irq(os_uart *u,const os_board_io *io,uint32_t now_ms);
size_t os_uart_take(os_uart *u,uint8_t *out);
bool os_uart_send(os_uart *u,const os_board_io *io,const uint8_t *frame,size_t n);
#endif

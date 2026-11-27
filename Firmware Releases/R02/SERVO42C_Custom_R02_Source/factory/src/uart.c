#include "firmware_api.h"
/* Clear RX state but leave TX count and buffer untouched. */
void uart_reset_rx(void) {
    UART->count=0;
    UART->ready=0;
    for (unsigned i=0;i<9;i++) UART->rx[i]=0;
}
/* Receive packet bytes, including embedded address bytes for the supported
 * commands. Dispatch only after the checksum is accepted. */
static inline uint8_t sum_bytes(volatile const uint8_t *p, unsigned n) {
    uint8_t s=0;
    for (unsigned i=0;i<n;i++)s=(uint8_t)(s+p[i]);
    return s;
}
void uart_receive_byte(volatile UartState *u, uint16_t address, uint8_t byte) {
    if (u->ready) return;
    uint8_t op=u->rx[1];
    if (byte==address) {
        if (u->count<2 || u->rx[0]!=address || !(op==0x84 || (op>>4)==0xA || op==0xF6 || op==0xFD))u->count=0;
    } else if (u->rx[0]!=address) {
        u->count=0;
        return;
    }
 /* On the factory path count cannot exceed 8; retain a guard for malformed state. */
    if (u->count>=9) {
        u->count=0;
        u->ready=0;
        for (unsigned i=0;i<9;i++)u->rx[i]=0;
        return;
    }
    u->rx[u->count]=byte;
    u->count++;
    unsigned n=u->count;
    op=u->rx[1];
    switch (n) {
    case 3: if ((op>>4)!=3 && op!=0xF7)return;
        break;
    case 4: if ((op>>4)!=8 && (op>>4)!=9 && op!=0xF3 && op!=0xF6 && op!=0xFF)return;
        break;
    case 5: if ((op>>4)!=0xA) {
            if (op!=0xFD)u->count=0;
            return;
        }
        break;
    case 6: case 7: if (op!=0xFD)u->count=0;
        return;
    case 8: break; /* No opcode restriction at count 8 in original. */
    case 9: u->count=0;
        u->ready=0;
        for (unsigned i=0;i<9;i++)u->rx[i]=0;
        return;
    default:return;
    }
    if (sum_bytes(u->rx, n-1)==u->rx[n-1])u->ready=1;
    else u->count=0;
}
/* UART interrupt service. Preserve register access order; some register bit
 * meanings are unconfirmed. */
void uart0_irq(void) {
    if (RAM32(0x40000010)&2u) {
        if (UART->tx_remaining==0) {
            RAM32(0x40020C38)&=~7u;
            RAM32(0x40020D00)|=0x4000u;
            factory_uart_tx_end();
            uart_reset_rx();
        } else {
            UART->tx_remaining--;
            RAM32(0x40000000)=(RAM32(0x40000000)&~255u)|UART->tx_reverse[UART->tx_remaining];
        }
        RAM32(0x40000014)&=~2u;
    } else if (RAM32(0x40000010)&1u) {
        uint8_t b=(uint8_t)RAM32(0x40000000);
        if (!UART->ready) {
            uart_receive_byte(UART, LIVE->uart_address, b);
            if (UART->ready) {
                if (RAM8(WORK_RAM(WS_UART_MOTION_STATE))==2 && (UART->rx[1]==0xFD || UART->rx[1]==0xF6))RAM8(WORK_RAM(WS_UART_MOTION_STATE))=0;
                factory_dispatch();
            }
        }
        RAM32(0x40000014)&=~1u;
    }
}
/* start transmitting the address, then IRQ sends reverse-buffer bytes. */
void uart_start_reply(void) {
    RAM32(0x40020D00)&=~0x4000u;
    RAM32(0x40020C38)=(RAM32(0x40020C38)&~7u)|2u;
    RAM32(0x40000004)=(RAM32(0x40000004)&~1u)|2u;
    RAM32(0x40000014)&=~3u;
    RAM32(0x40000000)=(RAM32(0x40000000)&~255u)|(uint8_t)LIVE->uart_address;
}

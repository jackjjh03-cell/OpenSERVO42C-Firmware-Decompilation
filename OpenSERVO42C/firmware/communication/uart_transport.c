#include "uart_transport.h"
#define R(a) io->read(io->ctx,(a))
#define W(a,v) io->write(io->ctx,(a),(v))
#define F(a,m,v) W((a),(R(a)&~(uint32_t)(m))|((uint32_t)(v)&(uint32_t)(m)))
#define U 0x40000000u
static void receive(const os_board_io *io){
    F(0x40020c38u,7u,0);F(0x40020d00u,0x4000u,0x4000u);
    F(U+0x14,3u,0);F(U+4,3u,1u);
}
bool os_uart_init(os_uart *u,const os_board_io *io,uint32_t peripheral_hz,uint32_t baud){
    if(!baud||baud>UINT32_MAX/8u)return false;
    uint32_t div=peripheral_hz/(baud*8u);
    if(!div||div>65535u)return false;
    *u=(os_uart){0};
    /* Original factory UART configuration; pin setup is a separate board call. */
    W(U+4,0);W(U+4,0x240u);W(U+0x18,div);F(U+4,16u,16u);
    F(0x40020c34u,7u,2u);receive(io);return true;
}
void os_uart_irq(os_uart *u,const os_board_io *io,uint32_t now_ms){
    uint32_t flags=R(U+0x10);
    if(flags&2u){
        if(u->tx_next<u->tx_len){F(U,255u,u->tx[u->tx_next]);u->tx_next++;F(U+0x14,2u,0);}
        else{u->tx_len=0;u->tx_next=0;receive(io);os_stream_reset(&u->rx);}
    }else if(flags&1u){
        uint8_t byte=(uint8_t)R(U);
        if(u->ready||u->tx_len){if(u->dropped!=UINT32_MAX)u->dropped++;}
        else{size_t n=os_stream_feed(&u->rx,byte,now_ms);if(n)u->ready=(uint8_t)n;}
        F(U+0x14,1u,0);
    }
}
size_t os_uart_take(os_uart *u,uint8_t *out){
    size_t n=u->ready;for(size_t i=0;i<n;i++)out[i]=u->rx.bytes[i];u->ready=0;return n;
}
bool os_uart_send(os_uart *u,const os_board_io *io,const uint8_t *frame,size_t n){
    if(u->tx_len||u->ready||n<8u||n>sizeof u->tx)return false;
    for(size_t i=0;i<n;i++)u->tx[i]=frame[i];
    u->tx_len=(uint8_t)n;u->tx_next=1;
    F(0x40020d00u,0x4000u,0);F(0x40020c38u,7u,2u);
    F(U+4,3u,2u);F(U+0x14,3u,0);F(U,255u,u->tx[0]);return true;
}

#include "uart_transport.h"
#include "protocol.h"
#include <assert.h>
#include <stdio.h>
typedef struct {uint32_t ctrl,flags,clear,div,rxmux,txmux,gpio,data;uint8_t sent[56];unsigned count;} mock;
static uint32_t read(void *ctx,uint32_t a){mock *m=ctx;switch(a){
    case 0x40000000u:return m->data;case 0x40000004u:return m->ctrl;case 0x40000010u:return m->flags;
    case 0x40000014u:return m->clear;case 0x40000018u:return m->div;case 0x40020c34u:return m->rxmux;
    case 0x40020c38u:return m->txmux;case 0x40020d00u:return m->gpio;default:assert(0);return 0;}}
static void write(void *ctx,uint32_t a,uint32_t v){mock *m=ctx;switch(a){
    case 0x40000000u:m->data=v;assert(m->count<56);m->sent[m->count++]=(uint8_t)v;break;
    case 0x40000004u:m->ctrl=v;break;case 0x40000014u:m->clear=v;break;case 0x40000018u:m->div=v;break;
    case 0x40020c34u:m->rxmux=v;break;case 0x40020c38u:m->txmux=v;break;case 0x40020d00u:m->gpio=v;break;default:assert(0);}}
int main(void){mock m={0};os_board_io io={&m,read,write,0,0};os_uart u;
    assert(!os_uart_init(&u,&io,48000000,0));assert(!os_uart_init(&u,&io,1,115200));
    assert(os_uart_init(&u,&io,48000000,115200));assert(m.div==52&&(m.ctrl&3)==1&&m.txmux==0);
    uint8_t req[8]={0xa5,0x5a,1,42,OS_HELLO,0,0,0},in[56],out[56];
    uint16_t crc=os_crc16(req+2,4);req[6]=(uint8_t)crc;req[7]=(uint8_t)(crc>>8);
    for(unsigned i=0;i<8;i++){m.data=req[i];m.flags=1;os_uart_irq(&u,&io,i);}
    assert(u.ready==8);m.data=0xa5;os_uart_irq(&u,&io,9);assert(u.dropped==1);
    assert(os_uart_take(&u,in)==8&&in[3]==42);
    os_controller c;os_init(&c);size_t n=os_dispatch(&c,in,8,out);assert(n==13);
    assert(os_uart_send(&u,&io,out,n));assert((m.ctrl&3)==2&&m.txmux==2);
    assert(!os_uart_send(&u,&io,out,n));
    m.flags=2;for(unsigned i=0;i<n;i++)os_uart_irq(&u,&io,10+i);
    assert(!u.tx_len&&(m.ctrl&3)==1&&m.txmux==0&&m.count==n);
    for(unsigned i=0;i<n;i++)assert(m.sent[i]==out[i]);
    puts("UART transport tests passed");return 0;
}

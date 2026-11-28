/* Original V1.1.3 ADC slots and timer compare addresses. Caller owns
 * initialization, interrupt synchronization and physical bridge inhibit. */
#include "current.h"
#define REG(a) (*(volatile uint32_t *)(uintptr_t)(a))
bool os_hc32_adc_snapshot(uint16_t *scale,uint16_t *slot2,uint16_t *slot1){
    if(!(REG(0x400024b0u)&0x20u))return false;
    uint32_t s=REG(0x40002490u),b=REG(0x40002494u),a=REG(0x40002498u);
    if(s>4095u||a>4095u||b>4095u)return false;
    *scale=(uint16_t)s;*slot2=(uint16_t)a;*slot1=(uint16_t)b;return true;
}
static void write_compare(uint32_t addr,uint16_t value){REG(addr)=(REG(addr)&0xffff0000u)|value;}
/* This only writes timer compare registers; it does NOT enable/disable
 * the power stage. Use the original output gate sequence independently. */
bool os_hc32_write_compare(const os_pwm_compare *p){
    if(p->a0>1200u||p->a1>1200u||p->b0>1200u||p->b1>1200u||
       (p->a0!=1200u&&p->a1!=1200u)||(p->b0!=1200u&&p->b1!=1200u))return false;
    if(p->a0==1200u){write_compare(0x40000c3cu,1200);write_compare(0x40000c40u,p->a1);}
    else{write_compare(0x40000c40u,1200);write_compare(0x40000c3cu,p->a0);}
    if(p->b0==1200u){write_compare(0x40000d3cu,1200);write_compare(0x40000d40u,p->b1);}
    else{write_compare(0x40000d40u,1200);write_compare(0x40000d3cu,p->b0);}
    return true;
}

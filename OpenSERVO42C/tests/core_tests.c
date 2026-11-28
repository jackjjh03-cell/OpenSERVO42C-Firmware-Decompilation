#include "controller.h"
#include "protocol.h"
#include <assert.h>
#include <string.h>
static os_sample healthy(void){return (os_sample){.enable_input=true,.feedback_valid=true};}
static void tick(os_controller *c,os_sample *s,unsigned n){while(n--)os_tick_1ms(c,s);}
static void setup(os_controller *c,os_sample *s){
    os_init(c);
    assert(!os_arm(c));
    c->hal_validated=true; /* simulation ONLY */
    *s=healthy();tick(c,s,100);assert(os_arm(c));
}
int main(void){
    os_controller c;os_sample s;
    setup(&c,&s);s.target=5000;s.load_permille=950;
    tick(&c,&s,100);assert(c.observed&OS_F_STALL);assert(c.state==OS_ACTIVE);assert(!c.warnings);
    os_disable(&c);os_config v=c.cfg;v.policy=OS_PROTECT;assert(os_configure(&c,&v));
    s=healthy();tick(&c,&s,100);assert(os_arm(&c));
    s.target=5000;s.load_permille=950;tick(&c,&s,99);assert(c.state==OS_ACTIVE);
    s.actual=s.target;tick(&c,&s,1);assert(c.state==OS_ACTIVE);
    s.actual=0;tick(&c,&s,100);assert(c.state==OS_FAULT);assert(c.faults&OS_F_FOLLOW);
    assert(!os_acknowledge(&c));s=healthy();s.enable_input=false;tick(&c,&s,100);
    assert(os_acknowledge(&c));assert(c.position_lost);assert(!os_arm(&c));
    setup(&c,&s);s.critical=OS_F_ELECTRICAL;tick(&c,&s,1);assert(c.state==OS_FAULT);
    assert(c.faults&OS_F_ELECTRICAL);os_disable(&c);assert(c.state==OS_FAULT);
    assert(os_crc16((const uint8_t*)"123456789",9)==0x29b1);
    uint8_t req[8]={0xa5,0x5a,1,42,OS_HELLO,0,0,0},out[OS_FRAME_MAX];
    uint16_t crc=os_crc16(req+2,4);req[6]=(uint8_t)crc;req[7]=(uint8_t)(crc>>8);
    assert(os_dispatch(&c,req,sizeof req,out)==13);assert(out[3]==42&&out[4]==0x81&&out[6]==0&&out[10]==0);
    assert(os_crc16(out+2,9)==(uint16_t)(out[11]|out[12]<<8));
    req[6]^=1;assert(os_dispatch(&c,req,sizeof req,out)==0);
    assert(os_dispatch(&c,req,7,out)==0);
    os_init(&c);v=c.cfg;v.dwell_ms=0;assert(!os_configure(&c,&v));
    /* The full signed-position span is UINT32_MAX, not a signed overflow. */
    for(unsigned i=0;i<2u;i++){
        setup(&c,&s);c.cfg.following_counts=UINT32_MAX;c.cfg.dwell_ms=1;
        s.actual=i?INT32_MIN:INT32_MAX;s.target=i?INT32_MAX:INT32_MIN;
        tick(&c,&s,1);assert(c.observed&OS_F_FOLLOW);
        s.target+=i?-1:1;tick(&c,&s,1);assert(!(c.observed&OS_F_FOLLOW));
    }
    return 0;
}

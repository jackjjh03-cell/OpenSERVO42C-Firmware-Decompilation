#include "output_gate.h"
#include <assert.h>
#include <stdio.h>
static const uint32_t addresses[]={0x40000c30u,0x40000d30u,0x40000c3cu,0x40000c40u,0x40000d3cu,0x40000d40u};
typedef struct{uint32_t values[6];bool fail;} mock;
static unsigned idx(uint32_t addr){for(unsigned i=0;i<6;i++)if(addresses[i]==addr)return i;assert(0);return 0;}
static uint32_t read(void *p,uint32_t addr){return ((mock *)p)->values[idx(addr)];}
static void write(void *p,uint32_t addr,uint32_t v){mock *m=p;unsigned i=idx(addr);if(i==1&&m->fail)v&=~0x1000u;m->values[i]=v;
    if(i<2&&v&0x1000u){for(unsigned j=2;j<6;j++)assert((m->values[j]&65535u)==1200u);}
    if(i>=2&&(m->values[0]&m->values[1]&0x1000u)){assert((m->values[2]&65535u)==1200u||(m->values[3]&65535u)==1200u);assert((m->values[4]&65535u)==1200u||(m->values[5]&65535u)==1200u);}
}
int main(void){mock m={0};os_board_io io={&m,read,write,0,0};os_output_gate g={0};os_pwm_compare a={900,1200,1200,950},b={1200,900,950,1200},bad={900,900,1200,1200};
    assert(!os_gate_open(&g,&io,false));assert(!g.enabled);
    assert(os_gate_open(&g,&io,true));assert(os_gate_update(&g,&io,true,&a));assert(os_gate_update(&g,&io,true,&b));
    assert(!os_gate_update(&g,&io,true,&bad));assert(!g.enabled);assert(!os_gate_update(&g,&io,true,&a));
    assert(os_gate_open(&g,&io,true));assert(!os_gate_update(&g,&io,false,&a));assert(!g.enabled);
    m.fail=true;assert(!os_gate_open(&g,&io,true));assert(!g.enabled&&!(m.values[0]&0x1000u));
    puts("output gate tests passed");return 0;}

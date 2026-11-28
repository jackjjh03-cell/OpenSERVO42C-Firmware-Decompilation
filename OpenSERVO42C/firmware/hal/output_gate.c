#include "output_gate.h"
#define R(a) io->read(io->ctx,(a))
#define W(a,v) io->write(io->ctx,(a),(v))
#define F(a,m,v) W((a),(R(a)&~(uint32_t)(m))|((uint32_t)(v)&(uint32_t)(m)))
static void compare(const os_board_io *io,uint32_t a,uint16_t n){F(a,0xffffu,n);}
static bool gates(const os_board_io *io){return (R(0x40000c30u)&R(0x40000d30u)&0x1000u)!=0u;}
void os_gate_close(os_output_gate *g,const os_board_io *io){
    g->enabled=false;
    F(0x40000c30u,0x1000u,0);F(0x40000d30u,0x1000u,0);
    compare(io,0x40000c3cu,1200);compare(io,0x40000c40u,1200);
    compare(io,0x40000d3cu,1200);compare(io,0x40000d40u,1200);
    /* Leave ADC/DMA running for disabled-state diagnostics and recovery. */
}
bool os_gate_open(os_output_gate *g,const os_board_io *io,bool permitted){
    os_gate_close(g,io);
    if(!permitted)return false;
    F(0x40000c30u,0x1000u,0x1000u);F(0x40000d30u,0x1000u,0x1000u);
    if(!gates(io)){os_gate_close(g,io);return false;}
    g->enabled=true;return true;
}
bool os_gate_update(os_output_gate *g,const os_board_io *io,bool permitted,const os_pwm_compare *p){
    if(!permitted||!g->enabled||!gates(io)||!p||p->a0>1200u||p->a1>1200u||p->b0>1200u||p->b1>1200u||
       (p->a0!=1200u&&p->a1!=1200u)||(p->b0!=1200u&&p->b1!=1200u)){
        os_gate_close(g,io);return false;
    }
    if(p->a0==1200u){compare(io,0x40000c3cu,1200);compare(io,0x40000c40u,p->a1);}
    else{compare(io,0x40000c40u,1200);compare(io,0x40000c3cu,p->a0);}
    if(p->b0==1200u){compare(io,0x40000d3cu,1200);compare(io,0x40000d40u,p->b1);}
    else{compare(io,0x40000d40u,1200);compare(io,0x40000d3cu,p->b0);}
    return true;
}

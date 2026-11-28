#include "feedback_dma.h"
#define R(a) io->read(io->ctx,(a))
#define W(a,v) io->write(io->ctx,(a),(v))
#define F(a,m,v) W((a),(R(a)&~(uint32_t)(m))|((uint32_t)(v)&(uint32_t)(m)))
/* Original startup_prepare.c, clock_adc_config.c and current_feedback.c.
 * Destination replaces only the recovered fixed work-RAM address. The caller
 * configures IRQ 4 before starting, and leaves both power-stage gates closed. */
void os_feedback_dma_start(const os_board_io *io,uint32_t destination){
    F(0x40000c30u,0x1000u,0);F(0x40000d30u,0x1000u,0);
    F(0x40002404u,0x8000u,0);F(0x40021014u,0xf0000u,0);F(0x400024b4u,0x20u,0);
    F(0x40002408u,0x200u,0x200u);F(0x4000244cu,0x300000u,0x200000u);
    /* Factory timer trigger fields, before starting timer 0. */
    F(0x40000c20u,0x93u,0x81u);W(0x40000c14u,0);W(0x40000c14u,0xfffffffeu);
    W(0x40021014u,0);W(0x40021014u,0x08e00001u);
    W(0x40021010u,(R(0x40021010u)&0xe0300000u)|0x12020000u);
    W(0x40021018u,0x40002490u);W(0x4002101cu,destination);
    F(0x40021014u,0x70000u,0);F(0x40021000u,0x80000000u,0x80000000u);
    F(0x40021010u,0x80000000u,0x80000000u);
    F(0x40021014u,0x80000u,0x80000u);F(0x40000c0cu,1u,1u);
}
void os_feedback_dma_stop(const os_board_io *io){
    F(0x40021014u,0x80000u,0);F(0x40021014u,0x70000u,0);F(0x400024b4u,0x20u,0);
}
bool os_feedback_dma_complete(const os_board_io *io){
    bool done=((R(0x40021014u)>>16)&7u)==5u;
    F(0x40021014u,0x70000u,0);F(0x400024b4u,0x20u,0);return done;
}

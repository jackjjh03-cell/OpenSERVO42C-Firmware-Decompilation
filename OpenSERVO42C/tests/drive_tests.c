#include "drive.h"
#include <assert.h>
#include <stdio.h>
static const os_drive_config cfg={16,1000,500,2048,300,25,false,600};
static const os_offsets offsets={2048,2048};
static void setup(os_drive *d){
    assert(os_drive_init(d,&cfg,&offsets));
    assert(!os_drive_arm(d));
    for(unsigned i=0;i<100;i++)os_drive_position(d,65530,true,65534,true,false,false);
    assert(os_drive_arm(d));
}
static bool inactive(os_pwm_compare p){return p.a0==1200&&p.a1==1200&&p.b0==1200&&p.b1==1200;}
int main(void){
    os_drive d;os_pwm_compare p;
    setup(&d);
    os_drive_position(&d,4,true,0,true,false,false);
    assert(d.control.latest.actual==10&&d.pulse_target==40);
    assert(os_drive_current(&d,true,true,4,true,2000,2048,2048,&p)&&!inactive(p));
    os_drive_position(&d,65530,true,65534,true,false,false);
    assert(d.pulse_target==0&&d.remainder==0);
    assert(os_drive_current(&d,true,true,4,true,2000,1500,2048,&p));
    assert(!os_drive_current(&d,true,true,4,true,2000,1400,2048,&p)&&inactive(p));
    assert(!os_drive_current(&d,true,true,4,true,2000,2048,2048,&p)&&inactive(p));
    os_drive_position(&d,65530,true,65534,true,false,false);
    assert(d.control.state==OS_FAULT&&(d.control.faults&OS_F_ELECTRICAL));
    assert(!os_drive_arm(&d));
    assert(!os_drive_confirm_reference(&d,0,65530,65534));
    for(unsigned i=0;i<100;i++)os_drive_position(&d,65530,true,65534,false,false,false);
    assert(os_acknowledge(&d.control));assert(d.control.position_lost);
    assert(!os_drive_confirm_reference(&d,0,65531,65534));
    assert(os_drive_confirm_reference(&d,1000,65530,65534));assert(!d.control.position_lost);
    assert(d.encoder.position==1000&&d.pulse_target==1000);
    os_drive_position(&d,65530,true,65534,true,false,false);assert(os_drive_arm(&d));
    setup(&d);os_drive_position(&d,0,false,65534,true,false,false);
    assert(d.control.state==OS_FAULT&&!os_drive_current(&d,true,true,4,true,2000,2048,2048,&p));
    setup(&d);os_drive_position(&d,65530,true,65534,false,false,false);
    assert(d.control.state==OS_OFF&&!os_drive_current(&d,true,true,4,true,2000,2048,2048,&p));
    setup(&d);
    assert(!os_drive_current(&d,false,true,65530,true,2000,2048,2048,&p));
    assert(!os_drive_current(&d,true,true,65530,true,2000,2048,2048,&p));
    os_drive_position(&d,65530,true,65534,true,false,false);
    assert(d.control.state==OS_OFF);
    setup(&d);assert(!os_drive_current(&d,true,false,65530,true,2000,2048,2048,&p));
    os_drive_position(&d,65530,true,65534,true,false,false);assert(d.control.faults&OS_F_ENCODER);
    os_drive_config invalid=cfg;invalid.geometry=0;
    assert(!os_drive_init(&d,&invalid,&offsets));assert(!os_drive_arm(&d));
    puts("drive integration passed");return 0;
}

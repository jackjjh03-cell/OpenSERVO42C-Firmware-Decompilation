#include "drive.h"
#include "phase.h"
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
    assert(os_drive_init(&d,&cfg,&offsets));assert(os_select_mode(&d.control,OS_OPEN_LOOP));
    for(unsigned i=0;i<100;i++)os_drive_position(&d,1000,true,0,true,true,false);
    assert(os_drive_arm(&d));
    assert(os_drive_current(&d,true,true,1000,true,2000,2048,2048,&p)&&!inactive(p));
    uint16_t phase=d.phase;assert(phase==os_electrical_phase(1000,1000,false,0,25));
    os_drive_position(&d,1100,true,0,true,true,false);
    assert(os_drive_current(&d,true,true,1100,true,2000,2048,2048,&p)&&d.phase==phase);
    os_drive_position(&d,1100,true,16,true,true,false);
    assert(d.pulse_target==327&&d.control.demand==500);
    assert(os_drive_current(&d,true,true,1100,true,2000,2048,2048,&p));
    assert(d.phase==os_electrical_phase(1327,1327,false,0,25));
    os_drive_position(&d,1100,true,16,true,false,false);assert(d.control.faults&OS_F_REFERENCE);
    assert(!os_drive_current(&d,true,true,1100,true,2000,2048,2048,&p)&&inactive(p));
    /* Compare the optimized pulse path to the original 64-bit calculation,
     * including a remainder larger than one pulse and signed wraparound. */
    for(unsigned geometry=25;geometry<=50;geometry+=25){
        for(unsigned microsteps=1;microsteps<=256;microsteps*=2){
            for(unsigned reverse=0;reverse<2;reverse++){
                os_drive_config c=cfg;c.geometry=(uint8_t)geometry;c.microsteps=(uint16_t)microsteps;c.reverse=reverse!=0;
                int32_t denom=(int32_t)(geometry*8u*microsteps);
                const int32_t deltas[]={-32767,-1000,-1,0,1,1000,32767};
                const int32_t rems[]={1-denom,0,denom-1};
                for(unsigned i=0;i<sizeof deltas/sizeof deltas[0];i++)for(unsigned j=0;j<3;j++){
                    assert(os_drive_init(&d,&c,&offsets));
                    d.control.state=OS_ACTIVE;d.control.mode=OS_CNC;
                    d.previous_pulse=65530;d.pulse_target=1234;d.remainder=rems[j];
                    int64_t num=(int64_t)(reverse?-deltas[i]:deltas[i])*65536+rems[j];
                    os_drive_position(&d,0,true,(uint16_t)(65530+deltas[i]),true,true,false);
                    assert(d.pulse_target==1234+num/denom&&d.remainder==num%denom);
                }
            }
        }
    }
    for(unsigned i=0;i<2;i++){
        assert(os_drive_init(&d,&cfg,&offsets));d.control.state=OS_ACTIVE;d.control.mode=OS_CNC;
        d.pulse_target=i?INT32_MAX:INT32_MIN;d.previous_pulse=0;
        os_drive_position(&d,0,true,i?1u:65535u,true,true,false);
        assert(d.control.faults&OS_F_TIMING);
    }
    puts("drive integration passed");return 0;
}

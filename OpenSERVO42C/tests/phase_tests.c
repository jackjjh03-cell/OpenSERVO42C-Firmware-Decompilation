#include "phase.h"
#include <assert.h>
#include <stdio.h>
static uint16_t lut[OS_LUT_ENTRIES];
int main(void){
    uint16_t angle=123;
    lut[0]=0;lut[16383]=65535;
    assert(!os_calibrated_angle(lut,0,1,0,&angle)&&angle==123);
    assert(!os_calibrated_angle(lut,1,0,0,&angle));
    assert(!os_calibrated_angle(lut,1,1,16384,&angle));
    assert(os_calibrated_angle(lut,1,1,0,&angle)&&angle==0);
    assert(os_calibrated_angle(lut,1,1,16383,&angle)&&angle==65535);
    assert(os_electrical_phase(0,0,false,0,50)==0);
    assert(os_electrical_phase(10,65530,true,0,50)==41);
    assert(os_electrical_phase(65530,10,true,0,50)==990);
    assert(os_electrical_phase(100,100,true,INT32_MAX,50)==233);
    assert(os_electrical_phase(100,100,true,INT32_MIN,50)==80);
    assert(os_sine_q14(0)==0&&os_sine_q14(256)==16384&&os_sine_q14(512)==0&&os_sine_q14(768)==-16384);
    for(unsigned i=0;i<1024;i++){
        assert(os_sine_q14((uint16_t)(i+512))==-os_sine_q14((uint16_t)i));
        assert(os_sine_q14((uint16_t)(i+1024))==os_sine_q14((uint16_t)i));
    }
    puts("phase tests passed");return 0;
}

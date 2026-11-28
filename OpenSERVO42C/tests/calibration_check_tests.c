#include "calibration_check.h"
#include "phase.h"
#include <assert.h>
#include <stdio.h>
static uint16_t lut[OS_LUT_ENTRIES];
int main(void){
    for(unsigned i=0;i<OS_LUT_ENTRIES;i++)lut[i]=(uint16_t)(i*4u+12345u);
    os_calibration_check c=os_check_calibration(lut,1,1);
    assert(c.flags==3&&c.winding==65536&&c.max_delta==4&&c.forward==16384);
    assert(!os_check_calibration(lut,1,0).flags);
    for(unsigned i=0;i<OS_LUT_ENTRIES;i++)lut[i]=(uint16_t)(12345u-i*4u);
    c=os_check_calibration(lut,1,1);assert(c.flags==3&&c.winding==-65536&&c.reverse==16384);
    lut[10]=(uint16_t)(lut[9]+32768u);c=os_check_calibration(lut,1,1);assert(c.flags&OS_CAL_AMBIGUOUS);
    for(unsigned i=0;i<OS_LUT_ENTRIES;i++)lut[i]=65535u;
    c=os_check_calibration(lut,1,1);assert(c.flags==1&&c.flat==16384&&c.winding==0);
    puts("calibration check passed");return 0;
}

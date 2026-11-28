#include "calibration_check.h"
#include "phase.h"
os_calibration_check os_check_calibration(const uint16_t *lut,uint16_t knots,uint16_t valid){
    os_calibration_check c={0};
    if(!lut||knots!=1u||valid!=1u)return c;
    c.flags=OS_CAL_MARKERS;
    uint16_t prev=lut[OS_LUT_ENTRIES-1u];
    for(unsigned i=0;i<OS_LUT_ENTRIES;i++){
        uint16_t cur=lut[i],delta=(uint16_t)(cur-prev);prev=cur;
        int32_t d=delta<32768u?(int32_t)delta:(int32_t)delta-65536;
        uint16_t mag=(uint16_t)(d<0?-d:d);
        if(mag>c.max_delta)c.max_delta=mag;
        if(delta==32768u)c.flags|=OS_CAL_AMBIGUOUS;
        if(d>0)c.forward++;else if(d<0)c.reverse++;else c.flat++;
        c.winding+=d;
    }
    if(c.winding==65536||c.winding==-65536)c.flags|=OS_CAL_WINDING;
    return c;
}

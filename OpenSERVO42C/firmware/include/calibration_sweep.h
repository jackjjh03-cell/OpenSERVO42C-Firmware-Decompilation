#ifndef OS_CALIBRATION_SWEEP_H
#define OS_CALIBRATION_SWEEP_H
#include "run.h"
#include "protocol.h"
enum {OS_CAL_START=40,OS_CAL_STEP=41,OS_CAL_STATUS=42,OS_CAL_STOP=43};
enum {OS_CAL_ACTIVE=1,OS_CAL_READY=2,OS_CAL_DONE=4,OS_CAL_ABORTED=8};
typedef struct {
    uint32_t last_tick,deadline;
    int32_t sum;
    uint16_t index,raw,base,previous,phase,current,wait;
    uint8_t flags,remaining,samples;
} os_cal_sweep;
void os_cal_stop(os_cal_sweep *s,os_run *r,const os_board_io *io,bool completed);
void os_cal_tick(os_cal_sweep *s,os_run *r,const os_board_io *io,uint32_t now,bool fresh,bool raw_valid,uint16_t raw);
void os_cal_adc(os_cal_sweep *s,os_run *r,const os_board_io *io,bool enable,bool raw_valid,bool valid,uint16_t scale,uint16_t slot2,uint16_t slot1);
size_t os_cal_dispatch(os_cal_sweep *s,os_run *r,const os_board_io *io,uint32_t now,bool raw_valid,uint16_t raw,const uint8_t *in,size_t n,uint8_t *out);
#endif

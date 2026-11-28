#ifndef OS_RUN_H
#define OS_RUN_H
#include "drive.h"
#include "output_gate.h"
/* Single control owner. The target must serialize these calls and provide a
 * monotonic 1 ms clock, fresh DMA completions and calibrated encoder samples. */
typedef struct {
    os_drive drive;
    os_output_gate gate;
    uint32_t last_tick, adc_count, last_count, missed_ticks;
    bool tick_seen, adc_seen, adc_good, enable;
} os_run;
bool os_run_init(os_run *r,const os_board_io *io,const os_drive_config *cfg,const os_offsets *offsets);
void os_run_tick(os_run *r,const os_board_io *io,uint32_t now,uint16_t angle,bool valid,uint16_t pulse,bool enable,bool reference,bool travel);
void os_run_adc(os_run *r,const os_board_io *io,bool enable,bool angle_valid,uint16_t angle,bool valid,uint16_t scale,uint16_t slot2,uint16_t slot1);
bool os_run_arm(os_run *r,const os_board_io *io);
void os_run_disable(os_run *r,const os_board_io *io);
/* Edge-driven hardware disable: never opens the gate on a rising edge. */
void os_run_enable(os_run *r,const os_board_io *io,bool enable);
#endif

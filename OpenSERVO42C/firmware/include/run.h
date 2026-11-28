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
    bool tick_seen, adc_seen, adc_good, enable, reference_confirmed;
    bool teaching;
    uint8_t captured;
    int32_t taught_min, taught_max;
} os_run;
bool os_run_init(os_run *r,const os_board_io *io,const os_drive_config *cfg,const os_offsets *offsets);
void os_run_tick(os_run *r,const os_board_io *io,uint32_t now,uint16_t angle,bool valid,uint16_t pulse,bool enable,bool reference,bool travel);
void os_run_adc(os_run *r,const os_board_io *io,bool enable,bool angle_valid,uint16_t angle,bool valid,uint16_t scale,uint16_t slot2,uint16_t slot1);
bool os_run_arm(os_run *r,const os_board_io *io);
bool os_run_confirm_reference(os_run *r,int32_t position);
void os_run_disable(os_run *r,const os_board_io *io);
bool os_run_teach_begin(os_run *r,const os_board_io *io);
bool os_run_teach_capture(os_run *r,bool maximum,int32_t *position);
bool os_run_teach_apply(os_run *r,uint32_t lower_margin,uint32_t upper_margin);
void os_run_teach_cancel(os_run *r);
/* Edge-driven hardware disable: never opens the gate on a rising edge. */
void os_run_enable(os_run *r,const os_board_io *io,bool enable);
#endif

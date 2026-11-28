#ifndef OS_OUTPUT_GATE_H
#define OS_OUTPUT_GATE_H
#include "board.h"
/* Caller serializes all gate and compare accesses, and ensures the timer/ADC
 * setup, current calibration, EN polarity and hardware readiness are valid. */
typedef struct { bool enabled; } os_output_gate;
void os_gate_close(os_output_gate *g,const os_board_io *io);
bool os_gate_open(os_output_gate *g,const os_board_io *io,bool permitted);
bool os_gate_update(os_output_gate *g,const os_board_io *io,bool permitted,const os_pwm_compare *p);
#endif

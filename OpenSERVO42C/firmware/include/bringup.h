#ifndef OS_BRINGUP_H
#define OS_BRINGUP_H
#include "protocol.h"
/* Wire opcode 0x0c: read-only hardware bring-up snapshot. All currents raw. */
#define OS_GET_BENCH 12u
typedef struct {
    uint32_t ms,core_hz,adc_samples,adc_errors,encoder_errors,missed_ticks,uart_dropped;
    uint16_t scale,slot1,slot2,raw_angle,cal_angle,pulse,offset1,offset2;
    uint8_t board_status,encoder_status,flags;
} os_bench;
/* flags: bit0 calibrated angle valid, bit1 raw angle valid,
 * bit2 electrical enable input level (not interpreted), bit3 ADC sample valid. */
size_t os_bench_dispatch(os_controller *c,const os_bench *b,const uint8_t *in,size_t n,uint8_t *out);
#endif

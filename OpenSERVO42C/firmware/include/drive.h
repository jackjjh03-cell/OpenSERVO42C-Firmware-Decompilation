#ifndef OS_DRIVE_H
#define OS_DRIVE_H
#include "controller.h"
#include "current.h"
#include "encoder.h"
/* All current values are ADC counts, not milliamps. The board integration
 * must establish these limits before commissioning. All calls serialized by
 * the target; a fast interrupt must not race foreground configuration. */
typedef struct {
    uint16_t microsteps, max_encoder_delta, phase_limit, current_gain_q11, compare_limit;
    uint8_t geometry; /* original geometry: 25 for 1.8 degrees, 50 for 0.9 */
    bool reverse;
    uint16_t trip_limit; /* independent measured peak cutoff; raw ADC counts */
} os_drive_config;
typedef struct {
    os_controller control;
    os_drive_config cfg;
    os_offsets offsets;
    os_encoder_tracker encoder;
    int32_t pulse_target, remainder;
    uint16_t previous_pulse, phase, previous_angle, load, open_offset;
    bool configured, phase_valid, fast_phase_valid, inhibit_requested;
    uint8_t pending_fault;
} os_drive;
bool os_drive_config_valid(const os_drive_config *cfg);
bool os_drive_init(os_drive *d,const os_drive_config *cfg,const os_offsets *offsets);
/* The target calls this with a fresh calibrated angle every 1 ms. */
void os_drive_position(os_drive *d,uint16_t angle,bool angle_valid,uint16_t pulse,bool enable,bool reference,bool travel);
/* Explicit arming after a healthy sample window; target owns actual output gate. */
bool os_drive_arm(os_drive *d);
/* Operator confirms a known mechanical reference with EN off after recovery. */
bool os_drive_confirm_reference(os_drive *d,int32_t position,uint16_t angle,uint16_t pulse);
void os_drive_disable(os_drive *d);
/* Called on every fresh ADC sample. False requires immediate output inhibit.
 * No MMIO in this module. A critical fault remains latched across subsequent samples. */
/* Split path: validate every ADC conversion once, then regulate only with
 * feedback from the same successful validation. Calls must be serialized. */
bool os_drive_feedback(os_drive *d,bool adc_valid,uint16_t scale,uint16_t slot2,uint16_t slot1,os_pair *feedback);
bool os_drive_current_checked(os_drive *d,bool enable,bool angle_valid,uint16_t angle,uint16_t scale,os_pair feedback,os_pwm_compare *p);
bool os_drive_current(os_drive *d,bool enable,bool angle_valid,uint16_t angle,bool adc_valid,
                      uint16_t scale,uint16_t slot2,uint16_t slot1,os_pwm_compare *p);
#endif

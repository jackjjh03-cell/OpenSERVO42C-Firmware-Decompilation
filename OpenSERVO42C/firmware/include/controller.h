#ifndef OPEN_CONTROLLER_H
#define OPEN_CONTROLLER_H
#include <stdint.h>
#include <stdbool.h>
#include "pid.h"
#include "motion.h"
typedef enum { OS_OFF, OS_READY, OS_ACTIVE, OS_FAULT } os_state;
typedef enum { OS_MONITOR, OS_WARN, OS_PROTECT } os_policy;
typedef enum { OS_CNC, OS_POSITION, OS_VELOCITY, OS_JOG, OS_TORQUE, OS_OPEN_LOOP } os_mode;
enum { OS_F_STALL=1u, OS_F_FOLLOW=2u, OS_F_TRAVEL=4u,
       OS_F_ENCODER=8u, OS_F_ELECTRICAL=16u, OS_F_TIMING=32u, OS_F_REFERENCE=64u };
#define OS_CRITICAL (OS_F_ENCODER|OS_F_ELECTRICAL|OS_F_TIMING|OS_F_REFERENCE)
typedef struct {
    uint32_t following_counts, stall_error_counts, stall_speed_counts_s;
    uint16_t stall_load_permille, dwell_ms, clear_ms;
    uint8_t policy;
} os_config;
typedef struct {
    int32_t target, actual, speed_counts_s;
    uint16_t load_permille;
    uint8_t critical; /* trusted HAL only; not a UART input */
    bool reference_valid, travel_violation, enable_input, feedback_valid;
} os_sample;
typedef struct { int32_t minimum,maximum; bool enabled; } os_limits;
typedef struct {
    os_config cfg;
    os_limits limits;
    os_pid_config live_pid_cfg;
    bool live_pid;
    os_pid_config pid_cfg;
    os_motion_config motion_cfg;
    os_pid pid;
    os_motion motion;
    os_sample latest, fault_sample;
    uint32_t faults, observed, warnings;
    uint16_t stall_ms, follow_ms, healthy_ms, still_ms;
    int32_t command, demand;
    os_state state;
    os_mode mode;
    bool hal_validated, position_lost, moving;
} os_controller;
void os_init(os_controller *c);
bool os_config_valid(const os_config *v);
bool os_configure(os_controller *c, const os_config *v);
bool os_set_pid(os_controller *c,const os_pid_config *v);
bool os_set_live_pid(os_controller *c,const os_pid_config *v);
bool os_set_limits(os_controller *c,const os_limits *v);
bool os_set_motion(os_controller *c,const os_motion_config *v);
bool os_select_mode(os_controller *c, os_mode m);
bool os_arm(os_controller *c);
void os_disable(os_controller *c);
bool os_start_move(os_controller *c,int32_t end);
void os_tick_1ms(os_controller *c, const os_sample *s);
bool os_acknowledge(os_controller *c);
#endif

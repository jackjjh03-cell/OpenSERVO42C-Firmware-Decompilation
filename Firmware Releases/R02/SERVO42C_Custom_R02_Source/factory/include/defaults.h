#ifndef SERVO42C_DEFAULTS_H
#define SERVO42C_DEFAULTS_H
/* Default settings for blank or invalid storage and factory restore. Valid
 * saved records take precedence. Motor and PID constants are raw firmware
 * values. */
#define DEFAULT_KP                   1638u
#define DEFAULT_KI                   1u
#define DEFAULT_KD                   1638u
#define DEFAULT_MICROSTEPS           16u
#define DEFAULT_ALTERNATE_MICROSTEPS 0u
#define DEFAULT_DIRECTION            0u
#define DEFAULT_ERROR_LIMIT_RAW      3276u
#define DEFAULT_ENABLE_MODE          0u
#define DEFAULT_CONTROL_MODE         1u
#define DEFAULT_AUTOSDD              0u
#define DEFAULT_PROTECTION           0u
#define DEFAULT_INTERPOLATION        1u
#define DEFAULT_UART_BAUD_RAW        0u
#define DEFAULT_UART_ADDRESS         0xE0u
#define DEFAULT_MOTION_SAVED_RAW     0xFFFFu
#define DEFAULT_ACCELERATION_INDEX   0u
#define DEFAULT_TORQUE_RAW           1080u
#define DEFAULT_ACCELERATION_RAW     286u
#define DEFAULT_ORIGIN_MODE          0u
#define DEFAULT_ORIGIN_POSITION      0xFFFFu
#define DEFAULT_ORIGIN_SPEED_INDEX   2u
#define DEFAULT_ORIGIN_DIRECTION     0u
#define DEFAULT_MOTOR_GEOMETRY       25u
#define DEFAULT_HALF_CURRENT_RAW     600u
#define DEFAULT_CURRENT_RAW          1200u
/* Only enforce confirmed basic representation constraints; these are not
 * physical limits or a substitute for testing new motor parameters. */
_Static_assert(DEFAULT_UART_ADDRESS>=0xe0u && DEFAULT_UART_ADDRESS<=0xe9u,"UART address outside original menu range");
_Static_assert(DEFAULT_MICROSTEPS>0 && DEFAULT_MICROSTEPS<=256,"invalid default microsteps");
_Static_assert(DEFAULT_MOTOR_GEOMETRY==25 || DEFAULT_MOTOR_GEOMETRY==50,"invalid motor geometry");
#endif

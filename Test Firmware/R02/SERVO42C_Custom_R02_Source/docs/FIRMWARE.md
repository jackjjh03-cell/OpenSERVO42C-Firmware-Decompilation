# Firmware reference

## Modules

| Area | Source modules |
|---|---|
| Startup and clocks | `reset.c`, `application_flow.c`, `board_startup.c`, `clock_and_bus.c`, `startup_prepare.c`, `boot_services.c`, `boot_validation.c` |
| Motor control | `motion_controller.c`, `motion_state.c`, `motor_math.c`, `current_feedback.c`, `mode_and_position.c`, `origin.c` |
| Encoder and calibration | `encoder.c`, `encoder_io.c`, `calibration_work.c` |
| UART | `uart.c`, `commands.c`, `auxiliary_timer.c` |
| Display and menu | `display.c`, `application_ui.c`, `menu.c`, `menu_navigation.c`, `telemetry.c`, `layout_assets.c` |
| Persistent storage | `settings.c`, `flash_driver.c`, `identity.c` |
| Hardware and runtime support | `board_io.c`, `board_timer_display.c`, `peripherals.c`, `pin_config.c`, `register_helpers.c`, `arithmetic_runtime.c`, `runtime_memory.c`, `layout_startup_data.c` and supporting modules |

## Main flash (64 KiB)

| Address range | Purpose |
|---|---|
| `0x0000–0x79FF` | Application build region |
| `0x7A00–0x7BFF` | Reserved identity page |
| `0x7C00–0x7DFF` | Settings, eight 64-byte records |
| `0x7E00–0x7FFF` | Encoder calibration knots and metadata |
| `0x8000–0xFFFF` | Encoder lookup table, 16,384 halfwords |

The entire identity page is reserved in the C linker even though the original executable's compressed startup data extended through `0x7A43`. The C startup uses relocated initialized-data assets. Keep reserved storage local to its original device.

## Settings and UART

Default values are defined in `defaults.h`; menu callbacks are implemented in `menu.c`. A callback argument of 255 queries its current setting. Other values select a valid menu option. The settings loader selects the last non-erased record and then checks its validity marker. A blank or invalid selected record triggers fallback defaults. It does not necessarily select an older valid record.

Default control mode is CR_vFOC (1), baud is 0 (UART disabled), address is `0xE0`, direction is CW, and raw PID gains are Kp=1638, Ki=1, Kd=1638. The baud menu retains the legacy behavior: selecting option 0 uses 38400 even though the menu label reads Disable; a zero stored baud is queried as option 0. Do not confuse this menu behavior with initial defaults.

## Implementation constraints

- Shared workspace fields are referenced by fixed offsets. Maintain their layout when editing structures or startup data.
- Preserve ordered volatile register accesses, fixed-point rounding and machine-word wrapping.
- OLED rows accept supported glyphs and at most 10 characters.
- Flash wrappers discard underlying erase/program status, consistent with the legacy behavior; this is not a robust power-loss recovery policy.
- Physical register semantics, loop timing, interrupt nesting and some raw parameter units require further hardware validation.

## R02 startup lockout and RPM

The volatile `magnet_lock` latch is separate from the original workspace. On a detected startup encoder fault, the firmware disables the step/ADC/motion/UART interrupt paths, uses the factory disable sequence, then enters the full settings-only menu. The latch lasts until a reset; settings commits do not apply a new motion mode or resume control while locked. UART is intentionally unavailable in this state. `identity.c` and the original device-ID comparison remain unchanged. No new flash layout or default values are introduced.

RPM uses the calibrated feedback low word and elapsed core clock cycles, sampled in foreground. Resetting motor position invalidates the estimate without changing the SysTick timer from an ISR. Next switches between RPM and the original pulse counter. Unavailable estimates show `RPM --`.

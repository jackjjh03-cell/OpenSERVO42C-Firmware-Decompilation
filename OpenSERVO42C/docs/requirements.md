# Development requirements
- General purpose with dedicated CNC Step/Dir; preserve external trajectory and explicit control ownership.
- Open/closed-loop Step/Dir, standalone position/velocity/jog, torque only after current validation.
- Both trapezoidal and physical jerk-limited S-curve, editable speed, acceleration, deceleration and jerk in Studio.
- Live PID with raw gains, understandable simplified controls and real response plots; PC-guided precision/balanced/speed/quiet/custom tuning.
- Adjustable hold current/deadband, calibrated current telemetry, travel limits only with valid reference, diagnostic snapshots.
- Independent mandatory critical shutdown; noncritical monitor/warn/protect with persistence and hysteresis. Manual or explicitly bounded recovery by eligible fault; no automatic CNC resume after position loss.
- Essential OLED settings, advanced settings on PC; one active onboard config, PC JSON profiles. Preserve board-specific identity/calibration.
- Retain selected V1.1.3 UART queries and later verified commands; versioned extended protocol, capability discovery, bounded parsing, no writes to flash on live tuning.
- Dark Studio, Windows 7 (Python 3.8-compatible), Windows 10/11, Linux where practical. Live plots, CSV, terminal, read-only engineering diagnostics; external flashing initially.

## Gates
1. Portable core and protocol (this increment); verify host tests.
2. Stock HAL with bridge inhibited, exact register/pin audit, clock and measured ISR budget; link and report app/RAM sizes.
3. Current/position loop and encoder calibration, bounded bench tests; validated critical protection and fault output if stock board supports it.
4. Studio transport, simulated controller and UI alongside hardware integration; tune and compare measured responses.
5. Standalone trajectory planner, guided tuning, persistent configuration migration and recovery validation.
No build is a hardware safety certification. Development test images may be provided for staged bench validation after source-level review, backup and restore preparation, and initial output-inhibit checks. Do not label them as hardware-validated before testing.

Use [memory and timing budgets](memory-and-timing.md) and the [bench procedure](bench-validation.md). Implement all supported functionality rather than leaving arbitrary feature locks; only hardware-dependent enable paths and features that cannot be validated on the stock board require restrictions.

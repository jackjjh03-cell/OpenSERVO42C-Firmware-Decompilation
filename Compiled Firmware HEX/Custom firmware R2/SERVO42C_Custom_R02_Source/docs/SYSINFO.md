# SysInfo diagnostic selector reference

Command 0x8D is a Custom R01/R02 extension in the factory four-byte request group. Selectors below are decimal. All responses are unsigned 32-bit raw values, most zero-extended from byte or halfword fields. Where an OLED number would exceed its available width, raw hex is used. Motion fields show the low word of the corresponding value, not an atomic full 64-bit position. `Part` shows the Cortex CPUID part field (decimal), `Div` the clock divider, `Src` the clock source index. These register reads are specifically chosen; no arbitrary peripheral-register access is exposed.

| ID | Field | OLED label | Description |
|---:|---|---|---|
| 0 | Control mode | `Mode` | Active control mode: 0 = CR_OPEN (open loop), 1 = CR_vFOC (closed loop with step/direction), 2 = CR_UART (closed loop with serial motion control). |
| 1 | UART baud | `Bd` | Configured UART baud rate in bits per second. A stored zero disables UART initialization; this is not a measurement of the physical line speed. |
| 2 | UART address | `Addr` | Controller address used in UART packets, normally 0xE0–0xE9. Shown as a decimal value on the OLED. |
| 3 | Microsteps | `Step` | Active microstep subdivision per full motor step. Higher values require more command pulses for the same movement. |
| 4 | Current raw | `Ma` | Configured motor current setting in raw firmware units; not a live measurement of winding current. |
| 5 | Direction | `Dir` | Configured motor direction: 0 = CW, 1 = CCW. |
| 6 | Enable mode | `En` | Enable-input behavior: 0 = L (active low), 1 = H (active high), 2 = Hold. |
| 7 | Protection | `Prot` | Stall protection setting: 0 = disabled, 1 = enabled. This is the configuration, not the current fault state. |
| 8 | Interpolation | `Mply` | Step interpolation setting (Mplyer): 0 = off, 1 = on. Smooths intermediate position targets in the control loop. |
| 9 | Auto display sleep | `SDD` | AutoSDD setting: 0 = disabled, 1 = enabled. Controls the automatic display sleep feature. |
| 10 | Alternate microsteps | `Alt` | Stored custom microstep value used by the alternate microstep menu entry. Zero indicates no custom value. |
| 11 | Motor geometry raw | `Geom` | Internal motor geometry factor: 25 for a 1.8° motor (200 full steps/rev), 50 for a 0.9° motor (400 full steps/rev). Used in position scaling. |
| 12 | Acceleration raw | `ACC` | Raw acceleration parameter used by the UART motion ramp. Factory fallback is 286; not a calibrated acceleration in physical units. |
| 13 | Torque raw | `Trq` | Raw torque/PWM command limit used by the current-control routine. Factory fallback is 1080; not measured shaft torque. |
| 14 | Error limit raw | `Lim` | Maximum magnitude of the internal position error used by the PID calculation. Factory fallback is 3276 raw units. |
| 15 | Origin mode | `0Mod` | Origin return mode: 0 = disabled, 1 = DirMode (direction-based), 2 = NearMode (nearest origin). |
| 16 | Origin speed index | `0Spd` | Selected origin-return speed index (0–4; default 2). It is a menu index, not RPM. |
| 17 | Origin direction | `0Dir` | Configured origin-return direction: 0 = CW, 1 = CCW. |
| 18 | Origin position raw | `0Pos` | Stored origin reference in the firmware's internal position units. 65535 (0xFFFF) indicates no valid origin. |
| 19 | Saved motion raw | `Save` | Packed saved UART motion setting: bit 15 holds direction, bits 0–14 hold raw speed. 65535 (0xFFFF) means no saved motion. A valid record may be restored at startup in CR_UART mode. |
| 20 | Acceleration index | `ACix` | Selected acceleration menu index (0–7). This is distinct from the raw acceleration parameter in selector 12. |
| 21 | Kp raw | `Kp` | Proportional PID gain. Scales the response to the current position error. Factory fallback 1638. |
| 22 | Ki raw | `Ki` | Integral PID gain. Scales the accumulated position error to reduce persistent error. Factory fallback 1. |
| 23 | Kd raw | `Kd` | Derivative PID gain. Scales the change in position error between control updates. Factory fallback 1638. |
| 24 | Drive enabled | `Drv` | Firmware drive-enable flag: 0 = disabled, 1 = enabled. Not an independent indication that motor power is isolated. |
| 25 | UART motion state | `Run` | UART motion state: 0 = idle, 1 = continuous motion, 2 = finite motion. Internal state rather than measured movement. |
| 26 | Stall flag | `Stal` | Live stall-detection flag: 0 = clear, 1 = detected. Whether output is shut down also depends on the protection setting. |
| 27 | Position error (raw 32-bit) | `E` | Signed, limited position error used by the control loop (interpolated target minus encoder feedback), returned as raw 32-bit two's-complement hex. |
| 28 | Pulse position low word | `P` | Low 32 bits of the accumulated command pulse count. Can wrap; does not represent a complete multi-turn position. |
| 29 | Target low word | `T` | Low 32 bits of the internal target position derived from the pulse count and motor scaling. Not the actual shaft position. |
| 30 | UART speed raw | `Spd` | Raw requested UART motion speed. Not a measured RPM value; see the main telemetry RPM readout for estimated shaft speed. |
| 31 | UART enable request | `Req` | UART drive-enable request flag. Indicates the requested state, which may differ from the applied drive-enable flag (selector 24). |
| 32 | Control output (raw 32-bit) | `U` | Signed output of the position PID loop, limited to ±8000 raw units and returned as two's-complement hex. Not measured torque or current. |
| 33 | Encoder angle raw | `Ang` | Latest calibrated encoder angle held by the motion-feedback routine, in raw internal angle units. May be stale when feedback is not updating. |
| 34 | Encoder status | `Stat` | Encoder startup diagnostic status: 0 = no recorded error, 3 = encoder fault bit detected, 4 = zero status after retry. Not a continuous magnet-health measurement. |
| 35 | Encoder LUT ready | `LUT` | RAM flag indicating that the encoder calibration lookup table (LUT) is available for use: 0 = not ready, 1 = ready. |
| 36 | Knot valid | `Knot` | Calibration knot validity flag in RAM. A value of 1 marks the calibration reference points as valid. |
| 37 | LUT valid | `LCal` | Calibration LUT validity flag in RAM. A value of 1 marks the stored encoder lookup table as valid; not a measure of calibration accuracy. |
| 38 | Feedback position low word | `F` | Low 32 bits of the firmware feedback position field. Raw position data; may wrap and is not a full 64-bit snapshot. |
| 39 | Startup state | `Boot` | Internal startup/feedback state byte used during initialization. It is not the processor reset cause or an independent hardware fault code. |
| 40 | Stall flag (faults) | `Stal` | Same live stall-detection flag as selector 26, repeated in the Faults category. |
| 41 | Protection (faults) | `Prot` | Same protection configuration as selector 7, repeated in the Faults category. |
| 42 | ADC feedback 1 raw | `A` | Raw ADC feedback sample 1 used by current sensing. Offset and scaling are applied elsewhere; not directly an amperage reading. |
| 43 | ADC feedback 2 raw | `B` | Raw ADC feedback sample 2 used by current sensing. Offset and scaling are applied elsewhere; not directly an amperage reading. |
| 44 | Phase index | `Phas` | Internal electrical phase index used by the motor-control routines (0–1023). Not the mechanical shaft angle in degrees. |

## Reading the values

- The table reports firmware RAM fields, not independent hardware measurements. Values can be stale while the motor is disabled, in open-loop operation or during startup lockout. UART is disabled during the R02 startup encoder-fault lockout.
- Factory fallback values are reference settings, not recommended tuning for every motor. SysInfo itself is read-only.
- `Part`, `Div` and `Src` are OLED-only register information: CPU identification part number, clock divider factor and clock source index (4 = PLL). They are not additional `0x8D` selectors.
- The optional Perf page reports approximate motion-interrupt frequency (`Hz`), workload (`Load`) and peak sampled cycles (`C`), not total CPU utilization. Memory page stack budget is a linked allocation, not measured peak stack usage.

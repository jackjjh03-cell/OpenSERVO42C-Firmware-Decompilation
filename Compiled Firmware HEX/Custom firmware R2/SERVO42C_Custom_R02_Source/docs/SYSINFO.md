# SysInfo diagnostic selector reference

Command 0x8D is a Custom R01/R02 extension in the factory four-byte request group. Selectors below are decimal. All responses are unsigned 32-bit raw values, most zero-extended from byte or halfword fields. Where an OLED number would exceed its available width, raw hex is used. Motion fields show the low word of the corresponding value, not an atomic full 64-bit position. `Part` shows the Cortex CPUID part field (decimal), `Div` the clock divider, `Src` the clock source index. These register reads are specifically chosen; no arbitrary peripheral-register access is exposed.

| ID | Field | OLED label |
|---:|---|---|
| 0 | Control mode | `Mode` |
| 1 | UART baud | `Bd` |
| 2 | UART address | `Addr` |
| 3 | Microsteps | `Step` |
| 4 | Current raw | `Ma` |
| 5 | Direction | `Dir` |
| 6 | Enable mode | `En` |
| 7 | Protection | `Prot` |
| 8 | Interpolation | `Mply` |
| 9 | Auto display sleep | `SDD` |
| 10 | Alternate microsteps | `Alt` |
| 11 | Motor geometry raw | `Geom` |
| 12 | Acceleration raw | `ACC` |
| 13 | Torque raw | `Trq` |
| 14 | Error limit raw | `Lim` |
| 15 | Origin mode | `0Mod` |
| 16 | Origin speed index | `0Spd` |
| 17 | Origin direction | `0Dir` |
| 18 | Origin position raw | `0Pos` |
| 19 | Saved motion raw | `Save` |
| 20 | Acceleration index | `ACix` |
| 21 | Kp raw | `Kp` |
| 22 | Ki raw | `Ki` |
| 23 | Kd raw | `Kd` |
| 24 | Drive enabled | `Drv` |
| 25 | UART motion state | `Run` |
| 26 | Stall flag | `Stal` |
| 27 | Position error (raw 32-bit) | `E` |
| 28 | Pulse position low word | `P` |
| 29 | Target low word | `T` |
| 30 | UART speed raw | `Spd` |
| 31 | UART enable request | `Req` |
| 32 | Control output (raw 32-bit) | `U` |
| 33 | Encoder angle raw | `Ang` |
| 34 | Encoder status | `Stat` |
| 35 | Encoder LUT ready | `LUT` |
| 36 | Knot valid | `Knot` |
| 37 | LUT valid | `LCal` |
| 38 | Feedback position low word | `F` |
| 39 | Startup state | `Boot` |
| 40 | Stall flag (faults) | `Stal` |
| 41 | Protection (faults) | `Prot` |
| 42 | ADC feedback 1 raw | `A` |
| 43 | ADC feedback 2 raw | `B` |
| 44 | Phase index | `Phas` |

# Extended UART wire contract v1 (prototype)
Separate from original V1.1.3 additive checksum protocol. No legacy handler or physical UART integration yet. All multibyte fields little-endian.
Frame: A5 5A | version (1) | sequence | opcode | payload length (0..48) | payload | CRC16-CCITT-FALSE (little endian; polynomial 0x1021, initial 0xffff, no reflection or final xor, covers version through payload). Response uses opcode OR 0x80, echoes sequence, starts payload with status (0 OK, 1 bad length, 2 rejected, 3 unsupported). Invalid framing/CRC yields no reply. One outstanding request. No motion/arm command in this prototype.

| Opcode | Request | Response data after status |
|---|---|---|
| 01 | empty | version major/minor/patch + capability byte (0) |
| 02 | empty | state u8, mode u8, faults u32, observed u32, warnings u32, target i32, actual i32, speed i32, load u16, position-lost u8 |
| 03 | empty | config |
| 04 | config | empty; RAM only, rejected while active or faulted |
| 05 | empty | empty; logical disable |
| 06 | empty | empty; acknowledgment requires disabled enable input and healthy dwell |
| 07 | empty | PID config (kp, ki, kd, deadband u32, output limit u16) |
| 08 | PID config | empty; RAM only, OFF state required |
| 09 | empty | motion config (speed, accel, decel, jerk u32, curve u8) |
| 0A | motion config | empty; RAM only, OFF state required |
| 0B | empty | signed demand i32, elapsed and total ms u32, moving u8 |

Config: following threshold u32 counts, stall error threshold u32 counts, stall speed threshold u32 counts/s, stall load threshold u16 permille, persistence u16 ms, healthy dwell u16 ms, policy u8 (0 monitor, 1 warn, 2 protect). Simulation values only. Critical bits always latch. Travel violation input requires a valid reference. Position loss is not cleared by acknowledgment. Recovery and physical arm are intentionally absent pending hardware validation. State values OFF=0, READY=1 (reserved), ACTIVE=2, FAULT=3. Fault bits: stall 1, following 2, travel 4, encoder 8, electrical 16, timing 32, lost position reference 64. Mode values CNC=0, position=1, velocity=2, jog=3, torque=4, open loop=5.

Future UART integration must add bounded stream framing, resynchronization, timeouts, address strategy, protocol coexistence, interrupt-safe snapshots, duplicate/retry behavior and hardware ownership. This file does not establish any electrical-level or baud-rate guarantee.

Gains use Q12 per 1 ms sample. Motion configuration uses counts/s, counts/s² and counts/s³; curve 0 = trapezoid, 1 = quintic S-curve. No units in amperes until validated. v0.1.1 adds these commands without changing prior frame layouts. Live gain changes while active and physical arm/move commands are deliberately not exposed yet. A host pipe fixture tests the same C dispatcher used by the firmware core against Studio's Python codec.

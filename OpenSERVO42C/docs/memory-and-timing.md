# Memory and real-time budgets

The original V1.1.3 layout (development/factory/layout.ld) reserves all bytes from 0x7a00 onwards. The independent application must preserve that boundary, the 512-byte identity, settings and knot pages, and the 32 KiB calibration LUT. The proposed target layout uses the entire 8 KiB SRAM address range but does not maintain the legacy fixed work-RAM ABI.

| Resource | Development budget |
|---|---:|
| Available application flash | 31,232 B |
| Application image target (vectors, code, constants, data initializers) | 28,672 B |
| Application flash headroom | 2,560 B |
| Static SRAM review target (not a hard cap) | 4,096 B |
| Maximum static SRAM including alignment | 6,144 B |
| Initial reserved stack | 2,048 B |
| Additional SRAM headroom | Variable; 0–2,048 B at 4–6 KiB static usage |

These are engineering targets, not actual measurements of the finished application. Static allocations beyond 4 KiB are permitted and marked for review by the linker symbol __ram_review_required; useful buffers and features may use that space. The hard limit prevents overlap with the initial 2 KiB stack reservation. Review the measured stack peak and worst-case interrupt nesting before changing the stack reservation. Extra RAM does not itself improve CPU execution speed. The independent firmware must not link a legacy library that depends on its fixed work-RAM ABI without reconciling the layout first.

The target linker script enforces the flash budget and stack boundary and tests the vector size. The regression suite now also permits a synthetic allocation above the 4 KiB review target while rejecting allocations that overlap the stack. The CI workflow runs these checks. This script is a target foundation, not a linked firmware yet. On the full target build, audit every allocated section and load segment, reject unexpected orphan sections, and report both program flash and SRAM use, including __ram_static_headroom and __ram_review_required.

## Resource management
- No heap allocation, recursive calls or unbounded queues on the MCU.
- One active configuration; store profiles, long telemetry logs, graphs and optimization on the PC.
- Keep encoder LUT in flash; use bounded calibration workspace and never duplicate the full LUT in SRAM.
- Fixed-size UART buffers and bounded telemetry; UI updates, flash operations and motion planning outside the current ISR.
- Audit 64-bit multiply/divide helpers, interrupt nesting, call depths and all startup stack use.
- Use a stack watermark in test firmware, including maximum observed stack use during calibration, display, UART and nested interrupts. Reserve at least 512 B of the 2 KiB stack beyond the measured maximum under stress; measured headroom does not replace worst-case analysis.
- Determine real clock and ISR periods from the original hardware configuration and measurements. Provisional target: worst-case current ISR execution under 60% of its verified period, with no missed deadlines during serial, display or motion activity. Report maximum duration, overruns, and actual sample rates.
- Test watchdog and safe hardware inhibit on deadline faults; a telemetry update must never delay current regulation.

A feature should be implemented and tested even if the physical actuation path is not yet validated. Do not advertise capabilities that are not implemented. Avoid permanent feature restrictions unless imposed by actual board capabilities or safety requirements.

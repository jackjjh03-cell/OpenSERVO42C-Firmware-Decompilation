# Hardware evidence and bring-up gates

References: [HC32L130 manufacturer documentation](https://www.xhsc.com.cn/product/1245.html), [Makerbase V1.00 schematic](https://github.com/makerbase-mks/MKS-SERVO42C/blob/MKS-SERVO42C-V1.1/Hardware/MKS%20SERVO42C%20V1.00/MKS%20SERVO42C-schematic.pdf), original development/factory/ (main commit 8b8e51611a0a497b6cec638fbcdfb0de16a47391). Check actual PCB revision before applying schematic conclusions.

| Evidence from original source | Implication |
|---|---|
| board_io.c: ADC sequence slots 0,1,2 select channels 8,9,18 | Trace to actual nets; slot 0 purpose not proven |
| current_feedback.c: slots 2 and 1 startup offsets, trimmed averaging, acceptance 1861–2234 | Two signed current feedback signals; ADC counts are not amperes |
| current_feedback.c: scale sample below 745 resets compares; otherwise scales controller limits and PWM | Preserve reference behavior until signal purpose and scaling proven |
| board_timer_display.c: timer blocks 0 and 1, period/compare initialized to 1200 | Verify real timer clock, waveform, polarity and dead time on a scope |
| control_flow.c: disable stops timer/ADC chain, clears output field and resets compares | Trace physical bridge outputs and verify inactive levels before energizing |
| motion_controller.c: timer pulse counter, LUT encoder feedback, error clamp and stall counter | Capture raw following error before clamping; test original loop timing |
| factory/layout.ld: app 0x0000–0x79ff, identity 0x7a00, settings 0x7c00, knots 0x7e00, calibration 0x8000–0xffff | Keep all reserved pages intact; no erase/write in this increment |

MCU family documentation advertises up to 48 MHz, 64 KiB flash, 8 KiB SRAM, ADC, DMA, watchdog, motor timers. The schematic identifies MT6816 encoder and SensorA/B analog paths. No new peripheral assignment is authorized on this evidence alone.

Bring-up order: verify pin/register map against manual and assembly; inspect PWM disable/reset with power stage unpowered; verify encoder and ADC sampling; establish offset, polarity and current scaling with reference instrumentation; measure timing and interrupt latency; verify watchdog and independent hardware shutdown possibilities. Only then enable bounded motor tests with current-limited supply, secured unloaded motor and independent power removal. A vertical load may fall when disabled.

The 24 V failure cause is unresolved. Sustained stall, switching/regen transients, and driver or gate logic failure are possibilities, not conclusions. Software cannot guarantee protection from all bridge failures. No board modifications required. Do not assume an external fault pin, bus voltage measurement or temperature sensing exists.

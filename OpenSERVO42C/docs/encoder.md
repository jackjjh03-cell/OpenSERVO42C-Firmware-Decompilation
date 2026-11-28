# MT6816 encoder bring-up

References: original development/factory/src/encoder_io.c and clock_and_bus.c; [MagnTek MT6816 Rev 1.4, section 7.6.5, page 21](https://uploadcdn.oneyac.com/attachments/files/brand_pdf/magntek/4E/F4/MT6816CT-AKD.pdf). The two separate reads return a 14-bit angle, low magnetic-field flag, and even parity bit.

The replacement reader distinguishes successful zero-degree samples from SPI timeouts, validates even parity and the weak-magnet flag, and deasserts chip select on all exits. The HC32 transport reproduces the original polling and register access sequence; SPI initialization and timing are not yet validated. No retries are performed inside the reader. The eventual feedback supervisor must explicitly specify retry/staleness policy and a maximum number of invalid samples. Never use an invalid sample as a position. Verify status with real hardware before enabling output.

The tracker accepts the existing calibrated 16-bit angle format, unwraps rollover in both directions into a signed 64-bit position and rejects jumps beyond a caller-supplied limit. It does not establish a machine-coordinate reference. Calibration LUT, physical maximum speed, sample cadence, missed-sample detection and safe conversion to the controller's current 32-bit coordinates remain integration work.

Host tests cover every one of the 16,384 possible angle words, parity errors, weak magnetic field, each of four possible transfer failures, CS release and wraparound in both directions. The decoder and HC32 transport compile to Cortex-M0+ objects. The physical encoder bus has not been exercised.

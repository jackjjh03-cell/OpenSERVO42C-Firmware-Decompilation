# Changelog

## R02

- Updated the project name to OpenSERVO42C-Firmware-Decompilation and refreshed the OLED Credits text. No motor-control logic changed.

- Added SysInfo pages for hardware, settings, PID, motion, encoder, memory and fault diagnostics.
- Added live encoder-derived RPM display, with the original pulse display available using Next.
- Added settings access after a startup encoder fault, with the motor locked out.
- Added UART diagnostic command `0x8D` and a PC configuration utility.
- Added an optional motion interrupt profiling build.
- Normal R02 has been flashed and booted on the original SERVO42C. Broader hardware and profiling validation remain outstanding.

[Source and documentation](Test%20Firmware/R02/)

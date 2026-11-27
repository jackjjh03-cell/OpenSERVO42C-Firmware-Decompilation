# SERVO42C Custom R02

Custom firmware for the MKS SERVO42C V1.1.3 controller (HC32L130). Based on the independently compiled factory reconstruction. Adds a read-only SysInfo menu, diagnostic UART query and desktop tuning utility without removing stock motor control, calibration, protection, original menus or UART commands. Normal R02 has been successfully programmed and booted on the original controller. It is not a hardware-qualified release.

## Build

Use Clang/LLVM 17, LLD, llvm-objdump, nm or llvm-nm, CMake 3.20+, Ninja and Python 3.8+. Open `SERVO42C.code-workspace` in VS Code and use Ctrl+Shift+B, or build from a terminal:

```sh
cmake -S . -B build/custom -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-clang.cmake
cmake --build build/custom
cmake --build build/custom --target verify
```

Optional profiling build (not the initial hardware-test build):

```sh
cmake -S . -B build/profile -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-clang.cmake -DSYSINFO_PROFILE=ON
cmake --build build/profile
cmake --build build/profile --target verify
```

Do not set CMAKE_BUILD_TYPE. The size-optimized build uses -Oz, -fomit-frame-pointer and LLD safe identical-code folding. The linker protects all reserved storage from application overlap. No vendor image is needed to compile; a same-board full backup is needed to export a full programming image.

## R02: startup encoder lockout and RPM

A startup encoder fault (factory diagnostic return code 3) now switches the controller to a latched settings-only state. The original diagnostic text is shown for about 10 seconds from detection; Menu or Enter dismisses it sooner. The full menu opens automatically. The motor-control outputs, step/ADC/motion interrupts and UART are disabled before the warning is shown. Settings may be saved, but Cal, Set 0, Goto 0 and drive enable cannot operate. Exit returns to `Motor lock / RPM -- / Menu`. The state cannot be cleared by a menu mode change or by acknowledging the screen; repair the sensor and restart. This is a software interlock, not an independent hardware safety function. Factory open-loop startup behavior and other diagnostic paths are retained; the new latch applies to the detected startup fault, not continuous runtime magnet monitoring. Do not select open-loop to bypass a sensor fault.

The main telemetry view keeps its original first two lines and defaults to an RPM third line. Next toggles the last line to the original pulse-count display (`clk`) and back. RPM is an unsigned magnitude estimated from calibrated encoder feedback over approximately half a second, using the existing live clock value and foreground SysTick polling. There is no new periodic interrupt or motion ISR instrumentation in the normal build. `RPM --` indicates unavailable data (drive off, open-loop mode, missing LUT, startup fault or initial/rebased sample). Display bus stalls and interrupt timing can affect measurements; verify against a tachometer during a safe bench test. Original clock/pulse telemetry is retained.

To fit both revisions, menu option strings are packed in flash and expanded at reset into the same original RAM offsets and 9-byte slots; duplicate menu descriptors reuse the main table in flash. Reset comparisons confirm the legacy workspace is preserved. No application code occupies the identity, settings or calibration pages.

## SysInfo

Enter the normal menu, scroll past Exit to SysInfo, then use Next to move between categories and Enter to select. Next cycles the pages of a category. Enter returns to the categories; Menu returns to telemetry. Exit in SysInfo returns to the main menu. Original font/format, three rows, maximum 10 characters per row. No configuration writes from the SysInfo OLED screen. Refresh target is 500 ms; timing is best-effort in foreground and interrupted by existing factory delays. Protection display has priority. Boot logo removed; display initialization and factory startup settling delay retained. Revision appears under Credits.

| Category | Contents |
|---|---|
| Credits | OpenSERVO42C project name and version |
| Hardware | Target MCU and architecture; clock in integer MHz from live firmware clock word |
| Settings | 21 live fields including mode, baud, address, microsteps, current, direction, enable, protection, interpolation, origin, acceleration, torque and error limit |
| PID | Raw Kp, Ki, Kd |
| Motion | Drive state, UART motion state, stall, error, low 32-bit position/target, speed, enable request and control output |
| Encoder | Angle, status, LUT and calibration flags, low 32-bit feedback position |
| Memory | Flash and RAM capacities, linked application extent/free bytes, reserved workspace and stack budget (not measured peak usage) |
| Regs | CPUID part number, decoded clock divider and clock source selector |
| Faults | Startup state, stall/protection and raw ADC/phase values |
| Perf (optional) | Approximate motion IRQ rate, IRQ workload and maximum sampled handler cycles |

Mode 0=CR_OPEN, 1=CR_vFOC, 2=CR_UART. Direction 0=CW, 1=CCW. Clock source selector follows the factory clock-frequency routine (4=PLL); divider is shown as a factor. Raw motion/ADC values are not asserted to be SI units. Multi-field reads are sequential, not an atomic snapshot. For the profiling build, the Memory screen shows capacities only. Profiling measures the motion handler, not total CPU utilization; it wraps the ISR and can affect timing. Timer samples are suspended when factory delays take over SysTick. Long foreground stalls, preemption and display time affect measurements. No stack high-water or reset-cause instrumentation is included.

## UART and configuration

All 36 factory commands remain. New command `8D` reads one diagnostic field without changing it:

- Request: address, `8D`, selector (0–44), additive checksum modulo 256.
- Reply: address, four-byte unsigned big-endian raw value, additive checksum.
- Unknown selector: `FFFFFFFF` if the packet reaches the dispatcher. Existing parser resynchronization rules remain.
- Example: `E0 8D 15 82` queries Kp (selector 21 decimal). A Kp of 1638 is `E0 00 00 06 66 4C`.

See [diagnostic field reference](docs/SYSINFO.md). The desktop utility provides a configuration viewer and explicit tuning writes through the original A1–A5 commands. Install pyserial and use:

```sh
python -m pip install pyserial
python tools/config_editor.py COM12 --baud 115200 --address 0xE0
```

Use the actual port, baud and device address. `--dump` gives a read-only console report; `--echo` is for adapters that echo the request exactly. The editor requires an idle, disabled UART drive before a write, asks for confirmation, and reads the value back. This is an advisory software check, not a motor safety interlock. The GUI has not been tested against physical hardware. Current, microsteps, direction and other original settings remain editable in the original OLED menus. On-device PID editing is not included in R02.

## Programming and validation

See [bench procedure](FLASHING.md). Factory fallback settings remain unchanged. A complete image preserves the identity, settings and calibration from the supplied original full backup, not any changes subsequently made on the physical board. Keep all personal flash data private.

To generate a same-board image yourself (no programming takes place):

```sh
python tools/export_board_image.py build/custom/servo42c.elf path/to/bin.hex private_build --acknowledge-unvalidated
```

The exporter checks the backup revision, build hash, application range and exact reserved data preservation. The factory baseline traceability and validation documents are historic reference material. Current R02 verification logs are in `verification/`, alongside historical test records. `python tools/test_r02.py build/custom/servo42c.elf path/to/bin.hex` runs the new fault, identity and RPM tests against the actual ELF and the same-board backup. `audit_factory.py --suite startup` compares unchanged startup/UI paths; the intentionally different R02 paths are exercised in the new test. The legacy `--extra` telemetry comparison is a baseline-only check unless the original pulse view is selected in its harness.

## Limitations

Normal R02 startup and programming are confirmed on the original controller. Physical motor operation, real-time interrupt deadlines and broader identity compatibility remain unverified. Both variants retain the original flash error-handling limitations. The profile variant is intended for controlled later testing, not the initial programming attempt. Vendor-derived source provenance and redistribution status are described in NOTICE.md.

The OpenSERVO42C Credits text was updated after the recorded R02 hardware test. Historical verification results and application hashes refer to the earlier build.

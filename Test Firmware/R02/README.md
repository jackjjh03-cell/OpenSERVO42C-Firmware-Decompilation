# SERVO42C Custom Firmware — R02

Custom firmware for the MKS SERVO42C closed-loop stepper controller based on the V1.1.3 factory firmware reconstruction in this repository. The aim is to retain normal controller operation while making it easier to inspect the firmware, troubleshoot the board and tune the motor.

R02 adds a system information menu, live RPM display, a settings-only mode for startup encoder faults and additional UART diagnostics. It also includes a PC configuration utility and a separate build for measuring motor interrupt performance.

## Hardware

| | |
|---|---|
| Controller | MKS SERVO42C |
| Factory firmware | V1.1.3 |
| MCU | HC32L130, ARM Cortex-M0+ |
| Flash | 64 KiB |
| RAM | 8 KiB |
| Display | Original OLED, three rows of up to 10 characters |
| Input | Original Menu, Next and Enter buttons |
| Toolchain | Clang/LLVM 17, LLD, CMake and Ninja |

This build targets the board revision used for the original firmware capture. Compatibility with other SERVO42C revisions, SERVO42D or SERVO57 controllers has not been established.

## Changes from factory firmware

| Area | Factory V1.1.3 | Custom R02 |
|---|---|---|
| Main display | Rotation, position error and pulse count | Original rotation and error readings; RPM or pulse count on the third row |
| Main menu | Original settings and Exit | Original entries retained; SysInfo appended after Exit |
| Encoder startup fault | Blocks normal startup and menu access | Warning timeout followed by settings access with the motor locked out |
| System information | Limited to original settings and telemetry | Categorized live diagnostic screens |
| UART | 36 recovered commands | Original commands plus read-only diagnostic query `0x8D` |
| Motor tuning | Original menu and UART commands | Original controls plus a PC configuration utility |
| Performance information | No dedicated profiling display | Optional motion interrupt profiling build |
| Startup banner | Original version banner | Banner omitted to save space; revision shown under Credits |
| OLED font | Original uncompressed font | Packed bitmap data; retained glyphs use the original pixels |
| Menu data | Original initialized data layout | Packed option strings expanded into the original RAM layout during startup |
| Compilation | Original vendor toolchain unknown | Size-optimized C reconstruction using Clang and LLD |

The original motor-control calculations, current regulation, supported control modes, step/direction input, encoder calibration, origin return, protection logic, settings storage and device identity check remain in the application. Changes to motor enable and startup paths are limited to the new fault interlock and related UI handling; the normal and profiling builds are not instruction-identical to the vendor executable.

### Main display and RPM

The first two lines use the original rotation and position-error formatting. The third line now defaults to measured RPM. Press **Next** on the main screen to switch between RPM and the original `clk` pulse-count reading.

```text
RPM view       Original pulse view
-----------    -------------------
360.0?         360.0?
0.00err        0.00err
RPM 120        3200clk
```

Example values only. RPM is an unsigned speed magnitude derived from calibrated encoder feedback (65,536 counts per revolution), not the commanded motor speed. The normal build measures it in the foreground over roughly 500 ms using the existing clock configuration and SysTick. It adds no periodic interrupt. `RPM --` indicates that a valid sample is unavailable, such as when the drive is off, in open-loop mode, without a valid encoder lookup table or after rebasing the measurement. Actual accuracy still needs to be checked against an independent speed measurement.

### Startup magnet/encoder fault

The original startup encoder check remains active. When the startup diagnostic returns an encoder fault, R02 takes a different path:

1. Lock the drive and disable the motion, step-input, ADC and UART interrupt paths.
2. Show the original magnet/encoder error for approximately 10 seconds. Menu or Enter can dismiss the warning sooner.
3. Open the full settings menu automatically, including SysInfo.
4. Allow configuration changes and settings saves without enabling motor movement.

Calibration, Set 0, Goto 0 and drive enabling are blocked in this state. UART is disabled, but its configuration can still be changed in the menu for the next startup. Exiting the menu shows `Motor lock / RPM -- / Menu`. The fault is latched until a restart; changing control mode cannot clear it.

Correct the encoder or magnet problem before restarting. This is a software interlock, not a hardware safety circuit. It applies to detected startup encoder faults, not continuous runtime magnet monitoring. Original open-loop startup behavior and other startup diagnostic paths are retained. The device-ID check is not bypassed.

## SysInfo

Open the original menu and scroll past Exit to **SysInfo**. Next selects a category, Enter opens it and Next cycles its pages. Enter returns to the category list; Menu returns to telemetry. The Exit category returns to the main menu.

```text
 Credits
*Hardware
 Settings
```

SysInfo uses the original OLED driver and button handling. Pages are limited to three lines of 10 characters and refresh approximately twice per second while active. The original protection display takes priority. SysInfo itself does not write configuration or flash.

| Category | Information |
|---|---|
| Credits | OpenSERVO42C project name and firmware revision |
| Hardware | MCU, processor architecture and live configured clock |
| Settings | 21 fields: mode, baud, address, microsteps, current, direction, enable mode, protection, interpolation, display sleep, origin settings and other motor parameters |
| PID | Raw proportional, integral and derivative gains |
| Motion | Drive and UART motion state, position error, pulse position, target, speed, enable request and control output |
| Encoder | Raw angle, status, calibration flags and feedback position |
| Memory | Flash/RAM capacity, application usage and available space in the normal build |
| Regs | CPUID part number, decoded clock divider and source selector |
| Faults | Startup state, stall/protection flags and raw ADC/phase values |
| Perf | Motion IRQ rate, estimated workload and peak sampled cycles (profiling build only) |
| Exit | Return to the original main menu |

Values are read from firmware RAM or selected processor registers. Some are raw fixed-point or internal values, not directly expressed in physical units. Motion positions may show only the low 32 bits. Reads are sequential rather than an atomic snapshot. The register page does not expose arbitrary memory access.

## UART extension

All 36 recovered factory UART commands are retained; the factory protocol reference is in the main repository README. R02 uses the same address and additive checksum convention for its new read-only command.

| Packet | Layout |
|---|---|
| Request (4 bytes) | `ADDR 8D SELECTOR CHECKSUM` |
| Reply (6 bytes) | `ADDR VALUE[31:24] VALUE[23:16] VALUE[15:8] VALUE[7:0] CHECKSUM` |

The checksum is the sum of all preceding bytes modulo 256. Values are returned as unsigned big-endian 32-bit raw data. An unsupported selector returns `FFFFFFFF` if the request reaches the dispatcher.

Example using address `E0`, reading Kp (selector 21 decimal / `15` hex):

```text
Request:  E0 8D 15 82
Response: E0 00 00 06 66 4C  (example Kp = 1638)
```

| Decimal selectors | Fields in order |
|---|---|
| 0–7 | Mode, baud, address, microsteps, current, direction, enable mode, protection |
| 8–14 | Interpolation, display sleep, alternate microsteps, motor geometry, acceleration, torque, error limit |
| 15–20 | Origin mode, origin speed, origin direction, origin position, saved motion, acceleration index |
| 21–23 | Kp, Ki, Kd |
| 24–32 | Drive enabled, UART motion state, stall, position error, pulse position, target, speed, enable request, control output |
| 33–38 | Encoder angle, encoder status, LUT ready, knot valid, LUT valid, feedback position |
| 39–44 | Startup state, stall, protection, ADC feedback 1, ADC feedback 2, phase index |

See [docs/SYSINFO.md](SERVO42C_Custom_R02_Source/docs/SYSINFO.md) for the full selector table and OLED labels. Control mode values are `0` (CR_OPEN), `1` (CR_vFOC) and `2` (CR_UART). Direction values are `0` (CW) and `1` (CCW).

## PC configuration utility

[View or download config_editor.py](SERVO42C_Custom_R02_Source/tools/config_editor.py).

The Python utility can read all 45 diagnostic fields and edit Kp, Ki, Kd, acceleration and torque using the original UART commands A1–A5. It requires confirmation before a write, checks for an idle, disabled UART drive and reads the value back. The checks are advisory software checks, not an independent safety interlock.

```sh
python -m pip install pyserial
python tools/config_editor.py COM12 --baud 115200 --address 0xE0
```

Replace the port, baud and address with the controller's actual settings. Add `--dump` for a read-only console report or `--echo` for an adapter that echoes the transmitted packet exactly.

The original OLED configuration menus remain available. The SysInfo pages are read-only; direct PID editing on the OLED is not implemented. The PC utility has passed mock serial tests, but has not been confirmed on physical hardware.

## Flash and memory usage

The custom features fit without using the reserved device identity, settings or calibration pages. R02 recovers space by omitting the startup banner, trimming unused font characters, compressing the retained glyph data, packing the menu options and reusing duplicate menu descriptors. The retained glyph pixels and original menu RAM layout have been checked against the factory reference.

```text
Application partition: 31,232 bytes (0x0000–0x79FF)

Factory C    30,428 B  [#############################-]  804 B free
Custom R02   30,960 B  [##############################]  272 B free
R02 Profile  31,152 B  [##############################]   80 B free
```

Application flash usage is approximately 97.43% for the factory C build, 99.13% for R02 and 99.74% for the profiling build. The factory C figure refers to the reconstructed application, not the byte-exact assembly build.

| Flash range | Purpose | R02 handling |
|---|---|---|
| `0x0000–0x79FF` | Application | Rebuilt |
| `0x7A00–0x7BFF` | Device identity | Preserved from same-board backup |
| `0x7C00–0x7DFF` | Persistent settings | Preserved |
| `0x7E00–0x7FFF` | Calibration knots | Preserved |
| `0x8000–0xFFFF` | Encoder lookup table | Preserved |

The original factory fallback settings remain unchanged (CR_vFOC, stored baud 0, address E0, microsteps 16 and raw Kp/Ki/Kd of 1638/1/1638). A full programming image instead retains the settings from the supplied backup, so the active settings may differ. A captured configuration is not necessarily safe to reuse without checking motor tuning, current and protection.

## Building

Open `SERVO42C.code-workspace` in VS Code and use Ctrl+Shift+B, or build from the command line. Requires Clang/LLVM 17, LLD, llvm-objdump, nm or llvm-nm, CMake 3.20+, Ninja and Python 3.8 or newer.

```sh
cmake -S . -B build/custom -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-clang.cmake
cmake --build build/custom
cmake --build build/custom --target verify
```

The output is `build/custom/servo42c.elf` with a linker map and build report. Leave `CMAKE_BUILD_TYPE` unset; the project uses `-Oz`, `-fomit-frame-pointer` and LLD safe identical-code folding. The original HEX is not needed to compile the C source.

### Optional profiling build

```sh
cmake -S . -B build/profile -G Ninja -DCMAKE_TOOLCHAIN_FILE=cmake/arm-clang.cmake -DSYSINFO_PROFILE=ON
cmake --build build/profile
cmake --build build/profile --target verify
```

The Perf screen measures the motion interrupt rate, estimated handler workload and maximum sampled execution cycles. It does not measure total CPU utilization, uptime or peak stack use. Profiling adds interrupt overhead and should be tested separately from the normal firmware. Factory delay routines can temporarily take over SysTick and suspend profiling samples. The profile build's Memory screen shows capacities only.

## Source layout

```text
custom firmware/
├── CMakeLists.txt
├── SERVO42C.code-workspace
├── factory/
│   ├── include/          Firmware definitions and memory layout
│   ├── src/              Reconstructed C application
│   ├── tests/            Reference comparison tests
│   └── layout.ld         Linker script
├── cmake/                ARM compiler configuration
├── tools/                Build checks, tests, image export and PC utility
├── docs/                 Diagnostic selector and reference documentation
├── verification/        Build hashes and software test results
├── FLASHING.md           Bench and recovery procedure
└── NOTICE.md             Source provenance
```

The `factory/` folder name is retained for compatibility with the existing build system; its contents in this project are the custom variant. The unchanged factory reconstruction remains in the parent repository.

Main source changes:

| File(s), relative to `factory/src/` | Purpose |
|---|---|
| `sysinfo.c`, `../include/sysinfo.h` | Diagnostics, categories, UART selector values and optional profiler |
| `application_ui.c` | Fault timeout, motor lock state and foreground UI integration |
| `telemetry.c`, `mode_and_position.c` | RPM calculation, toggle and measurement rebasing |
| `menu.c`, `menu_navigation.c`, `remaining_helpers.c` | Additional menu entry and navigation, motion guards, banner removal |
| `commands.c` | UART diagnostic dispatch |
| `board_io.c`, `control_flow.c`, `calibration_work.c`, `settings.c` | Enforce startup fault lockout in existing control/settings paths |
| `display.c`, `layout_assets.c`, `layout_startup_data.c` | Packed font and menu assets |
| `board_startup.c`, `reset.c` | Optional profiler timing suspension and interrupt wrapper |

## Programming

Read [FLASHING.md](SERVO42C_Custom_R02_Source/FLASHING.md) before programming. Keep a full original HEX backup and a working BOOT0/UART ISP recovery connection. Generate images for the same physical controller only; another board's identity and encoder calibration may not be valid on yours.

To export a complete 64 KiB image from the normal build and a matching factory backup:

```sh
python tools/export_board_image.py build/custom/servo42c.elf path/to/bin.hex private_build --acknowledge-unvalidated
```

The exporter checks the known factory application revision, the compiled application hash, its flash extent and preservation of the entire reserved region. It writes a HEX, BIN and manifest without programming the device. The acknowledgement flag remains required because the image is not a hardware-qualified release.

The normal R02 build was reported to boot successfully after programming through the HDSC UART programmer. This is not confirmation that arbitrary board revisions are supported or that programming is fail-safe. Keep motor drive power independently interruptible and verify tuning, current, calibration and protection before attempting unloaded motion. Never use the firmware lockout as the sole means of preventing movement.

Do not commit the original flash dump, full board-specific images or generated private builds to the repository.

## Verification

Software comparison tests execute the rebuilt and original instructions under modeled hardware where applicable. The following results are recorded in [verification/](SERVO42C_Custom_R02_Source/verification/).

| Test | Passing cases |
|---|---:|
| Board initialization / reset-to-identity | 72 / 24 |
| Original UART dispatch / receive IRQs | 10,951 / 17,408 |
| Original menu callbacks | 1,424 |
| Motion / ADC IRQ comparisons | 96 / 96 |
| Settings commits / low-level flash | 64 / 8 |
| Origin return / calibration sweeps | 96 / 8 |
| Startup encoder lockout | 12 |
| RPM sampling and toggle | 10 |
| SysInfo page checks | 63 |
| Diagnostic UART selectors | 256 |
| Retained glyph comparisons | 67 |

The actual device identity transform and physical timing cannot be established by the model. The test suite covers the intentional R02 UI differences separately from the unchanged factory paths. The profiling build has targeted feature tests, not the same full regression coverage as the normal build. Physical power-loss recovery is unverified.

Application SHA-256 (application partition, including padding):

```text
Normal:  1453a05c4500553a6093f4244421916ba6a6a0779ee371266c861809de4cf313
Profile: e3b2b28ec72994b981f7c4dcc35b88382be7dca142b96c22e6f289661a40e302
```

## Revision history

- **R02 (first published custom revision):** Added SysInfo, diagnostic UART command, PC tuning utility, optional interrupt profiling, startup encoder-fault settings-only mode, encoder-derived RPM display and pulse-count toggle. Removed the startup banner and optimized display and menu data to recover flash space. Retained the original device-ID check and board-specific flash regions. First reported successful physical boot of the normal custom build.

## Source notice

This is an unofficial firmware reconstruction and modification, not an original Makerbase source release. The custom application is independently compilable but is not byte-identical to the factory HEX. The separate assembly reference in the main repository is used for exact reproduction of the original instructions. See [NOTICE.md](SERVO42C_Custom_R02_Source/NOTICE.md). Project-authored code and documentation are available under the MIT license; see the root source notice for third-party material.

The Credits screen was updated to the OpenSERVO42C name after the original R02 hardware test; existing verification records describe the earlier build.

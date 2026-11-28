# OpenSERVO42C v0.1.1 — development

Independent firmware for the stock HC32L130F8UA MKS SERVO42C, based on the original V1.1.3 reconstruction in `../development/factory` and `../assembly`, not R02.

The motor application includes six control modes (CNC, position, velocity, jog, torque and open loop), position and current control, fault protection, travel-limit teaching, persistent configuration, encoder calibration and OLED status/setup controls. Studio supports configuration, motion commands, calibration, diagnostics, recorded PID trials and supervised gain search. Firmware and Studio are development builds: host tests and target links are not evidence of safe physical operation.

## Development build

From the repository root:

```sh
cmake -S OpenSERVO42C -B build/open
cmake --build build/open
ctest --test-dir build/open --output-on-failure
python OpenSERVO42C/tests/test_link_budget.py
python OpenSERVO42C/tools/check_target_link.py --motor --optimized
```

Host tests require a C compiler, CMake, Python 3.8+ and Tk for Studio queue tests. Target linking requires ARM-capable Clang, LLD and `nm` (overrides: `ARM_CLANG`, `ARM_LLD`, `ARM_NM`). The motor build requires size optimization. The acquisition variant (`--bringup --optimized`) does not authorize motor output.

An optional `--output path.hex` exports only file-backed application flash from an optimized application link. It refuses existing output files and data outside `0x0000–0x75ff`. It does not create a full device backup or authorize flashing. Before physical testing, retain a complete board-specific flash backup and establish recovery; never use mass erase with an application-only HEX.

## Studio

From `OpenSERVO42C`, install the pinned serial dependency and start the GUI:

```sh
python -m pip install -r studio/requirements.txt
python -m studio.app
```

Python 3.8/Tk 8.6 compatibility is intended; Windows 7 has not been independently tested. Serial disable is not an emergency stop. Keep motor output disabled and external EN inactive during settings and LUT programming. Calibration sweeps require an uncoupled motor.

See [hardware evidence](docs/hardware.md), [protocol](docs/protocol.md), [memory and timing budgets](docs/memory-and-timing.md), and [bench validation](docs/bench-validation.md). Older development notes describe earlier implementation stages. The original factory project remains unchanged.

# Firmware validation

## Reference build

Clang/LLD 17, Cortex-M0+ Thumb, `-Oz`. 44 C translation units; no unresolved symbols. The application extent is 30,428 bytes with 804 bytes remaining before reserved flash. SHA-256 of the application padded with `0xFF` to `0x7A00`:

```
9c0922b0aedabd35c9b22596dd5e2902c3c63e9b1f0c4293bda754a3a381b4c7
```

The source maintenance release changes comments and formatting only; both source distributions were rebuilt to confirm this hash. Earlier behavioral results apply to the same application binary. See `verification/source_build.txt` for the fresh package checks.

## Host tests

From the development project root (Python assertions must be enabled):

```sh
python factory/build_audit.py --integration
python factory/build_audit.py --persistence
python factory/build_audit.py --edit-smoke
python factory/tests/check_limits.py
python factory/tests/check_default_edits.py
python tools/check_stock_defaults.py
python tools/check_c_declarations.py
python tools/check_traceability.py build/factory/servo42c.elf
```

For comparison with a private matching full flash backup:

```sh
python tools/audit_factory.py build/factory/servo42c.elf /path/to/bin.hex --full --extra
python tools/final_factory_checks.py build/factory/servo42c.elf /path/to/bin.hex
python tools/runtime_parity.py build/factory/servo42c.elf /path/to/bin.hex
```

The reference logs cover startup and ordered peripheral writes, UART, menus, ADC/motion interrupts, settings and flash, origin return, scripted calibration and arithmetic/telemetry. Function addresses are indexed in `function_traceability.csv`; linked addresses refer to the reference build and may change after code edits. Recorded tests use modeled peripherals, scripted samples and explicit mock boundaries. Tests are not a proof of universal equivalence or real-time behavior.

## Image export and bench validation

```sh
python tools/export_board_image.py build/factory/servo42c.elf /path/to/bin.hex output --acknowledge-unvalidated
```

The output directory must not exist. The backup must match the supported revision and originate from the same board. The exporter checks the build hash and preserves every byte at `0x7A00–0xFFFF`, including any saved user configuration. It does not flash hardware. Establish safe recovery and validate the hardware before programming; see `factory/BENCH_VALIDATION.md`.

The C output differs from the original executable. A separate assembly reference was verified byte-exact against the private dump; it is not an original C source release. Exact interrupt timing, electrical motor behavior and flash power-loss recovery remain unverified.

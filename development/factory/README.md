# Firmware source

The application is built from `src/`, `include/` and `layout.ld`. Refer to the top-level README for build instructions. Run `python build_audit.py --integration` for host integration tests, `--persistence` for flash and settings tests, or `--edit-smoke` for a source-edit build check. Run Python without `-O` so assertions remain active.

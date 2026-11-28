# OpenSERVO42C v0.1.0 — development
Independent firmware for the stock HC32L130F8UA MKS SERVO42C, based on the original V1.1.3 reconstruction in ../development/factory and ../assembly, not R02.

This first increment is a portable controller/protection core and a versioned wire contract, with host tests. It is **not flashable motor firmware**. No register writes, bridge enabling, or validated electrical protection are present. Do not flash or connect this prototype to a live drive.

Build and test on a development PC:
```sh
cmake -S OpenSERVO42C -B build/open
cmake --build build/open
ctest --test-dir build/open --output-on-failure
```
Python tests require Python 3.8+ and no external dependencies. Studio's serial transport and UI are not yet implemented.

See [hardware evidence](docs/hardware.md), [protocol](docs/protocol.md), and [development requirements](docs/requirements.md). The original factory project remains unchanged.

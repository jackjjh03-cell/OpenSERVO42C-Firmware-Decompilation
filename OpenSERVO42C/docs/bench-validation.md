# First powered bench test

## Before motor power
Record PCB revision, supply voltage, motor nameplate current, supply current limit and test equipment. Back up the original complete flash privately, verify readback, and establish a proven restore method. The development image must not overwrite board identity or encoder calibration unless an explicit calibration procedure is requested. Use an unloaded secured motor, not a machine axis or gravity load.

Review the original PWM initialization, phase compare writes and disable sequence against the schematic and reference manual. Verify the physical inactive gate/input levels and how the bridge behaves on reset, not merely that PWM compare values equal 1200. Use a power-stage-unpowered check where possible without backfeeding the board or energizing it through an unsupported rail. Confirm the actual PWM timing, polarity and gate-driver input behavior. If using an oscilloscope on switching nodes or high-side gates, use appropriately rated differential/isolated probing; do not attach an earth-grounded probe clip to a floating switching node. If the necessary equipment is unavailable, document that limitation; motor movement alone does not establish safe switching.

## First powered run
Use a current-limited supply within the board's documented range, conservative motor current, short test movements and independent power disconnect. Monitor supply current and temperature, and stop on unexpected behavior. A bench supply current limit does not necessarily prevent brief bridge shoot-through or regenerative voltage spikes.

| Test | Expected |
|---|---|
| Power up with drive disabled | No unexpected motor torque or movement |
| Enable with target initialized to feedback | Holding torque, no jump |
| Disable while stationary | Output inhibited and holding torque removed |
| Small low-speed moves both directions | Correct direction, feedback and bounded demand |
| Disable during small move | Output inhibited, pending motion cancelled |
| Inject a software fault through a test-only path | Shutdown and latched status, even with continued external step commands |
| Acknowledge while disabled | No automatic movement; position validity checked |
| Reset while disabled | Inactive outputs, no restart of previous motion |

Record scope traces where possible, actual current, fault codes, maximum ISR execution time, stack watermark, and unexpected behavior. Test noncritical protections with synthetic signals or deliberately low warning thresholds before considering any mechanical load tests. Do not deliberately crash or hard-stall the motor at full current, short outputs or inject dangerous electrical faults.

Physical PWM/current control should be included in the first motor-capable test build once the source-level hardware audit is complete, with ordinary enable and conservative commissioning limits. It should not require users to bypass a hidden validation flag. A successful controlled bench test is needed before advertising the build as working on hardware. Until then, test status and capability flags must be accurate.

The stock SERVO42C is not an independently certified emergency stop. GRBL stopping step pulses alone is not equivalent to disabling the servo. A vertical axis may drop when torque is removed.

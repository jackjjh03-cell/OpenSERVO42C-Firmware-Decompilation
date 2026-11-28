"""Generate a factory-format inverse LUT from two measured 200-knot sweeps.
Both lists must be in the same mechanical knot order (reverse capture reversed).
This is an offline calculation only: it does not drive the motor or write flash.
"""
import struct

RAW_TURN = 16384
KNOTS = 200
ANGLE_TURN = 65536


def signed_delta(a, b):
    return (b-a+RAW_TURN//2) % RAW_TURN-RAW_TURN//2


def validate_sweep(samples):
    samples = tuple(samples)
    if len(samples) != KNOTS or any(type(v) is not int or not 0 <= v < RAW_TURN for v in samples):
        raise ValueError("Expected 200 integer raw encoder readings (0–16383)")
    delta = [signed_delta(a,b) for a,b in zip(samples,samples[1:]+samples[:1])]
    direction = 1 if sum(delta) == RAW_TURN else -1 if sum(delta) == -RAW_TURN else 0
    if not direction or any(not 0 < direction*d < RAW_TURN//2 for d in delta):
        raise ValueError("Sweep must cover exactly one turn, without repeated or reversed knots")
    return samples, direction


def merge_sweeps(forward, reverse, max_hysteresis):
    if type(max_hysteresis) is not int or not 0 <= max_hysteresis < RAW_TURN//2:
        raise ValueError("Invalid maximum raw-count hysteresis")
    forward, direction = validate_sweep(forward)
    reverse, reverse_direction = validate_sweep(reverse)
    if direction != reverse_direction:
        raise ValueError("Reverse capture must be reordered to the same mechanical knot order")
    differences = [signed_delta(a,b) for a,b in zip(forward,reverse)]
    if any(abs(d)>max_hysteresis for d in differences):
        raise ValueError("Forward and reverse sweeps disagree beyond the hysteresis limit")
    # Round the midpoint toward the smaller unwrapped reading, including wrap.
    knots = tuple((a+d//2)%RAW_TURN for a,d in zip(forward,differences))
    validate_sweep(knots)
    return knots, max(map(abs,differences))


def generate_lut(knots):
    knots, direction = validate_sweep(knots)
    lut = [None]*RAW_TURN
    for i,a in enumerate(knots):
        b = knots[(i+1)%KNOTS]
        span = direction*signed_delta(a,b)
        # Factory knot angle convention: nearest 16-bit angle for i/200 turn.
        y0 = (i*ANGLE_TURN+KNOTS//2)//KNOTS
        y1 = ((i+1)*ANGLE_TURN+KNOTS//2)//KNOTS
        for offset in range(span):
            raw = (a+direction*offset)%RAW_TURN
            if lut[raw] is not None:
                raise ValueError("Overlapping raw encoder intervals")
            lut[raw] = (y0+((y1-y0)*offset+span//2)//span)%ANGLE_TURN
    if any(v is None for v in lut):
        raise ValueError("Uncovered raw encoder readings")
    # Match the controller's winding/ambiguity check before returning bytes.
    delta = [(b-a+ANGLE_TURN//2)%ANGLE_TURN-ANGLE_TURN//2 for a,b in zip(lut,lut[1:]+lut[:1])]
    if sum(delta)!=direction*ANGLE_TURN or -ANGLE_TURN//2 in delta:
        raise ValueError("Generated lookup table has invalid winding")
    return struct.pack('<16384H',*lut)

"""Collect the ordered samples of one firmware-managed calibration sweep."""
from . import protocol as p
from .calibration import merge_sweeps, generate_lut, signed_delta


class Sweep:
    def __init__(self, hysteresis):
        if type(hysteresis) is not int or not 0 <= hysteresis < 8192:
            raise ValueError("Invalid maximum raw-count disagreement")
        self.hysteresis = hysteresis
        self.samples = []
        self.lut = None
        self.peak = None
        self.direction = 0

    def accept(self, status):
        """Return True only when the next step is authorized by a new sample."""
        index, raw, flags = status
        if flags not in (p.CAL_ACTIVE, p.CAL_ACTIVE|p.CAL_READY, p.CAL_DONE|p.CAL_READY):
            raise ValueError("Calibration stopped or aborted; check controller and motor")
        if not flags & p.CAL_READY:
            if index != len(self.samples):
                raise ValueError("Unexpected in-progress knot index")
            return False
        if not 0 <= raw < 16384 or not 0 <= index <= 400:
            raise ValueError("Invalid calibration sample")
        if index == len(self.samples)-1 and raw == self.samples[-1]:
            return False  # stale ready status must never cause another step
        if index != len(self.samples) or (index == 400) != bool(flags&p.CAL_DONE):
            raise ValueError("Out-of-order calibration sample")
        if index:
            delta = signed_delta(self.samples[-1],raw)
            if not delta or abs(delta)==8192:
                raise ValueError("Motor did not advance to a distinct calibration knot")
            direction = 1 if delta>0 else -1
            if index==1:self.direction=direction
            elif direction != (self.direction if index<=200 else -self.direction):
                raise ValueError("Motor moved in the wrong direction during the sweep")
        self.samples.append(raw)
        if index == 400:
            forward = self.samples[:200]
            reverse = self.samples[201:][::-1]
            if abs(signed_delta(forward[0],self.samples[200])) > self.hysteresis:
                raise ValueError("Forward sweep did not close at the initial knot")
            knots,self.peak = merge_sweeps(forward,reverse,self.hysteresis)
            self.lut = generate_lut(knots)
            return False
        return True

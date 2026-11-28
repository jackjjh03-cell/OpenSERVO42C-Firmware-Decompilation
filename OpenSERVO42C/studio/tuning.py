"""Measured response metrics for a recorded, single point-to-point move.
No motor commands and no automatic gain changes. Units are encoder counts/ms.
"""
import math


def analyze(rows, interval, faulted=False, tolerance=16):
    if faulted or len(rows) < 5 or interval < 1 or tolerance < 1:
        raise ValueError("Need a valid, non-faulted recording with at least five samples")
    times = [((r[0]-rows[0][0]) & 0xffffffff) for r in rows]
    if any(b-a != interval for a,b in zip(times,times[1:])):
        raise ValueError("Recording contains missing or irregular samples")
    initial, final = rows[0][2], rows[-1][2]
    direction = 1 if final > initial else -1
    travel = abs(final-initial)
    if travel <= tolerance or abs(rows[0][1]-initial)>tolerance:
        raise ValueError("Start at rest and record a move larger than the tolerance")
    command = [direction*(r[2]-initial) for r in rows]
    if any(b<a for a,b in zip(command,command[1:])):
        raise ValueError("Record one move without reversing the target")
    end = next(i for i,v in enumerate(command) if v==travel)
    if end >= len(rows)-2:
        raise ValueError("Capture must include a stationary target after the move")
    error = [r[2]-r[1] for r in rows]
    outside = [i for i in range(end,len(rows)) if abs(error[i])>tolerance]
    settled = max(end, outside[-1]+1 if outside else end)
    # Require at least two samples in band before calling the move settled.
    settling = times[settled]-times[end] if settled < len(rows)-1 else None
    return {"travel_counts":travel, "peak_error_counts":max(map(abs,error)),
            "rms_error_counts":math.sqrt(sum(e*e for e in error)/len(error)),
            "overshoot_counts":max(0,max(direction*(r[1]-final) for r in rows)),
            "settling_ms":settling, "peak_load_permille":max(r[4] for r in rows),
            "peak_demand":max(abs(r[3]) for r in rows)}

"""One explicit, bounded position-move trial. Never arms or changes gains."""
from dataclasses import dataclass
from . import protocol as p
from .tuning_trials import make_trial

QUALIFIED_STATUS = 8
UNQUALIFIED = 4


@dataclass(frozen=True)
class Capture:
    gains: tuple
    context: tuple
    start: int
    end: int
    count: int
    interval: int
    tolerance: int
    tail_ms: int
    tick_before: int
    tick_after: int


def configuration(client):
    gains = client.read(p.GET_PID)
    if client.read(p.GET_LIVE_PID)[0]:
        raise ValueError("Use normal PID gains, not temporary live gains, for comparative trials")
    context = tuple(client.read(op) for op in (p.GET_DRIVE,p.GET_MOTION,p.GET_CONFIG,p.GET_LIMITS))
    return gains, context


def begin_trial(client, end, count, interval, tolerance, tail_ms, cancelled=lambda:False, baseline=None, expected_gains=None):
    def check_cancelled():
        if cancelled():
            raise ValueError("Trial cancelled before motion was sent")
    check_cancelled()
    if not (-(2**31) <= end < 2**31 and 5 <= count <= 160 and 1 <= interval <= 65535 and
            1 <= tolerance <= 100000 and 3*interval <= tail_ms < (count-2)*interval and tail_ms % interval == 0):
        raise ValueError("Invalid endpoint or recording window")
    old = client.read(p.TRACE_STATUS)
    if not old[3]&QUALIFIED_STATUS:
        raise ValueError("Firmware does not report trace qualification; update before running trials")
    if old[3]&1:
        raise ValueError("Stop the existing recording first")
    gains, context = configuration(client)
    drive, motion, protection, limits = context
    status = client.status()
    control = client.read(p.GET_CONTROL)
    run = client.run_status()
    if (status['state'] != 2 or status['mode'] != 1 or status['faults'] or status['observed'] or
            status['warnings'] or status['position_lost'] or run['pending_fault'] or
            run['flags']&63 != 47 or control[3] or
            abs(status['actual']-status['target']) > tolerance or abs(status['speed_counts_s']) > protection[2]):
        raise ValueError("Arm position mode at a healthy, stationary, referenced starting position first")
    start = status['target']
    if not limits[2] or protection[6] != 2 or not (limits[0] <= min(start,end,status['actual']) and max(start,end,status['actual']) <= limits[1]):
        raise ValueError("Comparative test moves require enabled travel limits and protect policy")
    if abs(end-start) <= tolerance:
        raise ValueError("Declared move must exceed the settling tolerance")
    if baseline is not None and (context != baseline.context or
            (start,end,interval,tolerance,tail_ms,gains[3:]) !=
            (baseline.start,baseline.end,baseline.interval,baseline.tolerance,baseline.tail_ms,baseline.gains[3:])):
        raise ValueError("Return to the baseline start and use identical move and machine settings before recording")
    if expected_gains is not None and gains != tuple(expected_gains):
        raise ValueError("Applied gains do not match the reviewed trial candidate")
    check_cancelled()
    client.write(p.TRACE_START,count,interval)
    # Confirm at least one baseline sample before issuing motion. No motion retries.
    for _ in range(3):
        recorded, requested, spacing, flags = client.read(p.TRACE_STATUS)
        if requested != count or spacing != interval or flags != QUALIFIED_STATUS|1:
            raise ValueError("Recording not ready or no longer qualified; no move sent")
        if recorded:
            break
    else:
        raise ValueError("No baseline sample; no move sent")
    # Recheck immediately before sending motion: the start may have changed
    # while the trace was acquiring its first sample.
    current = client.status()
    after = client.run_status()
    if (after['flags']&63 != 47 or after['pending_fault'] or
            current['state'] != 2 or current['mode'] != 1 or current['faults'] or
            current['observed'] or current['warnings'] or current['position_lost'] or
            current['target'] != start or abs(current['actual']-start)>tolerance or
            abs(current['speed_counts_s'])>protection[2]):
        raise ValueError("Output or feedback changed; no move sent")
    capture = Capture(gains,context,start,end,count,interval,tolerance,tail_ms,run['tick'],after['tick'])
    check_cancelled()
    client.write(p.MOVE_ABS,end)
    return capture


def finish_trial(client, capture):
    status = client.status()
    run = client.run_status()
    if status['state'] != 0 or status['faults'] or run['flags']&1 or run['pending_fault']:
        raise ValueError("Wait for recording to complete, then disable output before retrieval")
    if configuration(client) != (capture.gains,capture.context):
        raise ValueError("Configuration changed since the trial began")
    expected = (capture.count,capture.count,capture.interval,QUALIFIED_STATUS)
    if client.read(p.TRACE_STATUS) != expected:
        raise ValueError("Recording incomplete, faulted, or interrupted")
    rows, interval, faulted = client.read_trace()
    if client.read(p.TRACE_STATUS) != expected or len(rows) != capture.count or interval != capture.interval:
        raise ValueError("Recording changed during retrieval")
    status, run = client.status(), client.run_status()
    if status['state'] != 0 or status['faults'] or run['flags']&1 or run['pending_fault'] or configuration(client) != (capture.gains,capture.context):
        raise ValueError("Controller state or configuration changed during trace retrieval")
    if ((rows[0][0]-capture.tick_before)&0xffffffff) > ((capture.tick_after-capture.tick_before)&0xffffffff):
        raise ValueError("Recording does not belong to the started trial")
    return make_trial(rows,interval,faulted,capture.gains,capture.context,capture.start,capture.end,
                      capture.tolerance,capture.tail_ms,capture.context[0][2])


def inspect_trial(client, capture):
    """One non-blocking progress check; never commands motion or disables output.

    Return (recorded, stage, trial). The operator must disable the motor before
    reading the trace; a completed trial is retrieved automatically when safe.
    """
    recorded, count, interval, flags = client.read(p.TRACE_STATUS)
    if (count, interval) != (capture.count, capture.interval) or recorded > count:
        raise ValueError("Trial recording changed")
    if flags == QUALIFIED_STATUS|1 and recorded < count:
        run = client.run_status()
        if run['pending_fault'] or run['flags']&63 != 47:
            raise ValueError("Output or feedback changed during trial")
        elapsed = (run['tick']-capture.tick_before)&0xffffffff
        if elapsed > (count+2)*interval+1000:
            raise ValueError("Trial recording did not complete in its expected window")
        return recorded, "recording", None
    if flags != QUALIFIED_STATUS or recorded != count:
        raise ValueError("Trial recording incomplete, faulted or interrupted")
    status = client.status()
    run = client.run_status()
    if status['faults'] or run['pending_fault']:
        raise ValueError("Controller fault after trial")
    if status['state'] != 0 or run['flags']&1:
        return recorded, "disable", None
    return recorded, "complete", finish_trial(client, capture)


def apply_trial_gains(client, candidate, baseline):
    """Apply one explicitly selected candidate to RAM, without arming or saving."""
    candidate = tuple(candidate)
    if (len(candidate)!=5 or any(type(v) is not int for v in candidate) or
            not all(0<=v<=1048576 for v in candidate[:3]) or candidate[3:]!=baseline.gains[3:]):
        raise ValueError("Candidate must preserve the baseline deadband and output limit")
    def check_disabled():
        s, r = client.status(), client.run_status()
        if s['state']!=0 or s['faults'] or r['pending_fault'] or r['flags']&19:
            raise ValueError("Disable motor output and external EN before applying gains")
    check_disabled()
    current, context = configuration(client)
    if context!=baseline.context or current[3:]!=baseline.gains[3:]:
        raise ValueError("Machine configuration changed since baseline; record a new comparison group")
    check_disabled()
    client.write(p.SET_PID,*candidate)
    if client.read(p.GET_PID)!=candidate:
        raise ValueError("PID readback does not match the requested gains")
    return candidate

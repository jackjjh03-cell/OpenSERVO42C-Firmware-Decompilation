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


def begin_trial(client, end, count, interval, tolerance, tail_ms, cancelled=lambda:False):
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
    after = client.run_status()
    if after['flags']&63 != 47 or after['pending_fault']:
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
    if ((rows[0][0]-capture.tick_before)&0xffffffff) > ((capture.tick_after-capture.tick_before)&0xffffffff):
        raise ValueError("Recording does not belong to the started trial")
    return make_trial(rows,interval,faulted,capture.gains,capture.context,capture.start,capture.end,
                      capture.tolerance,capture.tail_ms,capture.context[0][2])

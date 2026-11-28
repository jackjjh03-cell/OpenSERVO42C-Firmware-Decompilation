"""Rank measured PID trials, without sending commands or changing gains."""
from dataclasses import dataclass
from .tuning import analyze

PROFILES = ("Precision", "Balanced", "Speed", "Low effort")
# Low effort is a current-demand proxy, not a sound measurement.
WEIGHTS = ((5, 5, 1, 1), (3, 3, 3, 1), (1, 2, 6, 1), (2, 2, 1, 5))


@dataclass(frozen=True)
class Trial:
    gains: tuple
    context: tuple
    start: int
    end: int
    interval: int
    tolerance: int
    tail_ms: int
    move_ms: int
    metrics: dict


def make_trial(rows, interval, faulted, gains, context, start, end, tolerance,
               tail_ms, phase_limit):
    """Use the declared endpoint, not merely the final target in the trace."""
    gains = tuple(gains)
    if len(gains) != 5 or any(type(v) is not int for v in gains):
        raise ValueError("Invalid gain configuration")
    if not (all(0 <= v <= 1048576 for v in gains[:3]) and
            0 <= gains[3] <= 100000 and 0 < gains[4] <= 32767):
        raise ValueError("Invalid gain configuration")
    if not 0 < phase_limit <= 2047 or faulted or interval < 1 or tolerance < 1:
        raise ValueError("Need valid non-faulted trace and current limit")
    if tail_ms < 3*interval or tail_ms % interval:
        raise ValueError("Post-move window must be at least three sample intervals and divisible by interval")
    if len(rows) < 5 or abs(end-start) <= tolerance:
        raise ValueError("Move is too short or no trace is available")
    if any(((b[0]-a[0]) & 0xffffffff) != interval for a,b in zip(rows,rows[1:])):
        raise ValueError("Irregular sample intervals")
    if rows[0][2] != start or rows[-1][2] != end:
        raise ValueError("Trace does not cover the declared start and endpoint")
    first = next((i for i,r in enumerate(rows) if r[2] != start), None)
    finish = next((i for i,r in enumerate(rows) if r[2] == end), None)
    if first is None or first < 1 or finish is None or finish < first:
        raise ValueError("No complete move")
    last = finish + tail_ms//interval
    if last >= len(rows):
        raise ValueError("Trace ends before the post-move window")
    if any(r[2] != end for r in rows[finish:]):
        raise ValueError("Target changed after reaching the endpoint")
    window = rows[first-1:last+1]
    metrics = analyze(window, interval, False, tolerance)
    if metrics['settling_ms'] is None:
        raise ValueError("Response did not settle within the observation window")
    if any(abs(r[1]-end)>tolerance for r in rows[last+1:]):
        raise ValueError("Response left the settling band after the observation window")
    limit = min(gains[4], phase_limit)
    if (metrics['peak_demand'] >= limit or metrics['peak_load_permille'] >= 1000 or
            any(abs(r[3]) >= limit or r[4] >= 1000 for r in rows)):
        raise ValueError("Trial reached current or demand saturation")
    metrics['rms_demand_fraction'] = (sum((r[3]/limit)**2 for r in window)/len(window))**0.5
    return Trial(gains, tuple(context), start, end, interval, tolerance, tail_ms,
                 (finish-first+1)*interval, metrics)


def trial_key(t):
    # Changing current/output limits or deadband invalidates a comparison.
    return (t.context, t.start, t.end, t.interval, t.tolerance, t.tail_ms, t.gains[3:])


def rank_trials(trials, profile="Balanced"):
    if profile not in PROFILES:
        raise ValueError("Unknown tuning profile")
    if not trials:
        raise ValueError("Record at least one eligible trial")
    baseline = trials[0]
    if any(trial_key(t) != trial_key(baseline) or abs(t.move_ms-baseline.move_ms)>baseline.interval for t in trials):
        raise ValueError("Use the same move, drive, protection, motion settings and observation window")
    weights = WEIGHTS[PROFILES.index(profile)]
    def score(t):
        m = t.metrics
        return sum(w*v for w,v in zip(weights, (m['rms_error_counts']/t.tolerance,
            m['overshoot_counts']/t.tolerance, m['settling_ms']/t.tail_ms,
            m['rms_demand_fraction'])))
    # Repeated gains are ranked by mean score, not their single best run.
    groups = {}
    for t in trials:
        groups.setdefault(t.gains, []).append(score(t))
    return sorted(((sum(scores)/len(scores), gains, len(scores)) for gains,scores in groups.items()),
                  key=lambda r:(r[0],r[1]))

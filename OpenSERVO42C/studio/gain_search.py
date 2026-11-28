"""Bounded integer coordinate search over measured PID trials.

A suggestion is not a stability guarantee. This module never writes gains,
arms output or issues a move. Deadband and output limit are held fixed.
"""
from dataclasses import dataclass
from .tuning_trials import rank_trials

GAIN_MAX = 1048576


@dataclass(frozen=True)
class Suggestion:
    best: tuple
    candidate: tuple
    steps: tuple


def suggest(trials, profile, upper, percent=10, rejected=(), repeats=1):
    """Return the next unmeasured neighbor of the best measured configuration.

    The caller supplies per-gain upper bounds (zero locks a zero gain).
    Search progressively halves integer steps when all neighbors at a scale
    have been measured or explicitly rejected. No gain or current limit is
    ever changed by this function. Exhaustion is local, not a global optimum.
    """
    upper = tuple(upper)
    if len(upper) != 3 or any(type(v) is not int or not 0 <= v <= GAIN_MAX for v in upper):
        raise ValueError("Enter three gain upper bounds in the firmware Q12 range")
    if type(percent) is not int or not 1 <= percent <= 100:
        raise ValueError("Initial step must be 1–100 percent of each upper bound")
    if type(repeats) is not int or not 1 <= repeats <= 10:
        raise ValueError("Required runs per gain set must be 1–10")
    ranking = rank_trials(trials, profile)
    if any(any(not 0 <= g <= cap for g, cap in zip(row[1][:3], upper)) for row in ranking):
        raise ValueError("Search bounds must include all measured gains; clear comparisons to change the search domain")
    excluded = {tuple(g) for g in rejected}
    available = [row for row in ranking if row[1] not in excluded]
    if not available:
        raise ValueError("No measured gain sets remain eligible; reset skipped candidates")
    steps = tuple(max(1, (cap * percent + 99)//100) if cap else 0 for cap in upper)
    complete = [row for row in available if row[2] >= repeats]
    # Finish repeat measurements before comparing or proposing another neighbor.
    # Use first-seen order, not the promising score of a single run.
    counts = {row[1]:row[2] for row in available}
    best = complete[0][1] if complete else available[0][1]
    for trial in trials:
        if trial.gains in counts and counts[trial.gains] < repeats:
            return Suggestion(best, trial.gains, steps)
    seen = {row[1] for row in ranking} | excluded
    while True:
        # Test one gain at a time. Alternate directions within each axis.
        for axis, step in enumerate(steps):
            if not step:
                continue
            for direction in (-1, 1):
                values = list(best)
                values[axis] = min(upper[axis], max(0, values[axis] + direction*step))
                candidate = tuple(values)
                if candidate not in seen:
                    return Suggestion(best, candidate, steps)
        smaller = tuple((max(1, step//2) if step else 0) for step in steps)
        if smaller == steps:
            return Suggestion(best, None, steps)
        steps = smaller

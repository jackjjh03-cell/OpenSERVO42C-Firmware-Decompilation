"""Persist non-machine UI preferences, never command values or drive settings."""
import json
import os
from pathlib import Path

RANGES = ('10 s', '30 s', '60 s', '120 s', '300 s')
VIEWS = ('Position', 'Error', 'Speed', 'Load')
DEFAULTS = {'port':'COM3', 'graph_range':'60 s', 'graph_view':'Position'}


def path():
    return Path(os.environ.get('APPDATA') or Path.home()/'.config')/'OpenSERVO42C'/'studio.json'


def validate(value):
    result = dict(DEFAULTS)
    if isinstance(value, dict):
        port = value.get('port')
        if isinstance(port, str) and 0 < len(port) <= 260 and all(ord(c)>=32 for c in port):
            result['port'] = port
        if value.get('graph_range') in RANGES:
            result['graph_range'] = value['graph_range']
        if value.get('graph_view') in VIEWS:
            result['graph_view'] = value['graph_view']
    return result


def load(location):
    try:
        with location.open('r', encoding='utf-8') as f:
            return validate(json.loads(f.read(8193)))
    except (OSError, ValueError):
        return dict(DEFAULTS)


def save(location, values):
    location.parent.mkdir(parents=True, exist_ok=True)
    tmp = location.with_suffix('.tmp')
    try:
        tmp.write_text(json.dumps(validate(values), indent=2)+'\n', encoding='utf-8')
        os.replace(str(tmp), str(location))
    finally:
        if tmp.exists():
            tmp.unlink()


def graph_series(history, seconds, view):
    """Select an elapsed-time window; columns remain in controller-native units."""
    if view not in VIEWS or seconds <= 0:
        raise ValueError('Invalid graph view')
    if not history:
        return [], []
    cutoff = history[-1][0]-seconds
    rows = [r for r in history if r[0]>=cutoff]
    if view == 'Position':
        return rows, [(1,'Actual','#66b8f5'),(2,'Target','#eea34a')]
    index = {'Speed':3, 'Load':4}.get(view)
    if index is not None:
        return [(r[0],r[index]/10 if view=='Load' else r[index]) for r in rows], [(1,view,'#66b8f5')]
    return [(r[0],r[2]-r[1]) for r in rows], [(1,'Target − actual','#eea34a')]

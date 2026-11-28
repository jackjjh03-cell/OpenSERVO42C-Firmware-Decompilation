"""Portable RAM configuration snapshots. Import never sends controller commands."""
import json
import struct
from . import protocol as p

GROUPS = (
    ('Protection', p.GET_CONFIG, p.CONFIG), ('PID', p.GET_PID, p.PID_CONFIG),
    ('Motion', p.GET_MOTION, p.MOTION_CONFIG), ('Drive', p.GET_DRIVE, p.DRIVE_DATA),
    ('Travel', p.GET_LIMITS, p.LIMITS_DATA),
)


def validate_group(name, values):
    """Validate one settings editor using the same rules as profile import."""
    fmt = next((fmt for group, _, fmt in GROUPS if group == name), None)
    if fmt is None:
        raise ValueError('Unknown settings group: '+str(name))
    if not isinstance(values, (list, tuple)) or any(type(v) is not int for v in values):
        raise ValueError(name+': enter whole numbers in every field')
    try:
        fmt.pack(*values)
    except (ValueError, OverflowError, TypeError, struct.error) as exc:
        raise ValueError(name+': field count or numeric range is invalid') from exc
    v = tuple(values)
    if name == 'Protection':
        if not (v[0]>0 and v[1]>0 and 0<v[3]<=1000 and v[4]>0 and v[5]>0 and v[6]<=2):
            raise ValueError('Protection: errors and dwell times must be positive, stall load 1–1000, policy 0–2')
    elif name == 'PID':
        if not (all(x<=1048576 for x in v[:3]) and v[3]<=100000 and 0<v[4]<=32767):
            raise ValueError('PID: gains 0–1048576, deadband 0–100000, output limit 1–32767')
    elif name == 'Motion':
        if not (0<v[0]<=1000000 and 0<v[1]<=100000000 and 0<v[2]<=100000000 and 0<v[3]<=1000000000 and v[4]<=1):
            raise ValueError('Motion: speed 1–1000000, acceleration/deceleration 1–100000000, jerk 1–1000000000, curve 0 or 1')
    elif name == 'Drive':
        if not (0<v[0]<=256 and 0<v[1]<32768 and 0<v[2]<=2047 and v[3]>0 and 0<v[4]<=1080 and v[5] in (25,50) and v[6]<=1 and v[2]<v[7]<=4095 and v[8]<=1):
            raise ValueError('Drive: microsteps 1–256, encoder delta 1–32767, current 1–2047, gain positive, PWM 1–1080, geometry 25/50, flags 0/1; trip current must exceed phase current and be at most 4095')
    elif name == 'Travel':
        if not (v[0]<v[1] and v[2]<=1):
            raise ValueError('Travel: minimum must be below maximum; enabled must be 0 or 1')
    return v


def validate(groups):
    if not isinstance(groups, dict) or set(groups) != {name for name, _, _ in GROUPS}:
        raise ValueError('Profile must contain all five configuration groups')
    return {name:validate_group(name,groups[name]) for name, _, _ in GROUPS}


def dumps(groups):
    return json.dumps({'format':'OpenSERVO42C-profile','version':1,'settings':validate(groups)}, indent=2)+'\n'


def loads(data):
    obj=json.loads(data)
    if not isinstance(obj,dict) or set(obj)!={'format','version','settings'} or obj['format']!='OpenSERVO42C-profile' or type(obj['version']) is not int or obj['version']!=1:
        raise ValueError('Unsupported profile format')
    return validate(obj['settings'])


def capture(client):
    # The profile contains normal RAM settings, not transient live gains or calibration.
    def idle():
        if client.run_status()['flags']&19 or client.read(p.GET_LIVE_PID)[0]:
            raise ValueError('Disable output and external EN; finish teaching before capturing a profile')
    idle()
    groups={name:client.read(op) for name,op,_ in GROUPS}
    idle()
    return validate(groups)

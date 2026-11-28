"""Portable RAM configuration snapshots. Import never sends controller commands."""
import json
import struct
from . import protocol as p

GROUPS = (
    ('Protection', p.GET_CONFIG, p.CONFIG), ('PID', p.GET_PID, p.PID_CONFIG),
    ('Motion', p.GET_MOTION, p.MOTION_CONFIG), ('Drive', p.GET_DRIVE, p.DRIVE_DATA),
    ('Travel', p.GET_LIMITS, p.LIMITS_DATA),
)


def validate(groups):
    if not isinstance(groups, dict) or set(groups) != {name for name, _, _ in GROUPS}:
        raise ValueError('Profile must contain all five configuration groups')
    result = {}
    for name, _, fmt in GROUPS:
        values = groups[name]
        if not isinstance(values, (list, tuple)) or any(type(v) is not int for v in values):
            raise ValueError('Invalid integer fields in '+name)
        try:
            fmt.pack(*values)
        except (ValueError, OverflowError, TypeError, struct.error) as exc:
            raise ValueError('Invalid wire fields in '+name) from exc
        result[name] = tuple(values)
    a,b,c,d,e = (result[name] for name, _, _ in GROUPS)
    if not (a[0]>0 and a[1]>0 and 0<a[3]<=1000 and a[4]>0 and a[5]>0 and a[6]<=2):
        raise ValueError('Invalid protection configuration')
    if not (all(v<=1048576 for v in b[:3]) and b[3]<=100000 and 0<b[4]<=32767):
        raise ValueError('Invalid PID configuration')
    if not (0<c[0]<=1000000 and 0<c[1]<=100000000 and 0<c[2]<=100000000 and 0<c[3]<=1000000000 and c[4]<=1):
        raise ValueError('Invalid motion configuration')
    if not (0<d[0]<=256 and 0<d[1]<32768 and 0<d[2]<=2047 and d[3]>0 and 0<d[4]<=1080 and d[5] in (25,50) and d[6]<=1 and d[2]<d[7]<=4095 and d[8]<=1):
        raise ValueError('Invalid drive configuration')
    if not (e[0]<e[1] and e[2]<=1):
        raise ValueError('Invalid travel limits')
    return result


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

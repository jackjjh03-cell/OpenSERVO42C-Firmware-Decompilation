"""Read-only connection discovery, including interrupted commissioning."""
from . import protocol as p
from .client import DeviceRejected, LinkError


def probe(client):
    """Return (operation, version, monitor). Never send a write.

    An interrupted LUT upload excludes HELLO until explicitly aborted. A sweep
    does the same until stopped or timed out. Neither condition authorizes motion.
    """
    try:
        version = client.read(p.HELLO)
    except DeviceRejected as exc:
        if exc.status != 2:
            raise
        for op, label, active in ((p.LUT_STATUS, 'LUT upload', lambda v: bool(v[1])),
                                  (p.CAL_STATUS, 'Calibration sweep', lambda v: bool(v[2]&p.CAL_ACTIVE))):
            try:
                value = client.read(op)
            except DeviceRejected:
                continue
            if active(value):
                return label, None, None
        # A sweep may have ended during the probe. Read again, never write.
        version = client.read(p.HELLO)
    if version[:3] != (0, 1, 1):
        raise LinkError('Unsupported OpenSERVO42C firmware version: {}'.format(version[:3]))
    status = client.status()
    try:
        run = client.run_status()
    except DeviceRejected as exc:
        if exc.status != 3:
            raise
        # Failed board initialization exposes only the bench dispatcher.
        return 'Bench diagnostics only', version, None
    return None, version, (status, run)

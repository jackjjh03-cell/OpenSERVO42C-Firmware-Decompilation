"""Synchronous serial client; call from a single Studio worker, not the UI thread.
Python 3.8-compatible. No command is retried automatically.
"""
import struct
import time
from . import protocol as p


class LinkError(Exception):
    """Transport failed; the outcome of a sent write may be unknown."""


class DeviceRejected(Exception):
    def __init__(self, opcode, status):
        self.opcode, self.status = opcode, status
        super().__init__("Command {} rejected (status {})".format(opcode, status))


class FrameReader:
    def __init__(self):
        self.buffer = bytearray()

    def feed(self, data):
        self.buffer.extend(data)
        packets = []
        while True:
            start = self.buffer.find(p.SYNC)
            if start < 0:
                self.buffer[:] = self.buffer[-1:] if self.buffer[-1:] == p.SYNC[:1] else b""
                return packets
            del self.buffer[:start]
            if len(self.buffer) < 6:
                return packets
            size = self.buffer[5]
            if self.buffer[2] != p.VERSION or size > p.MAX_PAYLOAD:
                del self.buffer[0]
                continue
            end = 8 + size
            if len(self.buffer) < end:
                return packets
            candidate = bytes(self.buffer[:end])
            try:
                packets.append(p.decode(candidate))
            except ValueError:
                del self.buffer[0]
                continue
            del self.buffer[:end]


READ_FORMATS = {
    p.HELLO: struct.Struct("<4B"), p.STATUS: p.STATUS_DATA,
    p.GET_CONFIG: p.CONFIG, p.GET_PID: p.PID_CONFIG,
    p.GET_MOTION: p.MOTION_CONFIG, p.GET_CONTROL: p.CONTROL_DATA,
    p.GET_BENCH: p.BENCH_DATA, p.GET_RUN: p.RUN_DATA,
    p.GET_LIMITS: p.LIMITS_DATA, p.GET_LIVE_PID: p.LIVE_PID_DATA,
    p.GET_STORAGE: p.STORAGE_DATA, p.GET_DRIVE: p.DRIVE_DATA,
    p.GET_TIMING: p.TIMING_DATA, p.GET_RATE: p.RATE_DATA,
    p.GET_TORQUE: p.TORQUE_DATA, p.GET_CALIBRATION: p.CALIBRATION_DATA,
    p.TRACE_STATUS: p.TRACE_STATUS_DATA, p.CAL_STATUS: p.CAL_STATUS_DATA,
}
WRITE_FORMATS = {
    p.SET_CONFIG: p.CONFIG, p.SET_PID: p.PID_CONFIG,
    p.SET_MOTION: p.MOTION_CONFIG, p.SET_LIMITS: p.LIMITS_DATA,
    p.SET_LIVE_PID: p.PID_CONFIG, p.SET_DRIVE: p.DRIVE_DATA,
    p.SELECT_MODE: p.MODE_DATA, p.MOVE_ABS: p.POSITION_DATA,
    p.CONFIRM_REFERENCE: p.POSITION_DATA, p.TEACH_APPLY: p.TEACH_MARGIN_DATA,
    p.SET_RATE: p.RATE_COMMAND, p.SET_TORQUE: p.TORQUE_COMMAND, p.TRACE_START: p.TRACE_START_DATA, p.CAL_START: p.CAL_START_DATA,
}
NO_PAYLOAD = {p.DISABLE, p.ACK, p.ARM, p.TEACH_BEGIN, p.TEACH_CANCEL, p.SAVE_SETTINGS, p.TRACE_STOP, p.CAL_STEP, p.CAL_STOP}
STATUS_FIELDS = ("state", "mode", "faults", "observed", "warnings", "target", "actual",
                 "speed_counts_s", "load_permille", "position_lost")
RUN_FIELDS = ("flags", "pending_fault", "tick", "adc_count", "missed_ticks")
MODE_NAMES = ("CNC", "Position", "Velocity", "Jog", "Torque", "Open loop")


class ControllerClient:
    def __init__(self, port, timeout=0.5, clock=time.monotonic):
        self.port = port
        self.timeout = timeout
        self.clock = clock
        self.sequence = 0
        self.reader = FrameReader()
        if timeout <= 0:
            raise ValueError("timeout must be positive")

    @classmethod
    def open(cls, device, baud=115200, timeout=0.5):
        import serial  # optional until physical connection is requested
        port = serial.Serial(port=None, baudrate=baud, timeout=0.02, write_timeout=timeout)
        port.dtr = False
        port.rts = False
        port.port = device
        try:
            port.open()
            port.reset_input_buffer()
        except Exception:
            port.close()
            raise
        return cls(port, timeout)

    def close(self):
        self.port.close()

    def request(self, opcode, payload=b"", timeout=None):
        if not 0 < opcode < 128:
            raise ValueError("invalid request opcode")
        duration = self.timeout if timeout is None else timeout
        if duration <= 0:
            raise ValueError("timeout must be positive")
        seq = self.sequence
        self.sequence = (seq + 1) & 255
        packet = p.frame(seq, opcode, payload)
        # A partial old frame must not consume the next transaction's response.
        self.reader.buffer.clear()
        try:
            if self.port.write(packet) != len(packet):
                raise LinkError("partial serial write; command outcome unknown")
            deadline = self.clock() + duration
            while self.clock() < deadline:
                for rx_seq, rx_op, data in self.reader.feed(self.port.read(64)):
                    if rx_seq != seq or rx_op != (opcode | 0x80):
                        continue  # stale reply or adapter echo
                    if not data:
                        raise LinkError("missing response status")
                    if data[0]:
                        raise DeviceRejected(opcode, data[0])
                    return data[1:]
        except (LinkError, DeviceRejected):
            raise
        except Exception as exc:
            raise LinkError("serial I/O failed; command outcome unknown") from exc
        raise LinkError("reply timeout for command {}; not retried".format(opcode))

    def read(self, opcode):
        fmt = READ_FORMATS[opcode]
        data = self.request(opcode)
        if len(data) != fmt.size:
            raise LinkError("unexpected reply size for command {}".format(opcode))
        return fmt.unpack(data)

    def write(self, opcode, *values):
        if opcode in NO_PAYLOAD and not values:
            payload = b""
        else:
            payload = WRITE_FORMATS[opcode].pack(*values)
        # Flash programming is slower than an ordinary command; no retries.
        result = self.request(opcode, payload, timeout=5.0 if opcode == p.SAVE_SETTINGS else None)
        if result:
            raise LinkError("unexpected write reply")

    def status(self):
        return dict(zip(STATUS_FIELDS, self.read(p.STATUS)))

    def run_status(self):
        return dict(zip(RUN_FIELDS, self.read(p.GET_RUN)))

    def capture_limit(self, maximum):
        data = self.request(p.TEACH_CAPTURE, bytes((int(bool(maximum)),)))
        if len(data) != p.POSITION_DATA.size:
            raise LinkError("invalid capture reply")
        return p.POSITION_DATA.unpack(data)[0]

    def read_trace(self):
        count, requested, interval, flags = self.read(p.TRACE_STATUS)
        if flags & 1 or count > requested or requested > 160:
            raise LinkError("Trace is running or invalid")
        rows = []
        for offset in range(0, count, 2):
            size = min(2, count-offset)
            header = p.TRACE_READ_DATA.pack(offset,size)
            data = self.request(p.TRACE_READ,header)
            if len(data) != len(header)+size*p.TRACE_SAMPLE.size or data[:len(header)] != header:
                raise LinkError("Trace reply mismatch")
            rows.extend(p.TRACE_SAMPLE.iter_unpack(data[len(header):]))
        return rows, interval, bool(flags & 2)

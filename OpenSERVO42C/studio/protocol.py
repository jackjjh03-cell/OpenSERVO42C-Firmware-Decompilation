"""Wire contract for OpenSERVO42C prototype. Python 3.8+, no GUI or serial I/O."""
import struct
SYNC = b"\xa5\x5a"
VERSION = 1
MAX_PAYLOAD = 48
HELLO, STATUS, GET_CONFIG, SET_CONFIG, DISABLE, ACK = range(1, 7)
GET_PID, SET_PID, GET_MOTION, SET_MOTION, GET_CONTROL = range(7, 12)
CONFIG = struct.Struct("<IIIHHHB")
STATUS_DATA = struct.Struct("<BBIIIiiiHB")
PID_CONFIG = struct.Struct("<IIIIH")
MOTION_CONFIG = struct.Struct("<IIIIB")
CONTROL_DATA = struct.Struct("<iIIB")
GET_BENCH = 12
BENCH_DATA = struct.Struct("<7I8H3B")
BENCH_FIELDS = ("ms", "core_hz", "adc_samples", "adc_errors", "encoder_errors",
                "missed_ticks", "uart_dropped", "scale", "slot1", "slot2",
                "raw_angle", "cal_angle", "pulse", "offset1", "offset2",
                "board_status", "encoder_status", "flags")

def bench_snapshot(payload):
    return dict(zip(BENCH_FIELDS, BENCH_DATA.unpack(payload)))

def crc16(data):
    crc = 0xffff
    for value in data:
        crc ^= value << 8
        for _ in range(8):
            crc = ((crc << 1) ^ (0x1021 if crc & 0x8000 else 0)) & 0xffff
    return crc

def frame(sequence, opcode, payload=b""):
    if len(payload) > MAX_PAYLOAD:
        raise ValueError("payload too long")
    body = bytes((VERSION, sequence, opcode, len(payload))) + payload
    return SYNC + body + struct.pack("<H", crc16(body))

def decode(packet):
    if len(packet) < 8 or packet[:2] != SYNC or packet[2] != VERSION:
        raise ValueError("invalid header")
    size = packet[5]
    if size > MAX_PAYLOAD or len(packet) != 8 + size:
        raise ValueError("invalid length")
    if crc16(packet[2:-2]) != struct.unpack("<H", packet[-2:])[0]:
        raise ValueError("invalid CRC")
    return packet[3], packet[4], packet[6:-2]

def response(packet, sequence, opcode):
    seq, op, payload = decode(packet)
    if seq != sequence or op != opcode | 0x80 or not payload:
        raise ValueError("unexpected reply")
    if payload[0] != 0:
        raise ValueError("controller rejected request: {}".format(payload[0]))
    return payload[1:]

ARM, SELECT_MODE, MOVE_ABS, GET_RUN = range(13, 17)
RUN_DATA = struct.Struct("<BBIII")
MODE_DATA = struct.Struct("<B")
POSITION_DATA = struct.Struct("<i")
GET_LIMITS, SET_LIMITS, CONFIRM_REFERENCE, SET_LIVE_PID, GET_LIVE_PID = range(17, 22)
LIMITS_DATA = struct.Struct("<iiB")
LIVE_PID_DATA = struct.Struct("<BIIIIH")

TEACH_BEGIN, TEACH_CAPTURE, TEACH_APPLY, TEACH_CANCEL = range(22, 26)
TEACH_MARGIN_DATA = struct.Struct("<II")
RUN_TEACHING = 16
RUN_REFERENCE_VALID = 32

GET_STORAGE, SAVE_SETTINGS = range(26, 28)
STORAGE_DATA = struct.Struct("<BBI")

GET_DRIVE, SET_DRIVE = range(28, 30)
DRIVE_DATA = struct.Struct("<5HBBHB")

GET_TIMING = 30
TIMING_DATA = struct.Struct("<6I") # peak core clocks, observed overruns, per loop

SET_RATE, GET_RATE = range(31, 33)
RATE_COMMAND = struct.Struct("<iH") # counts/s, jog lease in ms (zero for velocity)
RATE_DATA = struct.Struct("<iiH") # requested counts/s, ramped milli-counts/s, remaining lease

SET_TORQUE, GET_TORQUE = range(33, 35)
TORQUE_COMMAND = struct.Struct("<iH") # signed raw ADC current counts, lease ms
TORQUE_DATA = struct.Struct("<iiHB") # requested, applied demand, lease, cutoff flags
TORQUE_TIMEOUT, TORQUE_SPEED, TORQUE_LIMIT = 1, 2, 4

GET_CALIBRATION = 35
CALIBRATION_DATA = struct.Struct("<i4HB") # winding, maximum jump, forward, reverse, flat, flags

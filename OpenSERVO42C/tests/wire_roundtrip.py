"""Exercise the real C firmware protocol dispatcher using the Studio codec."""
import os
import subprocess
import sys
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from studio import protocol as p

def main():
    proc = subprocess.Popen([sys.argv[1]], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    seq = 0
    def send(op, payload=b"", corrupt=False):
        nonlocal seq
        seq = (seq + 1) % 256
        raw = p.frame(seq, op, payload)
        if corrupt:
            raw = raw[:-1] + bytes([raw[-1] ^ 1])
        proc.stdin.write(raw.hex() + "\n")
        proc.stdin.flush()
        line = proc.stdout.readline().strip()
        if corrupt:
            assert line == "-"
            return
        return p.response(bytes.fromhex(line), seq, op)
    try:
        assert send(p.HELLO) == bytes([0, 1, 1, 0])
        assert p.STATUS_DATA.unpack(send(p.STATUS))[0] == 0
        config = (0x12345678,0x76543210,0xabcdef01,999,0x2345,0x3456,2)
        assert send(p.SET_CONFIG,p.CONFIG.pack(*config)) == b""
        assert send(p.GET_CONFIG) == p.CONFIG.pack(*config)
        gain = (1024, 32, 200, 3, 7000)
        assert send(p.SET_PID, p.PID_CONFIG.pack(*gain)) == b""
        assert p.PID_CONFIG.unpack(send(p.GET_PID)) == gain
        plan = (24000, 15000, 8000, 200000, 1)
        assert send(p.SET_MOTION, p.MOTION_CONFIG.pack(*plan)) == b""
        assert p.MOTION_CONFIG.unpack(send(p.GET_MOTION)) == plan
        try:
            send(p.SET_MOTION, p.MOTION_CONFIG.pack(0, *plan[1:]))
        except ValueError:
            pass
        else:
            raise AssertionError("invalid speed accepted")
        assert p.MOTION_CONFIG.unpack(send(p.GET_MOTION)) == plan
        send(p.SET_PID, p.PID_CONFIG.pack(2000, *gain[1:]), corrupt=True)
        assert p.PID_CONFIG.unpack(send(p.GET_PID)) == gain
        assert p.CONTROL_DATA.unpack(send(p.GET_CONTROL)) == (0, 0, 0, 0)
        assert send(p.DISABLE) == b""
        print("C/Python protocol round-trip passed")
    finally:
        proc.stdin.close()
        proc.wait(timeout=5)
        proc.stdout.close()
    assert proc.returncode == 0

if __name__ == "__main__":
    main()

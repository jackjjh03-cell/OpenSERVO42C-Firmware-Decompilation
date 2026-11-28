import unittest
from studio import protocol as p
from studio.client import ControllerClient, FrameReader, LinkError, DeviceRejected


class FakePort:
    def __init__(self, reply):
        self.reply = reply
        self.writes = []
        self.chunks = []
        self.time = 0.0

    def write(self, packet):
        self.writes.append(packet)
        seq, op, data = p.decode(packet)
        self.chunks.extend(self.reply(seq, op, data))
        return len(packet)

    def read(self, size):
        self.time += 0.025
        return self.chunks.pop(0) if self.chunks else b""

    def clock(self):
        return self.time


def reply(seq, op, payload=b"", status=0):
    return p.frame(seq, op | 0x80, bytes((status,)) + payload)


class ClientTests(unittest.TestCase):
    def client(self, responder):
        port = FakePort(responder)
        return ControllerClient(port, timeout=0.2, clock=port.clock), port

    def test_reader_resync(self):
        reader = FrameReader()
        good = reply(3, p.HELLO, b"abcd")
        bad = good[:-1] + bytes((good[-1] ^ 1,))
        self.assertEqual(reader.feed(b"noise\xa5"), [])
        self.assertEqual(reader.feed(b"\x5a\x01\x00\x00\xff" + bad + good[:4]), [])
        self.assertEqual(reader.feed(good[4:]), [(3, p.HELLO | 128, b"\x00abcd")])

    def test_status_fragment_echo_stale(self):
        values = (2, 1, 0, 0, 0, 300, 275, -20, 123, 0)
        def responder(seq, op, data):
            good = reply(seq, op, p.STATUS_DATA.pack(*values))
            return [p.frame(seq, op, data), reply((seq-1) & 255, op), good[:2], good[2:]]
        client, port = self.client(responder)
        self.assertEqual(client.status()["actual"], 275)
        self.assertEqual(len(port.writes), 1)

    def test_no_retry_after_timeout(self):
        client, port = self.client(lambda *args: [])
        with self.assertRaises(LinkError):
            client.write(p.MOVE_ABS, 1000)
        self.assertEqual(len(port.writes), 1)
        self.assertEqual(p.decode(port.writes[0])[2], p.POSITION_DATA.pack(1000))

    def test_rejection(self):
        client, port = self.client(lambda seq, op, data: [reply(seq, op, status=2)])
        with self.assertRaises(DeviceRejected) as ctx:
            client.write(p.ARM)
        self.assertEqual(ctx.exception.status, 2)
        self.assertEqual(len(port.writes), 1)

    def test_signed_capture_and_sequence_wrap(self):
        client, port = self.client(lambda seq, op, data: [reply(seq, op, p.POSITION_DATA.pack(-400))])
        client.sequence = 255
        self.assertEqual(client.capture_limit(False), -400)
        self.assertEqual(client.capture_limit(True), -400)
        self.assertEqual([p.decode(x)[0] for x in port.writes], [255, 0])
        self.assertEqual(p.decode(port.writes[1])[2], b"\x01")

    def test_reply_size_and_write(self):
        client, port = self.client(lambda seq, op, data: [reply(seq, op)])
        with self.assertRaises(LinkError):
            client.read(p.GET_DRIVE)
        client.write(p.SET_RATE, -300, 100)
        self.assertEqual(p.decode(port.writes[-1])[2], p.RATE_COMMAND.pack(-300, 100))
        client.write(p.SAVE_SETTINGS)


if __name__ == "__main__":
    unittest.main()

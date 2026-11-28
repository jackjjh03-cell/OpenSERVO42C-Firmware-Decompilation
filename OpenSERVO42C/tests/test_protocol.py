import unittest
from studio.protocol import crc16, frame, decode, response, CONFIG, STATUS_DATA

class ProtocolTests(unittest.TestCase):
    def test_crc(self):
        self.assertEqual(crc16(b"123456789"), 0x29b1)
    def test_frame(self):
        raw = frame(42, 1)
        self.assertEqual(decode(raw), (42, 1, b""))
        self.assertEqual(response(frame(42, 0x81, b"\x00\x00\x01\x00\x00"),42,1),b"\x00\x01\x00\x00")
        with self.assertRaises(ValueError):
            decode(raw[:-1]+bytes([raw[-1]^1]))
        with self.assertRaises(ValueError):
            response(frame(43, 0x81, b"\x00"),42,1)
    def test_sizes(self):
        self.assertEqual(CONFIG.size,19)
        self.assertEqual(STATUS_DATA.size,29)
        with self.assertRaises(ValueError):
            frame(1,1,bytes(49))

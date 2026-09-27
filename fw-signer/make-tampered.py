src = open("signed.bin", "rb").read()
def flip(name, offset):
    b = bytearray(src); b[offset] ^= 0x01
    open(name, "wb").write(b)
flip("tamper_payload.bin", 0x300)  # one bit in code
flip("tamper_version.bin", 0x1B8)  # one bit in firmware_info (MAC'd)
flip("tamper_tag.bin",     0x1C0)  # one bit in the stored MAC

import struct
b = bytearray(src)
struct.pack_into("<I", b, 0x1BC, 0x00100000)   # length field -> 1 MB
open("tamper_length.bin", "wb").write(b)
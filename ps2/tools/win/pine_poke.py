"""Write a 32-bit value to an ELF symbol in the running game (development helper, PCSX2 PINE).

    python ps2/tools/win/pine_poke.py SYMBOL VALUE [unstripped elf]
"""
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pine import Pine  # noqa: E402
from ds2d_dump import sym  # noqa: E402

OP_WRITE32 = 6


def main():
    elf = sys.argv[3] if len(sys.argv) > 3 else "build/obj/khdays-ps2/khdays-ps2.unstripped.elf"
    addr = sym(elf, sys.argv[1])
    val = int(sys.argv[2], 0)
    p = Pine()
    p.s.sendall(struct.pack("<IBII", 5 + 8, OP_WRITE32, addr, val & 0xffffffff))
    p.s.recv(16)
    print("%s @ 0x%08x = 0x%08x" % (sys.argv[1], addr, p.r32(addr)))


if __name__ == "__main__":
    main()

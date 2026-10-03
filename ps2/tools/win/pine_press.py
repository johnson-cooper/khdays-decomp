"""Press PS2 pad buttons in the running game through PCSX2 PINE (development helper).

    python ps2/tools/win/pine_press.py start cross cross down [--hold 150] [--gap 400]

Each name is pressed in turn: held for --hold ms, released, then --gap ms of pause.  Several
buttons at once: join them with '+', e.g. "l1+r1".  Unlike pcsx2_key.ps1 this does not need the
PCSX2 window to have the keyboard focus: it writes kh_dbg_pad_inject (ps2/src/platform/ps2_pad.c),
whose bits are ORed into port 0's buttons at every poll.
"""
import os
import struct
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pine import Pine  # noqa: E402
from ds2d_dump import sym  # noqa: E402

ELF = "build/obj/khdays-ps2/khdays-ps2.unstripped.elf"
OP_WRITE32 = 6
BTN = {
    "select": 1 << 0, "l3": 1 << 1, "r3": 1 << 2, "start": 1 << 3,
    "up": 1 << 4, "right": 1 << 5, "down": 1 << 6, "left": 1 << 7,
    "l2": 1 << 8, "r2": 1 << 9, "l1": 1 << 10, "r1": 1 << 11,
    "triangle": 1 << 12, "circle": 1 << 13, "cross": 1 << 14, "square": 1 << 15,
}


def main():
    args, hold, gap = [], 0.15, 0.4
    it = iter(sys.argv[1:])
    for a in it:
        if a == "--hold":
            hold = int(next(it)) / 1000.0
        elif a == "--gap":
            gap = int(next(it)) / 1000.0
        else:
            args.append(a)
    p = Pine()
    addr = sym(ELF, "kh_dbg_pad_inject")

    def put(v):
        p.s.sendall(struct.pack("<IBII", 5 + 8, OP_WRITE32, addr, v))
        p.s.recv(16)

    for name in args:
        mask = 0
        for part in name.lower().split("+"):
            mask |= BTN[part]
        put(mask)
        time.sleep(hold)
        put(0)
        time.sleep(gap)
        print("pressed", name)


if __name__ == "__main__":
    main()

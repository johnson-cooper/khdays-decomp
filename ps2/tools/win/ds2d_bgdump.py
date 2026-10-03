"""Print the BG screen entries of one DS engine in a screen rectangle (development helper, PINE).

    python ps2/tools/win/ds2d_bgdump.py ENG X0 Y0 X1 Y1     (tile units of the 256x192 screen)

For every enabled text BG: scroll, and per visible tile the screen entry as tile:palette(+h/v flip),
plus the 16 colours of every palette used (BGR555), read from the running game.
"""
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pine import Pine  # noqa: E402
from ds2d_dump import sym  # noqa: E402

ELF = "build/obj/khdays-ps2/khdays-ps2.unstripped.elf"
BANK_SIZE = [0x20000, 0x20000, 0x20000, 0x20000, 0x10000, 0x4000, 0x4000, 0x8000, 0x4000]


def main():
    eng, x0, y0, x1, y1 = (int(a) for a in sys.argv[1:6])
    p = Pine()
    io = sym(ELF, "kh_ds_io") + eng * 0x1000
    view_banks = p.r32(sym(ELF, "g_view_banks") + (2 if eng else 0) * 4)
    bank_ofs = [p.r32(sym(ELF, "kh_ds_vram_bank_ofs") + b * 4) for b in range(9)]
    vram = sym(ELF, "kh_ds_vram")
    pal = sym(ELF, "kh_ds_pal") + eng * 0x400

    def view(ofs):
        for b in range(9):
            if view_banks & (1 << b):
                if ofs < BANK_SIZE[b]:
                    return vram + bank_ofs[b] + ofs
                ofs -= BANK_SIZE[b]
        return None

    disp = p.r32(io)
    print(f"DISPCNT {disp:08x} view banks {view_banks:03x}")
    used = set()
    for bg in range(4):
        if not (disp >> (8 + bg)) & 1:
            continue
        cnt = p.r16(io + 8 + bg * 2)
        hofs = p.r16(io + 0x10 + bg * 4) & 0x1ff
        vofs = p.r16(io + 0x12 + bg * 4) & 0x1ff
        size = cnt >> 14
        mw, mh = (64 if size & 1 else 32), (64 if size & 2 else 32)
        scr = ((cnt >> 8) & 31) * 0x800 + (0 if eng else ((disp >> 27) & 7) * 0x10000)
        print(f"BG{bg} cnt {cnt:04x} hofs {hofs} vofs {vofs} map {mw}x{mh} scr 0x{scr:x} 8bpp {(cnt >> 7) & 1}")
        for ty in range(y0, y1 + 1):
            row = []
            for tx in range(x0, x1 + 1):
                mx = ((hofs >> 3) + tx) & (mw - 1)
                my = ((vofs >> 3) + ty) & (mh - 1)
                blk = (mx >> 5) + ((my >> 5) * 2 if mw == 64 else (my >> 5))
                a = view(scr + (blk * 1024 + (my & 31) * 32 + (mx & 31)) * 2)
                e = p.r16(a) if a else 0xffff
                row.append(f"{e & 0x3ff:3x}:{e >> 12:x}{'h' if e & 0x400 else ' '}{'v' if e & 0x800 else ' '}")
                if e & 0x3ff:
                    used.add(e >> 12)
            print(f"  ty {ty:2d} " + " ".join(row))
    for pl in sorted(used):
        cols = [p.r16(pal + pl * 32 + i * 2) for i in range(16)]
        print(f"palette {pl:2d}: " + " ".join(f"{c:04x}" for c in cols))


if __name__ == "__main__":
    main()

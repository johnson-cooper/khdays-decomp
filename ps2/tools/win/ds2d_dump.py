"""Dump the DS 2D state the PS2 compositor reads (development helper, PCSX2 PINE).

    python ps2/tools/win/ds2d_dump.py [unstripped elf]

Prints DISPCNT, BGxCNT, blend registers and every enabled OAM entry of both engines, decoded,
from kh_ds_io / kh_ds_oam in the running game.
"""
import os
import subprocess
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pine import Pine  # noqa: E402

NM = os.path.expandvars(r"%LOCALAPPDATA%\ps2build\toolchain\ee\bin\mips64r5900el-ps2-elf-nm.exe")


def sym(elf, name):
    for ln in subprocess.run([NM, elf], capture_output=True, text=True).stdout.splitlines():
        p = ln.split()
        if len(p) == 3 and p[2] == name:
            return int(p[0], 16)
    raise KeyError(name)


def read(pine, addr, n):
    out = b""
    for a in range(addr, addr + n, 4):
        out += struct.pack("<I", pine.r32(a))
    return out


def main():
    elf = sys.argv[1] if len(sys.argv) > 1 else "build/obj/khdays-ps2/khdays-ps2.unstripped.elf"
    pine = Pine()
    io = read(pine, sym(elf, "kh_ds_io"), 0x1070)
    oam = read(pine, sym(elf, "kh_ds_oam"), 0x800)
    for eng in range(2):
        b = eng * 0x1000
        disp, = struct.unpack_from("<I", io, b)
        print(f"engine {'AB'[eng]}: DISPCNT {disp:08x} mode {disp & 7} 3D {(disp >> 3) & 1} "
              f"objmap1d {(disp >> 4) & 1} bg {(disp >> 8) & 15:04b} obj {(disp >> 12) & 1} "
              f"win {(disp >> 13) & 7:03b} bgextpal {(disp >> 30) & 1} objextpal {(disp >> 31) & 1}")
        for i in range(4):
            cnt, = struct.unpack_from("<H", io, b + 8 + i * 2)
            print(f"  BG{i}CNT {cnt:04x} prio {cnt & 3} char {(cnt >> 2) & 15} mosaic {(cnt >> 6) & 1} "
                  f"8bpp {(cnt >> 7) & 1} scr {(cnt >> 8) & 31} ovf/extslot {(cnt >> 13) & 1} size {cnt >> 14}")
        bld, alpha, y = struct.unpack_from("<HHH", io, b + 0x50)
        print(f"  BLDCNT {bld:04x} BLDALPHA {alpha:04x} BLDY {y:04x} MASTER_BRIGHT {struct.unpack_from('<H', io, b + 0x6c)[0]:04x}")
        for i in range(128):
            a0, a1, a2 = struct.unpack_from("<HHH", oam, eng * 0x400 + i * 8)
            if (a0 & 0x300) == 0x200:
                continue
            if a0 == 0 and a1 == 0 and a2 == 0:
                continue
            aff = (a0 >> 8) & 1
            print(f"  OBJ{i:3d} y {a0 & 0xff:3d} x {a1 & 0x1ff:3d} affine {aff} dbl {(a0 >> 9) & 1 if aff else 0} "
                  f"mode {(a0 >> 10) & 3} mosaic {(a0 >> 12) & 1} 8bpp {(a0 >> 13) & 1} shape {a0 >> 14} "
                  f"size {a1 >> 14} {'affidx %d' % ((a1 >> 9) & 31) if aff else 'hf %d vf %d' % ((a1 >> 12) & 1, (a1 >> 13) & 1)} "
                  f"tile {a2 & 0x3ff} prio {(a2 >> 10) & 3} pal {a2 >> 12}")


if __name__ == "__main__":
    main()

#!/usr/bin/env python3
"""Minimal Nintendo DS ROM / NitroFS reader (no third-party dependencies).

    python ps2/tools/ndsfs.py info    rom.nds
    python ps2/tools/ndsfs.py list    rom.nds [--magic]
    python ps2/tools/ndsfs.py extract rom.nds OUTDIR

Used by the PS2 asset pipeline (ps2/tools/make_ps2data.py).  Nothing extracted
from the ROM is ever committed to the repository.
"""
import collections
import os
import struct
import sys


class Rom:
    def __init__(self, path):
        self.path = path
        self.f = open(path, "rb")
        h = self.read(0, 0x200)
        self.header = h
        self.title = h[0:12].rstrip(b"\0").decode("ascii", "replace")
        self.gamecode = h[12:16].decode("ascii", "replace")
        (self.arm9_off, self.arm9_entry, self.arm9_addr, self.arm9_size,
         self.arm7_off, self.arm7_entry, self.arm7_addr, self.arm7_size,
         self.fnt_off, self.fnt_size, self.fat_off, self.fat_size,
         self.ov9_off, self.ov9_size, self.ov7_off, self.ov7_size) = struct.unpack_from("<16I", h, 0x20)
        fat = self.read(self.fat_off, self.fat_size)
        self.fat = [struct.unpack_from("<II", fat, i * 8) for i in range(self.fat_size // 8)]
        self.files = {}   # file id -> path
        self.dirs = {}    # dir id -> path
        self._walk_fnt()
        self.overlay_file_ids = set()
        ovt = self.read(self.ov9_off, self.ov9_size)
        for i in range(self.ov9_size // 32):
            self.overlay_file_ids.add(struct.unpack_from("<I", ovt, i * 32 + 0x18)[0])

    def read(self, off, size):
        self.f.seek(off)
        return self.f.read(size)

    def _walk_fnt(self):
        fnt = self.read(self.fnt_off, self.fnt_size)

        def walk(dir_id, prefix):
            sub_off, first_file, _ = struct.unpack_from("<IHH", fnt, (dir_id & 0xFFF) * 8)
            self.dirs[dir_id] = prefix
            pos = sub_off
            fid = first_file
            while True:
                b = fnt[pos]
                pos += 1
                if b == 0:
                    break
                ln = b & 0x7F
                name = fnt[pos:pos + ln].decode("latin-1")
                pos += ln
                if b & 0x80:
                    sub_id = struct.unpack_from("<H", fnt, pos)[0]
                    pos += 2
                    walk(sub_id, prefix + name + "/")
                else:
                    self.files[fid] = prefix + name
                    fid += 1

        walk(0xF000, "")

    def file_data(self, fid):
        start, end = self.fat[fid]
        return self.read(start, end - start)


def magic_of(data):
    m = data[:4]
    if len(m) == 4 and all(32 <= c < 127 for c in m):
        return m.decode()
    if data[:1] in (b"\x10", b"\x11") and len(data) > 4:
        return "LZ%02x" % data[0]
    return "bin"


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 1
    cmd, rom = argv[1], Rom(argv[2])
    if cmd == "info":
        print(f"title={rom.title} gamecode={rom.gamecode}")
        print(f"arm9 off=0x{rom.arm9_off:x} addr=0x{rom.arm9_addr:x} size=0x{rom.arm9_size:x}")
        print(f"arm7 off=0x{rom.arm7_off:x} addr=0x{rom.arm7_addr:x} size=0x{rom.arm7_size:x}")
        print(f"files={len(rom.files)} dirs={len(rom.dirs)} fat_entries={len(rom.fat)} overlays={len(rom.overlay_file_ids)}")
        total = sum(rom.fat[i][1] - rom.fat[i][0] for i in rom.files)
        print(f"named file bytes={total} ({total / 1048576:.1f} MiB)")
    elif cmd == "list":
        withmagic = "--magic" in argv
        for fid in sorted(rom.files):
            s, e = rom.fat[fid]
            extra = ""
            if withmagic:
                extra = " " + magic_of(rom.read(s, 16))
            print(f"{fid:5d} {e - s:9d} {rom.files[fid]}{extra}")
    elif cmd == "extract":
        out = argv[3]
        for fid, p in rom.files.items():
            dst = os.path.join(out, p)
            os.makedirs(os.path.dirname(dst) or out, exist_ok=True)
            with open(dst, "wb") as f:
                f.write(rom.file_data(fid))
    elif cmd == "stats":
        c = collections.Counter()
        sz = collections.Counter()
        for fid, p in rom.files.items():
            s, e = rom.fat[fid]
            ext = os.path.splitext(p)[1].lower() or "(none)"
            key = ext + " " + magic_of(rom.read(s, 16))
            c[key] += 1
            sz[key] += e - s
        for k, n in c.most_common():
            print(f"{n:6d} {sz[k] / 1048576:8.2f} MiB  {k}")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))

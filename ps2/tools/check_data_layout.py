#!/usr/bin/env python3
"""Check that every DS pure-data file compiles to an object that fits its DS address range.

gen_link.py places these objects at their DS-relative addresses; an object that is larger than
its DS range, or whose section alignment does not divide its DS address, cannot be placed (ld
reports "cannot move location counter backwards").  This lists every such file at once.

    python ps2/tools/check_data_layout.py
"""
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ps2cfg  # noqa: E402

OBJDUMP = os.path.join(ps2cfg.sdk_root(), "toolchain", "ee", "bin",
                       "mips64r5900el-ps2-elf-objdump" + (".exe" if os.name == "nt" else ""))
KINDS = (".data", ".rodata", ".bss")


def main():
    objs = {}
    for dp, _, fs in os.walk(os.path.join(ps2cfg.ROOT, "build", "obj")):
        for f in fs:
            if f.endswith(".o"):
                path = os.path.join(dp, f)
                # a prepared copy (ps2/gen/src) supersedes an older object of the original
                if f not in objs or "gen" + os.sep + "src" in path:
                    objs[f] = path
    bad = 0
    for relp, (mod, secs) in sorted(ps2cfg.ds_data_files().items()):
        obj = objs.get(os.path.splitext(os.path.basename(relp))[0] + ".o")
        if not obj:
            continue
        out = subprocess.run([OBJDUMP, "-h", obj], capture_output=True, text=True).stdout
        ps2 = []   # (name, size, align)
        for line in out.splitlines():
            p = line.split()
            if len(p) >= 7 and p[1].split(".")[1:2] and any(p[1] == k or p[1].startswith(k + ".") for k in KINDS):
                size, align = int(p[2], 16), 2 ** int(p[6].split("**")[1])
                if size:
                    ps2.append((p[1], size, align))
        groups = [(secs[k], [s for s in ps2 if k == ".data" and not s[0].startswith(".rodata")
                             or k == ".rodata" and s[0].startswith(".rodata")])
                  for k in secs] if len(secs) == 2 else [(list(secs.values())[0], ps2)]
        for (start, end), parts in groups:
            pos = start
            for name, size, align in parts:
                if pos % align:
                    print(f"{relp}: {name} needs {align}-byte alignment at DS 0x{pos:08x}")
                    bad += 1
                    pos = (pos + align - 1) // align * align
                pos += size
            if pos > end:
                print(f"{relp}: {pos - start:#x} bytes on the PS2, DS range {end - start:#x} "
                      f"({', '.join(f'{n} {s:#x}/{a}' for n, s, a in parts)})")
                bad += 1
    print(f"{bad} problem(s)")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())

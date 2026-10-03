#!/usr/bin/env python3
"""Post-link R5900 audit for suspicious 64-bit memory instructions.

The EE raises an address error for ordinary ld/sd on a non-8-byte address.  Source preparation
(R19 in prep_sources.py) rewrites confirmed packed fields and mechanically routes direct 64-bit
pointer dereferences through alignment-1 lvalue slots.  This tool inspects the code GCC actually
emitted so those fixes cannot silently regress because of a compiler/source-shape change.

A repository-wide list of ld/sd instructions whose immediate is not a multiple of eight is also
written to build/ps2/r5900_ldsd.txt.  The immediate alone cannot prove the effective address is
misaligned (the base register may itself be offset), so that wider list is evidence for review,
not an automatic failure.  In functions touched by confirmed or automatic R19 handling, however, such an instruction is a
regression candidate and fails the build.
"""
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ps2cfg  # noqa: E402
import prep_sources  # noqa: E402

ROOT = ps2cfg.ROOT
KNOWN = {os.path.splitext(os.path.basename(p))[0] for p in prep_sources.R19_U64_UNALIGNED}
_manifest = os.path.join(ROOT, "build", "ps2", "r19_auto_files.txt")
if os.path.isfile(_manifest):
    with open(_manifest, encoding="utf-8") as _f:
        KNOWN.update(
            os.path.splitext(os.path.basename(line.strip()))[0]
            for line in _f
            if line.strip()
        )

FUNC = re.compile(r"^[0-9a-fA-F]+ <([^>]+)>:$")
INSN = re.compile(
    r"^\s*([0-9a-fA-F]+):.*\b(ld|sd)\s+\$?[A-Za-z0-9_]+,\s*"
    r"(-?(?:0[xX][0-9a-fA-F]+|[0-9]+))\(\$?[A-Za-z0-9_]+\)"
)


def parse_imm(s):
    neg = s.startswith("-")
    t = s[1:] if neg else s
    v = int(t, 16 if t.lower().startswith("0x") else 10)
    return -v if neg else v


def objdump_path():
    ext = ".exe" if os.name == "nt" else ""
    p = os.path.join(ps2cfg.sdk_root(), "toolchain", "ee", "bin",
                     "mips64r5900el-ps2-elf-objdump" + ext)
    if not os.path.isfile(p):
        raise SystemExit(f"R5900 audit: objdump not found at {p}")
    return p


def elf_path():
    candidates = [
        os.path.join(ROOT, "build", "obj", "khdays-ps2", "khdays-ps2.unstripped.elf"),
        os.path.join(ROOT, "build", "bin", "khdays-ps2.elf"),
    ]
    for p in candidates:
        if os.path.isfile(p):
            return p
    raise SystemExit("R5900 audit: linked khdays-ps2 ELF not found")


def main():
    elf = elf_path()
    proc = subprocess.run([objdump_path(), "-d", elf], check=False,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                          text=True, errors="replace")
    if proc.returncode:
        sys.stderr.write(proc.stderr)
        raise SystemExit(f"R5900 audit: objdump exited {proc.returncode}")

    current = "?"
    candidates = []
    confirmed_bad = []
    for line in proc.stdout.splitlines():
        fm = FUNC.match(line.strip())
        if fm:
            current = fm.group(1)
            continue
        m = INSN.match(line)
        if not m:
            continue
        off = parse_imm(m.group(3))
        if off % 8 == 0:
            continue
        row = (current, m.group(1), m.group(2), off, line.strip())
        candidates.append(row)
        base_name = current.split(".", 1)[0]
        if base_name in KNOWN:
            confirmed_bad.append(row)

    outdir = os.path.join(ROOT, "build", "ps2")
    os.makedirs(outdir, exist_ok=True)
    report = os.path.join(outdir, "r5900_ldsd.txt")
    with open(report, "w", encoding="utf-8", newline="\n") as f:
        f.write("# ld/sd with a non-8-byte immediate; review base alignment before calling unsafe\n")
        for fn, addr, op, off, insn in candidates:
            f.write(f"{fn}\t0x{addr}\t{op}\t{off}\t{insn}\n")

    if confirmed_bad:
        for fn, addr, op, off, insn in confirmed_bad:
            print(f"R5900 ALIGNMENT REGRESSION: {fn} 0x{addr}: {insn}", file=sys.stderr)
        raise SystemExit(
            "R5900 audit: confirmed R19 function still emits ld/sd at a non-8-byte offset"
        )

    print(f"R5900 audit: {len(KNOWN)} R19-covered functions clean; "
          f"{len(candidates)} global ld/sd candidates recorded in build/ps2/r5900_ldsd.txt")


if __name__ == "__main__":
    main()

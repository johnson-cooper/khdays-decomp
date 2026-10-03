"""Statistical EE profiler over the PCSX2 DebugServer (development helper).

    python ps2/tools/win/pcsx2_sample.py [samples] [unstripped elf]

Pauses the EE at random moments, records the PC and a short stack walk, resumes, and prints the
functions where time is spent: "self" (the PC's function) and "incl" (any function on the
sampled stack).  Symbols come from the unstripped ELF via nm.  Uses the DebugServer protocol
(newline-delimited JSON on 127.0.0.1:21512) that the PCSX2 MCP bridge also speaks.
"""
import bisect
import collections
import json
import os
import random
import socket
import subprocess
import sys
import time

NM = os.path.expandvars(r"%LOCALAPPDATA%\ps2build\toolchain\ee\bin\mips64r5900el-ps2-elf-nm.exe")


class Dbg:
    def __init__(self, port=21512):
        self.s = socket.create_connection(("127.0.0.1", port), timeout=10)
        self.buf = b""

    def call(self, **req):
        self.s.sendall((json.dumps(req) + "\n").encode())
        while b"\n" not in self.buf:
            self.buf += self.s.recv(65536)
        line, self.buf = self.buf.split(b"\n", 1)
        return json.loads(line)


def load_syms(elf):
    out = subprocess.run([NM, "-n", "--defined-only", elf], capture_output=True, text=True).stdout
    addrs, names = [], []
    for ln in out.splitlines():
        p = ln.split()
        if len(p) == 3 and p[1] in "tTwW":
            addrs.append(int(p[0], 16))
            names.append(p[2])
    return addrs, names


def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 300
    elf = sys.argv[2] if len(sys.argv) > 2 else "build/obj/khdays-ps2/khdays-ps2.unstripped.elf"
    addrs, names = load_syms(elf)

    def sym(a):
        i = bisect.bisect_right(addrs, a) - 1
        return names[i] if i >= 0 else hex(a)

    d = Dbg()
    self_c, incl_c = collections.Counter(), collections.Counter()
    for _ in range(n):
        time.sleep(random.uniform(0.01, 0.04))
        d.call(cmd="pause", cpu="ee")
        bt = d.call(cmd="get_backtrace", cpu="ee", max_frames=12)
        d.call(cmd="resume", cpu="ee")
        frames = bt.get("frames", [])
        if not frames:
            continue
        pcs = [int(str(f["pc"]), 16) if isinstance(f["pc"], str) else f["pc"] for f in frames]
        fn = [sym(p) for p in pcs]
        self_c[fn[0]] += 1
        for f in set(fn):
            incl_c[f] += 1
    tot = sum(self_c.values())
    print(f"{tot} samples")
    print("--- self ---")
    for f, c in self_c.most_common(30):
        print(f"{100.0 * c / tot:5.1f}%  {f}")
    print("--- inclusive ---")
    for f, c in incl_c.most_common(40):
        print(f"{100.0 * c / tot:5.1f}%  {f}")


if __name__ == "__main__":
    main()

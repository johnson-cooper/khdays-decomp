#!/usr/bin/env python3
"""Compile decomp sources with the PS2 EE compiler and categorise the failures.

This is a bring-up aid: it runs the PS2BUILD EE gcc directly (syntax-only by
default) over a set of source directories, in parallel, using the same compat
flags as ps2.yaml, and writes a summary of the diagnostics.

    python ps2/tools/trial_compile.py src/engine            # one directory
    python ps2/tools/trial_compile.py src                   # the whole game
    python ps2/tools/trial_compile.py --list-failing src    # only print failing files
"""
import argparse
import collections
import concurrent.futures as cf
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ps2cfg  # noqa: E402

DIAG = re.compile(r"^(?P<file>[^:]+):(?P<line>\d+):(?P<col>\d+): (?P<kind>error|warning): (?P<msg>.*?)(?: \[(?P<flag>-W[^\]]+)\])?$")


def compile_one(path, syntax_only):
    cmd = [ps2cfg.ee_gcc()] + ps2cfg.game_cflags(path) + (["-fsyntax-only"] if syntax_only else ["-c", "-o", os.devnull]) + [path]
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True)
    return path, p.returncode, p.stderr


def normalise(msg):
    msg = re.sub(r"'[^']*'", "'X'", msg)
    msg = re.sub(r"\b0x[0-9a-fA-F]+\b|\b\d+\b", "N", msg)
    return msg


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("paths", nargs="+")
    ap.add_argument("--object", action="store_true", help="really compile (default: -fsyntax-only)")
    ap.add_argument("--list-failing", action="store_true")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    args = ap.parse_args()

    files = []
    for p in args.paths:
        p = os.path.join(ROOT, p)
        if os.path.isfile(p):
            files.append(p)
            continue
        for dp, dn, fn in os.walk(p):
            dn.sort()
            files += [os.path.join(dp, f) for f in sorted(fn) if f.endswith(".c")]
    files = [os.path.relpath(f, ROOT).replace("\\", "/") for f in files]
    files = [f for f in files if not ps2cfg.is_excluded(f)]

    errors = collections.Counter()
    examples = {}
    failing = []
    with cf.ThreadPoolExecutor(args.j) as ex:
        for path, rc, err in ex.map(lambda f: compile_one(f, not args.object), files):
            if rc != 0:
                failing.append(path)
            for line in err.splitlines():
                m = DIAG.match(line)
                if m and m.group("kind") == "error":
                    key = normalise(m.group("msg"))
                    errors[key] += 1
                    examples.setdefault(key, f"{m.group('file')}:{m.group('line')}: {m.group('msg')}")

    if args.list_failing:
        print("\n".join(failing))
        return 1 if failing else 0
    print(f"{len(files)} files, {len(failing)} failing")
    for k, n in errors.most_common(60):
        print(f"{n:6d}  {k}\n        e.g. {examples[k]}")
    out = os.path.join(ROOT, "build", "ps2")
    os.makedirs(out, exist_ok=True)
    open(os.path.join(out, "trial_failing.txt"), "w").write("\n".join(failing) + "\n")
    return 1 if failing else 0


if __name__ == "__main__":
    sys.exit(main())

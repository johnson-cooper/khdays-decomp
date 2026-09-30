#!/usr/bin/env python3
"""Find functions whose per-file prototypes disagree in an ABI-relevant way on the EE.

The decomp declares callees locally in every source.  On the ARM9 a `long long` return is
r0:r1 and an `int` return is r0, so a function returning quotient|remainder<<32 can be read
either way.  Under the EE n32 ABI a 64-bit value travels in ONE 64-bit register and 32-bit
values are kept sign-extended in it, so callers that disagree with the callee on 64-bit-ness
(return or argument) get wrong results.  Floats (FPR vs GPR) have the same problem.

Uses gcc's -aux-info output (exact, post-preprocessing prototypes) for every source.

    python ps2/tools/proto_audit.py [--jobs N]
Writes build/ps2/proto_audit.json and prints a summary.
"""
import collections
import concurrent.futures as cf
import json
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ps2cfg  # noqa: E402

ROOT = ps2cfg.ROOT
W64 = re.compile(r"\b(long\s+long|u64|s64|fx64|fx64c|double|OSTick)\b")
FLT = re.compile(r"\b(float|f32|double)\b")
PROTO = re.compile(r"^/\* (?P<file>.*?):(?P<line>\d+):(?P<flags>[A-Z]+) \*/ (?P<decl>.*);\s*(/\*.*\*/)?$")


def cls(t):
    t = t.strip()
    if "*" in t or "(" in t:
        return "p"
    if FLT.search(t):
        return "f64" if "double" in t else "f"
    if W64.search(t):
        return "64"
    if re.fullmatch(r"(extern\s+)?(const\s+)?void", t):
        return "v"
    return "32"


def split_params(s):
    out, depth, cur = [], 0, ""
    for c in s:
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
        if c == "," and depth == 0:
            out.append(cur)
            cur = ""
        else:
            cur += c
    if cur.strip():
        out.append(cur)
    return out


def parse(decl):
    # "extern long long func_02020400 (int, int)" ; function-pointer returns are rare, skip them
    m = re.match(r"^(?P<ret>.*?)\b(?P<name>[A-Za-z_]\w*)\s*\((?P<params>.*)\)$", decl)
    if not m:
        return None
    ret = re.sub(r"\b(extern|static|inline|__inline__)\b", "", m.group("ret"))
    params = [p for p in split_params(m.group("params")) if p.strip() not in ("void", "...")]
    return m.group("name"), cls(ret), tuple(cls(p) for p in params), ("..." in m.group("params"))


def aux(path):
    auxdir = os.path.join(ROOT, "build", "ps2", "aux")
    os.makedirs(auxdir, exist_ok=True)
    tmp = os.path.join(auxdir, path.replace("/", "_") + ".aux")
    cmd = [ps2cfg.ee_gcc()] + ps2cfg.game_cflags(path) + ["-fsyntax-only", "-aux-info", tmp, path]
    subprocess.run(cmd, cwd=ROOT, capture_output=True)
    res = []
    if not os.path.exists(tmp):
        return path, res
    try:
        for line in open(tmp, encoding="utf-8", errors="replace"):
            m = PROTO.match(line.strip())
            if not m:
                continue
            f = m.group("file").replace("\\", "/")
            # only prototypes written in the source itself (not system headers)
            if "/ps2build/" in f:
                continue
            p = parse(m.group("decl"))
            if p:
                res.append((p, m.group("flags")))
    finally:
        os.remove(tmp)
    return path, res


def main():
    jobs = os.cpu_count()
    files = []
    for top in ("src", "libs"):
        for dp, dn, fn in os.walk(os.path.join(ROOT, top)):
            files += [os.path.relpath(os.path.join(dp, f), ROOT).replace("\\", "/") for f in fn if f.endswith(".c")]
    files = [f for f in files if not ps2cfg.is_excluded(f)]
    decls = collections.defaultdict(lambda: collections.defaultdict(list))  # name -> sig -> [files]
    defs = {}
    with cf.ThreadPoolExecutor(jobs) as ex:
        for path, res in ex.map(aux, files):
            for (name, ret, params, va), flags in res:
                sig = (ret, params)
                decls[name][sig].append(path)
                if "F" in flags:   # a definition
                    defs[name] = sig
    bad = {}
    for name, sigs in decls.items():
        if len(sigs) < 2:
            continue
        rets = {s[0] for s in sigs}
        ret_conflict = ("64" in rets or "f" in rets or "f64" in rets) and len(rets - {"v"}) > 1
        # argument conflicts: same position, one 64-bit/float and another not
        arg_conflict = False
        maxn = max(len(s[1]) for s in sigs)
        for i in range(maxn):
            kinds = {s[1][i] for s in sigs if i < len(s[1])}
            if len(kinds) > 1 and (kinds & {"64", "f", "f64"}):
                arg_conflict = True
        if ret_conflict or arg_conflict:
            bad[name] = {
                "definition": list(defs.get(name, ())) if name in defs else None,
                "variants": [{"ret": s[0], "params": list(s[1]), "files": sorted(fs)} for s, fs in sigs.items()],
            }
    out = os.path.join(ROOT, "build", "ps2")
    os.makedirs(out, exist_ok=True)
    json.dump(bad, open(os.path.join(out, "proto_audit.json"), "w"), indent=1)
    print(f"{len(files)} files, {len(decls)} functions declared, {len(bad)} with ABI-relevant prototype conflicts")
    for name, info in sorted(bad.items(), key=lambda kv: -sum(len(v["files"]) for v in kv[1]["variants"])):
        vs = ", ".join(f"{v['ret']}({','.join(v['params'])})x{len(v['files'])}" for v in info["variants"])
        print(f"  {name}: def={info['definition']} :: {vs}")


if __name__ == "__main__":
    main()

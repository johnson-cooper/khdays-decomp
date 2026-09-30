"""Shared configuration for the PS2 port tooling.

Single place that knows where the PS2BUILD toolchain lives, which compile
flags the decomp sources get on the EE, and which original sources the PS2
build leaves out (replaced by ps2/ implementations or overrides).
"""
import os
import shutil

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PS2 = os.path.join(ROOT, "ps2")


def sdk_root():
    exe = shutil.which("ps2build")
    if not exe:
        raise SystemExit("ps2build not found on PATH - install PS2BUILD (https://ps2.techwritescode.dev/)")
    return os.path.dirname(os.path.realpath(exe))


def ee_gcc():
    ext = ".exe" if os.name == "nt" else ""
    return os.path.join(sdk_root(), "toolchain", "ee", "bin", "mips64r5900el-ps2-elf-gcc" + ext)


# Flags every decomp (game) source is compiled with on the EE.  Keep in sync with
# the `cflags`/`defines` that ps2/tools/gen_ps2yaml.py writes into ps2.yaml.
GAME_DEFINES = ["PLATFORM_PS2", "KH_DECOMP_SOURCE"]
GAME_CFLAGS = [
    "-std=gnu99",
    "-fpermissive",               # gcc>=14: implicit decls/int-conversion become warnings again
    "-fno-strict-aliasing",       # decompiled code type-puns freely
    "-fwrapv",                    # ARM wraps signed overflow; keep that behaviour
    "-fno-builtin",               # the game has its own memcpy-likes with SDK names
    "-fsigned-char",              # the matching build uses mwcc -char signed
    "-fcommon",                   # tentative definitions shared between files, as mwcc merges them
    "-w",
]
GAME_INCLUDES = ["ps2/include", "include"]
PREFIX_HEADER = "ps2/include/platform/ps2/decomp_prefix.h"


def game_cflags(path=None):
    flags = ["-D_EE", "-G0", "-O2"]
    flags += ["-D" + d for d in GAME_DEFINES]
    flags += GAME_CFLAGS
    flags += ["-I" + os.path.join(ROOT, i) for i in GAME_INCLUDES]
    flags += ["-include", os.path.join(ROOT, PREFIX_HEADER)]
    return flags


def _read_list(name):
    p = os.path.join(PS2, "config", name)
    out = []
    if os.path.exists(p):
        for line in open(p):
            line = line.split("#", 1)[0].strip()
            if line:
                out.append(line)
    return out


def excluded_patterns():
    return _read_list("exclude.txt")


def is_excluded(relpath):
    relpath = relpath.replace("\\", "/")
    for pat in excluded_patterns():
        if pat.endswith("/"):
            if relpath.startswith(pat) or ("/" + pat) in relpath:
                return True
        elif relpath == pat or os.path.basename(relpath) == pat:
            return True
    # a PS2 override with the same function name replaces the original source
    name = os.path.splitext(os.path.basename(relpath))[0]
    return name in overrides()


_OVR = None


def overrides():
    global _OVR
    if _OVR is None:
        _OVR = set()
        top = os.path.join(PS2, "overrides")
        for dp, dn, fn in os.walk(top):
            for f in fn:
                if f.endswith(".c"):
                    _OVR.add(os.path.splitext(f)[0])
    return _OVR

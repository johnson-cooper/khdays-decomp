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
    "-fno-toplevel-reorder",      # preserve address-ordered data objects and pointer-table aliases
    "-fpack-struct=4",            # mwcc aligns 64-bit members to 4: DS struct sizes/offsets (heap
                                  # blocks are sized with DS sizeofs, e.g. ov002's gauge context)
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


def is_excluded(relpath, dirs=True):
    """dirs=False ignores directory patterns: a single file named in modules.txt is a reviewed
    exception to the exclusion of its directory (file patterns and overrides still apply)."""
    relpath = relpath.replace("\\", "/")
    for pat in excluded_patterns():
        if pat.endswith("/"):
            if dirs and (relpath.startswith(pat) or ("/" + pat) in relpath):
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


_DATA_FILES = None


def ds_data_files():
    """Source files the DS build has as pure data (no .text in config/arm9/**/delinks.txt):
    {relpath: (module, {".data": (start, end), ".rodata": (start, end)})}.  The PS2 link puts
    their objects at the same module-relative addresses (gen_link.py) and prep caps their
    alignment at 4 (R17), because game code reaches neighbouring objects by offset."""
    global _DATA_FILES
    if _DATA_FILES is not None:
        return _DATA_FILES
    import glob
    out = {}
    cfg = os.path.join(ROOT, "config", "arm9")
    dirs = [("main", cfg)] + [(os.path.basename(d), d)
                              for d in sorted(glob.glob(os.path.join(cfg, "overlays", "ov*")))]
    for mod, d in dirs:
        cur, secs = None, {}
        for line in list(open(os.path.join(d, "delinks.txt"))) + ["END:"]:
            if line and not line.startswith(" ") and line.strip().endswith(":"):
                if cur and ".text" not in secs and (".data" in secs or ".rodata" in secs):
                    out[cur] = (mod, {k: v for k, v in secs.items() if k in (".data", ".rodata")})
                cur, secs = line.strip()[:-1], {}
            elif cur and line.startswith("    ."):
                p = line.split()
                secs[p[0]] = (int(p[1].split(":")[1], 16), int(p[2].split(":")[1], 16))
    _DATA_FILES = out
    return out

#!/usr/bin/env python3
"""Generate the link-time glue for the PS2 game ELF from the compiled archives.

The matching DS build gets .bss and a few data labels from dsd's linker script, not from C.
This tool reproduces what the PS2 link needs, from config/arm9/**/symbols.txt and `nm` of the
compiled archives:

  ps2/gen/layout/<module>_bss.S   every .bss symbol of a module that no C file defines, at its
                                  original offset from the module's .bss start (so code that
                                  reaches a neighbour by offset still lands on it); weak, so a
                                  C definition wins.  One section per module (.bss.kh.<module>),
                                  which FS_LoadOverlay uses to clear an overlay's .bss.
  ps2/gen/khdays.ld               the PS2SDK linker script plus: OVERLAY_<n>_ID absolute
                                  symbols, per-overlay .data/.bss ranges, and aliases for data
                                  labels that point inside a larger C-defined object.
  ps2/gen/stubs/auto_stubs.c      a logging stub for every function still undefined (mostly
                                  NitroSDK/NitroSystem entry points not re-implemented yet),
                                  and zeroed storage for undefined data symbols.
  build/ps2/link_report.md        what was stubbed, grouped by SDK module - the list of
                                  dependencies that block further progress.

    python ps2/tools/gen_link.py            (after the game archives are built)
"""
import collections
import glob
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ps2cfg  # noqa: E402

ROOT = ps2cfg.ROOT
GEN = os.path.join(ROOT, "ps2", "gen")


def tool(name):
    ext = ".exe" if os.name == "nt" else ""
    return os.path.join(ps2cfg.sdk_root(), "toolchain", "ee", "bin", "mips64r5900el-ps2-elf-" + name + ext)


# ------------------------------------------------------------------ config/arm9 symbol maps

class Module:
    def __init__(self, name, ov_id):
        self.name = name
        self.ov_id = ov_id
        self.sections = {}   # ".bss" -> (start, end)
        self.syms = []       # (addr, name, kind)


def load_modules():
    cfgdir = os.path.join(ROOT, "config", "arm9")
    mods = []

    def load(name, ov_id, d):
        m = Module(name, ov_id)
        for line in open(os.path.join(d, "delinks.txt")):
            if not line.startswith("    ."):
                if line.strip() and not line.startswith(" "):
                    break
                continue
            p = line.split()
            m.sections[p[0]] = (int(p[1].split(":")[1], 16), int(p[2].split(":")[1], 16))
        for line in open(os.path.join(d, "symbols.txt")):
            p = line.split()
            if len(p) < 3:
                continue
            kind = p[1].split(":", 1)[1].split("(")[0]
            m.syms.append((int(p[2].split(":")[1], 16), p[0], kind))
        m.syms.sort()
        mods.append(m)

    load("main", None, cfgdir)
    load("itcm", None, os.path.join(cfgdir, "itcm"))
    load("dtcm", None, os.path.join(cfgdir, "dtcm"))
    for d in sorted(glob.glob(os.path.join(cfgdir, "overlays", "ov*"))):
        n = os.path.basename(d)
        load(n, int(n[2:]), d)
    return mods


# ------------------------------------------------------------------------------ nm

def nm(paths):
    """-> defined {name: (type, size)}, undefined set"""
    defined, undefined = {}, set()
    for i in range(0, len(paths), 40):
        chunk = paths[i:i + 40]
        out = subprocess.run([tool("nm"), "-S", "--defined-only", "-A"] + chunk, capture_output=True, text=True).stdout
        for line in out.splitlines():
            p = line.split()
            # path:obj: [value] [size] type name
            if len(p) >= 4:
                t, name = p[-2], p[-1]
                size = int(p[-3], 16) if len(p) >= 5 else 0
                if t in "TtDdBbRrVvWwGgSsC" and t.isupper() or t in "VW":
                    defined[name] = (t, size)
        out = subprocess.run([tool("nm"), "--undefined-only"] + chunk, capture_output=True, text=True).stdout
        for line in out.splitlines():
            p = line.split()
            if len(p) == 2 and p[0] == "U":
                undefined.add(p[1])
    return defined, undefined


def sdk_archives():
    sdk = ps2cfg.sdk_root()
    libs = []
    for pkg in ["startup", "kernel", "cglue", "pthreadglue", "profglue", "cdvd", "graph", "draw", "dma", "packet2",
                "pad", "filexio", "patches", "debug", "mc", "audsrv"]:
        libs += glob.glob(os.path.join(sdk, "packages", "*", pkg, "lib", "*.a"))
        libs += glob.glob(os.path.join(sdk, "packages", "*", pkg, "ee", "lib", "*.a"))
    tc = os.path.join(sdk, "toolchain", "ee")
    libs += glob.glob(os.path.join(tc, "mips64r5900el-ps2-elf", "lib", "libc.a"))
    libs += glob.glob(os.path.join(tc, "mips64r5900el-ps2-elf", "lib", "libm.a"))
    libs += glob.glob(os.path.join(tc, "lib", "gcc", "mips64r5900el-ps2-elf", "*", "libgcc.a"))
    return libs


# -------------------------------------------------------------------------- generation

LINK_PROVIDED = re.compile(r"_[A-Za-z_]\w*|(size_)?\w+_irx|kh_overlay_(count|info)|__kh_bss_(start|end)_\w+")


def main():
    libdir = os.path.join(ROOT, "build", "lib")
    game_archives = sorted(glob.glob(os.path.join(libdir, "libkh_*.a")))
    if not game_archives:
        raise SystemExit("no build/lib/libkh_*.a - build the game archives first (see build-ps2.sh)")
    # objects of the ELF target itself (main/, stubs are regenerated, so excluded)
    own = glob.glob(os.path.join(ROOT, "build", "obj", "khdays-ps2", "ps2", "src", "**", "*.o"), recursive=True)

    g_def, g_undef = nm(game_archives + own)
    s_def, _ = nm(sdk_archives())
    all_def = dict(s_def)
    all_def.update(g_def)

    mods = load_modules()
    audit_path = os.path.join(ROOT, "build", "ps2", "audit.json")
    lib_def = json.load(open(audit_path))["lib_def"] if os.path.exists(audit_path) else {}

    sym_mod = {}
    for m in mods:
        for addr, name, kind in m.syms:
            sym_mod[name] = (m, addr, kind)

    os.makedirs(os.path.join(GEN, "layout"), exist_ok=True)
    os.makedirs(os.path.join(GEN, "stubs"), exist_ok=True)
    for f in glob.glob(os.path.join(GEN, "layout", "*.S")):
        os.remove(f)

    # --- .bss layouts: every bss symbol of every module (weak) ---------------------------
    bss_count = 0
    for m in mods:
        if ".bss" not in m.sections:
            continue
        b0, b1 = m.sections[".bss"]
        syms = [(a, n) for a, n, k in m.syms if k == "bss" and b0 <= a < b1]
        if b1 <= b0:
            continue
        lines = [f"/* GENERATED by ps2/tools/gen_link.py from config/arm9 - {m.name} .bss, 0x{b0:08x}-0x{b1:08x} */",
                 f'    .section .bss.kh.{m.name},"aw",@nobits', "    .balign 32",
                 f"    .globl __kh_bss_start_{m.name}", f"__kh_bss_start_{m.name}:"]
        pos = b0
        for a, n in syms:
            if a > pos:
                lines.append(f"    .space 0x{a - pos:x}")
                pos = a
            if n in g_def:
                continue        # a C file defines it; keep the slot, use the C object
            lines += [f"    .weak {n}", f"    .type {n}, @object", f"{n}:"]
            bss_count += 1
        if b1 > pos:
            lines.append(f"    .space 0x{b1 - pos:x}")
        lines += [f"    .globl __kh_bss_end_{m.name}", f"__kh_bss_end_{m.name}:", ""]
        open(os.path.join(GEN, "layout", f"{m.name}_bss.S"), "w", newline="\n").write("\n".join(lines))

    # --- overlay table: .bss range per overlay id (FS_LoadOverlay clears it) --------------
    ovs = {m.ov_id: m for m in mods if m.ov_id is not None}
    n_ov = max(ovs) + 1 if ovs else 0
    lines = ["/* GENERATED by ps2/tools/gen_link.py - per overlay: config .bss layout, C .data, C .bss",
             " * (see nitro_fs.c: FS_LoadOverlay restores .data and clears both .bss ranges) */",
             "typedef struct { unsigned char *bss_start, *bss_end, *data_start, *data_end, *cbss_start, *cbss_end; } KhOverlayInfo;", ""]
    for i in range(n_ov):
        m = ovs.get(i)
        if not m:
            continue
        if ".bss" in m.sections and m.sections[".bss"][1] > m.sections[".bss"][0]:
            lines.append(f"extern unsigned char __kh_bss_start_{m.name}[], __kh_bss_end_{m.name}[];")
        lines.append(f"extern unsigned char __kh_data_start_{m.name}[], __kh_data_end_{m.name}[];")
        lines.append(f"extern unsigned char __kh_cbss_start_{m.name}[], __kh_cbss_end_{m.name}[];")
    lines += ["", f"const unsigned int kh_overlay_count = {n_ov};", "const KhOverlayInfo kh_overlay_info[] = {"]
    for i in range(n_ov):
        m = ovs.get(i)
        if not m:
            lines.append(f"    {{ 0, 0, 0, 0, 0, 0 }},   /* {i} */")
            continue
        if ".bss" in m.sections and m.sections[".bss"][1] > m.sections[".bss"][0]:
            b = f"__kh_bss_start_{m.name}, __kh_bss_end_{m.name}"
        else:
            b = "0, 0"
        lines.append(f"    {{ {b}, __kh_data_start_{m.name}, __kh_data_end_{m.name}, "
                     f"__kh_cbss_start_{m.name}, __kh_cbss_end_{m.name} }},   /* {i} */")
    lines.append("};")
    open(os.path.join(GEN, "stubs", "overlays.c"), "w", newline="\n").write("\n".join(lines) + "\n")

    # --- data labels inside larger C objects -> aliases ---------------------------------
    aliases, zero_data, stubs = [], [], []
    for name in sorted(g_undef):
        if name in all_def:
            continue
        if re.fullmatch(r"OVERLAY_\d+_ID", name):
            continue
        # provided by the linker script, the ELF target's own generated objects or PS2BUILD's
        # embed_irx objects - never stub these
        if LINK_PROVIDED.fullmatch(name):
            continue
        info = sym_mod.get(name)
        if info and info[2] == "bss":
            continue                       # provided by the layout above
        if info and info[2] in ("data", "label"):
            m, addr, _ = info
            # nearest lower symbol of the same module defined in C, if the address falls inside it
            best = None
            for a, n, k in m.syms:
                if a > addr:
                    break
                if n in g_def and g_def[n][0] in "DRBVdrbv" and n != name:
                    best = (a, n)
            if best and addr - best[0] < max(g_def[best[1]][1], 1):
                aliases.append((name, best[1], addr - best[0]))
                continue
            zero_data.append((name, m.name, addr))
            continue
        stubs.append(name)

    # --- linker script -----------------------------------------------------------------
    base = open(os.path.join(ps2cfg.sdk_root(), "packages", "core", "startup", "share", "ee", "linkfile")).read()
    extra = ["", "/* ---- GENERATED by ps2/tools/gen_link.py ---- */", "/* overlay ids: FS_OVERLAY_ID(x) is the address of this absolute symbol */"]
    for m in mods:
        if m.ov_id is not None:
            extra.append(f"OVERLAY_{m.ov_id}_ID = {m.ov_id};")
    extra.append("/* data labels that point inside a larger C-defined object */")
    for name, target, off in aliases:
        extra.append(f"PROVIDE({name} = {target} + 0x{off:x});")
    # Each overlay's own C objects are in archives named kh_<group>_ovNNN_<name>[_data][_k]; their
    # .data and .bss are grouped per overlay so FS_LoadOverlay can reinitialise them.
    data_lines, bss_lines = [], []
    for m in mods:
        if m.ov_id is None:
            continue
        pat = f"*libkh_*_{m.name}*.a:"
        data_lines.append(f"\t\t__kh_data_start_{m.name} = .; {pat}*(.data .data.*) __kh_data_end_{m.name} = .;")
        bss_lines.append(f"\t\t__kh_cbss_start_{m.name} = .; {pat}*(.bss .bss.*) {pat}(COMMON) __kh_cbss_end_{m.name} = .;")
    script = base.replace("\t\t_fdata = . ;", "\t\t_fdata = . ;\n" + "\n".join(data_lines), 1)
    script = script.replace("\t.bss ALIGN(128) : {", "\t.bss ALIGN(128) : {\n\t\t*(SORT(.bss.kh.*))\n" + "\n".join(bss_lines), 1)
    script += "\n".join(extra) + "\n"
    open(os.path.join(GEN, "khdays.ld"), "w", newline="\n").write(script)

    # --- stubs -------------------------------------------------------------------------
    by_lib = collections.defaultdict(list)
    for n in stubs:
        by_lib[lib_def.get(n, "game/other")].append(n)
    lines = ["/* GENERATED by ps2/tools/gen_link.py - DO NOT EDIT.",
             " *",
             " * Functions the game references that are not implemented for the PS2 yet.  Each logs",
             " * once when first called and returns 0.  Implementing one in ps2/src (or keeping the",
             " * decomp's C for it) removes it from here on the next generation. */",
             '#include "platform/kh_platform.h"', "",
             "#define KH_AUTO_STUB(name) int name(void); int name(void) { KH_UNIMPLEMENTED_ONCE(#name); return 0; }",
             ""]
    for lib in sorted(by_lib):
        lines.append(f"/* {lib} */")
        for n in sorted(by_lib[lib]):
            lines.append(f"KH_AUTO_STUB({n})")
        lines.append("")
    lines.append("/* undefined data symbols with no C definition: zero storage (contents unknown!) */")
    for n, mname, addr in zero_data:
        lines.append(f"unsigned char {n}[64] __attribute__((aligned(16)));   /* {mname} 0x{addr:08x} */")
    open(os.path.join(GEN, "stubs", "auto_stubs.c"), "w", newline="\n").write("\n".join(lines) + "\n")

    # --- report ------------------------------------------------------------------------
    rep = ["# PS2 link report (generated by ps2/tools/gen_link.py)", "",
           f"- game archives: {len(game_archives)}",
           f"- .bss symbols laid out from config: {bss_count}",
           f"- data aliases into C objects: {len(aliases)}",
           f"- undefined data with no definition (zero-filled): {len(zero_data)}",
           f"- functions auto-stubbed: {len(stubs)}", "",
           "## Auto-stubbed functions by SDK/library module", ""]
    for lib in sorted(by_lib, key=lambda k: -len(by_lib[k])):
        rep.append(f"### {lib} ({len(by_lib[lib])})")
        rep.append("")
        rep.append(", ".join(f"`{n}`" for n in sorted(by_lib[lib])))
        rep.append("")
    if zero_data:
        rep.append("## Zero-filled data")
        rep += [f"- `{n}` ({mn} 0x{a:08x})" for n, mn, a in zero_data]
    os.makedirs(os.path.join(ROOT, "build", "ps2"), exist_ok=True)
    open(os.path.join(ROOT, "build", "ps2", "link_report.md"), "w").write("\n".join(rep) + "\n")
    print("\n".join(rep[:8]))
    for lib in sorted(by_lib, key=lambda k: -len(by_lib[k])):
        print(f"  {lib}: {len(by_lib[lib])}")


if __name__ == "__main__":
    main()

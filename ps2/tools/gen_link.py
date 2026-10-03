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
                                  Zero-initialised globals that only exist as COMMON (R10) get
                                  a strong label at their DS offset instead, so address-coupled
                                  neighbours (NitroSDK's SNDCommandMgr) stay contiguous.
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

def nm(paths, commons=None, strong=None):
    """-> defined {name: (type, size)}, undefined set.
    commons (optional dict) receives {name: (alignment, size)} of every COMMON symbol;
    strong (optional set) receives every name that also has a non-COMMON definition."""
    defined, undefined = {}, set()
    for i in range(0, len(paths), 40):
        chunk = paths[i:i + 40]
        out = subprocess.run([tool("nm"), "-S", "--defined-only", "-A"] + chunk, capture_output=True, text=True).stdout
        for line in out.splitlines():
            p = line.split()
            # "path:obj:value [size] type name" - -A glues the value to the file name
            if len(p) not in (3, 4) or ":" not in p[0]:
                continue
            t, name = p[-2], p[-1]
            try:
                value = int(p[0].rsplit(":", 1)[1], 16)
                size = int(p[1], 16) if len(p) == 4 else 0
            except ValueError:
                continue
            if t in "TtDdBbRrVvWwGgSsC" and t.isupper() or t in "VW":
                defined[name] = (t, size)
                if t == "C":
                    if commons is not None:
                        # (BFD reports a COMMON symbol's size as its value; the alignment
                        # comes from readelf below)
                        a0, s0 = commons.get(name, (1, 0))
                        commons[name] = (a0, max(s0, size, value))
                elif strong is not None:
                    strong.add(name)
        if commons is not None:
            # ELF st_value of a COMMON symbol is its alignment (EE gcc raises arrays and structs
            # to 8); the largest over every copy is what the code may rely on
            out = subprocess.run([tool("readelf"), "-s", "-W"] + chunk, capture_output=True, text=True).stdout
            for line in out.splitlines():
                p = line.split()
                if len(p) == 8 and p[6] == "COM":
                    a0, s0 = commons.get(p[7], (1, 0))
                    commons[p[7]] = (max(a0, int(p[1], 16)), s0)
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
                "pad", "filexio", "patches", "debug", "eedebug", "mc", "audsrv"]:
        libs += glob.glob(os.path.join(sdk, "packages", "*", pkg, "lib", "*.a"))
        libs += glob.glob(os.path.join(sdk, "packages", "*", pkg, "ee", "lib", "*.a"))
    tc = os.path.join(sdk, "toolchain", "ee")
    libs += glob.glob(os.path.join(tc, "mips64r5900el-ps2-elf", "lib", "libc.a"))
    libs += glob.glob(os.path.join(tc, "mips64r5900el-ps2-elf", "lib", "libm.a"))
    libs += glob.glob(os.path.join(tc, "lib", "gcc", "mips64r5900el-ps2-elf", "*", "libgcc.a"))
    return libs


# ------------------------------------------------------------------ shared .bss blocks

SHARED_BSS = "khdays: shared-bss"
BSS_DECL = re.compile(r"^(?:volatile\s+)?[A-Za-z_][\w\s\*]*?[\s\*](\w+)\s*(?:\[[^\]]*\])?\s*=\s*[^;]*;")


def shared_bss_blocks():
    """Unique declaration orders of the decomp's "khdays: shared-bss" blocks: runs of zero-
    initialised globals that some source repeats from a library it shares .bss with."""
    blocks = set()
    for top in ("src", "libs"):
        for dp, _, files in os.walk(os.path.join(ROOT, top)):
            for f in files:
                if not f.endswith(".c"):
                    continue
                text = open(os.path.join(dp, f), encoding="utf-8", errors="replace").read()
                i = text.find(SHARED_BSS)
                if i < 0:
                    continue
                names = []
                for line in text[i:].splitlines()[1:]:
                    s = line.strip()
                    if not s and not names:
                        continue
                    m = BSS_DECL.match(s)
                    if not m:
                        break
                    names.append(m.group(1))
                if names:
                    blocks.add(tuple(names))
    return sorted(blocks)


def shared_bss_layout(sym_mod, commons):
    """-> ({module: [(ds_addr, name)]}, notes) for shared-bss names absent from symbols.txt.

    mwcc lays a file's zero-initialised globals out in reverse declaration order with the last
    declaration appended after them (InitPXI.c's comment; it reproduces every anchor below).
    Offsets are accumulated with each object's EE size and its alignment capped at 4, as on the
    ARM.  A block is used only if every symbols.txt name in it lands on its DS address, so a
    block whose EE sizes differ from the DS cannot be placed wrongly."""
    placed, notes, where = collections.defaultdict(list), [], {}
    for names in shared_bss_blocks():
        order = list(reversed(names[:-1])) + [names[-1]]
        if any(n not in commons for n in order):
            continue        # some copy is not COMMON (an override, or a real definition)
        offs, off = {}, 0
        for n in order:
            align, size = commons[n]
            align = min(max(align, 1), 4)
            off = (off + align - 1) // align * align
            offs[n] = off
            off += size
        anchors = [(n, sym_mod[n]) for n in order if n in sym_mod and sym_mod[n][2] == "bss"]
        if not anchors or len(anchors) == len(order):
            continue        # nothing to anchor, or nothing to infer
        base = {info[1] - offs[n] for n, info in anchors}
        mods_ = {info[0].name for _, info in anchors}
        if len(base) != 1 or len(mods_) != 1:
            notes.append(f"block {', '.join(names)}: anchors disagree with the mwcc layout; left COMMON")
            continue
        base, mod = base.pop(), mods_.pop()
        for n in order:
            if n in sym_mod:
                continue
            addr = base + offs[n]
            if where.get(n, addr) != addr:
                notes.append(f"`{n}`: two shared-bss blocks give different addresses; left COMMON")
                continue
            if n not in where:
                where[n] = addr
                placed[mod].append((addr, n))
    return placed, notes


# -------------------------------------------------------------------------- generation

LINK_PROVIDED = re.compile(r"_[A-Za-z_]\w*|(size_)?\w+_irx|kh_overlay_(count|info)|__kh_bss_(start|end)_\w+")


def main():
    libdir = os.path.join(ROOT, "build", "lib")
    game_archives = sorted(glob.glob(os.path.join(libdir, "libkh_*.a")))
    if not game_archives:
        raise SystemExit("no build/lib/libkh_*.a - build the game archives first (see build-ps2.sh)")
    # objects of the ELF target itself (main/, stubs are regenerated, so excluded)
    own = glob.glob(os.path.join(ROOT, "build", "obj", "khdays-ps2", "ps2", "src", "**", "*.o"), recursive=True)

    g_commons, g_strong = {}, set()
    g_def, g_undef = nm(game_archives + own, g_commons, g_strong)
    # Overlay code is reached through the DS overlay loader: many functions have no ordinary
    # static reference from the resident program.  Retain only the overlay archives (rather
    # than applying --whole-archive to the entire game) by emitting address references to each
    # global they actually define.  The table is also useful as a link-time audit: a missing
    # archive member becomes an undefined symbol instead of silently disappearing.
    overlay_archives = [p for p in game_archives
                        if re.search(r"_ov\d{3}(?:_|\.a$)", os.path.basename(p))]
    s_def, _ = nm(sdk_archives())
    all_def = dict(s_def)
    all_def.update(g_def)

    mods = load_modules()
    overlay_names = {name for m in mods if m.ov_id is not None for _, name, _ in m.syms}
    overlay_retain = sorted(name for name in overlay_names
                            if name in g_def and re.fullmatch(r"[A-Za-z_]\w*", name))
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

    # --- zero-initialised globals that only exist as COMMON -------------------------------
    # R10 turns the decomp's repeated `T x = 0;` definitions into COMMON symbols so the copies
    # merge, but ld then places COMMON wherever it likes.  Several of these are address-coupled
    # on the DS: code reaches a neighbour by offset (SND_PushCommand views data_02044748 as the
    # nine-word SNDCommandMgr whose other fields are sFinishedTag, sReserveList, ...).  A strong
    # label at the DS offset inside the module's .bss layout overrides every COMMON copy, so
    # both views agree.  Names absent from symbols.txt get their DS address from the
    # "khdays: shared-bss" block that declares them (see shared_bss_layout).
    placed, bss_notes = shared_bss_layout(sym_mod, g_commons)
    for m in mods:
        for addr, name in placed.get(m.name, []):
            m.syms.append((addr, name, "bss"))
        m.syms.sort()

    # --- .bss layouts: every bss symbol of every module (weak) ---------------------------
    bss_count = strong_count = 0
    for m in mods:
        if ".bss" not in m.sections:
            continue
        b0, b1 = m.sections[".bss"]
        syms = sorted({(a, n) for a, n, k in m.syms if k == "bss" and b0 <= a < b1})
        if b1 <= b0:
            continue
        # Pad so every label keeps its DS address modulo 32: alignment-sensitive objects
        # (8-byte EE loads) then land exactly as aligned as they were on the DS.
        lines = [f"/* GENERATED by ps2/tools/gen_link.py from config/arm9 - {m.name} .bss, 0x{b0:08x}-0x{b1:08x} */",
                 f'    .section .bss.kh.{m.name},"aw",@nobits', "    .balign 32"]
        if b0 % 32:
            lines.append(f"    .space 0x{b0 % 32:x}")
        lines += [f"    .globl __kh_bss_start_{m.name}", f"__kh_bss_start_{m.name}:"]
        pos = b0
        for a, n in syms:
            if a > pos:
                lines.append(f"    .space 0x{a - pos:x}")
                pos = a
            if n in g_commons and n not in g_strong:
                # Views wider than the next DS label are fine (Ov011Globals spans two labels,
                # as on the DS); what must hold is the EE alignment the compiler assumed.
                align, size = g_commons[n]
                if a % align == 0 and a + size <= b1:
                    lines += [f"    .globl {n}", f"    .type {n}, @object", f"{n}:"]
                    strong_count += 1
                    continue
                bss_notes.append(f"`{n}` stays COMMON: EE alignment {align} / size {size} does not "
                                 f"fit its DS address 0x{a:08x} in {m.name} .bss")
                continue
            if n in g_def:
                continue        # a C file defines it; keep the slot, use the C object
            lines += [f"    .weak {n}", f"    .type {n}, @object", f"{n}:"]
            bss_count += 1
        if b1 > pos:
            lines.append(f"    .space 0x{b1 - pos:x}")
        lines += [f"    .globl __kh_bss_end_{m.name}", f"__kh_bss_end_{m.name}:", ""]
        open(os.path.join(GEN, "layout", f"{m.name}_bss.S"), "w", newline="\n").write("\n".join(lines))

    # Strong references to every compiled overlay global.  Numeric overlay IDs and original
    # DS load addresses are runtime metadata, not ELF relocations, so without this selective
    # anchor ld would never extract most enemy/player archive members.  This is intentionally
    # narrower than --whole-archive: resident game, SDK and platform libraries remain demand-led.
    lines = ["/* GENERATED by ps2/tools/gen_link.py - selective overlay archive retention. */",
             '    .section .rodata.kh.overlay_retain,"a",@progbits', "    .balign 4",
             "    .globl __kh_overlay_retain_start", "__kh_overlay_retain_start:"]
    lines += [f"    .word {name}" for name in overlay_retain]
    lines += ["    .globl __kh_overlay_retain_end", "__kh_overlay_retain_end:", ""]
    open(os.path.join(GEN, "layout", "overlay_retain.S"), "w", newline="\n").write("\n".join(lines))

    # --- overlay table: .bss range per overlay id (FS_LoadOverlay clears it) --------------
    ovs = {m.ov_id: m for m in mods if m.ov_id is not None}
    n_ov = max(ovs) + 1 if ovs else 0
    overlay_meta = []
    for i in range(n_ov):
        m = ovs.get(i)
        entry = None
        ram_size = bss_size = 0
        if m:
            # Even an empty loadable section contributes its start address.  BSS-only
            # overlays have a zero-length .ctor followed by an alignment gap before .bss;
            # Nitro's OVT includes that gap in ram_size (0x20 for ov018/ov108-ov113).
            loaded = [(start, end) for sec, (start, end) in m.sections.items()
                      if sec != ".bss"]
            if loaded:
                load_start = min(x[0] for x in loaded)
                ram_end = m.sections.get(".bss", (max(x[1] for x in loaded), 0))[0]
                ram_size = max(0, ram_end - load_start)
            if ".bss" in m.sections:
                bss_size = m.sections[".bss"][1] - m.sections[".bss"][0]
            if ".text" in m.sections:
                text_start = m.sections[".text"][0]
                entry = next((name for addr, name, kind in m.syms
                              if addr == text_start and kind == "function" and name in g_def), None)
        overlay_meta.append((entry, ram_size, bss_size))

    lines = ["/* GENERATED by ps2/tools/gen_link.py - native overlay registry. */",
             "typedef void (*KhOverlayEntry)(void);",
             "typedef struct { KhOverlayEntry entry; unsigned int ram_size, bss_size;",
             "    unsigned char *bss_start, *bss_end, *data_start, *data_end, *cbss_start, *cbss_end;",
             "} KhOverlayInfo;", ""]
    for entry in sorted({x[0] for x in overlay_meta if x[0]}):
        lines.append(f"extern void {entry}(void);")
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
            lines.append(f"    {{ 0, 0, 0, 0, 0, 0, 0, 0, 0 }},   /* {i} */")
            continue
        entry, ram_size, bss_size = overlay_meta[i]
        if ".bss" in m.sections and m.sections[".bss"][1] > m.sections[".bss"][0]:
            b = f"__kh_bss_start_{m.name}, __kh_bss_end_{m.name}"
        else:
            b = "0, 0"
        lines.append(f"    {{ {entry or '0'}, 0x{ram_size:x}, 0x{bss_size:x}, {b}, "
                     f"__kh_data_start_{m.name}, __kh_data_end_{m.name}, "
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
    # The DS module's pure data files (no .text) go first, each at its address relative to the
    # module's lowest data-file address (DS alignment mod 32 kept): game code reaches neighbouring
    # objects across file boundaries (ov002 writes a caption's screen base through
    # data_ov002_0207ebf4 into data_ov002_0207ec00).  A PS2 object larger than its DS range makes
    # ld stop with "cannot move location counter backwards" instead of shifting its neighbours.
    data_files = collections.defaultdict(list)
    for relp, (mod, secs) in ps2cfg.ds_data_files().items():
        obj = os.path.splitext(os.path.basename(relp))[0] + ".o"
        if len(secs) == 1:
            (addr, end), = secs.values()
            data_files[mod].append((addr, end, obj, ".data .data.* .rodata .rodata.* .bss .bss.*"))
        else:
            data_files[mod].append((secs[".rodata"][0], secs[".rodata"][1], obj, ".rodata .rodata.*"))
            data_files[mod].append((secs[".data"][0], secs[".data"][1], obj, ".data .data.* .bss .bss.*"))

    def ds_layout(name):
        entries = sorted(data_files.get(name, []))
        if not entries:
            return ""
        base = entries[0][0]
        out = [f". = ALIGN(32) + 0x{base % 32:x}; __kh_dsdata_{name} = .;"]
        for addr, end, obj, secs in entries:
            out.append(f". = __kh_dsdata_{name} + 0x{addr - base:x}; KEEP(*libkh_*.a:{obj}({secs}))")
        out.append(f". = MAX(., __kh_dsdata_{name} + 0x{entries[-1][1] - base:x});")
        return "\n\t\t".join(out) + "\n\t\t"     # one placement per line: ld errors name the file

    data_lines, bss_lines = [], []
    data_lines.append("\t\t" + ds_layout("main"))
    for m in mods:
        if m.ov_id is None:
            continue
        pat = f"*libkh_*_{m.name}*.a:"
        data_lines.append(f"\t\t__kh_data_start_{m.name} = .; {ds_layout(m.name)}{pat}*(.data .data.*) "
                          f"__kh_data_end_{m.name} = .;")
        bss_lines.append(f"\t\t__kh_cbss_start_{m.name} = .; {pat}*(.bss .bss.*) {pat}(COMMON) __kh_cbss_end_{m.name} = .;")
    script, n_rodata = re.subn(
        r"(?m)^(\t\.rodata ALIGN\(128\)\s*:\s*\{)$",
        r"\1\n\t\tKEEP(*(.rodata.kh.overlay_retain))",
        base,
        count=1,
    )
    if n_rodata != 1:
        raise SystemExit("PS2SDK linkfile has no unique .rodata section; cannot retain overlay anchors")
    script = script.replace("\t\t_fdata = . ;", "\t\t_fdata = . ;\n" + "\n".join(data_lines), 1)
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
           f"- overlay archives selectively retained: {len(overlay_archives)} ({len(overlay_retain)} symbols)",
           f"- .bss symbols laid out from config: {bss_count}",
           f"- COMMON globals pinned to their DS .bss offset: {strong_count}",
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
        rep.append("")
    if bss_notes:
        rep.append("## Shared .bss not placed at its DS address")
        rep += [f"- {n}" for n in bss_notes]
    os.makedirs(os.path.join(ROOT, "build", "ps2"), exist_ok=True)
    open(os.path.join(ROOT, "build", "ps2", "link_report.md"), "w").write("\n".join(rep) + "\n")
    print("\n".join(rep[:8]))
    for lib in sorted(by_lib, key=lambda k: -len(by_lib[k])):
        print(f"  {lib}: {len(by_lib[lib])}")


if __name__ == "__main__":
    main()

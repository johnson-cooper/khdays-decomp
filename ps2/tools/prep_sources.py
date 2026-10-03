#!/usr/bin/env python3
"""Mechanical PS2 preparation of decomp sources.

A small number of matching sources contain constructs that are correct for mwccarm/ARM but
not for the EE gcc.  Instead of editing the DS sources (which must keep matching), this tool
writes adapted copies to ps2/gen/src/<same path>; gen_ps2yaml.py compiles the copy instead of
the original.  Every rule is mechanical and listed here:

  R1  mwcc `asm { clz x, x }` (ARM count-leading-zeros; the only inline asm in game code)
      -> `x = kh_clz(x);`  (kh_clz in decomp_prefix.h has ARM semantics: clz(0) == 32)
  R2  `register T v asm("rN")` ARM register pinning -> plain `register T v`
  R4  mwcc array assignment `*(T (*)[N])dst = *(T (*)[N])src;` -> __builtin_memcpy
  R5  literal DS addresses -> the PS2 display-state blocks (ps2/src/nitro/nitro_hw.c):
        0x04000000-0x040010ff  2D engine A/B + display control state   -> kh_ds_io + off
        0x05000000-0x050007ff  palette RAM                              -> kh_ds_pal + off
        0x07000000-0x070007ff  OAM                                      -> kh_ds_oam + off
        0x06800000-0x068a3fff  VRAM, LCDC view (banks A..I in order)    -> kh_ds_vram + off
        0x06000000/0x06200000/0x06400000/0x06600000 BG/OBJ windows      -> kh_ds_vram_win(n) + off
        0x027e0000-0x027fffff  DTCM + OS/ARM7 shared area               -> kh_ds_hiram + off
      The 2D engine registers are display *state* consumed by the GS compositor.  Registers
      with side effects (geometry engine 0x04000400-0x040006ff, divider/sqrt
      0x04000280-0x040002bf, IRQ/timers/DMA) must not be touched this way: files using them
      are listed in build/ps2/hw_semantic.txt and get hand-written overrides.
  R6  packed-pointer handles: resource handles keep bits 2..23 of a pointer and are decoded
      with `+ 0x01ff8000` (= 0x02000000, the DS RAM base, - 0x8000).  The decode constant
      becomes KH_DS_PACKED_PTR_BASE (decomp_prefix.h), valid because kh_mem_init() keeps the
      ELF and the game arena inside the first 16 MiB of EE RAM.
  R7  `extern T f(...);` followed by a `static [inline] T f(...) {...}` definition in the same
      file (mwcc accepts it, gcc does not): the extern prototype is dropped.
  R8  geometry engine registers (0x04000400-0x040006ff: GXFIFO, command ports, result
      registers) are not state: a store through a register macro or cast literal becomes
      kh_ge_port_write1(offset, value) and a read becomes kh_ge_port_read(offset)
      (ps2/src/nitro/nitro_g3.c), so command order and multi-word parameters are preserved.
      Taking the register's address (&REG, for MIi_CpuSend32) is left to R5.
  R9  cast-as-lvalue increments (old gcc/mwcc extension): `++((T *)(p))` -> `(*(T **)&(p) += 1)`,
      `((T *)p)++` -> `((*(T **)&(p))++)`, same for --.
  R10 top-level, non-static globals defined as zero (`T x = 0;`, `= NULL`, `= {0}`) become
      tentative definitions (`T x;`), which -fcommon merges: several decomp files repeat a library's
      globals (NitroSDK wh.c in ov105), harmless in the matching build, duplicates in a real link.
  R11 a function source that also defines (with an initializer) a global whose real definition is
      in a data/ file gets an extern declaration instead; the data file's object is the one the DS
      build uses.
  R12 decomp global names that collide with PS2SDK/newlib symbols are renamed in every decomp
      source (ps2/config/symbol_renames.txt), e.g. NitroSystem sound's internal `CreateThread`
      vs the EE kernel's.
  R13 address-coupled pointer-table fragments that game code traverses as one DS memory range
      get 4-byte alignment.  The EE compiler otherwise gives each top-level pointer array
      8-byte alignment, inserting gaps that are not present in the original table.
  R14 variadic functions that take the address of their last named parameter: on the ARM the
      variadic prologue stores r0-r3 next to the stacked arguments, so `&last` points at a
      contiguous block of 32-bit arguments (Text_FormatUtf16 hands `&c + 4` to a char* va_list;
      the projectile Fire/SetupFlight functions pass `&rest` on as an argument array).  On the EE
      the variadic arguments live in registers, so the block is rebuilt at function entry
      (the named value, then KH_VA_WORDS words fetched with va_arg) and `&last` points into it.
  R16 R8 for pointer variables: `volatile T *p = (volatile T *)0x04000440;` (a geometry register
      or register block) followed by `*p = v;` / `p->field = v;` -> kh_ge_port_write1(offset
      [+ offsetof(T, field)], v).  NitroSystem's NODEMIX drives MTX_MODE/STORE/RESTORE this way;
      as plain stores the engine never saw them and skinned models used the wrong matrices.
  R17 every definition in a file the DS links as pure data (no .text in delinks.txt) gets
      4-byte alignment, as mwcc gave it.  gen_link.py places those objects at their DS-relative
      addresses (code reaches neighbouring objects across files), and EE gcc's 8-byte boost for
      arrays and structs would otherwise push them off.  Natural 8-byte types keep 8.  Runs
      before R10, so zero-initialised data stays defined by its DS owner.
  R18 the cloned GetVarRecordByIndex functions walk even-sized, length-prefixed message records.
      ARMv5's LDR accepts their 2-mod-4 length fields (with rotate semantics); EE lw raises AdEL.
      Read those little-endian fields bytewise through kh_read_s32_le_unaligned instead.
  R19 confirmed 64-bit accesses reconstructed at offsets that are only 4-byte aligned are
      accessed through kh_unaligned.h.  R5900 ld/sd require 8-byte alignment; the DS layout
      and -fpack-struct=4 ABI stay untouched.  After all preparation rules, every PS2-bound
      source is also audited for direct 64-bit pointer dereferences whose address expression
      contains a literal non-8-byte offset; preparation fails rather than emitting a new
      hardware-only AdEL/AdES candidate.
  R15 literal ITCM addresses of data embedded in ITCM code (0x01ff8000-0x01ffffff is the top of
      EE RAM, the main thread's stack) become the PS2 definitions of that data, listed in
      ITCM_DATA (ps2/src/nitro/nitro_itcm_data.c), e.g. G3D's texture-matrix builder tables.
  R3  n32 ABI: callers that declare a function with a different 64-bit-ness than other callers
      (e.g. _s32_div_f read as `long long` = quotient|remainder<<32 vs `int` = quotient) are
      pointed at a variant symbol `<name>__<kind>`; the variants live in ps2/src/nitro/rt_*.c.
      Table: ps2/config/abi_variants.txt ("<function> <variant> <caller files>..."), generated
      by ps2/tools/abi_variants.py from ps2/tools/proto_audit.py.

    python ps2/tools/prep_sources.py          # regenerate ps2/gen/src
"""
import os
import re
import shutil
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ps2cfg  # noqa: E402

ROOT = ps2cfg.ROOT
OUT = os.path.join(ROOT, "ps2", "gen", "src")

CLZ_HELPER = re.compile(r"static\s+inline\s+unsigned\s+int\s+(\w+)\s*\(\s*unsigned\s+int\s+(\w+)\s*\)\s*\{\s*asm\s*\{\s*clz\s+\2\s*,\s*\2\s*\}\s*return\s+\2\s*;\s*\}")
CLZ_BLOCK = re.compile(r"asm\s*\{\s*clz\s+(\w+)\s*,\s*(\w+)\s*\}")
ARR_ASSIGN = re.compile(r"\*\(\s*([A-Za-z_][\w ]*?)\s*\(\*\)\s*\[(\w+)\]\s*\)\s*(\([^;=]*?\)|[\w\->.]+)\s*=\s*\*\(\s*\1\s*\(\*\)\s*\[\2\]\s*\)\s*([^;]+);")
HEXLIT = re.compile(r"\b0[xX]0*([0-9a-fA-F]{7,8})[uUlL]*\b")

# (start, end, base expression, name) -- see R5
REGIONS = [
    (0x04000000, 0x04001100, "kh_ds_io", "io"),
    (0x05000000, 0x05000800, "kh_ds_pal", "pal"),
    (0x07000000, 0x07000800, "kh_ds_oam", "oam"),
    (0x06800000, 0x068a4000, "kh_ds_vram", "vram"),
    (0x06000000, 0x06080000, "kh_ds_vram_win(0)", "vram"),
    (0x06200000, 0x06220000, "kh_ds_vram_win(1)", "vram"),
    (0x06400000, 0x06440000, "kh_ds_vram_win(2)", "vram"),
    (0x06600000, 0x06620000, "kh_ds_vram_win(3)", "vram"),
    (0x027e0000, 0x02800000, "kh_ds_hiram", "hiram"),
]
# registers with side effects: need a real replacement, never state
SEMANTIC = [(0x04000400, 0x04000700, "geometry engine"), (0x04000280, 0x040002c0, "divider/sqrt"),
            (0x040000b0, 0x04000110, "dma/timers"), (0x04000180, 0x040001c0, "ipc/card"),
            (0x04000200, 0x04000220, "irq")]
SEMANTIC_HITS = []


# An address context: right after a cast to a pointer type, or the register argument of the
# SDK's register helpers (G2x_SetBlendAlpha_(u32 reg, ...), GXx_SetMasterBrightness_, ...).
ADDR_CTX = re.compile(r"(\*\s*\)\s*\(?\s*$)"                                     # (T *)0x...
                      r"|(\*\s*\)\s*\(\s*[\w.>-]+\s*\+\s*$)"                   # (T *)(off + 0x...)
                      r"|(\b(G2x_|GXx_|G2_|GX_|G2S_|BG_CONTROL|REG_)\w*\s*\(\s*\(?\s*$)"  # reg helpers
                      r"|(#\s*define\s+\w*(ADDR|REG)\w*\s+\(?\s*$)")              # #define REG_X_ADDR 0x...
AMBIGUOUS = []
R5_FORCE = set()
_f = os.path.join(ROOT, "ps2", "config", "r5_force.txt")
if os.path.exists(_f):
    for _line in open(_f):
        _p = _line.split("#", 1)[0].split()
        if len(_p) == 2:
            R5_FORCE.add((_p[0], _p[1].lower()))


def map_addr(m, relp, before):
    v = int(m.group(1), 16)
    region = None
    for a, b, base, _ in REGIONS:
        if a <= v < b:
            region = (a, base)
    if region is None:
        return m.group(0)
    if not ADDR_CTX.search(before[-80:]) and (relp, m.group(0).lower().rstrip("ul")) not in R5_FORCE:
        AMBIGUOUS.append((relp, m.group(0), before[-60:].replace("\n", " ").strip()))
        return m.group(0)
    for a, b, why in SEMANTIC:
        if a <= v < b:
            SEMANTIC_HITS.append((relp, "0x%08x" % v, why))
    return "((unsigned int)%s + 0x%x)" % (region[1], v - region[0])


def sub_code(code, relp):
    out, pos = [], 0
    for m in HEXLIT.finditer(code):
        out.append(code[pos:m.start()])
        out.append(map_addr(m, relp, code[:m.start()]))
        pos = m.end()
    out.append(code[pos:])
    return "".join(out)


PACKED_BASE = re.compile(r"\+\s*0[xX]0?1[fF][fF]8000[uU]?\b")

STATIC_DEF = re.compile(r"^\s*static\b[^;{}()]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", re.M)


def drop_extern_of_static(text):
    names = set(STATIC_DEF.findall(text))
    for n in names:
        text = re.sub(r"^[ \t]*extern\b[^;{}]*?\b" + re.escape(n) + r"\s*\([^;{}]*\)\s*;[ \t]*\n", "", text, flags=re.M)
    return text


GE_ADDR = r"0[xX]0?4000([4-6][0-9a-fA-F]{2})[uUlL]*"
GE_CAST = r"\*\s*\(\s*(?:volatile\s+)?[A-Za-z_][\w ]*?\s*\*\s*\)\s*"
GE_DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+\(\s*" + GE_CAST + GE_ADDR + r"\s*\)[ \t]*$", re.M)
GE_DIRECT_STORE = re.compile(r"\(?\s*" + GE_CAST + GE_ADDR + r"\s*\)?\s*=(?!=)\s*([^;]+);")
GE_DIRECT_READ = re.compile(r"\(?\s*" + GE_CAST + GE_ADDR + r"\s*\)?")


import gen_hw_shadow  # noqa: E402

HW_REGS = gen_hw_shadow.hw_registers()
# register lvalue macros from include/nitro/hw.h: geometry ones for R8, side-effect ones to report
HW_GEOMETRY = {n: a - 0x04000000 for n, (a, lv) in HW_REGS.items() if lv and 0x04000400 <= a < 0x04000700}
HW_SEMANTIC_NAMES = {n: a for n, (a, lv) in HW_REGS.items() if lv}


def geometry_ports(text):
    """R8: stores/reads of geometry-engine registers become kh_ge_port_write1/read calls."""
    names = {n: off for n, off in HW_GEOMETRY.items() if re.search(r"\b" + n + r"\b", text)}
    for m in GE_DEFINE.finditer(text):
        names[m.group(1)] = int(m.group(2), 16)
    for name, off in names.items():
        # stores: NAME = expr;   (not ==, not part of a longer identifier, not &NAME)
        text = re.sub(r"(?<![\w&])" + re.escape(name) + r"\s*=(?!=)\s*([^;]+);",
                      lambda m, off=off: "kh_ge_port_write1(0x%x, (unsigned int)(%s));" % (off, m.group(1)), text)
    # remaining reads of the macros (not the #define line itself, not &NAME)
    out = []
    for line in text.split("\n"):
        if not re.match(r"^[ \t]*#[ \t]*define", line):
            for name, off in names.items():
                line = re.sub(r"(?<![\w&])" + re.escape(name) + r"\b", "kh_ge_port_read(0x%x)" % off, line)
        out.append(line)
    text = "\n".join(out)
    text = GE_DIRECT_STORE.sub(lambda m: "kh_ge_port_write1(0x%s, (unsigned int)(%s));" % (m.group(1), m.group(2)), text)
    return text


_VAR = r"[A-Za-z_][\w.]*(?:->[\w.]+)*"
_TYPE = r"[A-Za-z_][\w ]*?\s*\*"
# the variable is either bare or in ONE balanced pair of parentheses
CAST_PRE = re.compile(r"(\+\+|--)\s*\(\s*\(\s*(" + _TYPE + r")\s*\)\s*(?:\(\s*(" + _VAR + r")\s*\)|(" + _VAR + r"))\s*\)")
CAST_POST = re.compile(r"\(\s*\(\s*(" + _TYPE + r")\s*\)\s*(?:\(\s*(" + _VAR + r")\s*\)|(" + _VAR + r"))\s*\)\s*(\+\+|--)")


def cast_lvalues(text):
    orig = text
    text = CAST_PRE.sub(lambda m: "(*(%s *)&(%s) %s= 1)" % (m.group(2), m.group(3) or m.group(4), m.group(1)[0]), text)
    text = CAST_POST.sub(lambda m: "((*(%s *)&(%s))%s)" % (m.group(1), m.group(2) or m.group(3), m.group(4)), text)
    if text != orig:
        # the rewrite takes the variable's address, which `register` forbids; the keyword has no effect
        text = re.sub(r"\bregister\s+", "", text)
    return text


ZERO_GLOBAL = re.compile(r"^((?!static\b|extern\b|typedef\b|return\b)[A-Za-z_][\w \t\*]*?\b[A-Za-z_]\w*(?:\s*\[[^\]]*\])*)\s*=\s*(?:0[uUlL]*|NULL|\(\s*void\s*\*\s*\)\s*0|\{\s*0?\s*\})\s*;", re.M)


DATA_DEF = re.compile(r"(?m)^((?:static\s+)?(?!extern\b|typedef\b|return\b)(?:const\s+|volatile\s+)*"
                      r"[A-Za-z_][\w \t\*]*?\b[A-Za-z_]\w*(?:\s*\[[^\]\n]*\])*"
                      r"|\}\s*[A-Za-z_]\w*(?:\s*\[[^\]\n]*\])*"                 # `} name = {` (inline struct)
                      r"|(?:static\s+)?(?:const\s+)?(?:struct|union)\s*\w*\s*\{[^}\n]*\}\s*[A-Za-z_]\w*"
                      r"(?:\s*\[[^\]\n]*\])*)(\s*=)")                             # one-line struct { } name


def align_data_file(text, relp):
    """R17: definitions in a DS pure-data file get 4-byte alignment (see the header)."""
    if relp not in ps2cfg.ds_data_files():
        return text
    n = [0]

    def sub(m):
        decl = m.group(1)
        if "__attribute__" in decl:
            return m.group(0)
        n[0] += 1
        # the declared type's own alignment: switches off gcc's 8-byte boost for arrays and
        # structs without ever under-aligning (an inline anonymous struct cannot be named: 4)
        typ = re.sub(r"\b[A-Za-z_]\w*\s*(?:\[[^\]\n]*\]\s*)*$", "", decl)
        typ = re.sub(r"\b(static|const|volatile)\b", "", typ).strip()
        anonymous = decl.lstrip().startswith("}") or "{" in decl or not typ
        align = "4" if anonymous else f"__alignof__({typ})"
        return decl + f" __attribute__((aligned({align})))" + m.group(2)
    text = DATA_DEF.sub(sub, text)
    if n[0]:
        R17_FILES.append(relp)
    return text


R17_FILES = []


def unaligned_record_lengths(text, relp):
    """R18: make the known packed-message record boundary safe on the EE."""
    if not os.path.basename(relp).endswith("_GetVarRecordByIndex.c"):
        return text
    old = "p += *(int *)p;"
    count = text.count(old)
    if count != 1:
        raise SystemExit(f"prep R18: expected one packed length read in {relp}, found {count}")
    R18_HITS.append(relp)
    return '#include "platform/kh_unaligned.h"\n' + text.replace(
        old, "p += kh_read_s32_le_unaligned(p);", 1)


R18_HITS = []

R19_U64_UNALIGNED = {
    "src/overlays/scenes/ov000_title/Ov000_BeginSaveCheck.c": [
        (
            "    *(unsigned long long *)(data_ov000_0205ac24 + 0x4ae4) = OS_GetTick();",
            "    kh_write_u64_le_unaligned(data_ov000_0205ac24 + 0x4ae4, OS_GetTick());",
        ),
    ],
    "src/overlays/scenes/ov000_title/Ov000_TickFadeThenEnterState2.c": [
        (
            "        OS_GetTick() - *(u64 *)((u8 *)data_ov000_0205ac24 + 0x4ae4);",
            "        OS_GetTick() - kh_read_u64_le_unaligned((u8 *)data_ov000_0205ac24 + 0x4ae4);",
        ),
        (
            "    *(u64 *)((u8 *)data_ov000_0205ac24 + 0x4ae4) = OS_GetTick();",
            "    kh_write_u64_le_unaligned((u8 *)data_ov000_0205ac24 + 0x4ae4, OS_GetTick());",
        ),
    ],
    "src/overlays/scenes/ov000_title/Ov000_TickFadeOutFromCtxTimer.c": [
        (
            "        OS_GetTick() - *(u64 *)((u8 *)data_ov000_0205ac24 + 0x4ae4);",
            "        OS_GetTick() - kh_read_u64_le_unaligned((u8 *)data_ov000_0205ac24 + 0x4ae4);",
        ),
        (
            "    *(u64 *)((u8 *)data_ov000_0205ac24 + 0x4ae4) = OS_GetTick();",
            "    kh_write_u64_le_unaligned((u8 *)data_ov000_0205ac24 + 0x4ae4, OS_GetTick());",
        ),
    ],
    "src/overlays/scenes/ov000_title/Ov000_TickFadeOutFromObjTimer.c": [
        (
            "        OS_GetTick() - *(u64 *)((u8 *)data_ov000_0205ac28 + 0x14);",
            "        OS_GetTick() - kh_read_u64_le_unaligned((u8 *)data_ov000_0205ac28 + 0x14);",
        ),
        (
            "    *(u64 *)((u8 *)data_ov000_0205ac28 + 0x14) = OS_GetTick();",
            "    kh_write_u64_le_unaligned((u8 *)data_ov000_0205ac28 + 0x14, OS_GetTick());",
        ),
    ],
    "src/overlays/scenes/ov000_title/Ov000_TickFadeInFromObjTimer.c": [
        (
            "        OS_GetTick() - *(u64 *)((u8 *)data_ov000_0205ac28 + 0x14);",
            "        OS_GetTick() - kh_read_u64_le_unaligned((u8 *)data_ov000_0205ac28 + 0x14);",
        ),
        (
            "    *(u64 *)((u8 *)data_ov000_0205ac28 + 0x14) = OS_GetTick();",
            "    kh_write_u64_le_unaligned((u8 *)data_ov000_0205ac28 + 0x14, OS_GetTick());",
        ),
    ],
    "src/overlays/scenes/ov000_title/Ov000_WaitLoadThenBuildMenu.c": [
        (
            "        *(long long *)(data_ov000_0205ac28 + 0x14) = stamp;",
            "        kh_write_s64_le_unaligned(data_ov000_0205ac28 + 0x14, stamp);",
        ),
    ],
    "src/overlays/scenes/ov000_title/Ov000_TickBootFadeIn.c": [
        (
            "        context->llTimestamp = OS_GetTick();",
            "        kh_write_u64_le_unaligned((u8 *)context + 0x4c64, OS_GetTick());",
        ),
    ],
    "src/overlays/scenes/ov000_title/Ov000_MenuFadeInState.c": [
        (
            "        ctx->enterTick = OS_GetTick();",
            "        kh_write_s64_le_unaligned((u8 *)ctx + 0x4c64, OS_GetTick());",
        ),
    ],
    "src/overlays/scenes/ov000_title/Ov000_TickMenuLoop.c": [
        (
            "        ctx->enterTick = OS_GetTick();",
            "        kh_write_s64_le_unaligned((u8 *)ctx + 0x4c64, OS_GetTick());",
        ),
        (
            "    if (func_02020368((OS_GetTick() - ctx->enterTick) << 6, 0x01ff6210, 0) > 0x69) {",
            "    if (func_02020368((OS_GetTick() - kh_read_s64_le_unaligned((u8 *)ctx + 0x4c64)) << 6, 0x01ff6210, 0) > 0x69) {",
        ),
    ],
    "src/overlays/scenes/ov000_title/Ov000_TickTagTrackerNodes.c": [
        (
            "        *(unsigned long long *)(e + 0x14) += now - *(unsigned long long *)(e + 0x1c);\n"
            "        *(unsigned long long *)(e + 0x1c) = now;\n"
            "        if (*(unsigned long long *)(e + 0x14) <= *(unsigned long long *)(e + 0xc)) continue;",
            "        {\n"
            "            unsigned long long accum = kh_read_u64_le_unaligned((void *)(e + 0x14));\n"
            "            accum += now - kh_read_u64_le_unaligned((void *)(e + 0x1c));\n"
            "            kh_write_u64_le_unaligned((void *)(e + 0x14), accum);\n"
            "            kh_write_u64_le_unaligned((void *)(e + 0x1c), now);\n"
            "            if (accum <= kh_read_u64_le_unaligned((void *)(e + 0xc))) continue;\n"
            "        }",
        ),
        (
            "        *(unsigned long long *)(e + 0x14) =\n"
            "            func_02020374(*(unsigned long long *)(e + 0x14), *(unsigned long long *)(e + 0xc));",
            "        kh_write_u64_le_unaligned((void *)(e + 0x14),\n"
            "            func_02020374(kh_read_u64_le_unaligned((void *)(e + 0x14)),\n"
            "                             kh_read_u64_le_unaligned((void *)(e + 0xc))));",
        ),
    ],
    "src/overlays/enemies/ov171_enemy_grey_caprice/Ov171_CarryReleaseTick.c": [
        (
            "    if ((*(u64 *)((char *)state[3] + 0x464) & 0x8000) != 0) {",
            "    if ((kh_read_u64_le_unaligned((u8 *)state[3] + 0x464) & 0x8000) != 0) {",
        ),
    ],
    "src/overlays/enemies/ov172_enemy_grey_caprice_2/Ov172_CarryReleaseTick.c": [
        (
            "    if ((*(u64 *)((char *)state[3] + 0x464) & 0x8000) != 0) {",
            "    if ((kh_read_u64_le_unaligned((u8 *)state[3] + 0x464) & 0x8000) != 0) {",
        ),
    ],
}

R19_HITS = []


def unaligned_r5900_u64(text, relp):
    rules = R19_U64_UNALIGNED.get(relp)
    if not rules:
        return text
    for old, new in rules:
        count = text.count(old)
        if count < 1:
            raise SystemExit(f"prep R19: expected reviewed u64 access in {relp}: {old!r}")
        text = text.replace(old, new)
    R19_HITS.append(relp)
    return '#include "platform/kh_unaligned.h"\n' + text


# Direct 64-bit pointer casts are particularly dangerous on the R5900: gcc may emit ld/sd
# even when the decompiled DS address expression is only 4-byte aligned.  The explicit table
# above fixes accesses whose source shape is known; this second line of defence rejects the same
# class when it is written with another 64-bit spelling or appears in another PS2-bound file.
R19_U64_DEREF = re.compile(
    r"\*\s*\(\s*(?:(?:const|volatile)\s+)*"
    r"(?:u64|s64|uint64_t|int64_t|(?:(?:unsigned|signed)\s+)?long\s+long)"
    r"\s*\*\s*\)"
)
R19_AUDIT_FILES = 0


def _r19_code_only(text):
    def blank(m):
        return re.sub(r"[^\n]", " ", m.group(0))
    return re.sub(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"|'(?:\\.|[^'\\])*'",
                  blank, text, flags=re.S)


def _r19_matching_paren(text, pos):
    depth = 0
    for i in range(pos, len(text)):
        if text[i] == "(":
            depth += 1
        elif text[i] == ")":
            depth -= 1
            if depth == 0:
                return i
    return -1


def audit_r5900_u64_casts(text, relp):
    """Fail on statically provable non-8-byte direct u64/s64 pointer dereferences."""
    global R19_AUDIT_FILES
    if ps2cfg.is_excluded(relp):
        return
    R19_AUDIT_FILES += 1
    code = _r19_code_only(text)
    bad = []
    for m in R19_U64_DEREF.finditer(code):
        pos = m.end()
        while pos < len(code) and code[pos].isspace():
            pos += 1
        expr = None
        if pos < len(code) and code[pos] == "(":
            end = _r19_matching_paren(code, pos)
            if end >= 0:
                expr = code[pos + 1:end]
        else:
            lit = re.match(r"0[xX][0-9a-fA-F]+|\d+", code[pos:])
            if lit and (int(lit.group(0), 0) & 7):
                bad.append((code.count("\n", 0, m.start()) + 1, lit.group(0)))
        if expr is None:
            continue
        for off in re.finditer(r"[+-]\s*(0[xX][0-9a-fA-F]+|\d+)[uUlL]*\b", expr):
            value = int(off.group(1), 0)
            if value & 7:
                bad.append((code.count("\n", 0, m.start()) + 1, off.group(0).strip()))
                break
    if bad:
        where = ", ".join(f"line {line} ({off})" for line, off in bad[:8])
        raise SystemExit(
            f"prep R19 audit: unsafe direct 64-bit dereference in {relp}: {where}; "
            "use platform/kh_unaligned.h or add a reviewed mechanical R19 transform"
        )


def zero_globals(text):
    return ZERO_GLOBAL.sub(r"\1;", text)


TOP_DEF = re.compile(r"^((?!static\b|extern\b|typedef\b)(?:const\s+)?[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)((?:\s*\[[^\]]*\])*))\s*=", re.M)
DATA_DEFINED = None


def data_defined():
    """Names defined at top level in the data/ sources."""
    global DATA_DEFINED
    if DATA_DEFINED is None:
        DATA_DEFINED = set()
        for dp, dn, fn in os.walk(os.path.join(ROOT, "src")):
            if os.path.basename(dp) != "data":
                continue
            for f in fn:
                if f.endswith(".c"):
                    t = open(os.path.join(dp, f), encoding="utf-8", errors="replace").read()
                    DATA_DEFINED.update(m.group(2) for m in TOP_DEF.finditer(t))
    return DATA_DEFINED


def data_owned_to_extern(text, relp):
    if "/data/" in relp:
        return text
    names = data_defined()
    out, pos = [], 0
    for m in TOP_DEF.finditer(text):
        if m.group(2) not in names:
            continue
        # find the end of the initializer: the ';' at brace depth 0
        i, depth = m.end(), 0
        while i < len(text):
            c = text[i]
            if c in "{(":
                depth += 1
            elif c in "})":
                depth -= 1
            elif c == ";" and depth == 0:
                break
            i += 1
        out.append(text[pos:m.start()])
        decl = m.group(1)
        decl = re.sub(r"\[\s*\]", "[]", decl)
        out.append("extern " + decl + ";   /* PS2: defined in the data/ source (prep R11) */")
        pos = i + 1
    out.append(text[pos:])
    return "".join(out)


RENAMES = {}
_rp = os.path.join(ROOT, "ps2", "config", "symbol_renames.txt")
if os.path.exists(_rp):
    for _line in open(_rp):
        _p = _line.split("#", 1)[0].split()
        if len(_p) == 2:
            RENAMES[_p[0]] = _p[1]


def rename_symbols(text):
    for a, b in RENAMES.items():
        if a in text:
            text = re.sub(r"\b" + re.escape(a) + r"\b", b, text)
    return text


REG_PIN = re.compile(r"(\bregister\b[^;=]*?)\s+asm\s*\(\s*\"r\d+\"\s*\)")


ADDRESS_COUPLED_ALIGN4 = {
    # (src/engine/data/main_pointers_02041fd4.c, the first case found, is now covered by R17)
    # camp menu .bss: DS addresses 4 mod 8 inside the overlay's word-packed .bss (gen_link.py
    # pins them there; the EE's default 8-byte array alignment would forbid it)
    "src/overlays/scenes/ov008_camp_menu/data/ov008_bss_02090f14.c": ("data_ov008_02090f24",),
    "src/overlays/scenes/ov008_camp_menu/data/ov008_bss_02090fa0.c": ("data_ov008_02090fb4",),
}


def align_address_coupled_globals(text, relp):
    """R13: retain the original contiguous 32-bit layout of split DS tables."""
    for name in ADDRESS_COUPLED_ALIGN4.get(relp, ()):
        # runs after R10, so a zero definition is already tentative (`T x[n];`)
        pattern = re.compile(r"(?m)^((?!extern\b)[^/\n]*\b" + re.escape(name) +
                             r"\s*(?:\[[^\]\n]*\])?)\s*([=;])")
        text, count = pattern.subn(r"\1 __attribute__((aligned(4))) \2", text, count=1)
        if count != 1:
            raise SystemExit(f"prep R13: expected one definition of {name} in {relp}, found {count}")
    return text


GE_PTR_DECL = re.compile(
    r"volatile\s+([A-Za-z_][\w ]*?)\s*\*\s*(\w+)\s*=\s*\(\s*volatile\s+[\w ]+?\*\s*\)\s*"
    r"\(\(unsigned int\)kh_ds_io \+ (0x[0-9a-fA-F]+)\)\s*;")


def geometry_port_pointers(text, relp):
    """R16: stores through a pointer variable aimed at a geometry register (R8 for the pointer
    form; runs after R5, which turned the literal into kh_ds_io + offset)."""
    pos = 0
    while True:
        m = GE_PTR_DECL.search(text, pos)
        if not m:
            break
        pos = m.end()
        typ, name, off = m.group(1).strip(), m.group(2), int(m.group(3), 16)
        if not 0x400 <= off < 0x700:
            continue
        rest = text[m.end():]
        # statements only (a store starts a statement), never another declaration
        rest = re.sub(r"(^|[;{}])(\s*)\*\s*" + re.escape(name) + r"\s*=(?!=)\s*([^;]+);",
                      lambda s: "%s%skh_ge_port_write1(0x%x, (unsigned int)(%s));"
                                % (s.group(1), s.group(2), off, s.group(3).strip()), rest, flags=re.M)
        rest = re.sub(r"(^|[;{}])(\s*)" + re.escape(name) + r"\s*->\s*(\w+)\s*=(?!=)\s*([^;]+);",
                      lambda s: "%s%skh_ge_port_write1(0x%x + (unsigned int)__builtin_offsetof(%s, %s), "
                                "(unsigned int)(%s));" % (s.group(1), s.group(2), off, typ, s.group(3),
                                                          s.group(4).strip()), rest, flags=re.M)
        if re.search(r"(\*\s*" + re.escape(name) + r"|\b" + re.escape(name) + r"\s*->\s*\w+)\s*[|&+\-^]=", rest):
            raise SystemExit(f"prep R16: read-modify-write through geometry register pointer {name} in {relp}")
        text = text[:m.end()] + rest
        R16_HITS.append(f"{relp}: {name} (0x{off:x})")
    return text


R16_HITS = []


ITCM_DATA = {
    0x01ffa1f8: "kh_itcm_data_01ffa1f8",
    0x01ffa598: "kh_itcm_data_01ffa598",
}
ITCM_LIT = re.compile(r"\b0[xX]0*(1ff[89a-fA-F][0-9a-fA-F]{3})[uU]?\b")


def itcm_data_literals(text, relp):
    """R15: literal addresses of ITCM-resident data -> the PS2 definitions (see the header)."""
    used = set()

    def sub(m):
        name = ITCM_DATA.get(int(m.group(1), 16))
        if not name:
            return m.group(0)
        used.add(name)
        return "((unsigned int)" + name + ")"
    code = re.sub(r"/\*.*?\*/|//[^\n]*", " ", text, flags=re.S)
    if not any(int(m.group(1), 16) in ITCM_DATA for m in ITCM_LIT.finditer(code)):
        return text
    text = ITCM_LIT.sub(sub, text)
    decl = "".join("extern void *const %s[];\n" % n for n in sorted(used))
    return "/* PS2 R15 */\n" + decl + text


VARIADIC_DEF = re.compile(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(([^;{)]*?,\s*\.\.\.)\s*\)\s*\{", re.M)
KH_VA_WORDS = 12


def rebuild_variadic_blocks(text, relp):
    """R14: give `&last` of a variadic function the DS meaning (see the header)."""
    out, pos = [], 0
    for m in VARIADIC_DEF.finditer(text):
        named = [p.strip() for p in m.group(2).split(",")[:-1]]
        last = re.findall(r"(\w+)\s*$", named[-1]) if named else []
        if not last:
            continue
        last = last[0]
        i, depth = m.end(), 1
        while i < len(text) and depth:
            depth += {"{": 1, "}": -1}.get(text[i], 0)
            i += 1
        body = text[m.end():i]
        addr = re.compile(r"(?<![&\w])&\s*" + re.escape(last) + r"\b")
        # files that inline the APCS va_start as a macro (`#define va_start(ap, last) ... &(last)
        # ...`) hide the `&last` inside it: hand the macro the block instead
        vstart = re.compile(r"\bva_start\s*\(\s*(\w+)\s*,\s*" + re.escape(last) + r"\s*\)")
        macro = re.search(r"#\s*define\s+va_start\s*\([^)]*\)[^\n]*&", text) is not None
        if not addr.search(body) and not (macro and vstart.search(body)):
            continue
        body = addr.sub("((__typeof__(" + last + ") *)__kh_va)", body)
        if macro:
            body = vstart.sub(lambda v: "va_start(" + v.group(1) + ", *((__typeof__(" + last + ") *)__kh_va))",
                              body)
        init = ("\n    /* PS2 R14: the DS argument block `&" + last + "` pointed at */"
                "\n    unsigned int __kh_va[1 + " + str(KH_VA_WORDS) + "];"
                "\n    { __builtin_va_list __kh_ap; int __kh_i;"
                " __builtin_memcpy(__kh_va, &" + last + ", 4); __builtin_va_start(__kh_ap, " + last + ");"
                " for (__kh_i = 1; __kh_i <= " + str(KH_VA_WORDS) + "; __kh_i++)"
                " __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int);"
                " __builtin_va_end(__kh_ap); }\n")
        out.append(text[pos:m.end()])
        out.append(init + body)
        pos = i
        R14_HITS.append(relp + ": " + m.group(1))
    out.append(text[pos:])
    return "".join(out)


R14_HITS = []


def load_variants():
    """-> {source path: [(function, variant)]} from ps2/config/abi_variants.txt"""
    out = {}
    p = os.path.join(ROOT, "ps2", "config", "abi_variants.txt")
    if not os.path.exists(p):
        return out
    for line in open(p):
        line = line.split("#", 1)[0].split()
        if len(line) < 3:
            continue
        fn, var = line[0], line[1]
        for f in line[2:]:
            out.setdefault(f, []).append((fn, var))
    return out


def apply_variants(text, rules):
    for fn, var in rules:
        text = re.sub(r"\b" + re.escape(fn) + r"\b", var, text)
    return text


def strip_comments_aware_sub(text, relp):
    """Apply R5 to code only (comments keep the DS addresses they document)."""
    out, pos = [], 0
    for c in re.finditer(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"", text, re.S):
        out.append(sub_code(text[pos:c.start()], relp))
        out.append(c.group(0))
        pos = c.end()
    out.append(sub_code(text[pos:], relp))
    return "".join(out)


def main():
    variants = load_variants()
    wanted = set()
    n = 0
    for top in ("src", "libs"):
        for dp, dn, fn in os.walk(os.path.join(ROOT, top)):
            for f in fn:
                if not f.endswith(".c"):
                    continue
                path = os.path.join(dp, f)
                relp = os.path.relpath(path, ROOT).replace("\\", "/")
                text = open(path, encoding="utf-8", errors="surrogateescape").read()
                new = CLZ_HELPER.sub(lambda m: f"static inline unsigned int {m.group(1)}(unsigned int {m.group(2)}) {{ return kh_clz({m.group(2)}); }}", text)
                new = CLZ_BLOCK.sub(lambda m: f"{m.group(1)} = kh_clz({m.group(2)});", new)
                new = REG_PIN.sub(r"\1", new)
                new = ARR_ASSIGN.sub(lambda m: f"__builtin_memcpy((void *)({m.group(3)}), (const void *)({m.group(4)}), sizeof({m.group(1)}[{m.group(2)}]));", new)
                new = unaligned_r5900_u64(new, relp)
                new = apply_variants(new, variants.get(relp, ()))
                code_only = re.sub(r"/\*.*?\*/|//[^\n]*", " ", text, flags=re.S)
                for name, addr in HW_SEMANTIC_NAMES.items():
                    if name in code_only and re.search(r"\b" + name + r"\b", code_only):
                        for a, b, why in SEMANTIC:
                            if a <= addr < b and why != "geometry engine":
                                SEMANTIC_HITS.append((relp, "0x%08x" % addr, why))
                new = rename_symbols(new)
                new = unaligned_record_lengths(new, relp)
                new = itcm_data_literals(new, relp)
                new = cast_lvalues(new)
                new = data_owned_to_extern(new, relp)
                new = align_data_file(new, relp)
                new = zero_globals(new)
                new = align_address_coupled_globals(new, relp)
                new = rebuild_variadic_blocks(new, relp)
                new = drop_extern_of_static(new)
                new = geometry_ports(new)
                new = PACKED_BASE.sub("+ KH_DS_PACKED_PTR_BASE", new)
                new = strip_comments_aware_sub(new, relp)
                new = geometry_port_pointers(new, relp)
                audit_r5900_u64_casts(new, relp)
                if new != text:
                    dst = os.path.join(OUT, relp)
                    hdr = "/* PS2: mechanically prepared copy of %s (ps2/tools/prep_sources.py). Do not edit. */\n" % relp
                    content = hdr + new
                    wanted.add(os.path.normcase(os.path.abspath(dst)))
                    # only rewrite changed files, so the build does not recompile everything
                    try:
                        same = open(dst, encoding="utf-8", errors="surrogateescape").read() == content
                    except OSError:
                        same = False
                    if not same:
                        os.makedirs(os.path.dirname(dst), exist_ok=True)
                        open(dst, "w", encoding="utf-8", errors="surrogateescape", newline="\n").write(content)
                    n += 1
    # drop prepared copies whose source no longer needs preparing
    for dp, dn, fn in os.walk(OUT):
        for f in fn:
            path = os.path.join(dp, f)
            if os.path.normcase(os.path.abspath(path)) not in wanted:
                os.remove(path)
    os.makedirs(os.path.join(ROOT, "build", "ps2"), exist_ok=True)
    with open(os.path.join(ROOT, "build", "ps2", "hw_semantic.txt"), "w") as f:
        for relp, addr, why in sorted(set(SEMANTIC_HITS)):
            overridden = ps2cfg.is_excluded(relp)
            f.write(f"{relp}\t{addr}\t{why}\t{'overridden' if overridden else 'NEEDS OVERRIDE'}\n")
    with open(os.path.join(ROOT, "build", "ps2", "hw_ambiguous.txt"), "w") as f:
        for relp, lit, ctx in AMBIGUOUS:
            f.write(f"{relp}\t{lit}\t{ctx}\n")
    ok = set()
    okp = os.path.join(ROOT, "ps2", "config", "semantic_ok.txt")
    if os.path.exists(okp):
        for line in open(okp):
            line = line.split("#", 1)[0].split()
            if line:
                ok.add(line[0])
    pending = sorted({r for r, a, w in SEMANTIC_HITS if not ps2cfg.is_excluded(r) and r not in ok})
    print(f"prepared {n} sources into ps2/gen/src; {len(pending)} sources use side-effect registers "
          f"without a PS2 override (build/ps2/hw_semantic.txt); "
          f"{len(AMBIGUOUS)} DS-range literals left alone as non-addresses (build/ps2/hw_ambiguous.txt); "
          f"{len(R14_HITS)} variadic argument blocks rebuilt (R14); "
          f"{len(R16_HITS)} geometry register pointers routed to the engine (R16); "
          f"{len(R17_FILES)} data files aligned (R17); "
          f"{len(R18_HITS)} packed record walkers made alignment-safe (R18); "
          f"{len(R19_HITS)} confirmed sources made 64-bit alignment-safe; "
          f"{R19_AUDIT_FILES} PS2-bound sources passed the direct-u64 audit (R19)")
    for h in R16_HITS:
        print("  R16", h)


if __name__ == "__main__":
    main()

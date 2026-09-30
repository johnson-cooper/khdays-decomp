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
                new = apply_variants(new, variants.get(relp, ()))
                code_only = re.sub(r"/\*.*?\*/|//[^\n]*", " ", text, flags=re.S)
                for name, addr in HW_SEMANTIC_NAMES.items():
                    if name in code_only and re.search(r"\b" + name + r"\b", code_only):
                        for a, b, why in SEMANTIC:
                            if a <= addr < b and why != "geometry engine":
                                SEMANTIC_HITS.append((relp, "0x%08x" % addr, why))
                new = rename_symbols(new)
                new = cast_lvalues(new)
                new = data_owned_to_extern(new, relp)
                new = zero_globals(new)
                new = drop_extern_of_static(new)
                new = geometry_ports(new)
                new = PACKED_BASE.sub("+ KH_DS_PACKED_PTR_BASE", new)
                new = strip_comments_aware_sub(new, relp)
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
          f"{len(AMBIGUOUS)} DS-range literals left alone as non-addresses (build/ps2/hw_ambiguous.txt)")


if __name__ == "__main__":
    main()

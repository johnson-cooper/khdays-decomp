#!/usr/bin/env python3
"""Generate ps2.yaml for the PS2 port.

The decomp has ~23k one-function source files.  They are compiled into many small static
libraries (ee_lib), because PS2BUILD archives with a single `ar rcs lib.a <objects>` command
and Windows limits a command line to 32 KiB.  The final ELF links them all inside
--start-group/--end-group, so link order does not matter and only code reachable from the
entry point is pulled in (one function per archive member).

Source selection:
  * every directory under the configured roots (ps2/config/modules.txt) is a module;
  * files listed in ps2/config/exclude.txt are left out;
  * a file whose function has a PS2 override (ps2/overrides/**/<name>.c) is left out and the
    override is compiled instead;
  * a file with a mechanically prepared copy (ps2/gen/src/<same path>, from prep_sources.py)
    is replaced by that copy.
Directories without any of those use a glob; others list their files.

    python ps2/tools/gen_ps2yaml.py            # writes ps2.yaml
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ps2cfg  # noqa: E402
import libscan  # noqa: E402

ROOT = ps2cfg.ROOT
CHUNK = 250

IRX = ["sio2man", "padman", "mcman", "mcserv", "iomanx", "filexio", "usbd", "bdm", "bdmfs_fatfs",
       "usbmass_bd", "mmceman", "dev9", "atad", "hdd", "fs", "libsd", "khsnd"]
SDK_LIBS = ["graph", "draw", "dma", "packet2", "pad", "filexio", "patches", "debug", "eedebug", "mc", "m"]
PLATFORM_HEADERS = ["kernel", "graph", "draw", "dma", "packet2", "pad", "filexio", "patches", "debug", "eedebug", "mc"]
DEBUG = os.environ.get("KH_PS2_DEBUG", "0").lower() not in ("", "0", "false", "no", "off")
HEAP_CHECK = os.environ.get("KH_PS2_HEAP_CHECK", "0").lower() not in ("", "0", "false", "no", "off")
DEBUG_DEFINES = ["KH_PS2_DEBUG=1", "KH_PS2_PROFILE=1"] if DEBUG else []
if HEAP_CHECK:
    DEBUG_DEFINES.append("KH_PS2_HEAP_CHECK=1")


def rel(p):
    return os.path.relpath(p, ROOT).replace("\\", "/")


def module_roots():
    roots = []
    for line in open(os.path.join(ROOT, "ps2", "config", "modules.txt")):
        line = line.split("#", 1)[0].strip()
        if line:
            roots.append(line)
    return roots


def slug(path):
    s = path.replace("src/overlays/", "").replace("src/", "").replace("libs/", "lib_")
    return "kh_" + "".join(c if c.isalnum() else "_" for c in s).strip("_")


AUTOEXCLUDED = []


def collect_modules():
    """-> list of (name, [sources], uses_glob_dir or None)"""
    mods = []
    singles = []
    for root in module_roots():
        top = os.path.join(ROOT, root)
        if os.path.isfile(top):
            # a single reviewed file from an otherwise replaced (or excluded) module
            if ps2cfg.is_excluded(root, dirs=False):
                continue
            prepared = os.path.join(ROOT, "ps2", "gen", "src", root)
            singles.append("ps2/gen/src/" + root if os.path.exists(prepared) else root)
            continue
        if not os.path.isdir(top):
            raise SystemExit(f"modules.txt: {root} does not exist")
        for dp, dn, fn in os.walk(top):
            dn.sort()
            srcs = sorted(f for f in fn if f.endswith((".c", ".cpp")))
            if not srcs:
                continue
            d = rel(dp)
            if ps2cfg.is_excluded(d + "/"):
                continue
            files, replaced = [], False
            for f in srcs:
                path = d + "/" + f
                if ps2cfg.is_excluded(path):
                    replaced = True
                    continue
                why = libscan.hw_reason(path)
                if why:
                    AUTOEXCLUDED.append((path, why))
                    replaced = True
                    continue
                prepared = os.path.join(ROOT, "ps2", "gen", "src", path)
                if os.path.exists(prepared):
                    files.append("ps2/gen/src/" + path)
                    replaced = True
                else:
                    files.append(path)
            if not files:
                continue
            ext = {os.path.splitext(f)[1] for f in srcs}
            if not replaced and len(files) <= CHUNK and len(ext) == 1:
                mods.append((slug(d), ["%s/*%s" % (d, ext.pop())]))
                continue
            for i in range(0, len(files), CHUNK):
                name = slug(d) + ("" if len(files) <= CHUNK else "_%d" % (i // CHUNK))
                mods.append((name, files[i:i + CHUNK]))
    if singles:
        mods.append(("kh_lib_singles", singles))
    return mods


def overrides():
    out = []
    top = os.path.join(ROOT, "ps2", "overrides")
    for dp, dn, fn in os.walk(top):
        for f in sorted(fn):
            if f.endswith(".c"):
                out.append(rel(os.path.join(dp, f)))
    return sorted(out)


def yaml_list(items, indent):
    pad = " " * indent
    return "".join(f"{pad}- {i}\n" for i in items)


def game_flags_yaml(indent):
    pad = " " * indent
    # -include searches the -I chain, so this stays independent of ninja's working directory
    flags = ["-G0"] + ps2cfg.GAME_CFLAGS + ["-include", "platform/ps2/decomp_prefix.h"]
    return (f"{pad}include_dirs: [{', '.join(ps2cfg.GAME_INCLUDES)}]\n"
            f"{pad}defines: [{', '.join(ps2cfg.GAME_DEFINES + DEBUG_DEFINES)}]\n"
            f"{pad}cflags: [{', '.join(flags)}]\n")


def main():
    mods = collect_modules()
    ovr = overrides()
    out = []
    out.append(f"""# Kingdom Hearts 358/2 Days - native PlayStation 2 port.
#
# GENERATED by ps2/tools/gen_ps2yaml.py - edit that script (or ps2/config/*), not this file.
# Build:   ps2build build           (./build-ps2.sh regenerates the generated parts first)
# Output:  build/bin/khdays-ps2.elf, build/bin/khdays-platform-test.elf
project: khdays-ps2

targets:
  # ---- IOP: PCM output to the SPU2 (ps2/iop/khsnd) -------------------------------------
  - name: khsnd
    type: iop
    sources:
      - ps2/iop/khsnd/khsnd.c
    imports_lst: ps2/iop/khsnd/imports.lst
    include_dirs: [ps2/iop/khsnd]
    defines: [{", ".join(DEBUG_DEFINES)}]
    headers: [loadcore, threadman, sifcmd, libsd, sysclib, stdio]

  # ---- PS2 platform layer --------------------------------------------------------------
  - name: kh_platform
    type: ee_lib
    sources:
      - ps2/src/platform/*.c
      - ps2/src/gfx/*.c
    include_dirs: [ps2/include, ps2/src/platform]
    defines: [{", ".join(DEBUG_DEFINES)}]
    headers: [{", ".join(PLATFORM_HEADERS)}]
    cflags: [-std=gnu11, -Wall, -Wno-unused-function, -Wno-format]

  # ---- NitroSDK / NitroSystem API re-implemented for the PS2 ----------------------------
  - name: kh_nitro
    type: ee_lib
    sources:
      - ps2/src/nitro/*.c
      - ps2/src/audio/*.c
    include_dirs: [ps2/include, ps2/src/nitro]
    defines: [{", ".join(DEBUG_DEFINES)}]
    headers: [{", ".join(PLATFORM_HEADERS)}]
    cflags: [-std=gnu11, -Wall, -Wno-unused-function, -Wno-format, -fno-strict-aliasing]

  # ---- MobiClip frame decoder in portable C++ (ov024 calls it in place of its ARM payload) --
  - name: kh_mobiclip
    type: ee_lib
    sources:
      - libs/mobiclip/video/portable/mobiclip_frame_core.cpp
      - libs/mobiclip/video/portable/mobiclip_reference.cpp
    include_dirs: [libs/mobiclip/video/portable]
    cflags: [-O2, -Wall, -fno-exceptions, -fno-rtti]

  # ---- platform bring-up test ------------------------------------------------------------
  - name: khdays-platform-test
    type: ee
    sources:
      - ps2/src/bringup/platform_test.c
    include_dirs: [ps2/include]
    defines: [{", ".join(DEBUG_DEFINES)}]
    cflags: [-std=gnu11, -Wall, -Wno-format]
    libs: [kh_platform, {", ".join(SDK_LIBS)}]
    embed_irx: [{", ".join(IRX)}]
""")

    out.append("\n  # ---- PS2 code compiled like game code: function overrides + ABI variants -----------\n")
    out.append("  - name: kh_ps2game\n    type: ee_lib\n    sources:\n")
    out.append(yaml_list(ovr + ["ps2/src/abi/*.c"], 6))
    out.append(game_flags_yaml(4))

    out.append("\n  # ---- decomp sources (game + kept libraries), %d archives ------------------------\n" % len(mods))
    for name, srcs in mods:
        out.append(f"  - name: {name}\n    type: ee_lib\n    sources:\n")
        out.append(yaml_list(srcs, 6))
        out.append(game_flags_yaml(4))

    resident_and_overlay_libs = ["kh_ps2game"] + [m[0] for m in mods if not m[0].startswith("kh_lib_")]
    decomp_sdk_libs = [m[0] for m in mods if m[0].startswith("kh_lib_")]
    out.append(f"""
  # ---- the game ------------------------------------------------------------------------
  - name: khdays-ps2
    type: ee
    sources:
      - ps2/src/main/*.c
      - ps2/src/vu/*.vsm
      - ps2/gen/layout/*.S
      - ps2/gen/stubs/*.c
    include_dirs: [ps2/include]
    defines: [{", ".join(DEBUG_DEFINES)}]
    cflags: [-std=gnu11, -Wall, -Wno-format, -Wno-unused-function]
    linkfile: ps2/gen/khdays.ld
    # --wrap: frees to the game heaps are validated first (ps2/src/nitro/nitro_heapdbg.c); enemy
    # spawns are logged (ps2/src/nitro/nitro_objdbg.c)
    ldflags: ["-Wl,--start-group", "-Wl,-Map=khdays-ps2.map", "-Wl,--wrap=NNS_FndFreeToExpHeap",
              "-Wl,--wrap=NNS_FndAllocFromExpHeapEx",
              "-Wl,--wrap=Ov107_SpawnEntityClass", "-Wl,--wrap=Ov002_CreateSlotObjectAndStart",
              "-Wl,--wrap=Ov002_RegisterEventSlot", "-Wl,--wrap=Ov002_HandleSeatMessage",
              "-Wl,--wrap=Ov002_PopAndDispatchEvent", "-Wl,--wrap=FS_OpenFile"]
    libs:
""")
    # The retention anchor extracts overlay members while the game archives are scanned.  Put
    # the PS2 compatibility archives immediately after them, before the original DS libraries,
    # so those newly exposed SDK references resolve to the PS2 implementation on the same pass.
    # Merely putting kh_nitro first is insufficient: it has already been scanned when an overlay
    # later creates the undefined reference, allowing the DS archive to win before group rescan.
    out.append(yaml_list(resident_and_overlay_libs + ["kh_nitro", "kh_platform", "kh_mobiclip"] +
                         decomp_sdk_libs + SDK_LIBS, 6))
    out.append("    embed_irx: [%s]\n" % ", ".join(IRX))
    open(os.path.join(ROOT, "ps2.yaml"), "w", newline="\n").write("".join(out))
    os.makedirs(os.path.join(ROOT, "build", "ps2"), exist_ok=True)
    with open(os.path.join(ROOT, "build", "ps2", "lib_autoexcluded.txt"), "w") as f:
        for path, why in AUTOEXCLUDED:
            f.write(path + "\t" + why + "\n")
    # Compiled sources that touch side-effect registers and were neither overridden nor reviewed.
    compiled = set()
    for _, srcs in mods:
        for f in srcs:
            compiled.add(f[len("ps2/gen/src/"):] if f.startswith("ps2/gen/src/") else f)
    ok = set()
    okp = os.path.join(ROOT, "ps2", "config", "semantic_ok.txt")
    if os.path.exists(okp):
        for line in open(okp):
            line = line.split("#", 1)[0].split()
            if line:
                ok.add(line[0])
    pending = set()
    semp = os.path.join(ROOT, "build", "ps2", "hw_semantic.txt")
    if os.path.exists(semp):
        for line in open(semp):
            parts = line.rstrip("\n").split("\t")
            if len(parts) >= 3 and parts[2] != "geometry engine" and parts[0] in compiled and parts[0] not in ok:
                pending.add(parts[0])
    with open(os.path.join(ROOT, "build", "ps2", "hw_pending.txt"), "w") as f:
        for x in sorted(pending):
            f.write(x + "\n")
    if pending:
        print(f"WARNING: {len(pending)} compiled sources use side-effect registers without review "
              f"(build/ps2/hw_pending.txt): " + ", ".join(sorted(pending)[:5]))
    nsrc = sum(len(s) for _, s in mods)
    print(f"ps2.yaml: {len(mods)} archives ({nsrc} entries), {len(ovr)} overrides, "
          f"{len(AUTOEXCLUDED)} library files auto-excluded")


if __name__ == "__main__":
    main()

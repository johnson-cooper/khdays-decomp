#!/bin/sh
# Build the native PlayStation 2 port with PS2BUILD.
#
#   ./build-ps2.sh                         normal build (debug logs/profiler off)
#   KH_PS2_DEBUG=1 ./build-ps2.sh          diagnostic build (logs/profiler/checks on)
#   ./build-ps2.sh --quick                 skip regeneration (same as plain `ps2build build`)
#
# Output: build/bin/khdays-ps2.elf (the game) and build/bin/khdays-platform-test.elf.
# Game data is prepared separately, from your own ROM: see docs/PS2_PORT.md ("Game data").
#
# Steps:
#   1. prep_sources.py  - PS2 copies of the few sources needing mechanical adaptation
#   2. gen_ps2yaml.py   - ps2.yaml (archives per source directory, overrides, ELF targets)
#   3. ninja            - compile every archive (the ELF link needs step 4 first)
#   4. gen_link.py      - .bss layouts, data aliases, overlay ids, stubs for missing SDK functions
#   5. ps2build build   - link
#   6. audit_r5900_ldsd.py - inspect emitted EE code for unsafe 64-bit accesses
# Any failure stops the script with the tool's own error output.
trap 'echo "An error occurred!"; read -p "Press Enter to close..." ' EXIT
set -eu
cd "$(dirname "$0")"

die() { echo "build-ps2.sh: $*" >&2; exit 1; }

command -v ps2build >/dev/null 2>&1 || die "ps2build not found on PATH (install PS2BUILD: https://ps2.techwritescode.dev/)"
PY=
for cand in ${PYTHON:-} python3 python; do
    # (on Windows "python3" may be a Microsoft Store stub that exists but does not run)
    if "$cand" -c "import sys; sys.exit(sys.version_info < (3, 8))" >/dev/null 2>&1; then PY=$cand; break; fi
done
[ -n "$PY" ] || die "python 3.8+ not found (set PYTHON=...)"

if [ "${1:-}" != "--quick" ]; then
    echo "== [1/6] preparing sources"
    "$PY" ps2/tools/gen_hw_shadow.py || die "gen_hw_shadow.py failed"
    "$PY" ps2/tools/prep_sources.py || die "prep_sources.py failed"
    echo "== [2/6] generating ps2.yaml"
    "$PY" ps2/tools/gen_ps2yaml.py || die "gen_ps2yaml.py failed"
    echo "== [3/6] compiling archives"
    ps2build generate >/dev/null || die "ps2build generate failed"
    SDK=$(dirname "$(command -v ps2build)")
    NINJA="$SDK/tools/ninja"
    [ -x "$NINJA" ] || [ -x "$NINJA.exe" ] || NINJA=ninja
    # PS2BUILD's archive rule uses `ar rcs`, which updates an existing archive but does not
    # remove members whose source was excluded since the previous generation.  Recreate just
    # the generated game archives so their membership exactly matches ps2.yaml; compiled
    # objects remain cached, so this is cheap compared with a clean rebuild.
    rm -f build/lib/libkh_*.a
    TARGETS=$(sed -n 's/^  - name: \(kh_[A-Za-z0-9_]*\)$/\1/p' ps2.yaml)
    # shellcheck disable=SC2086
    # On Windows `ar` occasionally fails with "could not create temporary file ... Permission
    # denied" while a scanner holds the directory.  The failed step leaves an empty archive newer
    # than its inputs, which ninja would then consider up to date (silently dropping that
    # overlay's code or data), so delete exactly the outputs that failed and run once more; a
    # genuine compile error fails the second pass too.
    NINJA_LOG=build/ninja_archives.log
    # shellcheck disable=SC2086
    "$NINJA" -k 0 -C build $TARGETS 2>&1 | tee "$NINJA_LOG"
    if grep -q '^FAILED: ' "$NINJA_LOG"; then
        FAILED=$(sed -n 's/^FAILED: \(\[code=[0-9-]*\] \)\{0,1\}\(lib\/[^ ]*\.a\).*/\2/p' "$NINJA_LOG")
        echo "build-ps2.sh: retrying failed steps once:" $FAILED
        for a in $FAILED; do rm -f "build/$a"; done
        # shellcheck disable=SC2086
        "$NINJA" -k 0 -C build $TARGETS || die "compiling the game archives failed (errors above)"
    fi
    # an archive holding no members means a lost step, never a legitimately empty module
    for a in build/lib/libkh_*.a; do
        [ "$(wc -c < "$a")" -gt 8 ] || die "$a is empty (a failed archive step?); delete it and rebuild"
    done
    echo "== [4/6] generating link glue"
    "$PY" ps2/tools/gen_link.py || die "gen_link.py failed"
fi
echo "== [5/6] ps2build build"
ps2build build || die "ps2build build failed (errors above)"
echo "== [6/6] auditing R5900 64-bit loads/stores"
"$PY" ps2/tools/audit_r5900_ldsd.py || die "R5900 ld/sd alignment audit failed"
ls -l build/bin/*.elf

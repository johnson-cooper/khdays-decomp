#!/bin/sh
# Build the native PlayStation 2 port with PS2BUILD.
#
#   ./build-ps2.sh            regenerate the generated parts, build everything
#   ./build-ps2.sh --quick    skip regeneration (same as plain `ps2build build`)
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
# Any failure stops the script with the tool's own error output.
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
    echo "== [1/5] preparing sources"
    "$PY" ps2/tools/gen_hw_shadow.py || die "gen_hw_shadow.py failed"
    "$PY" ps2/tools/prep_sources.py || die "prep_sources.py failed"
    echo "== [2/5] generating ps2.yaml"
    "$PY" ps2/tools/gen_ps2yaml.py || die "gen_ps2yaml.py failed"
    echo "== [3/5] compiling archives"
    ps2build generate >/dev/null || die "ps2build generate failed"
    SDK=$(dirname "$(command -v ps2build)")
    NINJA="$SDK/tools/ninja"
    [ -x "$NINJA" ] || [ -x "$NINJA.exe" ] || NINJA=ninja
    TARGETS=$(sed -n 's/^  - name: \(kh_[A-Za-z0-9_]*\)$/\1/p' ps2.yaml)
    # shellcheck disable=SC2086
    "$NINJA" -k 0 -C build $TARGETS || die "compiling the game archives failed (errors above)"
    echo "== [4/5] generating link glue"
    "$PY" ps2/tools/gen_link.py || die "gen_link.py failed"
fi
echo "== [5/5] ps2build build"
ps2build build || die "ps2build build failed (errors above)"
ls -l build/bin/*.elf

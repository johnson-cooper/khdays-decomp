#!/usr/bin/env python3
"""Patch PS2BUILD's generated Ninja link rule so khdays-ps2 closes its archive group.

The final game has hundreds of mutually dependent static archives, so gen_ps2yaml.py
opens a GNU ld --start-group before them. PS2BUILD exposes target ldflags before libs,
but currently has no post-library ldflags field. Appending --end-group to the generated
target's `libs =` value is not reliable because PS2BUILD/Ninja may transform that
variable while constructing the response file.

Instead, extend the generated linker rule's rspfile_content with a new `$postlibs`
variable and set that variable only on the khdays-ps2 target. This guarantees the
response file ends with -Wl,--end-group:

    <objects> <flags including --start-group> <libs> -Wl,--end-group

The patch is idempotent and only changes build/build.ninja, which PS2BUILD regenerates.
"""

from pathlib import Path

NINJA = Path("build/build.ninja")
TARGET = "build obj/khdays-ps2/khdays-ps2.unstripped.elf:"
END_GROUP = "-Wl,--end-group"


def main() -> None:
    if not NINJA.is_file():
        raise SystemExit(f"{NINJA} not found; run ps2build generate first")

    lines = NINJA.read_text(encoding="utf-8").splitlines()

    # Extend every rspfile_content line that already emits $libs so a target-local
    # postlibs variable can place raw linker-driver arguments after the libraries.
    rule_patched = False
    for i, line in enumerate(lines):
        if "rspfile_content =" in line and "$libs" in line:
            if "$postlibs" not in line:
                lines[i] = line.replace("$libs", "$libs $postlibs")
            rule_patched = True

    if not rule_patched:
        raise SystemExit(f"could not find a PS2BUILD linker rspfile_content using $libs in {NINJA}")

    target_index = next((i for i, line in enumerate(lines) if line.startswith(TARGET)), None)
    if target_index is None:
        raise SystemExit(f"could not find khdays-ps2 link target in {NINJA}")

    # Remove the previous attempted fix if it is still present on the libs line.
    insert_at = target_index + 1
    found_postlibs = False
    for i in range(target_index + 1, len(lines)):
        line = lines[i]
        if line.startswith("build "):
            insert_at = i
            break
        if line.startswith("  libs = ") and END_GROUP in line:
            lines[i] = line.replace(" " + END_GROUP, "").replace(END_GROUP, "")
        if line.startswith("  postlibs = "):
            lines[i] = f"  postlibs = {END_GROUP}"
            found_postlibs = True
        insert_at = i + 1

    if not found_postlibs:
        lines.insert(insert_at, f"  postlibs = {END_GROUP}")

    NINJA.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print("PS2BUILD link fix: khdays-ps2 response file will close --start-group after libraries")


if __name__ == "__main__":
    main()

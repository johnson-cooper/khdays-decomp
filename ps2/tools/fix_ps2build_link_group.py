#!/usr/bin/env python3
"""Patch PS2BUILD's generated Ninja file so khdays-ps2 closes its linker group.

ps2/tools/gen_ps2yaml.py intentionally starts the final game's libraries with
-Wl,--start-group because hundreds of static archives have circular references.
PS2BUILD emits target ldflags before its libs, but its schema has no post-library
ldflags field, so --end-group cannot be expressed in ps2.yaml without putting it
before the libraries.

After `ps2build generate`, add --end-group to the target-local `libs =` value
for khdays-ps2.  Ninja then writes the response file as:

    <objects> <ldflags including --start-group> <libs ... --end-group>

which is the ordering GNU ld requires.
"""

from pathlib import Path

NINJA = Path("build/build.ninja")
TARGET = "build obj/khdays-ps2/khdays-ps2.unstripped.elf:"
END_GROUP = "-Wl,--end-group"


def main() -> None:
    if not NINJA.is_file():
        raise SystemExit(f"{NINJA} not found; run ps2build generate first")

    lines = NINJA.read_text(encoding="utf-8").splitlines()
    target_index = next((i for i, line in enumerate(lines) if line.startswith(TARGET)), None)
    if target_index is None:
        raise SystemExit(f"could not find khdays-ps2 link target in {NINJA}")

    libs_index = None
    for i in range(target_index + 1, len(lines)):
        line = lines[i]
        if line.startswith("build "):
            break
        if line.startswith("  libs = "):
            libs_index = i
            break

    if libs_index is None:
        raise SystemExit(f"could not find target-local libs line for khdays-ps2 in {NINJA}")

    if END_GROUP not in lines[libs_index]:
        lines[libs_index] += f" {END_GROUP}"
        NINJA.write_text("\n".join(lines) + "\n", encoding="utf-8")
        print("PS2BUILD link fix: appended --end-group after khdays-ps2 libraries")
    else:
        print("PS2BUILD link fix: --end-group already present")


if __name__ == "__main__":
    main()

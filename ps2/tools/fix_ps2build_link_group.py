#!/usr/bin/env python3
"""Patch PS2BUILD's generated Ninja link rule so khdays-ps2 closes its archive group.

The final game has hundreds of mutually dependent static archives. gen_ps2yaml.py opens a GNU ld
--start-group before them, but PS2BUILD currently exposes only pre-library ldflags. The closing
--end-group therefore has to be injected after the response file contents.

Older revisions tried to add a target-local $postlibs token to rspfile_content. On current
PS2BUILD/Ninja that can disappear before the compiler driver reaches ld, producing:

    ld: missing --end-group; added as last command line option

Patch the linker rule's *command* instead. A target-local $postlink variable is appended after
@$rspfile, so the compiler driver receives:

    ... @khdays-ps2.unstripped.elf.rsp -Wl,--end-group

This is unambiguously after every library in the response file. The patch is idempotent and only
changes build/build.ninja, which PS2BUILD regenerates.
"""

from pathlib import Path

NINJA = Path("build/build.ninja")
TARGET = "build obj/khdays-ps2/khdays-ps2.unstripped.elf:"
END_GROUP = "-Wl,--end-group"


def main() -> None:
    if not NINJA.is_file():
        raise SystemExit(f"{NINJA} not found; run ps2build generate first")

    lines = NINJA.read_text(encoding="utf-8").splitlines()

    # Find the khdays-ps2 build edge and the rule it uses.
    target_index = next((i for i, line in enumerate(lines) if line.startswith(TARGET)), None)
    if target_index is None:
        raise SystemExit(f"could not find khdays-ps2 link target in {NINJA}")

    target_line = lines[target_index]
    try:
        rule_name = target_line.split(":", 1)[1].strip().split()[0]
    except (IndexError, ValueError):
        raise SystemExit(f"could not determine linker rule from: {target_line}")

    # Remove the older rspfile-content injection if this checkout/build.ninja already has it.
    for i, line in enumerate(lines):
        if "rspfile_content =" in line and "$postlibs" in line:
            lines[i] = line.replace(" $postlibs", "").replace("$postlibs ", "")
        if line.startswith("  postlibs = "):
            lines[i] = ""

    # Patch the exact rule used by khdays-ps2. The postlink token goes after @$rspfile (or after
    # the full command as a fallback), which guarantees --end-group reaches g++ after all libs.
    rule_index = next((i for i, line in enumerate(lines) if line == f"rule {rule_name}"), None)
    if rule_index is None:
        raise SystemExit(f"could not find Ninja rule {rule_name!r}")

    command_patched = False
    for i in range(rule_index + 1, len(lines)):
        line = lines[i]
        if line.startswith("rule ") or line.startswith("build "):
            break
        if line.lstrip().startswith("command = "):
            if "$postlink" not in line:
                lines[i] = line.rstrip() + " $postlink"
            command_patched = True
            break

    if not command_patched:
        raise SystemExit(f"could not find command for Ninja rule {rule_name!r}")

    # Set postlink only on the game target. Other EE links (platform test, IOP modules) must not
    # receive an unmatched --end-group.
    insert_at = target_index + 1
    found_postlink = False
    for i in range(target_index + 1, len(lines)):
        line = lines[i]
        if line.startswith("build "):
            insert_at = i
            break
        if line.startswith("  postlink = "):
            lines[i] = f"  postlink = {END_GROUP}"
            found_postlink = True
        insert_at = i + 1

    if not found_postlink:
        lines.insert(insert_at, f"  postlink = {END_GROUP}")

    NINJA.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print("PS2BUILD link fix: khdays-ps2 command will append --end-group after its response file")


if __name__ == "__main__":
    main()

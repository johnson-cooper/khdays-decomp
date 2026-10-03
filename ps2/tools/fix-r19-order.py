#!/usr/bin/env python3
from pathlib import Path
import sys

p = Path("ps2/tools/prep_sources.py")
if not p.is_file():
    print("ERROR: run this from the khdays-decomp repository root.", file=sys.stderr)
    raise SystemExit(1)

text = p.read_text(encoding="utf-8")

apply_line = '                new = apply_variants(new, variants.get(relp, ()))'
late_block = (
    '                new = unaligned_record_lengths(new, relp)\n'
    '                new = unaligned_title_u64(new, relp)\n'
)

if apply_line not in text:
    print("ERROR: could not find apply_variants() line; prep_sources.py differs from expected.", file=sys.stderr)
    raise SystemExit(1)

if late_block not in text:
    # If the early call is already present, report success rather than duplicating it.
    early = (
        '                new = unaligned_title_u64(new, relp)\n'
        '                new = apply_variants(new, variants.get(relp, ()))'
    )
    if early in text:
        print("R19 ordering is already fixed.")
        raise SystemExit(0)
    print("ERROR: could not find the current late R19 call; no changes made.", file=sys.stderr)
    raise SystemExit(1)

# Remove the late call first.
text = text.replace(
    late_block,
    '                new = unaligned_record_lengths(new, relp)\n',
    1,
)

# Insert R19 immediately before the ABI variant renaming pass.
text = text.replace(
    apply_line,
    '                new = unaligned_title_u64(new, relp)\n' + apply_line,
    1,
)

p.write_text(text, encoding="utf-8", newline="\n")

print("Fixed R19 ordering in ps2/tools/prep_sources.py")
print("R19 now runs before apply_variants(), so it matches the original source text.")
print("No commit or push was performed.")
print("")
print("Next:")
print("  git diff --check")
print("  ./build-ps2.sh")

#!/bin/sh
# Development helper: pcsx2_to_story.sh, then advance the first story scenes with Cross and close
# the first field tutorial page with Start, leaving the player free to move in the Grey Area.
#   ps2/tools/win/pcsx2_to_field.sh [build/bin/khdays-ps2.elf]
cd "$(dirname "$0")/../../.." || exit 1
KEY="python ps2/tools/win/pine_press.py"   # PINE input: needs no window focus
sh ps2/tools/win/pcsx2_to_story.sh "${1:-build/bin/khdays-ps2.elf}" >/dev/null || exit 1
i=0
while [ "$i" -lt 30 ]; do
    $KEY cross >/dev/null; sleep 2
    i=$((i + 1))
done
$KEY start >/dev/null; sleep 3

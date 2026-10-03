#!/bin/sh
# Development helper: boot, start a New Game (pcsx2_newgame.sh) and skip the two opening movies
# with Start, leaving the game at the first story dialogue (state slot 1 is worth saving there,
# e.g. through the PCSX2 MCP).  Fails if a step does not happen.
#   ps2/tools/win/pcsx2_to_story.sh build/bin/khdays-ps2.elf
cd "$(dirname "$0")/../../.." || exit 1
LOG="${PCSX2_DIR:-/e/PCSX2-MCP-v1.0.0-win64/PCSX2-MCP-v1.0.0-win64}/logs/emulog.txt"
KEY="python ps2/tools/win/pine_press.py"   # PINE input: needs no window focus
sh ps2/tools/win/pcsx2_newgame.sh "${1:-build/bin/khdays-ps2.elf}" 6 >/dev/null || exit 1
$KEY start >/dev/null            # skip the opening movie (ov012)
t=0
until [ "$(grep -ac 'MobiClip stream opened' "$LOG")" -ge 2 ]; do
    t=$((t + 1)); [ "$t" -gt 60 ] && { echo "pcsx2_to_story.sh: second movie never started" >&2; exit 1; }
    sleep 1
done
sleep 4
$KEY start >/dev/null; sleep 14  # skip the first field movie (ov024)
grep -aE "\[[IWED] +[0-9]+ |PANIC|xception|Syscall|TLB Miss" "$LOG" | grep -v "unmapped VRAM" | tail -${LINES_MAX:-8}

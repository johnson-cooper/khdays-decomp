#!/bin/sh
# Development helper: boot an ELF in PCSX2 and drive the title menu to a Standard-mode New Game
# (empty save: Story Mode -> New Game -> Standard -> Yes).  Waits on emulog markers rather than
# fixed delays, and fails if the game does not get there.
#   ps2/tools/win/pcsx2_newgame.sh build/bin/khdays-ps2.elf [seconds to wait afterwards]
cd "$(dirname "$0")/../../.." || exit 1
LOG="${PCSX2_DIR:-/e/PCSX2-MCP-v1.0.0-win64/PCSX2-MCP-v1.0.0-win64}/logs/emulog.txt"
KEY="python ps2/tools/win/pine_press.py"   # PINE input: needs no window focus

# wait_for <regex> <timeout seconds>: 0 once the emulog has a matching line
wait_for() {
    t=0
    while [ "$t" -lt "$2" ]; do
        grep -aqE "$1" "$LOG" 2>/dev/null && return 0
        sleep 1
        t=$((t + 1))
    done
    echo "pcsx2_newgame.sh: timed out waiting for '$1'" >&2
    return 1
}

ps2/tools/win/run_pcsx2.sh "${1:-build/bin/khdays-ps2.elf}" 3 >/dev/null
wait_for "PCM channels" 60 || exit 1   # the title menu's sound starts once it is interactive
sleep 2
$KEY cross >/dev/null; sleep 3        # Story Mode
$KEY cross >/dev/null; sleep 3        # New Game
$KEY cross >/dev/null; sleep 3        # Standard Mode
$KEY left >/dev/null; sleep 1; $KEY cross >/dev/null   # Yes
wait_for "MobiClip stream opened" 60 || exit 1
sleep "${2:-2}"
grep -aE "\[[IWED] +[0-9]+ |PANIC|xception|Unhandled" "$LOG" | tail -${LINES_MAX:-20}

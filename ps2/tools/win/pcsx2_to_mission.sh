#!/bin/sh
# Development helper: boot an ELF and drive the title menu to Mission Mode as a guest with Roxas,
# up to the character screen's "Start: proceed" prompt (pass "go" as the second argument to press
# Start as well).
#   ps2/tools/win/pcsx2_to_mission.sh [build/bin/khdays-ps2.elf] [go]
cd "$(dirname "$0")/../../.." || exit 1
LOG="${PCSX2_DIR:-/e/PCSX2-MCP-v1.0.0-win64/PCSX2-MCP-v1.0.0-win64}/logs/emulog.txt"
KEY="python ps2/tools/win/pine_press.py"   # PINE input: needs no window focus

ps2/tools/win/run_pcsx2.sh "${1:-build/bin/khdays-ps2.elf}" 3 >/dev/null
t=0
until grep -aq "PCM channels" "$LOG" 2>/dev/null; do
    t=$((t + 1)); [ "$t" -gt 60 ] && { echo "pcsx2_to_mission.sh: title never came up" >&2; exit 1; }
    sleep 1
done
sleep 2
$KEY down >/dev/null; sleep 1; $KEY cross >/dev/null; sleep 4      # Mission Mode
$KEY cross >/dev/null; sleep 6                                      # Solo
$KEY down >/dev/null; sleep 1; $KEY down >/dev/null; sleep 1
$KEY down >/dev/null; sleep 1; $KEY cross >/dev/null; sleep 3       # Guest Play
$KEY left >/dev/null; sleep 1; $KEY cross >/dev/null; sleep 8       # Yes
$KEY cross >/dev/null; sleep 5                                      # Roxas
[ "$2" = go ] || [ "$2" = start ] && { $KEY start >/dev/null; sleep 6; }
if [ "$2" = start ]; then                                      # mission 00 (tutorial)
    $KEY cross >/dev/null; sleep 4                                  # Holo-Missions
    $KEY cross >/dev/null; sleep 4                                  # mission 00
    $KEY cross >/dev/null; sleep 3                                  # Embark
    $KEY left >/dev/null; sleep 1; $KEY cross >/dev/null; sleep 8   # Yes
    $KEY start >/dev/null; sleep 10                            # Start: begin
fi
exit 0

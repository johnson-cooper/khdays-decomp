#!/bin/sh
# Development helper: symbolize the last "[objs] states:" line of the PCSX2 log (object state
# functions, logged every 300 VBlanks by kh_debug_log_object_states).
#   ps2/tools/win/symstates.sh [count of lines from the end, default 1]
cd "$(dirname "$0")/../../.." || exit 1
LOG="${PCSX2_DIR:-/e/PCSX2-MCP-v1.0.0-win64/PCSX2-MCP-v1.0.0-win64}/logs/emulog.txt"
A2L=/c/Users/yvnem/AppData/Local/ps2build/toolchain/ee/bin/mips64r5900el-ps2-elf-addr2line.exe
ELF=build/obj/khdays-ps2/khdays-ps2.unstripped.elf
grep -a "objs\] states:" "$LOG" | tail -"${1:-1}" | while read -r line; do
    addrs=$(echo "$line" | sed 's/.*states://')
    echo "$line" | cut -c1-24
    for a in $addrs; do
        printf '  %8s %s\n' "$a" "$($A2L -f -e "$ELF" "0x$a" | head -1)"
    done
done

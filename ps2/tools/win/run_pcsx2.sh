#!/bin/sh
# Development helper: (re)start PCSX2 on an ELF and wait for it to boot.
#   ps2/tools/win/run_pcsx2.sh build/bin/khdays-ps2.elf [seconds]
PCSX2_DIR=${PCSX2_DIR:-/e/PCSX2-MCP-v1.0.0-win64/PCSX2-MCP-v1.0.0-win64}
ELF=$(cygpath -w "$(realpath "$1")")
taskkill //F //IM pcsx2-qt.exe >/dev/null 2>&1
sleep 1
rm -f "$PCSX2_DIR/logs/emulog.txt"
powershell -NoProfile -Command "Start-Process -FilePath '$(cygpath -w "$PCSX2_DIR")\pcsx2-qt.exe' -WorkingDirectory '$(cygpath -w "$PCSX2_DIR")' -ArgumentList '-elf','\"$ELF\"'"
sleep "${2:-15}"
echo "--- emulog (filtered) ---"
grep -aE "\[[IWED] +[0-9]+ |PANIC|Unhandled|exception|Exception|TLB" "$PCSX2_DIR/logs/emulog.txt" | head -${LINES_MAX:-80}

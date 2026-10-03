# Press keys in the PCSX2 window (development helper, Windows only).
#   powershell -ExecutionPolicy Bypass -File ps2/tools/win/pcsx2_key.ps1 "Return K Down" [-HoldMs 120] [-GapMs 400]
# Key names are .NET System.Windows.Forms.Keys names (Return, Up, Down, Left, Right, K, L, ...),
# separated by spaces or commas.  With the default PCSX2 bindings: Return=Start, K=Cross,
# L=Circle, J=Square, I=Triangle, Backspace=Select, arrows=D-pad, W/A/S/D=left stick.
param([string]$KeyList = "", [int]$HoldMs = 120, [int]$GapMs = 400)
$Keys = $KeyList -split '[,\s]+' | Where-Object { $_ }
Add-Type -AssemblyName System.Windows.Forms
Add-Type @"
using System;
using System.Runtime.InteropServices;
public class K {
  [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
  [DllImport("user32.dll")] public static extern void keybd_event(byte vk, byte scan, uint flags, UIntPtr extra);
  [DllImport("user32.dll")] public static extern uint MapVirtualKey(uint code, uint type);
}
"@
$p = Get-Process pcsx2-qt -ErrorAction Stop | Where-Object { $_.MainWindowHandle -ne 0 } | Select-Object -First 1
[K]::SetForegroundWindow($p.MainWindowHandle) | Out-Null
Start-Sleep -Milliseconds 150
$extended = @('Up', 'Down', 'Left', 'Right', 'Insert', 'Delete', 'Home', 'End', 'PageUp', 'PageDown')
foreach ($name in $Keys) {
    $vk = [byte][System.Windows.Forms.Keys]::$name
    $scan = [byte][K]::MapVirtualKey($vk, 0)
    $ext = if ($extended -contains $name) { 1 } else { 0 }
    [K]::keybd_event($vk, $scan, $ext, [UIntPtr]::Zero)
    Start-Sleep -Milliseconds $HoldMs
    [K]::keybd_event($vk, $scan, $ext -bor 2, [UIntPtr]::Zero)
    Start-Sleep -Milliseconds $GapMs
    Write-Output "pressed $name"
}

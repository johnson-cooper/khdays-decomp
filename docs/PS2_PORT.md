# Kingdom Hearts 358/2 Days — native PlayStation 2 port

This document is the architecture audit and working plan for the native PS2 port that lives on
the `ps2-port` branch. It is written from the actual decomp sources, not from general DS
knowledge; figures come from `ps2/tools/audit_deps.py` (report in `build/ps2/audit.md`) and
`ps2/tools/trial_compile.py`.

**Goal:** a real EE executable, `build/bin/khdays-ps2.elf`, built with PS2BUILD, that runs the
decompiled game logic natively on a PlayStation 2, with the Nintendo DS platform layer (NitroSDK,
NitroSystem, the ARM7, DS hardware registers) replaced by a PS2 platform layer. No DS emulation.

**Assets are never committed.** The user supplies their own ROM; `ps2/tools/` extracts and
converts it into `ps2data/` (git-ignored).

---

## 1. What the repository is

A *matching* decompilation of the EU (`YKGP`) ARM9 program: 23,336 game C files (one function per
file) plus 1,284 library functions in `libs/` (NitroSDK, NitroSystem, MSL runtime, MobiClip).
BSS and some data are not defined in C: the matching build takes their addresses from
`config/arm9/**/symbols.txt` through dsd's linker script. The ARM7 program is not decompiled
(`docs/ARM7.md` maps it).

Things that make a port viable:

* The EE, like the ARM946, is 32-bit little-endian (`long`/pointer = 32 bits under the n32 ABI
  PS2BUILD uses), so the decomp's pervasive int/pointer punning and struct offsets carry over.
* The game is almost entirely portable C. A syntax-only EE compile of **all 23,336 game files
  fails on just 10** (`asm { clz }` blocks in 5 functions, one mwcc array assignment, and the
  DS Protect overlay).
* Every function and data symbol name is unique across the main module and all 303 overlays,
  so every overlay can be linked statically into one ELF.

Things that make it hard (all detailed below): the renderer (G3D display lists, the 2D BG/OAM
engines, both screens), audio (the ARM7 runs the sequencer), per-file prototypes that disagree
with each other (dangerous under n32), code that encodes DS addresses in integers, and ~900 raw
hardware-register accesses in ~400 game functions.

### Region note

The decomp is the EU build. The ROM available during development is the **US** build (`YKGE`).
The PS2 port runs the decomp's (EU) code and reads *assets* from the user's ROM. Assets are
opened by path (`FS_OpenFile(Msg_BuildLangPath(name))`), not by file id, so region differences
reduce to which language directories exist (US: `en`, `fr` for most UI; EU adds `de`, `it`,
`es`). The language setting is forced to one present in the supplied ROM. File-id based access
(FS_OpenFileFast) is not used by game code. This must be re-verified per asset family as
subsystems come up; see *Open questions*.

---

## 2. Classification of the code

| Class | What | Where | PS2 treatment |
|---|---|---|---|
| Portable game logic | scenes, field, battle, AI, enemies, players, scripts, menus logic, collision math | `src/engine`, `src/overlays/*` | compiled unchanged |
| Portable with minor adaptation | functions with `asm{clz}`, mwcc extensions, conflicting 64-bit prototypes, DS address arithmetic | ~20 known + audit | PS2 overrides / prepared copies (never edits to the DS files) |
| DS abstraction layer (SDK API used by the game) | OS, FS, MI, FX, MTX, GX, G2, G3, SND, PAD, TP, CARD, RTC, PM, WM | `libs/nitro` (523 functions referenced) | re-implemented in `ps2/src/nitro/` on top of the kh platform layer |
| DS hardware-specific | register pokes, VRAM/palette/OAM, DMA, divider/sqrt unit, IRQ, ARM7 shared words | ~400 game functions, most of `libs/nitro` | replaced (overrides call platform APIs); never wrapped as fake memory |
| DS middleware | NitroSystem G3D/G2D/GFD/SND/FND, MobiClip | `libs/nns`, `libs/mobiclip`, `ov024` | FND reused as C; G3D/G2D data-side reused, output side replaced; SND replaced; MobiClip via the portable decoder |

"Do not port NitroSDK blindly" in practice: the port implements the **523 SDK/NitroSystem
entry points the game actually calls**, listed in `build/ps2/audit.md`, not the SDK.

---

## 3. Subsystem audit

### 3.1 Initialization / boot

`main()` (`src/engine/main.c`): `OS_Init`, `FS_Init(3)`, loads overlay **ov001** once to run
`Ov001_BootInit` (VRAM banks, display registers, touch panel, heaps), unloads it, then
`PublishArchiveVTableAndSeedRng`, `InputState_Init`, reads the boot profile
(`Game_ReadLocalProfile`), `Record_EnsureAllocatedAndSetId`, loads the boot resource tables,
`Obj_InitSystem`, `Callbacks_Init`, `SoundCtx_Init`, and instantiates the root task
(`InstantiateClass(&gBootTaskClass)` → `BootTask_Construct` → `Scene_RequestPending(SCENE_TITLE)`).

`Ov001_CreateMainAndSubHeaps` sizes the heaps from **DS Protect** predicates in ov028 (a
tampered cart gets a crippled heap). → PS2 override: always the untampered layout.

PS2: `ps2/src/platform/boot.c` does platform bring-up (IOP reset, IRX loading, boot-path
detection, logging, memory, pads, GS, audio), then calls the decomp's `main()` unchanged in
spirit (its DS-only parts are handled by the replaced SDK functions and overrides).

### 3.2 Main loop

One thread, fixed-step: `OS_WaitVBlankIntr` → `FrameStep_UpdateTaskQueue` → `Pad_Sample` →
`G3X_ResetMtxStack` → per pause-mode: `Obj_UpdateAll` (3D path) and/or `Callbacks_Run` (2D /
capture path) → `SoundMgr_Update` → frame pacing → `GXi_FlushCommandList` + `REG 0x04000540 = 1`
(G3_SwapBuffers) → scene poll. Lid-close/sleep handling (`PM_*`, the ARM7 hinge bit) is dropped
on PS2.

### 3.3 Timing / VBlank

The simulation is locked to the DS VBlank (59.826 Hz). `gObjSystem` byte 0 is the frame-rate
mode: 0 = 60 Hz, 1 = 30 Hz (waits for 2 VBlanks), 2 = unpaced. Simulation advances one step per
game frame; there is no delta time. `OS_GetTick` (timer 0 at 33.51 MHz / 64) is used in 120
files, mostly for profiling, RNG seeding and time-outs.

PS2: VBlank from the GS (`WaitSema` on a VBlank-start handler); NTSC 59.94 Hz / PAL 50 Hz.
A frame-count based pacer keeps the simulation at the DS rate: on NTSC one step per VBlank
(+0.2 % speed), on PAL the simulation is stepped from an accumulator (60/50) so gameplay speed
is preserved; rendering always shows the latest simulated state. `OS_GetTick` is backed by the
EE `COUNT` register scaled to the DS tick rate so tick arithmetic keeps its units.

### 3.4 Rendering (3D)

The game draws 3D through NitroSystem **G3D** (`NNS_G3dRenderObjInit`, `NNS_G3dDraw*`,
`NNS_G3dGlb*`, material/polygon setters: 48 G3D functions in 433 files), plus its own display
lists (`Gfx_SubmitCachedCommandBlock`, `GX_SendFifoWords` in 49 files) and billboards
(`Billboard_DrawList` writes the geometry registers directly). G3D executes the model's SBC
(structure byte code: node/material/shape commands) and sends each shape's **display list** —
packed GX geometry commands (`BEGIN_VTXS`, `VTX_16/10/XY/…`, `NORMAL`, `TEXCOORD`, `COLOR`,
`MTX_*`, `POLYGON_ATTR`, `TEXIMAGE_PARAM`, `PLTT_BASE`) — to the geometry engine.

PS2 design:

1. **Geometry front end** (`ps2/src/gfx/ge.c`): a DS-geometry-state tracker that implements
   the matrix stack, the current matrices (projection, position, vector, texture), material
   and polygon state, and decodes packed command lists. It is not a FIFO emulator: commands are
   decoded from memory (display lists, `G3_*` calls) straight into
   **batched EE vertex buffers** (per material/texture/state), with fixed point converted once
   at this boundary.
2. **GS back end** (`ps2/src/gfx/gs_*.c`): converts batches into GIF packets (triangles /
   strips, `PRIM` with Gouraud, texture, alpha, fog, Z) and sends them by DMA (path 3 first,
   path 1 via VU1 later). DS depth (`W`/`Z` buffering, 1/4096 fixed) maps to a 24-bit Z buffer.
3. **Offline pre-conversion** (next step after correctness): NSBMD display lists are converted
   on the PC into PS2 vertex batches + VU1 packets (the SBC stays: it drives matrices,
   materials and visibility), so the EE no longer decodes display lists per frame.
4. VU1 (transform, lighting, clipping) replaces the EE transform once profiling says so; the
   batch format is designed for `VIF UNPACK` from day one.

DS lighting (4 directional lights, material diffuse/ambient/specular/emission, toon/shininess
tables), fog, edge marking, translucency (polygon ID rules) and alpha blending are mapped to GS
equivalents where they exist; where they do not (edge marking, polygon-ID translucency rules),
approximations are documented at the implementation.

### 3.5 Model rendering / 3.6 animation

Model files (`BMD0`, 566 in the ROM) and animations (`BCA0` joint 3,630, `BMA0` material 306,
`BVA0` visibility 256, `BTA0` texture SRT 102, `BTP0` texture pattern 88) live inside the game's
`.p`/`.p2` packs. G3D animation evaluation (`NNSi_G3dAnmCalcNsBca` etc.) and the SBC walker are
pure fixed-point C in `libs/nns/g3d` and are reused unchanged; only the geometry output
changes. The game's own animation helpers (`Anim_*`, `JointModel_*`, `ModelAnimSet_*`) are
portable game code.

### 3.7 Textures

DS texture formats used by G3D: A3I5, 4-colour, 16-colour, 256-colour, 4x4 compressed
(texel + palette index data), A5I3, direct 16-bit. `BTX0` blocks (474) are uploaded by the game
into texture VRAM through NitroSystem GFD (`NNS_GfdAllocTexVram`, 19 GFD functions in 107
files) and bound with `TEXIMAGE_PARAM`/`PLTT_BASE`.

PS2 plan:

| DS format | PS2 representation | Notes |
|---|---|---|
| 4-colour / 16-colour | PSMT4 + 16-entry CLUT (CSM1, PSMCT16/32) | colour 0 transparency flag → CLUT alpha |
| 256-colour | PSMT8 + 256-entry CLUT | |
| A3I5 / A5I3 | PSMT8 with a per-texture expanded CLUT (index × alpha level) | stays 8 bpp |
| 4x4 compressed | pre-decoded offline to PSMT8+CLUT where ≤256 colours, else PSMCT16 | never decoded per frame |
| direct | PSMCT16 (RGBA5551) | |

`NNS_Gfd*` allocations become **handles into a GS VRAM residency cache**
(`ps2/src/gfx/vram_cache.c`): each texture/palette records VRAM address, PSM, size, CLUT,
owner, generation and last-used frame; misses upload by DMA (path 3 IMAGE), eviction is LRU
among entries not used this frame. Framebuffers and Z are allocated first and never evicted.

### 3.8 Sprites / HUD / 2D

The HUD, menus, text and most of the UI are DS 2D engines: BG layers (text/affine tilemaps,
`RGCN`/`RSCN`/`RLCN`), OAM sprites (NitroSystem G2D cells/animations, `RCSN`/`RECN`/`RNAN`,
28 G2D functions in 142 files), blending (`BLDCNT`/`BLDALPHA`/`BLDY`), windows, master
brightness, and 3D-to-BG0. The game writes most 2D state through registers directly (the
`io:2d-main` / `io:2d-sub` rows below).

PS2: a **2D compositor** (`ps2/src/gfx/disp2d.c`) owns the state of each logical DS screen
(engine A/B: display mode, BG0–3 control/scroll/affine, OBJ, blend, windows, brightness) and
the logical 2D VRAM/palette/OAM contents written through the SDK (`GX_LoadBG*Char`,
`GX_LoadOAM`, palette loads, G2D). Each frame it renders BG layers as GS textured sprites
(tiles converted to PSMT4/PSMT8 with CLUTs, cached in GS VRAM by the residency cache) and OAM
entries as sprites, in DS priority order, with DS blend equations mapped to GS alpha.
Register-poking game functions are replaced by overrides calling this API — no DS addresses
remain.

### 3.9 Input

`Pad_Sample` reads `KEYINPUT` (`0x04000130`) and folds in the ARM7's X/Y/hinge word
(`0x027fffa8`). Touch: `TP_*` (touch panel) in menus/minigames.

PS2 mapping (DualShock 2 via `libpad`, `ps2/src/platform/pad.c` → platform-neutral
`KhPadState`):

| DS | PS2 |
|---|---|
| D-pad | D-pad (and left stick as digital until analog movement is added deliberately) |
| A / B / X / Y | Circle / Cross / Triangle / Square (Japanese-style A=confirm is configurable) |
| L / R | L1 / R1 (camera / lock-on as in the DS game) |
| Start / Select | Start / Select |
| touch | virtual cursor (right stick / D-pad in touch screens), Cross = tap, Circle = cancel |
| lid | none |

`Pad_Sample` is overridden to build the DS key word from `KhPadState`, so the rest of the
game's input code (repeat, key-trigger logic) is unchanged.

### 3.10 Touch screen

`TP_*` is used by the title/boot sequence (`Ov001_InitTouchPanel`, calibration), menus and the
panel system. Audit per scene is ongoing; gameplay itself is button-driven. PS2: `TP_*` is
implemented on a virtual cursor that screens can enable; screens that *require* touch get
explicit focus navigation.

### 3.11 Filesystem

Game code calls `FS_OpenFile(path)` / `FS_ReadFile(Async)` / `FS_SeekFile` /
`FS_OpenFileDirect(archive, top, bottom)` and `FS_LoadOverlay`. The file loader thread
(`FileLoader_ThreadMain`) reads in 0x200-byte blocks and LZ-decompresses on the fly.
Paths are language-substituted (`Msg_BuildLangPath`).

PS2 (`ps2/src/platform/vfs.c`): a VFS over the boot device. Game files are packed offline
into `ps2data/khdays.pak` (NitroFS directory + file table + 2 KiB-aligned file data; ARM code
overlays are dropped). The VFS reads through a large (256 KiB) read-ahead cache so the
loader's small sequential reads become large aligned device reads. `FS_OpenFileDirect` image
offsets are relative to the pack, like ROM offsets are to the cartridge.

Boot device independence: the ELF path from `argv[0]` gives the device and directory
(`mass0:/…`, `mass1:`, `mmce0:`, `hdd0:…:pfs:`, `host:`, `cdrom0:`); `ps2data/` is resolved
relative to it. USB (BDM + usbmass_bd), MMCE, HDD and host drivers are loaded only as needed by
the detected device, from IRX modules embedded with PS2BUILD `embed_irx`.

### 3.12 Resource manager

`Archive_*`, `FileLoader_*`, `Gfx_*`, `ResCache` (`0x0201e1d0–0x0201f79c`) are game code and
stay. They depend on FS (above) and on a DS-address encoding: `Archive_OpenSubfileByHandle` /
`Archive_SubfileIsCompressed` and the `*_TickTagTrackerNodes` family pack a pointer into
24 bits relative to `0x01ff8000`. These are overridden with a handle table.

### 3.13 Overlay manager

303 overlays (`FS_LoadOverlay`/`FS_UnloadOverlay`, overlay id = address of the absolute
symbol `OVERLAY_<n>_ID`). Why they exist: DS RAM (4 MiB). Kinds: scenes (ov000–ov012),
sub-screens, field modules, **four load slots of 19 player characters** (ov030–ov104: the same
code linked at different addresses), and enemies (ov114–ov301, many byte-identical copies for
the same reason).

PS2: all overlay code is statically linked (no symbol collisions exist). `FS_LoadOverlay`
keeps the DS *semantics* that matter: it re-initialises the overlay's `.data` from a pristine
copy, clears its `.bss`, and runs its static initialisers (`.ctor`), so scene re-entry behaves
as on the DS. The linker script gives each overlay its own data/bss output range for this.
Duplicate slot overlays cost EE RAM only for their code; deduplication is a later
optimisation. ov001 (boot), ov028 (DS Protect) and ov105 (wireless) are special-cased.

### 3.14 Sound / 3.15 Music

ARM9 side: NitroSystem SND (`NNS_Snd*`, 41 functions in 55 files; `NNS_SndArcLoadSeq/Bank`,
`SoundMgr_*`, `SoundCtx_*` game code). ARM7 side: the NitroSDK sound driver, which **runs the
sequencer** (`SND_SeqMain`), the channel mixer, envelopes, LFOs, ADPCM/PCM playback and
PSG/noise channels. The single `snd/*.sdat` (60 MiB) holds SSEQ (sequences), SBNK (banks),
SWAR (wave archives) and STRM streams.

PS2: the ARM7 driver is **replaced**, not run: `ps2/src/audio/` implements the SND command
interface (the ARM9 side of NNS SND stays, its commands go to our driver instead of PXI).
The sequencer and envelope logic runs on the EE at the DS tick rate; voices go to **SPU2** —
SWAR waves pre-converted offline to SPU2 ADPCM (VAG) and uploaded per bank to SPU2 RAM
(2 MiB) with residency management; streams go through `audsrv`. No per-frame codec work on
the EE.

### 3.16 Save data

`CARD_*` backup (EEPROM/flash) from the save scenes (ov009, ov000, ov008/ov025 run a card
thread). PS2: save slots become files in a memory-card directory (`BASLUS-KHDAYS/` style name,
`icon.sys` + icon), written atomically (write `*.tmp`, verify, rename). Bring-up uses a plain
file on the boot device. The DS backup API is emulated at the *API* level only
(read/write/verify by offset) over the slot file.

### 3.17 Threading

Threads created by the game: the file loader thread (`FileLoader_Init`) and the card/save
thread (ov000/ov008/ov009/ov025). `OS_SendMessage`/`OS_ReceiveMessage`, `OS_SleepThread`/
`OS_WakeupThread`, `OS_RescheduleThread`, alarms (2 users). PS2: `OSThread`, `OSMessageQueue`
and `OSThreadQueue` are implemented on EE kernel threads/semaphores with the same
cooperative-priority semantics (the game relies on `OS_RescheduleThread` yields).

### 3.18 Interrupts

`OS_DisableInterrupts`/`OS_RestoreInterrupts` for critical sections, `OS_SetIrqFunction`,
`OS_EnableIrqMask` (VBlank), `OSi_VBlankInterruptHandler`. PS2: critical sections map to
`DI`/`EI` (or a mutex where only threads race); VBlank callbacks run from our VBlank handler
at thread level (never inside the interrupt), DMA/timer/card IRQs have no equivalent and are
dropped.

### 3.19 Memory allocation

DS: OS arena → NNS FND expanded heaps (`Ov001_CreateMainAndSubHeaps`: an 800 KiB main heap
and a sub heap), frame heaps and unit heaps; the game allocates through
`NNSi_FndAllocFromDefaultExpHeap`/`NNSi_FndFreeFromDefaultHeap` (hundreds of call sites). FND is
portable C and is reused unchanged.

PS2 budget (32 MiB EE RAM):

| Region | Budget |
|---|---|
| ELF code+data (all overlays static) | ≈ 10–12 MiB (measured per build, see diagnostics) |
| DS-equivalent game arena (FND heaps) | 6 MiB (DS had < 4 MiB) |
| PS2 renderer (display lists, batches, GIF packets double-buffered) | 2 MiB |
| texture staging / 2D logical VRAM | 1.5 MiB |
| file cache + pack index | 1 MiB |
| audio (sequencer, stream buffers) | 0.5 MiB |
| newlib heap / stacks / IOP buffers | 2 MiB |
| safety margin | ≥ 4 MiB |

`ps2/src/platform/mem.c` provides tagged arenas with lifetimes (global, scene, mission, room,
character, enemy, effect, frame) for PS2-side allocations and reports used/free/largest-free
per arena and for the FND heaps (debug overlay + serial log).

### 3.20 Video playback

`.mods` MobiClip files (46, 108 MiB) played by ov024 (C++ decoder calling ARM assembly
kernels) and ov012 (opening). `libs/mobiclip/video/portable/` already contains a portable C++
frame decoder (`docs/MOBICLIP_DECODER.md`). PS2 plan: evaluate the portable decoder on the EE
(256×192 at 15–24 fps is small); if too slow, pre-convert offline to PS2 MPEG-2 (IPU decode via
`libmpeg`). Until then the player is a **clearly logged temporary skip** that reports "movie
finished" so scenes proceed. Cutscenes are not removed.

### 3.21 Wireless / networking

ov105 (NitroSDK WM + `wh.c`), Mission Mode multiplayer (ov006/ov008 lobby), `MB_*`. PS2: single
player only for now. `WM_*` returns "unavailable" so the lobby shows no partners; ov105 is not
started. The ARM7-shared words it pokes (`0x027ffc3c` etc.) are replaced with a PS2 state
struct.

### 3.22 ARM7 dependencies

| ARM7 service | Used through | PS2 replacement |
|---|---|---|
| Sound driver (SND_* commands, sequencer, mixer) | PXI from NNS SND / SND | EE sequencer + SPU2 voices (`ps2/src/audio`) |
| X/Y buttons + hinge (`0x027fffa8`) | `Pad_Sample`, main loop | pad backend |
| Touch panel (SPI) | `TP_*` | virtual cursor |
| RTC | `RTC_*` (date/time for saves) | EE `sceCdReadClock` / platform time |
| Microphone | none found in game code | — |
| Power management / lid | `PM_*` | not needed (no-op) |
| Wireless (WM) | ov105 | unavailable |
| Firmware user info (language, name, MAC) | `OS_GetMacAddress`, `Game_ReadLocalProfile` | PS2 config (language from `ps2data/config` / OSD) |
| Card/backup via ARM7 | `CARD_*` | memory card |

### 3.23 Direct DS hardware accesses in game code

From `build/ps2/audit.md` (regenerate with `python ps2/tools/audit_deps.py`):

| Region | Refs | Files | Replacement |
|---|---:|---:|---|
| 2D engine A registers (`0x0400000x`) | 376 | 167 | 2D compositor API |
| 2D engine B registers (`0x0400100x`) | 327 | 167 | 2D compositor API |
| POWCNT (`0x04000304`) / LCD swap | 45 | 43 | compositor "which logical screen is main" |
| ARM7/OS shared RAM (`0x027ffc20`, `0x027ffc3c`, `0x027fffa8`, …) | 34 | 32 | boot/pad/wireless state structs |
| palette RAM | 29 | 17 | compositor palette |
| divider / sqrt unit (`0x04000280–0x040002b8`) | 25 | 15 | exact integer C (collision, tweens) |
| 3D geometry registers | 11 | 3 | geometry front end |
| OAM / VRAM | 20 | 17 | compositor |
| KEYINPUT | 6 | 6 | pad backend |
| ITCM/DTCM-relative addresses | 18 | 10 | handle table / real symbols |

The divider/sqrt users (`Coll_*`, `RoomBox_HitTop`, tweens) are gameplay-critical: their
replacements reproduce the hardware results bit-exactly (64/32 signed division, truncating).

---

## 4. Compiler portability

| Issue | Where | Fix |
|---|---|---|
| gcc 15 defaults to C23 and makes implicit declarations / int-conversion errors | everywhere | `-std=gnu99 -fpermissive` |
| mwcc `-char signed -enum int` | everywhere | `-fsigned-char`, gcc default enums |
| ARM wraps signed overflow; code relies on it | math, RNG | `-fwrapv` |
| type punning | everywhere | `-fno-strict-aliasing` |
| sources re-declare SDK names with different types | everywhere | `-fno-builtin`, no SDK headers force-included |
| `asm { clz }` (5 functions) | MsgQueue, PartyState, ov002, ov003 | overrides using a `clz` with ARM semantics (clz(0)=32) |
| **per-file prototypes returning `long long` vs `int` for the same function** (`_s32_div_f`/`func_02020400` returns quotient in r0 and remainder in r1; files read it as `int` or as `long long`) | ~300 files | n32 returns 64-bit values in one register and `int` callers use the full register, so one symbol cannot serve both: the PS2 source-prep step (`ps2/tools/prep_sources.py`) renames the callee per declared type |
| `long long`/`double` alignment 8 on MIPS | rare structs | static asserts in overrides where layouts cross the DS data boundary |
| ARM assembly in libs (`asm_stubs`: MSL runtime, SDK hand-written routines) | libs only | re-implemented in C at the API boundary; no ARM interpreter |
| mwcc array assignment | Ov252_PlaceBodyParts | override |
| DS Protect (ov028) | anti-tamper | excluded; its callers overridden |

The DS sources are never edited for the PS2. PS2-only changes live in `ps2/overrides/`
(hand-written replacements of whole functions) and `ps2/gen/` (mechanically prepared copies
generated by `ps2/tools/`), selected by the build generator.

---

## 5. Repository layout for the port

```
ps2.yaml                     PS2BUILD project (generated by ps2/tools/gen_ps2yaml.py, committed)
build-ps2.sh                 prepare + ps2build build; fails loudly
docs/PS2_PORT.md             this file
ps2/
  include/platform/          kh platform interface (kh_platform.h, kh_*.h)
  include/platform/ps2/      decomp_prefix.h and PS2-private headers
  src/platform/              PS2 platform layer: boot, log, mem, time, vfs, pad, gs, audio, save
  src/nitro/                 NitroSDK API subset re-implemented on the platform layer
  src/nns/                   NitroSystem replacements (GFD → VRAM cache, SND → audio, G2D/G3D output)
  src/gfx/                   geometry front end, GS back end, 2D compositor, VRAM residency cache
  overrides/                 PS2 replacements of individual game functions (same name as the DS file)
  config/                    exclude lists, per-module options
  tools/                     audit, trial compile, source prep, yaml generator, ROM → ps2data
  gen/                       generated sources (symbol layouts, prepared copies)
ps2data/                     user-generated game data (git-ignored, never committed)
```

---

## 6. Bring-up plan and status

| # | Milestone | Status |
|---|---|---|
| 1 | Architecture audit, this document | done (living document) |
| 2 | PS2BUILD project, minimal ELF, serial/stdout log | in progress |
| 3 | Platform layer: memory diagnostics, timing/VBlank, VFS + boot path, pad, GS clear | in progress |
| 4 | Link the game: generated BSS/data layouts, overlay IDs, SDK replacement stubs that log | pending |
| 5 | `main()` runs: heaps, task system, scene 1 constructed | pending |
| 6 | 2D compositor: title screen BGs/OAM | pending |
| 7 | Geometry front end + GS back end: first G3D model | pending |
| 8 | Title/menus, new game | pending |
| 9 | Field scene, player + camera, combat | pending |
| 10 | Audio (SDAT sequencer + SPU2) | pending |
| 11 | Memory-card saves | pending |
| 12 | MobiClip playback | pending |
| 13 | VU1 path, pre-converted models, profiling-driven optimisation | pending |

## 7. Open questions

* US vs EU assets: confirm every path the EU code builds exists in the US ROM (log misses in
  the VFS); confirm pack/model formats are identical between regions.
* Duplicate player/enemy overlays: link once and alias, or keep separate (only RAM at stake).
* Polygon-ID based translucency and edge marking on GS.

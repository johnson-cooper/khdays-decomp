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

## 0. Building and running

**Requirements:** PS2BUILD (`ps2build` on PATH, https://ps2.techwritescode.dev/), Python 3.8+.
Nothing else: the toolchain, PS2SDK packages and IOP modules come from PS2BUILD.

```sh
./build-ps2.sh            # full: prepare sources, generate ps2.yaml + link glue, build
KH_PS2_DEBUG=1 ./build-ps2.sh
                           # diagnostic build: INFO/debug logs, profiler, expensive checks
ps2build build            # same result once the generated files exist (they are committed)
```

Normal builds leave routine logging and profiling disabled. Warnings, errors and panic diagnostics
remain enabled.

Outputs: `build/bin/khdays-ps2.elf` (the game) and `build/bin/khdays-platform-test.elf` (platform
diagnostics: boot device, pads, memory, GS).  A full build compiles ~24,000 files; later builds are
incremental.

**Game data (from your own ROM; never committed, never distributed):**

```sh
python ps2/tools/make_ps2data.py "Kingdom Hearts 358-2 Days.nds" ps2data
```

This writes `ps2data/khdays.pak` (the ROM's NitroFS files, 2 KiB aligned, ~245 MiB; no program
code) and `ps2data/info.txt`.  Put the `ps2data` folder next to the ELF:

```
mass:/KHDAYS/khdays-ps2.elf          (USB; also mass1:, mmce0:/..., hdd0:PART:pfs:/..., host:)
mass:/KHDAYS/ps2data/khdays.pak
```

The ELF finds its own directory from `argv[0]`; saves go to `khdays.sav` in the same directory
during bring-up (memory card saves are planned, 3.16).

**PCSX2:** boot the ELF (`pcsx2-qt -elf build/bin/khdays-ps2.elf`) with `ps2data/` next to it
(e.g. a directory junction `build/bin/ps2data -> ps2data`).  `ps2/tools/win/run_pcsx2.sh` restarts
PCSX2 on an ELF and prints the port's log lines from `emulog.txt`.

**Controls (DualShock 2):** Cross = A (attack/confirm), Circle = B (jump/cancel), Triangle = X,
Square = Y, L1/L2 = L, R1/R2 = R, Start, Select, D-pad or left stick = directions.
L3 cycles the screen layout while the single-screen presentation is being built.
Hold R3 and press L3 to toggle a readout of the DS 2D display registers (layer enables, BG
controls, blending, windows, capture, VRAM banks) over the picture, for reporting rendering bugs.

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

The active development data is the European **YKGP** release, matching the decompiled program.
`make_ps2data.py` records the ROM game code in the pack header and the PS2 VFS logs it during
startup; bring-up builds are expected to report `code target YKGP; data source YKGP`. Other regions are not an active
compatibility target until the matching EU port is playable.

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

PS2 implementation (as built, 2026-10-01):

```
EE   game logic, G3D SBC walk (matrices, materials)                 [game code]
EE   geometry front end  ps2/src/nitro/nitro_ge.c
       packed-command decoder (parameters used in place), DS matrix stack in DS fixed point
       (MULT+3xMADD dot products = DS 64-bit precision), lighting (fixed point)
VU0  (macro mode) float clip matrix = pos x proj, vertex transform (LQC2/VMULA../SQC2)
EE   face culling (3x3 homogeneous determinant, no divides), outcodes, guard-band clipping
       (only near/far always clipped; x/y only beyond 4x the view volume), triangles appended
       to per-pass lists (opaque / translucent) in the VU1 input layout
VIF1 DMA chain (the whole frame packet): GS state + 2D as DIRECT (PATH2), each run of same-
       state triangles as UNPACK V4-32 *by reference* from the triangle list (no copy) + MSCAL,
       double-buffered through BASE/OFFSET; FLUSH before DIRECT data that follows VU1 work
VU1  ps2/src/vu/vu1_tri.vsm: 1/w, viewport + 24-bit Z mapping, ST*q, colour scale,
       GIF PACKED (ST RGBAQ XYZ2) -> XGKICK (PATH1), output double-buffered
GS   rasterisation, texture, blending, Z, alpha/destination-alpha tests
```

The EE projection path is kept (`kh_ge_use_vu1 = 0`) for A/B comparison.  A next step is
offline pre-conversion of NSBMD display lists into VU1 vertex batches, so the EE stops
decoding display lists per frame; with the current costs (6.6 ms GE in Mission 00, 14.4 ms in
the Grey Area at 30 fps) it is not yet needed.

DS 3D semantics implemented on the GS: opaque / translucent passes (translucent = polygon alpha
1-30 **or a translucent texture format, A3I5 / A5I3**), translucent depth writes per
POLYGON_ATTR bit 11, the rear plane (CLEAR_COLOR colour and alpha), **shadow polygons**
(POLYGON_ATTR mode 3: mask polygons, ID 0, set a stencil bit where they fail the depth test;
shadow polygons draw only where it is set - on the GS the stencil is frame-buffer alpha bit 7,
written with FBA/FBMSK and tested with DATE), mirrored texture repeat (a doubled, mirrored copy
stored once per cache miss), and the 3D layer drawn at **BG0's priority** among the 2D layers
(only when BG0 is enabled, as the DS).  Not modelled yet: wireframe (alpha 0, drawn solid),
polygon-ID rules for translucent overdraw, fog, edge marking, toon/highlight tables (none of
these were seen in the scenes audited so far: the geometry front end logs each polygon mode and
DISP3DCNT setting the first time it is used).

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

As built (`ps2/src/nitro/nitro_tex.c`): converted once per cache miss and uploaded by REF DMA
(no per-frame conversion; 0 misses per frame in steady state).  The key is the texture's VRAM
offset, size, format, colour-0 flag and palette, plus only the mirror bits that change the stored
texels.  Texture VRAM also changes outside `GX_LoadTex` (VRAM-transfer tasks, LCDC writes), so an
entry's source texels + palette are checksummed and re-verified on its first use each frame
(every word up to 4 KiB, 256 sampled words above); a change re-converts it (`stale` in the perf
line).  GS space is allocated by the texture's exact block footprint (`gs_footprint`: PSMT8/PSMT4
images are block-swizzled, so a small texture's last block can lie far past w x h x bpp): sizing
by bytes let a CLUT placed after it overwrite texels (the dashed title logo).

**DS VRAM model** (`nitro_gx.c`, `nitro_mi.c`), three rules the cutscenes depend on:
* a bank's place in a view is fixed when the view is set; taking a bank away (to LCDC, to the
  other engine) leaves the others where they are - texture slot 3 stays slot 3;
* `GX_SetBankForLCDC` takes the banks out of their other views (VRAMCNT holds one use per bank);
  `GX_DisableBankFor*` switches banks off without clearing them, as the SDK's `disableBankForX_`;
* CPU fills/copies into VRAM (MI) only land in banks mapped CPU-visibly (LCDC, BG, OBJ).  Scene
  setups clear the whole LCDC window (`MIi_CpuClearFast(0, 0x06800000, 0xa4000)`); on the DS that
  cannot touch banks mapped as texture memory.  Letting it do so zeroed the character textures
  the cutscenes then drew (black eyes on Roxas, black close-ups once the cache checked contents).

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

As built (`ps2/src/gfx/ds2d.c`):
* text BGs: one pass over the 33x25 visible entries, tiles bucketed by palette, coordinates from
  per-row/column tables, sprites in REGLIST mode (2 qwords each);
* CLUTs: DS BGR555 = GS PSMCT16; 256 colours as a 16x16 CSM1 CLUT, **16-colour palettes each in
  their own 64-word block** (CSA cannot select a palette of a larger CLUT for 4-bit textures:
  TEX0.CLD loads the 16 entries *at CBP* - using CSA drew every 4bpp tile/sprite with palette 0);
* affine BGs (8-bit maps), extended tiled affine BGs (16-bit maps), bitmap BGs, affine OBJs
  (double-size boxes, clipped exactly with SCISSOR) - drawn as forward-mapped textured
  triangles; windows WIN0/WIN1 (screen cut into rectangles, one SCISSOR pass per rectangle and
  layer, effect bit per region); BLDCNT alpha (exact when EVA+EVB = 16), darken, **brighten**
  (a second, brightened CLUT set); master brightness;
* display capture (engine A, DISPCAPCNT): VRAM display of the bank being captured draws the
  capture source and feeds back the previous capture (a 256x192 PSMCT16 GS image, blended with
  FIX alpha EVB) - the cutscenes' blur / cross-fades;
* extended palettes (DISPCNT bits 30/31; BG slots 0-3, BG0/1 slot select BGxCNT bit 13; OBJ):
  each palette in use is converted into a frame-lifetime buffer and loaded as a 16x16 CLUT right
  before its TEX0 - the opening monologue and many cutscene backgrounds are ext-palette BGs;
* 3D layer over 2D: translucent 3D pixels blend onto the 2D layers below only where BG0 is a
  BLDCNT first target; otherwise they replace them (coverage blending, `nitro_ge.c`), as the
  title logo needs;
* bitmap BGs: a non-wrapping bitmap is drawn as its own quad (transparent outside, so the movie
  frames sit 16 lines down), rows past the mapped VRAM are skipped; the screen-base of a bitmap
  BG is in 16 KiB units (`G2_GetBGxScrPtr`, `nitro_gx.c`);
* bitmap OBJs (mode 3): 2D (128/256 wide) and 1D layouts, direct colour; the rows a pass needs
  are uploaded once (the movie player shows every other frame as a 4x3 grid of 64x64 bitmap OBJs).
* OBJ atlases cover the whole 128 KiB OBJ area (1D mapping with 128/256-byte units reaches past
  64 KiB - the camp menu's buttons): 128 tiles per column, 128x1024 PSMT8 / 256x1024 PSMT4;
* window cells are cut with the GS fill rule (ceil of the scaled edges), so no row between two
  cells is left to the backdrop at non-integer scales;
* L3 cycles the layouts: vertical (default), horizontal, top screen only, bottom screen only.
* sprite UVs are clamped to the 14-bit UV range (1023.9375): the far edge of an atlas column's last
  tile row (v = 1024) wrapped to 0 and drew the column backwards (boxes on highlighted menu rows).
Output: 640x448 (PAL 640x512) 16-bit colour + 16-bit Z, interlaced FIELD mode, ordered dither with
non-negative offsets (DS 2D colours stay exact).  Every frame is presented, also in pause mode 1
(cutscene pause): a frame without geometry commands redraws the last 3D frame (nitro_ge.c), as the
DS keeps displaying its last rendered 3D image.
Not yet: OBJ window, mosaic, OBJ semi-transparency (bitmap OBJ alpha is drawn opaque), 3D layer
scroll.  Each logs once when used.

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
interface (the ARM9 side of NNS SND stays; `PXI_SendWordByFifo` hands its command lists to our
driver instead of the ARM7):
* `snd_seq.c` - the SSEQ sequencer (16 players x 16 tracks, the full command set incl. variables,
  random/conditional prefixes, ties, portamento), SBNK instrument lookup (drum sets, key splits),
  SWAR wave lookup;
* `snd_chan.c` - 16 channels (PCM8/PCM16/IMA-ADPCM with loop state, PSG duty, noise LFSR), the
  ADSR envelope in 1/10 dB, sweep, LFO, the SDK's channel allocation order and type masks, and the
  mixer (48 kHz stereo, the driver updated at the DS rate of 191.97 Hz between mix slices);
* `snd_driver.c` - the command front end (sequence start/stop/pause/skip, player and track
  parameters, variables, direct PCM channels with timers - the stream players and the movie
  audio -, locks, alarms, master volume) and the shared work area the ARM9 polls (player status,
  channel status, tick counters, variables).
Output (`ps2/src/platform/ps2_audio.c`): an EE thread (priority 27) renders one 512-frame half
(2048 bytes, 10.7 ms) per SIF RPC to **`khsnd.irx`** (`ps2/iop/khsnd/`), our IOP module. This
gives the lower-priority music-stream refill thread a scheduling point between halves while keeping
the SIF rate at about 94 calls/s. Each half uses the SPU2 block-input layout
`[256 L][256 R][256 L][256 R]`. Libsd loop transfer
feeds SPU2 core 1 from a two-half buffer refilled from an 8-block ring; the RPC blocks while the
ring is full, so the EE thread is paced by the SPU2 clock. The stream worker runs at priority 26,
one level above the mixer, so refills can preempt a render immediately. Stream-refill alarms
advance from the
number of samples actually rendered, not VBlank time, so rendering ahead cannot overwrite a
stream block the mixer is still reading. (PS2BUILD's `audsrv.irx` is a voice-only build without
PCM streaming.) The per-channel
status lines of the `[snd]` log are `KH_PS2_DEBUG` only.

**Soft reset** (`OS_ResetSystem`: the pause menu's Title Screen) re-executes the ELF
(`kh_platform_restart`, LoadExecPS2 of argv[0]); the boot path rebuilds everything.

### 3.16 Save data

`CARD_*` backup (EEPROM/flash) from the save scenes (ov009, ov000, ov008/ov025 run a card
thread). PS2: save slots become files in a memory-card directory (`BASLUS-KHDAYS/` style name,
`icon.sys` + icon). The DS backup API is emulated at the *API* level only (read/write/verify by
offset) over a 64 KiB image. Bring-up storage (`ps2/src/nitro/nitro_card.c`) is two slot files on
the boot device, `khdays_a.sav` / `khdays_b.sav`, each the image plus a {magic, version, sequence,
CRC-32} trailer: a save overwrites the slot that does not hold the newest valid image, so an
interrupted write leaves the previous save intact (its CRC fails at load and the other slot is
used). No rename is needed -- PCSX2's `host:` is a legacy ioman device without one, and the old
temp-file + remove + rename scheme was not atomic anyway. A raw 64 KiB `khdays.sav` from older
builds is still read.

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

`.mods` MobiClip files (46, 108 MiB; 256x160 at 14.985 fps, IMA-ADPCM audio at 22050 or
32728 Hz) played by ov024's player, used by ov012 (opening) and the movie scenes.  **The game's
own player runs** (decompiled C/C++: container, streaming reads, frame alarm, ADPCM audio into
two looping PCM16 channels, subtitles, display setup).  Only its ARM parts are replaced, by
overrides in `ps2/overrides/overlays/`:
* `Ov024_MobiClip_GetDecoderCodeCached` returns `MobiClip_DecodeFrameCore`, the portable C++ frame
  decoder (`libs/mobiclip/video/portable`, library `kh_mobiclip`; `docs/MOBICLIP_DECODER.md`)
  instead of copying the ARM payload to ITCM - same state, same planes;
* `Ov024_MobiClip_BlitRows` - the YCoCg->RGB555 conversion (with its checkerboard dither) in C;
* `Ov024_TickStreamSlots` - the original plus the VBlank the DS would take while the opening scene
  spins on it: while a slot can decode ahead, a passed VBlank is serviced without waiting
  (`kh_nitro_poll_vblank`); with nothing to decode the frame is presented and the next VBlank
  waited for.
The player double-buffers on the display: frames alternate between the BG3 direct-colour bitmap
and a 4x3 grid of 64x64 bitmap OBJs, flipped by DISPCNT in its VBlank task (which checks VCOUNT:
it reads line 192 while the VBlank handler runs).  The FastAudio and transform audio codecs and
the deblocking filter stay ARM-only: KH Days' movies use none of them.  Verified in PCSX2: the
opening and the first field movie (818.mods plays its 108.0 s in 108 s), Start skips.

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
| `long long`/`double` members are 8-aligned on MIPS, 4-aligned by mwcc | game structs (ov002's gauge context grew 0x188 → 0x1a0 and overran its DS-sized heap block) | game code is built with `-fpack-struct=4`; SDK structs the PS2 layer shares with it use a 4-aligned 64-bit typedef (`OSTick`) |
| EE gcc raises arrays and structs to 8-byte alignment | data tables traversed across objects | R13/R17 cap them at their type's own alignment |
| DS varargs: a variadic function hands `&last_named` on as a pointer to the argument block | `Text_FormatUtf16`, projectile Fire functions, ... | R14 rebuilds the block from `va_arg` |
| zero-initialised globals repeated in several files become COMMON and lose their DS placement | NitroSDK SND command manager, OS thread block, ... | gen_link.py pins COMMON symbols at their DS `.bss` offsets |
| ARM assembly in libs (`asm_stubs`: MSL runtime, SDK hand-written routines) | libs only | re-implemented in C at the API boundary; no ARM interpreter |
| mwcc array assignment | Ov252_PlaceBodyParts | override |
| DS Protect (ov028) | anti-tamper | excluded; its callers overridden |

The DS sources are never edited for the PS2. PS2-only changes live in `ps2/overrides/`
(hand-written replacements of whole functions) and `ps2/gen/` (mechanically prepared copies
generated by `ps2/tools/`), selected by the build generator.

---

## 5. How the DS sources become an EE executable

The DS sources are never edited for the PS2.  Four mechanisms, all under `ps2/`, adapt them.

### 5.1 Source preparation (`ps2/tools/prep_sources.py`)

Mechanical, rule-based rewrites into `ps2/gen/src/<same path>`, compiled instead of the original.
Every rule is listed in the tool; the important ones:

| Rule | What | Why |
|---|---|---|
| R1 | `asm { clz }` → `kh_clz()` (clz(0) = 32) | ARM inline asm |
| R2 | `register T v asm("rN")` → `register T v` | ARM register pinning |
| R3 | per-caller ABI variants (`ps2/config/abi_variants.txt`, from `proto_audit.py`) | n32: 64-bit values live in one register, so callers reading `_s32_div_f` as `long long` vs `int`, or passing a 64-bit argument as two words, need different symbols |
| R4 | mwcc array assignment → `__builtin_memcpy` | mwcc extension |
| R5 | literal DS addresses in an address context → PS2 state blocks | see 5.2 |
| R6 | packed-pointer decode base `0x01ff8000` → `KH_DS_PACKED_PTR_BASE` | handles keep 24 bits of a heap pointer; valid because the ELF + game arena stay below 16 MiB |
| R7 | drop an `extern` prototype of a function the file defines `static` | mwcc accepts it, gcc does not |
| R8 | stores/reads of geometry-engine registers → `kh_ge_port_write1/read` | 3D commands must reach the geometry front end in order, word by word |
| R13 | 4-byte alignment for named address-coupled arrays (ov008 camp-menu `.bss`) | EE gcc's 8-byte array alignment opened gaps |
| R14 | variadic functions taking `&last_named`: the DS argument block is rebuilt from `va_arg` | on the EE variadic arguments are in registers |
| R15 | literal ITCM addresses of data embedded in ITCM code → PS2 definitions (`nitro_itcm_data.c`) | 0x01ff8000-0x01ffffff is the top of EE RAM (G3D texture-matrix builder tables) |
| R16 | R8 for pointer variables aimed at geometry registers (`p->field = v`, `*p = v`) | NODEMIX drove MTX_MODE/STORE/RESTORE this way; skinned models used wrong matrices |
| R17 | every definition in a DS pure-data file gets its type's own alignment | the link places these objects at their DS-relative addresses (5.5) |

`build/ps2/hw_semantic.txt` lists every source touching a register with side effects;
`ps2/config/semantic_ok.txt` records the reviewed ones that are correct as prepared.

### 5.2 DS hardware state vs. side effects

Two kinds of DS hardware access exist in the game and libraries, and they are treated differently:

* **Display state** -- 2D engine A/B control, blending, windows, master brightness, BG offsets,
  palette RAM, OAM, VRAM, the DTCM/shared-RAM words (boot parameters, ARM7 key word, VBlank
  counter).  Writing them has no side effect beyond "this is how the screen should look" (or "this
  is the value the OS keeps here").  They live in PS2 memory blocks (`kh_ds_io`, `kh_ds_pal`,
  `kh_ds_oam`, `kh_ds_vram`, `kh_ds_hiram`, `ps2/src/nitro/nitro_hw.c`) that the GS compositor
  reads each frame; the PS2 input/VBlank code keeps the mirrored words current.  No DS address is
  ever dereferenced.
* **Side-effect registers** -- the geometry engine, the divider/sqrt unit, DMA, timers, IPC, IRQ.
  These are never mere state: geometry registers go through R8 to the geometry front end; the
  divider/sqrt unit is modelled exactly (`nitro_cp.c`: operands are state, results are computed
  when read through the SDK getters or when the control register is polled); DMA/timers/IPC/IRQ
  users are replaced functions.

`include/nitro/hw.h` defines register macros with raw addresses, which would bypass R5; it is
shadowed by the generated `ps2/include/nitro/hw.h` (`ps2/tools/gen_hw_shadow.py`), which remaps
state registers, makes the divider control registers computing accessors, and turns every
geometry-register macro R8 did not convert into a compile error.

### 5.3 Overrides (`ps2/overrides/`)

Hand-written PS2 versions of individual game functions, named like the function; the original
file is left out of the build.  Used where the change is not mechanical: the main loop
(`kh_game_main`), `Pad_Sample` (DualShock 2 → DS key word), `Fx_Tween` (divider).

### 5.4 Libraries

The decomp's library C is compiled where portable (`ps2/config/modules.txt`); files that still
reach DS hardware after preparation are excluded automatically (`ps2/tools/libscan.py`,
`build/ps2/lib_autoexcluded.txt`).  The NitroSDK surface the game needs is implemented in
`ps2/src/nitro/` and wins over library code at link time (it is scanned first in the
`--start-group`).  What is still undefined after that is stubbed by `ps2/tools/gen_link.py`, each
stub logging on first call; `build/ps2/link_report.md` lists them by SDK module -- the list of
dependencies still blocking a complete port.

### 5.5 Link glue (`ps2/tools/gen_link.py`)

* `.bss` (and data labels) exist only as addresses in `config/arm9/**/symbols.txt`: every module's
  `.bss` is laid out in `ps2/gen/layout/<module>_bss.S` at its original relative offsets (weak, so
  C definitions win), one section per module;
* `OVERLAY_<n>_ID` absolute symbols (the SDK's overlay-id idiom) and per-overlay `.bss` ranges for
  `FS_LoadOverlay`;
* aliases for data labels that point inside a larger C object;
* COMMON symbols (R10's merged zero-initialised globals) get a strong label at their DS `.bss`
  offset; names absent from `symbols.txt` take it from the `khdays: shared-bss` block that
  declares them (mwcc order: reverse declaration order, last declaration appended);
* the objects of every DS pure-data file (`delinks.txt` entry without `.text`) are placed at their
  addresses relative to the module's first data file: game code reaches neighbouring objects
  across file boundaries (ov002 writes a caption's screen base through `data_ov002_0207ebf4`
  into `data_ov002_0207ec00`). An object larger than its DS range stops the link
  ("cannot move location counter backwards"); `ps2/tools/check_data_layout.py` lists all of them
  at once.

### 5.7 Bring-up diagnostics

* `KH_PS2_DEBUG` is off by default. Set it for a diagnostic build with
  `KH_PS2_DEBUG=1 ./build-ps2.sh`; this also enables `KH_PS2_PROFILE`, INFO/debug logging,
  periodic performance reports, heap walks after every allocation/free and VBlank, and periodic
  object-state logs. Warnings, errors and panic diagnostics remain in normal builds.
* `nitro_heapdbg.c` wraps `NNS_FndAllocFromExpHeapEx` / `NNS_FndFreeToExpHeap` (`--wrap`):
  frees of blocks a heap does not hold are logged and dropped (always); with `KH_PS2_DEBUG`,
  after every operation and every VBlank both game heaps are checked (signatures, links,
  overlaps) and the first corruption is reported with the operations around it; heap usage is
  logged every 1200 VBlanks and at `OS_Terminate`.
* `OS_Terminate` logs its caller. A jump to address 0 shows in PCSX2's log as `TLB Miss, pc=0x0`:
  break on 0x0 in the debugger and read `$ra`.
* `G2_GetBG*Ptr` for an unmapped VRAM window returns a scratch sink (logged) instead of NULL: a
  NULL base turned tilemap writes into stores to EE kernel RAM (below 0x80000).
* Threads: the DS game thread sleeps once per frame in `OS_WaitVBlankIntr`, which is what lets
  the lower-priority DS threads (file loader, sound stream, save) run. `kh_vblank_wait` therefore
  drains every pending VBlank signal before `WaitSema`: the EE kernel does not clamp a semaphore
  to `max_count`, and with frames longer than one VBlank a single `PollSema` left the game thread
  never blocking (the loader starved, field loads queued forever). Symptom: a DS thread `READY`
  in `pcsx2_get_threads` that never runs.
* `OSi_VBlankInterruptHandler` is the game's own VBlank handler despite its SDK-style name
  (master brightness commit + the `RegisterNamedTask` VBlank task list, e.g. ov002's BGUIVBFUNC
  that drives caption fades); `ps2/overrides/nitro/` compiles it, and `run_vblank` calls it at
  thread level. A stub there froze the first field tutorial (caption fade tween never sampled).
* `ps2/tools/win/`: `run_pcsx2.sh`, `pcsx2_shot.ps1` (screenshot), `pcsx2_key.ps1` (key presses),
  `pcsx2_newgame.sh` / `pcsx2_to_story.sh` (drive the title menu to a New Game / the first story
  scene, waiting on log markers), `pcsx2_to_field.sh` / `pcsx2_to_mission.sh ... start` (the two
  benchmark scenes), `pcsx2_sample.py` (statistical EE profiler over the DebugServer: random
  pauses + stack walks -> self/inclusive time per function), `ds2d_dump.py` / `ds2d_bgdump.py`
  (live DS 2D registers, OAM, BG screen entries and palettes over PINE), `pine_poke.py` (write
  a variable by symbol, e.g. `kh_ge_dbg_skip` to hide triangles by texture format / pass).

### 5.8 Performance

The profiler (`ps2/src/platform/ps2_prof.c`) times exclusive zones with the cp0 Count register
and logs every 120 frames in a `KH_PS2_DEBUG=1` diagnostic build. It is compiled out of the normal
build:

```
[perf] 33.4ms (max 33.4) 30.0fps | game 1.9 ge 14.4 r3d 1.5 tex 0.0 r2d 2.7 gsw 0.4 vbl 12.3 ...
[perf] vtx 6040 poly 1839 cull 175 clip 10 off 32 tri 2752 draw 4 texbind 61 miss 0 stale 0 |
       upload 102 (210 KB) clut 6 gif 195 KB spr 4768 shadow 5
```

game = game update without the zones inside it; ge = geometry front end; r3d = 3D packet
building (VU1 headers, texture binds); r2d = 2D compositor; gsw = waiting for the GS; vbl =
sleeping until VBlank (headroom); tri = triangles to the GS; gif = frame packet size.

Measured in PCSX2 (EE cycle counts; real hardware will differ, chiefly in cache behaviour):

| scene | baseline (session start) | now |
|---|---|---|
| Mission 00 (Sandlot, enemies) | 83.4 ms, 12 fps: GE 41.2, 2D 19.0, debug 1.8 | 33.4 ms (30 fps cap), GE 6.6, r3d 0.7, 2D 2.8, 22 ms idle |
| Grey Area field | 33.4 ms with ~4 ms to spare: GE 17.0, r3d 7.8, 2D 3.0 | 33.4 ms, GE 14.4, r3d 1.5, 2D 2.7, 12 ms idle |

The main loop (`kh_game_main`) caps the game at 30 fps in its 60 Hz mode, as on the DS: one
VBlank wait at the top of the loop and the pacing wait for the next one.  What changed:
* EE vertex transform: the 64-bit fixed-point products went through soft-float `__floatdisf` per
  coordinate -> VU0 macro-mode transform; DS matrix products use MULT/MADD (exact DS precision);
* 3D GIF packets built per vertex on the EE -> VU1 microprogram, vertices DMA'd by reference;
* clipping: guard band (x/y) + divide-free culling; packed-command decoder without copies;
* 2D: text BGs were walked 16 times (once per palette) with two divisions per entry -> one pass;
* `fps %.1f` on screen pulled soft double printf every frame -> integer formatting;
* VBlanks that pass while a frame is computed are delivered when the game next reads its VBlank
  counter (`VBlank_GetCount`), as the DS IRQ would - pacing no longer adds a VBlank to frames
  that overran one;
* heap checking after every allocation/free and VBlank is a `KH_PS2_DEBUG` feature.

### 5.6 The ARM7

The ARM7 program is not ported.  Its services are replaced at the ARM9/ARM7 boundary: the sound
command lists the ARM9 sound library sends over PXI are executed by the PS2 sound driver
(`ps2/src/audio/snd_driver.c`), which completes them and maintains the shared work area the ARM9
reads; touch, RTC, power, card and wireless are replaced at the SDK API level (3.22).

## 6. Repository layout for the port

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

## 7. Bring-up plan and status

| # | Milestone | Status |
|---|---|---|
| 1 | Architecture audit, this document | done (living document) |
| 2 | PS2BUILD project, minimal ELF, serial/stdout log | done |
| 3 | Platform layer: memory diagnostics, timing/VBlank, VFS + boot path, pad, GS clear | done |
| 4 | Link the game: generated BSS/data layouts, overlay IDs, SDK replacement stubs that log | done |
| 5 | `main()` runs: heaps, task system, scene 1 constructed | done |
| 6 | 2D compositor: title screen BGs/OAM | done: text/affine/extended/bitmap BGs, affine OBJ, windows, blending, brightness, display capture; OBJ window, mosaic, ext palettes pending |
| 7 | Geometry front end + GS back end: first G3D model | done (skinned characters, textured rooms) |
| 8 | Title/menus, new game | done (PCSX2): title, Story Mode, difficulty, opening movies (virtual clock) |
| 9 | Field scene, player + camera, combat | in progress (PCSX2): story scenes, Grey Area field with HUD, tutorial captions, player movement and camera; NPC talk, missions and combat next |
| 10 | Audio (SDAT sequencer + SPU2) | pending |
| 11 | Memory-card saves | pending |
| 12 | MobiClip playback | pending |
| 13 | VU1 path, pre-converted models, profiling-driven optimisation | VU1 projection/GIF path, VU0 transform, profiler done (5.8); pre-converted models pending |

## 8. Open questions

* Duplicate player/enemy overlays: link once and alias, or keep separate (only RAM at stake).
* Polygon-ID based translucency and edge marking on GS.
* Game Over (ov027) puts the 3D layer in front of its 2D background (BG0 priority 0); fixed by
  drawing the 3D layer at BG0's priority, not yet seen on screen (needs a death in play).

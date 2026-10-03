/* Profiler zones and counters (no structs: also included by game-side PS2 code that is built
 * with the game's packing flags). */
#ifndef KH_PROF_H
#define KH_PROF_H

#include <stdint.h>

/* Build switches (set by build-ps2.sh / gen_ps2yaml.py from the environment):
 *   KH_PS2_DEBUG    0 (default): diagnostic logs and expensive bring-up checks
 *   KH_PS2_PROFILE  KH_PS2_DEBUG (default): zone timing, counters and periodic perf logs */
#ifndef KH_PS2_DEBUG
#define KH_PS2_DEBUG 0
#endif
#ifndef KH_PS2_PROFILE
#define KH_PS2_PROFILE KH_PS2_DEBUG
#endif

/* Zones are exclusive: entering a zone pauses the enclosing one, so the zone times of a frame add
 * up to the frame time (anything outside every zone is "misc").  Zones below KH_PROF_ASYNC_FIRST
 * belong to the game thread; the async ones (other threads) are timed independently. */
typedef enum KhProfZone {
    KH_PROF_FRAME = 0,   /* (no zone: misc) */
    KH_PROF_GAME,        /* game update (objects, callbacks, task queue) minus the zones below */
    KH_PROF_GE,          /* geometry front end: DS command decode, transform, lighting, clipping */
    KH_PROF_R3D,         /* 3D triangle list -> GIF packet */
    KH_PROF_TEX,         /* texture cache misses: conversion + upload setup */
    KH_PROF_R2D,         /* 2D compositor */
    KH_PROF_GS_WAIT,     /* waiting for the GS / GIF DMA */
    KH_PROF_VBLANK,      /* sleeping until VBlank */
    KH_PROF_VBTASK,      /* VBlank work: game VBlank handler, alarms, input */
    KH_PROF_SOUND,       /* SoundMgr_Update + the PS2 sound driver */
    KH_PROF_DEBUG,       /* bring-up checks (heap walks, logs) */
    KH_PROF_ASYNC_FIRST,
    KH_PROF_LOAD = KH_PROF_ASYNC_FIRST,   /* file reads (loader thread) */
    KH_PROF_AUDIO,
    KH_PROF_COUNT
} KhProfZone;

typedef enum KhProfCounter {
    KH_PC_VERTS,         /* vertices into the geometry front end */
    KH_PC_POLYS,         /* polygons assembled */
    KH_PC_CULLED,        /* polygons removed by face culling */
    KH_PC_CLIPPED,       /* polygons that crossed a clip plane */
    KH_PC_OFFSCREEN,     /* polygons entirely outside the view volume */
    KH_PC_TRIS,          /* triangles sent to the GS (3D) */
    KH_PC_DRAWS,         /* GS state batches (3D) */
    KH_PC_TEXBIND,       /* texture changes (3D) */
    KH_PC_TEXMISS,       /* texture cache misses (3D) */
    KH_PC_TEXSTALE,      /* cached textures whose source changed (3D) */
    KH_PC_UPLOADS,       /* IMAGE transfers */
    KH_PC_UPLOAD_BYTES,
    KH_PC_GIF_BYTES,     /* frame packet size */
    KH_PC_SPRITES,       /* 2D sprites */
    KH_PC_CLUTS,         /* CLUT uploads */
    KH_PC_SHADOW,        /* shadow-volume triangles (mask + shadow) */
    KH_PC_COUNT
} KhProfCounter;

void kh_prof_begin(KhProfZone z);
void kh_prof_end(KhProfZone z);

#endif

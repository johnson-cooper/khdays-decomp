/* PS2: mechanically prepared copy of src/overlays/enemies/ov253_enemy_leechgrave/data/ov253_rodata_020d4964.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov253 .rodata tables 0x020d4964-0x020d49a8 (split by alignment so the objects tile their run). */

#include "nitro/types.h"

const u16 data_ov253_020d4964[13] __attribute__((aligned(__alignof__(u16)))) = {
    0, 1541, 0, 517, 0, 1029, 0, 261,
    0, 1797, 0, 1285, 0,
};

/* read by Ov253_GrabTick (not yet decompiled) */
const u16 data_ov253_020d497e[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by release wait tick: the +0x18 speed follows twice the frame step (020cf66c): const PosMsg data_ov253_020d498c; */
const u16 data_ov253_020d498c[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by roar tick: the +0x18 speed follows twice the frame step (30 / 15); (020cfa44): const PosMsg data_ov253_020d499a; */
const u16 data_ov253_020d499a[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

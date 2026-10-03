/* PS2: mechanically prepared copy of src/overlays/enemies/ov265_enemy_aerial_master/data/ov265_tables_020d368c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov265 .rodata tables, 0x020d368c-0x020d36b0.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov265_ChaseTick (020d1164): const Vec3 data_ov265_020d368c; */

#include "nitro/types.h"

const u8 data_ov265_020d368c[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 144, 255, 255, 0, 0, 0, 0,
};

/* read by decide which way to sidestep, then hand off to Ov265_UpdateAimPoint. (020d16fc): VecFx32 data_ov265_020d3698; */
const u8 data_ov265_020d3698[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 176, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by AI hover-dive step (byte-identical in ov232 / (020d1990): const Vec3 data_ov265_020d36a4; */
const int data_ov265_020d36a4[3] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 2048,
};

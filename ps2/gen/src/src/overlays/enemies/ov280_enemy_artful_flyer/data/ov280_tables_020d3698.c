/* PS2: mechanically prepared copy of src/overlays/enemies/ov280_enemy_artful_flyer/data/ov280_tables_020d3698.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov280 .rodata tables, 0x020d3698-0x020d36bc.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov280_ChaseTick (020d1170): const Vec3 data_ov280_020d3698; */

#include "nitro/types.h"

const u8 data_ov280_020d3698[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 144, 255, 255, 0, 0, 0, 0,
};

/* read by decide which way to sidestep, then hand off to Ov280_UpdateAimPoint. (020d1708): VecFx32 data_ov280_020d36a4; */
const u8 data_ov280_020d36a4[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 176, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by AI hover-dive step (byte-identical in ov232 / (020d199c): const Vec3 data_ov280_020d36b0; */
const int data_ov280_020d36b0[3] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 2048,
};

/* PS2: mechanically prepared copy of src/overlays/enemies/ov133_enemy_minute_bomb_3/data/ov133_tables_020d49b8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov133 .rodata tables, 0x020d49b8-0x020d4a2c.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov133_nodeConstructor (020d1a24): struct v5 data_ov133_020d49b8; */

#include "nitro/types.h"

const int data_ov133_020d49b8[5] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4, 5, 6,
};

/* read by Ov133_stateTransformAimVec (020d3338): unsigned short data_ov133_020d49cc[];
 *   Ov133_stateStartThrow (020d3980): unsigned short data_ov133_020d49cc[]; */
const u8 data_ov133_020d49cc[8] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 4,
};

/* read by Ov133_throwRelease_tick (020d3d14): const struct Msg data_ov133_020d49d4; */
const u16 data_ov133_020d49d4[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov133_HomingDash_Tick (020d3404): const struct Msg data_ov133_020d49e2; */
const u16 data_ov133_020d49e2[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov133_throwRelease_tick (020d3d14): const struct Msg data_ov133_020d49f0; */
const u16 data_ov133_020d49f0[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov133_ThrowRelease_Enter (020d3b48): const struct Msg data_ov133_020d49fe; */
const u16 data_ov133_020d49fe[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by Ov133_collectObjectsInSphereRec (020d20ec): const struct tbl8 data_ov133_020d4a0c; */
const u8 data_ov133_020d4a0c[32] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 255, 255, 255, 255, 255, 255, 1, 0, 0, 0, 255, 255, 255, 255,
    255, 255, 255, 255, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
};

/* PS2: mechanically prepared copy of src/overlays/enemies/ov132_enemy_minute_bomb_2/data/ov132_tables_020d0d78.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov132 .rodata tables, 0x020d0d78-0x020d0dec.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov132_nodeConstructor (020cdde4): struct v5 data_ov132_020d0d78; */

#include "nitro/types.h"

const int data_ov132_020d0d78[5] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4, 5, 6,
};

/* read by Ov132_stateTransformAimVec (020cf6f8): unsigned short data_ov132_020d0d8c[];
 *   Ov132_stateStartThrow (020cfd40): unsigned short data_ov132_020d0d8c[]; */
const u8 data_ov132_020d0d8c[8] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 4,
};

/* read by Ov132_throwRelease_tick (020d00d4): const struct Msg data_ov132_020d0d94; */
const u16 data_ov132_020d0d94[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov132_HomingDash_Tick (020cf7c4): const struct Msg data_ov132_020d0da2; */
const u16 data_ov132_020d0da2[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov132_throwRelease_tick (020d00d4): const struct Msg data_ov132_020d0db0; */
const u16 data_ov132_020d0db0[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov132_ThrowRelease_Enter (020cff08): const struct Msg data_ov132_020d0dbe; */
const u16 data_ov132_020d0dbe[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by Ov132_collectObjectsInSphereRec (020ce4ac): const struct tbl8 data_ov132_020d0dcc; */
const u8 data_ov132_020d0dcc[32] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 255, 255, 255, 255, 255, 255, 1, 0, 0, 0, 255, 255, 255, 255,
    255, 255, 255, 255, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
};

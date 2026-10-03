/* PS2: mechanically prepared copy of src/overlays/players/ov062_player_xemnas_2/data/ov062_tables_020b7f6c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov062 .rodata tables, 0x020b7f6c-0x020b7fd4.
 *
 * 4 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Seeds the two request rows of the node (+0x12c, stride 0x240) from the local tables: (020b73d8): const IntTable4 data_ov062_020b7f6c; */

#include "nitro/types.h"

const u8 data_ov062_020b7f6c[16] __attribute__((aligned(__alignof__(u8)))) = {
    68, 1, 0, 0, 188, 254, 255, 255, 68, 1, 0, 0, 188, 254, 255, 255,
};

/* read by Seeds the two request rows of the node (+0x12c, stride 0x240) from the local tables: (020b73d8): const IntTable4 data_ov062_020b7f7c; */
const u8 data_ov062_020b7f7c[16] __attribute__((aligned(__alignof__(u8)))) = {
    170, 42, 0, 0, 170, 42, 0, 0, 170, 10, 0, 0, 170, 10, 0, 0,
};

/* read by Computes a world position for the mission enemy's request slot: picks the local offset of (020b76f0): const OffsetPair data_ov062_020b7f8c; */
const u8 data_ov062_020b7f8c[24] __attribute__((aligned(__alignof__(u8)))) = {
    236, 17, 0, 0, 143, 34, 0, 0, 143, 254, 255, 255, 20, 238, 255, 255,
    143, 34, 0, 0, 143, 254, 255, 255,
};

/* read by Seeds the two request rows of the node (+0x12c, stride 0x240) from the local tables: (020b73d8): const VecTable4 data_ov062_020b7fa4; */
const u8 data_ov062_020b7fa4[48] __attribute__((aligned(__alignof__(u8)))) = {
    102, 6, 0, 0, 205, 252, 255, 255, 154, 9, 0, 0, 154, 249, 255, 255,
    205, 252, 255, 255, 154, 9, 0, 0, 205, 4, 0, 0, 0, 0, 0, 0,
    0, 8, 0, 0, 51, 251, 255, 255, 0, 0, 0, 0, 0, 8, 0, 0,
};

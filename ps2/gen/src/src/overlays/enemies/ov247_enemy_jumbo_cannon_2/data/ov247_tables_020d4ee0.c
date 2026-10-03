/* PS2: mechanically prepared copy of src/overlays/enemies/ov247_enemy_jumbo_cannon_2/data/ov247_tables_020d4ee0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov247 .rodata tables, 0x020d4ee0-0x020d4f34.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov246 enemy (and its byte-identical twin): installs the handlers (+8 ti (020d1a24): struct Ov246Kinds data_ov247_020d4ee0; */

#include "nitro/types.h"

const int data_ov247_020d4ee0[8] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4, 5, 6, 7, 8, 9,
};

/* read by c634 handler: arm the second phase of the move. (020d31f8): unsigned short data_ov247_020d4f00[];
 *   Attack wait tick of the ov246 enemy: the +0x3c timer accumulates the owner's rate. In phas (020d32c4): unsigned short data_ov247_020d4f00[];
 *   set the follow-up delay, and if the counter allows, commit the move. (020d34d0): unsigned short data_ov247_020d4f00[];
 *   Aimed attack wait tick of the ov246 enemy: the +0x18 direction takes the owner's +0xa0 bas (020d35ac): unsigned short data_ov247_020d4f00[]; */
const u8 data_ov247_020d4f00[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 5, 4, 0, 0, 5, 2, 0, 0, 5, 0,
    0, 0, 5, 3,
};

/* read by Construction of the ov246 enemy's item: installs the handlers (+8 020ceaf0, +0xc 020ceb20, (020d3970): const struct Ov246Pose data_ov247_020d4f14; */
const int data_ov247_020d4f14[1] __attribute__((aligned(__alignof__(int)))) = {
    7,
};

/* read by Send the ov246 enemy's position message: the overlay's 14-byte template (data_ov247_020d4f (020d43a4): const PosMsg data_ov247_020d4f18; */
const u8 data_ov247_020d4f18[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Entry of the ov246 enemy's ground drop: a ray from the +0x10 anchor straight down (-2.0, (020d4c70): Vec3 data_ov247_020d4f28; */
const u8 data_ov247_020d4f28[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 248, 255, 255, 0, 0, 0, 0,
};

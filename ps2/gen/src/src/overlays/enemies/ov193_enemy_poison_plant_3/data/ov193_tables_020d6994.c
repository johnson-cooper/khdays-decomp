/* PS2: mechanically prepared copy of src/overlays/enemies/ov193_enemy_poison_plant_3/data/ov193_tables_020d6994.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov193 .rodata tables, 0x020d6994-0x020d6a04.
 *
 * 9 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Big constructor of the ov193 enemy (x3: ov193/192/193): raises bit 8 of the +0 flags, inst (020d3844): struct Ov193Vec3 data_ov193_020d6994; */

#include "nitro/types.h"

const int data_ov193_020d6994[3] __attribute__((aligned(__alignof__(int)))) = {
    0, 1024, 0,
};

/* read by Big constructor of the ov193 enemy (x3: ov193/192/193): raises bit 8 of the +0 flags, inst (020d3844): struct Ov193Kinds data_ov193_020d69a0; */
const int data_ov193_020d69a0[4] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 6,
};

/* read by Begin the ov191 enemy's "pick target" state (x3: ov191/192/193): plays pose 3, clears the (020d4e14): Pair data_ov193_020d69b0; */
const u8 data_ov193_020d69b0[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0,
};

/* read by * Box sweep of the ov191 enemy (x3: ov191/192/193). Builds an ov107 box query at *aim with (020d437c): const PosMsg data_ov193_020d69b4; */
const u8 data_ov193_020d69b4[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Constructor of the ov191 enemy (x3: ov191/192/193): installs the handlers (+8 tick, +0xc (020d5a8c): const int data_ov193_020d69c4[2]; */
const int data_ov193_020d69c4[2] __attribute__((aligned(__alignof__(int)))) = {
    5, 7,
};

/* read by * Flight tick of the ov191 projectile state (x3: ov191/192/193). Each frame it rebuilds th (020d6158): const PosMsg data_ov193_020d69cc; */
const u16 data_ov193_020d69cc[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by * Flight tick of the ov191 projectile state (x3: ov191/192/193). Each frame it rebuilds th (020d6158): const PosMsg data_ov193_020d69da; */
const u16 data_ov193_020d69da[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by * Flight tick of the ov191 projectile state (x3: ov191/192/193). Each frame it rebuilds th (020d6158): const PosMsg data_ov193_020d69e8; */
const u16 data_ov193_020d69e8[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Begin the ov191 enemy's shockwave (x3: ov191/192/193): copies the canned {id 0, kind 5, (020d66a0): PosMsg data_ov193_020d69f6; */
const u16 data_ov193_020d69f6[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

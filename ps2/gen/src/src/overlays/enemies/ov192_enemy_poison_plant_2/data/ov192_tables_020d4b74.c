/* PS2: mechanically prepared copy of src/overlays/enemies/ov192_enemy_poison_plant_2/data/ov192_tables_020d4b74.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov192 .rodata tables, 0x020d4b74-0x020d4be4.
 *
 * 9 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Big constructor of the ov192 enemy (x3: ov192/192/193): raises bit 8 of the +0 flags, inst (020d1a24): struct Ov192Vec3 data_ov192_020d4b74; */

#include "nitro/types.h"

const int data_ov192_020d4b74[3] __attribute__((aligned(__alignof__(int)))) = {
    0, 1024, 0,
};

/* read by Big constructor of the ov192 enemy (x3: ov192/192/193): raises bit 8 of the +0 flags, inst (020d1a24): struct Ov192Kinds data_ov192_020d4b80; */
const int data_ov192_020d4b80[4] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 6,
};

/* read by Begin the ov191 enemy's "pick target" state (x3: ov191/192/193): plays pose 3, clears the (020d2ff4): Pair data_ov192_020d4b90; */
const u8 data_ov192_020d4b90[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0,
};

/* read by * Box sweep of the ov191 enemy (x3: ov191/192/193). Builds an ov107 box query at *aim with (020d255c): const PosMsg data_ov192_020d4b94; */
const u8 data_ov192_020d4b94[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Constructor of the ov191 enemy (x3: ov191/192/193): installs the handlers (+8 tick, +0xc (020d3c6c): const int data_ov192_020d4ba4[2]; */
const int data_ov192_020d4ba4[2] __attribute__((aligned(__alignof__(int)))) = {
    5, 7,
};

/* read by * Flight tick of the ov191 projectile state (x3: ov191/192/193). Each frame it rebuilds th (020d4338): const PosMsg data_ov192_020d4bac; */
const u16 data_ov192_020d4bac[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by * Flight tick of the ov191 projectile state (x3: ov191/192/193). Each frame it rebuilds th (020d4338): const PosMsg data_ov192_020d4bba; */
const u16 data_ov192_020d4bba[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by * Flight tick of the ov191 projectile state (x3: ov191/192/193). Each frame it rebuilds th (020d4338): const PosMsg data_ov192_020d4bc8; */
const u16 data_ov192_020d4bc8[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Begin the ov191 enemy's shockwave (x3: ov191/192/193): copies the canned {id 0, kind 5, (020d4880): PosMsg data_ov192_020d4bd6; */
const u16 data_ov192_020d4bd6[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

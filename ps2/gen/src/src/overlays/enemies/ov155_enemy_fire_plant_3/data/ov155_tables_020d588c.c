/* PS2: mechanically prepared copy of src/overlays/enemies/ov155_enemy_fire_plant_3/data/ov155_tables_020d588c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov155 .rodata tables, 0x020d588c-0x020d58e4.
 *
 * 8 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Big constructor of the ov153 enemy (x3: ov153/154/155): raises bit 8 of the +0 flags, inst (020d3844): const int data_ov155_020d588c[2]; */

#include "nitro/types.h"

const int data_ov155_020d588c[2] __attribute__((aligned(__alignof__(int)))) = {
    1, 2,
};

/* read by Big constructor of the ov153 enemy (x3: ov153/154/155): raises bit 8 of the +0 flags, inst (020d3844): struct Ov153Vec3 data_ov155_020d5894; */
const int data_ov155_020d5894[3] __attribute__((aligned(__alignof__(int)))) = {
    0, 2560, 0,
};

/* read by Constructor of the ov153 enemy (x3: ov153/154/155): installs the handlers (+8 tick, +0xc (020d3e08): const int data_ov155_020d58a0[2]; */
const int data_ov155_020d58a0[2] __attribute__((aligned(__alignof__(int)))) = {
    3, 5,
};

/* read by * Homing flight tick of the ov153 projectile state (x3: ov153/154/155). The actors inside  (020d4578): const PosMsg data_ov155_020d58a8; */
const u16 data_ov155_020d58a8[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by * Homing flight tick of the ov153 projectile state (x3: ov153/154/155). The actors inside  (020d4578): const PosMsg data_ov155_020d58b6; */
const u16 data_ov155_020d58b6[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by broadcast this node's position. When the caller's flag word has both (020d4114): struct Msg data_ov155_020d58c4; */
const u16 data_ov155_020d58c4[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by * Homing flight tick of the ov153 projectile state (x3: ov153/154/155). The actors inside  (020d4578): const PosMsg data_ov155_020d58d2; */
const u16 data_ov155_020d58d2[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov155_stateAnimFlagCallback (020d5694): struct pair data_ov155_020d58e0; */
const u8 data_ov155_020d58e0[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0,
};

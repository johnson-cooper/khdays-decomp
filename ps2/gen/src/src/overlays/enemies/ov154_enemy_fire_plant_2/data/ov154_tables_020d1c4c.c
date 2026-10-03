/* PS2: mechanically prepared copy of src/overlays/enemies/ov154_enemy_fire_plant_2/data/ov154_tables_020d1c4c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov154 .rodata tables, 0x020d1c4c-0x020d1ca4.
 *
 * 8 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Big constructor of the ov153 enemy (x3: ov153/154/155): raises bit 8 of the +0 flags, inst (020cfc04): const int data_ov154_020d1c4c[2]; */

#include "nitro/types.h"

const int data_ov154_020d1c4c[2] __attribute__((aligned(__alignof__(int)))) = {
    1, 2,
};

/* read by Big constructor of the ov153 enemy (x3: ov153/154/155): raises bit 8 of the +0 flags, inst (020cfc04): struct Ov153Vec3 data_ov154_020d1c54; */
const int data_ov154_020d1c54[3] __attribute__((aligned(__alignof__(int)))) = {
    0, 2560, 0,
};

/* read by Constructor of the ov153 enemy (x3: ov153/154/155): installs the handlers (+8 tick, +0xc (020d01c8): const int data_ov154_020d1c60[2]; */
const int data_ov154_020d1c60[2] __attribute__((aligned(__alignof__(int)))) = {
    3, 5,
};

/* read by * Homing flight tick of the ov153 projectile state (x3: ov153/154/155). The actors inside  (020d0938): const PosMsg data_ov154_020d1c68; */
const u16 data_ov154_020d1c68[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by * Homing flight tick of the ov153 projectile state (x3: ov153/154/155). The actors inside  (020d0938): const PosMsg data_ov154_020d1c76; */
const u16 data_ov154_020d1c76[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by broadcast this node's position. When the caller's flag word has both (020d04d4): struct Msg data_ov154_020d1c84; */
const u16 data_ov154_020d1c84[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by * Homing flight tick of the ov153 projectile state (x3: ov153/154/155). The actors inside  (020d0938): const PosMsg data_ov154_020d1c92; */
const u16 data_ov154_020d1c92[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov154_stateAnimFlagCallback (020d1a54): struct pair data_ov154_020d1ca0; */
const u8 data_ov154_020d1ca0[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0,
};

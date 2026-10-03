/* PS2: mechanically prepared copy of src/overlays/enemies/ov152_enemy_bubble_beat_2/data/ov152_tables_020d6460.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov152 .rodata tables, 0x020d6460-0x020d64c0.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov151 enemy (and its byte-identical twin): installs the handlers (+8 ti (020d3844): const struct PoolIds data_ov152_020d6460; */

#include "nitro/types.h"

const int data_ov152_020d6460[5] __attribute__((aligned(__alignof__(int)))) = {
    10, 2, 3, 4, 5,
};

/* read by Constructor of the ov151 enemy's summoned pet (and its byte-identical twin): installs the (020d4248): const int data_ov152_020d6474[2]; */
const int data_ov152_020d6474[2] __attribute__((aligned(__alignof__(int)))) = {
    7, 8,
};

/* read by Shot tick of the ov151 enemy (and its byte-identical twin): the +0x38 clock accumulates th (020d4c2c): const PosMsg data_ov152_020d647c; */
const u16 data_ov152_020d647c[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Shot launch of the ov151 enemy (and its byte-identical twin): clears the +0x38 hit count,  (020d49ec): const PosMsg data_ov152_020d648a; */
const u16 data_ov152_020d648a[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by broadcast this node's position. When the caller's flag word has both (020d4594): struct Msg data_ov152_020d6498; */
const u16 data_ov152_020d6498[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Shot tick of the ov151 enemy (and its byte-identical twin): the +0x38 clock accumulates th (020d4c2c): const PosMsg data_ov152_020d64a6; */
const u16 data_ov152_020d64a6[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Projectile action enter step (Ghidra: Ov149_ProjectileAction_Enter). (020d5d70): u16 data_ov152_020d64b4[4];
 *   Ov152_AiTrackOffsetThenNotify (020d5e24): unsigned short data_ov152_020d64b4[];
 *   Ov152_stateAnimCallbackEffect (020d61b0): unsigned short data_ov152_020d64b4[]; */
const u8 data_ov152_020d64b4[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 1, 0, 0, 5, 2,
};

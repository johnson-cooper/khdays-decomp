/* PS2: mechanically prepared copy of src/overlays/enemies/ov151_enemy_bubble_beat/data/ov151_tables_020cebe0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov151 .rodata tables, 0x020cebe0-0x020cec40.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov151 enemy (and its byte-identical twin): installs the handlers (+8 ti (020cbfc4): const struct PoolIds data_ov151_020cebe0; */

#include "nitro/types.h"

const int data_ov151_020cebe0[5] __attribute__((aligned(__alignof__(int)))) = {
    10, 2, 3, 4, 5,
};

/* read by Constructor of the ov151 enemy's summoned pet (and its byte-identical twin): installs the (020cc9c8): const int data_ov151_020cebf4[2]; */
const int data_ov151_020cebf4[2] __attribute__((aligned(__alignof__(int)))) = {
    7, 8,
};

/* read by Shot tick of the ov151 enemy (and its byte-identical twin): the +0x38 clock accumulates th (020cd3ac): const PosMsg data_ov151_020cebfc; */
const u16 data_ov151_020cebfc[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Shot launch of the ov151 enemy (and its byte-identical twin): clears the +0x38 hit count,  (020cd16c): const PosMsg data_ov151_020cec0a; */
const u16 data_ov151_020cec0a[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by broadcast this node's position. When the caller's flag word has both (020ccd14): struct Msg data_ov151_020cec18; */
const u16 data_ov151_020cec18[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Shot tick of the ov151 enemy (and its byte-identical twin): the +0x38 clock accumulates th (020cd3ac): const PosMsg data_ov151_020cec26; */
const u16 data_ov151_020cec26[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Projectile action enter step (Ghidra: Ov149_ProjectileAction_Enter). (020ce4f0): u16 data_ov151_020cec34[4];
 *   Ov151_AiTrackOffsetThenNotify (020ce5a4): unsigned short data_ov151_020cec34[];
 *   Ov151_stateAnimCallbackEffect (020ce930): unsigned short data_ov151_020cec34[]; */
const u8 data_ov151_020cec34[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 1, 0, 0, 5, 2,
};

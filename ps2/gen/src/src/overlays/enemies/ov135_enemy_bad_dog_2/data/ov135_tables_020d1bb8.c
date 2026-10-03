/* PS2: mechanically prepared copy of src/overlays/enemies/ov135_enemy_bad_dog_2/data/ov135_tables_020d1bb8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov135 .rodata tables, 0x020d1bb8-0x020d1c00.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov135_Construct (020cfc04): struct v5 data_ov135_020d1bb8; */

#include "nitro/types.h"

const int data_ov135_020d1bb8[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by Ov135_stateAnimPairCallback (020d19d0): unsigned short data_ov135_020d1bc4[]; */
const u8 data_ov135_020d1bc4[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0,
};

/* read by * Ov135_BurstAttackTick -- burst attack tick of the ov134 enemy (and its byte-identical twin (020d116c): struct Ov134AreaMsg data_ov135_020d1bd4; */
const u16 data_ov135_020d1bd4[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by * Ov135_BurstAttackTick -- burst attack tick of the ov134 enemy (and its byte-identical twin (020d116c): struct Ov134AreaMsg data_ov135_020d1be2; */
const u16 data_ov135_020d1be2[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Attack tick of the ov134 enemy (x3: ov134/135/136). Counts the +0x30 timer and fires react (020d1694): const struct Msg data_ov135_020d1bf0; */
const u8 data_ov135_020d1bf0[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

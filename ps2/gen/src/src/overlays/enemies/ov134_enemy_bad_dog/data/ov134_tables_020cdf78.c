/* PS2: mechanically prepared copy of src/overlays/enemies/ov134_enemy_bad_dog/data/ov134_tables_020cdf78.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov134 .rodata tables, 0x020cdf78-0x020cdfc0.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov134_Construct (020cbfc4): struct v5 data_ov134_020cdf78; */

#include "nitro/types.h"

const int data_ov134_020cdf78[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by Ov134_stateAnimPairCallback (020cdd90): unsigned short data_ov134_020cdf84[]; */
const u8 data_ov134_020cdf84[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0,
};

/* read by * Ov134_BurstAttackTick -- burst attack tick of the ov134 enemy (and its byte-identical twin (020cd52c): struct Ov134AreaMsg data_ov134_020cdf94; */
const u16 data_ov134_020cdf94[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by * Ov134_BurstAttackTick -- burst attack tick of the ov134 enemy (and its byte-identical twin (020cd52c): struct Ov134AreaMsg data_ov134_020cdfa2; */
const u16 data_ov134_020cdfa2[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Attack tick of the ov134 enemy (x3: ov134/135/136). Counts the +0x30 timer and fires react (020cda54): const struct Msg data_ov134_020cdfb0; */
const u8 data_ov134_020cdfb0[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

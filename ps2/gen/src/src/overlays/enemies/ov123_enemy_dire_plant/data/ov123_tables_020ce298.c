/* PS2: mechanically prepared copy of src/overlays/enemies/ov123_enemy_dire_plant/data/ov123_tables_020ce298.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov123 .rodata tables, 0x020ce298-0x020ce2f0.
 *
 * 8 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov123 enemy (and its byte-identical twin): raises bit 8 of the +0 flags (020cbfc4): const int data_ov123_020ce298[2]; */

#include "nitro/types.h"

const int data_ov123_020ce298[2] __attribute__((aligned(__alignof__(int)))) = {
    1, 2,
};

/* read by Constructor of the ov123 enemy (and its byte-identical twin): raises bit 8 of the +0 flags (020cbfc4): struct Ov153Vec3 data_ov123_020ce2a0; */
const int data_ov123_020ce2a0[3] __attribute__((aligned(__alignof__(int)))) = {
    0, 1024, 0,
};

/* read by Ov123_ResetAndInvokeMethodThenAdvance (020cce74): short data_ov123_020ce2ac; */
const u8 data_ov123_020ce2ac[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0,
};

/* read by Constructor of the ov156 enemy's held item (and its byte-identical twin): installs the (020cd500): const int data_ov123_020ce2b0[2]; */
const int data_ov123_020ce2b0[2] __attribute__((aligned(__alignof__(int)))) = {
    3, 5,
};

/* read by Shot flight tick of the ov123 enemy (and its byte-identical twin). Without a +0x24 target  (020cdbf8): const PosMsg data_ov123_020ce2b8; */
const u16 data_ov123_020ce2b8[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Shot flight tick of the ov123 enemy (and its byte-identical twin). Without a +0x24 target  (020cdbf8): const PosMsg data_ov123_020ce2c6; */
const u16 data_ov123_020ce2c6[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Shot flight tick of the ov123 enemy (and its byte-identical twin). Without a +0x24 target  (020cdbf8): const PosMsg data_ov123_020ce2d4; */
const u16 data_ov123_020ce2d4[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Shot flight tick of the ov123 enemy (and its byte-identical twin). Without a +0x24 target  (020cdbf8): const PosMsg data_ov123_020ce2e2; */
const u16 data_ov123_020ce2e2[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

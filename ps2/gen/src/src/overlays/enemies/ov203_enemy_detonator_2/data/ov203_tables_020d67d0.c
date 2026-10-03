/* PS2: mechanically prepared copy of src/overlays/enemies/ov203_enemy_detonator_2/data/ov203_tables_020d67d0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov203 .rodata tables, 0x020d67d0-0x020d6844.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by ported from a matched sibling family (same shape, constants and offsets adjusted). (020d3848): struct v5 data_ov203_020d67d0; */

#include "nitro/types.h"

const int data_ov203_020d67d0[5] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4, 5, 6,
};

/* read by Ov203_stateTransformAimVec (020d5118): unsigned short data_ov203_020d67e4[];
 *   recoil entry of the ov202 enemy (x2), variant of the matched ov132 sibling (+0x60 bit 6 cl (020d5760): unsigned short data_ov203_020d67e4[]; */
const u8 data_ov203_020d67e4[8] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 4,
};

/* read by ported from a matched sibling family (same shape, constants and offsets adjusted). (020d5b00): const struct Msg data_ov203_020d67ec; */
const u16 data_ov203_020d67ec[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by ported from a matched sibling family (same shape, constants and offsets adjusted). (020d51e4): const struct Msg data_ov203_020d67fa; */
const u16 data_ov203_020d67fa[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by ported from a matched sibling family (same shape, constants and offsets adjusted). (020d5b00): const struct Msg data_ov203_020d6808; */
const u16 data_ov203_020d6808[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by ported from a matched sibling family (same shape, constants and offsets adjusted). (020d5934): const struct Msg data_ov203_020d6816; */
const u16 data_ov203_020d6816[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by Ov203_collectObjectsInSphereRec (020d3ee8): const struct tbl8 data_ov203_020d6824; */
const u8 data_ov203_020d6824[32] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 255, 255, 255, 255, 255, 255, 1, 0, 0, 0, 255, 255, 255, 255,
    255, 255, 255, 255, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
};

/* PS2: mechanically prepared copy of src/overlays/enemies/ov269_enemy_bully_dog/data/ov269_tables_020d4a28.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov269 .rodata tables, 0x020d4a28-0x020d4ab0.
 *
 * 9 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov269 enemy (and its byte-identical twins): installs the handlers (+8 t (020d1a24): struct Ov269Vec3 data_ov269_020d4a28; */

#include "nitro/types.h"

const int data_ov269_020d4a28[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by Ov269_stateAnimPairCallback (020d3ca4): unsigned short data_ov269_020d4a34[]; */
const u8 data_ov269_020d4a34[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 2,
};

/* read by Charge tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fir (020d3280): const Vec3 data_ov269_020d4a38; */
const u8 data_ov269_020d4a38[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 0, 0,
};

/* read by Combo tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fire (020d44dc): const Vec3 data_ov269_020d4a44; */
const u8 data_ov269_020d4a44[24] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 22, 0, 0,
};

/* read by Charge tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fir (020d3280): const struct Msg14 data_ov269_020d4a5c; */
const u16 data_ov269_020d4a5c[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Swing tick of the ov269 enemy (and its byte-identical twin): the +0x14 turn step is zeroed (020d3930): const PosMsg data_ov269_020d4a6a; */
const u16 data_ov269_020d4a6a[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Combo tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fire (020d44dc): const struct Msg14 data_ov269_020d4a78; */
const u8 data_ov269_020d4a78[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Combo tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fire (020d44dc): const struct Msg20 data_ov269_020d4a88; */
const u8 data_ov269_020d4a88[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

/* read by Charge tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fir (020d3280): const struct Msg20 data_ov269_020d4a9c; */
const u8 data_ov269_020d4a9c[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

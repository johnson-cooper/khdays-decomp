/* PS2: mechanically prepared copy of src/overlays/enemies/ov270_enemy_bully_dog_2/data/ov270_tables_020d6848.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov270 .rodata tables, 0x020d6848-0x020d68d0.
 *
 * 9 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov269 enemy (and its byte-identical twins): installs the handlers (+8 t (020d3844): struct Ov269Vec3 data_ov270_020d6848; */

#include "nitro/types.h"

const int data_ov270_020d6848[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by Ov270_stateAnimPairCallback (020d5ac4): unsigned short data_ov270_020d6854[]; */
const u8 data_ov270_020d6854[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 2,
};

/* read by Charge tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fir (020d50a0): const Vec3 data_ov270_020d6858; */
const u8 data_ov270_020d6858[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 0, 0,
};

/* read by Combo tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fire (020d62fc): const Vec3 data_ov270_020d6864; */
const u8 data_ov270_020d6864[24] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 22, 0, 0,
};

/* read by Charge tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fir (020d50a0): const struct Msg14 data_ov270_020d687c; */
const u16 data_ov270_020d687c[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Swing tick of the ov269 enemy (and its byte-identical twin): the +0x14 turn step is zeroed (020d5750): const PosMsg data_ov270_020d688a; */
const u16 data_ov270_020d688a[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Combo tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fire (020d62fc): const struct Msg14 data_ov270_020d6898; */
const u8 data_ov270_020d6898[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Combo tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fire (020d62fc): const struct Msg20 data_ov270_020d68a8; */
const u8 data_ov270_020d68a8[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

/* read by Charge tick of the ov269 enemy (and its byte-identical twin). Until the +0x53 one-shot fir (020d50a0): const struct Msg20 data_ov270_020d68bc; */
const u8 data_ov270_020d68bc[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

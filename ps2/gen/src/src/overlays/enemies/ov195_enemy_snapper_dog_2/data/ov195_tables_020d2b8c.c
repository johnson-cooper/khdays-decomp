/* PS2: mechanically prepared copy of src/overlays/enemies/ov195_enemy_snapper_dog_2/data/ov195_tables_020d2b8c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov195 .rodata tables, 0x020d2b8c-0x020d2c08.
 *
 * 8 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov194 enemy (x3: ov194/195/196): installs the handlers (+8 tick, +0xc (020cfc04): struct Ov194Vec3 data_ov195_020d2b8c; */

#include "nitro/types.h"

const int data_ov195_020d2b8c[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by Ov195_stateAnimPairCallback (020d1e08): unsigned short data_ov195_020d2b98[]; */
const u8 data_ov195_020d2b98[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0,
};

/* read by Combo tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020d2640): const Vec3 data_ov195_020d2ba8; */
const u8 data_ov195_020d2ba8[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0,
};

/* read by Swing tick of the ov194 enemy (and its byte-identical twin): the +0x14 turn step is zeroed (020d1a94): const PosMsg data_ov195_020d2bb4; */
const u16 data_ov195_020d2bb4[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Combo tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020d2640): const struct Msg14 data_ov195_020d2bc2; */
const u16 data_ov195_020d2bc2[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Charge tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020d1464): const struct Msg14 data_ov195_020d2bd0; */
const u8 data_ov195_020d2bd0[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Charge tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020d1464): const struct Msg20 data_ov195_020d2be0; */
const u8 data_ov195_020d2be0[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

/* read by Combo tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020d2640): const struct Msg20 data_ov195_020d2bf4; */
const u8 data_ov195_020d2bf4[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

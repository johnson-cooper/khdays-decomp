/* PS2: mechanically prepared copy of src/overlays/enemies/ov194_enemy_snapper_dog/data/ov194_tables_020cef4c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov194 .rodata tables, 0x020cef4c-0x020cefc8.
 *
 * 8 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov194 enemy (x3: ov194/195/196): installs the handlers (+8 tick, +0xc (020cbfc4): struct Ov194Vec3 data_ov194_020cef4c; */

#include "nitro/types.h"

const int data_ov194_020cef4c[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by Ov194_stateAnimPairCallback (020ce1c8): unsigned short data_ov194_020cef58[]; */
const u8 data_ov194_020cef58[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0,
};

/* read by Combo tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020cea00): const Vec3 data_ov194_020cef68; */
const u8 data_ov194_020cef68[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 40, 0, 0,
};

/* read by Swing tick of the ov194 enemy (and its byte-identical twin): the +0x14 turn step is zeroed (020cde54): const PosMsg data_ov194_020cef74; */
const u16 data_ov194_020cef74[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Combo tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020cea00): const struct Msg14 data_ov194_020cef82; */
const u16 data_ov194_020cef82[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Charge tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020cd824): const struct Msg14 data_ov194_020cef90; */
const u8 data_ov194_020cef90[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Charge tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020cd824): const struct Msg20 data_ov194_020cefa0; */
const u8 data_ov194_020cefa0[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

/* read by Combo tick of the ov194 enemy (x3: ov194/195/196). Until the +0x53 one-shot fires, (020cea00): const struct Msg20 data_ov194_020cefb4; */
const u8 data_ov194_020cefb4[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

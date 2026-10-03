/* PS2: mechanically prepared copy of src/overlays/enemies/ov143_enemy_loudmouth_3/data/ov143_tables_020d623c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov143 .rodata tables, 0x020d623c-0x020d62a8.
 *
 * 8 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by ov141 actor initializer: install the callback table, seed the camera pose, (020d3844): struct Ov143KindTable data_ov143_020d623c; */

#include "nitro/types.h"

const int data_ov143_020d623c[5] __attribute__((aligned(__alignof__(int)))) = {
    9, 2, 3, 4, 5,
};

/* read by Projectile action enter step (Ghidra: Ov149_ProjectileAction_Enter). (020d4b30): u16 data_ov143_020d6250[4];
 *   Ov143_stTransformProjectilePose (020d4be4): unsigned short data_ov143_020d6250[];
 *   Ov143_stateAnimCallbackEffect (020d4f6c): unsigned short data_ov143_020d6250[]; */
const u8 data_ov143_020d6250[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 1, 0, 0, 5, 2,
};

/* read by Constructor for the actor's sub-object: install the five entry points, set the (020d5260): const struct Ov143ChildIds data_ov143_020d625c; */
const int data_ov143_020d625c[2] __attribute__((aligned(__alignof__(int)))) = {
    7, 8,
};

/* read by One frame of the ground slam (Ghidra: Ov143_StepSlamProbe). (020d57fc): struct Vec3 data_ov143_020d6264; */
const u8 data_ov143_020d6264[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0,
};

/* read by The slam's follow-through (Ghidra: Ov143_StepSlamStrike). (020d5ab0): struct Ov143Cmd data_ov143_020d6270; */
const u16 data_ov143_020d6270[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by One frame of the ground slam (Ghidra: Ov143_StepSlamProbe). (020d57fc): struct Ov143Cmd data_ov143_020d627e; */
const u16 data_ov143_020d627e[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by The slam's follow-through (Ghidra: Ov143_StepSlamStrike). (020d5ab0): struct Ov143Cmd data_ov143_020d628c; */
const u16 data_ov143_020d628c[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by The slam's follow-through (Ghidra: Ov143_StepSlamStrike). (020d5ab0): struct Ov143Cmd data_ov143_020d629a; */
const u16 data_ov143_020d629a[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

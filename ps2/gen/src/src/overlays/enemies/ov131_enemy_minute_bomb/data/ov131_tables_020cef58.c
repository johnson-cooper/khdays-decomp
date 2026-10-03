/* PS2: mechanically prepared copy of src/overlays/enemies/ov131_enemy_minute_bomb/data/ov131_tables_020cef58.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov131 .rodata tables, 0x020cef58-0x020cefcc.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov131_nodeConstructor (020cbfc4): struct v5 data_ov131_020cef58; */

#include "nitro/types.h"

const int data_ov131_020cef58[5] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4, 5, 6,
};

/* read by Ov131_stateTransformAimVec (020cd8d8): unsigned short data_ov131_020cef6c[];
 *   Ov131_stateStartThrow (020cdf20): unsigned short data_ov131_020cef6c[]; */
const u8 data_ov131_020cef6c[8] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 4,
};

/* read by Ov131_throwRelease_tick (020ce2b4): const struct Msg data_ov131_020cef74; */
const u16 data_ov131_020cef74[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov131_HomingDash_Tick (020cd9a4): const struct Msg data_ov131_020cef82; */
const u16 data_ov131_020cef82[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov131_throwRelease_tick (020ce2b4): const struct Msg data_ov131_020cef90; */
const u16 data_ov131_020cef90[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov131_ThrowRelease_Enter (020ce0e8): const struct Msg data_ov131_020cef9e; */
const u16 data_ov131_020cef9e[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by Ov131_collectObjectsInSphereRec (020cc68c): const struct tbl8 data_ov131_020cefac; */
const u8 data_ov131_020cefac[32] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 255, 255, 255, 255, 255, 255, 1, 0, 0, 0, 255, 255, 255, 255,
    255, 255, 255, 255, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
};

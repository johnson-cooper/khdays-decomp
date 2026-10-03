/* PS2: mechanically prepared copy of src/overlays/enemies/ov228_enemy_dual_blade/data/ov228_tables_020d2c2c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov228 .rodata tables, 0x020d2c2c-0x020d2cdc.
 *
 * 4 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov228_Construct (not yet decompiled) */

#include "nitro/types.h"

const int data_ov228_020d2c2c[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 29, 30, 31, 32, 33, 34, 37,
    38, 39,
};

/* read by Rebuild the +0x384 work list for the actor at *(+0x3a8): copy the const pose table to (020ce7fc): const struct Buf_020ce7fc data_ov228_020d2c54; */
const int data_ov228_020d2c54[27] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27,
};

/* read by Ov228_OnHit (020ce910): const struct ModeTable data_ov228_020d2cc0; */
const u8 data_ov228_020d2cc0[24] __attribute__((aligned(__alignof__(u8)))) = {
    0, 1, 2, 3, 0, 96, 0, 0, 184, 3, 0, 0, 248, 7, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0,
};

/* read by Ov228_CompanionSetup (not yet decompiled) */
const int data_ov228_020d2cd8[1] __attribute__((aligned(__alignof__(int)))) = {
    35,
};

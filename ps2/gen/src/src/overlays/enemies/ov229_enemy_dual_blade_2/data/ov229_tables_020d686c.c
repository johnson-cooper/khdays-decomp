/* PS2: mechanically prepared copy of src/overlays/enemies/ov229_enemy_dual_blade_2/data/ov229_tables_020d686c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov229 .rodata tables, 0x020d686c-0x020d691c.
 *
 * 4 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov229_Construct (not yet decompiled) */

#include "nitro/types.h"

const int data_ov229_020d686c[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 29, 30, 31, 32, 33, 34, 37,
    38, 39,
};

/* read by Rebuild the +0x384 work list for the actor at *(+0x3a8): copy the const pose table to (020d243c): const struct Buf_020ce7fc data_ov229_020d6894; */
const int data_ov229_020d6894[27] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27,
};

/* read by Hit handler of the ov228 enemy (x2 with ov229): ignored while the +0x21a stamina is spent. (020d2550): const struct ModeTable data_ov229_020d6900; */
const u8 data_ov229_020d6900[24] __attribute__((aligned(__alignof__(u8)))) = {
    0, 1, 2, 3, 0, 96, 0, 0, 184, 3, 0, 0, 248, 7, 0, 0,
    1, 0, 0, 0, 1, 0, 0, 0,
};

/* read by Ov229_CompanionSetup (not yet decompiled) */
const int data_ov229_020d6918[1] __attribute__((aligned(__alignof__(int)))) = {
    35,
};

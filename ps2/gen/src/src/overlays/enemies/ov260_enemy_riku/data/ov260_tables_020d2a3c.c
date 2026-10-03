/* PS2: mechanically prepared copy of src/overlays/enemies/ov260_enemy_riku/data/ov260_tables_020d2a3c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov260 .rodata tables, 0x020d2a3c-0x020d2a90.
 *
 * 4 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov260_Construct (not yet decompiled) */

#include "nitro/types.h"

const int data_ov260_020d2a3c[12] __attribute__((aligned(__alignof__(int)))) = {
    36, 37, 41, 45, 46, 47, 48, 50,
    51, 52, 53, 54,
};

/* read by Ov260_OnHit (not yet decompiled) */
const u8 data_ov260_020d2a6c[20] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 0, 48, 0, 0, 80, 5, 0, 0, 80, 5, 0, 0,
    0, 64, 0, 0,
};

/* read by Ov260_SubPartAConstruct (020d0a58): IdTable2 data_ov260_020d2a80; */
const int data_ov260_020d2a80[2] __attribute__((aligned(__alignof__(int)))) = {
    43, 44,
};

/* read by Ov260_SubPartBConstruct (020d1858): IdTable2 data_ov260_020d2a88; */
const int data_ov260_020d2a88[2] __attribute__((aligned(__alignof__(int)))) = {
    39, 40,
};

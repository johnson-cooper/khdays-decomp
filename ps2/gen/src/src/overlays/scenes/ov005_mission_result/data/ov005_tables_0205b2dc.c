/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/data/ov005_tables_0205b2dc.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov005 .rodata tables, 0x0205b2dc-0x0205b368.
 *
 * 8 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by For each variable-stride record, build a sprite via Ov005_AddElem (kind-mapped prior (0204cb00): unsigned char data_ov005_0205b2dc; */

#include "nitro/types.h"

const u8 data_ov005_0205b2dc[4] __attribute__((aligned(__alignof__(u8)))) = {
    8, 9, 10, 11,
};

/* read by For each variable-stride record, build a sprite via Ov005_AddElem (kind-mapped prior (0204cb00): unsigned char data_ov005_0205b2e0; */
const u8 data_ov005_0205b2e0[4] __attribute__((aligned(__alignof__(u8)))) = {
    24, 25, 26, 27,
};

/* read by Advance 02030788, then map the current 020315c0 slot to a priority table, storing its inde (0204eda0): int data_ov005_0205b2e4; */
const int data_ov005_0205b2e4[20] __attribute__((aligned(__alignof__(int)))) = {
    10, 0, 16, 11, 14, 12, 1, 4,
    5, 6, 7, 9, 13, 15, 17, 18,
    8, 2, 3, 10,
};

/* read by Ov005_InitializeResultResources (02052ba8): unsigned short data_ov005_0205b334[]; */
const u16 data_ov005_0205b334[3] __attribute__((aligned(__alignof__(u16)))) = {
    1, 10, 3,
};

/* read by Ov005_UpdateDialogVisibility (02054184): const short data_ov005_0205b34c[6],data_ov005_0205b33a[3]; */
const u16 data_ov005_0205b33a[3] __attribute__((aligned(__alignof__(u16)))) = {
    48, 49, 29,
};

/* read by Ov005_EnqueueDirtyTextBuffers (02053e44): const int data_ov005_0205b340[3]; */
const int data_ov005_0205b340[3] __attribute__((aligned(__alignof__(int)))) = {
    9, 10, 11,
};

/* read by Ov005_UpdateDialogVisibility (02054184): const short data_ov005_0205b34c[6],data_ov005_0205b33a[3]; */
const u8 data_ov005_0205b34c[12] __attribute__((aligned(__alignof__(u8)))) = {
    24, 0, 25, 0, 26, 0, 27, 0, 28, 0, 29, 0,
};

/* read by Ov005_InitializeResultSprites (02052ca4): EntryIds data_ov005_0205b358; */
const int data_ov005_0205b358[4] __attribute__((aligned(__alignof__(int)))) = {
    25, 26, 27, 28,
};

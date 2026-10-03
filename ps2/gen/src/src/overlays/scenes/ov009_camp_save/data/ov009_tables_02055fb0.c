/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/data/ov009_tables_02055fb0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov009 .rodata tables, 0x02055fb0-0x02056108.
 *
 * 10 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Advance 02030788, then map the current 020315c0 slot to a priority table, storing its inde (0204ee50): int data_ov009_02055fb0; */

#include "nitro/types.h"

const int data_ov009_02055fb0[20] __attribute__((aligned(__alignof__(int)))) = {
    10, 0, 16, 11, 14, 12, 1, 4,
    5, 6, 7, 9, 13, 15, 17, 18,
    8, 2, 3, 10,
};

/* read by For each variable-stride record, build a sprite via Ov009_AddElem (kind-mapped prior (02051c98): unsigned char data_ov009_02056000; */
const u8 data_ov009_02056000[4] __attribute__((aligned(__alignof__(u8)))) = {
    8, 9, 10, 11,
};

/* read by For each variable-stride record, build a sprite via Ov009_AddElem (kind-mapped prior (02051c98): unsigned char data_ov009_02056004; */
const u8 data_ov009_02056004[4] __attribute__((aligned(__alignof__(u8)))) = {
    24, 25, 26, 27,
};

/* read by Ov009_SetMenuEntriesVisible (02054070): const int data_ov009_02056008[2];
 *   Ov009_UpdateSlotSelectionTargets (020544b4): const int data_ov009_02056008[2]; */
const int data_ov009_02056008[2] __attribute__((aligned(__alignof__(int)))) = {
    20, 21,
};

/* read by Ov009_SetMenuEntriesVisible (02054070): const int data_ov009_02056010[4]; */
const int data_ov009_02056010[4] __attribute__((aligned(__alignof__(int)))) = {
    14, 16, 15, 61,
};

/* read by Ov009_SaveMenu_BuildLayout (020530c4): const Ov009ObjectConfig data_ov009_02056020; */
const int data_ov009_02056020[4] __attribute__((aligned(__alignof__(int)))) = {
    0, 1, 0, 0,
};

/* read by Ov009_SaveMenu_BuildTextSurfaces (02053404): const TileSurfaceCfg data_ov009_02056030; */
const int data_ov009_02056030[10] __attribute__((aligned(__alignof__(int)))) = {
    19, 0, 32, 4, 453, 15, 0, 5,
    0, 32,
};

/* read by Ov009_SaveMenu_BuildTextSurfaces (02053404): const TileSurfaceCfg data_ov009_02056058; */
const int data_ov009_02056058[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 14, 18, 2, 1, 15, 0, 5,
    0, 32,
};

/* read by Ov009_SaveMenu_BuildTextSurfaces (02053404): const TileSurfaceCfg data_ov009_02056080; */
const int data_ov009_02056080[10] __attribute__((aligned(__alignof__(int)))) = {
    4, 0, 32, 13, 37, 15, 0, 5,
    0, 32,
};

/* read by Ov009_SaveMenu_BuildLayout (020530c4): const int data_ov009_020560a8[3][8];
 *   Page-scroll tick for the ov009 menu: eases each of the three pages towards its (02054180): const int data_ov009_020560a8[3][8];
 *   Ov009_SaveMenu_UpdateNumbers (02054558): const int data_ov009_020560a8[3][8];
 *   Ov009_SaveMenu_RefreshRows (02054b58): const int data_ov009_020560a8[3][8]; */
const int data_ov009_020560a8[24] __attribute__((aligned(__alignof__(int)))) = {
    1, 31, 34, 37, 40, 50, 51, 52,
    2, 32, 35, 38, 41, 53, 54, 55,
    3, 33, 36, 39, 42, 56, 57, 58,
};

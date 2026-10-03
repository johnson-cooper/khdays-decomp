/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_tables_020903c4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .data tables, 0x020903c4-0x020903f0.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov008_020903c4: Ov008_DrawSavePage
 *   data_ov008_020903d0: Ov008_DrawSavePage
 *   data_ov008_020903dc: Ov008_DrawSavePage
 *   data_ov008_020903e0: Ov008_DrawSavePage
 *   data_ov008_020903e8: Ov008_DrawSavePage
 */

#include "nitro/types.h"

u8 data_ov008_020903c4[12] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 100, 0, 47, 0, 37, 0, 100, 0, 0, 0,
};

u8 data_ov008_020903d0[12] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 115, 0, 47, 0, 37, 0, 115, 0, 0, 0,
};

int data_ov008_020903dc[1] __attribute__((aligned(__alignof__(int)))) = {
    45,
};

u8 data_ov008_020903e0[8] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 100, 0, 0, 0, 0, 0,
};

u8 data_ov008_020903e8[8] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 115, 0, 0, 0, 0, 0,
};

/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_tables_02090ea0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .data tables, 0x02090ea0-0x02090ed4.
 *
 * 6 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov008_02090ea0: Ov008_BuildTable9AndDelegate
 *   data_ov008_02090ea8: Ov008_DrawNumber3Shadowed
 *   data_ov008_02090eb0: Ov008_Shop_DrawRow
 *   data_ov008_02090eb8: Ov008_Shop_DrawRow
 *   data_ov008_02090ecc: (no C reader yet)
 *   data_ov008_02090ed0: (no C reader yet)
 */

#include "nitro/types.h"

u8 data_ov008_02090ea0[8] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 100, 0, 0, 0, 0, 0,
};

u8 data_ov008_02090ea8[8] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 51, 0, 100, 0, 0, 0,
};

u8 data_ov008_02090eb0[8] __attribute__((aligned(__alignof__(u8)))) = {
    63, 0, 63, 0, 0, 0, 0, 0,
};

u8 data_ov008_02090eb8[20] __attribute__((aligned(__alignof__(u8)))) = {
    63, 0, 63, 0, 63, 0, 63, 0, 63, 0, 63, 0, 63, 0, 63, 0,
    0, 0, 0, 0,
};

int data_ov008_02090ecc[1] __attribute__((aligned(__alignof__(int)))) = {
    91,
};

int data_ov008_02090ed0[1] __attribute__((aligned(__alignof__(int)))) = {
    93,
};

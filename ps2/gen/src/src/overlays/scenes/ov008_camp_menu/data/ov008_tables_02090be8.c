/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_tables_02090be8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .data tables, 0x02090be8-0x02090cf8.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov008_02090be8: Ov008_UpdateMissionMemberSelectionInput
 *   data_ov008_02090c00: Ov008_UpdateMissionMemberSelectionInput
 *   data_ov008_02090c60: Ov008_UpdateMissionMemberSelectionInput
 */

#include "nitro/types.h"

int data_ov008_02090be8[6] __attribute__((aligned(__alignof__(int)))) = {
    13, 16, 15, 14, 17, 18,
};

u8 data_ov008_02090c00[96] __attribute__((aligned(__alignof__(u8)))) = {
    254, 255, 255, 255, 254, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0,
    1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0,
    5, 0, 0, 0, 6, 0, 0, 0, 255, 255, 255, 255, 254, 255, 255, 255,
    254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 7, 0, 0, 0,
    8, 0, 0, 0, 9, 0, 0, 0, 10, 0, 0, 0, 11, 0, 0, 0,
    12, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 254, 255, 255, 255,
};

int data_ov008_02090c60[38] __attribute__((aligned(__alignof__(int)))) = {
    0, 2, 0, 3, 0, 4, 0, 5,
    0, 6, 0, 7, 0, 8, 0, 9,
    0, 10, 1, 1, 1, 2, 1, 3,
    1, 4, 1, 5, 1, 6, 1, 7,
    1, 8, 1, 9, 1, 10,
};

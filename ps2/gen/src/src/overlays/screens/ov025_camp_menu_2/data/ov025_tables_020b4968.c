/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_tables_020b4968.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .rodata tables, 0x020b4968-0x020b49a0.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov025_020b4968: Ov025_ReportDetail_SetupEntries
 *   data_ov025_020b4978: Ov025_ReportDetail_SetupSurface
 */

#include "nitro/types.h"

const int data_ov025_020b4968[4] __attribute__((aligned(__alignof__(int)))) = {
    0, 2, 0, 0,
};

const int data_ov025_020b4978[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 32, 24, 0, 15, 0, 21,
    0, 32,
};

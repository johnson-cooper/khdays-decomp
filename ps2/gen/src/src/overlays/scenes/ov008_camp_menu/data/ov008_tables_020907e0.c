/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_tables_020907e0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .data tables, 0x020907e0-0x020907f0.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov008_020907e0: Ov008_DrawListEntryRow, Ov008_BuildTable11AndDelegate
 *   data_ov008_020907e8: Ov008_DrawListEntryRow, Ov008_BuildTableAndDelegate
 */

#include "nitro/types.h"

u8 data_ov008_020907e0[8] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 43, 0, 100, 0, 0, 0,
};

u8 data_ov008_020907e8[8] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 100, 0, 0, 0, 0, 0,
};

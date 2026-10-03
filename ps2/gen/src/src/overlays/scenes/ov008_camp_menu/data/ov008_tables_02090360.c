/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_tables_02090360.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .data tables, 0x02090360-0x02090380.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov008_02090360: Ov008_EnqueueRowPalette
 *   data_ov008_02090370: Ov008_EnqueueRowPalette
 */

#include "nitro/types.h"

u8 data_ov008_02090360[16] __attribute__((aligned(__alignof__(u8)))) = {
    196, 36, 196, 36, 196, 36, 196, 36, 109, 102, 232, 85, 70, 61, 228, 44,
};

u8 data_ov008_02090370[16] __attribute__((aligned(__alignof__(u8)))) = {
    75, 12, 105, 16, 104, 16, 135, 20, 152, 24, 117, 20, 114, 16, 77, 12,
};

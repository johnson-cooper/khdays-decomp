/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_tables_020b4d2c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .data tables, 0x020b4d2c-0x020b4d4c.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov025_020b4d2c: Ov025_EnqueueRowPalette
 *   data_ov025_020b4d3c: Ov025_EnqueueRowPalette
 */

#include "nitro/types.h"

u8 data_ov025_020b4d2c[16] __attribute__((aligned(__alignof__(u8)))) = {
    196, 36, 196, 36, 196, 36, 196, 36, 109, 102, 232, 85, 70, 61, 228, 44,
};

u8 data_ov025_020b4d3c[16] __attribute__((aligned(__alignof__(u8)))) = {
    75, 12, 105, 16, 104, 16, 135, 20, 152, 24, 117, 20, 114, 16, 77, 12,
};

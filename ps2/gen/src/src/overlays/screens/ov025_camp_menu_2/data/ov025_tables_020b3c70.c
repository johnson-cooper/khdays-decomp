/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_tables_020b3c70.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .rodata tables, 0x020b3c70-0x020b3cac.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov025_020b3c70: Ov025_InitGridMenuWidgets
 *   data_ov025_020b3c78: (no C reader yet)
 *   data_ov025_020b3c84: Ov025_BuildActionPage
 *   data_ov025_020b3c90: Ov025_InitializeSavePageLayout
 *   data_ov025_020b3c9c: Ov025_ShowGridPage
 */

#include "nitro/types.h"

const int data_ov025_020b3c70[2] __attribute__((aligned(__alignof__(int)))) = {
    62, 63,
};

const int data_ov025_020b3c78[3] __attribute__((aligned(__alignof__(int)))) = {
    16, 17, 18,
};

const int data_ov025_020b3c84[3] __attribute__((aligned(__alignof__(int)))) = {
    6, 5, 4,
};

const int data_ov025_020b3c90[3] __attribute__((aligned(__alignof__(int)))) = {
    4, 3, 0,
};

const int data_ov025_020b3c9c[4] __attribute__((aligned(__alignof__(int)))) = {
    72, 73, 70, 71,
};

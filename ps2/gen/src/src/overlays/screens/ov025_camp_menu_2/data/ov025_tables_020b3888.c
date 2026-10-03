/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_tables_020b3888.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .rodata tables, 0x020b3888-0x020b3924.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov025_020b3888: (no C reader yet)
 *   data_ov025_020b3894: (no C reader yet)
 *   data_ov025_020b38b8: (no C reader yet)
 *   data_ov025_020b38d4: Ov025_Hub_SetupTextSurfaces
 *   data_ov025_020b38fc: Ov025_Hub_SetupTextSurfaces
 */

#include "nitro/types.h"

const int data_ov025_020b3888[3] __attribute__((aligned(__alignof__(int)))) = {
    7, 2, 2,
};

const int data_ov025_020b3894[9] __attribute__((aligned(__alignof__(int)))) = {
    0, 1, 0, 0, 1, 2, 3, 4,
    5,
};

const int data_ov025_020b38b8[7] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 5, 3, 4, 6, 9,
};

const int data_ov025_020b38d4[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 32, 2, 0, 15, 0, 5,
    0, 32,
};

const int data_ov025_020b38fc[10] __attribute__((aligned(__alignof__(int)))) = {
    19, 0, 32, 4, 65, 15, 0, 5,
    0, 32,
};

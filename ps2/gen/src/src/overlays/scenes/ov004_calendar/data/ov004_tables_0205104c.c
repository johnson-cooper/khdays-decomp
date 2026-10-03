/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/data/ov004_tables_0205104c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov004 .rodata tables, 0x0205104c-0x020510a4.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Advance 02030788, then map the current 020315c0 slot to a priority table, storing its inde (0204d368): int data_ov004_0205104c; */

#include "nitro/types.h"

const int data_ov004_0205104c[20] __attribute__((aligned(__alignof__(int)))) = {
    10, 0, 16, 11, 14, 12, 1, 4,
    5, 6, 7, 9, 13, 15, 17, 18,
    8, 2, 3, 10,
};

/* read by Ov004_CreateMissionSelectScene (0204fa44): const Ov004SceneArgs data_ov004_0205109c; */
const int data_ov004_0205109c[2] __attribute__((aligned(__alignof__(int)))) = {
    1, 1,
};

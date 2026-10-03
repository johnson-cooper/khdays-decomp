/* PS2: mechanically prepared copy of src/overlays/enemies/ov215_enemy_morning_star_2/data/ov215_tables_020d2824.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov215 .rodata tables, 0x020d2824-0x020d2868.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov215_ConstructActor (020cfe3c): KindTable data_ov215_020d2824; */

#include "nitro/types.h"

const int data_ov215_020d2824[5] __attribute__((aligned(__alignof__(int)))) = {
    0, 11, 12, 13, 15,
};

/* read by Ov215_spawnFromTable (020d0620): int data_ov215_020d2838[]; */
const int data_ov215_020d2838[9] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9,
};

/* read by Applies one hit event to the actor (Ghidra: Ov215_ApplyHitEvent). (020d06f4): u8 data_ov215_020d285c[]; */
const u8 data_ov215_020d285c[12] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 248, 7, 0, 0, 102, 62, 0, 0,
};

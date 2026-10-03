/* PS2: mechanically prepared copy of src/overlays/enemies/ov214_enemy_morning_star/data/ov214_tables_020cebe4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov214 .rodata tables, 0x020cebe4-0x020cec28.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov214_ConstructActor (020cc1fc): KindTable data_ov214_020cebe4; */

#include "nitro/types.h"

const int data_ov214_020cebe4[5] __attribute__((aligned(__alignof__(int)))) = {
    0, 11, 12, 13, 15,
};

/* read by Ov214_spawnFromTable (020cc9e0): int data_ov214_020cebf8[]; */
const int data_ov214_020cebf8[9] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9,
};

/* read by Ov214_ApplyHitEvent (020ccab4): u8 data_ov214_020cec1c[]; */
const u8 data_ov214_020cec1c[12] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 248, 7, 0, 0, 102, 62, 0, 0,
};

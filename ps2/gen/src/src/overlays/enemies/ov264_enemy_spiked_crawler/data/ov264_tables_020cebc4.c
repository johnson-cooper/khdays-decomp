/* PS2: mechanically prepared copy of src/overlays/enemies/ov264_enemy_spiked_crawler/data/ov264_tables_020cebc4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov264 .rodata tables, 0x020cebc4-0x020cec08.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov264_initActor (020cc1fc): KindTable data_ov264_020cebc4; */

#include "nitro/types.h"

const int data_ov264_020cebc4[5] __attribute__((aligned(__alignof__(int)))) = {
    0, 11, 12, 13, 15,
};

/* read by Ov264_spawnFromTable (020cc9e0): int data_ov264_020cebd8[]; */
const int data_ov264_020cebd8[9] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9,
};

/* read by Ov264_handleDamageEvent (020ccab4): u8 data_ov264_020cebfc[]; */
const u8 data_ov264_020cebfc[12] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 248, 7, 0, 0, 102, 62, 0, 0,
};

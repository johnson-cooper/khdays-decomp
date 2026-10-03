/* PS2: mechanically prepared copy of src/overlays/enemies/ov226_enemy_windstorm/data/ov226_tables_020d4b20.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov226 .rodata tables, 0x020d4b20-0x020d4b54.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov226_EnemyInit (not yet decompiled) */

#include "nitro/types.h"

const u8 data_ov226_020d4b20[32] __attribute__((aligned(__alignof__(u8)))) = {
    25, 0, 0, 0, 27, 0, 0, 0, 28, 0, 0, 0, 29, 0, 0, 0,
    30, 0, 0, 0, 255, 255, 255, 255, 32, 0, 0, 0, 31, 0, 0, 0,
};

/* read by Ov226_HandleHit (020d0704): const struct Ov226ReactionModes data_ov226_020d4b40; */
const u8 data_ov226_020d4b40[16] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 0, 144, 0, 0, 51, 25, 0, 0, 51, 99, 0, 0,
};

/* read by Ov226_ConstructItem (020d3f94): const struct Ov226Saved data_ov226_020d4b50; */
const int data_ov226_020d4b50[1] __attribute__((aligned(__alignof__(int)))) = {
    25,
};

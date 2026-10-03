/* PS2: mechanically prepared copy of src/overlays/players/ov098_player_vexen_4/data/ov098_tables_020bbc30.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov098 .rodata tables, 0x020bbc30-0x020bbc54.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020bae74): Angles data_ov098_020bbc30; */

#include "nitro/types.h"

const u8 data_ov098_020bbc30[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 85, 21, 0, 0, 171, 234, 255, 255,
};

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020bae74): const Vec3 data_ov098_020bbc3c; */
const u8 data_ov098_020bbc3c[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0,
};

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020bae74): Vec3 data_ov098_020bbc48; */
const u8 data_ov098_020bbc48[12] __attribute__((aligned(__alignof__(u8)))) = {
    31, 1, 0, 0, 20, 22, 0, 0, 246, 16, 0, 0,
};

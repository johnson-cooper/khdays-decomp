/* PS2: mechanically prepared copy of src/overlays/players/ov066_player_donald_2/data/ov066_tables_020b6a4c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov066 .rodata tables, 0x020b6a4c-0x020b6adc.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Spawns the ov066 panel's effect instances around the actor. Builds a rotation matrix (020b6454): struct SpawnRing4 data_ov066_020b6a4c; */

#include "nitro/types.h"

const u8 data_ov066_020b6a4c[48] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 8, 0, 0, 51, 19, 0, 0, 51, 3, 0, 0,
    205, 12, 0, 0, 205, 20, 0, 0, 205, 252, 255, 255, 51, 19, 0, 0,
    102, 22, 0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 24, 0, 0,
};

/* read by Spawns the ov066 panel's effect instances around the actor. Builds a rotation matrix (020b6454): struct SpawnRing8 data_ov066_020b6a7c; */
const u8 data_ov066_020b6a7c[96] __attribute__((aligned(__alignof__(u8)))) = {
    0, 32, 0, 0, 205, 12, 0, 0, 0, 0, 0, 0, 0, 224, 255, 255,
    51, 19, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 205, 12, 0, 0,
    0, 16, 0, 0, 0, 240, 255, 255, 51, 19, 0, 0, 0, 240, 255, 255,
    0, 0, 0, 0, 205, 12, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0,
    51, 19, 0, 0, 0, 224, 255, 255, 0, 240, 255, 255, 205, 12, 0, 0,
    0, 16, 0, 0, 0, 16, 0, 0, 51, 19, 0, 0, 0, 240, 255, 255,
};

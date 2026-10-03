/* PS2: mechanically prepared copy of src/overlays/players/ov050_player_axel_2/data/ov050_tables_020b7448.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov050 .rodata tables, 0x020b7448-0x020b7460.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by ov050 actor: fire the attack. Marks the two effect slots busy, asks ov022_0209fe20 (020b6a00): Vec3 data_ov050_020b7448; */

#include "nitro/types.h"

const u8 data_ov050_020b7448[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0,
};

/* read by ov050 actor: fire the attack. Marks the two effect slots busy, asks ov022_0209fe20 (020b6a00): Vec3 data_ov050_020b7454; */
const u8 data_ov050_020b7454[12] __attribute__((aligned(__alignof__(u8)))) = {
    31, 1, 0, 0, 20, 22, 0, 0, 246, 16, 0, 0,
};

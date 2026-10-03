/* PS2: mechanically prepared copy of src/overlays/players/ov042_player_vexen/data/ov042_tables_020b4690.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov042 .rodata tables, 0x020b4690-0x020b46b4.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020b38d4): Angles data_ov042_020b4690; */

#include "nitro/types.h"

const u8 data_ov042_020b4690[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 85, 21, 0, 0, 171, 234, 255, 255,
};

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020b38d4): const Vec3 data_ov042_020b469c; */
const u8 data_ov042_020b469c[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0,
};

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020b38d4): Vec3 data_ov042_020b46a8; */
const u8 data_ov042_020b46a8[12] __attribute__((aligned(__alignof__(u8)))) = {
    31, 1, 0, 0, 20, 22, 0, 0, 246, 16, 0, 0,
};

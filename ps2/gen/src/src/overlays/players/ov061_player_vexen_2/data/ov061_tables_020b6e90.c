/* PS2: mechanically prepared copy of src/overlays/players/ov061_player_vexen_2/data/ov061_tables_020b6e90.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov061 .rodata tables, 0x020b6e90-0x020b6eb4.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020b60d4): Angles data_ov061_020b6e90; */

#include "nitro/types.h"

const u8 data_ov061_020b6e90[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 85, 21, 0, 0, 171, 234, 255, 255,
};

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020b60d4): const Vec3 data_ov061_020b6e9c; */
const u8 data_ov061_020b6e9c[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0,
};

/* read by * Attack step: the per-frame body of the ov042 enemy's attack state. (020b60d4): Vec3 data_ov061_020b6ea8; */
const u8 data_ov061_020b6ea8[12] __attribute__((aligned(__alignof__(u8)))) = {
    31, 1, 0, 0, 20, 22, 0, 0, 246, 16, 0, 0,
};

/* PS2: mechanically prepared copy of src/overlays/enemies/ov209_enemy_clay_armor_2/data/ov209_tables_020d65fc.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov209 .rodata tables, 0x020d65fc-0x020d663c.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Bounce-shot flight tick: the shot faces its +0x54 velocity (+0x34), the +0x2c timer accumu (020d4a8c): const Cmd14 data_ov209_020d65fc; */

#include "nitro/types.h"

const u8 data_ov209_020d65fc[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Ov209_collectObjectsInSphereRec (020d2788): const struct tbl8 data_ov209_020d660c; */
const u8 data_ov209_020d660c[32] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 255, 255, 255, 255, 255, 255, 1, 0, 0, 0, 255, 255, 255, 255,
    255, 255, 255, 255, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
};

/* read by Construction of the ov208 enemy's item (x3 with ov209/ov268): installs the handlers (+8 (020d5588): const struct PoseIds data_ov209_020d662c; */
const int data_ov209_020d662c[4] __attribute__((aligned(__alignof__(int)))) = {
    17, 17, 18, 19,
};

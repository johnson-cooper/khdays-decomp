/* PS2: mechanically prepared copy of src/overlays/enemies/ov297_enemy_mystery_71/data/ov297_tables_020d5690.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov297 .rodata tables, 0x020d5690-0x020d56b4.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov297 enemy: installs the handlers (+8 tick, +0xc draw, +0x1c message, (020d3844): const int data_ov297_020d5690[2]; */

#include "nitro/types.h"

const int data_ov297_020d5690[2] __attribute__((aligned(__alignof__(int)))) = {
    2, 3,
};

/* read by Hit handler of the ov297 enemy: the damage is 1; the hit is ignored while the +0x21a stami (020d3dec): const struct ModeTable data_ov297_020d5698; */
const u8 data_ov297_020d5698[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 1, 2, 3,
};

/* read by Hide entry of the ov297 enemy: spawns effect 0 and fires reaction 0x176 mode 4 at the +8 (020d5540): const struct HideTable data_ov297_020d569c; */
const u8 data_ov297_020d569c[24] __attribute__((aligned(__alignof__(u8)))) = {
    102, 234, 1, 0, 35, 211, 255, 255, 4, 102, 1, 0, 92, 51, 255, 255,
    129, 175, 255, 255, 0, 224, 1, 0,
};

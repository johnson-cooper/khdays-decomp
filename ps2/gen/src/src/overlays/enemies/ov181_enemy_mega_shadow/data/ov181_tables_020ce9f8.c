/* PS2: mechanically prepared copy of src/overlays/enemies/ov181_enemy_mega_shadow/data/ov181_tables_020ce9f8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov181 .rodata tables, 0x020ce9f8-0x020cea20.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov181 enemy (x4: ov181/182/183/184): installs the handlers (+8 tick, +0 (020cbfc4): struct Ov181Vec3 data_ov181_020ce9f8; */

#include "nitro/types.h"

const int data_ov181_020ce9f8[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by * Swing sweep of the ov181 enemy (x4: ov181/182/183/184): from the +0x14 heading builds th (020cc704): const struct Msg data_ov181_020cea04; */
const u16 data_ov181_020cea04[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by * Dive tick of the ov181 enemy (x4: ov181/182/183/184): copies the +0x78 velocity into +0x (020ce24c): const struct Msg data_ov181_020cea12; */
const u16 data_ov181_020cea12[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

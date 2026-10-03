/* PS2: mechanically prepared copy of src/overlays/enemies/ov184_enemy_mega_shadow_4/data/ov184_tables_020d4458.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov184 .rodata tables, 0x020d4458-0x020d4480.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov181 enemy (x4: ov181/182/183/184): installs the handlers (+8 tick, +0 (020d1a24): struct Ov181Vec3 data_ov184_020d4458; */

#include "nitro/types.h"

const int data_ov184_020d4458[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by * Swing sweep of the ov181 enemy (x4: ov181/182/183/184): from the +0x14 heading builds th (020d2164): const struct Msg data_ov184_020d4464; */
const u16 data_ov184_020d4464[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by * Dive tick of the ov181 enemy (x4: ov181/182/183/184): copies the +0x78 velocity into +0x (020d3cac): const struct Msg data_ov184_020d4472; */
const u16 data_ov184_020d4472[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

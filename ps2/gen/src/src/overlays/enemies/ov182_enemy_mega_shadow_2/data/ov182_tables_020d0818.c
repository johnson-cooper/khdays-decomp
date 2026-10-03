/* PS2: mechanically prepared copy of src/overlays/enemies/ov182_enemy_mega_shadow_2/data/ov182_tables_020d0818.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov182 .rodata tables, 0x020d0818-0x020d0840.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov181 enemy (x4: ov181/182/183/184): installs the handlers (+8 tick, +0 (020cdde4): struct Ov181Vec3 data_ov182_020d0818; */

#include "nitro/types.h"

const int data_ov182_020d0818[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

/* read by * Swing sweep of the ov181 enemy (x4: ov181/182/183/184): from the +0x14 heading builds th (020ce524): const struct Msg data_ov182_020d0824; */
const u16 data_ov182_020d0824[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by * Dive tick of the ov181 enemy (x4: ov181/182/183/184): copies the +0x78 velocity into +0x (020d006c): const struct Msg data_ov182_020d0832; */
const u16 data_ov182_020d0832[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

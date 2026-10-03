/* PS2: mechanically prepared copy of src/overlays/enemies/ov166_enemy_scarlet_tango/data/ov166_tables_020cead0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov166 .rodata tables, 0x020cead0-0x020ceae0.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov166 enemy (x3: ov166/167/168). Installs the handlers (+8 release, +0x (020cc020): IdTable data_ov166_020cead0; */

#include "nitro/types.h"

const int data_ov166_020cead0[3] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4,
};

/* read by * Hit handler of the ov166 enemy (x3: ov166/167/168): copies the hit point into +0x2c and  (020cc7b8): const u8 data_ov166_020ceadc[]; */
const u8 data_ov166_020ceadc[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

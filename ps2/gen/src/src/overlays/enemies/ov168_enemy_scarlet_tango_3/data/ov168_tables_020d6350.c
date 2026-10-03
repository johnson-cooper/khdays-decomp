/* PS2: mechanically prepared copy of src/overlays/enemies/ov168_enemy_scarlet_tango_3/data/ov168_tables_020d6350.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov168 .rodata tables, 0x020d6350-0x020d6360.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov166 enemy (x3: ov166/167/168). Installs the handlers (+8 release, +0x (020d38a0): IdTable data_ov168_020d6350; */

#include "nitro/types.h"

const int data_ov168_020d6350[3] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4,
};

/* read by * Hit handler of the ov166 enemy (x3: ov166/167/168): copies the hit point into +0x2c and  (020d4038): const u8 data_ov168_020d635c[]; */
const u8 data_ov168_020d635c[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

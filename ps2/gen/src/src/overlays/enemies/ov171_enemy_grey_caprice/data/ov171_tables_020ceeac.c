/* PS2: mechanically prepared copy of src/overlays/enemies/ov171_enemy_grey_caprice/data/ov171_tables_020ceeac.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov171 .rodata tables, 0x020ceeac-0x020ceebc.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov171 enemy (twins by byte identity). Installs the handlers (+8 release (020cc020): IdTable data_ov171_020ceeac; */

#include "nitro/types.h"

const int data_ov171_020ceeac[3] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4,
};

/* read by * Hit handler of the ov171 enemy (and its byte-identical twins): copies the hit point into (020cc7bc): const u8 data_ov171_020ceeb8[]; */
const u8 data_ov171_020ceeb8[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

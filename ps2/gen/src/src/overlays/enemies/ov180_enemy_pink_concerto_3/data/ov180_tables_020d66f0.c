/* PS2: mechanically prepared copy of src/overlays/enemies/ov180_enemy_pink_concerto_3/data/ov180_tables_020d66f0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov180 .rodata tables, 0x020d66f0-0x020d670c.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov180 enemy (twins by byte identity). Installs the handlers (+8 release (020d38a0): IdTable data_ov180_020d66f0; */

#include "nitro/types.h"

const int data_ov180_020d66f0[6] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4, 5, 7, 6,
};

/* read by * Hit handler of the ov178 enemy (and its byte-identical twins): copies the hit point into (020d4124): const u8 data_ov180_020d6708[]; */
const u8 data_ov180_020d6708[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

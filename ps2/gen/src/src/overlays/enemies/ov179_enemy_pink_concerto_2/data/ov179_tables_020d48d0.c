/* PS2: mechanically prepared copy of src/overlays/enemies/ov179_enemy_pink_concerto_2/data/ov179_tables_020d48d0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov179 .rodata tables, 0x020d48d0-0x020d48ec.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov179 enemy (twins by byte identity). Installs the handlers (+8 release (020d1a80): IdTable data_ov179_020d48d0; */

#include "nitro/types.h"

const int data_ov179_020d48d0[6] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4, 5, 7, 6,
};

/* read by * Hit handler of the ov178 enemy (and its byte-identical twins): copies the hit point into (020d2304): const u8 data_ov179_020d48e8[]; */
const u8 data_ov179_020d48e8[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

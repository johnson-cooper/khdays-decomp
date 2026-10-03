/* PS2: mechanically prepared copy of src/overlays/enemies/ov170_enemy_turquoise_march_2/data/ov170_tables_020d0c64.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov170 .rodata tables, 0x020d0c64-0x020d0c78.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov170 enemy (twins by byte identity). Installs the handlers (+8 release (020cde40): IdTable data_ov170_020d0c64; */

#include "nitro/types.h"

const int data_ov170_020d0c64[4] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4, 5,
};

/* read by * Hit handler of the ov169 enemy (and its byte-identical twins): copies the hit point into (020ce604): const u8 data_ov170_020d0c74[]; */
const u8 data_ov170_020d0c74[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

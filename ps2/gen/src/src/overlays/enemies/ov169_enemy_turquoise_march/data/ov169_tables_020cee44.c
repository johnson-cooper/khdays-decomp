/* PS2: mechanically prepared copy of src/overlays/enemies/ov169_enemy_turquoise_march/data/ov169_tables_020cee44.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov169 .rodata tables, 0x020cee44-0x020cee58.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov169 enemy (twins by byte identity). Installs the handlers (+8 release (020cc020): IdTable data_ov169_020cee44; */

#include "nitro/types.h"

const int data_ov169_020cee44[4] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4, 5,
};

/* read by * Hit handler of the ov169 enemy (and its byte-identical twins): copies the hit point into (020cc7e4): const u8 data_ov169_020cee54[]; */
const u8 data_ov169_020cee54[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

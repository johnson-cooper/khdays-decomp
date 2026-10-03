/* PS2: mechanically prepared copy of src/overlays/enemies/ov177_enemy_striped_aria_3/data/ov177_tables_020d62f8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov177 .rodata tables, 0x020d62f8-0x020d6308.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov175 enemy (twins by byte identity). Installs the handlers (+8 release (020d38a0): IdTable data_ov177_020d62f8; */

#include "nitro/types.h"

const int data_ov177_020d62f8[3] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4,
};

/* read by Hit handler of the ov175 enemy (x3: ov175/176/177), ported from the matched ov166 sibling. (020d4038): const u8 data_ov177_020d6304[]; */
const u8 data_ov177_020d6304[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

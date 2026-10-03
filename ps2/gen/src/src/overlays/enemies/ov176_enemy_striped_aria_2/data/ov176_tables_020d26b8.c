/* PS2: mechanically prepared copy of src/overlays/enemies/ov176_enemy_striped_aria_2/data/ov176_tables_020d26b8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov176 .rodata tables, 0x020d26b8-0x020d26c8.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov175 enemy (twins by byte identity). Installs the handlers (+8 release (020cfc60): IdTable data_ov176_020d26b8; */

#include "nitro/types.h"

const int data_ov176_020d26b8[3] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4,
};

/* read by Hit handler of the ov175 enemy (x3: ov175/176/177), ported from the matched ov166 sibling. (020d03f8): const u8 data_ov176_020d26c4[]; */
const u8 data_ov176_020d26c4[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

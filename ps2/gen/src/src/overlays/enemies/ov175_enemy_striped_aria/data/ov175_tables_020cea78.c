/* PS2: mechanically prepared copy of src/overlays/enemies/ov175_enemy_striped_aria/data/ov175_tables_020cea78.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov175 .rodata tables, 0x020cea78-0x020cea88.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov175 enemy (twins by byte identity). Installs the handlers (+8 release (020cc020): IdTable data_ov175_020cea78; */

#include "nitro/types.h"

const int data_ov175_020cea78[3] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 4,
};

/* read by Hit handler of the ov175 enemy (x3: ov175/176/177), ported from the matched ov166 sibling. (020cc7b8): const u8 data_ov175_020cea84[]; */
const u8 data_ov175_020cea84[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

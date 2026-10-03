/* PS2: mechanically prepared copy of src/overlays/enemies/ov173_enemy_sapphire_elegy/data/ov173_tables_020cec98.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov173 .rodata tables, 0x020cec98-0x020cecac.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov173 enemy (twins by byte identity). Installs the handlers (+8 release (020cc020): IdTable data_ov173_020cec98; */

#include "nitro/types.h"

const int data_ov173_020cec98[4] __attribute__((aligned(__alignof__(int)))) = {
    4, 2, 3, 1,
};

/* read by Hit handler of the ov173 enemy (x2: ov173/174), variant of the matched ov166 sibling. Reco (020cc7c8): const u8 data_ov173_020ceca8[]; */
const u8 data_ov173_020ceca8[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1,
};

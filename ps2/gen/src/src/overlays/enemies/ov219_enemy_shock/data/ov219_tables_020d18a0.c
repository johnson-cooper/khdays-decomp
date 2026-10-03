/* PS2: mechanically prepared copy of src/overlays/enemies/ov219_enemy_shock/data/ov219_tables_020d18a0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov219 .rodata tables, 0x020d18a0-0x020d18a8.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov219 enemy (twin of the ov220 constructor, sound 0x136): installs the  (020cfc04): const Kinds data_ov219_020d18a0; */

#include "nitro/types.h"

const u8 data_ov219_020d18a0[4] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 4, 0,
};

/* read by Hit handler of the ov219 enemy: records the hit point (+0x30) and parameter (+0x40) in the (020d00e8): const u8 data_ov219_020d18a4[]; */
const u8 data_ov219_020d18a4[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 1, 2, 3,
};

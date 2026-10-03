/* PS2: mechanically prepared copy of src/overlays/enemies/ov114_enemy_shadow/data/ov114_tables_020cdfb4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov114 .rodata tables, 0x020cdfb4-0x020cdfd0.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov114 enemy: installs the handlers (+8 tick, +0xc draw, +0x1c/+0x20 (020cbfc4): struct Ov114Kinds data_ov114_020cdfb4; */

#include "nitro/types.h"

const int data_ov114_020cdfb4[2] __attribute__((aligned(__alignof__(int)))) = {
    1, 2,
};

/* read by Face the target and start the wind-up: if there is no target, drop straight to action 2. (020cdb14): Ov114Pair data_ov114_020cdfbc; */
const u8 data_ov114_020cdfbc[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 2,
};

/* read by Swing sweep for ov114: probe the forward volume, notify each accepted hit, (020cc6f4): const struct Msg data_ov114_020cdfc0; */
const u8 data_ov114_020cdfc0[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

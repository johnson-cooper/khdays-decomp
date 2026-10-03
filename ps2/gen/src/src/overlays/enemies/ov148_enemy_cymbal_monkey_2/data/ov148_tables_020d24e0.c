/* PS2: mechanically prepared copy of src/overlays/enemies/ov148_enemy_cymbal_monkey_2/data/ov148_tables_020d24e0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov148 .rodata tables, 0x020d24e0-0x020d24f0.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov148_ResolveHitReaction (020d0440): const struct ReactionModes data_ov148_020d24e0; */

#include "nitro/types.h"

const u8 data_ov148_020d24e0[8] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 96, 8, 0, 0,
};

/* read by Ov148_InitSubActor (020d1974): const struct ChildIds data_ov148_020d24e8; */
const int data_ov148_020d24e8[2] __attribute__((aligned(__alignof__(int)))) = {
    3, 5,
};

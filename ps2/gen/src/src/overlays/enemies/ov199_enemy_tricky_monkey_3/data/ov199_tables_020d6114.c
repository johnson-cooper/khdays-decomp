/* PS2: mechanically prepared copy of src/overlays/enemies/ov199_enemy_tricky_monkey_3/data/ov199_tables_020d6114.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov199 .rodata tables, 0x020d6114-0x020d6124.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov199_ResolveHitReaction (020d4078): const struct ReactionModes data_ov199_020d6114; */

#include "nitro/types.h"

const u8 data_ov199_020d6114[8] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 96, 8, 0, 0,
};

/* read by Ov199_InitSubActor (020d55a8): const struct ChildIds data_ov199_020d611c; */
const int data_ov199_020d611c[2] __attribute__((aligned(__alignof__(int)))) = {
    3, 5,
};

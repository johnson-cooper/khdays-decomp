/* PS2: mechanically prepared copy of src/overlays/enemies/ov197_enemy_tricky_monkey/data/ov197_tables_020ce894.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov197 .rodata tables, 0x020ce894-0x020ce8a4.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov197_ResolveHitReaction (020cc7f8): const struct ReactionModes data_ov197_020ce894; */

#include "nitro/types.h"

const u8 data_ov197_020ce894[8] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 96, 8, 0, 0,
};

/* read by Ov197_InitSubActor (020cdd28): const struct ChildIds data_ov197_020ce89c; */
const int data_ov197_020ce89c[2] __attribute__((aligned(__alignof__(int)))) = {
    3, 5,
};

/* PS2: mechanically prepared copy of src/overlays/enemies/ov147_enemy_cymbal_monkey/data/ov147_tables_020ce8a0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov147 .rodata tables, 0x020ce8a0-0x020ce8b0.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov147_ResolveHitReaction (020cc800): const struct ReactionModes data_ov147_020ce8a0; */

#include "nitro/types.h"

const u8 data_ov147_020ce8a0[8] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 96, 8, 0, 0,
};

/* read by Ov147_InitSubActor (020cdd34): const struct ChildIds data_ov147_020ce8a8; */
const int data_ov147_020ce8a8[2] __attribute__((aligned(__alignof__(int)))) = {
    3, 5,
};

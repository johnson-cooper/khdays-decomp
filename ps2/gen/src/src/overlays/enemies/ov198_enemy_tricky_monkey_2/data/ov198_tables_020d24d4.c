/* PS2: mechanically prepared copy of src/overlays/enemies/ov198_enemy_tricky_monkey_2/data/ov198_tables_020d24d4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov198 .rodata tables, 0x020d24d4-0x020d24e4.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov198_ResolveHitReaction (020d0438): const struct ReactionModes data_ov198_020d24d4; */

#include "nitro/types.h"

const u8 data_ov198_020d24d4[8] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 96, 8, 0, 0,
};

/* read by Ov198_InitSubActor (020d1968): const struct ChildIds data_ov198_020d24dc; */
const int data_ov198_020d24dc[2] __attribute__((aligned(__alignof__(int)))) = {
    3, 5,
};

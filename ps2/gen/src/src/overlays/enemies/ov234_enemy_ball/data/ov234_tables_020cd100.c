/* PS2: mechanically prepared copy of src/overlays/enemies/ov234_enemy_ball/data/ov234_tables_020cd100.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov234 .rodata tables, 0x020cd100-0x020cd108.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov234_InitEffectActor (020cbfc4): const struct Ov234TextureTable data_ov234_020cd100; */

#include "nitro/types.h"

const int data_ov234_020cd100[1] __attribute__((aligned(__alignof__(int)))) = {
    1,
};

/* read by Ov234_ResolveHitReaction (020cc574): const struct Ov234ReactionModes data_ov234_020cd104; */
const u8 data_ov234_020cd104[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 1, 2, 3,
};

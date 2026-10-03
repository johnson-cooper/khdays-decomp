/* PS2: mechanically prepared copy of src/overlays/enemies/ov117_enemy_possessor/data/ov117_tables_020cdc28.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov117 .rodata tables, 0x020cdc28-0x020cdc44.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov117_OrbitStep (020cd2f4): Pair data_ov117_020cdc28[];
 *   Publish the landing: copy the pending anchor into the live slot and ask the placement help (020cd494): Pair data_ov117_020cdc28[];
 *   Ov117_EmitAtOrbit (020cd78c): Pair16 data_ov117_020cdc28[]; */

#include "nitro/types.h"

const u8 data_ov117_020cdc28[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0,
};

/* read by Ov117_InitEffectActor (020cbfc4): const struct CameraWork data_ov117_020cdc34; */
const int data_ov117_020cdc34[4] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 0, 2560,
};

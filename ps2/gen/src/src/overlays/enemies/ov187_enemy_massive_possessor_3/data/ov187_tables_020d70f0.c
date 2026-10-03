/* PS2: mechanically prepared copy of src/overlays/enemies/ov187_enemy_massive_possessor_3/data/ov187_tables_020d70f0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov187 .rodata tables, 0x020d70f0-0x020d711c.
 *
 * 3 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov187_Actor_Construct (020d3848): const struct CameraWork data_ov187_020d70f0; */

#include "nitro/types.h"

const int data_ov187_020d70f0[4] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 0, 3072,
};

/* read by Ov187_OrbitStep (020d67bc): Pair data_ov187_020d7100[];
 *   Publish the landing: copy the pending anchor into the live slot and ask the placement help (020d695c): Ev data_ov187_020d7100[];
 *   Ov187_EmitAtOrbit (020d6c54): Pair16 data_ov187_020d7100[]; */
const u8 data_ov187_020d7100[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0,
};

/* read by Ov187_InitEffectActor (020d548c): const struct CameraWork data_ov187_020d710c; */
const int data_ov187_020d710c[4] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 0, 2560,
};

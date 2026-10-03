/* PS2: mechanically prepared copy of src/overlays/enemies/ov118_enemy_possessor_2/data/ov118_tables_020d1868.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov118 .rodata tables, 0x020d1868-0x020d1884.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov118_OrbitStep (020d0f34): Pair data_ov118_020d1868[];
 *   Publish the landing: copy the pending anchor into the live slot and ask the placement help (020d10d4): Ev data_ov118_020d1868[];
 *   Ov118_EmitAtOrbit (020d13cc): Pair16 data_ov118_020d1868[]; */

#include "nitro/types.h"

const u8 data_ov118_020d1868[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0,
};

/* read by Ov118_InitEffectActor (020cfc04): const struct CameraWork data_ov118_020d1874; */
const int data_ov118_020d1874[4] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 0, 2560,
};

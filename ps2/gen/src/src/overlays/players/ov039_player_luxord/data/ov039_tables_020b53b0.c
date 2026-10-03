/* PS2: mechanically prepared copy of src/overlays/players/ov039_player_luxord/data/ov039_tables_020b53b0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov039 .rodata tables, 0x020b53b0-0x020b5404.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Compute one of this enemy's attack anchor points into `out`: the plain offset {0, 0x2000, (020b4050): const Ov039SidePair data_ov039_020b53b0; */

#include "nitro/types.h"

const u8 data_ov039_020b53b0[24] __attribute__((aligned(__alignof__(u8)))) = {
    0, 244, 255, 255, 0, 32, 0, 0, 174, 15, 0, 0, 0, 12, 0, 0,
    0, 32, 0, 0, 215, 15, 0, 0,
};

/* read by Push this enemy's attack event(s): event 5 at each attack anchor (Ov039_GetAttackAnchor) (020b46b0): const Ov039BandRow data_ov039_020b53c8; */
const int data_ov039_020b53c8[15] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 0, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 1, 1, 1,
};

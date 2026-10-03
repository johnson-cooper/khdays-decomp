/* PS2: mechanically prepared copy of src/overlays/players/ov058_player_luxord_2/data/ov058_tables_020b7bb0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov058 .rodata tables, 0x020b7bb0-0x020b7c04.
 *
 * 2 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Compute one of this enemy's attack anchor points into `out`: the plain offset {0, 0x2000, (020b6850): const Ov039SidePair data_ov058_020b7bb0; */

#include "nitro/types.h"

const u8 data_ov058_020b7bb0[24] __attribute__((aligned(__alignof__(u8)))) = {
    0, 244, 255, 255, 0, 32, 0, 0, 174, 15, 0, 0, 0, 12, 0, 0,
    0, 32, 0, 0, 215, 15, 0, 0,
};

/* read by Push this enemy's attack event(s): event 5 at each attack anchor (Ov039_GetAttackAnchor) (020b6eb0): const Ov039BandRow data_ov058_020b7bc8; */
const int data_ov058_020b7bc8[15] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 0, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 1, 1, 1,
};

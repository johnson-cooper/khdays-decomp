/* PS2: mechanically prepared copy of src/overlays/enemies/ov271_enemy_destroyer/data/ov271_tables_020d3678.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov271 .rodata tables, 0x020d3678-0x020d36c4.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov271_Construct (020cfc04): IdTable data_ov271_020d3678; */

#include "nitro/types.h"

const int data_ov271_020d3678[4] __attribute__((aligned(__alignof__(int)))) = {
    1, 6, 7, 7,
};

/* read by Ov271_Construct (020cfc04): Box data_ov271_020d3688; */
const u8 data_ov271_020d3688[24] __attribute__((aligned(__alignof__(u8)))) = {
    14, 241, 255, 255, 223, 30, 0, 0, 111, 245, 255, 255, 241, 14, 0, 0,
    67, 76, 0, 0, 197, 12, 0, 0,
};

/* read by Ov271_HandleHit (020d04cc): const u8 data_ov271_020d36a0[];
 *   Publish the swing: send the canned 4-byte block from the config table to the owner's notif (020d1b70): struct blk data_ov271_020d36a0;
 *   * Ov271_OrientReadyTimerNodeGate -- x3. AI-state tick: orient toward the target, then transition on a (020d1c14): struct h2 data_ov271_020d36a0[];
 *   Enter the attack state: refresh the owner, convert the owner's per-frame delta into the (020d1dc8): struct blk data_ov271_020d36a0;
 *   * Ov271_OrientTimerNodeGate -- x3. AI-state tick: clamp, aim, and transition on a timer. (020d1e7c): struct h2 data_ov271_020d36a0[]; */
const u8 data_ov271_020d36a0[20] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 0, 0, 5, 2, 0, 0, 5, 1, 0, 0, 5, 3,
    0, 0, 5, 4,
};

/* read by Beam charge tick of the ov200 enemy (x3: ov200/ov201/ov271). The aim point is the midpoint (020d26ec): const Cmd4 data_ov271_020d36b4; */
const u8 data_ov271_020d36b4[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0,
};

/* read by Beam state builder of the ov200 enemy (x3: ov200/ov201/ov271). Spawns the 0x54-byte beam (020d21a0): const KindTable data_ov271_020d36b8; */
const int data_ov271_020d36b8[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

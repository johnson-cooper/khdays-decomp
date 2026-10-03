/* PS2: mechanically prepared copy of src/overlays/enemies/ov201_enemy_guardian_2/data/ov201_tables_020d5444.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov201 .rodata tables, 0x020d5444-0x020d5490.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Constructor of the ov200 enemy (x2 with ov201). Installs the handlers (+8, +0xc draw, +0x1 (020d1a24): IdTable data_ov201_020d5444; */

#include "nitro/types.h"

const int data_ov201_020d5444[4] __attribute__((aligned(__alignof__(int)))) = {
    1, 6, 7, 7,
};

/* read by Constructor of the ov200 enemy (x2 with ov201). Installs the handlers (+8, +0xc draw, +0x1 (020d1a24): Box data_ov201_020d5454; */
const u8 data_ov201_020d5454[24] __attribute__((aligned(__alignof__(u8)))) = {
    14, 241, 255, 255, 223, 30, 0, 0, 111, 245, 255, 255, 241, 14, 0, 0,
    67, 76, 0, 0, 197, 12, 0, 0,
};

/* read by Hit handler of the ov200 enemy (x3: ov200/ov201/ov271). An actor without hit points ignore (020d22ec): const u8 data_ov201_020d546c[];
 *   Publish the swing: send the canned 4-byte block from the config table to the owner's notif (020d393c): struct blk data_ov201_020d546c;
 *   * Ov201_OrientReadyTimerNodeGate -- x3. AI-state tick: orient toward the target, then transition on a (020d39e0): struct h2 data_ov201_020d546c[];
 *   Enter the attack state: refresh the owner, convert the owner's per-frame delta into the (020d3b94): struct blk data_ov201_020d546c;
 *   * Ov201_OrientTimerNodeGate -- x3. AI-state tick: clamp, aim, and transition on a timer. (020d3c48): struct h2 data_ov201_020d546c[]; */
const u8 data_ov201_020d546c[20] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 0, 0, 5, 2, 0, 0, 5, 1, 0, 0, 5, 3,
    0, 0, 5, 4,
};

/* read by Beam charge tick of the ov200 enemy (x3: ov200/ov201/ov271). The aim point is the midpoint (020d44b8): const Cmd4 data_ov201_020d5480; */
const u8 data_ov201_020d5480[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0,
};

/* read by Beam state builder of the ov200 enemy (x3: ov200/ov201/ov271). Spawns the 0x54-byte beam (020d3f6c): const KindTable data_ov201_020d5484; */
const int data_ov201_020d5484[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

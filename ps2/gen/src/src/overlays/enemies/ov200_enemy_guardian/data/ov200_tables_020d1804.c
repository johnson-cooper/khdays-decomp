/* PS2: mechanically prepared copy of src/overlays/enemies/ov200_enemy_guardian/data/ov200_tables_020d1804.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov200 .rodata tables, 0x020d1804-0x020d1850.
 *
 * 5 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov200_Construct (020cdde4): IdTable data_ov200_020d1804; */

#include "nitro/types.h"

const int data_ov200_020d1804[4] __attribute__((aligned(__alignof__(int)))) = {
    1, 6, 7, 7,
};

/* read by Ov200_Construct (020cdde4): Box data_ov200_020d1814; */
const u8 data_ov200_020d1814[24] __attribute__((aligned(__alignof__(u8)))) = {
    14, 241, 255, 255, 223, 30, 0, 0, 111, 245, 255, 255, 241, 14, 0, 0,
    67, 76, 0, 0, 197, 12, 0, 0,
};

/* read by Ov200_HandleHit (020ce6ac): const u8 data_ov200_020d182c[];
 *   Publish the swing: send the canned 4-byte block from the config table to the owner's notif (020cfcfc): struct blk data_ov200_020d182c;
 *   * Ov200_OrientReadyTimerNodeGate -- x3. AI-state tick: orient toward the target, then transition on a (020cfda0): struct h2 data_ov200_020d182c[];
 *   Enter the attack state: refresh the owner, convert the owner's per-frame delta into the (020cff54): struct blk data_ov200_020d182c;
 *   * Ov200_OrientTimerNodeGate -- x3. AI-state tick: clamp, aim, and transition on a timer. (020d0008): struct h2 data_ov200_020d182c[]; */
const u8 data_ov200_020d182c[20] __attribute__((aligned(__alignof__(u8)))) = {
    2, 3, 0, 1, 0, 0, 5, 2, 0, 0, 5, 1, 0, 0, 5, 3,
    0, 0, 5, 4,
};

/* read by Ov200_BeamChargeTick (020d0878): const Cmd4 data_ov200_020d1840; */
const u8 data_ov200_020d1840[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0,
};

/* read by Ov200_BuildBeamState (020d032c): const KindTable data_ov200_020d1844; */
const int data_ov200_020d1844[3] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4,
};

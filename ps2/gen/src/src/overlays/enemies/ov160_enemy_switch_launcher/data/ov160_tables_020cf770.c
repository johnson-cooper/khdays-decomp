/* PS2: mechanically prepared copy of src/overlays/enemies/ov160_enemy_switch_launcher/data/ov160_tables_020cf770.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov160 .rodata tables, 0x020cf770-0x020cf7e4.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov160_Construct (020cbfc4): struct Ov160Kinds data_ov160_020cf770; */

#include "nitro/types.h"

const int data_ov160_020cf770[9] __attribute__((aligned(__alignof__(int)))) = {
    2, 3, 4, 5, 6, 7, 8, 9,
    10,
};

/* read by c634 handler: arm the second phase of the move. (020cd84c): unsigned short data_ov160_020cf794[];
 *   Ov160_AttackWaitTick (020cd918): unsigned short data_ov160_020cf794[];
 *   set the follow-up delay, and if the counter allows, commit the move. (020cdb28): unsigned short data_ov160_020cf794[];
 *   Ov160_AimedAttackWaitTick (020cdc04): unsigned short data_ov160_020cf794[]; */
const u8 data_ov160_020cf794[20] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 1, 0, 0, 5, 4, 0, 0, 5, 2, 0, 0, 5, 0,
    0, 0, 5, 3,
};

/* read by Ov160_EnterGroundDrop (020ce738): Vec3 data_ov160_020cf7a8; */
const u8 data_ov160_020cf7a8[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 248, 255, 255, 0, 0, 0, 0,
};

/* read by Ground drop tick of the ov160 enemy. In owner mode 1 the +0x28 radius grows by the owner's (020ce834): const PosMsg data_ov160_020cf7b4; */
const u8 data_ov160_020cf7b4[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* read by Ov160_ConstructItem (020cebc0): const struct Ov160Pose data_ov160_020cf7c4; */
const int data_ov160_020cf7c4[1] __attribute__((aligned(__alignof__(int)))) = {
    7,
};

/* read by Ov160_DashTick (020cf1a4): const PosMsg data_ov160_020cf7c8; */
const u16 data_ov160_020cf7c8[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 5, 0, 0, 0, 0, 0,
};

/* read by Ov160_GrabTick (020cf4f4): const PosMsg data_ov160_020cf7d6; */
const u16 data_ov160_020cf7d6[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 1285, 0, 0, 0, 0, 0,
};

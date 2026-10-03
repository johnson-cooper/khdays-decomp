/* PS2: mechanically prepared copy of src/overlays/enemies/ov257_enemy_xion_51/data/ov257_rodata_020d3064.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov257 .rodata 0x020d3064-0x020d334c: the actor's tables in one unit, in address order. The
 * pointer tables are all zero in the ROM image (every entry is a relocation onto the unit's
 * resource name strings); the other tables are written in the width their contents are in. */

#include "nitro/types.h"

extern int gOv257Tag02FlName;
extern int gOv257Tag02BrName;
extern int gOv257Tag02BlName;
extern int gOv257Tag02FrName;
extern int gOv257B2RHand02Name;
extern int gOv257B2LHand02Name;
extern int gOv257B2RHand03Name;
extern int gOv257B2LHand03Name;

/* read by Ov257_EnemyConstruct (not yet decompiled) */
const int data_ov257_020d3064[4] __attribute__((aligned(__alignof__(int)))) = {
    71, 72, 73, 74,
};

void *const data_ov257_020d3074[4] __attribute__((aligned(__alignof__(void *)))) = {
    &gOv257B2LHand02Name,
    &gOv257B2RHand02Name,
    &gOv257B2LHand03Name,
    &gOv257B2RHand03Name,
};

void *const data_ov257_020d3084[4] __attribute__((aligned(__alignof__(void *)))) = {
    &gOv257Tag02FlName,
    &gOv257Tag02FrName,
    &gOv257Tag02BlName,
    &gOv257Tag02BrName,
};

/* read by Ov257_EnemyConstruct (not yet decompiled) */
const int data_ov257_020d3094[12] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 75, 76, 77,
};

/* read by Ov257_SetMotion (020ccbac): MotionTable data_ov257_020d30c4; */
const int data_ov257_020d30c4[34] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32,
    33, 34,
};

/* read by Ov257_SetMotion (020ccbac): MotionTable data_ov257_020d314c; */
const int data_ov257_020d314c[34] __attribute__((aligned(__alignof__(int)))) = {
    36, 37, 38, 39, 40, 41, 42, 43,
    44, 45, 46, 47, 48, 49, 50, 51,
    52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67,
    68, 69,
};

/* read by Ov257_SetMotion (020ccbac): MotionTable data_ov257_020d31d4; */
const u8 data_ov257_020d31d4[136] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0,
    255, 255, 255, 255, 255, 255, 255, 255, 3, 0, 0, 0, 4, 0, 0, 0,
    5, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    6, 0, 0, 0, 7, 0, 0, 0, 8, 0, 0, 0, 9, 0, 0, 0,
    10, 0, 0, 0, 11, 0, 0, 0,
};

/* read by Ov257_GroundStrikeEnterTick (020cf3f8): const Cmd4 data_ov257_020d325c;
 *   Ov257_HoverInTick (020d01ec): const Cmd4 data_ov257_020d325c[];
 *   Ov257_HealBurstTick (not yet decompiled)
 *   Ov257_SwoopEnterTick (020d096c) */
const u8 data_ov257_020d325c[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 0, 0, 0, 5, 8, 0, 0, 5, 1, 0, 0, 5, 2,
};

/* read by Ov257_LandingSlamTick (020d1f00): const Vec3 data_ov257_020d326c; */
const u8 data_ov257_020d326c[12] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 0, 0,
};

/* read by Ov257_FollowUpStrikeTick (020d101c): const Cmd14 data_ov257_020d3278; */
const u16 data_ov257_020d3278[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 2565, 0, 0, 0, 0, 0,
};

/* read by Ov257_FollowUpStrikeTick (020d101c): const Cmd14 data_ov257_020d3286; */
const u16 data_ov257_020d3286[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 2565, 0, 0, 0, 0, 0,
};

/* read by Ov257_LandingSlamTick (020d1f00): const Cmd14 data_ov257_020d3294; */
const u16 data_ov257_020d3294[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by Ov257_BiteTick (020ce870): const Cmd14 data_ov257_020d32a2; */
const u16 data_ov257_020d32a2[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by Ov257_FinishingStrikeTick (020d167c): const Cmd14 data_ov257_020d32b0; */
const u16 data_ov257_020d32b0[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 2565, 0, 0, 0, 0, 0,
};

/* read by Ov257_HealBurstTick (not yet decompiled) */
const u16 data_ov257_020d32be[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 1541, 0, 0, 0, 0, 0,
};

/* read by Ov257_DoubleStrikeTick (020d0a00): const Cmd14 data_ov257_020d32cc; */
const u16 data_ov257_020d32cc[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 2565, 0, 0, 0, 0, 0,
};

/* read by Ov257_WhirlFinishTick (020cfbac): const Cmd14 data_ov257_020d32da; */
const u16 data_ov257_020d32da[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by Ov257_WhirlTick (020cf7d4): const Cmd14 data_ov257_020d32e8; */
const u16 data_ov257_020d32e8[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 1285, 0, 0, 0, 0, 0,
};

/* read by Ov257_GroundStrikeEnterTick (020cf3f8): const Cmd14 data_ov257_020d32f6; */
const u16 data_ov257_020d32f6[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 2821, 0, 0, 0, 0, 0,
};

/* read by Ov257_DoubleStrikeTick (020d0a00): const Cmd14 data_ov257_020d3304; */
const u16 data_ov257_020d3304[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 2565, 0, 0, 0, 0, 0,
};

/* read by Ov257_ThirdBiteTick (020cf044): const Cmd14 data_ov257_020d3312; */
const u16 data_ov257_020d3312[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by Ov257_SecondBiteTick (020cec5c): const Cmd14 data_ov257_020d3320; */
const u16 data_ov257_020d3320[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 773, 0, 0, 0, 0, 0,
};

/* read by Ov257_FinishingStrikeTick (020d167c): const Cmd14 data_ov257_020d332e; */
const u16 data_ov257_020d332e[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 2565, 0, 0, 0, 0, 0,
};

/* read by Ov257_LandingSlamTick (020d1f00): const Cmd14 data_ov257_020d333c; */
const u8 data_ov257_020d333c[16] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 5, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

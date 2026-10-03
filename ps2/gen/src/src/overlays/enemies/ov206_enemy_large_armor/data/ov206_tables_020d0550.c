/* PS2: mechanically prepared copy of src/overlays/enemies/ov206_enemy_large_armor/data/ov206_tables_020d0550.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov206 .rodata tables, 0x020d0550-0x020d065c.
 *
 * 14 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by Ov206_Construct (020cbfc4): struct Ov206Kinds data_ov206_020d0550; */

#include "nitro/types.h"

const int data_ov206_020d0550[5] __attribute__((aligned(__alignof__(int)))) = {
    18, 19, 21, 22, 23,
};

/* read by * Ov206_MessageArmEmitterTable -- x4. Message handler: on kinds 6 and 0xa, (re-)arm the emitter, th (020cc8c8): struct t17 data_ov206_020d0564; */
const int data_ov206_020d0564[17] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 3, 4, 5, 6, 7, 8,
    9, 10, 11, 12, 13, 14, 15, 16,
    17,
};

/* read by reset the tuning, quiesce the owner and queue the effect, then hand on. (020ce328): unsigned short data_ov206_020d05a8[]; */
const u8 data_ov206_020d05a8[8] __attribute__((aligned(__alignof__(u8)))) = {
    184, 3, 0, 0, 0, 0, 5, 0,
};

/* read by Ov206_RockFlightTick (not yet decompiled) */
const u16 data_ov206_020d05b0[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 1285, 0, 0, 0, 0, 0,
};

/* read by Ov206_RockFlightTick (not yet decompiled) */
const u16 data_ov206_020d05be[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov206_JumpWindupTick (020cf688): const PosMsg data_ov206_020d05cc; */
const u16 data_ov206_020d05cc[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 1029, 0, 0, 0, 0, 0,
};

/* read by Ov206_StompTick (020cf0e4): const PosMsg data_ov206_020d05da; */
const u16 data_ov206_020d05da[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov206_LandingTick (020ce898): const PosMsg data_ov206_020d05e8; */
const u16 data_ov206_020d05e8[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov206_StompTick (020cf0e4): const PosMsg data_ov206_020d05f6; */
const u16 data_ov206_020d05f6[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov206_LandingTick (020ce898): const PosMsg data_ov206_020d0604; */
const u16 data_ov206_020d0604[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov206_ChargeTick (020ce470): const PosMsg data_ov206_020d0612; */
const u16 data_ov206_020d0612[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov206_RockFlightTick (not yet decompiled) */
const u16 data_ov206_020d0620[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov206_ShockwaveTick (020d01a4): const PosMsg data_ov206_020d062e; */
const u16 data_ov206_020d062e[7] __attribute__((aligned(__alignof__(u16)))) = {
    0, 261, 0, 0, 0, 0, 0,
};

/* read by Ov206_collectObjectsInSphereRec (020ccef4): const struct tbl8 data_ov206_020d063c; */
const u8 data_ov206_020d063c[32] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 255, 255, 255, 255, 255, 255, 1, 0, 0, 0, 255, 255, 255, 255,
    255, 255, 255, 255, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
};

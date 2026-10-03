/* PS2: mechanically prepared copy of src/overlays/scenes/ov006_mission_mode_select/data/ov006_tables_020561c0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov006 .rodata tables, 0x020561c0-0x0205628c.
 *
 * 12 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 */

/* read by For each variable-stride record, build a sprite via Ov006_AddElem (kind-mapped prior (0204cb00): unsigned char data_ov006_020561c0; */

#include "nitro/types.h"

const u8 data_ov006_020561c0[4] __attribute__((aligned(__alignof__(u8)))) = {
    8, 9, 10, 11,
};

/* read by For each variable-stride record, build a sprite via Ov006_AddElem (kind-mapped prior (0204cb00): unsigned char data_ov006_020561c4; */
const u8 data_ov006_020561c4[4] __attribute__((aligned(__alignof__(u8)))) = {
    24, 25, 26, 27,
};

/* read by Ov006_MissionExpireRows (0204eda4): u8 data_ov006_020561c8[]; */
const u8 data_ov006_020561c8[8] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 255, 255, 255, 255, 0, 0,
};

/* read by Ov006_CreateMissionMenu (020500c4): u16 data_ov006_020561d0[];
 *   per-frame update of the Mission Mode member menu screen, ov006. (02050fc8): u8 data_ov006_020561d0[];
 *   Ov006_UpdateMissionMemberSelectionInput (02051b34): MissionMenuRow data_ov006_020561d0; */
const u8 data_ov006_020561d0[16] __attribute__((aligned(__alignof__(u8)))) = {
    4, 5, 7, 255, 255, 255, 6, 1, 3, 2, 15, 0, 8, 0, 0, 0,
};

/* read by Ov006_MissionApplyParameterRow (0204ff34): u8 data_ov006_020561e0[]; */
const u8 data_ov006_020561e0[1] __attribute__((aligned(__alignof__(u8)))) = {
    8,
};

/* read by Ov006_MissionApplyParameterRow (0204ff34): u8 data_ov006_020561e1[]; */
const u8 data_ov006_020561e1[1] __attribute__((aligned(__alignof__(u8)))) = {
    8,
};

/* read by Ov006_MissionApplyParameterRow (0204ff34): u8 data_ov006_020561e2[]; */
const u8 data_ov006_020561e2[1] __attribute__((aligned(__alignof__(u8)))) = {
    4,
};

/* read by Ov006_MissionApplyParameterRow (0204ff34): u8 data_ov006_020561e3[]; */
const u8 data_ov006_020561e3[1] __attribute__((aligned(__alignof__(u8)))) = {
    4,
};

/* read by Ov006_MissionApplyParameterRow (0204ff34): u8 data_ov006_020561e4[]; */
const u8 data_ov006_020561e4[3] __attribute__((aligned(__alignof__(u8)))) = {
    6, 1, 5,
};

/* read by Ov006_MissionApplyParameterRow (0204ff34): u8 data_ov006_020561e7[]; */
const u8 data_ov006_020561e7[1] __attribute__((aligned(__alignof__(u8)))) = {
    5,
};

/* read by Ov006_MissionApplyParameterRow (0204ff34): u8 data_ov006_020561e8[]; */
const u8 data_ov006_020561e8[56] __attribute__((aligned(__alignof__(u8)))) = {
    5, 3, 3, 3, 1, 4, 6, 5, 10, 2, 6, 4, 5, 3, 4, 4,
    5, 5, 4, 3, 8, 4, 6, 4, 5, 5, 3, 2, 10, 1, 10, 2,
    10, 7, 5, 1, 8, 2, 9, 1, 1, 1, 6, 5, 7, 10, 6, 3,
    4, 10, 7, 4, 5, 4, 10, 6,
};

const u8 data_ov006_02056220[108] __attribute__((aligned(__alignof__(u8)))) = {
    5, 4, 5, 5, 6, 5, 6, 5, 4, 7, 3, 4, 4, 7, 5, 5,
    3, 4, 6, 5, 4, 4, 5, 5, 5, 5, 7, 6, 3, 7, 6, 8,
    5, 3, 7, 3, 7, 2, 3, 3, 5, 5, 10, 1, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 8,
    4, 8, 3, 10, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 3, 5, 6, 6, 5, 5, 5, 8, 1, 10, 1, 4, 1, 5, 5,
    4, 3, 5, 1, 8, 4, 6, 5, 5, 5, 5, 0,
};

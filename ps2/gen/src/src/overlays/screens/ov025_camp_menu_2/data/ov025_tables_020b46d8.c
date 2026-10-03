/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_tables_020b46d8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .rodata tables, 0x020b46d8-0x020b4968.
 *
 * 15 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes, followed by the four character tag "KPTS" (two bytes of
 * padding) of the report detail byte tables data_ov025_020b4960 .. 020b4968,
 * which are indexed by the mode; the tag shares this unit because it sits
 * at a 2-byte offset and mwcc keeps byte objects packed inside one section.
 *
 * Readers:
 *   data_ov025_020b46d8: Ov025_MissionMenu_DrawTags
 *   data_ov025_020b46e8: Ov025_MissionMenuDestroy
 *   data_ov025_020b46fc: Ov025_InitMissionMenuSurfaces
 *   data_ov025_020b4724: Ov025_InitMissionMenuSurfaces
 *   data_ov025_020b474c: Ov025_InitMissionMenuSurfaces
 *   data_ov025_020b4774: Ov025_InitMissionMenuSurfaces
 *   data_ov025_020b479c: Ov025_InitMissionMenuSurfaces
 *   data_ov025_020b47c4: Ov025_InitMissionMenuSurfaces
 *   data_ov025_020b47ec: Ov025_MissionMenu_DrawTags
 *   data_ov025_020b481c: Ov025_ScrollList_SetupSurfaces
 *   data_ov025_020b4844: Ov025_InitTutorialSurfaces
 *   data_ov025_020b495c: Ov025_ReportDetail_LoadBackground, Ov025_ReportDetail_SetupEntries
 *   data_ov025_020b495d: (no C reader yet)
 *   data_ov025_020b4960: Ov025_ReportDetail_LoadBackground, Ov025_ReportDetail_SetupEntries
 *   data_ov025_020b4961: (no C reader yet)
 *   gOv025KptsName: (no C reader yet)
 */

#include "nitro/types.h"

const int data_ov025_020b46d8[4] __attribute__((aligned(__alignof__(int)))) = {
    47, 44, 45, 46,
};

const int data_ov025_020b46e8[5] __attribute__((aligned(__alignof__(int)))) = {
    1, 2, 11, 12, 13,
};

const int data_ov025_020b46fc[10] __attribute__((aligned(__alignof__(int)))) = {
    1, 24, 2, 2, 352, 8, 0, 20,
    0, 32,
};

const int data_ov025_020b4724[10] __attribute__((aligned(__alignof__(int)))) = {
    1, 21, 5, 2, 352, 8, 0, 20,
    0, 32,
};

const int data_ov025_020b474c[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 2, 25, 30, 1, 0, 0, 22,
    0, 32,
};

const int data_ov025_020b4774[10] __attribute__((aligned(__alignof__(int)))) = {
    22, 0, 32, 2, 352, 8, 0, 20,
    0, 32,
};

const int data_ov025_020b479c[10] __attribute__((aligned(__alignof__(int)))) = {
    3, 5, 20, 2, 352, 8, 0, 20,
    0, 32,
};

const int data_ov025_020b47c4[10] __attribute__((aligned(__alignof__(int)))) = {
    1, 5, 18, 2, 352, 8, 0, 20,
    0, 32,
};

const int data_ov025_020b47ec[12] __attribute__((aligned(__alignof__(int)))) = {
    1100, 1105, 1104, 1101, 1103, 1106, 1100, 1100,
    1107, 1102, 1100, 1100,
};

const int data_ov025_020b481c[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 10, 17, 2, 288, 8, 0, 22,
    0, 32,
};

const int data_ov025_020b4844[70] __attribute__((aligned(__alignof__(int)))) = {
    0, 2, 22, 3, 1, 15, 0, 23,
    0, 32, 0, 25, 6, 3, 67, 15,
    0, 23, 0, 32, 3, 2, 28, 4,
    85, 15, 0, 23, 0, 32, 20, 2,
    28, 3, 197, 15, 0, 23, 0, 32,
    8, 18, 13, 4, 197, 15, 0, 23,
    0, 32, 13, 18, 13, 4, 249, 15,
    0, 23, 0, 32, 19, 18, 13, 4,
    301, 15, 0, 23, 0, 32,
};

const u8 data_ov025_020b495c[1] __attribute__((aligned(__alignof__(u8)))) = {
    3,
};

const u8 data_ov025_020b495d[3] __attribute__((aligned(__alignof__(u8)))) = {
    4, 4, 6,
};

const u8 data_ov025_020b4960[1] __attribute__((aligned(__alignof__(u8)))) = {
    72,
};

const u8 data_ov025_020b4961[1] __attribute__((aligned(__alignof__(u8)))) = {
    76,
};

const char gOv025KptsName[6] __attribute__((aligned(__alignof__(char)))) = "KPTS";

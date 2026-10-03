/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_tables_020b3cbc.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .rodata tables, 0x020b3cbc-0x020b4220.
 *
 * 31 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov025_020b3cbc: Ov025_InitGridMenuWidgets
 *   data_ov025_020b3ce0: Ov025_DrawMenuEntry
 *   data_ov025_020b3cf8: Ov025_ShowGridPage
 *   data_ov025_020b3d10: Ov025_HideGridMenu
 *   data_ov025_020b3d2c: Ov025_FinishMenuModeSwitch
 *   data_ov025_020b3d68: Ov025_BuildInventoryLists
 *   data_ov025_020b3da8: Ov025_InitGridMenuSurfaces
 *   data_ov025_020b3dd0: Ov025_InitGridMenuSurfaces
 *   data_ov025_020b3df8: Ov025_InitGridMenuSurfaces
 *   data_ov025_020b3e20: Ov025_SetSavePageGroupVisible
 *   data_ov025_020b3e48: Ov025_Menu_ChangePage
 *   data_ov025_020b3e88: Ov025_GetPlayerSlotConfig
 *   data_ov025_020b3ed8: Ov025_DrawTextRow
 *   data_ov025_020b3f28: (no C reader yet)
 *   data_ov025_020b3f80: Ov025_InitGridMenuSurfaces
 *   data_ov025_020b4048: Ov025_SetMenuEntriesVisible, Ov025_SaveMenu_SlideArrows
 *   data_ov025_020b4050: Ov025_SetMenuEntriesVisible
 *   data_ov025_020b4060: Ov025_SaveMenu_BuildLayout
 *   data_ov025_020b4070: Ov025_SaveMenu_BuildTextSurfaces
 *   data_ov025_020b4098: Ov025_SaveMenu_BuildTextSurfaces
 *   data_ov025_020b40c0: Ov025_SaveMenu_BuildTextSurfaces
 *   data_ov025_020b40e8: Ov025_SaveMenu_BuildLayout, Ov025_TickPageScroll, Ov025_RefreshSaveRowDigits, Ov025_SaveMenu_RefreshRows
 *   data_ov025_020b4148: Ov025_Config_Setup
 *   data_ov025_020b4158: Ov025_DrawMenuPageTexts
 *   data_ov025_020b4178: Ov025_SetupMenuSurface
 *   data_ov025_020b41a0: Ov025_Tutorial_HandleTouch
 *   data_ov025_020b41a4: Ov025_Tutorial_SetupEntries
 *   data_ov025_020b41b4: Ov025_Tutorial_SetupSurfaces
 *   data_ov025_020b41dc: (no C reader yet)
 *   data_ov025_020b4218: Ov025_Reports_SetupEntries
 *   data_ov025_020b421c: Ov025_Reports_HandleInput
 */

#include "nitro/types.h"

const int data_ov025_020b3cbc[9] __attribute__((aligned(__alignof__(int)))) = {
    0, 1, 0, 0, 2, 3, 4, 14,
    15,
};

const int data_ov025_020b3ce0[6] __attribute__((aligned(__alignof__(int)))) = {
    12, 13, 19, 20, 21, 22,
};

const int data_ov025_020b3cf8[6] __attribute__((aligned(__alignof__(int)))) = {
    63, 64, 65, 60, 61, 62,
};

const int data_ov025_020b3d10[7] __attribute__((aligned(__alignof__(int)))) = {
    41, 42, 43, 44, 45, 46, 47,
};

const int data_ov025_020b3d2c[15] __attribute__((aligned(__alignof__(int)))) = {
    41, 42, 43, 44, 45, 46, 47, 7,
    0, 1, 4, 3, 5, 6, 2,
};

const int data_ov025_020b3d68[16] __attribute__((aligned(__alignof__(int)))) = {
    7, 0, 3, 1, 5, 4, 6, 2,
    7, 0, 3, 1, 5, 4, 6, 2,
};

const int data_ov025_020b3da8[10] __attribute__((aligned(__alignof__(int)))) = {
    3, 13, 16, 24, 352, 15, 0, 6,
    0, 64,
};

const int data_ov025_020b3dd0[10] __attribute__((aligned(__alignof__(int)))) = {
    19, 0, 32, 4, 224, 15, 0, 7,
    0, 64,
};

const int data_ov025_020b3df8[10] __attribute__((aligned(__alignof__(int)))) = {
    4, 14, 16, 10, 800, 15, 0, 5,
    0, 64,
};

const int data_ov025_020b3e20[10] __attribute__((aligned(__alignof__(int)))) = {
    5, 6, 40, 41, 42, 43, 44, 45,
    46, 47,
};

const int data_ov025_020b3e48[16] __attribute__((aligned(__alignof__(int)))) = {
    40, 41, 42, 43, 44, 45, 46, 47,
    20, 21, 22, 23, 24, 25, 26, 27,
};

const int data_ov025_020b3e88[20] __attribute__((aligned(__alignof__(int)))) = {
    10, 0, 16, 11, 14, 12, 1, 4,
    5, 6, 7, 9, 13, 15, 17, 18,
    8, 2, 3, 10,
};

const u8 data_ov025_020b3ed8[80] __attribute__((aligned(__alignof__(u8)))) = {
    3, 0, 0, 0, 16, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255,
    2, 0, 0, 0, 17, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255,
    5, 0, 0, 0, 18, 0, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255,
    6, 0, 0, 0, 14, 0, 0, 0, 5, 0, 0, 0, 255, 255, 255, 255,
    4, 0, 0, 0, 15, 0, 0, 0, 2, 0, 0, 0, 255, 255, 255, 255,
};

const u8 data_ov025_020b3f28[88] __attribute__((aligned(__alignof__(u8)))) = {
    12, 0, 0, 0, 7, 0, 0, 0, 1, 0, 0, 0, 6, 0, 0, 0,
    2, 0, 0, 0, 12, 0, 0, 0, 8, 0, 0, 0, 11, 0, 0, 0,
    4, 0, 0, 0, 9, 0, 0, 0, 10, 0, 0, 0, 255, 255, 255, 255,
    3, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 5, 0, 0, 0,
    12, 0, 0, 0, 12, 0, 0, 0, 12, 0, 0, 0, 12, 0, 0, 0,
    12, 0, 0, 0, 12, 0, 0, 0,
};

const int data_ov025_020b3f80[50] __attribute__((aligned(__alignof__(int)))) = {
    4, 15, 14, 2, 800, 15, 0, 5,
    0, 64, 7, 15, 14, 2, 828, 15,
    0, 5, 0, 64, 10, 15, 14, 2,
    856, 15, 0, 5, 0, 64, 13, 15,
    14, 2, 968, 15, 0, 5, 0, 64,
    16, 15, 14, 2, 996, 15, 0, 5,
    0, 64,
};

const int data_ov025_020b4048[2] __attribute__((aligned(__alignof__(int)))) = {
    20, 21,
};

const int data_ov025_020b4050[4] __attribute__((aligned(__alignof__(int)))) = {
    14, 16, 15, 61,
};

const int data_ov025_020b4060[4] __attribute__((aligned(__alignof__(int)))) = {
    0, 1, 0, 0,
};

const int data_ov025_020b4070[10] __attribute__((aligned(__alignof__(int)))) = {
    19, 0, 32, 4, 453, 15, 0, 5,
    0, 32,
};

const int data_ov025_020b4098[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 14, 18, 2, 1, 15, 0, 5,
    0, 32,
};

const int data_ov025_020b40c0[10] __attribute__((aligned(__alignof__(int)))) = {
    4, 0, 32, 13, 37, 15, 0, 5,
    0, 32,
};

const int data_ov025_020b40e8[24] __attribute__((aligned(__alignof__(int)))) = {
    1, 31, 34, 37, 40, 50, 51, 52,
    2, 32, 35, 38, 41, 53, 54, 55,
    3, 33, 36, 39, 42, 56, 57, 58,
};

const int data_ov025_020b4148[4] __attribute__((aligned(__alignof__(int)))) = {
    0, 1, 0, 0,
};

const u8 data_ov025_020b4158[32] __attribute__((aligned(__alignof__(u8)))) = {
    182, 255, 255, 255, 2, 0, 0, 0, 7, 0, 0, 0, 2, 0, 0, 0,
    204, 255, 255, 255, 2, 0, 0, 0, 7, 0, 0, 0, 2, 0, 0, 0,
};

const int data_ov025_020b4178[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 32, 24, 0, 15, 0, 5,
    0, 32,
};

const u8 data_ov025_020b41a0[4] __attribute__((aligned(__alignof__(u8)))) = {
    224, 32, 16, 144,
};

const int data_ov025_020b41a4[4] __attribute__((aligned(__alignof__(int)))) = {
    0, 1, 0, 0,
};

const int data_ov025_020b41b4[10] __attribute__((aligned(__alignof__(int)))) = {
    0, 0, 32, 24, 0, 15, 0, 5,
    0, 32,
};

const u8 data_ov025_020b41dc[60] __attribute__((aligned(__alignof__(u8)))) = {
    0, 1, 15, 2, 29, 5, 3, 4, 6, 7, 8, 37, 42, 17, 18, 43,
    9, 10, 11, 12, 13, 14, 16, 25, 20, 21, 22, 24, 30, 26, 27, 40,
    33, 38, 56, 57, 44, 53, 55, 46, 49, 51, 50, 28, 31, 47, 32, 34,
    35, 39, 41, 45, 23, 36, 48, 52, 54, 19, 58, 0,
};

const int data_ov025_020b4218[1] __attribute__((aligned(__alignof__(int)))) = {
    1797,
};

const u8 data_ov025_020b421c[4] __attribute__((aligned(__alignof__(u8)))) = {
    232, 16, 16, 160,
};

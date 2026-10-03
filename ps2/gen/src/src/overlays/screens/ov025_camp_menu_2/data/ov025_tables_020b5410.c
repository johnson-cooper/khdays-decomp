/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_tables_020b5410.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .data tables, 0x020b5410-0x020b5474.
 *
 * 7 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov025_020b5410: Ov025_MissionMenu_DrawInfo
 *   data_ov025_020b5414: Ov025_MissionMenu_DrawInfo
 *   data_ov025_020b5438: Ov025_MissionMenu_DrawInfo
 *   data_ov025_020b5444: Ov025_MissionMenu_DrawInfo
 *   data_ov025_020b545c: Ov025_MissionMenu_BuildRewardRows
 *   data_ov025_020b5460: Ov025_MissionMenu_BuildRewardRows
 *   data_ov025_020b5464: Ov025_DrawMissionDetail
 */

#include "nitro/types.h"

int data_ov025_020b5410[1] __attribute__((aligned(__alignof__(int)))) = {
    10,
};

u8 data_ov025_020b5414[36] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 115, 0, 32, 0, 37, 0, 48, 0, 50, 0, 100, 0, 58, 0,
    37, 0, 48, 0, 50, 0, 100, 0, 58, 0, 37, 0, 48, 0, 50, 0,
    100, 0, 0, 0,
};

u8 data_ov025_020b5438[12] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 115, 0, 32, 0, 37, 0, 100, 0, 0, 0,
};

u8 data_ov025_020b5444[24] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 115, 0, 32, 0, 45, 0, 45, 0, 45, 0, 45, 0, 45, 0,
    45, 0, 45, 0, 45, 0, 0, 0,
};

int data_ov025_020b545c[1] __attribute__((aligned(__alignof__(int)))) = {
    32,
};

int data_ov025_020b5460[1] __attribute__((aligned(__alignof__(int)))) = {
    47,
};

u8 data_ov025_020b5464[16] __attribute__((aligned(__alignof__(u8)))) = {
    37, 0, 48, 0, 50, 0, 100, 0, 37, 0, 99, 0, 0, 0, 0, 0,
};

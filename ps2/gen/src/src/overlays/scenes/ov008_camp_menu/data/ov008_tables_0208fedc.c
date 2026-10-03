/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_tables_0208fedc.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .rodata tables, 0x0208fedc-0x0208ff6c.
 *
 * 10 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov008_0208fedc: Ov008_InitFilterRows, Ov008_ShowFilterRow, Ov008_ShopTabSelectTick
 *   data_ov008_0208fedd: Ov008_ShowFilterRow
 *   data_ov008_0208fef0: Ov008_InitPanelSlotManagers
 *   data_ov008_0208fef1: Ov008_InitPanelSlotManagers
 *   data_ov008_0208fef2: Ov008_LoadShopResources
 *   data_ov008_0208fef3: Ov008_InitPanelSlotManagers
 *   data_ov008_0208ff04: Ov008_FlushDirtyVramBanks
 *   data_ov008_0208ff20: Ov008_DrawCounterPanel
 *   data_ov008_0208ff3c: Ov008_ShopListInput
 *   data_ov008_0208ff64: Ov008_MergePendingUnlockBits
 */

#include "nitro/types.h"

const u8 data_ov008_0208fedc[1] __attribute__((aligned(__alignof__(u8)))) = {
    80,
};

const u8 data_ov008_0208fedd[19] __attribute__((aligned(__alignof__(u8)))) = {
    24, 96, 16, 80, 48, 96, 16, 80, 72, 96, 16, 80, 96, 96, 16, 80,
    120, 96, 16,
};

const u8 data_ov008_0208fef0[1] __attribute__((aligned(__alignof__(u8)))) = {
    0,
};

const u8 data_ov008_0208fef1[1] __attribute__((aligned(__alignof__(u8)))) = {
    4,
};

const u8 data_ov008_0208fef2[1] __attribute__((aligned(__alignof__(u8)))) = {
    9,
};

const u8 data_ov008_0208fef3[17] __attribute__((aligned(__alignof__(u8)))) = {
    13, 255, 5, 255, 14, 1, 6, 10, 15, 2, 7, 11, 16, 3, 8, 12,
    17,
};

const int data_ov008_0208ff04[7] __attribute__((aligned(__alignof__(int)))) = {
    9, 10, 11, 24, 25, 26, 27,
};

const int data_ov008_0208ff20[7] __attribute__((aligned(__alignof__(int)))) = {
    20, 22, 25, 23, 21, 24, 26,
};

const u8 data_ov008_0208ff3c[40] __attribute__((aligned(__alignof__(u8)))) = {
    32, 0, 24, 20, 56, 0, 24, 20, 80, 0, 24, 20, 104, 0, 24, 20,
    128, 0, 24, 20, 152, 0, 24, 20, 176, 0, 24, 20, 200, 0, 24, 20,
    16, 0, 40, 20, 56, 0, 40, 20,
};

const int data_ov008_0208ff64[2] __attribute__((aligned(__alignof__(int)))) = {
    32, 32,
};

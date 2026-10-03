/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_tables_0208fdc8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .rodata tables, 0x0208fdc8-0x0208fec8.
 *
 * 12 contiguous tables, each written in the width its contents are in:
 * words where the values are small integers, bytes where the words are
 * packed bytes.
 *
 * Readers:
 *   data_ov008_0208fdc8: Ov008_MissionBuildScreenCells, Ov008_MissionPlaceCellPair
 *   data_ov008_0208fe60: Ov008_RebuildShopList
 *   data_ov008_0208fe64: Ov008_Shop_QuantityDialogTick
 *   data_ov008_0208fe6c: Ov008_ShopListInput
 *   data_ov008_0208fe74: Ov008_OpenShopDetailPanel, Ov008_ShopDetailConfirmTick
 *   data_ov008_0208fe78: Ov008_ShopDetailConfirmTick
 *   data_ov008_0208fe7c: Ov008_ShopListInput
 *   data_ov008_0208fe84: Ov008_Shop_QuantityDialogTick
 *   data_ov008_0208fe88: Ov008_Shop_QuantityDialogTick
 *   data_ov008_0208fe98: Ov008_ShopTabSelectTick
 *   data_ov008_0208fea7: Ov008_Shop_QuantityDialogTick
 *   data_ov008_0208feb8: Ov008_RebuildShopList
 */

#include "nitro/types.h"

const int data_ov008_0208fdc8[38] __attribute__((aligned(__alignof__(int)))) = {
    32, 64, 56, 64, 80, 64, 104, 64,
    128, 64, 152, 64, 176, 64, 200, 64,
    224, 64, 20, 128, 44, 128, 68, 128,
    92, 128, 116, 128, 140, 128, 164, 128,
    188, 128, 212, 128, 236, 128,
};

const u8 data_ov008_0208fe60[4] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 1, 0,
};

const u8 data_ov008_0208fe64[8] __attribute__((aligned(__alignof__(u8)))) = {
    32, 0, 64, 0, 16, 0, 128, 0,
};

const u8 data_ov008_0208fe6c[8] __attribute__((aligned(__alignof__(u8)))) = {
    40, 24, 160, 16, 24, 24, 192, 16,
};

const u8 data_ov008_0208fe74[4] __attribute__((aligned(__alignof__(u8)))) = {
    32, 104, 96, 16,
};

const u8 data_ov008_0208fe78[4] __attribute__((aligned(__alignof__(u8)))) = {
    128, 104, 96, 16,
};

const u8 data_ov008_0208fe7c[8] __attribute__((aligned(__alignof__(u8)))) = {
    200, 24, 16, 128, 216, 24, 16, 128,
};

const u8 data_ov008_0208fe84[4] __attribute__((aligned(__alignof__(u8)))) = {
    32, 117, 96, 16,
};

const u8 data_ov008_0208fe88[16] __attribute__((aligned(__alignof__(u8)))) = {
    128, 117, 96, 16, 255, 255, 0, 0, 1, 0, 5, 0, 4, 0, 6, 0,
};

const u8 data_ov008_0208fe98[15] __attribute__((aligned(__alignof__(u8)))) = {
    0, 0, 1, 4, 4, 0, 0, 1, 2, 4, 0, 1, 2, 3, 4,
};

const u8 data_ov008_0208fea7[17] __attribute__((aligned(__alignof__(u8)))) = {
    80, 88, 16, 16, 104, 76, 16, 16, 120, 88, 16, 16, 101, 100, 16, 16,
    0,
};

const u8 data_ov008_0208feb8[16] __attribute__((aligned(__alignof__(u8)))) = {
    255, 255, 0, 0, 3, 0, 1, 0, 5, 0, 4, 0, 6, 0, 2, 0,
};

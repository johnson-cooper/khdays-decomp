/* PS2: mechanically prepared copy of libs/nns/g2d/auto/g2d_screen_tables.c (ps2/tools/prep_sources.py). Do not edit. */
/* NitroSystem g2d_Screen.c: the BG screen-size maps and the BG-mode selection tables
 * (NNS_G2dBGSetup / NNS_G2dBGSetupEx and their helpers), plus the BG control register offsets
 * GetBGExtPlttSlot indexes. Laid out in the ROM's order (the linker sorted the unit's const
 * objects by size). */

#include "nitro/types.h"
#include "nitro/hw.h"

typedef struct ScreenSizeMap {
    u16 width;
    u16 height;
    u16 scnSize;
} ScreenSizeMap;

/* GXBGMode */
#define GX_BGMODE_0 0
#define GX_BGMODE_1 1
#define GX_BGMODE_2 2
#define GX_BGMODE_3 3
#define GX_BGMODE_4 4
#define GX_BGMODE_5 5
#define GX_BGMODE_6 6
#define BG_MODE_WARNING 8

/* GXBGScrSizeText / GXBGScrSizeAffine / GXBGScrSize256x16Pltt */
#define GX_BG_SCRSIZE_TEXT_256x256 0
#define GX_BG_SCRSIZE_TEXT_512x256 1
#define GX_BG_SCRSIZE_TEXT_256x512 2
#define GX_BG_SCRSIZE_TEXT_512x512 3
#define GX_BG_SCRSIZE_AFFINE_128x128 0
#define GX_BG_SCRSIZE_AFFINE_256x256 1
#define GX_BG_SCRSIZE_AFFINE_512x512 2
#define GX_BG_SCRSIZE_AFFINE_1024x1024 3
#define GX_BG_SCRSIZE_256x16PLTT_128x128 0
#define GX_BG_SCRSIZE_256x16PLTT_256x256 1
#define GX_BG_SCRSIZE_256x16PLTT_512x512 2
#define GX_BG_SCRSIZE_256x16PLTT_1024x1024 3

/* sBG256x16PlttModeTable[2][8] */
const u8 data_020419f4[2][8] __attribute__((aligned(__alignof__(u8)))) = {
    {
        GX_BGMODE_5 + BG_MODE_WARNING,
        GX_BGMODE_5 + BG_MODE_WARNING,
        GX_BGMODE_5 + BG_MODE_WARNING,
        GX_BGMODE_5,
        GX_BGMODE_5,
        GX_BGMODE_5,
        GX_BGMODE_5,
        GX_BGMODE_5 + BG_MODE_WARNING,
    },
    {
        GX_BGMODE_3,
        GX_BGMODE_3,
        GX_BGMODE_4,
        GX_BGMODE_3,
        GX_BGMODE_4,
        GX_BGMODE_5,
        GX_BGMODE_3 + BG_MODE_WARNING,
        GX_BGMODE_3 + BG_MODE_WARNING,
    }
};

/* GetBGExtPlttSlot's addrTable[]: the BGnCNT register offsets of the extended-palette BGs */
const u16 data_02041a04[8] __attribute__((aligned(__alignof__(u16)))) = {
    REG_BG0CNT_OFFSET,
    REG_BG1CNT_OFFSET,
    0,
    0,
    REG_DB_BG0CNT_OFFSET,
    REG_DB_BG1CNT_OFFSET,
    0,
    0
};

/* sBGAffineModeTable[2][8] */
const u8 data_02041a14[2][8] __attribute__((aligned(__alignof__(u8)))) = {
    {
        GX_BGMODE_2 + BG_MODE_WARNING,
        GX_BGMODE_2,
        GX_BGMODE_2,
        GX_BGMODE_4,
        GX_BGMODE_4,
        GX_BGMODE_4,
        GX_BGMODE_4,
        GX_BGMODE_2 + BG_MODE_WARNING,
    },
    {
        GX_BGMODE_1,
        GX_BGMODE_1,
        GX_BGMODE_2,
        GX_BGMODE_1,
        GX_BGMODE_2,
        GX_BGMODE_1 + BG_MODE_WARNING,
        GX_BGMODE_1 + BG_MODE_WARNING,
        GX_BGMODE_1 + BG_MODE_WARNING,
    }
};

/* sAffineExtScnSize[4] */
const ScreenSizeMap data_02041a24[4] __attribute__((aligned(__alignof__(ScreenSizeMap)))) = {
    {  128,  128, GX_BG_SCRSIZE_256x16PLTT_128x128 },
    {  256,  256, GX_BG_SCRSIZE_256x16PLTT_256x256 },
    {  512,  512, GX_BG_SCRSIZE_256x16PLTT_512x512 },
    { 1024, 1024, GX_BG_SCRSIZE_256x16PLTT_1024x1024 },
};

/* sAffineScnSize[4] */
const ScreenSizeMap data_02041a3c[4] __attribute__((aligned(__alignof__(ScreenSizeMap)))) = {
    {  128,  128, GX_BG_SCRSIZE_AFFINE_128x128 },
    {  256,  256, GX_BG_SCRSIZE_AFFINE_256x256 },
    {  512,  512, GX_BG_SCRSIZE_AFFINE_512x512 },
    { 1024, 1024, GX_BG_SCRSIZE_AFFINE_1024x1024 },
};

/* sTextScnSize[4] */
const ScreenSizeMap data_02041a54[4] __attribute__((aligned(__alignof__(ScreenSizeMap)))) = {
    { 256, 256, GX_BG_SCRSIZE_TEXT_256x256 },
    { 256, 512, GX_BG_SCRSIZE_TEXT_256x512 },
    { 512, 256, GX_BG_SCRSIZE_TEXT_512x256 },
    { 512, 512, GX_BG_SCRSIZE_TEXT_512x512 },
};

/* sBGTextModeTable[4][8] */
const u8 data_02041a6c[4][8] __attribute__((aligned(__alignof__(u8)))) = {
    {
        GX_BGMODE_0,
        GX_BGMODE_1,
        GX_BGMODE_2,
        GX_BGMODE_3,
        GX_BGMODE_4,
        GX_BGMODE_5,
        GX_BGMODE_6,
        BG_MODE_WARNING,
    },
    {
        GX_BGMODE_0,
        GX_BGMODE_1,
        GX_BGMODE_2,
        GX_BGMODE_3,
        GX_BGMODE_4,
        GX_BGMODE_5,
        GX_BGMODE_6,
        BG_MODE_WARNING,
    },
    {
        GX_BGMODE_0,
        GX_BGMODE_1,
        GX_BGMODE_1,
        GX_BGMODE_3,
        GX_BGMODE_3,
        GX_BGMODE_3,
        GX_BGMODE_0,
        GX_BGMODE_0 + BG_MODE_WARNING,
    },
    {
        GX_BGMODE_0,
        GX_BGMODE_0,
        GX_BGMODE_0 + BG_MODE_WARNING,
        GX_BGMODE_0,
        GX_BGMODE_0 + BG_MODE_WARNING,
        GX_BGMODE_0 + BG_MODE_WARNING,
        GX_BGMODE_0 + BG_MODE_WARNING,
        GX_BGMODE_0 + BG_MODE_WARNING,
    }
};

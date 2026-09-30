/* PS2: mechanically prepared copy of libs/nitro/nns/calls/func_02012e1c.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"

typedef enum GXTexFmt {
    GX_TEXFMT_NONE = 0,
    GX_TEXFMT_A3I5 = 1,
    GX_TEXFMT_PLTT4 = 2,
    GX_TEXFMT_PLTT16 = 3,
    GX_TEXFMT_PLTT256 = 4
} GXTexFmt;

typedef enum NNSG2dBGSelect {
    NNS_G2D_BGSELECT_MAIN0,
    NNS_G2D_BGSELECT_MAIN1,
    NNS_G2D_BGSELECT_MAIN2,
    NNS_G2D_BGSELECT_MAIN3,
    NNS_G2D_BGSELECT_SUB0,
    NNS_G2D_BGSELECT_SUB1,
    NNS_G2D_BGSELECT_SUB2,
    NNS_G2D_BGSELECT_SUB3
} NNSG2dBGSelect;

typedef struct NNSG2dPaletteData {
    GXTexFmt fmt;
    BOOL bExtendedPlt;
    u32 szByte;
    void *pRawData;
} NNSG2dPaletteData;

typedef struct NNSG2dScreenData {
    u16 screenWidth;
    u16 screenHeight;
    u16 colorMode;
    u16 screenFormat;
    u32 szByte;
    u32 rawData[1];
} NNSG2dScreenData;

typedef struct NNSG2dPaletteCompressInfo {
    u16 numPalette;
    u16 pad16;
    void *pPlttIdxTbl;
} NNSG2dPaletteCompressInfo;

extern void LoadBGPaletteSelect(NNSG2dBGSelect bg, BOOL useExtended,
                          const NNSG2dPaletteData *pPltData,
                          const NNSG2dPaletteCompressInfo *pCmpInfo);

static inline BOOL IsMainBG(NNSG2dBGSelect bg)
{
    return bg <= NNS_G2D_BGSELECT_MAIN3;
}

static inline BOOL IsBGUseExtPlttMain(void)
{
    return (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0) & 0x40000000) != 0;
}

static inline BOOL IsBGUseExtPlttSub(void)
{
    return (*(volatile u32 *)((unsigned int)kh_ds_io + 0x1000) & 0x40000000) != 0;
}

static inline BOOL IsBGUseExtPltt(NNSG2dBGSelect bg)
{
    return IsMainBG(bg) ? IsBGUseExtPlttMain() : IsBGUseExtPlttSub();
}

void func_02012e1c(NNSG2dBGSelect bg,
                   const NNSG2dPaletteData *pPltData,
                   const NNSG2dScreenData *pScreenData,
                   const NNSG2dPaletteCompressInfo *pCmpInfo)
{
    const u16 screenFormat = pScreenData->screenFormat;

    if (screenFormat == 0 && pScreenData->colorMode == 1) {
        LoadBGPaletteSelect(bg, IsBGUseExtPltt(bg),
                      pPltData, pCmpInfo);
        return;
    }

    LoadBGPaletteSelect(bg,
                  pPltData->fmt != GX_TEXFMT_PLTT16 && screenFormat != 1,
                  pPltData, pCmpInfo);
}

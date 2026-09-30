/* PS2: mechanically prepared copy of libs/nns/g2d/calls/NNS_G2dLoadImage1DMapping.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/os_types.h"
#include "nitro/mi.h"
#include "nitro/spi.h"
#include "nitro/rtc.h"
#include "nitro/wm.h"

void DC_FlushRange(const void * startAddr, u32 nBytes);
typedef enum {
    GX_TEXSIZE_S8       = 0,
    GX_TEXSIZE_S16      = 1,
    GX_TEXSIZE_S32      = 2,
    GX_TEXSIZE_S64      = 3,
    GX_TEXSIZE_S128     = 4,
    GX_TEXSIZE_S256     = 5,
    GX_TEXSIZE_S512     = 6,
    GX_TEXSIZE_S1024    = 7
} GXTexSizeS;
typedef enum {
    GX_TEXSIZE_T8       = 0,
    GX_TEXSIZE_T16      = 1,
    GX_TEXSIZE_T32      = 2,
    GX_TEXSIZE_T64      = 3,
    GX_TEXSIZE_T128     = 4,
    GX_TEXSIZE_T256     = 5,
    GX_TEXSIZE_T512     = 6,
    GX_TEXSIZE_T1024    = 7
} GXTexSizeT;
typedef enum {
    GX_TEXFMT_NONE       = 0,
    GX_TEXFMT_A3I5       = 1,
    GX_TEXFMT_PLTT4      = 2,
    GX_TEXFMT_PLTT16     = 3,
    GX_TEXFMT_PLTT256    = 4,
    GX_TEXFMT_COMP4x4    = 5,
    GX_TEXFMT_A5I3       = 6,
    GX_TEXFMT_DIRECT     = 7
} GXTexFmt;
typedef enum {
    GX_TEXPLTTCOLOR0_USE  = 0,
    GX_TEXPLTTCOLOR0_TRNS = 1
} GXTexPlttColor0;
typedef enum {
    GX_OBJVRAMMODE_CHAR_2D      = (0 << 4 ) | (0 << 20 ),
    GX_OBJVRAMMODE_CHAR_1D_32K  = (1 << 4 ) | (0 << 20 ),
    GX_OBJVRAMMODE_CHAR_1D_64K  = (1 << 4 ) | (1 << 20 ),
    GX_OBJVRAMMODE_CHAR_1D_128K = (1 << 4 ) | (2 << 20 ),
    GX_OBJVRAMMODE_CHAR_1D_256K = (1 << 4 ) | (3 << 20 )
} GXOBJVRamModeChar;
static inline void GX_SetOBJVRamModeChar (GXOBJVRamModeChar mode)
{
    (*( REGType32v *) (((unsigned int)kh_ds_io + 0x0) + 0x000)) = (u32)((*( REGType32v *) (((unsigned int)kh_ds_io + 0x0) + 0x000)) &
                           ~(0x00300000 | 0x00000010 ) | mode);
}
static inline void GXS_SetOBJVRamModeChar (GXOBJVRamModeChar mode)
{
    (*( REGType32v *) (((unsigned int)kh_ds_io + 0x0) + 0x1000)) = (u32)((*( REGType32v *) (((unsigned int)kh_ds_io + 0x0) + 0x1000)) &
                               ~(0x00300000 |
                                 0x00000010 ) | mode);
}
void GX_LoadOBJ(const void * pSrc, u32 offset, u32 szByte);
void GXS_LoadOBJ(const void * pSrc, u32 offset, u32 szByte);
void GX_BeginLoadTex(void);
void GX_LoadTex(const void * pSrc, u32 destSlotAddr, u32 szByte);
void GX_EndLoadTex(void);
typedef enum WVRResult {
    WVR_RESULT_SUCCESS = 0,
    WVR_RESULT_OPERATING,
    WVR_RESULT_DISABLE,
    WVR_RESULT_INVALID_PARAM,
    WVR_RESULT_FIFO_ERROR,
    WVR_RESULT_ILLEGAL_STATUS,
    WVR_RESULT_VRAM_LOCKED,
    WVR_RESULT_FATAL_ERROR,
    WVR_RESULT_MAX
} WVRResult;
typedef void (*WVRCallbackFunc) (void * arg, WVRResult result);
typedef void (*MBFakeScanCallbackFunc) (u16 type, void * arg);
typedef BOOL (*MBFakeCompareGGIDCallbackFunc) (WMStartScanCallback * arg, u32 defaultGGID);
typedef enum NNSG2dCharacterFmt {
    NNS_G2D_CHARACTER_FMT_CHAR,
    NNS_G2D_CHARACTER_FMT_BMP,
    NNS_G2D_CHARACTER_FMT_MAX
} NNSG2dCharacterFmt;
typedef struct NNSG2dCharacterData {
    u16 H;
    u16 W;
    GXTexFmt pixelFmt;
    GXOBJVRamModeChar mappingType;
    u32 characterFmt;
    u32 szByte;
    void * pRawData;
} NNSG2dCharacterData;
inline NNSG2dCharacterFmt NNSi_G2dGetCharacterFmtType (u32 characterFmt)
{
    return (NNSG2dCharacterFmt)
           (0xFF & (characterFmt >> 0 ));
}
typedef enum NNS_G2D_VRAM_TYPE {
    NNS_G2D_VRAM_TYPE_3DMAIN = 0,
    NNS_G2D_VRAM_TYPE_2DMAIN = 1,
    NNS_G2D_VRAM_TYPE_2DSUB  = 2,
    NNS_G2D_VRAM_TYPE_2DBOTH = 3,
    NNS_G2D_VRAM_TYPE_MAX    = 3
} NNS_G2D_VRAM_TYPE;
typedef struct NNSG2dImageAttr {
    GXTexSizeS sizeS;
    GXTexSizeT sizeT;
    GXTexFmt fmt;
    BOOL bExtendedPlt;
    GXTexPlttColor0 plttUse;
    GXOBJVRamModeChar mappingType;
} NNSG2dImageAttr;
typedef struct NNSG2dVRamLocation {
    u32 baseAddrOfVram[NNS_G2D_VRAM_TYPE_MAX];
} NNSG2dVRamLocation;
typedef struct NNSG2dImageProxy {
    NNSG2dVRamLocation vramLocation;
    NNSG2dImageAttr attr;
} NNSG2dImageProxy;
void IntArray_Set(NNSG2dImageProxy * pImg, NNS_G2D_VRAM_TYPE type, u32 addr);
static inline int GetPow_ (u16 num)
{
    switch (num) {
    case 1:
        return GX_TEXSIZE_S8;
    case 2:
        return GX_TEXSIZE_S16;
    case 4:
        return GX_TEXSIZE_S32;
    case 8:
        return GX_TEXSIZE_S64;
    case 16:
        return GX_TEXSIZE_S128;
    case 32:
        return GX_TEXSIZE_S256;
    default:
        ((void)0) ;
        return GX_TEXSIZE_S8;
    }
}
static inline void CopyCharDataToImageAttr_ (const NNSG2dCharacterData * pSrc, NNSG2dImageAttr * pDst)
{
    if (pSrc->mappingType == GX_OBJVRAMMODE_CHAR_2D) {
        pDst->sizeS = (GXTexSizeS)(GetPow_(pSrc->W));
        pDst->sizeT = (GXTexSizeT)(GetPow_(pSrc->H));
    } else {
        ((void) 0)                       ;
        pDst->sizeS = (GXTexSizeS)pSrc->W;
        pDst->sizeT = (GXTexSizeT)pSrc->H;
    }
    pDst->fmt = pSrc->pixelFmt;
    pDst->bExtendedPlt = 0 ;
    pDst->plttUse = GX_TEXPLTTCOLOR0_TRNS;
    pDst->mappingType = pSrc->mappingType;
}
static inline void DoLoadingToVram_ (const NNSG2dCharacterData * pSrcData, u32 baseAddr, NNS_G2D_VRAM_TYPE type)
{
    const NNSG2dCharacterFmt charFmt = NNSi_G2dGetCharacterFmtType(pSrcData->characterFmt);
    ((void) 0)                      ;
    DC_FlushRange(pSrcData->pRawData, pSrcData->szByte);
    switch (type) {
    case NNS_G2D_VRAM_TYPE_3DMAIN:
        GX_BeginLoadTex();
        GX_LoadTex((void *)pSrcData->pRawData, baseAddr, pSrcData->szByte);
        GX_EndLoadTex();
        break;
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_LoadOBJ((void *)pSrcData->pRawData, baseAddr, pSrcData->szByte);
        break;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_LoadOBJ((void *)pSrcData->pRawData, baseAddr, pSrcData->szByte);
        break;
    default:
    }
}
static inline void SetOBJVRamModeCharacterMapping_ (NNS_G2D_VRAM_TYPE vramType, GXOBJVRamModeChar vramMode)
{
    switch (vramType) {
    case NNS_G2D_VRAM_TYPE_3DMAIN:
        break;
    case NNS_G2D_VRAM_TYPE_2DMAIN:
        GX_SetOBJVRamModeChar(vramMode);
        break;
    case NNS_G2D_VRAM_TYPE_2DSUB:
        GXS_SetOBJVRamModeChar(vramMode);
        break;
    default:
    }
}
static inline void SetupImageProxyPrams_ (const NNSG2dCharacterData * pSrcData, u32 baseAddr, NNS_G2D_VRAM_TYPE type, NNSG2dImageProxy * pImgProxy)
{
    CopyCharDataToImageAttr_(pSrcData, &pImgProxy->attr);
    IntArray_Set(pImgProxy, type, baseAddr);
}
extern void IntArray_Set (NNSG2dImageProxy * pImg, NNS_G2D_VRAM_TYPE type, u32 addr);

/* NNS_G2dLoadImage1DMapping -- NitroSystem g2d_Image.c: NNS_G2dLoadImage1DMapping. */
void NNS_G2dLoadImage1DMapping (const NNSG2dCharacterData * pSrcData, u32 baseAddr, NNS_G2D_VRAM_TYPE type, NNSG2dImageProxy * pImgProxy)
{

    SetOBJVRamModeCharacterMapping_(type, pSrcData->mappingType);

    DoLoadingToVram_(pSrcData, baseAddr, type);

    SetupImageProxyPrams_(pSrcData, baseAddr, type, pImgProxy);
}

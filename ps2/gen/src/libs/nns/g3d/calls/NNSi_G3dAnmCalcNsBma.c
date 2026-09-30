/* PS2: mechanically prepared copy of libs/nns/g3d/calls/NNSi_G3dAnmCalcNsBma.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/fx_types.h"
#include "nitro/os.h"
#include "nitro/pxi.h"
#include "nnsys/g3d.h"

static inline s32 FX_Whole (fx32 v)
    {
        return (s32)(v >> 12 );
    }
inline void * NNS_G3dGetResDataByIdx(const NNSG3dResDict * dict, u32 idx);
inline void * NNS_G3dGetResDataByIdx (const NNSG3dResDict * dict, u32 idx)
{
    NNSG3dResDictEntryHeader * hdr;
    if (dict != NULL && idx < dict->numEntry) {
        hdr = (NNSG3dResDictEntryHeader *)((u8 *)dict + dict->ofsEntry);
        return (void *)(&hdr->data[0] + hdr->sizeUnit * idx);
    } else {
        return NULL ;
    }
}
extern u16 func_01ff9fd4 (const NNSG3dResMatCAnm * pAnm, u32 info, u32 frame);
extern u16 GetMatColAnmuAlphaValue_ (const NNSG3dResMatCAnm * pAnm, u32 info, u32 frame);
static inline void GetMatColAnm_ (const NNSG3dResMatCAnm * pAnm, u16 idx, u32 frame, NNSG3dMatAnmResult * pResult)
{
    {
        u16 diffuse, ambient, emission, specular, polygon_alpha;
        const NNSG3dResDictMatCAnmData * pAnmData =
            (const NNSG3dResDictMatCAnmData *)NNS_G3dGetResDataByIdx(&pAnm->dict, idx);
        diffuse = func_01ff9fd4(pAnm, pAnmData->diffuse, frame);
        ambient = func_01ff9fd4(pAnm, pAnmData->ambient, frame);
        pResult->prmMatColor0 = ((u32)((diffuse) | (( ambient) << 16) | ((( (pResult->prmMatColor0 & 0x00008000)) != 0) << 15)))
                                                      ;
        emission = func_01ff9fd4(pAnm, pAnmData->emission, frame);
        specular = func_01ff9fd4(pAnm, pAnmData->specular, frame);
        pResult->prmMatColor1 = ((u32)((specular) | (( emission) << 16) | ((( (pResult->prmMatColor1 & 0x00008000)) != 0) << 15)))
                                                      ;
        polygon_alpha = GetMatColAnmuAlphaValue_(pAnm, pAnmData->polygon_alpha, frame);
        pResult->prmPolygonAttr = (pResult->prmPolygonAttr & ~0x001f0000 ) |
                                  (polygon_alpha << 16 );
    }
}

/* NNSi_G3dAnmCalcNsBma -- NitroSystem nsbma.c: NNSi_G3dAnmCalcNsBma. */
void NNSi_G3dAnmCalcNsBma (NNSG3dMatAnmResult * pResult, const NNSG3dAnmObj * pAnmObj, u32 dataIdx)
{
    {
        const NNSG3dResMatCAnm * pMatAnm = (const NNSG3dResMatCAnm *)pAnmObj->resAnm;
        GetMatColAnm_(pMatAnm,
                      (u16)dataIdx,
                      (u32)FX_Whole(pAnmObj->frame),
                      pResult);
    }
}

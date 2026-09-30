/* PS2: mechanically prepared copy of libs/nns/g3d/calls/NNSi_G3dAnmCalcNsBtp.c (ps2/tools/prep_sources.py). Do not edit. */


#include "nitro/types.h"
#include "nitro/fx_types.h"
#include "nitro/os.h"
#include "nnsys/g3d.h"

static inline s32 FX_Whole (fx32 v)
    {
        return (s32)(v >> 12 );
    }
const NNSG3dResName * NNSi_G3dGetTexPatAnmTexNameByIdx(const NNSG3dResTexPatAnm * pPatAnm, u8 texIdx);
const NNSG3dResName * NNSi_G3dGetTexPatAnmPlttNameByIdx(const NNSG3dResTexPatAnm * pPatAnm, u8 plttIdx);
const NNSG3dResTexPatAnmFV * NNSi_G3dGetTexPatAnmFV(const NNSG3dResTexPatAnm * pPatAnm, u32 idx, u32 frame);
extern void SetTexParamaters_ (const NNSG3dResTex * pTex, const NNSG3dResName * pTexName, NNSG3dMatAnmResult * pResult);
extern void SetPlttParamaters_ (const NNSG3dResTex * pTex, const NNSG3dResName * pPlttName, NNSG3dMatAnmResult * pResult);

/* NNSi_G3dAnmCalcNsBtp -- NitroSystem nsbtp.c: NNSi_G3dAnmCalcNsBtp. */
void NNSi_G3dAnmCalcNsBtp (NNSG3dMatAnmResult * pResult, const NNSG3dAnmObj * pAnmObj, u32 dataIdx)
{

    {

        const NNSG3dResTexPatAnm * pPatAnm
            = (const NNSG3dResTexPatAnm *)pAnmObj->resAnm;

        const NNSG3dResTexPatAnmFV * pTexFV
            = NNSi_G3dGetTexPatAnmFV(pPatAnm,
                                     (u16)dataIdx,
                                     (u16)FX_Whole(pAnmObj->frame));

        SetTexParamaters_(pAnmObj->resTex,
                          NNSi_G3dGetTexPatAnmTexNameByIdx(pPatAnm, pTexFV->idTex),
                          pResult);

        if (pTexFV->idPltt != 255) {
            SetPlttParamaters_(pAnmObj->resTex,
                               NNSi_G3dGetTexPatAnmPlttNameByIdx(pPatAnm, pTexFV->idPltt),
                               pResult);
        }
    }
}

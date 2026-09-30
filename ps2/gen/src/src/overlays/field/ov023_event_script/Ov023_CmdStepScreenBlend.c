/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/Ov023_CmdStepScreenBlend.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov023_CmdStepScreenBlend -- Ov023_CmdStepScreenBlend: one frame of the screen blend started by
 * Ov023_CmdStartScreenBlend (02085284).  On the first frame in global mode 4 (02020a9c) the
 * graphics mode is switched to 0xe / 4 / 1 (020056b4).  The elapsed count (+0x24 of the event
 * block) advances, the level (+0x20) becomes from + elapsed * (to - from) / frames clamped to
 * 1..16 and is published as the blend weight (+0x2c).  Once the elapsed count reaches the frame
 * count, a full weight of 16 finishes the blend (Ov023_ScreenBlendDone 02085258) and 1 is
 * returned, else 0. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov023EventBlock {
    u8   pad_00[0x14];
    int  nFadeFrom;           /* 0x14 */
    int  nFadeTo;             /* 0x18 */
    int  nFadeFrames;         /* 0x1c */
    int  nFadeLevel;          /* 0x20 */
    int  nFadeElapsed;        /* 0x24 */
    int  nField28;            /* 0x28 */
    int  nBlendWeight;        /* 0x2c */
} Ov023EventBlock;

typedef struct Ov023ScriptCtx {
    u8   pad_000[0x128];
    Ov023EventBlock *pEvent;  /* 0x128 */
} Ov023ScriptCtx;

/* The quotient is the low half of the helper's long long return; writing `/` emits _s32_div_f,
 * which is not linkable here. */
extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern void GX_SetGraphicsMode(int nMode, int nBgMode, int nBg0Mode);    /* GX_SetGraphicsMode */
extern void Ov023_SceneExit(int nWeight);                       /* Ov023_ScreenBlendDone */

int Ov023_CmdStepScreenBlend(Ov023ScriptCtx *pCtx)
{
    Ov023EventBlock *pEvent;
    int nFrom;

    if (pCtx->pEvent->nFadeElapsed == 0 && LoadGlobalU16At0() == 4) {
        GX_SetGraphicsMode(0xe, 4, 1);
    }
    pCtx->pEvent->nFadeElapsed++;
    pEvent = pCtx->pEvent;
    nFrom = pEvent->nFadeFrom;
    pEvent->nFadeLevel = nFrom + (int)kh_rt_s32_divmod(pEvent->nFadeElapsed * (pEvent->nFadeTo - nFrom), pEvent->nFadeFrames);
    if (pCtx->pEvent->nFadeLevel > 16) {
        pCtx->pEvent->nFadeLevel = 16;
    }
    if (pCtx->pEvent->nFadeLevel < 1) {
        pCtx->pEvent->nFadeLevel = 1;
    }
    pCtx->pEvent->nBlendWeight = pCtx->pEvent->nFadeLevel;
    pEvent = pCtx->pEvent;
    if (pEvent->nFadeElapsed >= pEvent->nFadeFrames) {
        if (pEvent->nBlendWeight == 16) {
            Ov023_SceneExit(pEvent->nBlendWeight);
        }
        return 1;
    }
    return 0;
}

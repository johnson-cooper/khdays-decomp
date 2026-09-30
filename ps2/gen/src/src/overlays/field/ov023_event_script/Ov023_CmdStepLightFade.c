/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/Ov023_CmdStepLightFade.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov023_CmdStepLightFade -- Ov023_CmdStepLightFade: one frame of the light fade started by
 * Ov023_CmdStartLightFade (02086fac).  Operand 2 is the light.  The elapsed count (+0x24 of
 * the event block) advances, the level (+0x20) becomes from + elapsed * (to - from) / frames
 * clamped to 0..31 and is applied (Ov023_SetLightLevel 02089cdc).  Returns 1 once the elapsed
 * count reaches the frame count, else 0. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov023EventBlock {
    u8   pad_00[0x14];
    int  nFadeFrom;           /* 0x14 */
    int  nFadeTo;             /* 0x18 */
    int  nFadeFrames;         /* 0x1c */
    int  nFadeLevel;          /* 0x20 */
    int  nFadeElapsed;        /* 0x24 */
} Ov023EventBlock;

typedef struct Ov023ScriptCtx {
    u8   pad_000[0x128];
    Ov023EventBlock *pEvent;  /* 0x128 */
} Ov023ScriptCtx;

/* The quotient is the low half of the helper's long long return; writing `/` emits _s32_div_f,
 * which is not linkable here. */
extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern void Ov023_SetGateValue(int nLevel, int nLight);            /* Ov023_SetLightLevel */

int Ov023_CmdStepLightFade(Ov023ScriptCtx *pCtx, u8 *pOperand)
{
    Ov023EventBlock *pEvent;
    int nFrom;
    int nLight;

    nLight = ScriptVm_ReadOperandInt(pCtx, pOperand + 0x10);
    pCtx->pEvent->nFadeElapsed++;
    pEvent = pCtx->pEvent;
    nFrom = pEvent->nFadeFrom;
    pEvent->nFadeLevel = nFrom + (int)kh_rt_s32_divmod(pEvent->nFadeElapsed * (pEvent->nFadeTo - nFrom), pEvent->nFadeFrames);
    if (pCtx->pEvent->nFadeLevel > 31) {
        pCtx->pEvent->nFadeLevel = 31;
    }
    if (pCtx->pEvent->nFadeLevel < 0) {
        pCtx->pEvent->nFadeLevel = 0;
    }
    Ov023_SetGateValue(pCtx->pEvent->nFadeLevel, nLight);
    if (pCtx->pEvent->nFadeElapsed >= pCtx->pEvent->nFadeFrames) {
        return 1;
    }
    return 0;
}

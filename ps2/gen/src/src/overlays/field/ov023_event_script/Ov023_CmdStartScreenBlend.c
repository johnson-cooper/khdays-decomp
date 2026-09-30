/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/Ov023_CmdStartScreenBlend.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov023_CmdStartScreenBlend -- Ov023_CmdStartScreenBlend: script command that starts a screen blend
 * (the weight at +0x2c of the event block, applied by Ov023_ApplyScreenBlend 020838b8).  The
 * weight becomes the fade's level (+0x20) and start (+0x14), operand 0 its end (+0x18),
 * operand 1 its frame count (+0x1c) and the elapsed count (+0x24) is cleared.  Starting from
 * a full weight of 16 in global mode 4 (02020a9c) VRAM bank D goes to the LCDC and a plain
 * capture is armed (DISPCAPCNT 0xc0330010), and the game flag set (02023560 1).  With no frames
 * the end weight is applied at once (Ov023_ScreenBlendDone 02085258) and 1 returned, else the
 * command is re-queued (020219b4) for Ov023_CmdStepScreenBlend (02085310) and 0 returned. */

#include "nitro/types.h"
#include "game/engine.h"

static volatile u32 *const REG_DISPCAPCNT = (volatile u32 *)((unsigned int)kh_ds_io + 0x64);

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

extern void GX_SetBankForLCDC(int nBanks);
extern void Ov023_SceneExit(int nWeight);                       /* Ov023_ScreenBlendDone */
extern void Slot48_StoreAtCurrentIndex(Ov023ScriptCtx *pCtx, void *pCmd);        /* ScriptVm_RequeueCommand */

int Ov023_CmdStartScreenBlend(Ov023ScriptCtx *pCtx, u8 *pOperand)
{
    pCtx->pEvent->nFadeLevel = pCtx->pEvent->nBlendWeight;
    pCtx->pEvent->nFadeFrom = pCtx->pEvent->nFadeLevel;
    pCtx->pEvent->nFadeTo = ScriptVm_ReadOperandInt(pCtx, pOperand);
    pCtx->pEvent->nFadeFrames = ScriptVm_ReadOperandInt(pCtx, pOperand + 8);
    pCtx->pEvent->nFadeElapsed = 0;
    if (pCtx->pEvent->nBlendWeight == 16) {
        if (LoadGlobalU16At0() == 4) {
            GX_SetBankForLCDC(8);
            *REG_DISPCAPCNT = 0xc0330010;
        }
        StoreToGlobalPtr4FieldE4IfSet(1);
    }
    if (pCtx->pEvent->nFadeFrames == 0) {
        pCtx->pEvent->nBlendWeight = pCtx->pEvent->nFadeTo;
        Ov023_SceneExit(pCtx->pEvent->nBlendWeight);
        return 1;
    }
    Slot48_StoreAtCurrentIndex(pCtx, pOperand);
    return 0;
}

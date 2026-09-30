/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/Ov023_CmdResetStage.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov023_CmdResetStage -- Ov023_CmdResetStage: script command that tears the event's stage down
 * for a scene change.  Operand 0 is the change kind.  The current camera's target mode (+0xf8
 * of camera +0x488 of the event block) is cleared to -1.  Unless bit 1 of the global mode
 * (02020a9c) is set: kind 0 turns the LCD on (POWCNT bit 15), 1 turns it off, both with
 * texture bank 7 and sprite mode 3 / 1 (02010e80), kind 3 uses bank 0xf with mode 4 / 1; the
 * sprite layer is reset (0201133c), the screens handed back (Ov023_Teardown 02084d64), the
 * ov002 side reset (0206da28), the actors reset (02083bd4), pending entity work drained
 * (0202c4b0 while 0202c57c), the entity manager reset (0202b788 / 0202b73c / 0202c440), the
 * scene's sub-object rebuilt (0208402c), game fields 0x2480 := 0 and 0x248f := 1 (020235e8),
 * in a host session (bit 2 of data_0204c240) 02023574 run, and for kind 2 the resource 0x2da
 * requested (0203355c).  Returns 1. */

#include "nitro/types.h"
#include "game/engine.h"

static volatile u16 *const REG_POWCNT = (volatile u16 *)((unsigned int)kh_ds_io + 0x304);

typedef struct Ov023Camera {
    u8   pad_000[0xf8];
    int  nTargetMode;         /* 0xf8 */
    int  nTargetActor;        /* 0xfc */
    int  nField100;           /* 0x100 */
} Ov023Camera;                /* 0x104 */

typedef struct Ov023EventBlock {
    u8   pad_000[0x30];
    Ov023Camera aCamera[2];   /* 0x030 */
    Ov023Camera aCameraAlt[2]; /* 0x238 */
    u8   pad_440[0x488 - 0x440];
    int  nCamera;             /* 0x488 */
} Ov023EventBlock;

typedef struct Ov023ScriptCtx {
    u8   pad_000[0x128];
    Ov023EventBlock *pEvent;  /* 0x128 */
} Ov023ScriptCtx;

extern void  GX_SetBankForTex(int nBank);
extern void  NNS_GfdInitFrmTexVramManager(int nA, int nB);
extern void  NNS_GfdResetFrmPlttVramState(void);
extern void  Ov023_Teardown(void);                             /* Ov023_Teardown */
extern void  Ov002_ScheduleRetry(void);
extern void  Ov023_ResetEntryTable(void);                             /* Ov023_ResetActors */
extern void  Ov023_RebuildSubObject(void);                             /* Ov023_RebuildSubObject */
extern u8    data_0204c240;                                         /* session bits */

int Ov023_CmdResetStage(Ov023ScriptCtx *pCtx, void *pOperand)
{
    int nKind;

    nKind = ScriptVm_ReadOperandInt(pCtx, pOperand);
    pCtx->pEvent->aCamera[pCtx->pEvent->nCamera].nTargetMode = -1;
    if (!(LoadGlobalU16At0() & 2)) {
        switch (nKind) {
        case 0:
            *REG_POWCNT |= 0x8000;
            GX_SetBankForTex(7);
            NNS_GfdInitFrmTexVramManager(3, 1);
            break;
        case 1:
            *REG_POWCNT &= 0xffff7fff;
            GX_SetBankForTex(7);
            NNS_GfdInitFrmTexVramManager(3, 1);
            break;
        case 3:
            GX_SetBankForTex(0xf);
            NNS_GfdInitFrmTexVramManager(4, 1);
            break;
        }
        NNS_GfdResetFrmPlttVramState();
        Ov023_Teardown();
        Ov002_ScheduleRetry();
        Ov023_ResetEntryTable();
        while (EntityMgr_GetVramStateDepth() != 0) {
            EntityMgr_PopVramState();
        }
        EntityManager_ReleaseViews();
        EntityManager_ResetSingleton();
        EntityMgr_PushVramState();
        Ov023_RebuildSubObject();
        GameState_SetField(0x2480, 1, 0);
        GameState_SetField(0x248f, 1, 1);
        if (data_0204c240 & 4) {
            ClearGlobalPtrE8AndHead();
        }
        if (nKind == 2) {
            Res_RequestIdPair(0x2da);
        }
    }
    return 1;
}

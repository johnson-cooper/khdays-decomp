/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_ElementPromptStep.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"

#include "nitro/types.h"

extern void *Ov002_GetModuleScale(char *pElement);
extern int SoundBank_Release(int nSlot, int nId);
extern int SoundBank_Acquire(int nSlot, int nId);
extern void Slot_Spawn(int nId, int nMode, void *pBlock, int nParam);
extern void GameState_SetFlag(int nField);
extern void GameState_ClearFlag(int nField);
extern int func_ov022_02083f0c(void);
extern int func_ov022_02083f5c(void);
extern void func_ov022_02086818(int nHandle, int nMode);
extern void Ov002_SetOrClearFlag200(int nHandle, int nMode);
extern int QueryActiveStateOrDelegate(void);
extern void *GetEntryField20ByIndex(int nIndex);
extern void Ov002_SetRosterHighlight(char *pElement, int nIndex, int bOn);
extern int func_ov022_020882f8(void);
extern void func_ov022_020ad838(void *pEntry, int bOn);
extern int Ov002_PollSession(void);
extern int Ov002_GetPhaseWord(void);
extern int Ov002_AdvanceElementClock(char *pElement, char *pAnim, void *pCtx,
                               int nMode, int nRange, void *pOut);
extern void Ov002_RebindAnimTracks(short *pAnim, int nTrack, int nFrame);
extern void SceneNode_Disable(u16 *pAnim);
extern void Ov002_ElementChoiceStep(void);

/* Drive an element through its talk step and say what runs next.
 *
 * The pending bit is dropped on entry and an element that is not active does
 * nothing. An element already finished tears the prompt down and hands back the
 * follow-up handler. Otherwise the prompt is put up if it is not up yet, and
 * once the player is in a state that allows it the line is advanced: each step
 * counts the track down, and the last one either plays the closing effect or
 * releases the animation. When the line has run far enough the camera and the
 * other entries are let go again.
 */
void *Ov002_ElementPromptStep(char *pElement)
{
    void *pCtx;
    int nFlags;
    int nA;
    int nB;
    int i;

    pCtx = Ov002_GetModuleScale(pElement);
    *(u8 *)(pElement + 0x1b5) &= ~0x80;

    if ((*(u16 *)(pElement + 0x12) & 4) == 0) {
        return 0;
    }

    nFlags = *(u8 *)(pElement + 0x1b5);

    if ((nFlags & 1) != 0) {
        if ((nFlags & 4) != 0) {
            if (SoundBank_Release(-1, 0x2be) == 0) {
                return 0;
            }
            *(u8 *)(pElement + 0x1b5) &= ~4;
            if ((*(u8 *)(pElement + 0x1b5) & 0x10) != 0) {
                *(u8 *)(pElement + 0x1b5) &= ~0x10;
                GameState_ClearFlag(0x20e0);
            }
        }
        *(u8 *)(pElement + 0x17) = 1;
        *(u16 *)(pElement + 0x12) |= 8;
        return Ov002_ElementChoiceStep;
    }

    if ((nFlags & 2) != 0) {
        nA = func_ov022_02083f0c();
        func_ov022_02086818(func_ov022_02083f5c(), 0);
        Ov002_SetOrClearFlag200(nA, 1);
        Ov002_SetRosterHighlight(pElement, QueryActiveStateOrDelegate(), 1);

        for (i = 0; i < func_ov022_020882f8(); i++) {
            *(kh_unaligned_u64 *)GetEntryField20ByIndex(i) |= 0x40000000000ULL;
            func_ov022_020ad838(GetEntryField20ByIndex(i), 1);
        }
    }

    if ((*(u8 *)(pElement + 0x1b5) & 4) == 0) {
        if (SoundBank_Acquire(-1, 0x2be) != 0) {
            *(u8 *)(pElement + 0x1b5) |= 4;
            *(u8 *)(pElement + 0x1b5) |= 0x10;
            GameState_SetFlag(0x20e0);
        } else {
            return 0;
        }
    }

    if (Ov002_PollSession() != 0
        && (Ov002_GetPhaseWord() == 1 || Ov002_GetPhaseWord() == 5)) {
        if (*(signed char *)(pElement + 0x1b8) == 2
            && (*(u8 *)(pElement + 0x1b5) & 8) == 0) {
            Slot_Spawn(0x2be, 0, pElement + 0xe0, 0);
            *(u8 *)(pElement + 0x1b5) |= 8;
        }

        if (Ov002_AdvanceElementClock(pElement, pElement + 0x3c, pCtx, 0, 0x2a000,
                                pElement + 0x1b0) == 0) {
            *(signed char *)(pElement + 0x1b8) -= 1;
            Ov002_RebindAnimTracks((short *)(pElement + 0x3c),
                                *(signed char *)(pElement + 0x1b8), 0);
            *(int *)(pElement + 0x1b0) = 0;

            if (*(signed char *)(pElement + 0x1b8) == 1) {
                Slot_Spawn(0x2be, 1, pElement + 0xe0, 0);
            } else if (*(signed char *)(pElement + 0x1b8) == 0) {
                *(u8 *)(pElement + 0x1b5) |= 1;
                SceneNode_Disable((u16 *)(pElement + 0x3c));
            }
        } else if ((*(u8 *)(pElement + 0x1b5) & 2) != 0
                   && *(signed char *)(pElement + 0x1b8) == 1
                   && *(int *)(pElement + 0x1b0) >= 0x5000) {
            nA = func_ov022_02083f0c();
            nB = func_ov022_02083f5c();
            Ov002_SetOrClearFlag200(nA, 0);
            func_ov022_02086818(nB, 1);
            Ov002_SetRosterHighlight(pElement, QueryActiveStateOrDelegate(), 0);

            for (i = 0; i < func_ov022_020882f8(); i++) {
                *(kh_unaligned_u64 *)GetEntryField20ByIndex(i) &= ~0x40000000000ULL;
                func_ov022_020ad838(GetEntryField20ByIndex(i), 0);
            }
        }
    }

    return 0;
}

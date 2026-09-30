/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_PanelStepCursor.c (ps2/tools/prep_sources.py). Do not edit. */
/* Step the panel cursor back one position; answer whether anything moved.
 *
 * The current mode is classified and a six-way jump table dispatches. Class 0
 * scans the slot table backwards for the first slot that is not 7, wrapping
 * modulo the slot count. Class 1 steps within the row. Classes 2 and 3 each
 * walk their own list, falling into the other when the index is already zero
 * and that list has entries. Class 5 refuses unless the cached entry is live
 * and passes both group filters. Either way the new kind goes to the rebuild.
 *
 * Three codegen notes. This is THUMB, so there is no smull and every division
 * goes through the runtime helper -- called explicitly here, as everywhere else
 * in the tree, because spelling it `%` emits a symbol that does not link.
 * The four locals are a struct so that their stack slots come out in the ROM's
 * order, which no declaration order of four separate locals can reproduce. And
 * the first member is volatile, which is what keeps the group in memory instead
 * of letting mwcc cache the running value in a callee-saved register across the
 * calls in cases 2 and 3.
 */

#include "nitro/types.h"

typedef struct {
    u16 nKey;
    u16 nTag;
    int nState;
} Ov002PanelEntry;

typedef struct {
    u8 bKind;
    u8 bMode;
    u8 bField0002;
    u8 bListIndex;
    u8 bField0004;
    u8 pad0005[0x13];
    u8 bSlotCount;
    u8 pad0019;
    u16 aSlots[0x243];
    u8 pad04a0[4];
    Ov002PanelEntry *pCachedEntry;
    u8 pad04a8[4];
    u8 bListRowBase;
    u8 bListRowOffset;
} Ov002PanelSession;

typedef struct {
    volatile int nOldMode;
    int nNext;
    int nRow;
    int nClass;
} Ov002PanelWork;

extern Ov002PanelSession *data_ov002_0207f620;

extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern int Ov002_ClassifyCode(int *pOut, int nMode);
extern int Ov002_GetLastRowPosition(int *pOut, int nIndex);
extern int Ov002_CountPanelListEntries(void);
extern int Ov002_CountSecondListEntries(void);
extern int Ov002_Panel_IsSlotEnabledA(int nGroup, int nKey);
extern int Ov002_Panel_IsSlotEnabledB(int nGroup, int nKey);
extern void Ov002_HandlePanelInput(int nTarget, int nFlag);
extern void Ov002_PanelApplyCursorMove(int nFrom, int nTo);

int Ov002_PanelStepCursor(void) {
    Ov002PanelSession *s = data_ov002_0207f620;
    Ov002PanelWork work;
    int nCount;
    int nPage;
    int nResult;
    int i;

    work.nNext = s->bKind;
    work.nOldMode = s->bMode;

    switch (Ov002_ClassifyCode(&work.nClass, work.nOldMode)) {
    case 0:
        nCount = s->bSlotCount;
        i = 1;
        if (i < nCount) {
        {
            int nKindAndCount = s->bKind + nCount;

            do {
                int k = (u8)(kh_rt_s32_divmod(nKindAndCount - i, nCount) >> 32);

                if (s->aSlots[k] != 7) {
                    work.nNext = k;
                    break;
                }
                i++;
            } while (i < nCount);
        }
        }
        break;

    case 1:
        work.nNext = (u8)(kh_rt_s32_divmod(
            Ov002_GetLastRowPosition(&work.nRow, s->bField0002), 6) >> 32);
        if (work.nRow != work.nClass) {
            Ov002_HandlePanelInput(work.nRow + 1, -1);
        }
        break;

    case 2:
        nCount = Ov002_CountPanelListEntries();
        if (s->bListIndex == 0 && Ov002_CountSecondListEntries() > 0) {
            Ov002_HandlePanelInput(s->bListRowOffset + 5, -1);
            work.nNext = (u8)(kh_rt_s32_divmod(Ov002_CountSecondListEntries() + 5, 6) >> 32);
        } else {
            work.nNext = (u8)(kh_rt_s32_divmod(
                s->bListIndex + nCount - 1, nCount) >> 32);
            nPage = (u8)kh_rt_s32_divmod(work.nNext, 6);
            if (nPage != work.nClass) {
                Ov002_HandlePanelInput(nPage + 4, -1);
            }
            work.nNext = (u8)(kh_rt_s32_divmod(work.nNext, 6) >> 32);
        }
        break;

    case 3:
        nCount = Ov002_CountSecondListEntries();
        if (s->bField0004 == 0 && Ov002_CountPanelListEntries() > 0) {
            Ov002_HandlePanelInput(s->bListRowBase + 3, -1);
            work.nNext = (u8)(kh_rt_s32_divmod(Ov002_CountPanelListEntries() + 5, 6) >> 32);
        } else {
            work.nNext = (u8)(kh_rt_s32_divmod(
                s->bField0004 + nCount - 1, nCount) >> 32);
            nPage = (u8)kh_rt_s32_divmod(work.nNext, 6);
            if (nPage != work.nClass) {
                Ov002_HandlePanelInput(nPage + 6, -1);
            }
            work.nNext = (u8)(kh_rt_s32_divmod(work.nNext, 6) >> 32);
        }
        break;

    case 5: {
        Ov002PanelEntry *pEntry = s->pCachedEntry;
        int nKey = pEntry->nKey;

        if (pEntry->nState == 0 ||
            Ov002_Panel_IsSlotEnabledA(0, nKey) == 0 ||
            Ov002_Panel_IsSlotEnabledB(0, nKey) == 0) {
            return 0;
        }
        work.nNext = (u8)(s->bKind == 0);
        break;
    }
    }

    if (s->bMode != work.nOldMode || s->bKind != work.nNext) {
        nResult = 1;
    } else {
        nResult = 0;
    }
    Ov002_PanelApplyCursorMove(s->bKind, work.nNext);
    return nResult;
}

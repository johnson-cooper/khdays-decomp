/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_PanelAdvanceCursor.c (ps2/tools/prep_sources.py). Do not edit. */
#pragma opt_lifetimes off

/*
 * Ov002_PanelAdvanceCursor - move the panel cursor one step and report whether
 * it actually went anywhere.
 *
 * Where the cursor goes depends on what it is sitting on, which the classifier
 * at 0205a3f0 reports as one of six kinds while also handing back the screen row
 * the cursor is currently drawn on.
 *
 *   kind 0  a slot: walk forward from the current slot to the next one that is
 *           not occupied, wrapping around the slot count and giving up after a
 *           full lap;
 *   kind 1  a grid cell: ask 0205a87c for the next cell and its page, keep the
 *           cell within its row of six, and tell the input handler when the page
 *           changed;
 *   kind 2  the first list, kind 3 the second: step to the next entry; on running
 *           past the end hand over to the other list when that one has entries
 *           and otherwise fall back to row zero, and while stepping normally tell
 *           the input handler whenever the row changed;
 *   kind 5  the cached entry: move only if it still has data and both of its
 *           predicates accept it, and refuse outright otherwise;
 *   kind 4  and anything else: leave the cursor where it is.
 *
 * The answer is the comparison of the new position against the old: true when
 * either the kind or the mode ended up different.
 *
 * THUMB.
 *
 * Three things below are load-bearing for codegen and are not style:
 *
 *   - `#pragma opt_lifetimes off` gives the two snapshots taken at the top a
 *     stack home that is written on every assignment and read on every use, which
 *     is how the original treats them.
 *   - the two `volatile` reads are the only ones the original does not satisfy
 *     from a register that still holds the value: the list arms reload the next
 *     kind at the head of the stepping branch, and the final call reloads it
 *     again after the mode comparison has spent that register.
 *   - `nWork` is shared by the slot walk, the grid position and the cached
 *     entry's code word, and `nResult` by the walk's attempt counter and the
 *     returned flag, because in each case the original keeps them in one
 *     register.
 */

#include "nitro/types.h"

typedef struct {
    u16 nKey;
    u16 nTag;
    void *pObject;
    u8 pad0008[0x10];
} Ov002PanelSubEntry;

typedef struct {
    u8 bKind;                /* +0x000 */
    u8 bMode;                /* +0x001 */
    u8 bIndex;               /* +0x002 */
    u8 bListIndex;           /* +0x003 */
    u8 bKey;                 /* +0x004 */
    u8 pad05[0x13];
    u8 bSlotCount;           /* +0x018 */
    u8 pad19;
    u16 aSlots[11];                     /* +0x01a */
    u8 pad30[0x474];
    Ov002PanelSubEntry *pCachedEntry;   /* +0x4a4 */
} Ov002PanelSession;

extern Ov002PanelSession *data_ov002_0207f620;

/* The quotient is the low half of the helper's long long return and the
 * remainder the high half; writing / or % emits _s32_div_f, which is not
 * linkable here. */
extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);

extern int Ov002_ClassifyCode(int *pOutRow, int nMode);
extern int Ov002_CountPanelListEntries(void);
extern int Ov002_CountSecondListEntries(void);
extern int Ov002_GetGridPosition(int *pOutPage, int nIndex);
extern void Ov002_HandlePanelInput(int nRaw, int nSound);
extern void Ov002_PanelApplyCursorMove(int nFromSlot, int nToSlot);
extern int Ov002_Panel_IsSlotEnabledA(int nZero, int wCode);
extern int Ov002_Panel_IsSlotEnabledB(int nZero, int wCode);

int Ov002_PanelAdvanceCursor(void)
{
    Ov002PanelSession *s;
    Ov002PanelSubEntry *pEntry;
    int nClassRow;
    int nGridPage;
    int nNextKind;
    int nModeWas;
    int nCount;
    int bSame;
    int nResult;
    int nWork;
    int nIndex;

    s = data_ov002_0207f620;
    nNextKind = s->bKind;
    nModeWas = s->bMode;
    switch (Ov002_ClassifyCode(&nClassRow, nModeWas)) {
    case 0:
        nCount = s->bSlotCount;
        nResult = 1;
        if (nCount > 1) {
            nWork = s->bKind + 1;
            do {
                nIndex = (unsigned char)(kh_rt_s32_divmod(nWork, nCount) >> 32);
                if (s->aSlots[nIndex] != 7) {
                    nNextKind = nIndex;
                    break;
                }
                nWork++;
                nResult++;
            } while (nResult < nCount);
        }
        break;

    case 1:
        nWork = Ov002_GetGridPosition(&nGridPage, s->bIndex);
        nNextKind = (unsigned char)(kh_rt_s32_divmod(nWork, 6) >> 32);
        if (nGridPage != nClassRow) {
            Ov002_HandlePanelInput(nGridPage + 1, -1);
        }
        break;

    case 2:
        nCount = Ov002_CountPanelListEntries();
        nNextKind = (unsigned char)(s->bListIndex + 1);
        if (nNextKind >= nCount) {
            if (Ov002_CountSecondListEntries() != 0) {
                Ov002_HandlePanelInput(6, -1);
            } else {
                Ov002_HandlePanelInput(4, -1);
            }
            nNextKind = 0;
        } else {
            nIndex = (unsigned char)kh_rt_s32_divmod(*(volatile int *)&nNextKind, 6);
            if (nIndex != nClassRow) {
                Ov002_HandlePanelInput(nIndex + 4, -1);
            }
            nNextKind = (unsigned char)(kh_rt_s32_divmod(nNextKind, 6) >> 32);
        }
        break;

    case 3:
        nCount = Ov002_CountSecondListEntries();
        nNextKind = (unsigned char)(s->bKey + 1);
        if (nNextKind >= nCount) {
            if (Ov002_CountPanelListEntries() > 0) {
                Ov002_HandlePanelInput(4, -1);
            } else {
                Ov002_HandlePanelInput(6, -1);
            }
            nNextKind = 0;
        } else {
            nIndex = (unsigned char)kh_rt_s32_divmod(*(volatile int *)&nNextKind, 6);
            if (nIndex != nClassRow) {
                Ov002_HandlePanelInput(nIndex + 6, -1);
            }
            nNextKind = (unsigned char)(kh_rt_s32_divmod(nNextKind, 6) >> 32);
        }
        break;

    case 5:
        pEntry = s->pCachedEntry;
        nWork = pEntry->nKey;
        if (pEntry->pObject == 0 || Ov002_Panel_IsSlotEnabledA(0, nWork) == 0 ||
            Ov002_Panel_IsSlotEnabledB(0, nWork) == 0) {
            return 0;
        }
        nNextKind = (unsigned char)(s->bKind == 0);
        break;
    }

    bSame = 0;
    if (s->bKind == nNextKind && s->bMode == nModeWas) {
        bSame = 1;
    }
    if (bSame == 0) {
        nResult = 1;
    } else {
        nResult = 0;
    }
    Ov002_PanelApplyCursorMove(s->bKind, *(volatile int *)&nNextKind);
    return nResult;
}

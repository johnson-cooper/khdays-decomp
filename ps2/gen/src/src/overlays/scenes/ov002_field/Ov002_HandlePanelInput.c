/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_HandlePanelInput.c (ps2/tools/prep_sources.py). Do not edit. */
/* Dispatch one input on the panel screen.
 *
 * The raw input is normalised and both the old mode and the new code are run
 * through the classifier, which also hands back a column in an out parameter.
 * The new code's class selects the case: a mode change rebuilds the panel, a
 * column move clamps to the last column when the index runs past the count or
 * lands on a 0xff entry, a list move clamps against whichever list is
 * populated, the cancel case disarms the tag tracker and suppresses the sound
 * by forcing the id negative, and the restore case copies back the default
 * kind. The sound is then played, and the mode stored and the panel reapplied.
 *
 * Two arguments, not four: the third and fourth registers are written before
 * any read, and the pushed r3 slot is only ever the classifier's out parameter.
 * The normaliser likewise takes one argument.
 */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct {
    u8 bKind;           /* +0 */
    u8 bMode;           /* +1 */
    u8 bIndex;          /* +2 */
    u8 bPanelRow;       /* +3 */
    u8 bKey;            /* +4 */
    u8 pad0005[2];
    u8 bSpare7;         /* +7 */
    u8 pad0008[0x28];
    u8 bColumns;        /* +0x30 */
    u8 bRowSpan;        /* +0x31 */
    u8 aBitIndex[2];    /* +0x32, stride 2 */
} Ov002PanelSession;

extern Ov002PanelSession *data_ov002_0207f620;

extern void Ov002_PanelClearItemRows(void);
extern int Ov002_RebuildPanelSlots(int nRaw);
extern int Ov002_ClassifyCode(int *pOut, int nCode);
extern void Ov002_PanelRefreshAllRowHeaders(void);
extern void Ov002_PanelApplyCursorMove(int nKind, int nKindAgain);
extern int Ov002_Ctx_FindActiveEntryByTag(int nTag);
extern void Ov002_Ctx_SetTagTrackerNodeArmed_5(int nEntry, int bArmed);
extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern void Ov002_PanelRepaintGroup(int nColumn);
extern void Ov002_DrawListSpanStrip(int nX, int nY, int nW, int nH);
extern int Ov002_CountPanelListEntries(void);
extern int Ov002_CountSecondListEntries(void);
extern void Ov002_PanelRepaintListGroup(int nCode);
extern void Ov002_PanelRepaintSubListGroup(int nColumn);
extern void Ov002_RedrawPartyStrip(void);
extern void Ov002_PanelRepaintCachedEntry(void);
extern void Ov002_PanelRepaintForKind(int nCode, int nKind, int nFlag);

void Ov002_HandlePanelInput(int nRaw, int nSound) {
    int nColumn;
    Ov002PanelSession *s = data_ov002_0207f620;
    int nCode;
    int nPrev;

    if (s == 0) {
        return;
    }

    Ov002_PanelClearItemRows();
    nCode = Ov002_RebuildPanelSlots(nRaw);
    nPrev = Ov002_ClassifyCode(0, s->bMode);

    switch (Ov002_ClassifyCode(&nColumn, nCode)) {
    case 0:
        switch (Ov002_ClassifyCode(0, s->bMode)) {
        case 1:
            s->bKind = 1;
            break;
        case 2:
        case 3:
            s->bKind = 2;
            break;
        case 4:
            s->bKind = 0;
            break;
        }
        Ov002_PanelRefreshAllRowHeaders();
        s->bMode = (u8)nCode;
        Ov002_PanelApplyCursorMove(s->bKind, s->bKind);
        Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(2), 1);
        break;

    case 1:
        s->bIndex = (u8)(s->bKind + nColumn * 6);
        if (s->bIndex >= s->bColumns ||
            *(u8 *)((u8 *)s + s->bIndex * 2 + 0x32) == 0xff) {
            s->bIndex = (u8)(s->bColumns - 1);
        }
        s->bKind = (u8)((unsigned long long)kh_rt_s32_divmod(s->bIndex, 6) >> 32);
        Ov002_PanelRepaintGroup(nColumn);
        Ov002_DrawListSpanStrip(nColumn + 1, s->bRowSpan, 7, 0xb);
        Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(2), 1);
        break;

    case 2:
        s->bPanelRow = (u8)(s->bKind + nColumn * 6);
        if (Ov002_CountPanelListEntries() > 0) {
            if (s->bPanelRow >= Ov002_CountPanelListEntries()) {
                s->bPanelRow = (u8)(Ov002_CountPanelListEntries() - 1);
            }
        } else {
            if (s->bKey >= Ov002_CountSecondListEntries()) {
                s->bKey = (u8)(Ov002_CountSecondListEntries() - 1);
            }
        }
        s->bKind =
            (u8)((unsigned long long)kh_rt_s32_divmod(s->bPanelRow, 6) >> 32);
        Ov002_PanelRepaintListGroup(nCode);
        Ov002_DrawListSpanStrip(nColumn + 1,
                            *(u8 *)((u8 *)s + 0x4ac) +
                                *(u8 *)((u8 *)s + 0x4ad),
                            7, 0xb);
        Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(2), 1);
        break;

    case 3:
        s->bKey = (u8)(s->bKind + nColumn * 6);
        if (s->bKey >= 1 && s->bKey >= Ov002_CountSecondListEntries()) {
            s->bKey = (u8)(Ov002_CountSecondListEntries() - 1);
        }
        nColumn = (int)kh_rt_s32_divmod(s->bKey, 6);
        s->bKind = (u8)((unsigned long long)kh_rt_s32_divmod(s->bKey, 6) >> 32);
        nCode = nColumn + 6;
        Ov002_PanelRepaintSubListGroup(nColumn);
        Ov002_DrawListSpanStrip(*(u8 *)((u8 *)s + 0x4ac) + nColumn + 1,
                            *(u8 *)((u8 *)s + 0x4ac) +
                                *(u8 *)((u8 *)s + 0x4ad),
                            7, 0xb);
        Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(2), 1);
        break;

    case 4:
        Ov002_RedrawPartyStrip();
        s->bMode = (u8)nCode;
        Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(2), 0);
        nSound = -1;
        break;

    case 5:
        s->bKind = s->bSpare7;
        Ov002_PanelRepaintCachedEntry();
        break;
    }

    if (nPrev != 4 && nSound >= 0) {
        if (nSound <= 7) {
            PlaySound(0, nSound);
        } else {
            PlaySoundChecked(0, nSound);
        }
    }
    s->bMode = (u8)nCode;
    Ov002_PanelRepaintForKind(nCode, s->bKind, 1);
}

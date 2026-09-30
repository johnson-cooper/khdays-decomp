/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_PanelAddSubEntryAndRepaint.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov002_PanelAddSubEntryAndRepaint - add a sub-entry, then repaint what it touched.
 *
 * The entry is added first and its handle kept for the return. The panel's
 * current mode is refreshed, then the classifier is run over the caller's code
 * to decide how much of the screen has to be redrawn: a column move repaints
 * one strip of the list, and a list move first disarms the tag tracker unless
 * something still holds it, then repaints the sub-list group, its strip, and
 * the row the running key falls on.
 *
 * THUMB. Two arguments, not four: the third and fourth registers are never read,
 * and the pushed r3 slot is only ever the classifier's out parameter - the
 * prologue pushes r3 as the cheap way to reserve that word.
 */

#include "nitro/types.h"

typedef struct {
    u8 bKind;                           /* +0x00 */
    u8 bMode;                           /* +0x01 */
    u8 bIndex;                          /* +0x02 */
    u8 bPanelRow;                       /* +0x03 */
    u8 bKey;                            /* +0x04 */
} Ov002PanelSession;

extern Ov002PanelSession *data_ov002_0207f620;

extern int Ov002_PanelAddSubEntry(unsigned int nKey, int nTag);
extern void Ov002_PanelRefreshCurrentMode(void);
extern int Ov002_ClassifyCode(int *pnCode, int nMode);
extern void Ov002_DrawListSpanStrip(int nCount, int nRows, int nX, int nY);
extern int Ov002_Ctx_FindActiveEntryByTag(int nTag);
extern int Ov002_ForwardToSubDc_4(int nEntry);
extern int Ov002_ForwardToSubDc(int nId);
extern int Ov002_ForwardToSubDc_2(int nHandle);
extern void Ov002_PanelRepaintSubListGroup(int nGroup);
extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern void Ov002_PanelRepaintForKind(int nMode, int nKind, int nFlag);

int Ov002_PanelAddSubEntryAndRepaint(unsigned int nKey, int nTag)
{
    Ov002PanelSession *pSess;
    int nHandle;
    int nBase;
    int nColumn;

    pSess = data_ov002_0207f620;
    nHandle = Ov002_PanelAddSubEntry(nKey, nTag);
    Ov002_PanelRefreshCurrentMode();

    switch (Ov002_ClassifyCode(&nColumn, pSess->bMode)) {
    case 2:
        Ov002_DrawListSpanStrip(nColumn + 1,
                            *(u8 *)((u8 *)pSess + 0x4ac) +
                                *(u8 *)((u8 *)pSess + 0x4ad),
                            7, 0xb);
        break;
    case 3:
        if (Ov002_ForwardToSubDc_4(Ov002_Ctx_FindActiveEntryByTag(0xe)) == 0) {
            Ov002_ForwardToSubDc_2(Ov002_ForwardToSubDc(0x79));
        }
        Ov002_PanelRepaintSubListGroup(nColumn);
        nBase = *(u8 *)((u8 *)pSess + 0x4ac);
        Ov002_DrawListSpanStrip(nBase + nColumn + 1,
                            nBase + *(u8 *)((u8 *)pSess + 0x4ad),
                            7, 0xb);
        Ov002_PanelRepaintForKind(pSess->bMode,
                            (int)((unsigned long long)kh_rt_s32_divmod(pSess->bKey, 6) >> 32),
                            1);
        break;
    }
    return nHandle;
}

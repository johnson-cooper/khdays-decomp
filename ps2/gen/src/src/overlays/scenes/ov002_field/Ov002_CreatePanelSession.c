/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_CreatePanelSession.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_CreatePanelSession - allocate the panel session and fill it in.
 *
 * The block comes off the root heap, is cleared, and becomes the overlay's one
 * live session. The caller's request names the entries: a bitmask of the
 * fifteen cells that exist, the keys of the entries themselves, and two words
 * copied across untouched. Cells the mask does not claim are stamped 0xff so
 * the drawing code skips them, and the cursor starts on the row that holds the
 * last claimed cell.
 *
 * The twenty-four entry slots are then walked once: every slot the request
 * reaches takes its key, looks the record up in the message database, records
 * whether that record's own field is empty, and joins the list. Slots past the
 * request keep a zero key but still join, so the list length is fixed.
 *
 * Returns the screen step the caller runs from here on.
 *
 * THUMB. The declaration order below is load-bearing: mwcc hands the counter
 * and the walker of each loop their registers by it, and this is the order that
 * gives the ROM's pairing. Each loop also assigns its counter before its
 * walker, which is what fixes the order the two initialisations come out in.
 */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct {
    u16 *pKeys;                         /* +0x00 */
    u16 wCount;                         /* +0x04 */
    u16 pad0006;
    int nCellMask;                      /* +0x08 */
    int nField000c;                     /* +0x0c */
    int aWords[2];                      /* +0x10 */
} Ov002PanelSetup;

typedef struct {
    u8 pad0000[8];
    int nField0008;                     /* +0x008 */
    int nPrimaryValue;                  /* +0x00c */
    int nState;                         /* +0x010 */
    u8 pad0014[0x14];
    int aWords[2];                      /* +0x028 */
    u8 bColumns;                        /* +0x030 */
    u8 bCursorRow;                      /* +0x031 */
    u8 aCells[0x1e];                    /* +0x032 */
} Ov002PanelSession;

typedef int (*Ov002PanelStepFn)(void);

extern Ov002PanelSession *data_ov002_0207f620;
extern int gOv002UiBtlMagicTextPath[];

extern void *NNSi_FndGetCurrentRootHeap(void);
extern void NNS_FndInitList(void *pList, int nLinkOffset);
extern void NNS_FndAppendListObject(void *pList, void *pObject);
extern void MI_CpuFill8(void *pDst, int nValue, unsigned int nSize);
extern long long kh_rt_s32_divmod(int nNum, int nDen);

extern int Ov002_GetPanelField0058(void);
extern void Ov002_InitResourceRecord(void *pSet, const void *pTable);
extern void Ov002_BuildTileRow(void);
extern void Ov002_LoadIconSet(void);
extern void Ov002_PanelRefreshCurrentMode(void);
extern int Ov002_Ctx_FindActiveEntryByTag(int nTag);
extern void Ov002_Ctx_SetTagTrackerNodeArmed_6(int nEntry, int nValue);
extern void Ov002_Ctx_SetTagTrackerNodeArmed_5(int nEntry, int nValue);
extern void Ov002_PanelSetHelpVisible(int nValue);
extern void Ov002_UpdateVisibleColumnMask(int nValue);
extern int Ov002_PanelTickLockTimeout(void);

Ov002PanelStepFn Ov002_CreatePanelSession(Ov002PanelSetup *pReq)
{
    int nOffset;
    int i;
    Ov002PanelSetup *pSrc;
    int nRecord;
    Ov002PanelSession *pCell;
    Ov002PanelSession *pWalk;
    int k;
    Ov002PanelSession *s;
    void *pNode;
    int nCount;

    nCount = 0;
    nRecord = 0;
    s = (Ov002PanelSession *)NNSi_FndGetCurrentRootHeap();
    data_ov002_0207f620 = s;
    MI_CpuFill8(s, 0, 0x638);
    s->nPrimaryValue = (Ov002_GetPanelField0058() == 0);
    s->nState = 0;

    pSrc = pReq;
    pWalk = s;
    i = 0;
    do {
        i++;
        pWalk->aWords[0] = pSrc->aWords[0];
        pSrc = (Ov002PanelSetup *)((char *)pSrc + 4);
        pWalk = (Ov002PanelSession *)((char *)pWalk + 4);
    } while (i < 2);

    k = 0;
    pCell = s;
    do {
        if (pReq->nCellMask & (1 << k)) {
            pCell->aCells[0] = (u8)k;
            pCell = (Ov002PanelSession *)((char *)pCell + 2);
            nCount++;
        }
        k++;
    } while (k < 0xf);

    *(int *)((char *)s + 0x4a8) = pReq->nCellMask;
    s->nField0008 = pReq->nField000c;
    s->bColumns = (u8)nCount;
    s->bCursorRow = (u8)kh_rt_s32_divmod(nCount + 5, 6);

    if (nCount < 0xf) {
        pWalk = (Ov002PanelSession *)((char *)s + nCount * 2);
        do {
            pWalk->aCells[0] = 0xff;
            nCount++;
            pWalk = (Ov002PanelSession *)((char *)pWalk + 2);
        } while (nCount < 0xf);
    }

    NNS_FndInitList((char *)s + 0x480, 0xc);
    NNS_FndInitList((char *)s + 0x48c, 0xc);
    MI_CpuFill8((char *)s + 0x50, 0, 0x240);
    *(u16 *)((char *)s + 0x4ae) = pReq->wCount;
    MsgDb_LoadDb(0x15, 0xe);

    i = 0;
    nOffset = 0;
    pWalk = s;
    pNode = (char *)s + 0x50;
    do {
        if (i < pReq->wCount) {
            *(u16 *)((char *)pWalk + 0x4b8) = *(u16 *)((char *)pReq->pKeys + nOffset);
            MsgDb_FetchRecord(&nRecord, 0x15, *(u16 *)((char *)pWalk + 0x4b8), 0xe);
            *(int *)((char *)pWalk + 0x4bc) = (*(int *)(nRecord + 0x18) == 0);
            *(int *)((char *)s + 0x4b0) = *(int *)((char *)s + 0x4b0) | (1 << i);
            DispatchByNodeKind(&nRecord);
        } else {
            *(u16 *)((char *)pWalk + 0x4b8) = 0;
        }
        *(int *)((char *)s + 0x4b4) = *(int *)((char *)s + 0x4b0);
        NNS_FndAppendListObject((char *)s + 0x48c, pNode);
        nOffset += 2;
        pWalk = (Ov002PanelSession *)((char *)pWalk + 0xc);
        pNode = (char *)pNode + 0x18;
        i++;
    } while (i < 0x18);

    ResSlot_Release_2(0x15);
    NNS_FndInitList((char *)s + 0x498, 0xc);

    i = 0;
    pWalk = s;
    do {
        *(int *)((char *)pWalk + 0x298) = 1;
        pWalk = (Ov002PanelSession *)((char *)pWalk + 0x18);
        i++;
    } while (i < 0x12);

    Ov002_InitResourceRecord((char *)s + 0x5e8, gOv002UiBtlMagicTextPath);
    Ov002_BuildTileRow();
    Ov002_LoadIconSet();
    Ov002_PanelRefreshCurrentMode();

    i = Ov002_Ctx_FindActiveEntryByTag(2);
    Ov002_Ctx_SetTagTrackerNodeArmed_6(i, 0);
    Ov002_Ctx_SetTagTrackerNodeArmed_5(i, 1);
    Ov002_Ctx_SetTagTrackerNodeArmed_6(Ov002_Ctx_FindActiveEntryByTag(2), 0);

    if (Ov002_GetPanelField0058() != 0) {
        Ov002_PanelSetHelpVisible(0);
        Ov002_UpdateVisibleColumnMask(0);
    }
    return Ov002_PanelTickLockTimeout;
}

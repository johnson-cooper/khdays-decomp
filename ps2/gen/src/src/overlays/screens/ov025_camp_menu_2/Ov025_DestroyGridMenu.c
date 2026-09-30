/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_DestroyGridMenu.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov025_DestroyGridMenu -- Ov008_DestroyGridMenu: tear the panel grid menu down.
 * The tracked grid cells are cleared, the text loader (+0x1e68) shut, the
 * widget lists and the embedded sub-objects (0205f8d0) freed, the reveal mask
 * rebuilt and every tracked node unlinked (list at +0x1e7c).  The item ids of
 * the 3 x 40 page slots (+0x19c4) are then saved to GameState equippedItems
 * (0xee0, 0 for an empty slot); the summary (+0x1f78) and the variable text
 * records (+0x28c) are released, the main display's mode bits (DISPCNT bits
 * 13-15) cleared, the icon texture archive (+0x364) freed, block 9500 and the
 * context reset, the scratch buffers freed, resource slots 0x15, 0x14, 0x1b,
 * 0x16, 0x13 and the slot at +0x209c dereferenced, the pending bit ranges
 * forwarded and the context callback word cleared.
 */

#include "nitro/types.h"

#define GRID_PAGES 3
#define PAGE_SLOTS 40
#define REG_DISPCNT (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0))
#define DISPCNT_MODE_BITS 0xe000

typedef struct Ov008Message15Record {
    u8  pad_00[0x14];
    int nItemId;              /* 0x14 */
    u8  pad_18[0x9c - 0x18];
} Ov008Message15Record;

typedef struct Ov008GridSummary {
    u8 pad[0x100];
} Ov008GridSummary;

typedef struct Ov008MenuContext {
    u8  pad_0000[0x28c];
    u8  records[0x364 - 0x28c];                   /* 0x028c: variable text records */
    void *pIconArchive;                           /* 0x0364 */
    u8  pad_0368[0x19c4 - 0x368];
    Ov008Message15Record *apPageSlot[GRID_PAGES][PAGE_SLOTS]; /* 0x19c4 */
    u8  pad_1ba4[0x1e68 - 0x1ba4];
    u8  textLoader[0x14];                         /* 0x1e68 */
    u8  trackedNodeList[12];                      /* 0x1e7c */
    u8  pad_1e88[0x1f78 - 0x1e88];
    Ov008GridSummary summary;                     /* 0x1f78 */
    u8  pad_2078[0x209c - 0x2078];
    int nResourceSlot;                            /* 0x209c */
} Ov008MenuContext;

typedef struct GameState {
    u8  pad_0000[0xee0];
    u16 equippedItems[GRID_PAGES][PAGE_SLOTS];    /* 0x0ee0 */
} GameState;

extern GameState *gGameState;
extern void  Ov025_ClearTrackedGridCells(Ov008MenuContext *pCtx);                /* Ov008_ClearTrackedGridCells */
extern void  Ov025_InvokeMethod8(void *pObject);                          /* ov008_InvokeMethod8 */
extern void  Ov025_FreeAllWidgetLists(Ov008MenuContext *pCtx);                /* Ov008_FreeAllWidgetLists */
extern void  Ov025_ReleaseSubObjects(Ov008MenuContext *pCtx);                /* free the embedded sub-objects */
extern void  Ov025_RebuildPanelRevealMask(Ov008MenuContext *pCtx);                /* Ov008_RebuildPanelRevealMask */
extern void *NNS_FndGetNextListObject(void *pList, void *pObject);
extern void  Ov025_RemoveAndFreeBlock(Ov008MenuContext *pCtx, void *pNode);   /* unlink a tracked node */
extern void  func_ov025_02087254(Ov008GridSummary *pSummary);    /* release a summary */
extern void  Ov025_FreeResourceRecordBuffer(void *pRecords);                         /* release a text cache */
extern void  NNSi_FndFreeFromDefaultHeap(void *pBlock);
extern void *Ov025_GetCtxBlock9500(void);                                   /* Ov008_GetCtxBlock9500 */
extern void *Ov025_GetContext(void);                                   /* Ov008_GetContext */
extern void  Ov025_SweepElements(void *pBlock);
extern void  Ov025_DestroyAllListObjects(void *pContext);                         /* Ov008_Set_46ec */
extern void  Ov025_ReleaseIfMarked(void *pContext);
extern void  Ov025_FreeFiveBuffers(Ov008MenuContext *pCtx);                /* Ov008_FreeScratchBuffers */
extern void  Ov025_DecRefSlot(int nSlot);                              /* Ov008_DecRefSlot */
extern void  Ov025_ForwardSetBitsInRange(Ov008MenuContext *pCtx);                /* Ov008_ForwardSetBitsInRange */
extern void  Ov025_StoreWordAt0x4a50(void *pContext, void *pCallback);        /* ov008_StoreWordAt0x4a50 */

void Ov025_DestroyGridMenu(Ov008MenuContext *pCtx)
{
    void *pNode;
    void *pNext;
    int nPage;
    int i;
    Ov008Message15Record *pRecord;
    void *pBlock;
    void *pContext;

    Ov025_ClearTrackedGridCells(pCtx);
    Ov025_InvokeMethod8(pCtx->textLoader);
    Ov025_FreeAllWidgetLists(pCtx);
    Ov025_ReleaseSubObjects(pCtx);
    Ov025_RebuildPanelRevealMask(pCtx);
    pNode = NNS_FndGetNextListObject(pCtx->trackedNodeList, 0);
    while (pNode != 0) {
        pNext = NNS_FndGetNextListObject(pCtx->trackedNodeList, pNode);
        Ov025_RemoveAndFreeBlock(pCtx, pNode);
        pNode = pNext;
    }
    for (nPage = 0; nPage < GRID_PAGES; nPage++) {
        for (i = 0; i < PAGE_SLOTS; i++) {
            pRecord = pCtx->apPageSlot[nPage][i];
            if (pRecord != 0) {
                gGameState->equippedItems[nPage][i] = pRecord->nItemId;
            } else {
                gGameState->equippedItems[nPage][i] = 0;
            }
        }
    }
    func_ov025_02087254(&pCtx->summary);
    Ov025_FreeResourceRecordBuffer(pCtx->records);
    REG_DISPCNT &= ~DISPCNT_MODE_BITS;
    if (pCtx->pIconArchive != 0) {
        NNSi_FndFreeFromDefaultHeap(pCtx->pIconArchive);
        pCtx->pIconArchive = 0;
    }
    pBlock = Ov025_GetCtxBlock9500();
    pContext = Ov025_GetContext();
    Ov025_SweepElements(pBlock);
    Ov025_DestroyAllListObjects(pContext);
    Ov025_ReleaseIfMarked(pContext);
    Ov025_FreeFiveBuffers(pCtx);
    Ov025_DecRefSlot(0x15);
    Ov025_DecRefSlot(0x14);
    Ov025_DecRefSlot(0x1b);
    Ov025_DecRefSlot(0x16);
    Ov025_DecRefSlot(0x13);
    Ov025_DecRefSlot(pCtx->nResourceSlot);
    Ov025_ForwardSetBitsInRange(pCtx);
    Ov025_StoreWordAt0x4a50(pContext, 0);
}

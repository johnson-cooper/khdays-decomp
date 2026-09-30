/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_MissionMenuDestroy.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov025_MissionMenuDestroy -- Ov008_MissionMenuDestroy: tear the mission menu
 * down.  During a transfer the global int array c22c is cleared first.  The
 * mission list (+0x548) is destroyed, the two text caches (+0x530, +0x53c)
 * released, the +0x174 block freed, resource slots 0x15, 0x19 and 0x1a
 * dereferenced; the five tag-tracker nodes named by data_ov025_020b46e8
 * (1, 2, 11, 12, 13, copied to the stack) are disarmed in context block 954c;
 * the five row lists (+0xc, stride 0x3c) drop their sub-buffers; block 954c's
 * element list is cleared; widgets 0x40 and 0x41 of block 4a80 are hidden;
 * the list node at +0x170 is removed and freed; and the sub engine's BG2 /
 * BG3 scroll registers are zeroed.
 */

#include "nitro/types.h"

#define TAG_COUNT 5
#define WIDGET_A  0x40
#define WIDGET_B  0x41

typedef struct Ov008RowList {
    u8 pad[0x3c];
} Ov008RowList;

typedef struct Ov008MissionMenu {
    u8  pad_000[0xc];
    Ov008RowList aRowList[5]; /* 0x00c */
    u8  pad_138[0x150 - 0x138];
    int bTransfer;            /* 0x150 */
    u8  pad_154[0x170 - 0x154];
    void *pListNode;          /* 0x170 */
    u8  pad_174[0x530 - 0x174];
    u8  textCacheA[0xc];      /* 0x530 */
    u8  textCacheB[0xc];      /* 0x53c */
    u8  missionList[4];       /* 0x548 */
} Ov008MissionMenu;

typedef struct Ov008TagTable {
    int aTag[TAG_COUNT];
} Ov008TagTable;

extern const Ov008TagTable data_ov025_020b46e8;
extern void ClearGlobalArrayInt(int nFlag);                                   /* ClearGlobalArrayInt_c22c */
extern void Ov025_DestroyMissionList(void *pList);                           /* Ov008_DestroyMissionList */
extern void Ov025_FreeResourceRecordBuffer(void *pCache);                          /* release a text cache */
extern void Ov025_FreeDetailBuffer(Ov008MissionMenu *pMenu);               /* free the +0x174 block */
extern void Ov025_DecRefSlot(int nSlot);                             /* Ov008_DecRefSlot */
extern int  Ov025_GetCtxBlock954c(void);                                  /* Ov008_GetCtxBlock954c */
extern int  Ov025_FindActiveEntryByTag(int nOwner, u32 nTag);                  /* ov008_FindActiveEntryByTag */
extern void Ov025_SetTagTrackerNodeArmed(int nOwner, int nEntry, int bArmed);    /* SetTagTrackerNodeArmed */
extern void FreeAllListNodeSubBuffers(void *pList);                                 /* FreeAllListNodeSubBuffers */
extern void Ov025_ClearElementList(int *pBlock);                           /* ClearElementList */
extern int  Ov025_GetBlock4a80(void);                                  /* Ov008_GetCtxBlock4a80 */
extern void *Ov025_FindEntryById(int nCtx, int nId);                    /* FindEntryById */
extern void Ov025_SetEntrySlotsVisible(int nCtx, void *pEntry, int bVisible);  /* SetEntrySlotsVisible */
extern void Ov025_ListRemoveAndFree(void *pNode);                           /* Ov008_ListRemoveAndFree */

void Ov025_MissionMenuDestroy(Ov008MissionMenu *pMenu)
{
    Ov008TagTable tags;
    int i;
    int nOwner;
    int nCtx;
    volatile int *pScroll = (volatile int *)((unsigned int)kh_ds_io + 0x1018);

    if (pMenu->bTransfer != 0) {
        ClearGlobalArrayInt(0);
    }
    Ov025_DestroyMissionList(pMenu->missionList);
    Ov025_FreeResourceRecordBuffer(pMenu->textCacheA);
    Ov025_FreeResourceRecordBuffer(pMenu->textCacheB);
    Ov025_FreeDetailBuffer(pMenu);
    Ov025_DecRefSlot(0x15);
    Ov025_DecRefSlot(0x19);
    Ov025_DecRefSlot(0x1a);
    tags = data_ov025_020b46e8;
    nOwner = Ov025_GetCtxBlock954c();
    for (i = 0; i < TAG_COUNT; i++) {
        Ov025_SetTagTrackerNodeArmed(nOwner, Ov025_FindActiveEntryByTag(nOwner, (u16)tags.aTag[i]), 0);
    }
    FreeAllListNodeSubBuffers(&pMenu->aRowList[0]);
    FreeAllListNodeSubBuffers(&pMenu->aRowList[1]);
    FreeAllListNodeSubBuffers(&pMenu->aRowList[2]);
    FreeAllListNodeSubBuffers(&pMenu->aRowList[3]);
    FreeAllListNodeSubBuffers(&pMenu->aRowList[4]);
    Ov025_ClearElementList((int *)Ov025_GetCtxBlock954c());
    nCtx = Ov025_GetBlock4a80();
    Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, WIDGET_A), 0);
    Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, WIDGET_B), 0);
    Ov025_ListRemoveAndFree(pMenu->pListNode);
    pScroll[0] = 0;
    pScroll[1] = 0;
}

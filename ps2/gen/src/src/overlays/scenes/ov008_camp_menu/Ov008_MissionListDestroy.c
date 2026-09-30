/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_MissionListDestroy.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_MissionListDestroy -- Ov008_MissionListDestroy: tear the mission list
 * down.  Mode (+0x504) becomes 1, the list node (+0x80) is removed and
 * freed, the global int array c22c cleared while the entry gate (+0x40) is
 * set, the mission list (+0x4d4) destroyed, the two text caches (+0x4bc,
 * +0x4c8) released, the list's helper state reset (0206fcbc), the 3 x 6 row
 * surfaces (+0x84 / +0x1ec / +0x354, 0x3c each) drop their sub-buffers, the
 * six tag-tracker nodes of data_ov008_0208f8f8 are disarmed in block 954c
 * and its element list cleared, widgets 4 / 5 of the context and 0x3d, 0x3e,
 * 0x3f, 1, 0x47, 0x48 of block 4a80 hidden, and the sub engine's BG1 / BG3
 * scroll registers zeroed.
 */

#include "nitro/types.h"

#define ROW_COUNT 6
#define TAG_COUNT 6

typedef struct TileSurface {
    u8 pad[0x3c];
} TileSurface;

typedef struct Ov008MissionList {
    u8  pad_000[0x40];
    int bEntryGate;           /* 0x040 */
    u8  pad_044[0x80 - 0x44];
    void *pListNode;          /* 0x080 */
    TileSurface aRowNameSurface[ROW_COUNT];   /* 0x084 */
    TileSurface aRowInfoSurface[ROW_COUNT];   /* 0x1ec */
    TileSurface aRowExtraSurface[ROW_COUNT];  /* 0x354 */
    u8  textCacheA[0xc];      /* 0x4bc */
    u8  textCacheB[0xc];      /* 0x4c8 */
    u8  missionList[0x504 - 0x4d4]; /* 0x4d4 */
    int nMode;                /* 0x504 */
} Ov008MissionList;

typedef struct Ov008TagTable {
    int aTag[TAG_COUNT];
} Ov008TagTable;

extern const Ov008TagTable data_ov008_0208f8f8;
extern void Ov008_ListRemoveAndFree(void *pNode);                             /* Ov008_ListRemoveAndFree */
extern void ClearGlobalArrayInt(int nFlag);                                     /* ClearGlobalArrayInt_c22c */
extern void Ov008_DestroyMissionList(void *pList);                             /* Ov008_DestroyMissionList */
extern void Ov008_FreeResourceRecordBuffer(void *pCache);                            /* release a text cache */
extern void Ov008_Set_fcbc(Ov008MissionList *pList);                 /* Ov008_Set_fcbc */
extern void FreeAllListNodeSubBuffers(void *pList);                                   /* FreeAllListNodeSubBuffers */
extern int  Ov008_GetCtxBlock954c(void);                                    /* Ov008_GetCtxBlock954c */
extern int  Ov008_FindActiveEntryByTag(int nOwner, u32 nTag);                    /* ov008_FindActiveEntryByTag */
extern void Ov008_SetTagTrackerNodeArmed(int nOwner, int nEntry, int bArmed);      /* SetTagTrackerNodeArmed */
extern void Ov008_ClearElementList(int *pBlock);                             /* ClearElementList */
extern int  Ov008_GetContext(void);                                    /* Ov008_GetContext */
extern void *Ov008_FindEntryById(int nCtx, int nId);                      /* FindEntryById */
extern void Ov008_SetEntrySlotsVisible(int nCtx, void *pEntry, int bVisible);    /* SetEntrySlotsVisible */
extern int  Ov008_GetCtxBlock4a80(void);                                    /* Ov008_GetCtxBlock4a80 */

void Ov008_MissionListDestroy(Ov008MissionList *pList)
{
    Ov008TagTable tags;
    int i;
    int nOwner;
    int nCtx;
    volatile int *pScroll = (volatile int *)((unsigned int)kh_ds_io + 0x1014);

    pList->nMode = 1;
    Ov008_ListRemoveAndFree(pList->pListNode);
    if (pList->bEntryGate != 0) {
        ClearGlobalArrayInt(0);
    }
    Ov008_DestroyMissionList(pList->missionList);
    Ov008_FreeResourceRecordBuffer(pList->textCacheA);
    Ov008_FreeResourceRecordBuffer(pList->textCacheB);
    Ov008_Set_fcbc(pList);
    for (i = 0; i < ROW_COUNT; i++) {
        FreeAllListNodeSubBuffers(&pList->aRowNameSurface[i]);
        FreeAllListNodeSubBuffers(&pList->aRowInfoSurface[i]);
        FreeAllListNodeSubBuffers(&pList->aRowExtraSurface[i]);
    }
    tags = data_ov008_0208f8f8;
    nOwner = Ov008_GetCtxBlock954c();
    for (i = 0; i < TAG_COUNT; i++) {
        Ov008_SetTagTrackerNodeArmed(nOwner, Ov008_FindActiveEntryByTag(nOwner, (u16)tags.aTag[i]), 0);
    }
    Ov008_ClearElementList((int *)Ov008_GetCtxBlock954c());
    nCtx = Ov008_GetContext();
    Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 4), 0);
    Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 5), 0);
    nCtx = Ov008_GetCtxBlock4a80();
    Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x3d), 0);
    Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x3e), 0);
    Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x3f), 0);
    Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 1), 0);
    Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x47), 0);
    Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x48), 0);
    pScroll[0] = 0;
    pScroll[2] = 0;
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_DestroyUiContext.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_DestroyUiContext - tear the overlay's UI context down.
 *
 * Turns the hardware blending off first, then gives back everything the
 * constructor took: the six map buffers, and every node on the list at +0x98
 * along with the six blocks each of those holds. The script runner at +0xdc is
 * shut down through its own four entry points, and the global slot is cleared
 * so nothing can reach the object afterwards.
 *
 * THUMB. Three levers, all about keeping mwcc from adding work: each block is
 * loaded into a temporary and tested there rather than loaded twice, both
 * loops initialise their counter before their walk pointer, and the
 * declaration order is what colours the eight locals the ROM's way.
 */

#include "nitro/types.h"

extern void NNSi_FndFreeFromDefaultHeap(void *pBlock);
extern void *NNS_FndGetNextListObject(void *pList, void *pObj);
extern void Ov002_DrainAndFreeListB0(void);
extern void Ov002_FreeAllNodes(void);
extern void Ov002_SweepReleasePendingElements(void *pRunner);
extern void Ov002_FlushFlaggedElements(void *pRunner, int nFlag);
extern void Ov002_SweepFreeElementBuffers(void *pRunner);
extern void Ov002_ReleaseThreeBuffers(void *pRunner);

extern int data_ov002_0207f60c;

void Ov002_DestroyUiContext(void)
{
    int *pNode;
    unsigned char *pUi;
    unsigned char *pSlot;
    int i;
    int *pEntry;
    int j;
    void *pBlock;
    int *pNext;

    pUi = *(unsigned char **)&data_ov002_0207f60c;
    *(vu16 *)((unsigned int)kh_ds_io + 0x50) = 0;

    pSlot = pUi;
    for (i = 0; i < 6; i++) {
        pBlock = *(void **)(pSlot + 0x58);
        if (pBlock != 0) {
            NNSi_FndFreeFromDefaultHeap(pBlock);
        }
        pSlot = pSlot + 4;
    }

    pNode = (int *)NNS_FndGetNextListObject(pUi + 0x98, 0);
    while (pNode != 0) {
        pNext = (int *)NNS_FndGetNextListObject(pUi + 0x98, pNode);
        for (j = 0, pEntry = pNode; j < 6; j++) {
            pBlock = (void *)pEntry[1];
            if (pBlock != 0) {
                NNSi_FndFreeFromDefaultHeap(pBlock);
            }
            pEntry = pEntry + 1;
        }
        if (pNode != 0) {
            NNSi_FndFreeFromDefaultHeap(pNode);
        }
        pNode = pNext;
    }

    Ov002_DrainAndFreeListB0();
    Ov002_FreeAllNodes();
    Ov002_SweepReleasePendingElements(pUi + 0xdc);
    Ov002_FlushFlaggedElements(pUi + 0xdc, 0);
    Ov002_SweepFreeElementBuffers(pUi + 0xdc);
    Ov002_ReleaseThreeBuffers(pUi + 0xdc);
    *(int *)&data_ov002_0207f60c = 0;
}

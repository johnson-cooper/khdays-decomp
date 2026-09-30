/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_CreateUiContext.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_CreateUiContext - build the overlay's UI context and publish it.
 *
 * Takes the whole 0x128-byte object from the root heap, wipes it, and wires up
 * everything the rest of the overlay reads through the global slot: the three
 * object lists, the script runner at +0xdc with its five-word setup and its two
 * callbacks, the blend tween at +0xc0, the hardware blend registers, the
 * vblank task, and the per-frame step - which is chosen by a global, one build
 * of the game getting a different one.
 *
 * Then each of the six map slots gets its enable flag from the caller's mask
 * and a cleared 0x800-byte buffer, and the item resource table is pointed at
 * the context's own +0x54 so the accessors reach the flags and buffers through
 * one indirection.
 *
 * THUMB. Returns the per-frame entry point the caller installs.
 */

extern void *NNSi_FndGetCurrentRootHeap(void);
extern void MI_CpuFill8(void *pDst, int nValue, unsigned int nSize);
extern void NNS_FndInitList(void *pList, int nLinkOffset);
extern void Ov002_Container_Init(void *pRunner, int *pSetup);
extern void Ov002_StorePairAt0x44(void *pRunner, void *pfnA, void *pfnB);
extern void Tween_Clear(void *pTween);
extern void G2x_SetBlendAlpha_(int nReg, int nFirst, int nSecond, int nEva,
                               int nEvb);
extern void RegisterNamedTask(int nSlot, const char *pName, void *pfnTask);
extern int LoadGlobalU16At0(void);
extern void Ov002_CreateStepNode(void *pfnStep);
extern void Ov002_LoadAndInitResourceSections(void *pRunner, char *pName);
extern void *NNS_FndAllocFromDefaultExpHeapEx(unsigned int nSize, int nAlign);
extern void MIi_CpuClear16(int nValue, void *pDst, unsigned int nSize);

extern void Ov002_UiBlitTilesA(void);
extern void Ov002_UiBlitTilesB(void);
extern void Ov002_SelectEntryByKey(void);
extern void Ov002_Ui_GetListHead(void);
extern void Ov002_RunQueuedSteps(void);
extern void Ov002_CommitDisplayState(void);
extern void Ov002_UiTweenCallback(void);
extern void Ov002_ResetMenuRoot(void);

extern char gOv002BguivbfuncName[];
extern int data_ov002_0207f60c;

void *Ov002_CreateUiContext(int *pDesc)
{
    unsigned char *pUi;
    int i;
    unsigned char *pSlot;
    int aSetup[5];

    pUi = (unsigned char *)NNSi_FndGetCurrentRootHeap();
    *(unsigned char **)&data_ov002_0207f60c = pUi;
    MI_CpuFill8(pUi, 0, 0x128);
    *(int *)(pUi + 0xbc) = -1;

    NNS_FndInitList(pUi + 0x98, 0x34);
    NNS_FndInitList(pUi + 0xa4, 4);
    NNS_FndInitList(pUi + 0xb0, 8);

    aSetup[0] = 0x140;
    aSetup[1] = 0x1c;
    aSetup[2] = 0xc;
    aSetup[3] = (int)Ov002_UiBlitTilesA;
    aSetup[4] = (int)Ov002_UiBlitTilesB;
    Ov002_Container_Init(pUi + 0xdc, aSetup);
    Ov002_StorePairAt0x44(pUi + 0xdc, Ov002_SelectEntryByKey, Ov002_Ui_GetListHead);

    *(int *)pUi = 0;
    pUi[0x11] = 0;
    *(int *)(pUi + 8) = pDesc[2];
    Tween_Clear(pUi + 0xc0);

    G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x50), 8, 0x21, 0, 0x10);
    RegisterNamedTask(1, gOv002BguivbfuncName, Ov002_RunQueuedSteps);

    if (LoadGlobalU16At0() != 0x2a) {
        Ov002_CreateStepNode(Ov002_CommitDisplayState);
    } else {
        Ov002_CreateStepNode(Ov002_UiTweenCallback);
    }

    if (pDesc[0] != 0) {
        Ov002_LoadAndInitResourceSections(pUi + 0xdc, (char *)pDesc[0]);
    }

    for (i = 0, pSlot = pUi; i < 6; i++) {
        *(int *)(pSlot + 0x70) = (*((unsigned char *)pDesc + 4) & 1 << i) != 0;
        *(int *)(pSlot + 0x58) = (int)NNS_FndAllocFromDefaultExpHeapEx(0x800, 2);
        MIi_CpuClear16(0, *(void **)(pSlot + 0x58), 0x800);
        pSlot = pSlot + 4;
    }

    *(int *)(pUi + 0x54) = -1;
    *(int *)(pUi + 0x94) = (int)(pUi + 0x54);
    return (void *)Ov002_ResetMenuRoot;
}

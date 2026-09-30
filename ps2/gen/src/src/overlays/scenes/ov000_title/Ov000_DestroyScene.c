/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_DestroyScene.c (ps2/tools/prep_sources.py). Do not edit. */
/* Teardown for the ov000 title/menu/Load scene: releases every buffer the scene
 * allocated, blanks the four sub-screen background layers and drops the context
 * pointer, so the next scene starts from a clean slate.
 *
 * Order matters and is the ROM's: mask the display's BG enable bits first (so nothing
 * is scanned out of memory that is about to be freed), then release the var table, the
 * three node lists, the two scene-owned buffer sets and the three selection-object
 * pools; then free the three per-slot buffers and the page buffer from the default
 * heap; then clear the four BG screen maps; then hand the three resource ids back.
 * Publishing NULL to the context global last is what makes the scene unreachable.
 */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov000DestroyContext {
    u8 pad_0000[0x4c];
    u8 aSelectionObject[1];
} Ov000DestroyContext;

typedef struct Ov000SceneContext {
    u8 pad_0000[0x4aec];
    void *apSlotBuffer[3];
    u8 pad_4af8[0x4b04 - 0x4af8];
    void *pPageBuffer;
    u8 pad_4b08[0x4cb4 - 0x4b08];
    u8 aVarTable[1];
} Ov000SceneContext;

extern Ov000SceneContext *data_ov000_0205ac24;

extern void Ov000_FreeResourceRecordBuffer(u8 *table);
extern void FreeAllListNodeSubBuffers(u8 *list);
extern void Ov000_SweepElements(Ov000SceneContext *ctx);
extern void Ov000_ReleaseThreeBuffers(Ov000SceneContext *ctx);
extern void Ov000_DestroyAllListObjects(u8 *obj);
extern void Ov000_ReleaseIfMarked(u8 *obj);
extern void Ov000_DestroyObjectsAndRelease(u8 *obj);
extern void NNSi_FndFreeFromDefaultHeap(void *p);
extern void *G2S_GetBG0ScrPtr(void);
extern void *G2S_GetBG1ScrPtr(void);
extern void *G2S_GetBG2ScrPtr(void);
extern void *G2S_GetBG3ScrPtr(void);
extern void MIi_CpuClearFast(u32 value, void *dst, u32 size);

static volatile u32 *const REG_DISPCNT = (volatile u32 *)((unsigned int)kh_ds_io + 0x0);

void Ov000_DestroyScene(void)
{
    Ov000SceneContext *ctx = data_ov000_0205ac24;
    u8 i;

    *REG_DISPCNT &= ~0xe000;

    Ov000_FreeResourceRecordBuffer((u8 *)data_ov000_0205ac24 + 0x4cb4);
    FreeAllListNodeSubBuffers((u8 *)data_ov000_0205ac24 + 0x4cfc);
    FreeAllListNodeSubBuffers((u8 *)data_ov000_0205ac24 + 0x4cc0);
    FreeAllListNodeSubBuffers((u8 *)data_ov000_0205ac24 + 0x4d38);

    Ov000_SweepElements(ctx);
    Ov000_ReleaseThreeBuffers(ctx);
    Ov000_DestroyAllListObjects(((Ov000DestroyContext *)ctx)->aSelectionObject);
    Ov000_ReleaseIfMarked(((Ov000DestroyContext *)ctx)->aSelectionObject);
    Ov000_DestroyObjectsAndRelease(((Ov000DestroyContext *)ctx)->aSelectionObject);

    for (i = 0; i < 3; i++) {
        if (data_ov000_0205ac24->apSlotBuffer[i] != 0) {
            NNSi_FndFreeFromDefaultHeap(data_ov000_0205ac24->apSlotBuffer[i]);
            data_ov000_0205ac24->apSlotBuffer[i] = 0;
        }
    }
    if (data_ov000_0205ac24->pPageBuffer != 0) {
        NNSi_FndFreeFromDefaultHeap(data_ov000_0205ac24->pPageBuffer);
        data_ov000_0205ac24->pPageBuffer = 0;
    }

    MIi_CpuClearFast(0, G2S_GetBG0ScrPtr(), 0x800);
    MIi_CpuClearFast(0, G2S_GetBG1ScrPtr(), 0x800);
    MIi_CpuClearFast(0, G2S_GetBG2ScrPtr(), 0x800);
    MIi_CpuClearFast(0, G2S_GetBG3ScrPtr(), 0x800);

    GameState_ClearFlag(0x200a);
    GameState_ClearFlag(0x200c);
    GameState_ClearFlag(0x200d);

    data_ov000_0205ac24 = 0;
}

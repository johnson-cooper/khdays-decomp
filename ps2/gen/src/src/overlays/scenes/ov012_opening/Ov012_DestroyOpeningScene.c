/* PS2: mechanically prepared copy of src/overlays/scenes/ov012_opening/Ov012_DestroyOpeningScene.c (ps2/tools/prep_sources.py). Do not edit. */
/* Releases the opening archives, renderer and font resource, clears sound slot 3, unloads ov024,
 * invalidates the opening clip id and restores the LCD swap bit. */

#include "game/engine.h"

extern char *NNSi_FndGetCurrentRootHeap(void);
extern void NNSi_FndFreeFromDefaultHeap(void *block);
extern int func_ov012_0205bb78(void *renderer);

extern unsigned int OVERLAY_24_ID[1];
#define FS_OVERLAY_ID_ov024 ((unsigned int)&OVERLAY_24_ID)

extern int data_ov012_0205c2a0;
extern char *data_ov012_0205cb20;

void Ov012_DestroyOpeningScene(void) {
    char *context;

    context = NNSi_FndGetCurrentRootHeap();
    NNSi_FndFreeFromDefaultHeap(*(void **)(context + 0x8bf8));
    NNSi_FndFreeFromDefaultHeap(*(void **)(context + 0x8bfc));
    func_ov012_0205bb78(context + 0x8b4c);
    FontResource_Destroy(context + 0x8b40);
    StoreGlobalArrayEntry(3, 0);
    UnloadOverlaySync(0, FS_OVERLAY_ID_ov024);
    data_ov012_0205c2a0 = -1;
    data_ov012_0205cb20 = 0;
    *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x304) |= 0x8000;
}

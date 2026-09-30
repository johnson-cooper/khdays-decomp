/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_UpdateTitleFrame.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov011_UpdateTitleFrame -- per-frame title update: step the layout animator, scroll both panes,
 * update their sprite groups, advance the background scroll, drop each pane's pending sprite in
 * once its anchor is on screen, float every visible sprite of both managers upwards (hiding the
 * ones that leave the top), and flush both sprite managers.
 *
 * The scene pointer is the file-defined shared-bss global: through the extern Ov011Globals the
 * reload of pScene is not hoisted above the pos.y store. The second pass counts panes with k and
 * slots with i (the other way round the allocator rotates four registers). */

#include "nitro/types.h"

typedef struct UiLayoutPos {
    int x;
    int y;
} UiLayoutPos;

typedef struct Ov011Pane {
    void *pBuffer;
    u8    pad_00004[0x10908 - 4];
    int   aSlot[4];
    u8    pad_10918[0x1091a - 0x10918];
    u16   wScrollPhase;
    u8    pad_1091c[0x10940 - 0x1091c];
} Ov011Pane;

typedef struct Ov011SpriteManager {
    u8 data[0x4a38];
} Ov011SpriteManager;

typedef struct Ov011Scene {
    int                nA;
    int                nMode;
    u8                 pad_00008[4];
    u32                nArchiveBase;
    u8                 pad_00010[4];
    Ov011Pane          aPane[2];
    u8                 pad_21294[0x23aac - 0x21294];
    int                nBgScroll;
    int                nPrevBgScroll;
    u8                 pad_23ab4[0x23ad0 - 0x23ab4];
    Ov011SpriteManager aManager[2];
} Ov011Scene;

/* khdays: shared-bss */
u32 data_ov011_0205e960;
Ov011Scene *data_ov011_0205e964;

extern void         Ov011_RefreshCurrentPhase(void);
extern void         Ov011_StepPaneScroll(int nPane, int nUnused);
extern void         Ov011_ScrollTitleText(int nPane);
extern UiLayoutPos *Slot_GetPositionPtr(Ov011SpriteManager *pManager, int nSlot);
extern void         Slot_SetPosition(Ov011SpriteManager *pManager, int nSlot, UiLayoutPos *pPos);
extern void         Slot_SetVisible(Ov011SpriteManager *pManager, int nSlot, int bVisible);
extern int          Slot_IsVisible(Ov011SpriteManager *pManager, int nSlot);
extern void         DispObjList_UpdateImmediate(Ov011SpriteManager *pManager);

void Ov011_UpdateTitleFrame(void)
{
    int i;
    int k;
    UiLayoutPos pos;

    Ov011_RefreshCurrentPhase();
    Ov011_StepPaneScroll(0, 1);
    Ov011_StepPaneScroll(1, 1);
    Ov011_ScrollTitleText(0);
    Ov011_ScrollTitleText(1);
    data_ov011_0205e964->nPrevBgScroll = data_ov011_0205e964->nBgScroll;
    data_ov011_0205e964->nBgScroll += 0x94;

    for (i = 0; i < 2; i++) {
        if (data_ov011_0205e964->aPane[i].wScrollPhase != 0) {
            pos = *Slot_GetPositionPtr(&data_ov011_0205e964->aManager[i],
                                 data_ov011_0205e964->aPane[i].aSlot[1]);
            if ((pos.y >> 12) <= 0x80) {
                pos.y += 0x40000;
                Slot_SetPosition(&data_ov011_0205e964->aManager[i],
                              data_ov011_0205e964->aPane[i].aSlot[2], &pos);
                Slot_SetVisible(&data_ov011_0205e964->aManager[i],
                              data_ov011_0205e964->aPane[i].aSlot[2], 1);
                data_ov011_0205e964->aPane[i].wScrollPhase = 0;
            }
        }
    }

    for (k = 0; k < 2; k++) {
        for (i = 0; i < 4; i++) {
            int nSlot = data_ov011_0205e964->aPane[k].aSlot[i];
            Ov011SpriteManager *pManager = &data_ov011_0205e964->aManager[k];

            if (Slot_IsVisible(pManager, nSlot)) {
                pos = *Slot_GetPositionPtr(pManager, nSlot);
                pos.y -= 0x17ae;
                Slot_SetPosition(pManager, nSlot, &pos);
                Slot_SetVisible(pManager, nSlot, pos.y > -0x40000);
            }
        }
    }
    DispObjList_UpdateImmediate(&data_ov011_0205e964->aManager[0]);
    DispObjList_UpdateImmediate(&data_ov011_0205e964->aManager[1]);
}

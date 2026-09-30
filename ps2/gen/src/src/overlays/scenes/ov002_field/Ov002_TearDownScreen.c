/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_TearDownScreen.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov002ScreenCtx {
    char pad000[0xc];
    void *pOverlay;             /* 0x0c */
    void *pBuffer;              /* 0x10 */
} Ov002ScreenCtx;

extern Ov002ScreenCtx *data_ov002_0207fa18;
extern char gOv002UinowldtaskfuncName[];

extern void NNSi_FndFreeFromDefaultHeap(void *pBlock);
extern void Ov002_World_SetField4(int nMode);
extern void Ov002_HoldPanelScreen(int nMode, int nArg);
extern void Ov002_SelectEntry(int nMode);
extern int Ov002_HasAssignedPeerId(void);
extern void *G2_GetBG3ScrPtr(void);
extern void MIi_CpuClearFast(u32 nValue, void *pDest, u32 nSize);
extern void Ov002_SaveOrRestoreLcdSwap(int nMode);

/* Tear the screen down and hand it back.
 *
 * The screen's own slot is released and its work buffer freed; a screen that
 * had brought an overlay up closes that too.  Unless something else is already
 * holding the display, both engines get their four backgrounds turned back on
 * with the objects left off, the BG3 map is cleared and the screen pointer is
 * dropped.
 */
void Ov002_TearDownScreen(void)
{
    vu32 *pDispCnt;
    vu32 *pSubDispCnt;

    VBlank_UnregisterCallback(1, gOv002UinowldtaskfuncName);
    SetMasterBrightnessMain(-16);

    if (data_ov002_0207fa18->pBuffer != 0) {
        NNSi_FndFreeFromDefaultHeap(data_ov002_0207fa18->pBuffer);
        data_ov002_0207fa18->pBuffer = 0;
    }

    if (data_ov002_0207fa18->pOverlay != 0) {
        Ov002_World_SetField4(0);
        Ov002_HoldPanelScreen(0, 0);
        Ov002_SelectEntry(9);
    }

    if (Ov002_HasAssignedPeerId() != 0) {
        pDispCnt = (vu32 *)((unsigned int)kh_ds_io + 0x0);
        *pDispCnt = (*pDispCnt & 0xffffe0ff) | 0xf00;
        pSubDispCnt = (vu32 *)((unsigned int)kh_ds_io + 0x1000);
        *pSubDispCnt = (*pSubDispCnt & 0xffffe0ff) | 0xf00;
    }

    MIi_CpuClearFast(0, G2_GetBG3ScrPtr(), 0x800);
    Ov002_SaveOrRestoreLcdSwap(0);
    data_ov002_0207fa18 = 0;
}

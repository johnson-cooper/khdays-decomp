/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_ReentryState.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov000_ReentryState -- Scene 1 (boot/logo) re-entry scene state, ov000.
 * The initial state fn the ctor returns on a non-fresh entry (arg != 0). Does the
 * one-time sub-screen bring-up: clears BG1/BG2 char, sets the sub graphics mode and
 * BG control regs, re-arms animation player 5, programs the sub blend-alpha, sets BG
 * priorities, runs the object/text init (Ov000_RefreshMenuLayout / Ov000_RegisterLogoObjects)
 * and the scroll-bounds setup (Camera_CommitMatricesEx), then hands off to Ov000_TickBootFadeIn. */

#include "nitro/types.h"
#include "game/engine.h"

typedef void          *StateFn;

extern void *NNSi_FndGetCurrentRootHeap(void);
extern void *G2S_GetBG1CharPtr(void);
extern void *G2S_GetBG2CharPtr(void);
extern void  MIi_CpuClearFast(int val, void *dst, int size);
extern void  GXS_SetGraphicsMode(int);
extern void  Bg_LoadPaletteForScreen(int, void *, void *, int, int);
extern void  G2x_SetBlendAlpha_(int reg, int a, int b, int c, int d);
extern void  Ov000_RefreshMenuLayout(void);
extern void  Ov000_RegisterLogoObjects(void);
extern void  Camera_CommitMatricesEx(void *, int, int, int, int);
extern void  Scene_DrawNode(void *);
extern void  Ov000_TickBootFadeIn(void);

StateFn Ov000_ReentryState(void) {
    int *h = (int *)NNSi_FndGetCurrentRootHeap();
    MIi_CpuClearFast(0, G2S_GetBG1CharPtr(), 0x20);
    MIi_CpuClearFast(0, G2S_GetBG2CharPtr(), 0x20);
    GXS_SetGraphicsMode(0);

    {
        vu16 *sub = (vu16 *)((unsigned int)kh_ds_io + 0x100a);
        vu32 *subd = (vu32 *)((unsigned int)kh_ds_io + 0x1000);
        sub[0] = (sub[0] & 0x43) | 0x4084;
        sub[1] = (sub[1] & 0x43) | 0x4284;
        sub[2] = (sub[2] & 0x43) | 0x404;
        *subd = (*subd & ~0x1f00) | 0x1200;
    }

    Bg_LoadPaletteForScreen(5, (void *)h[0x5f], (void *)h[0x5d], 0, *(int *)(h[0x5f] + 8));
    Gfx_EnqueueTableCmdAt14(5, (void *)h[0x5e], 0, *(int *)(h[0x5e] + 0x10));
    Gfx_EnqueueTableCmdAtC(5, (void *)h[0x5d], 0, *(int *)(h[0x5d] + 8));

    G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 0x10, 0x22, 0x10, 0);

    {
        vu16 *sub = (vu16 *)((unsigned int)kh_ds_io + 0x100a);
        sub[0] = (sub[0] & ~3) | 3;
        sub[1] = (sub[1] & ~3) | 2;
        sub[2] = (sub[2] & ~3) | 1;
    }

    Ov000_RefreshMenuLayout();
    Ov000_RegisterLogoObjects();
    Camera_CommitMatricesEx((void *)(h + 0x45), 0x3b33, -0x3b33, -0x4d9a, 0x4d9a);
    Scene_DrawNode((void *)(h + 3));
    return (StateFn)Ov000_TickBootFadeIn;
}

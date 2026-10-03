/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_MenuFadeInState.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov000_MenuFadeInState -- fade the ov000 menu back in after a re-entry.
 *
 * The state Ov000_HandoffState hands control to on a non-fresh entry into scene 1, i.e.
 * every time the player comes back to the menu from somewhere else.  Both screens start
 * blacked out and this state walks them back up over roughly a second and a half:
 *
 *   - first tick only (layoutReady): run the one-shot setup, main master brightness to 0
 *     and sub to full, DISPCNT display mode 1, seed both animation tracks from the frame
 *     counter, main blend brightness at full, then Ov000_ReleaseLogoResources;
 *   - if Ov000_IsConfirmPressed reports the fade should be skipped, jump straight to the end
 *     state: snap the counter to 0x79 and both tracks to their last frame, clear every
 *     blend, refresh the menu and go to Ov000_TickMenuLoop;
 *   - otherwise ramp.  Below frame 0x3c nothing is shown yet; from 0x3c the main blend
 *     brightness drops one step every two frames until it reaches 0, and the sub screen
 *     master brightness follows the same slope between 0x3c and 0x5c while the two layers
 *     cross-fade.  When the node animation reports done (bit 0 of BuildSlotMask) the entry
 *     tick is stamped and the next state is Ov000_TickMenuLoop; otherwise tick the counter.
 *
 * CODEGEN NOTE -- `level` has to be computed BEFORE the DISPCNT write.  The write goes
 * through a pointer, so mwcc has to assume it can alias ctx->counter and reloads it; with
 * the read first, the value the `>= 0x3c` test already loaded is reused, which is the
 * ROM's one-instruction-shorter form.
 */

#include "nitro/types.h"

typedef void         *StateFn;

typedef struct Ov000MenuContext {
    int       counter;
    u8        pad_0004[8];
    u8        node[0x108];
    u8        scrollBounds[0x4b28];
    int       layoutReady;
    u8        pad_4c40[0x24];
    long long enterTick;
} Ov000MenuContext;

extern Ov000MenuContext *NNSi_FndGetCurrentRootHeap(void);
extern void      Ov000_FadeStateHookNoOp(void);
extern void      Ov000_RestoreReentryGraphics(void);
extern void      SetMasterBrightnessMain(int brightness);
extern void      SetMasterBrightnessSub(int brightness);
extern int       Anim_SetFrameWrapped(void *node, int track, int frame);
extern void      G2x_SetBlendBrightnessExt_(u32 reg, int a, int b, int c, int d, int e);
extern void      G2x_SetBlendAlpha_(u32 reg, int a, int b, int c, int d);
extern void      StampByteAndInvokeSubStructAt(int a, int b);
extern void      Ov000_ReleaseLogoResources(void);
extern int       Ov000_IsConfirmPressed(void);
extern int       Anim_GetLengthQ12(void *node, int track);
extern void      Ov000_FadePaletteToWhite(int level);
extern void      Ov000_RefreshMenuLayout(void);
extern void      Ov000_RegisterLogoObjects(void);
extern void      Camera_CommitMatricesEx(void *bounds, int a, int b, int c, int d);
extern void      Scene_DrawNode(void *node);
extern long long OS_GetTick(void);
extern unsigned short       Sequence_UpdateTracks(void *node, int mask);
extern int       BuildSlotMask(void *node, int mask);
extern void      Ov000_TickMenuLoop(void);

StateFn Ov000_MenuFadeInState(void) {
    Ov000MenuContext *ctx;
    int level;
    vu32 *dispcnt = (vu32 *)((unsigned int)kh_ds_io + 0x0);

    ctx = NNSi_FndGetCurrentRootHeap();
    Ov000_FadeStateHookNoOp();
    if (ctx->layoutReady == 0) {
        Ov000_RestoreReentryGraphics();
        SetMasterBrightnessMain(0);
        SetMasterBrightnessSub(0x10);
        *dispcnt = (*dispcnt & ~0x1f00) | 0x100;
        Anim_SetFrameWrapped(ctx->node, 0, ctx->counter << 12);
        Anim_SetFrameWrapped(ctx->node, 2, ctx->counter << 12);
        G2x_SetBlendBrightnessExt_(((unsigned int)kh_ds_io + 0x50), 2, 2, 0, 0x10, 0);
        StampByteAndInvokeSubStructAt(0, 0);
        Ov000_ReleaseLogoResources();
        ctx->layoutReady = 1;
    }

    if (Ov000_IsConfirmPressed() != 0) {
        ctx->counter = 0x79;
        Anim_SetFrameWrapped(ctx->node, 0, Anim_GetLengthQ12(ctx->node, 0) - 1);
        Anim_SetFrameWrapped(ctx->node, 2, Anim_GetLengthQ12(ctx->node, 2) - 1);
        *dispcnt = (*dispcnt & ~0x1f00) | 0x300;
        G2x_SetBlendBrightnessExt_(((unsigned int)kh_ds_io + 0x50), 2, 0x20, 0, 0, 0);
        SetMasterBrightnessSub(0);
        Ov000_FadePaletteToWhite(0);
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 0x10, 0x22, 0x10, 0);
        Ov000_RefreshMenuLayout();
        Ov000_RegisterLogoObjects();
        Camera_CommitMatricesEx(ctx->scrollBounds, 0x3b33, -0x3b33, -0x4d9a, 0x4d9a);
        Scene_DrawNode(ctx->node);
        kh_write_s64_le_unaligned((u8 *)ctx + 0x4c64, OS_GetTick());
        return (StateFn)Ov000_TickMenuLoop;
    }

    if (ctx->counter >= 0x3c) {
        level = 0x10 - (ctx->counter - 0x3c) / 2;
        *dispcnt = (*dispcnt & ~0x1f00) | 0x300;
        if (level < 0) {
            level = 0;
        }
        G2x_SetBlendBrightnessExt_(((unsigned int)kh_ds_io + 0x50), 2, 2, 0x10 - level, level, 0);
        Ov000_FadePaletteToWhite(level);
    } else {
        G2x_SetBlendBrightnessExt_(((unsigned int)kh_ds_io + 0x50), 2, 0x20, 0, 0, 0);
        Ov000_FadePaletteToWhite(0x10);
    }

    if (ctx->counter < 0x3c) {
        SetMasterBrightnessSub(0x10);
    } else if (ctx->counter <= 0x5c) {
        SetMasterBrightnessSub(0x10 - (ctx->counter - 0x3c) / 2);
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 0x10, 0x22, (ctx->counter - 0x3c) / 2,
                           0x10 - (ctx->counter - 0x3c) / 2);
    } else {
        SetMasterBrightnessSub(0);
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 0x10, 0x22, 0x10, 0);
    }

    Ov000_RefreshMenuLayout();
    Ov000_RegisterLogoObjects();
    Camera_CommitMatricesEx(ctx->scrollBounds, 0x3b33, -0x3b33, -0x4d9a, 0x4d9a);
    Scene_DrawNode(ctx->node);
    Sequence_UpdateTracks(ctx->node, 0x1000);
    if ((BuildSlotMask(ctx->node, 0x1000) & 1) != 0) {
        kh_write_s64_le_unaligned((u8 *)ctx + 0x4c64, OS_GetTick());
        return (StateFn)Ov000_TickMenuLoop;
    }
    ctx->counter = ctx->counter + 1;
    return 0;
}

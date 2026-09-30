/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/Ov000_TickMenuLevelChange.c (ps2/tools/prep_sources.py). Do not edit. */
/* Scene step that cross-fades a menu level change, then hands control back to the menu loop.
 *
 * What the two s8 fields are was settled by reading Ov000_TickMenuLoop (the menu root loop this
 * returns to): it uses +0x4c2d as the current menu DEPTH and indexes the per-level selection array
 * at +0x4c2e with it. So +0x4c2c is a PENDING depth delta -- a push or a pop that has been
 * requested but not applied. This function applies it at step 4, i.e. exactly at the midpoint of
 * the fade, so the level swap happens while the screen is covered.
 *
 * The fade itself is two symmetric halves over 8 frames, alpha = frame * 4:
 *   frames 1..4  EVA = 16-alpha, EVB = alpha    (out)
 *   frames 5..7  EVA = alpha-16, EVB = 32-alpha (back in)
 * 0x04001050 is engine B's REG_BLDALPHA; masks 16/2 select OBJ as first target and BG1 as second.
 *
 * The terminal 0x79 is not arbitrary: the menu loop thresholds the same counter at 0x3c and 0x5d,
 * so 0x79 lands it past both, in the fully-lit steady state.
 */

#include "nitro/types.h"

typedef void (*OverlayCallback)(void);

typedef struct {
    int nStateFrame;      /* +0x00    frame counter, reset on every state change */
    u8 pad_0004[8];
    u8 render_node[0x4c20]; /* +0x0c  scene root node; RenderNode() takes this in 0204e5b0 */
    s8 nPendingDepthDelta; /* +0x4c2c */
    s8 nMenuDepth;         /* +0x4c2d */
} Ov000SceneContext;

extern Ov000SceneContext *NNSi_FndGetCurrentRootHeap(void);
extern void G2x_SetBlendAlpha_(int reg, int first_mask, int second_mask,
                               int eva, int evb);
extern void Ov000_RefreshMenuLayout(void);
extern void Ov000_RegisterLogoObjects(void);
extern void Scene_DrawNode(void *object);
extern void Ov000_TickMenuLoop(void);

OverlayCallback Ov000_TickMenuLevelChange(void) {
    OverlayCallback next = 0;
    Ov000SceneContext *context = NNSi_FndGetCurrentRootHeap();
    int alpha;

    context->nStateFrame++;
    if (context->nStateFrame == 4) {
        context->nMenuDepth += context->nPendingDepthDelta;
        context->nPendingDepthDelta = 0;
    }

    if (context->nStateFrame <= 4) {
        alpha = context->nStateFrame << 2;
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 16, 2, 16 - alpha, alpha);
    } else if (context->nStateFrame < 8) {
        alpha = context->nStateFrame << 2;
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 16, 2, alpha - 16, 32 - alpha);
    } else {
        context->nStateFrame = 0x79;
        next = Ov000_TickMenuLoop;
    }

    Ov000_RefreshMenuLayout();
    Ov000_RegisterLogoObjects();
    Scene_DrawNode(context->render_node);
    return next;
}

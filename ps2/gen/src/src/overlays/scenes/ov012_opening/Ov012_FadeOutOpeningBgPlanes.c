/* PS2: mechanically prepared copy of src/overlays/scenes/ov012_opening/Ov012_FadeOutOpeningBgPlanes.c (ps2/tools/prep_sources.py). Do not edit. */
/* Fades the selected opening background planes out of the visible mask across 16 steps, updates
 * DISPCNT/BLDCNT, and advances when the fade completes. */

#include "nitro/types.h"

typedef struct Ov012OpeningEvent {
    u32 nTriggerThreadDelta;
    u16 uHandlerIndex;
    u16 uArgument0;
    u16 uArgument1;
    u16 uArgument2;
} Ov012OpeningEvent;

extern int Math_DivMod(int numerator, u32 denominator);
extern void G2x_SetBlendAlpha_(volatile u16 *pBlendControl, u32 planeA,
                               u32 planeB, int eva, int evb);

#define REG_DISPCNT (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0))
#define REG_BLDCNT  (*(volatile u16 *)((unsigned int)kh_ds_io + 0x50))

int Ov012_FadeOutOpeningBgPlanes(void *pContext, u32 nThreadDelta,
                             Ov012OpeningEvent *pEvent) {
    char *context;
    int nBlendStep;
    u32 uRemainingBgPlaneMask;

    context = (char *)pContext;
    nBlendStep = Math_DivMod(
        (nThreadDelta - pEvent->nTriggerThreadDelta) << 4,
        pEvent->uArgument1);
    uRemainingBgPlaneMask =
        *(u8 *)(context + 0x8bf0) & ~pEvent->uArgument0;

    if (uRemainingBgPlaneMask == 0) {
        if (nBlendStep >= 16) {
            REG_DISPCNT &= ~0x1f00;
            *(u8 *)(context + 0x8bf0) = 0;
            *(int *)(context + 0x8bf4) = -16;
            return 1;
        }
        *(int *)(context + 0x8bf4) = -nBlendStep;
        return 0;
    }

    if (nBlendStep >= 16) {
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) |
                      ((*(u8 *)(context + 0x8bf0) = uRemainingBgPlaneMask) << 8);
        REG_BLDCNT = 0;
        return 1;
    }

    G2x_SetBlendAlpha_(&REG_BLDCNT, pEvent->uArgument0,
                       uRemainingBgPlaneMask, 16 - nBlendStep, nBlendStep);
    return 0;
}

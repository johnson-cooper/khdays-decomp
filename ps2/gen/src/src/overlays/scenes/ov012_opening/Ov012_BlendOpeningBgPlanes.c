/* PS2: mechanically prepared copy of src/overlays/scenes/ov012_opening/Ov012_BlendOpeningBgPlanes.c (ps2/tools/prep_sources.py). Do not edit. */
/* Cross-fades a new opening background-plane mask over the current mask across the event duration,
 * updates DISPCNT/BLDCNT, and advances when the 16-step blend completes. */

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

int Ov012_BlendOpeningBgPlanes(void *pContext, u32 nThreadDelta,
                             Ov012OpeningEvent *pEvent) {
    char *context;
    int nBlendStep;

    context = (char *)pContext;
    nBlendStep = Math_DivMod(
        (nThreadDelta - pEvent->nTriggerThreadDelta) << 4,
        pEvent->uArgument1);

    if (*(u8 *)(context + 0x8bf0) == 0) {
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) |
                      (pEvent->uArgument0 << 8);
        if (nBlendStep >= 16) {
            *(u8 *)(context + 0x8bf0) = pEvent->uArgument0;
            *(int *)(context + 0x8bf4) = 0;
            return 1;
        }
        *(int *)(context + 0x8bf4) = nBlendStep - 16;
        return 0;
    }

    REG_DISPCNT = (REG_DISPCNT & ~0x1f00) |
                  ((*(u8 *)(context + 0x8bf0) | pEvent->uArgument0) << 8);
    if (nBlendStep >= 16) {
        *(u8 *)(context + 0x8bf0) |= pEvent->uArgument0;
        REG_BLDCNT = 0;
        return 1;
    }

    G2x_SetBlendAlpha_(&REG_BLDCNT, pEvent->uArgument0,
                       *(u8 *)(context + 0x8bf0),
                       nBlendStep, 16 - nBlendStep);
    return 0;
}

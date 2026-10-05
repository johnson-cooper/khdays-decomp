/*
 * PS2 replacement for the one platform-specific part of the calendar.
 *
 * The original waits for the DAY/logo animation, then loads ov028 and runs
 * Nintendo DS cartridge/protection probes.  Those probes have no PS2
 * equivalent.  Preserve both original readiness gates and continue into the
 * original phase-3 fade; only the DS hardware probes themselves are omitted.
 */

#include "nitro/types.h"

typedef struct Ov004Ps2Context {
    unsigned char opaque0000[0xaf8];
    int transitionPhase;                 /* +0x0af8 */
    int opaque0afc;
    u64 lastTick;                        /* +0x0b00 */
    unsigned char opaque0b08[0x4a7c];
    int transitionState;                 /* +0x5584 */
} Ov004Ps2Context;

extern Ov004Ps2Context *data_ov004_02051384;
extern u64 OS_GetTick(void);
extern void kh_debug_mark(const char *stage, int a, int b);

void Ov004_RunDelayedProtectionChecks(void)
{
    Ov004Ps2Context *context = data_ov004_02051384;
    u64 elapsed;

    if (context == 0)
        return;

    elapsed = OS_GetTick() - context->lastTick;
    if (elapsed <= 0x11942b)
        return;

    /* Ov004_StepLogoSlide writes 2 when the original visual motion is done. */
    if (context->transitionState != 2)
        return;

    context->lastTick = OS_GetTick();
    context->transitionPhase = 3;
    kh_debug_mark("calendar: DS checks omitted", 2, 3);
}

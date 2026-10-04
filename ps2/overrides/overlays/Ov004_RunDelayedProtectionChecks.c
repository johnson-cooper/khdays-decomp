/*
 * PS2 override for the calendar's delayed DS Protect checks.
 *
 * ov004 normally loads ov028 here and runs DS Protect 1.10 cartridge/emulator
 * probes.  Those probes directly drive Nintendo DS cartridge/MMIO hardware.
 * There is no equivalent operation on PS2; attempting the original path can
 * strand the calendar before it ever requests SCENE_FIELD.
 *
 * Keep the original visual/timing gates.  Once they are satisfied, mark the
 * calendar transition complete directly on PS2 instead of entering the DS
 * fade-out/protection tail.  Hardware testing showed that the DS-only checks
 * were bypassed correctly but the calendar then remained alive in phase 3 and
 * never raised its completion flag, leaving cur=5/pend=0 forever.
 */

#include "nitro/types.h"

typedef struct Ov004Ps2Context {
    unsigned char opaque0000[0xaf8];
    int transitionPhase;                 /* +0x0af8 */
    int opaque0afc;
    u64 lastTick;                        /* +0x0b00 */
    unsigned char opaque0b08[0x4a48];
    int transitionComplete;              /* +0x5550 */
    unsigned char opaque5554[0x30];
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

    /* Ov004_StepLogoSlide writes 2 here when the visual transition is ready. */
    if (context->transitionState != 2)
        return;

    /*
     * On DS, phase 2 runs anti-piracy checks and then enters a timed fade-out
     * (phase 3); phase 4 only sets transitionComplete.  The PS2 path has no DS
     * cartridge hardware, and hardware testing shows the phase-3 tail does not
     * complete reliably.  The next-scene logic only depends on the completion
     * flag, so perform the same terminal state change directly.
     */
    context->lastTick = OS_GetTick();
    context->transitionPhase = 4;
    context->transitionComplete = 1;
    kh_debug_mark("calendar: complete PS2", context->transitionState, 4);
}

/* PS2: mechanically prepared copy of src/overlays/players/ov033_player_saix/Ov033_HandleMsgAndReaimArc.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/*
 * Message handler: 0x21 arms the intro state, 0x22 re-aims at the tracked target, 0x23 hands
 * over to the outro.
 *
 * THE 64-BIT OR.  The ROM's `orr rN, rN, #0` is not a no-op and not a macro artifact: it is
 * the HIGH HALF of a 64-bit OR on a pair of adjacent flag words.  `*(kh_unaligned_s64 *)(p) |= mask`
 * emits exactly two loads, `orr` low with the mask, `orr` high with zero, two stores.
 * Everything else tried on this -- plain `|= 0`, volatile `|= 0`, explicit `*p = *p | 0`,
 * volatile pointer locals -- recovers the load and the store but never the orr, because there
 * is no `| 0` in the source to preserve.  It also explains the second oddity: the two-step
 * base (`add r0, r5, #0x64` then `[r0, #0x404]`) is just how mwcc addresses the high half,
 * not a separate source construct.
 *
 * One more lever: the aim angle is computed into a local BEFORE the node's flag is tested.
 * Computing it inside the guarded block reorders the whole tail.
 */

#include "game/engine.h"

extern void Ov022_FillEightHalvesMinus1At0x2bd4(char *self);
extern int Ov022_IsSlotReady(char *p);
extern void Ov033_IdleStep(void);
extern void Ov033_AttackStep(void);
extern void Ov033_LandingStep(void);

void *Ov033_HandleMsgAndReaimArc(char *self, int msg) {
    char *blk = self + 0x2c + 0x2c00;
    void *next = 0;
    int *node;
    int base;

    switch (msg) {
    case 0x21:
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x464) |= 0x10000;
        }
        next = (void *)&Ov033_IdleStep;
        if (Ov022_IsSlotReady(self + 0x2f8 + 0x2000) != 0) {
            (*(void (**)(char *, int))(self + 0x664))(self, 0x33);
        } else {
            (*(void (**)(char *, int))(self + 0x664))(self, 0x2f);
        }
        break;
    case 0x22:
        Ov022_FillEightHalvesMinus1At0x2bd4(self);
        next = (void *)&Ov033_AttackStep;
        if (Ov022_IsSlotReady(self + 0x2f8 + 0x2000) != 0) {
            *(int *)(blk + 8) = 1;
            (*(void (**)(char *, int))(self + 0x664))(self, 0x31);
        } else {
            *(int *)(blk + 8) = 1;
            (*(void (**)(char *, int))(self + 0x664))(self, 0x30);
        }
        base = *(int *)(self + 0x2000 + 0xabc);
        if (base != -1) {
            unsigned short ang = (unsigned short)(base + *(short *)(self + 0x400 + 0x78));
            node = *(int **)(self + 0x20);
            if ((node[0] & 0x20) == 0) {
                *(unsigned short *)((char *)node + 0x80) = ang + 0x8000;
                *(unsigned short *)((char *)node + 4) |= 0x20;
            }
        }
        break;
    case 0x23:
        next = (void *)&Ov033_LandingStep;
        (*(void (**)(char *, int))(self + 0x664))(self, 0x32);
        break;
    }
    return next;
}

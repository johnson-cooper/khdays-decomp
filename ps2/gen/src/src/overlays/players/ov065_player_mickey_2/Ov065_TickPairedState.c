/* PS2: mechanically prepared copy of src/overlays/players/ov065_player_mickey_2/Ov065_TickPairedState.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/*
 * Per-frame tick of the paired state: resolve the node handle, run the partner countdown in
 * the shared block (+0x11c / +0x120, expiring at 0x6000), refresh the sub-object at +0x2644
 * three times, raise the 64-bit flag pairs at +0x464 and +0x46c when it reports idle, and
 * publish the pose.
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
 * Two more levers: func_ov022_02083f0c's RETURN VALUE is Ov002_StoreVAndToggleBit25's first argument
 * (the ROM sets only r1 and r2 before that call), and the locals are declared base, handle,
 * blk -- any other order permutes r5 and r6.
 */
extern int Anim_GetFrame(int *node, int a);
extern int Session_GetLocalPlayerIndex(void);
extern int Ov022_GetGlobal34(void);
extern int func_ov022_02083f0c(void);
extern void Ov002_StoreVAndToggleBit25(int a, int b, int c);
extern int Ov022_FollowGroundRumble(char *self);
extern void Ov022_ForwardToNodeHandler(int a, int b);
extern void Ov022_InvokeCallback24IfBit0(int a);
extern int Ov022_AreStreamsIdle(int a);
extern void Ov002_WidgetScrollCommit(char *dst, char *src, int v, int w);
extern void func_ov022_020ad588(char *blk);
extern char *data_ov065_020b7340;

void Ov065_TickPairedState(char *self) {
    char *base = data_ov065_020b7340;
    int r5;
    char *blk;

    r5 = Anim_GetFrame((int *)(*(int *)(self + 0x20) + 4), 0);
    blk = base + 0x2c80;

    if (*(int *)(self + 0x6bc) != 0x31) {
        if (*(int *)(blk + 0x11c) == 1) {
            *(int *)(blk + 0x120) = *(int *)(blk + 0x120) + Ov022_GetGlobal34();
            if (*(int *)(blk + 0x120) >= 0x6000) {
                Ov002_StoreVAndToggleBit25(func_ov022_02083f0c(), 0, 0);
                *(int *)(blk + 0x11c) = 0;
            }
        }
    } else if (*(unsigned char *)(self + 8) == Session_GetLocalPlayerIndex()) {
        *(int *)(blk + 0x11c) = Ov022_FollowGroundRumble(self);
    }

    Ov022_ForwardToNodeHandler(*(int *)(self + 0x2644), Ov022_GetGlobal34());
    Ov022_InvokeCallback24IfBit0(*(int *)(self + 0x2644));
    if (Ov022_AreStreamsIdle(*(int *)(self + 0x2644)) == 0) {
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x464) |= 0x10000;
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_s64 *)(self + 0x46c) |= 0x10000;
        }
    }

    Ov002_WidgetScrollCommit(self + 0x30c + 0xc00, base + 0x2c + 0x2c00,
                        *(short *)(self + 0x2a00 + 0xba), r5);
    func_ov022_020ad588(base);
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_TickTagTrackerNodes.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov002_TickTagTrackerNodes -- advance every armed node of a tag-tracker table by elapsed real time.
 *
 * Each node (0x30 bytes, built by BuildTagTrackerNode) cycles through a list of
 * tags: +0x2c is the tag array, +4 the selected slot, +2 the slot count, +0xc the
 * dwell time per slot and +0x14 an accumulator -- both 64-bit tick counts. Once the
 * accumulator passes one slot's dwell the node steps to the next tag, wrapping with
 * a modulo (or clamping to the last slot when +8 selects play-once), hands the newly
 * selected tag to the table's apply callback (ctx+0x3c), and takes the consumed time
 * back out of the accumulator with a 64-bit modulo -- so a long stall skips whole
 * slots instead of drifting. The +0x24 armed bit is the one the arm/disarm helpers
 * set (Ov008_RewindTagTrackerNode, Ov025_SetTagTrackerNodeArmed).
 *
 * ctx+0x44/+0x48 are an optional save/restore pair wrapped around the apply call
 * (+0x48 reads the current selection, +0x44 sets it), used to point the apply at the
 * node's own target (+0x28) and put the old selection back afterwards.
 *
 * TWO ROM BUGS ARE REPRODUCED HERE, both in that save/restore pair:
 *   - `prev` is never assigned in the loop, so the "target already selected, skip the
 *     swap" test never fires -- it compares against the 0x7fffffff sentinel forever.
 *     The `prev = <target>` line was left out.
 *   - `swap` is only assigned when ctx+0x48 is non-NULL but is read unconditionally.
 *     Without a +0x48 getter it is r9, which this function never sets on that path:
 *     the caller's r9 for the first stepped node, then whatever the previous node
 *     left. A nonzero value makes the ROM call the +0x44 setter with the node's
 *     target and then with `saved`, still -1; in a table without a setter both calls
 *     go to address 0, the ITCM mirror, where OSi_VBlankInterruptHandler sits
 *     (0x01ff8000). The r9 that arrives is the one the caller of Ov002_TickSelectionWidget
 *     (this function's only caller, which leaves r9 alone too) had:
 *       - Ov002_ResetMenuRoot leaves r9 alone, so it comes from further up;
 *     where the chain leaves r9 alone up to the object update, it is what
 *     Obj_UpdateAll left there: the result of the last update it called
 *     (`blx r0; mov r9, r0`, 0x02023b64), 0 unless that object changed handler.
 * Both are in the ROM's own instruction stream; do not "fix" them.
 *
 * The +4 slot index is volatile: the ROM reloads it at every single use while happily
 * caching +2 right next to it, which is what a volatile field looks like. */
extern unsigned long long OS_GetTick(void);
extern long long kh_rt_u32_divmod(int a, int b);
extern unsigned long long func_02020374(unsigned long long a, unsigned long long b);

struct NodeFlags { unsigned char armed : 1, visible : 1; };

void Ov002_TickTagTrackerNodes(int param_1) {
    int saved = -1;
    int prev = 0x7fffffff;
    int swap;
    int i;
    int e;
    unsigned long long now = OS_GetTick();
    int (*get)(void);

    for (i = 0; i < *(int *)(param_1 + 0x34); i++) {
        e = *(int *)(param_1 + 0x10) + i * 0x30;

        if (((struct NodeFlags *)(e + 0x24))->visible == 0) continue;
        if (((struct NodeFlags *)(e + 0x24))->armed == 0) continue;
        if (*(volatile unsigned short *)(e + 4) >= *(unsigned short *)(e + 2) - 1 &&
            *(int *)(e + 8) == 1) continue;

        *(kh_unaligned_u64 *)(e + 0x14) += now - *(kh_unaligned_u64 *)(e + 0x1c);
        *(kh_unaligned_u64 *)(e + 0x1c) = now;
        if (*(kh_unaligned_u64 *)(e + 0x14) <= *(kh_unaligned_u64 *)(e + 0xc)) continue;

        *(volatile unsigned short *)(e + 4) = *(volatile unsigned short *)(e + 4) + 1;
        if (*(volatile unsigned short *)(e + 4) >= *(unsigned short *)(e + 2)) {
            if (*(int *)(e + 8) == 1) {
                *(volatile unsigned short *)(e + 4) = *(unsigned short *)(e + 2) - 1;
            } else {
                *(volatile unsigned short *)(e + 4) = (unsigned short)(
                    kh_rt_u32_divmod(*(volatile unsigned short *)(e + 4),
                                  *(unsigned short *)(e + 2)) >> 32);
            }
        }

        get = *(int (**)(void))(param_1 + 0x48);
        if (get != 0) {
            saved = get();
            swap = (*(int *)(param_1 + 0x44) != 0 && *(int *)(e + 0x28) != prev);
        }
        if (swap != 0) {
            (*(void (**)(int))(param_1 + 0x44))(*(int *)(e + 0x28));
        }
        (*(void (**)(int))(param_1 + 0x3c))(
            *(int *)(*(int *)(e + 0x2c) + *(volatile unsigned short *)(e + 4) * 4));
        if (swap != 0) {
            (*(void (**)(int))(param_1 + 0x44))(saved);
        }
        *(kh_unaligned_u64 *)(e + 0x14) =
            func_02020374(*(kh_unaligned_u64 *)(e + 0x14), *(kh_unaligned_u64 *)(e + 0xc));
    }
}

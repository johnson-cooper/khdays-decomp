/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_StepCursorWrapping.c (ps2/tools/prep_sources.py). Do not edit. */
/* Advance the wrap-around cursor: step +0x7dc on by one modulo the count at
 * +0x7e0, hand the new index to Ov002_MoveCursorTo and play the move sound.
 * Does nothing while the count is zero or negative.
 *
 * kh_rt_s32_divmod is the MetroWerks signed-divide helper: quotient in r0, remainder
 * in r1. It has to be called by address rather than written as `%` -- mwcc emits
 * the call under the name _s32_div_f, which this project does not define, so the
 * arithmetic spelling fails on the reloc even though the instructions are right.
 * The long long return picks the remainder out of r1. */

#include "game/engine.h"

extern long long kh_rt_s32_divmod(int a, int b);
extern void Ov002_MoveCursorTo(int index);

extern char *data_ov002_0207f624;

void Ov002_StepCursorWrapping(void) {
    char *ctx = data_ov002_0207f624;
    int count = *(int *)(ctx + 0x7e0);

    if (count <= 0) {
        return;
    }

    Ov002_MoveCursorTo((int)(kh_rt_s32_divmod(*(int *)(ctx + 0x7dc) + 1, count) >> 0x20));
    PlaySound(0, 0);
}

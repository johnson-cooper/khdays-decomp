/* PS2 replacement for Fx_Tween (src/engine/Fx_Tween.c): the interpolation curves, identical
 * arithmetic, with the hardware divider (DIVCNT/DIV_NUMER/DIV_DENOM/DIV_RESULT at 0x04000280)
 * replaced by kh_cp_div_q, which reproduces the divider's results exactly. */

#include "nitro/types.h"

extern int FX_Div(int x, int y);
extern s64 kh_cp_div_q(int mode, s64 numer, s64 denom);

static inline s32 FX_Mul(s32 a, s32 b)
{
    return (s32)(((s64)a * b + 0x800) >> 12);
}

s32 Fx_Tween(s32 start, s32 end, u32 elapsed, u32 duration, u32 curve)
{
    s32 diff;
    u64 weight;
    s32 offsetFromMid;
    s32 halfDiff;
    s32 offsetSq;
    s32 eased;

    if (start == end) {
        return end;
    }
    if (elapsed >= duration) {
        return end;
    }

    diff = end - start;

    switch (curve) {
    case 0:
        /* 64/32 mode, the low word of the quotient */
        return start + (s32)kh_cp_div_q(1, (s64)diff * elapsed, (s64)(u64)duration);

    case 1:
        weight = elapsed * elapsed;
        return start + (s32)((weight * (u64)kh_cp_div_q(2, (s64)diff << 32, (s64)((u64)duration * duration))) >> 32);

    case 2:
        weight = (u64)(duration - elapsed) * (duration - elapsed);
        return end - (s32)((weight * (u64)kh_cp_div_q(2, (s64)diff << 32, (s64)((u64)duration * duration))) >> 32);

    case 3:
        /* +0x1000 at the start, 0 at the midpoint, -0x1000 at the end. */
        offsetFromMid = 0x1000 - (FX_Div(elapsed, duration) << 1);

        if (elapsed < (duration >> 1)) {
            offsetSq = FX_Mul(offsetFromMid, offsetFromMid);
            halfDiff = diff / 2;
            eased = FX_Mul(halfDiff, 0x1000 - offsetSq);
            return eased + start;
        } else {
            offsetSq = FX_Mul(offsetFromMid, offsetFromMid);
            halfDiff = diff / 2;
            eased = FX_Mul(halfDiff, offsetSq);
            return start + halfDiff + eased;
        }
    }
    return end;   /* unreachable for the curves the game uses (the DS returns garbage) */
}

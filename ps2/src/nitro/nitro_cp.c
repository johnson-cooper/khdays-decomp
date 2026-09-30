/* The DS ARM9 math coprocessor (divider + square root unit) as exact C, and the NitroSDK FX/VEC
 * functions that drive it.
 *
 * The DS computes these in hardware through registers 0x04000280-0x040002bf; game and SDK code
 * programs the registers, spins on the busy bit and reads the result.  Every such function has a
 * PS2 version that calls kh_cp_div / kh_cp_sqrt instead, which reproduce the unit's results bit
 * for bit (including division by zero and the overflow cases), because collision, camera and
 * animation code depends on them.
 *
 * Divider (DIVCNT bits 0-1):  0 = s32/s32, 1 = s64/s32, 2|3 = s64/s64.
 *   denominator 0 : quotient = numerator < 0 ? +1 : -1, remainder = numerator
 *                   (32/32 mode: the upper word of the 64-bit quotient is inverted as well)
 *   INT_MIN / -1  : quotient = INT_MIN (as an unsigned magnitude in 32/32 mode), remainder 0
 * Square root (SQRTCNT bit 0): 0 = 32-bit input, 1 = 64-bit input; floor(sqrt(x)).
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

typedef int32_t fx32;
typedef int64_t fx64;
typedef struct { fx32 x, y, z; } VecFx32;
typedef struct { int16_t x, y, z; } VecFx16;

void kh_cp_div(int mode, s64 numer, s64 denom, s64 *quot, s64 *rem)
{
    s64 q, r;
    switch (mode & 3) {
    case 0: {
        s32 n = (s32)numer, d = (s32)denom;
        if (d == 0) {
            u32 lo = n < 0 ? 1u : 0xffffffffu;
            u32 hi = n < 0 ? 0xffffffffu : 1u;   /* upper word inverted */
            q = (s64)(((u64)hi << 32) | lo);
            r = n;
        } else if (n == INT32_MIN && d == -1) {
            q = (s64)(u64)0x80000000u;
            r = 0;
        } else {
            q = n / d;
            r = n % d;
        }
        break;
    }
    case 1: {
        s32 d = (s32)denom;
        if (d == 0) {
            q = numer < 0 ? 1 : -1;
            r = numer;
        } else if (numer == INT64_MIN && d == -1) {
            q = INT64_MIN;
            r = 0;
        } else {
            q = numer / d;
            r = numer % d;
        }
        break;
    }
    default:
        if (denom == 0) {
            q = numer < 0 ? 1 : -1;
            r = numer;
        } else if (numer == INT64_MIN && denom == -1) {
            q = INT64_MIN;
            r = 0;
        } else {
            q = numer / denom;
            r = numer % denom;
        }
        break;
    }
    if (quot)
        *quot = q;
    if (rem)
        *rem = r;
}

s64 kh_cp_div_q(int mode, s64 numer, s64 denom)
{
    s64 q;
    kh_cp_div(mode, numer, denom, &q, NULL);
    return q;
}

u32 kh_cp_sqrt(int mode, u64 param)
{
    u64 x = (mode & 1) ? param : (u64)(u32)param;
    u64 res = 0, bit = (u64)1 << 62;
    while (bit > x)
        bit >>= 2;
    while (bit) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return (u32)res;
}

/* ------------------------------------------------ FX (libs/nitro/fx equivalents) */

/* The divider/sqrt register block as state (kh_ds_io + 0x280, DS layout).  Game code that was
 * written against the registers stores its operands here (prep_sources.py R5) and fetches the
 * result through the SDK getters below, which compute it from the stored operands, so both
 * styles of use see the same, exact results. */
#define CP_DIVCNT     (*(volatile u16 *)(kh_ds_io + 0x280))
#define CP_DIV_NUMER  (*(volatile s64 *)(kh_ds_io + 0x290))
#define CP_DIV_DENOM  (*(volatile s64 *)(kh_ds_io + 0x298))
#define CP_DIV_RESULT (*(volatile s64 *)(kh_ds_io + 0x2a0))
#define CP_DIVREM     (*(volatile s64 *)(kh_ds_io + 0x2a8))
#define CP_SQRTCNT    (*(volatile u16 *)(kh_ds_io + 0x2b0))
#define CP_SQRT_RES   (*(volatile u32 *)(kh_ds_io + 0x2b4))
#define CP_SQRT_PARAM (*(volatile u64 *)(kh_ds_io + 0x2b8))

static s64 div_now(void)
{
    s64 q, r;
    kh_cp_div(CP_DIVCNT & 3, CP_DIV_NUMER, CP_DIV_DENOM, &q, &r);
    CP_DIV_RESULT = q;
    CP_DIVREM = r;
    CP_DIVCNT = (u16)((CP_DIVCNT & ~0xc000) | (CP_DIV_DENOM == 0 ? 0x4000 : 0));
    return q;
}

static u32 sqrt_now(void)
{
    u32 v = kh_cp_sqrt(CP_SQRTCNT & 1, CP_SQRT_PARAM);
    CP_SQRT_RES = v;
    CP_SQRTCNT = (u16)(CP_SQRTCNT & ~0x8000);
    return v;
}

/* REG_DIVCNT / REG_SQRTCNT from the shadow nitro/hw.h: compute, then hand out the register */
volatile void *kh_cp_divcnt_ptr(void) { div_now(); return &CP_DIVCNT; }
volatile void *kh_cp_sqrtcnt_ptr(void) { sqrt_now(); return &CP_SQRTCNT; }

void FX_DivAsync(fx32 numer, fx32 denom)
{
    CP_DIVCNT = 1;
    CP_DIV_NUMER = (s64)((u64)(u32)numer << 32);
    CP_DIV_DENOM = (s64)(u64)(u32)denom;
}

void FX_InvAsync(fx32 x)
{
    CP_DIVCNT = 1;
    CP_DIV_NUMER = (s64)0x1000 << 32;
    CP_DIV_DENOM = (s64)(u64)(u32)x;
}

fx32 FX_GetDivResult(void) { return (fx32)((div_now() + 0x80000) >> 20); }
fx64 FX_GetDivResultFx64c(void) { return div_now(); }
fx32 FX_GetInvResult(void) { return FX_GetDivResult(); }
fx64 FX_DivFx64c(fx32 numer, fx32 denom) { FX_DivAsync(numer, denom); return div_now(); }
fx64 FX_InvFx64c(fx32 x) { FX_InvAsync(x); return div_now(); }

void FX_SqrtAsync(fx32 x)
{
    CP_SQRTCNT = 1;
    CP_SQRT_PARAM = (u64)(u32)x << 32;
}

fx32 FX_GetSqrtResult(void) { return (fx32)((sqrt_now() + 0x200) >> 10); }

fx32 FX_Sqrt(fx32 x)
{
    if (x <= 0)
        return 0;
    FX_SqrtAsync(x);
    return FX_GetSqrtResult();
}

/* CP context save/restore (VBlank handlers wrap their divider use in these) */
typedef struct { u64 numer, denom, sqrt; u16 divcnt, sqrtcnt; } CPContext;

void CP_SaveContext(CPContext *c)
{
    c->numer = (u64)CP_DIV_NUMER;
    c->denom = (u64)CP_DIV_DENOM;
    c->sqrt = CP_SQRT_PARAM;
    c->divcnt = CP_DIVCNT;
    c->sqrtcnt = CP_SQRTCNT;
}

void CP_RestoreContext(const CPContext *c)
{
    CP_DIVCNT = (u16)(c->divcnt & 3);
    CP_DIV_NUMER = (s64)c->numer;
    CP_DIV_DENOM = (s64)c->denom;
    CP_SQRTCNT = (u16)(c->sqrtcnt & 1);
    CP_SQRT_PARAM = c->sqrt;
}

fx32 VEC_Mag(const VecFx32 *v)
{
    s64 sq = (s64)v->x * v->x + (s64)v->y * v->y + (s64)v->z * v->z;
    return (fx32)((kh_cp_sqrt(1, (u64)(sq * 4)) + 1) >> 1);
}

fx32 VEC_Distance(const VecFx32 *a, const VecFx32 *b)
{
    fx32 x = a->x - b->x, y = a->y - b->y, z = a->z - b->z;
    s64 sq = (s64)x * x + (s64)y * y + (s64)z * z;
    return (fx32)((kh_cp_sqrt(1, (u64)(sq * 4)) + 1) >> 1);
}

/* inverse-length scale the SDK computes: (2^56 / |v|^2) * sqrt(4 |v|^2) */
static s64 inv_len_scale(s64 sq, fx32 *mag)
{
    fx32 m = (fx32)kh_cp_sqrt(1, (u64)(sq * 4));
    s64 q = kh_cp_div_q(2, (s64)0x0100000000000000ll, sq);
    *mag = m;
    return q * m;
}

fx32 VEC_Normalize(const VecFx32 *src, VecFx32 *dst)
{
    s64 sq = (s64)src->x * src->x + (s64)src->y * src->y + (s64)src->z * src->z;
    fx32 m;
    s64 s = inv_len_scale(sq, &m);
    VecFx32 in = *src;
    dst->x = (fx32)((s * in.x + (1ll << 44)) >> 45);
    dst->y = (fx32)((s * in.y + (1ll << 44)) >> 45);
    dst->z = (fx32)((s * in.z + (1ll << 44)) >> 45);
    return (m + 1) >> 1;
}

/* normalises two vectors in place (the SDK interleaves the two computations) */
void func_01ffa7fc(VecFx32 *a, VecFx32 *b)
{
    s64 sa = (s64)a->x * a->x + (s64)a->y * a->y + (s64)a->z * a->z;
    s64 sb = (s64)b->x * b->x + (s64)b->y * b->y + (s64)b->z * b->z;
    fx32 m;
    s64 s = inv_len_scale(sa, &m);
    a->x = (fx32)((s * a->x + (1ll << 44)) >> 45);
    a->y = (fx32)((s * a->y + (1ll << 44)) >> 45);
    a->z = (fx32)((s * a->z + (1ll << 44)) >> 45);
    s = inv_len_scale(sb, &m);
    b->x = (fx32)((s * b->x + (1ll << 44)) >> 45);
    b->y = (fx32)((s * b->y + (1ll << 44)) >> 45);
    b->z = (fx32)((s * b->z + (1ll << 44)) >> 45);
}

/* VEC_Normalize to a VecFx16 */
void func_01ffcfd0(const VecFx32 *in, VecFx16 *out)
{
    s64 sq = (s64)in->x * in->x + (s64)in->y * in->y + (s64)in->z * in->z;
    fx32 m;
    s64 s = inv_len_scale(sq, &m);
    out->x = (int16_t)((s * in->x + 0x100000000000ll) >> 45);
    out->y = (int16_t)((s * in->y + 0x100000000000ll) >> 45);
    out->z = (int16_t)((s * in->z + 0x100000000000ll) >> 45);
}

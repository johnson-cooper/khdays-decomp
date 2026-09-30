/* CodeWarrior ARM runtime helpers that game code calls by name, with ARM semantics, in C.
 *
 * On the ARM9 these are hand-written assembly in the MSL runtime (libs/msl/runtime/asm_stubs).
 * Game sources call them explicitly and read r0 (and sometimes r1) back.  Under the EE n32 ABI
 * a caller that reads `long long` and one that reads `int` cannot share one symbol, so callers
 * are pointed at the variant matching their declaration by ps2/tools/prep_sources.py (rule R3,
 * table ps2/config/abi_variants.txt).
 *
 * Division by zero follows the ARM routines exactly: the quotient is the dividend and the
 * remainder is 0 (signed and unsigned); INT_MIN / -1 = INT_MIN.
 */
#include <stdint.h>

typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

static inline void s32_divmod(s32 a, s32 b, s32 *q, s32 *r)
{
    if (b == 0) {
        *q = a;
        *r = 0;
    } else if (a == INT32_MIN && b == -1) {
        *q = INT32_MIN;
        *r = 0;
    } else {
        *q = a / b;
        *r = a % b;
    }
}

static inline void u32_divmod(u32 a, u32 b, u32 *q, u32 *r)
{
    if (b == 0) {
        *q = a;
        *r = 0;
    } else {
        *q = a / b;
        *r = a % b;
    }
}

/* _s32_div_f read as int: the quotient */
s32 func_02020400(s32 a, s32 b)
{
    s32 q, r;
    s32_divmod(a, b, &q, &r);
    return q;
}

/* _s32_div_f read as long long: quotient | remainder << 32 */
s64 kh_rt_s32_divmod(s32 a, s32 b)
{
    s32 q, r;
    s32_divmod(a, b, &q, &r);
    return (s64)(((u64)(u32)r << 32) | (u32)q);
}

/* _u32_div_f read as u32: the quotient */
u32 Math_DivMod(u32 a, u32 b)
{
    u32 q, r;
    u32_divmod(a, b, &q, &r);
    return q;
}

/* _u32_div_f read as u64: quotient | remainder << 32 */
u64 kh_rt_u32_divmod(u32 a, u32 b)
{
    u32 q, r;
    u32_divmod(a, b, &q, &r);
    return ((u64)r << 32) | q;
}

/* 64-bit helpers: ARM returns the dividend unchanged for a zero divisor. */
static inline u64 ull_div(u64 a, u64 b) { return b ? a / b : a; }
static inline u64 ull_mod(u64 a, u64 b) { return b ? a % b : a; }

/* _ll_udiv(u64, u64) */
u64 func_02020368(u64 a, u64 b) { return ull_div(a, b); }
/* _ll_udiv with the divisor passed as two words */
u64 kh_rt_ll_udiv_w(u64 a, u32 blo, u32 bhi) { return ull_div(a, ((u64)bhi << 32) | blo); }
/* ... and the result read as int (low word) */
s32 kh_rt_ll_udiv_w_32(u64 a, u32 blo, u32 bhi) { return (s32)(u32)ull_div(a, ((u64)bhi << 32) | blo); }
/* ... with both operands passed as words */
u64 kh_rt_ll_udiv_ww(u32 alo, u32 ahi, u32 blo, u32 bhi)
{
    return ull_div(((u64)ahi << 32) | alo, ((u64)bhi << 32) | blo);
}

/* _ull_mod */
u64 func_02020374(u64 a, u64 b) { return ull_mod(a, b); }

/* _ll_sdiv: zero divisor returns the dividend; the magnitude division is unsigned, so
 * INT64_MIN / -1 wraps to INT64_MIN as on the ARM. */
s64 func_020201b8(s64 a, s64 b)
{
    u64 ua, ub, q;
    int neg;
    if (b == 0)
        return a;
    neg = (a < 0) != (b < 0);
    ua = a < 0 ? (u64)0 - (u64)a : (u64)a;
    ub = b < 0 ? (u64)0 - (u64)b : (u64)b;
    q = ua / ub;
    return (s64)(neg ? (u64)0 - q : q);
}

/* _ll_shl: shift count taken mod 64 */
u64 func_020203d0(u64 v, int n)
{
    n &= 63;
    return n ? v << n : v;
}

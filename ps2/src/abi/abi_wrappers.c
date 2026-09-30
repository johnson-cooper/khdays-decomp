/* Split-argument wrappers for game functions (rule R3 of ps2/tools/prep_sources.py).
 *
 * These functions take one 64-bit parameter.  Some of their callers were decompiled with the
 * value spelled as two 32-bit words, which on the ARM9 is the same register pair (r1:r2 or
 * r2:r3).  Under the EE n32 ABI a 64-bit argument is one register, so those callers are pointed
 * at the `__w` / `__ll` wrappers below, which rebuild the call the definition expects.
 */
#include <stdint.h>

typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t s64;

#define JOIN(lo, hi) (((u64)(u32)(hi) << 32) | (u32)(lo))

#define SWEEP(n)                                                                  \
    extern void n(void *state, s64 t, const void *at);                           \
    void n##__w(void *state, u32 lo, u32 hi, const void *at) { n(state, (s64)JOIN(lo, hi), at); }
SWEEP(Ov156_GroundSweep)
SWEEP(Ov157_GroundSweep)
SWEEP(Ov191_BoxSweepPush)
SWEEP(Ov192_BoxSweepPush)
SWEEP(Ov193_BoxSweepPush)

#define FOLD(n)                                                                    \
    extern void n(void *self, void *grid, u64 covered, u64 *out);                  \
    void n##__w(void *self, void *grid, u32 lo, u32 hi, u64 *out) { n(self, grid, JOIN(lo, hi), out); }
FOLD(Ov000_FoldPanelIntoTally)
FOLD(Ov004_FoldPanelIntoTally)
FOLD(Ov005_FoldPanelIntoTally)
FOLD(Ov008_FoldPanelIntoTally)
FOLD(Ov009_FoldPanelIntoTally)
FOLD(Ov025_FoldPanelIntoTally)
FOLD(Ov069_FoldPanelIntoTally)

/* Defined as (int lo, int hi, int select, sampler); one caller passes the value as a u64. */
extern unsigned char Ov002_AddPanelCounter(int lo, int hi, int select, long long (*sample)(void));
int Ov002_AddPanelCounter__ll(u64 v, int select, int sample)
{
    return Ov002_AddPanelCounter((int)(u32)v, (int)(u32)(v >> 32), select, (long long (*)(void))(uintptr_t)(u32)sample);
}

/* Defined with a 32-bit duration; the rumble caller passes a 64-bit value (low word used). */
extern void Ov002_ArmShake(unsigned int duration);
void Ov002_ArmShake__ll(u64 duration) { Ov002_ArmShake((unsigned int)duration); }

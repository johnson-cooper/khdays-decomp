/* PS2: mechanically prepared copy of libs/nitro/fx/auto/func_01ffcfd0.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"
#include "nitro/fx_types.h"

typedef struct VecFx16 { s16 x; s16 y; s16 z; } VecFx16;

#define DIVCNT      (*(volatile u16 *)((unsigned int)kh_ds_io + 0x280))
#define DIV_RESULT  (*(volatile s64 *)((unsigned int)kh_ds_io + 0x2a0))
#define SQRTCNT     (*(volatile u16 *)((unsigned int)kh_ds_io + 0x2b0))
#define SQRT_RESULT (*(volatile u32 *)((unsigned int)kh_ds_io + 0x2b4))

static inline s16 NormalizeComponent(s32 value, s64 scale)
{
    return (s16)((scale * value + 0x100000000000LL) >> 45);
}

static inline void CP_SetDivImm64_64_NS_(u64 numerator, u64 denominator)
{
    *(u64 *)((unsigned int)kh_ds_io + 0x290) = numerator;
    *(u64 *)((unsigned int)kh_ds_io + 0x298) = denominator;
}

static inline void CP_SetDiv64_64(u64 numerator, u64 denominator)
{
    DIVCNT = 2;
    CP_SetDivImm64_64_NS_(numerator, denominator);
}

static inline void CP_SetSqrtImm64_NS_(u64 parameter)
{
    *(u64 *)((unsigned int)kh_ds_io + 0x2b8) = parameter;
}

static inline void CP_SetSqrt64(u64 parameter)
{
    SQRTCNT = 1;
    CP_SetSqrtImm64_NS_(parameter);
}

void func_01ffcfd0(const VecFx32 *input, VecFx16 *output)
{
    s64 squared;
    s64 scale;
    s32 sqrtResult;
    s32 y;
    s32 x;

    y = input->y;
    x = *(volatile const s32 *)&input->x;
    squared = (s64)x * x;
    squared += (s64)y * y;
    squared += (s64)input->z * input->z;

    CP_SetDiv64_64(0x0100000000000000LL, (u64)squared);
    CP_SetSqrt64((u64)(squared * 4));

    while (SQRTCNT & 0x8000) {
    }
    sqrtResult = (s32)SQRT_RESULT;
    while (DIVCNT & 0x8000) {
    }
    scale = DIV_RESULT * (s64)sqrtResult;

    output->x = NormalizeComponent(input->x, scale);
    output->y = NormalizeComponent(input->y, scale);
    output->z = NormalizeComponent(input->z, scale);
}

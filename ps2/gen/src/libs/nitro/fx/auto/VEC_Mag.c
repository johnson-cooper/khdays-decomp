/* PS2: mechanically prepared copy of libs/nitro/fx/auto/VEC_Mag.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"
#include "nitro/fx_types.h"

typedef struct SqrtRegisters {
    volatile unsigned short control;
    unsigned short padding[3];
    volatile s64 parameter;
} SqrtRegisters;

#define SQRT_REGISTERS ((SqrtRegisters *)((unsigned int)kh_ds_io + 0x2b0))

fx32 VEC_Mag(const VecFx32 *v)
{
    s64 squared;
    fx32 y = v->y;
    fx32 x = *(volatile const fx32 *)&v->x;

    squared = (s64)x * x;
    squared += (s64)y * y;
    squared += (s64)v->z * v->z;

    SQRT_REGISTERS->control = 1;
    SQRT_REGISTERS->parameter = squared * 4;
    while (SQRT_REGISTERS->control & 0x8000) {
    }
    return (*(volatile fx32 *)((unsigned int)kh_ds_io + 0x2b4) + 1) >> 1;
}

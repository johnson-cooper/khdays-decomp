/* PS2: mechanically prepared copy of libs/nitro/fx/auto/VEC_Distance.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"
#include "nitro/fx_types.h"

#define SQRT_CONTROL (*(volatile u16 *)((unsigned int)kh_ds_io + 0x2b0))
#define SQRT_RESULT (*(volatile fx32 *)((unsigned int)kh_ds_io + 0x2b4))
#define SQRT_PARAMETER (*(volatile u64 *)((unsigned int)kh_ds_io + 0x2b8))

fx32 VEC_Distance(const VecFx32 *a, const VecFx32 *b)
{
    fx32 x = a->x - b->x;
    fx32 y = a->y - b->y;
    fx32 z = a->z - b->z;
    fx64 squaredDistance = (fx64)x * x;

    squaredDistance += (fx64)y * y;
    squaredDistance += (fx64)z * z;

    SQRT_CONTROL = 1;
    SQRT_PARAMETER = (u64)(squaredDistance * 4);
    while (SQRT_CONTROL & 0x8000) {
    }

    return (SQRT_RESULT + 1) >> 1;
}

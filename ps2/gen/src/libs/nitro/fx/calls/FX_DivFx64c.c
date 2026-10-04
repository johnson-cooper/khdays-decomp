/* PS2: mechanically prepared copy of libs/nitro/fx/calls/FX_DivFx64c.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
extern void FX_DivAsync(int num, int denom);

/* Kick an async divide, spin on DIVCNT busy, return the 64-bit DIV_RESULT. */
long long FX_DivFx64c(int num, int denom)
{
    volatile unsigned short *reg_divcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x280);
    FX_DivAsync(num, denom);
    while (*reg_divcnt & 0x8000) {
    }
    return *(kh_unaligned_s64 *)((unsigned int)kh_ds_io + 0x2a0);
}

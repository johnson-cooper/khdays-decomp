/* PS2: mechanically prepared copy of libs/nitro/fx/auto/FX_GetDivResult.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Spins on DIVCNT, then rounds the 64-bit quotient down to fx32 (0x80000 = half of 1<<20). */
int FX_GetDivResult(void) {
    while (*(volatile unsigned short *)((unsigned int)kh_ds_io + 0x280) & 0x8000) {
    }
    return (int)((*(volatile kh_unaligned_s64 *)((unsigned int)kh_ds_io + 0x2a0) + 0x80000) >> 20);
}

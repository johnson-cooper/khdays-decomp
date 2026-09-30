/* PS2: mechanically prepared copy of libs/nitro/fx/auto/FX_GetSqrtResult.c (ps2/tools/prep_sources.py). Do not edit. */
/* Spins on the SQRTCNT busy bit, then rounds SQRT_RESULT to fx32 (0x200 = half a unit). */
unsigned FX_GetSqrtResult(void) {
    while (*(volatile unsigned short *)((unsigned int)kh_ds_io + 0x2b0) & 0x8000) {
    }
    return (*(volatile unsigned *)((unsigned int)kh_ds_io + 0x2b4) + 0x200) >> 10;
}

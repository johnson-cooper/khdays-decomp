/* PS2: mechanically prepared copy of libs/nitro/fx/auto/FX_GetDivResultFx64c.c (ps2/tools/prep_sources.py). Do not edit. */
/* Waits for the hardware divider to finish (DIVCNT busy bit), then reads the
 * 64-bit quotient from DIV_RESULT (0x040002a0). */
long long FX_GetDivResultFx64c(void) {
    volatile unsigned short *reg_divcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x280);
    while (*reg_divcnt & 0x8000)
        ;
    return *(long long *)((unsigned int)kh_ds_io + 0x2a0);
}

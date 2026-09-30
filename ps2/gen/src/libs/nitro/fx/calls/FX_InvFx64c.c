/* PS2: mechanically prepared copy of libs/nitro/fx/calls/FX_InvFx64c.c (ps2/tools/prep_sources.py). Do not edit. */
extern void FX_InvAsync(int x);

/* Kick an async reciprocal, spin on DIVCNT busy, return the 64-bit DIV_RESULT. */
long long FX_InvFx64c(int x) {
    volatile unsigned short *reg_divcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x280);
    FX_InvAsync(x);
    while (*reg_divcnt & 0x8000) ;
    return *(long long *)((unsigned int)kh_ds_io + 0x2a0);
}

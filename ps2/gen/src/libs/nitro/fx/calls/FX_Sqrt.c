/* PS2: mechanically prepared copy of libs/nitro/fx/calls/FX_Sqrt.c (ps2/tools/prep_sources.py). Do not edit. */
/* Hardware square root of a positive fixed-point value; non-positive input gives 0. */
extern int FX_GetSqrtResult(void);

int FX_Sqrt(int x) {
    volatile unsigned short *sqrtcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x2b0);
    if (x <= 0) {
        return 0;
    }
    *sqrtcnt = 1;
    *(volatile int *)(sqrtcnt + 4) = 0;
    *(volatile int *)(sqrtcnt + 6) = x;
    return FX_GetSqrtResult();
}

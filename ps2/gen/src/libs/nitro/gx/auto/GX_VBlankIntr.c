/* PS2: mechanically prepared copy of libs/nitro/gx/auto/GX_VBlankIntr.c (ps2/tools/prep_sources.py). Do not edit. */
/* Enables or disables the V-blank interrupt in DISPSTAT; returns the previous state. */
int GX_VBlankIntr(int enable) {
    volatile unsigned short *dispstat = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x4);
    int prev = *dispstat & 8;
    if (enable != 0) {
        *dispstat |= 8;
        return prev;
    }
    *dispstat &= ~8;
    return prev;
}

/* PS2: mechanically prepared copy of libs/nitro/gx/auto/G3X_SetClearColor.c (ps2/tools/prep_sources.py). Do not edit. */
void G3X_SetClearColor(unsigned color, unsigned alpha, unsigned depth,
                       unsigned polygonID, int fog) {
    unsigned v = color | (alpha << 16) | (polygonID << 24);
    if (fog != 0) {
        v |= 0x8000;
    }
    *(volatile unsigned *)((unsigned int)kh_ds_io + 0x350) = v;
    *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x354) = (unsigned short)depth;
}

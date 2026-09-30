/* PS2: mechanically prepared copy of libs/nitro/gx/auto/G2_GetBG2CharPtr.c (ps2/tools/prep_sources.py). Do not edit. */
/* From BG mode 5 up, BG2 can be a bitmap layer: then it has no character base at all. */
void *G2_GetBG2CharPtr(void) {
    int mode = *(volatile unsigned *)((unsigned int)kh_ds_io + 0x0) & 7;
    unsigned cnt = *(volatile unsigned short *)((unsigned int)kh_ds_io + 0xc);
    if (mode < 5 || (cnt & 0x80) == 0) {
        unsigned dispBase = (*(volatile unsigned *)((unsigned int)kh_ds_io + 0x0) & 0x07000000) >> 24;
        return (void *)(((unsigned int)kh_ds_vram_win(0) + 0x0) + (dispBase << 16) + (((cnt & 0x3c) >> 2) << 14));
    }
    return 0;
}

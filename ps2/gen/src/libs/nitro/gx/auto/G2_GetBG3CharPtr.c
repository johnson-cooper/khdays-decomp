/* PS2: mechanically prepared copy of libs/nitro/gx/auto/G2_GetBG3CharPtr.c (ps2/tools/prep_sources.py). Do not edit. */
/* BG3 only has a character base in the tiled modes: below 3 always, 3..5 unless it is running as
 * a bitmap layer, and never from mode 6 up. */
void *G2_GetBG3CharPtr(void) {
    int mode = *(volatile unsigned *)((unsigned int)kh_ds_io + 0x0) & 7;
    unsigned cnt = *(volatile unsigned short *)((unsigned int)kh_ds_io + 0xe);
    if (mode < 3 || (mode < 6 && (cnt & 0x80) == 0)) {
        unsigned dispBase = (*(volatile unsigned *)((unsigned int)kh_ds_io + 0x0) & 0x07000000) >> 24;
        return (void *)(((unsigned int)kh_ds_vram_win(0) + 0x0) + (dispBase << 16) + (((cnt & 0x3c) >> 2) << 14));
    }
    return 0;
}

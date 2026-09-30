/* PS2: mechanically prepared copy of libs/nitro/gx/auto/G2S_GetBG2CharPtr.c (ps2/tools/prep_sources.py). Do not edit. */
/* From BG mode 5 up, BG2 can be a bitmap layer: then it has no character base at all. */
void *G2S_GetBG2CharPtr(void) {
    int mode = *(volatile unsigned *)((unsigned int)kh_ds_io + 0x1000) & 7;
    unsigned cnt = *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x100c);
    if (mode < 5 || (cnt & 0x80) == 0) {
        return (void *)(((unsigned int)kh_ds_vram_win(1) + 0x0) + (((cnt & 0x3c) >> 2) << 14));
    }
    return 0;
}

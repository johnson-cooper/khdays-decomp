/* PS2: mechanically prepared copy of libs/nitro/gx/auto/G2_GetBG3ScrPtr.c (ps2/tools/prep_sources.py). Do not edit. */
/* BG3's screen base moves with the BG mode: tiled modes use the 2K slot, mode 5 uses the 16K one
 * when BG3 is a bitmap, and from mode 6 up BG3 has no screen at all. */
void *G2_GetBG3ScrPtr(void) {
    int mode = *(volatile unsigned *)((unsigned int)kh_ds_io + 0x0) & 7;
    unsigned cnt = *(volatile unsigned short *)((unsigned int)kh_ds_io + 0xe);
    unsigned base = ((*(volatile unsigned *)((unsigned int)kh_ds_io + 0x0) & 0x38000000) >> 27) << 16;
    unsigned slot = (cnt & 0x1f00) >> 8;
    switch (mode) {
    case 0:
    case 1:
    case 2:
        return (void *)(((unsigned int)kh_ds_vram_win(0) + 0x0) + base + (slot << 11));
    case 3:
    case 4:
    case 5:
        if ((cnt & 0x80) != 0) {
            return (void *)(((unsigned int)kh_ds_vram_win(0) + 0x0) + (slot << 14));
        }
        return (void *)(((unsigned int)kh_ds_vram_win(0) + 0x0) + base + (slot << 11));
    case 6:
        return 0;
    default:
        return 0;
    }
}

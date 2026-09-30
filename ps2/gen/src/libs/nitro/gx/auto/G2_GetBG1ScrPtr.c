/* PS2: mechanically prepared copy of libs/nitro/gx/auto/G2_GetBG1ScrPtr.c (ps2/tools/prep_sources.py). Do not edit. */
/* Main-engine BG base: the display-wide 64K block from DISPCNT plus the per-BG slot in BG1CNT. */
void *G2_GetBG1ScrPtr(void) {
    int slot = (*(volatile unsigned short *)((unsigned int)kh_ds_io + 0xa) & 0x1f00) >> 8;
    unsigned dispBase = (*(volatile unsigned *)((unsigned int)kh_ds_io + 0x0) & 0x38000000) >> 27;
    return (void *)(((unsigned int)kh_ds_vram_win(0) + 0x0) + (dispBase << 16) + (slot << 11));
}

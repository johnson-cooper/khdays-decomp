/* PS2: mechanically prepared copy of libs/nitro/gx/auto/G2S_GetBG1ScrPtr.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sub-engine BG base: 0x06200000 plus the per-BG slot in BG1CNT (no display-wide block). */
void *G2S_GetBG1ScrPtr(void) {
    int slot = (*(volatile unsigned short *)((unsigned int)kh_ds_io + 0x100a) & 0x1f00) >> 8;
    return (void *)(((unsigned int)kh_ds_vram_win(1) + 0x0) + (slot << 11));
}

/* PS2: mechanically prepared copy of src/engine/Bg_WriteMainBg0Cnt.c (ps2/tools/prep_sources.py). Do not edit. */
/* Writes the main engine's BG0CNT (0x04000008) from its fields, keeping the priority and mosaic
 * bits (0x43); the caller gives the extended-palette slot (Bg_SetMainBg0Control picks it itself). */
void Bg_WriteMainBg0Cnt(int size, int colorMode, int screenBase, int charBase, int extPal) {
    volatile unsigned short *reg_bg0cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x8);
    *reg_bg0cnt = *reg_bg0cnt & 0x43
        | (size << 0xe) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) | (extPal << 0xd);
}

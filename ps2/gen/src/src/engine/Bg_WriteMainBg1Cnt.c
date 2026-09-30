/* PS2: mechanically prepared copy of src/engine/Bg_WriteMainBg1Cnt.c (ps2/tools/prep_sources.py). Do not edit. */
/* Writes the main engine's BG1CNT (0x0400000a) from its fields, keeping the priority and mosaic
 * bits (0x43); the caller gives the extended-palette slot. */
void Bg_WriteMainBg1Cnt(int size, int colorMode, int screenBase, int charBase, int extPal) {
    volatile unsigned short *reg_bg1cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xa);
    *reg_bg1cnt = *reg_bg1cnt & 0x43
        | (size << 0xe) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) | (extPal << 0xd);
}

/* PS2: mechanically prepared copy of src/engine/Bg_SetMainBg0Control.c (ps2/tools/prep_sources.py). Do not edit. */
/* Program BG0CNT (0x04000008) for a BG ext-palette slot. Reads DISPCNT VRAM-mode
 * bits to pick the palette-load path, then writes BG0CNT preserving 0x43. */
extern int GX_GetBankForBGExtPltt(int arg);
extern void GX_SetGraphicsMode(int a, int b, int c);

void Bg_SetMainBg0Control(int size, int colorMode, int screenBase, int charBase) {
    int bank = GX_GetBankForBGExtPltt(size);
    int extPal;
    if (bank == 0x20 || bank == 0x10 || bank == 0x60) {
        extPal = 0;
    } else {
        extPal = 1;
    }
    GX_SetGraphicsMode(1, *(volatile unsigned int *)((unsigned int)kh_ds_io + 0x0) & 7, 0);
    {
        volatile unsigned short *reg_bg0cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x8);
        *reg_bg0cnt = (*reg_bg0cnt & 0x43)
            | (size << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) | (extPal << 13);
    }
}

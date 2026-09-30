/* PS2: mechanically prepared copy of src/engine/Bg_SetMainBg1Control.c (ps2/tools/prep_sources.py). Do not edit. */
/* Program BG1CNT (0x0400000a) for a BG ext-palette slot. Reads DISPCNT VRAM-mode
 * bits (incl. bit 3) to pick the palette-load path, then writes BG1CNT keeping 0x43. */
extern int GX_GetBankForBGExtPltt(int arg);
extern void GX_SetGraphicsMode(int a, int b, int c);

void Bg_SetMainBg1Control(int size, int colorMode, int screenBase, int charBase) {
    int bank = GX_GetBankForBGExtPltt(size);
    int extPal;
    if (bank == 0x20 || bank == 0x10 || bank == 0x60) {
        extPal = 0;
    } else {
        extPal = 1;
    }
    int t = (*(volatile unsigned int *)((unsigned int)kh_ds_io + 0x0) & 8) ? 1 : 0;
    GX_SetGraphicsMode(1, *(volatile unsigned int *)((unsigned int)kh_ds_io + 0x0) & 7, t ? 1 : 0);
    {
        volatile unsigned short *reg_bg1cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xa);
        *reg_bg1cnt = (*reg_bg1cnt & 0x43)
            | (size << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2) | (extPal << 13);
    }
}

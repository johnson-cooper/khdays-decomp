/* PS2: mechanically prepared copy of src/engine/PackDisplayReg0x400000c.c (ps2/tools/prep_sources.py). Do not edit. */
/* Set BG2CNT (0x0400000c), preserving priority + mosaic bits (0x43). */
void PackDisplayReg0x400000c(int size, int colorMode, int screenBase, int charBase)
{
    volatile unsigned short *reg_bg2cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xc);
    unsigned short h = *reg_bg2cnt;
    *reg_bg2cnt = (h & 0x43) | (size << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2);
}

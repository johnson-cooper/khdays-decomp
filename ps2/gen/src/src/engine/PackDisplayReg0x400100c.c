/* PS2: mechanically prepared copy of src/engine/PackDisplayReg0x400100c.c (ps2/tools/prep_sources.py). Do not edit. */
/* Set BG2CNT engine B (0x0400100c), preserving priority + mosaic (0x43). */
void PackDisplayReg0x400100c(int size, int colorMode, int screenBase, int charBase)
{
    volatile unsigned short *reg_bg2cnt_b = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x100c);
    unsigned short h = *reg_bg2cnt_b;
    *reg_bg2cnt_b = (h & 0x43) | (size << 14) | (colorMode << 7) | (screenBase << 8) | (charBase << 2);
}

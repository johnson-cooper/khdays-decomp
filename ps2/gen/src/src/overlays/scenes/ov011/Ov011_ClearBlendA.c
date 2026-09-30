/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_ClearBlendA.c (ps2/tools/prep_sources.py). Do not edit. */
/* Disable colour special effects: BLDCNT (0x04000050) = 0. */
void Ov011_ClearBlendA(void) {
    volatile unsigned short *reg_bldcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x50);
    *reg_bldcnt = 0;
}

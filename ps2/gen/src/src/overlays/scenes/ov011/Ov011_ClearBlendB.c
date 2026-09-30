/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_ClearBlendB.c (ps2/tools/prep_sources.py). Do not edit. */
/* Disable colour special effects on engine B: BLDCNT (0x04001050) = 0. */
void Ov011_ClearBlendB(void) {
    volatile unsigned short *reg_bldcnt_b = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x1050);
    *reg_bldcnt_b = 0;
}

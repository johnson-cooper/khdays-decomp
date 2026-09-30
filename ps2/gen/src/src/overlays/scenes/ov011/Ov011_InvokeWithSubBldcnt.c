/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_InvokeWithSubBldcnt.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sets the blend brightness on one screen's blend registers (G2x_SetBlendBrightness_). */

extern void *G2x_SetBlendBrightness_();

void *Ov011_InvokeWithSubBldcnt(int this_, int arg1) {
    return G2x_SetBlendBrightness_(((unsigned int)kh_ds_io + 0x1050), this_, arg1);
}

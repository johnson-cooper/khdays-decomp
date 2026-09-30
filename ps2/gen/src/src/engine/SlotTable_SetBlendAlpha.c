/* PS2: mechanically prepared copy of src/engine/SlotTable_SetBlendAlpha.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sets the alpha blend of the table's engine (main or sub) to alpha. */

extern void G2x_SetBlendAlpha_(unsigned int reg, int a, int b, int c, int d);

void SlotTable_SetBlendAlpha(char *p, int alpha) {
    if (*(int *)(p + 0x4604) == 2)
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 0, alpha, 0x10, 0);
    else
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x50), 0, alpha, 0x10, 0);
}

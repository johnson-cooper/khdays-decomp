/* PS2: mechanically prepared copy of src/overlays/screens/ov106/Ov106_ApplyFadeBlend.c (ps2/tools/prep_sources.py). Do not edit. */
/* Blend the ov106 fade layer: the current fade level (ov002 02053dc4) weights plane 8 against planes
 * 0x21 (level / 16 - level) on the main engine, or on the sub engine while +0x8e48 is 1. */
extern char *data_ov106_020b8b60;
extern int Ov002_Ui_GetState(void);
extern void G2x_SetBlendAlpha_(unsigned int reg, int a, int b, int c, int d);

void Ov106_ApplyFadeBlend(void)
{
    int level = Ov002_Ui_GetState();

    if (*(int *)(data_ov106_020b8b60 + 0x8e48) == 1) {
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x50), 8, 0x21, level, 0x10 - level);
    } else {
        G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x1050), 8, 0x21, level, 0x10 - level);
    }
}

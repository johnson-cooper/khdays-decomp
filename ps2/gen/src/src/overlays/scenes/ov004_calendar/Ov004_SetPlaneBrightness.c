/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/Ov004_SetPlaneBrightness.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sets the brightness blend of a background plane (or all planes), clamped to -16..16. */

extern void G2x_SetBlendBrightnessExt_(unsigned int reg, int firstMask,
                                       int secondMask, int eva, int evb,
                                       int brightness);

void Ov004_SetPlaneBrightness(int brightness, int plane) {
    if (plane == 0) {
        return;
    }

    if (brightness < -16) {
        brightness = -16;
    } else if (brightness > 16) {
        brightness = 16;
    }

    switch (plane) {
    case 1:
        G2x_SetBlendBrightnessExt_(((unsigned int)kh_ds_io + 0x50), 2, 0x3f, 0, -16, brightness);
        return;
    case 2:
        G2x_SetBlendBrightnessExt_(((unsigned int)kh_ds_io + 0x50), 4, 0x3f, 0, -16, brightness);
        return;
    case 3:
        G2x_SetBlendBrightnessExt_(((unsigned int)kh_ds_io + 0x50), 8, 0x3f, 0, -16, brightness);
        return;
    default:
        G2x_SetBlendBrightnessExt_(((unsigned int)kh_ds_io + 0x50), 0x3f, 0x3f, 0, -16, brightness);
        return;
    }
}

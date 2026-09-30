/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_FadeMasterBrightnessBothEngines.c (ps2/tools/prep_sources.py). Do not edit. */
/* Master-brightness fade for both 2D engines. Engine A gets plane mask 1; engine B takes its plane
 * mask from the live DISPCNT (0x04001000) layer-enable bits 8-12. brightness = 0 when disabled, -8
 * when enabled. */

extern void G2x_SetBlendBrightness_(void *reg, int planeMask, int brightness);
void Ov008_FadeMasterBrightnessBothEngines(int bEnable) {
    volatile unsigned int *pDispcntB = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000);
    int brightness = bEnable != 0 ? -8 : 0;
    G2x_SetBlendBrightness_((void *)((unsigned int)kh_ds_io + 0x50), 1, brightness);
    G2x_SetBlendBrightness_((char *)pDispcntB + 0x50, (*pDispcntB & 0x1f00) >> 8, brightness);
}

/* PS2: mechanically prepared copy of libs/nitro/gx/auto/GXS_SetGraphicsMode.c (ps2/tools/prep_sources.py). Do not edit. */
/* Replaces the BG mode bits of the sub-engine DISPCNT. */
void GXS_SetGraphicsMode(unsigned int mode) {
    volatile unsigned int *dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000);
    *dispcnt = (*dispcnt & ~7) | mode;
}

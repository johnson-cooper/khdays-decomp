/* PS2: mechanically prepared copy of libs/nitro/gx/calls/G3X_SetHOffset.c (ps2/tools/prep_sources.py). Do not edit. */
/* G3X_SetHOffset: writes the 3D engine horizontal offset register (0x04000010). */

void G3X_SetHOffset(int value) {
    *(volatile int *)((unsigned int)kh_ds_io + 0x10) = value;
}

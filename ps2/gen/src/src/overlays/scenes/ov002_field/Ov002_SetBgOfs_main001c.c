/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetBgOfs_main001c.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Set a background scroll register: pack the 9-bit H offset (param_1) and 9-bit V
 * offset (param_2) into the 32-bit register at 0x0400001c (BGxOFS pair).
 */
void Ov002_SetBgOfs_main001c(unsigned int param_1, unsigned int param_2) {
    *(volatile unsigned int *)((unsigned int)kh_ds_io + 0x1c) =
        (param_1 & 0x1ff) | ((param_2 << 16) & (0x1ff << 16));
}

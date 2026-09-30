/* PS2: mechanically prepared copy of src/overlays/scenes/ov006_mission_mode_select/Ov006_ConfigDispcntBothEngines.c (ps2/tools/prep_sources.py). Do not edit. */
/* Set display mode (DISPCNT bits 8-12) to mode 0x800 on both 2D engines. */
void Ov006_ConfigDispcntBothEngines(void) {
    volatile int *reg_dispcnt_a = (volatile int *)((unsigned int)kh_ds_io + 0x0);
    volatile int *reg_dispcnt_b = (volatile int *)((unsigned int)kh_ds_io + 0x1000);
    *reg_dispcnt_a = (*reg_dispcnt_a & ~0x1f00) | 0x800;
    *reg_dispcnt_b = (*reg_dispcnt_b & ~0x1f00) | 0x800;
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_DisableMainWindows.c (ps2/tools/prep_sources.py). Do not edit. */
extern int data_ov008_02090f1c;

/* Clear DISPCNT bits 13-15 of engine A (the WIN0, WIN1 and OBJ window enables). */
void Ov008_DisableMainWindows(void)
{
    volatile unsigned int *reg_dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x0);
    *reg_dispcnt &= ~0xe000;
    data_ov008_02090f1c = 0;
}

/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_InitDisplayRegs.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sub 2D engine (DISPCNT-B 0x04001000): set display mode 0x1f00 and give BG0..3
 * priorities 0,1,2,3 (BGnCNT-B priority = bits 0-1). */
void Ov025_InitDisplayRegs(void) {
    volatile unsigned int *reg_dispcnt_b = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000);
    *reg_dispcnt_b = *reg_dispcnt_b & 0xffffe0ff | 0x1f00;
    *(volatile unsigned short *)(reg_dispcnt_b + 2) &= ~3;
    *(volatile unsigned short *)((int)reg_dispcnt_b + 10) = *(volatile unsigned short *)((int)reg_dispcnt_b + 10) & ~3 | 1;
    *(volatile unsigned short *)(reg_dispcnt_b + 3) = *(volatile unsigned short *)(reg_dispcnt_b + 3) & ~3 | 2;
    *(volatile unsigned short *)((int)reg_dispcnt_b + 0xe) = *(volatile unsigned short *)((int)reg_dispcnt_b + 0xe) & ~3 | 3;
}

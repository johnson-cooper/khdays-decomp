/* PS2: mechanically prepared copy of libs/nitro/gx/calls/GX_DispOff.c (ps2/tools/prep_sources.py). Do not edit. */
/* Save DISPCNT's display-mode/VRAM-block field (bits 16-17) into data_020446d0,
 * clear the pending-request flag data_020422b4, and clear that field in DISPCNT. */
extern unsigned short data_020422b4;
extern unsigned short data_020446d0;

void GX_DispOff(void) {
    volatile unsigned int *reg_dispcnt = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x0);
    unsigned int disp = *reg_dispcnt;
    data_020422b4 = 0;
    data_020446d0 = (unsigned short)((disp & 0x30000) >> 0x10);
    *reg_dispcnt = disp & ~0x30000u;
}

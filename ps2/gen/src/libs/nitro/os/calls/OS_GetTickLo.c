/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_GetTickLo.c (ps2/tools/prep_sources.py). Do not edit. */
/* OS_GetTickLo: reads the low 16 bits of the tick timer (TM0CNT_L, 0x04000100). */

int OS_GetTickLo(void) {
    return *(volatile unsigned short *)((unsigned int)kh_ds_io + 0x100);
}

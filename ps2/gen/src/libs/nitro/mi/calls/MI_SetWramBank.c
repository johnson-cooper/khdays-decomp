/* PS2: mechanically prepared copy of libs/nitro/mi/calls/MI_SetWramBank.c (ps2/tools/prep_sources.py). Do not edit. */
/* MI_SetWramBank: selects the WRAM bank via WRAMCNT (0x04000247). */

void MI_SetWramBank(unsigned char bank) {
    *(volatile unsigned char *)((unsigned int)kh_ds_io + 0x247) = bank;
}

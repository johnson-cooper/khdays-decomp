/* PS2: mechanically prepared copy of libs/nitro/nitro/auto/CTRDGi_ChangeLatestAccessCycle.c (ps2/tools/prep_sources.py). Do not edit. */
/* Saves the two cartridge access-cycle fields of EXMEMCNT and switches them to the slowest
 * setting. */
void CTRDGi_ChangeLatestAccessCycle(int *saved) {
    volatile unsigned short *exmemcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x204);
    saved[0] = (*exmemcnt & 0xc) >> 2;
    saved[1] = (*exmemcnt & 0x10) >> 4;
    *exmemcnt = (unsigned short)((*exmemcnt & ~0xc) | 0xc);
    *exmemcnt = (unsigned short)(*exmemcnt & ~0x10);
}

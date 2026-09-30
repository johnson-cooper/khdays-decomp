/* PS2: mechanically prepared copy of libs/nitro/os/auto/OSi_AllocateCardBus.c (ps2/tools/prep_sources.py). Do not edit. */
/* Clears EXMEMCNT bit 11 so the ARM9 owns the card bus. */
void OSi_AllocateCardBus(void) {
    volatile unsigned short *exmemcnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x204);
    *exmemcnt = (unsigned short)(*exmemcnt & ~0x800);
}

/* PS2: mechanically prepared copy of libs/nitro/os/auto/OS_EnableIrqMask.c (ps2/tools/prep_sources.py). Do not edit. */
/* IME is read back before being restored: the hardware needs the read to settle. */
static inline unsigned OS_DisableInterrupts(void) {
    volatile unsigned short *ime = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x208);
    unsigned old = *ime;
    *ime = 0;
    return old;
}

static inline void OS_RestoreInterrupts(unsigned state) {
    volatile unsigned short *ime = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x208);
    (void)*ime;
    *ime = (unsigned short)state;
}

unsigned OS_EnableIrqMask(unsigned mask) {
    unsigned enabled = OS_DisableInterrupts();
    unsigned last = *(volatile unsigned *)((unsigned int)kh_ds_io + 0x210);
    *(volatile unsigned *)((unsigned int)kh_ds_io + 0x210) = last | mask;
    OS_RestoreInterrupts(enabled);
    return last;
}

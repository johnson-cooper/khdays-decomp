/* PS2: mechanically prepared copy of libs/nitro/mi/calls/func_01ff85d0.c (ps2/tools/prep_sources.py). Do not edit. */
extern int OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(int);

void func_01ff85d0(unsigned int dmaNo, unsigned int src, unsigned int dest, unsigned int ctrl) {
    int mask = OS_DisableInterrupts();
    volatile unsigned int *p = (volatile unsigned int *)(((unsigned int)kh_ds_io + 0xb0) + dmaNo * 12);
    *p = (volatile unsigned int)src;
    *(p + 1) = (volatile unsigned int)dest;
    *(p + 2) = (volatile unsigned int)ctrl;
    (void)*(volatile unsigned int *)((unsigned int)kh_ds_io + 0xb0);
    (void)*(volatile unsigned int *)((unsigned int)kh_ds_io + 0xb0);
    if (dmaNo == 0) {
        *p = 0;
        *(p + 1) = 0;
        *(p + 2) = 0x81400001;
    }
    OS_RestoreInterrupts(mask);
}

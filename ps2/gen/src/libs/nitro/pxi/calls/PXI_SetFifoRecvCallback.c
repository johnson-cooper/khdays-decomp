/* PS2: mechanically prepared copy of libs/nitro/pxi/calls/PXI_SetFifoRecvCallback.c (ps2/tools/prep_sources.py). Do not edit. */
extern int OS_DisableInterrupts(void);
extern void OS_RestoreInterrupts(int state);

extern void *data_02046288[];

/* Installs (or removes) the handler for one PXI FIFO tag and keeps the enabled-tag mask in the
 * shared work area in sync. */
void PXI_SetFifoRecvCallback(int tag, void *callback) {
    int enabled = OS_DisableInterrupts();
    int *shared = (int *)((unsigned int)kh_ds_hiram + 0x1fc00);
    data_02046288[tag] = callback;
    if (callback != 0) {
        shared[0xe2] |= 1 << tag;
    } else {
        shared[0xe2] &= ~(1 << tag);
    }
    OS_RestoreInterrupts(enabled);
}

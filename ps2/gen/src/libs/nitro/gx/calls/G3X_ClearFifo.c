/* PS2: mechanically prepared copy of libs/nitro/gx/calls/G3X_ClearFifo.c (ps2/tools/prep_sources.py). Do not edit. */
/* Flushes the geometry FIFO with 128 NOP commands and waits for the engine to go idle. */
extern void GXi_NopClearFifo128_(volatile unsigned int *fifo);

void G3X_ClearFifo(void) {
    GXi_NopClearFifo128_((volatile unsigned int *)((unsigned int)kh_ds_io + 0x400));
    while (*(volatile unsigned int *)((unsigned int)kh_ds_io + 0x600) & 0x8000000) {
        ;
    }
}

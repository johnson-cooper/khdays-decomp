/* PS2: mechanically prepared copy of libs/nitro/mi/calls/MIi_DMACallback.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/os.h"

extern unsigned int OS_DisableIrqMask(unsigned int mask);
extern void OS_SetIrqFunction(unsigned int intrBits, OSIrqFunction function);
extern int data_020446b0;

/* Geometry-FIFO DMA completion handler, the undo of what MI_SendGXCommandAsync
 * set up: it drops the DMA0 IRQ mask, restores GXSTAT's command-FIFO config bits
 * (30-31) from the value saved at +0x18, puts the previous DMA0 IRQ handler saved
 * at +0x1c back in place, clears the busy flag, and dispatches the caller's own
 * completion callback stored at +0x10 with its argument at +0x14. */
void MIi_DMACallback(void) {
    void (*callback)(void *);
    void *callbackArg;

    OS_DisableIrqMask(0x200000);kh_ge_port_write1(0x600, (unsigned int)((*(unsigned int *)((char *)&data_020446b0 + 0x18) << 30) |
        (*(volatile unsigned int *)((unsigned int)kh_ds_io + 0x600) & ~0xc0000000)));

    OS_SetIrqFunction(0x200000, *(OSIrqFunction *)((char *)&data_020446b0 + 0x1c));

    data_020446b0 = 0;
    callback = *(void (**)(void *))((int)&data_020446b0 + 0x10);
    callbackArg = *(void **)((int)&data_020446b0 + 0x14);
    if (callback != 0) {
        callback(callbackArg);
    }
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetDisplaySwap.c (ps2/tools/prep_sources.py). Do not edit. */
/* Set which physical LCD shows the main engine. While the deferred-swap slot at
 * data_ov002_0207f408 is still armed (non-negative) the request is only parked
 * there and applied later; once it has gone negative the swap goes straight into
 * POWCNT1 bit 15. */
extern int data_ov002_0207f408;

void Ov002_SetDisplaySwap(int top) {
    volatile unsigned short *reg304;

    if (data_ov002_0207f408 >= 0) {
        data_ov002_0207f408 = top;
        return;
    }

    reg304 = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x304);
    *reg304 = (*reg304 & ~0x8000) | (top << 15);
}

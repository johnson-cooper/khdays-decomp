/* PS2: mechanically prepared copy of src/engine/remapIndexIfHwFlagSet.c (ps2/tools/prep_sources.py). Do not edit. */
extern unsigned short data_02041dd4[];

/* If the BGnCNT register for BG `param_1` has its ext-palette bit (0x2000) set,
 * the caller consumes two palette slots (param_1 + 2). */
int remapIndexIfHwFlagSet(int param_1) {
    unsigned short regOff = data_02041dd4[param_1];
    if (regOff == 0) return param_1;
    {
        volatile unsigned short *reg = (volatile unsigned short *)(regOff + ((unsigned int)kh_ds_io + 0x0));
        if (*reg & 0x2000) return param_1 + 2;
    }
    return param_1;
}

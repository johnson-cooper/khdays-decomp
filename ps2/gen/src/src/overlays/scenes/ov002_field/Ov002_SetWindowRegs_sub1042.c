/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetWindowRegs_sub1042.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Program a pair of window-coordinate registers at 0x04001042: pack (param_1 high
 * byte, param_3 low byte) into the halfword at +0 and (param_2, param_4) into the
 * halfword at +4.
 */
void Ov002_SetWindowRegs_sub1042(unsigned int param_1, unsigned int param_2,
                         unsigned int param_3, unsigned int param_4) {
    volatile unsigned short *reg = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x1042);

    reg[0] = ((param_1 << 8) & 0xff00) | (param_3 & 0xff);
    reg[2] = ((param_2 << 8) & 0xff00) | (param_4 & 0xff);
}

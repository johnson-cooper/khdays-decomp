/* PS2: mechanically prepared copy of libs/nitro/card/auto/CARDi_SetRomOp.c (ps2/tools/prep_sources.py). Do not edit. */
/* Writes the 8-byte ROM command (two big-endian words) into CARD_COMMAND, after waiting
 * for the card control register to go idle. */
void CARDi_SetRomOp(unsigned int hi, unsigned int lo) {
    volatile unsigned char *cmd;
    while (*(volatile unsigned int *)((unsigned int)kh_ds_io + 0x1a4) & 0x80000000) {
        ;
    }
    cmd = (volatile unsigned char *)((unsigned int)kh_ds_io + 0x1a1);
    cmd[0] = 0xc0;
    cmd[7] = (unsigned char)(hi >> 24);
    cmd[8] = (unsigned char)(hi >> 16);
    cmd[9] = (unsigned char)(hi >> 8);
    cmd[10] = (unsigned char)hi;
    cmd[11] = (unsigned char)(lo >> 24);
    cmd[12] = (unsigned char)(lo >> 16);
    cmd[13] = (unsigned char)(lo >> 8);
    cmd[14] = (unsigned char)lo;
}

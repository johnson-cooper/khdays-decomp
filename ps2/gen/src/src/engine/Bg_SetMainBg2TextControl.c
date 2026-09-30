/* PS2: mechanically prepared copy of src/engine/Bg_SetMainBg2TextControl.c (ps2/tools/prep_sources.py). Do not edit. */
/* Selects the text-BG2 display mode and sets main-engine BG2CNT. */

extern void DispMode_LookupWordAndDispatch(void *ptr);
extern char data_02041f4c;

void Bg_SetMainBg2TextControl(int arg0, int arg1, int arg2, int arg3) {
    volatile unsigned short *reg_bg2cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xc);

    DispMode_LookupWordAndDispatch(&data_02041f4c);
    *reg_bg2cnt = (*reg_bg2cnt & 0x43) | (arg0 << 14) | (arg1 << 7) | (arg2 << 8) | (arg3 << 2);
}

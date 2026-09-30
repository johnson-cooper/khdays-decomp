/* PS2: mechanically prepared copy of src/engine/Bg_SetMainBg3TextControl.c (ps2/tools/prep_sources.py). Do not edit. */
/* Selects the text-BG3 display mode and sets main-engine BG3CNT. */

extern void DispMode_LookupWordAndDispatch(void *ptr);
extern char data_02041eec;

void Bg_SetMainBg3TextControl(int arg0, int arg1, int arg2, int arg3) {
    volatile unsigned short *reg_bg3cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xe);

    DispMode_LookupWordAndDispatch(&data_02041eec);
    *reg_bg3cnt = (*reg_bg3cnt & 0x43) | (arg0 << 14) | (arg1 << 7) | (arg2 << 8) | (arg3 << 2);
}

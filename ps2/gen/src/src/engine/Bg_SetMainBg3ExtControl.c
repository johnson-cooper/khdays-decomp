/* PS2: mechanically prepared copy of src/engine/Bg_SetMainBg3ExtControl.c (ps2/tools/prep_sources.py). Do not edit. */
/* Selects the extended-BG3 display mode and sets main-engine BG3CNT. */

extern void DispMode_LookupWordAndDispatch(void *ptr);
extern char data_02041e8c;
extern int data_02047390;

void Bg_SetMainBg3ExtControl(int arg0, int arg1, int arg2) {
    volatile unsigned short *reg_bg3cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xe);
    int arg3;

    DispMode_LookupWordAndDispatch(&data_02041e8c);
    arg3 = data_02047390;
    *reg_bg3cnt = (*reg_bg3cnt & 0x43) | (arg0 << 14) | (arg2 << 2) | (arg1 << 8) | (arg3 << 13);
}

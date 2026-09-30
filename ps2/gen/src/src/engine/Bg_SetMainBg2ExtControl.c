/* PS2: mechanically prepared copy of src/engine/Bg_SetMainBg2ExtControl.c (ps2/tools/prep_sources.py). Do not edit. */
/* Selects the extended-BG2 display mode and sets main-engine BG2CNT. */

extern void DispMode_LookupWordAndDispatch(void *ptr);
extern char data_02041f0c;
extern int data_02047390;

void Bg_SetMainBg2ExtControl(int arg0, int arg1, int arg2) {
    volatile unsigned short *reg_bg2cnt = (volatile unsigned short *)((unsigned int)kh_ds_io + 0xc);
    int arg3;

    DispMode_LookupWordAndDispatch(&data_02041f0c);
    arg3 = data_02047390;
    *reg_bg2cnt = (*reg_bg2cnt & 0x43) | (arg0 << 14) | (arg2 << 2) | (arg1 << 8) | (arg3 << 13);
}

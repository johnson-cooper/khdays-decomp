/* PS2: mechanically prepared copy of src/engine/Bg_SetSubBg3ExtControl.c (ps2/tools/prep_sources.py). Do not edit. */
/* Selects the sub extended-BG3 display mode and sets sub-engine BG3CNT. */

extern void SetSubEngineGraphicsModeFromTable(void *ptr);
extern char data_02041e8c;
extern int data_02047390;

void Bg_SetSubBg3ExtControl(int arg0, int arg1, int arg2) {
    volatile unsigned short *reg_bg3cnt_b = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x100e);
    int arg3;

    SetSubEngineGraphicsModeFromTable(&data_02041e8c);
    arg3 = data_02047390;
    *reg_bg3cnt_b = (*reg_bg3cnt_b & 0x43) | (arg0 << 14) | (arg2 << 2) | (arg1 << 8) | (arg3 << 13);
}

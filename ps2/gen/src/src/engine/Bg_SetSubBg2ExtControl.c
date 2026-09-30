/* PS2: mechanically prepared copy of src/engine/Bg_SetSubBg2ExtControl.c (ps2/tools/prep_sources.py). Do not edit. */
/* Selects the sub extended-BG2 display mode and sets sub-engine BG2CNT. */

extern void SetSubEngineGraphicsModeFromTable(void *ptr);
extern char data_02041f0c;
extern int data_02047390;

void Bg_SetSubBg2ExtControl(int arg0, int arg1, int arg2) {
    volatile unsigned short *reg_bg2cnt_b = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x100c);
    int arg3;

    SetSubEngineGraphicsModeFromTable(&data_02041f0c);
    arg3 = data_02047390;
    *reg_bg2cnt_b = (*reg_bg2cnt_b & 0x43) | (arg0 << 14) | (arg2 << 2) | (arg1 << 8) | (arg3 << 13);
}

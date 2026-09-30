/* PS2: mechanically prepared copy of src/engine/Bg_SetSubBg2TextControl.c (ps2/tools/prep_sources.py). Do not edit. */
/* Selects the sub text-BG2 display mode and sets sub-engine BG2CNT. */

extern void SetSubEngineGraphicsModeFromTable(void *ptr);
extern char data_02041f4c;

void Bg_SetSubBg2TextControl(int arg0, int arg1, int arg2, int arg3) {
    volatile unsigned short *reg_bg2cnt_b = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x100c);

    SetSubEngineGraphicsModeFromTable(&data_02041f4c);
    *reg_bg2cnt_b = (*reg_bg2cnt_b & 0x43) | (arg0 << 14) | (arg1 << 7) | (arg2 << 8) | (arg3 << 2);
}

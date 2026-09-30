/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetScrollPosition.c (ps2/tools/prep_sources.py). Do not edit. */
/* Update the scroll position: scale the row by 0x4d and divide by the viewport
 * height at +0xcc, then store the pair at +0x34/+0x38 and relayout -- but only
 * when it actually changed. kh_rt_s32_divmod is the signed divide helper; the
 * QUOTIENT is the low half of its return. */
extern long long kh_rt_s32_divmod(int a, int b);
extern void Ov002_RepaintPanelRows(void);

extern char *data_ov002_0207f618;

void Ov002_SetScrollPosition(int row, int offset) {
    char *ctx = data_ov002_0207f618;
    int scaled;

    if (ctx == 0) {
        return;
    }

    scaled = (int)kh_rt_s32_divmod(offset * 0x4d, *(unsigned short *)(ctx + 0xcc));

    if (*(int *)(ctx + 0x34) == row && *(unsigned short *)(ctx + 0x38) == scaled) {
        return;
    }

    *(int *)(ctx + 0x34) = row;
    *(short *)(ctx + 0x38) = (short)scaled;
    Ov002_RepaintPanelRows();
}

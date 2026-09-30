/* PS2: mechanically prepared copy of src/overlays/screens/ov026_shop/Ov026_ScrollListToRow.c (ps2/tools/prep_sources.py). Do not edit. */
extern char *data_ov026_02091368;
extern long long kh_rt_s32_divmod(int a, int b);
extern void Ov026_LayoutListAtScroll(int y);

/* Scrolls the list to the given row, clamped to the scrollable range. */
void Ov026_ScrollListToRow(int row, int base) {
    char *list = *(char **)&data_ov026_02091368 + 0xc57c;
    int last;
    if (*(int *)(list + 0x3c) > 8) {
        *(int *)(list + 0x40) = row;
        if (row < 0) {
            row = 0;
        }
        last = *(int *)(list + 0x3c) - 8;
        if (row > last) {
            row = last;
        }
        Ov026_LayoutListAtScroll(base + (int)kh_rt_s32_divmod(
            row * ((0x10 - *(int *)(list + 0x44)) * 8), last));
    }
}

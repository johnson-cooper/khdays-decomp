/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_ResetListTo.c (ps2/tools/prep_sources.py). Do not edit. */
extern char *data_ov008_02090fac;
extern void Slot_SetVisible(int handle, int cell, int visible);
extern long long kh_rt_s32_divmod(int a, int b);
extern void Ov008_LayoutListAtScroll(int y);

/* Resets the list to `total` entries: hides every row, recomputes the scrollbar thumb height
 * (clamped to 4..16 pixels) and shows the frame. */
void Ov008_ResetListTo(int total) {
    char *st = *(char **)&data_ov008_02090fac;
    char *list = st + 0xc57c;
    int handle = *(int *)(st + 0xbfb0);
    int i;
    for (i = 0; i < 0xc; i++) {
        Slot_SetVisible(handle, *(int *)(list + i * sizeof(int) + 8), 0);
    }
    *(int *)(list + 0x3c) = total;
    *(int *)(list + 0x40) = 0;
    *(int *)(list + 0x48) = 0;
    if (total == 0) {
        *(int *)(list + 0x44) = 0x10;
    } else {
        *(int *)(list + 0x44) = (int)kh_rt_s32_divmod(0x80, total);
        if (*(int *)(list + 0x44) < 4) {
            *(int *)(list + 0x44) = 4;
        }
        if (*(int *)(list + 0x44) > 0x10) {
            *(int *)(list + 0x44) = 0x10;
        }
    }
    *(int *)(list + 0x38) = *(int *)(list + 0x44) - 4;
    Slot_SetVisible(handle, *(int *)list, 1);
    Slot_SetVisible(handle, *(int *)(list + 4), 1);
    Ov008_LayoutListAtScroll(0);
}

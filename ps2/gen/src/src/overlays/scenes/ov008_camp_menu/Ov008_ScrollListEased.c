/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_ScrollListEased.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_ScrollListEased -- scroll the menu list to a target row with easing, ov008.
 * Kicks the low-level scroll (Ov008_LayoutMenuRows2) then computes the eased pixel delta:
 * a 64-bit multiply of the row span ((p3-p4)*0x10 - 1) * p2 against the remaining distance
 * (p5 - obj+0x2f8), rounded down to whole rows (/16). If a scroll is already committed
 * (obj+8 != 0) it stops; otherwise it applies the delta to the list object (obj+0x70) via
 * Ov008_RefreshInventoryRows. */
extern void Ov008_LayoutMenuRows2(int obj, int a, int b, int c);
extern long long kh_rt_s32_divmod(int a, int b);
extern void Ov008_RefreshInventoryRows(int obj, int listObj, int rows);

void Ov008_ScrollListEased(int obj, int p2, int p3, int p4, int p5) {
    long long v;
    int iv;
    Ov008_LayoutMenuRows2(obj, p2, p3, p4);
    v = kh_rt_s32_divmod(p2 * ((p3 - p4) * 0x10 - 1), p5 - *(int *)(obj + 0x2f8));
    if (*(int *)(obj + 8) != 0) {
        return;
    }
    iv = (int)v + 8;
    Ov008_RefreshInventoryRows(obj, *(int *)(obj + 0x70),
                        (int)(iv + ((unsigned int)(iv >> 3) >> 0x1c)) >> 4);
}

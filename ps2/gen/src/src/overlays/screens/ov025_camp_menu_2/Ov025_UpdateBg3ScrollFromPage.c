/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_UpdateBg3ScrollFromPage.c (ps2/tools/prep_sources.py). Do not edit. */
/* Push the current page's scroll value (field 0x168, shifted into bits 16-24) into engine B's
 * BG3 H/V scroll registers, and reload the BG3 tilemap once when the page's dirty flag (0x16c) is
 * set. Always returns 1. */
extern int Ov025_GetPageB(void);
extern int Ov025_LookupEntry(int entry);
extern void GXS_LoadBG3Scr(int src, int offset, int size);

int Ov025_UpdateBg3ScrollFromPage(void) {
    int page = Ov025_GetPageB();
    volatile int *pScroll = (volatile int *)((unsigned int)kh_ds_io + 0x1018);
    pScroll[0] = 0x01ff0000 & (*(int *)(page + 0x168) << 16);
    pScroll[1] = 0x01ff0000 & (*(int *)(page + 0x168) << 16);
    if (*(int *)(page + 0x16c) != 0) {
        int scr = Ov025_LookupEntry(0x1b);
        GXS_LoadBG3Scr(scr, 0, 0x800);
        *(int *)(page + 0x16c) = 0;
    }
    return 1;
}

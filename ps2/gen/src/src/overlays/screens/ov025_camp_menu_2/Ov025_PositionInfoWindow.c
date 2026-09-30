/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_PositionInfoWindow.c (ps2/tools/prep_sources.py). Do not edit. */
/* Reposition the info window: mirror the masked X (param_1 & 0x1ff) into engine A BG1 H/V scroll,
 * scroll the render context by -param_1*0x1000, and set the window bounds (WIN0H from 0x68/0xe9
 * minus param_1) and control (param_2 low byte | 0x1800). */
extern int Ov025_GetContext(void);
extern void Ov025_OffsetActiveEntryX(int ctx, int dx, int flag);
extern void Ov025_CallSelectionHandler(int ctx, int flag);

void Ov025_PositionInfoWindow(unsigned int param_1, unsigned short param_2) {
    volatile unsigned int *pScroll = (volatile unsigned int *)((unsigned int)kh_ds_io + 0x14);
    volatile unsigned short *pWin = (volatile unsigned short *)((unsigned int)kh_ds_io + 0x40);
    unsigned int v = param_1 & 0x1ff;
    int ctx;
    pScroll[0] = v;
    pScroll[1] = v;
    ctx = Ov025_GetContext();
    Ov025_OffsetActiveEntryX(ctx, param_1 * -0x1000, 0);
    Ov025_CallSelectionHandler(ctx, 0);
    pWin[0] = ((0x68 - param_1) << 8 & 0xff00) | ((0xe9 - param_1) & 0xff);
    pWin[2] = (param_2 & 0xff) | 0x1800;
}

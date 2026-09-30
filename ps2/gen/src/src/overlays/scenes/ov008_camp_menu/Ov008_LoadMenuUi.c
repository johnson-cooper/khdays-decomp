/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_LoadMenuUi.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_LoadMenuUi -- load the menu UI container and build its root cell, ov008.
 * Runs once (guarded by heap[0x608]): clears the two OBJ palettes, loads the UI archive
 * (gOv008UiMltResPath = "UI/mlt/res.p2") via Msg_OpenContainerAndReadHeader, then registers the root cell
 * in the object manager (heap+0x60c) from a {resAddr, 2, 0, 0} descriptor (ObjNode_InitFromDesc),
 * creates it (func_02032444 slot 5 -> heap[0x5044]), sets frame 0 and scale
 * 1.0, and enables sub-BG mode 1. */
extern char *data_ov008_02090f00;
extern char  gOv008UiMltResPath[];
extern void *Msg_OpenContainerAndReadHeader(void *desc, int mode);
extern void  SetMasterBrightnessSub(int);
extern void  ObjNode_InitFromDesc(void *mgr, int *desc);
extern void *func_02032444(void *mgr, int slot, int);
extern void  Slot_ForwardToEntry(void *mgr, void *obj, int);
extern void  Slot_ClearFlagBit1(void *mgr, void *obj);
extern void  Slot_SetPosition(void *mgr, void *obj, int *scale);

void Ov008_LoadMenuUi(void) {
    if (*(int *)(data_ov008_02090f00 + 0x608) != 0) {
        return;
    }
    *(unsigned short *)((unsigned int)kh_ds_pal + 0x0) = 0;
    *(unsigned short *)((unsigned int)kh_ds_pal + 0x400) = 0;
    *(void **)(data_ov008_02090f00 + 0x608) = Msg_OpenContainerAndReadHeader(gOv008UiMltResPath, 0xe);
    SetMasterBrightnessSub(0);
    {
        int desc[4];
        desc[0] = ((*(int *)(data_ov008_02090f00 + 0x608) + 0x8000 & 0xfffffc) << 7) | 0x80000001;
        desc[1] = 2;
        desc[2] = 0;
        desc[3] = 0;
        ObjNode_InitFromDesc(data_ov008_02090f00 + 0x60c, desc);
    }
    *(void **)(data_ov008_02090f00 + 0x5044) =
        func_02032444(data_ov008_02090f00 + 0x60c, 5, 0);
    {
        int scale[2];
        scale[0] = 0x8000;
        scale[1] = 0x8000;
        Slot_ForwardToEntry(data_ov008_02090f00 + 0x60c, *(void **)(data_ov008_02090f00 + 0x5044), 0);
        Slot_ClearFlagBit1(data_ov008_02090f00 + 0x60c, *(void **)(data_ov008_02090f00 + 0x5044));
        Slot_SetPosition(data_ov008_02090f00 + 0x60c, *(void **)(data_ov008_02090f00 + 0x5044), scale);
    }
    *(volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000) = (*(volatile unsigned int *)((unsigned int)kh_ds_io + 0x1000) & ~0x1f00) | 0x1000;
}

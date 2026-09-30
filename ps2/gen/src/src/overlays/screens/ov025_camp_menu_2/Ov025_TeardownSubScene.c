/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_TeardownSubScene.c (ps2/tools/prep_sources.py). Do not edit. */
extern void Ov025_ProcessAllAtField1cc(char *self);
extern void Ov025_FreeResourceRecordBuffer(char *p);
extern void FreeAllListNodeSubBuffers(char *p);
extern void Ov025_DecRefSlot(int id);
extern int Ov025_GetMenuMsgDbId(void);
extern int Ov025_GetCtxBlock954c(void);
extern int Ov025_SweepElements(int a);
extern int Ov025_GetBlock4a80(int a);
extern void Ov025_DestroyAllListObjects(int a);
extern void Ov025_ReleaseIfMarked(int a);
extern void Ov025_ListRemoveAndFree(int node);
extern int data_ov025_020b575c;

/* Scene teardown: releases the sub-allocators, drops the three fixed resources plus the current
 * one, unwinds the active object and clears the sub-engine's window registers. */
void Ov025_TeardownSubScene(char *self) {
    int obj;
    Ov025_ProcessAllAtField1cc(self);
    Ov025_FreeResourceRecordBuffer(self + 0x58);
    FreeAllListNodeSubBuffers(self + 0x64);
    FreeAllListNodeSubBuffers(self + 0xa0);
    FreeAllListNodeSubBuffers(self + 0xdc);
    FreeAllListNodeSubBuffers(self + 0x118);
    FreeAllListNodeSubBuffers(self + 0x154);
    FreeAllListNodeSubBuffers(self + 0x190);
    Ov025_DecRefSlot(0x1c);
    Ov025_DecRefSlot(0x15);
    Ov025_DecRefSlot(0x13);
    Ov025_DecRefSlot(Ov025_GetMenuMsgDbId());
    obj = Ov025_GetBlock4a80(Ov025_SweepElements(Ov025_GetCtxBlock954c()));
    Ov025_DestroyAllListObjects(obj);
    Ov025_ReleaseIfMarked(obj);
    Ov025_ListRemoveAndFree(*(int *)(self + 0x23c));
    {
        volatile int *win = (volatile int *)((unsigned int)kh_ds_io + 0x1010);
        win[0] = 0;
        win[1] = 0;
        win[2] = 0;
        win[3] = 0;
        data_ov025_020b575c = 0;
    }
}

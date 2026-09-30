/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_TeardownSubScene.c (ps2/tools/prep_sources.py). Do not edit. */
extern void Ov008_ForEachNode(char *self);
extern void Ov008_FreeResourceRecordBuffer(char *p);
extern void FreeAllListNodeSubBuffers(char *p);
extern void Ov008_DecRefSlot(int id);
extern int Ov008_GetLocalPlayerStatB(void);
extern int Ov008_GetCtxBlock954c(void);
extern int Ov008_SweepElements(int a);
extern int Ov008_GetCtxBlock4a80(int a);
extern void Ov008_DestroyAllListObjects(int a);
extern void Ov008_ReleaseIfMarked(int a);
extern void Ov008_ListRemoveAndFree(int node);
extern int data_ov008_02090f20;

/* Scene teardown: releases the sub-allocators, drops the three fixed resources plus the current
 * one, unwinds the active object and clears the sub-engine's window registers. */
void Ov008_TeardownSubScene(char *self) {
    int obj;
    Ov008_ForEachNode(self);
    Ov008_FreeResourceRecordBuffer(self + 0x58);
    FreeAllListNodeSubBuffers(self + 0x64);
    FreeAllListNodeSubBuffers(self + 0xa0);
    FreeAllListNodeSubBuffers(self + 0xdc);
    FreeAllListNodeSubBuffers(self + 0x118);
    FreeAllListNodeSubBuffers(self + 0x154);
    FreeAllListNodeSubBuffers(self + 0x190);
    Ov008_DecRefSlot(0x1c);
    Ov008_DecRefSlot(0x15);
    Ov008_DecRefSlot(0x13);
    Ov008_DecRefSlot(Ov008_GetLocalPlayerStatB());
    obj = Ov008_GetCtxBlock4a80(Ov008_SweepElements(Ov008_GetCtxBlock954c()));
    Ov008_DestroyAllListObjects(obj);
    Ov008_ReleaseIfMarked(obj);
    Ov008_ListRemoveAndFree(*(int *)(self + 0x23c));
    {
        volatile int *win = (volatile int *)((unsigned int)kh_ds_io + 0x1010);
        win[0] = 0;
        win[1] = 0;
        win[2] = 0;
        win[3] = 0;
        data_ov008_02090f20 = 0;
    }
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_TeardownBriefing.c (ps2/tools/prep_sources.py). Do not edit. */
extern int Ov002_Ctx_FindActiveEntryByTag(int id);
extern void Ov002_Ctx_SetTagTrackerNodeArmed_5(int handle, int a);
extern int Ov002_GetPanelField005c(void);
extern int Ov002_ForwardToSubDc(int id);
extern void Ov002_Ctx_InvokeTagTrackerCallback(int);
extern void func_ov002_02067944(char *p);
extern void NNSi_FndFreeFromDefaultHeap(void *p);
extern void ZeroHalfThenFree(int h);
extern void Ov002_ReleasePageSurfaces(void);
extern void Ov002_FreeResourceRecordBuffer(char *p);
extern char *data_ov002_0207f9fc;

/* Mission-briefing teardown: drops the five widgets, prints the two closing lines when the
 * briefing was not skipped, releases the buffers and restores the sub-engine's BG priority. */
void Ov002_TeardownBriefing(void) {
    char *self = data_ov002_0207f9fc;
    Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(0x18), 0);
    Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(0x19), 0);
    Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(0x16), 0);
    Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(0x15), 0);
    Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(0x17), 0);
    if (Ov002_GetPanelField005c() == 0) {
        Ov002_Ctx_InvokeTagTrackerCallback(Ov002_ForwardToSubDc(0x57));
        Ov002_Ctx_InvokeTagTrackerCallback(Ov002_ForwardToSubDc(0x53));
    }
    func_ov002_02067944(self + 0xc);
    {
        void *tmp = *(void **)(self + 0x20);
        if (tmp != 0) {
            NNSi_FndFreeFromDefaultHeap(tmp);
        }
    }
    ZeroHalfThenFree(*(int *)(self + 8));
    ZeroHalfThenFree(*(int *)(self + 4));
    Ov002_ReleasePageSurfaces();
    Ov002_FreeResourceRecordBuffer(self + (0x67 << 2));
    *(volatile unsigned *)((unsigned int)kh_ds_io + 0x1000) =
        (*(volatile unsigned *)((unsigned int)kh_ds_io + 0x1000) & 0xffffe0ff) | (*(int *)(self + 0x2c) << 8);
    data_ov002_0207f9fc = 0;
}

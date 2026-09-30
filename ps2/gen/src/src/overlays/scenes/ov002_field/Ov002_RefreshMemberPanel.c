/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_RefreshMemberPanel.c (ps2/tools/prep_sources.py). Do not edit. */
extern int Ov002_ClassifyCode(int *out, int kind);
extern int Ov002_CountPanelListEntries(void);
extern int Ov002_CountSecondListEntries(void);
extern void Ov002_HandlePanelInput(int a, int b);
extern void Ov002_Panel_MoveCursor(int a);
extern long long kh_rt_s32_divmod(int a, int b);
extern char *data_ov002_0207f620;

/* Refreshes the member panel for the current list kind: clears it when the list is empty, and
 * for kind 5 republishes the member id and its row. */
void Ov002_RefreshMemberPanel(void) {
    int slot;
    char *self = data_ov002_0207f620;
    switch (Ov002_ClassifyCode(&slot, *(unsigned char *)(self + 1))) {
    case 2:
        if (Ov002_CountPanelListEntries() == 0) {
            Ov002_HandlePanelInput(0, -1);
            Ov002_Panel_MoveCursor(0);
        }
        return;
    case 3:
        if (Ov002_CountSecondListEntries() == 0) {
            Ov002_HandlePanelInput(0, -1);
            Ov002_Panel_MoveCursor(0);
        }
        return;
    case 5:
        if (Ov002_CountSecondListEntries() == 0) {
            Ov002_HandlePanelInput(0, -1);
            Ov002_Panel_MoveCursor(0);
            return;
        }
        self[0] = (char)(kh_rt_s32_divmod(*(unsigned char *)(self + 4), 6) >> 32);
        Ov002_HandlePanelInput((int)kh_rt_s32_divmod(*(unsigned char *)(self + 4), 6) + 6, -1);
        return;
    }
}

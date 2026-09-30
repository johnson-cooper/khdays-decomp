/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_RemoveEntryAndReopen.c (ps2/tools/prep_sources.py). Do not edit. */
/* Drop the entry (kind, value) from the list and reopen whatever the cursor now points at.
 *
 * The removal is done by Ov002_PanelRemoveSubEntry, which takes both halves of the entry key straight from
 * this function's own parameters -- they are never touched here, which is why the pointer load
 * ahead of the call cannot use r0 or r1. After the list shrinks, a cursor sitting past the new
 * end is pulled back to the last row, and if the entry under the cursor classifies as kind 3 it
 * is announced with its column (six to a row) and opened.
 *
 * The remainder has to come from kh_rt_s32_divmod by hand: writing `%%` emits _s32_div_f, which is
 * not linkable here. Ghidra carries the field names on Ov002PanelSession. */

typedef struct {
    unsigned char bKind;        /* +0 */
    unsigned char bMode;        /* +1 */
    char pad0002[2];
    unsigned char bKey;         /* +4 */
} Ov002PanelSession;

extern long long kh_rt_s32_divmod(int numerator, int denominator);
extern Ov002PanelSession *data_ov002_0207f620;
extern int Ov002_PanelRemoveSubEntry(unsigned int kind, unsigned int value);
extern void Ov002_PanelRefreshCurrentMode(void);
extern int Ov002_CountSecondListEntries(void);
extern int Ov002_ClassifyCode(int *out, int mode);
extern void Ov002_PanelApplyCursorMove(int kind, int column);
extern void Ov002_PanelRepaintSubListGroup(int entry);

void Ov002_RemoveEntryAndReopen(unsigned int kind, unsigned int value) {
    Ov002PanelSession *session = data_ov002_0207f620;
    int out;
    int count;

    Ov002_PanelRemoveSubEntry(kind, value);
    Ov002_PanelRefreshCurrentMode();
    count = Ov002_CountSecondListEntries();
    if (count >= 1 && session->bKey >= count) {
        session->bKey = (unsigned char)(count - 1);
    }
    if (Ov002_ClassifyCode(&out, session->bMode) == 3) {
        Ov002_PanelApplyCursorMove(session->bKind, (int)(kh_rt_s32_divmod(session->bKey, 6) >> 32));
        Ov002_PanelRepaintSubListGroup(out);
    }
}

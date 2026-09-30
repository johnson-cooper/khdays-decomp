/* PS2: mechanically prepared copy of src/engine/PauseMenu_ConfirmFrame.c (ps2/tools/prep_sources.py). Do not edit. */
#pragma thumb on
/* PauseMenu_ConfirmFrame -- pause menu confirmation page (yes / no), MAIN (THUMB). B or an inactive menu
 * (PauseMenu_GetMode) resets the page cursor (data_02042730, "no" by default) to 1, returns to the
 * pause menu (PauseMenu_Frame) and closes. Otherwise the page cursor moves (TabPanel_HandleUpDown); A (or
 * key bit 1, which forces "no") acts on it: "yes" (0) confirms the pause menu entry (+0xd4) --
 * entry 1 just closes, entry 2 calls into the overlay (Ov002_PauseMissionScene), closes and sets game
 * field 0x20ef --; "no" (1) restores the pause menu layout (3 entries when LoadGlobalU16At0 bit 1 is
 * set, else 2), reselects its entry, redraws the panels and returns to PauseMenu_Frame. Every other
 * frame refreshes the panels for the page cursor (Input_DebounceSlot) and ends with ForwardQuery64Result. The
 * cursor is defined here (the ROM keeps two pool words for it). */
typedef struct {
    int debounce;                       /* +0x00 */
    int selected;                       /* +0x04 */
} PanelSlot;

typedef struct {
    char pad0000[0xc];
    char panel[0xa0];                   /* +0x0c -- opaque, passed to SubObject_NudgeAndRedraw */
    PanelSlot slots[3];                 /* +0xac */
    int field_c4;
    int timer;                          /* +0xc8 */
    int field_cc;
    int count;                          /* +0xd0 */
    int cursor;                         /* +0xd4 */
} TabContext;

typedef struct {
    char pad0000[4];
    TabContext *pCtx;                   /* +0x04 */
} Root0204be08;

extern Root0204be08 data_0204be08;
extern unsigned short gPadPressed;    /* keys pressed this frame */
/* khdays: shared-bss */
extern int data_02042730;   /* PS2: defined in the data/ source (prep R11) */                  /* two-tab page cursor */

extern int PauseMenu_GetMode(void);
extern void PlaySound(int a, int b);            /* play menu sound */
extern void Callbacks_ClearByteAndRun2(void);                    /* close the menu */
extern void TabPanel_HandleUpDown(int *pIndex);
extern int LoadGlobalU16At0(void);
extern void setDualArrayEntry(int a, void (*step)(void), int b);
extern void SubObject_SetupDrawsForMode(void);
extern void MI_CpuFill8(void *dest, int data, unsigned int size);
extern void SubObject_NudgeAndRedraw(void *panel, int index, int state);
extern void ForwardQuery64Result(void);
extern void Input_DebounceSlot(int cursor);
extern void Ov002_PauseMissionScene(int a);
extern void GameState_SetField(int flag, int a, int b);  /* GameState_SetFlag */
extern void PauseMenu_Frame(void);

void PauseMenu_ConfirmFrame(void)
{
    TabContext *ctx = data_0204be08.pCtx;
    int i;

    if ((gPadPressed & 8) || PauseMenu_GetMode() == 0) {
        data_02042730 = 1;
        PlaySound(0, 3);
        setDualArrayEntry(1, PauseMenu_Frame, 0);
        Callbacks_ClearByteAndRun2();
        return;
    }
    TabPanel_HandleUpDown(&data_02042730);
    if ((gPadPressed & 1) || (gPadPressed & 2)) {
        if (gPadPressed & 2) {
            data_02042730 = 1;
        }
        switch (data_02042730) {
        case 0:
            PlaySound(0, 1);
            data_02042730 = 1;
            switch (ctx->cursor) {
            case 2:
                Ov002_PauseMissionScene(0);
                Callbacks_ClearByteAndRun2();
                GameState_SetField(0x20ef, 1, 0);
                return;
            case 1:
                Callbacks_ClearByteAndRun2();
                return;
            }
            break;
        case 1:
            PlaySound(0, 3);
            ctx->count = (LoadGlobalU16At0() & 2) ? 3 : 2;
            SubObject_SetupDrawsForMode();
            MI_CpuFill8(ctx->slots, 0, sizeof(ctx->slots));
            ctx->slots[ctx->cursor].selected = 1;
            for (i = 0; i < ctx->count; i++) {
                SubObject_NudgeAndRedraw(ctx->panel, i, ctx->slots[i].selected);
            }
            setDualArrayEntry(1, PauseMenu_Frame, 0);
            break;
        }
    }
    Input_DebounceSlot(data_02042730);
    ForwardQuery64Result();
}
#pragma thumb off

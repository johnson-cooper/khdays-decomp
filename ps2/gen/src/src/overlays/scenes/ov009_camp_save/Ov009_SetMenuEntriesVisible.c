/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/Ov009_SetMenuEntriesVisible.c (ps2/tools/prep_sources.py). Do not edit. */
/* Shows or hides the camp-menu status entries (four main, two secondary); when shown, draws the
 * counter and the play time (saved time plus the ticks since it was latched), otherwise runs the
 * closing callback of tag 2. */

#include "nitro/types.h"

typedef struct Ov009GameState {
    int value0;
    int pad004;
    int value8;
} Ov009GameState;

extern Ov009GameState *volatile gGameState;
extern const int data_ov009_02056010[4];
extern const int data_ov009_02056008[2];

extern int Ov009_GetContext(void);
extern int Ov009_GetCtxBlock9500(void);
extern int Ov009_FindEntryById(int manager, int id);
extern void Ov009_SetEntrySlotsVisible(int manager, int entry, int visible);
extern void Ov009_DrawNumberDigits(int value);
extern long long OS_GetTick(void);
extern long long Ov009_GetLatchedTick(void);
extern u64 kh_rt_ll_udiv_w(long long value, unsigned int divisor, int unused);
extern void Ov009_RenderTimeDigits(u32 value);
extern int Ov009_FindEntryByTag(int tracker, int tag);
extern void Ov009_TagTracker_InvokeCallback(int tracker, int entry);

void Ov009_SetMenuEntriesVisible(int visible, int secondaryVisible)
{
    int manager = Ov009_GetContext();
    int tracker = Ov009_GetCtxBlock9500();
    u8 i;

    for (i = 0; i < 4; i++) {
        int entry = Ov009_FindEntryById(
            manager, data_ov009_02056010[i]);
        Ov009_SetEntrySlotsVisible(manager, entry, visible);
    }

    for (i = 0; i < 2; i++) {
        int entry = Ov009_FindEntryById(
            manager, data_ov009_02056008[i]);
        Ov009_SetEntrySlotsVisible(manager, entry, secondaryVisible);
    }

    if (visible != 0) {
        long long elapsed;

        Ov009_DrawNumberDigits(gGameState->value8);
        elapsed = OS_GetTick() - Ov009_GetLatchedTick();
        Ov009_RenderTimeDigits(
            (u32)(gGameState->value0 +
                  kh_rt_ll_udiv_w(elapsed << 6, 0x1ff6210, 0)));
    } else {
        int entry = Ov009_FindEntryByTag(tracker, 2);
        Ov009_TagTracker_InvokeCallback(tracker, entry);
    }
}

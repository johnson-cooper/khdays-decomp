/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_SetMenuEntriesVisible.c (ps2/tools/prep_sources.py). Do not edit. */
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
extern const int data_ov008_0208f4f0[4];
extern const int data_ov008_0208f4e8[2];

extern int Ov008_GetContext(void);
extern int Ov008_GetCtxBlock9500(void);
extern int Ov008_FindEntryById(int manager, int id);
extern void Ov008_SetEntrySlotsVisible(int manager, int entry, int visible);
extern void Ov008_DrawNumberDigits(int value);
extern long long OS_GetTick(void);
extern long long Ov008_GetLatchedTick(void);
extern u64 kh_rt_ll_udiv_w(long long value, unsigned int divisor, int unused);
extern void Ov008_RenderTimeDigits(u32 value);
extern int Ov008_FindEntryByTag(int tracker, int tag);
extern void Ov008_TagTracker_InvokeCallback(int tracker, int entry);

void Ov008_SetMenuEntriesVisible(int visible, int secondaryVisible)
{
    int manager = Ov008_GetContext();
    int tracker = Ov008_GetCtxBlock9500();
    u8 i;

    for (i = 0; i < 4; i++) {
        int entry = Ov008_FindEntryById(
            manager, data_ov008_0208f4f0[i]);
        Ov008_SetEntrySlotsVisible(manager, entry, visible);
    }

    for (i = 0; i < 2; i++) {
        int entry = Ov008_FindEntryById(
            manager, data_ov008_0208f4e8[i]);
        Ov008_SetEntrySlotsVisible(manager, entry, secondaryVisible);
    }

    if (visible != 0) {
        long long elapsed;

        Ov008_DrawNumberDigits(gGameState->value8);
        elapsed = OS_GetTick() - Ov008_GetLatchedTick();
        Ov008_RenderTimeDigits(
            (u32)(gGameState->value0 +
                  kh_rt_ll_udiv_w(elapsed << 6, 0x1ff6210, 0)));
    } else {
        int entry = Ov008_FindEntryByTag(tracker, 2);
        Ov008_TagTracker_InvokeCallback(tracker, entry);
    }
}

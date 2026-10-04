/*
 * PS2 calendar parent-state override.
 *
 * The child calendar animation can reach its terminal state on hardware while
 * the protected scene object remains live.  Keep the original successful
 * completion behavior, but breadcrumb every irreversible step so hardware
 * testing can show exactly where a handoff stops.
 */

#include "nitro/types.h"
#include "game/engine.h"
#include "game/scene.h"
#include "platform/kh_platform.h"

typedef struct BootModeState {
    u8 flags;
    u8 state;
    u16 elapsed;
    u16 resetWord;
} BootModeState;

typedef struct Ov004SceneState {
    void *task;
    int selectedDay;
} Ov004SceneState;

extern Ov004SceneState *data_ov004_02051380;
extern BootModeState data_0204c240;

extern int Ov004_GetResult(void);
extern void Ov004_ResetPartyMemberAndLayout(int arg);
extern void kh_debug_mark(const char *stage, int a, int b);

int Ov004_StepMissionSelectScene(void)
{
    int result = Ov004_GetResult();

    if (result != 0) {
        int selectedDay = data_ov004_02051380 ? data_ov004_02051380->selectedDay : 0xff;

        kh_debug_mark("calendar parent: complete", result, selectedDay);

        GameState_ClearFlag(0x18ae);
        GameState_SetField(0, 9, (u16)selectedDay);

        data_0204c240.elapsed = selectedDay == 0x165 ? 0x2711 : 0x2710;
        data_0204c240.resetWord = 0;
        data_0204c240.state = 0;

        kh_debug_mark("calendar parent: reset buffers", selectedDay, 0);
        PartyState_ResetBuffers();

        kh_debug_mark("calendar parent: reset layout", selectedDay, 0);
        Ov004_ResetPartyMemberAndLayout(0);

        kh_debug_mark("calendar parent: request field", SCENE_FIELD, 0);
        Scene_RequestPending(SCENE_FIELD, 0);

        kh_debug_mark("calendar parent: dead", SCENE_FIELD, -2);
        return -2;
    }

    return 0;
}

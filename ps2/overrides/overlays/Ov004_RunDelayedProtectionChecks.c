/*
 * PS2 override for the calendar's delayed DS Protect checks and terminal handoff.
 *
 * ov004 normally loads ov028 here and runs DS Protect 1.10 cartridge/emulator
 * probes. Those probes directly drive Nintendo DS cartridge/MMIO hardware and
 * have no PS2 equivalent.
 *
 * Hardware showed two separate stalls:
 *   1) entering the DS Protect path itself;
 *   2) after bypassing it, the child calendar object reached completion but the
 *      protected scene object remained live at cur=5/pend=0.
 *
 * Once the original visual/timing gates are satisfied, perform the same game
 * state updates as Ov004_StepMissionSelectScene, request SCENE_FIELD, and mark
 * the protected calendar scene object dead.  The existing PS2 main-loop guard
 * then runs Scene_AdvanceToPending after the frame is presented.
 */

#include "nitro/types.h"
#include "game/scene.h"

typedef struct Ov004Ps2Context {
    unsigned char opaque0000[0xaf8];
    int transitionPhase;                 /* +0x0af8 */
    int opaque0afc;
    u64 lastTick;                        /* +0x0b00 */
    unsigned char opaque0b08[0x4a48];
    int transitionComplete;              /* +0x5550 */
    unsigned char opaque5554[0x30];
    int transitionState;                 /* +0x5584 */
} Ov004Ps2Context;

typedef struct Ov004SceneState {
    void *task;
    int selectedDay;
} Ov004SceneState;

typedef struct BootModeState {
    u8 flags;
    u8 state;
    u16 elapsed;
    u16 resetWord;
} BootModeState;

extern Ov004Ps2Context *data_ov004_02051384;
extern Ov004SceneState *data_ov004_02051380;
extern BootModeState data_0204c240;
extern char gSceneCtl[];

extern u64 OS_GetTick(void);
extern void kh_debug_mark(const char *stage, int a, int b);
extern void GameState_ClearFlag(int flag);
extern void GameState_SetField(int field, int width, int value);
extern void PartyState_ResetBuffers(void);
extern void Ov004_ResetPartyMemberAndLayout(int arg);
extern void Scene_RequestPending(int sceneId, int arg);

void Ov004_RunDelayedProtectionChecks(void)
{
    Ov004Ps2Context *context = data_ov004_02051384;
    int *scene = (int *)gSceneCtl;
    int *sceneObj;
    int selectedDay;
    u64 elapsed;

    if (context == 0)
        return;

    elapsed = OS_GetTick() - context->lastTick;
    if (elapsed <= 0x11942b)
        return;

    /* Ov004_StepLogoSlide writes 2 here when the visual transition is ready. */
    if (context->transitionState != 2)
        return;

    /* Never issue the handoff twice if the dispatcher has already picked it up. */
    if (scene[3] != 0)
        return;

    sceneObj = (int *)scene[0];
    if (sceneObj == 0 || scene[2] != SCENE_CALENDAR)
        return;

    selectedDay = data_ov004_02051380 ? data_ov004_02051380->selectedDay : 0xff;

    /*
     * Mirror Ov004_StepMissionSelectScene's successful-completion path.  Doing
     * it here avoids depending on the parent calendar callback running again
     * after the child reaches its terminal state on PS2 hardware.
     */
    kh_debug_mark("calendar: handoff begin", selectedDay, sceneObj[5]);

    GameState_ClearFlag(0x18ae);
    GameState_SetField(0, 9, (u16)selectedDay);

    data_0204c240.elapsed = selectedDay == 0x165 ? 0x2711 : 0x2710;
    data_0204c240.resetWord = 0;
    data_0204c240.state = 0;

    kh_debug_mark("calendar: reset party", selectedDay, 0);
    PartyState_ResetBuffers();
    Ov004_ResetPartyMemberAndLayout(0);

    context->lastTick = OS_GetTick();
    context->transitionPhase = 4;
    context->transitionComplete = 1;

    /*
     * Request the field first, then mark the protected scene object dead.
     * Obj_UpdateAll intentionally does not destroy flag-1 scene objects; the
     * PS2 frame-loop guard sees pend=2/state=-2 and calls the dispatcher safely
     * after presentation.
     */
    Scene_RequestPending(SCENE_FIELD, 0);
    sceneObj[5] = -2;

    kh_debug_mark("calendar: field requested", scene[3], sceneObj[5]);
}

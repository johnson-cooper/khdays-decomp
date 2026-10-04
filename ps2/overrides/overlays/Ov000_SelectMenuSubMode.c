/* PS2 override: make accepting New Game a bounded state change.
 *
 * The DS routine rebuilds the complete 0x2018-byte game-state work area synchronously from the
 * input callback.  The same initializer has already run during Ov000_InitSaveSystem, before the
 * title menu appears; running it again here is redundant except for the selected difficulty.
 * On real PS2 hardware this second rebuild is the last operation reached when Yes is pressed and
 * can leave the frame loop stopped before the parent sees result 5.  Preserve the initialized
 * defaults, refresh the New Game selector and difficulty fields, and publish result 5 immediately.
 */
#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov000SubSceneContext {
    u8 pad_0000[0x14];
    u32 tickLow;
    u32 tickHigh;
    u16 inputCode;
    u8 pad_001e[2];
    int selection;
    int baseMode;
    u8 pad_0028[0x4b9c];
    int transitionMode;
} Ov000SubSceneContext;

extern Ov000SubSceneContext *volatile data_ov000_0205ac28;
extern volatile u16 gPadPressed;
extern u64 OS_GetTick(void);
extern void Ov000_DispatchLogoAction(int selector, int argument);
extern void Ov000_ModeSelect_ShowGroup(int mode);
extern void Ov000_PlaceModeMarker(int mode);

void Ov000_SelectMenuSubMode(void)
{
    Ov000SubSceneContext *context = data_ov000_0205ac28;
    int previousBaseMode = context->baseMode;
    int mode = -1;

    if (context->inputCode == 0x10) {
        if (previousBaseMode != 1) {
            context->baseMode = 1;
            PlaySound(0, 0);
        }
    } else if (context->inputCode == 0x20) {
        if (previousBaseMode != 0) {
            context->baseMode = 0;
            PlaySound(0, 0);
        }
    } else {
        switch (gPadPressed) {
        case 1:
            if (previousBaseMode == 0) {
                mode = 5;
            } else {
                mode = 2;
                PlaySound(0, 3);
            }
            break;
        case 2:
            mode = 2;
            PlaySound(0, 3);
            break;
        default:
            break;
        }
    }

    if (mode == 2) {
        context->transitionMode = mode;
        Ov000_DispatchLogoAction(0, mode);
        Ov000_DispatchLogoAction(1, mode);
        Ov000_DispatchLogoAction(2, mode);
    } else if (mode == 5) {
        u64 tick = OS_GetTick();
        int difficulty = context->selection;

        if (difficulty < 0)
            difficulty = 0;
        if (difficulty > 3)
            difficulty = 3;

        context->tickLow = (u32)tick;
        context->tickHigh = (u32)(tick >> 32);

        /* SetupWorkArea already installed all New Game defaults at boot. */
        GameState_SetField(0, 9, 0x191);
        GameState_SetField(0x40a, 2, (u16)difficulty);

        /* Publish this last: the parent may consume it on its next update. */
        context->transitionMode = mode;
    }

    if (mode != -1) {
        Ov000_ModeSelect_ShowGroup(mode);
        Ov000_PlaceModeMarker(mode);
    }

    context = data_ov000_0205ac28;
    if (previousBaseMode != context->baseMode)
        Ov000_PlaceModeMarker(context->transitionMode);
}

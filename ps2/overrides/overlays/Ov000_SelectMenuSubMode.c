/* Menu sub-mode selector. Input code 0x10 sets baseMode to 1 and 0x20 sets it to 0 (each resetting
 * the fade via PlaySound only on an actual change). Any other input dispatches on the external code
 * (gPadPressed) into a mode (2/5/...), restarting the fade as needed. */

#include "nitro/types.h"
#include "game/engine.h"
#include "platform/kh_platform.h"

extern void kh_debug_stage(const char *stage, int a, int b);

typedef struct Ov000SubSceneContext {
    u8 pad_0000[0x14];
    u32 tickLow;
    u32 tickHigh;
    u16 inputCode;
    u8 pad_001e[2];
    int setupSlot;
    int baseMode;
    u8 pad_0028[0x4b9c];
    int transitionMode;
} Ov000SubSceneContext;

extern Ov000SubSceneContext *volatile data_ov000_0205ac28;
extern volatile u16 gPadPressed;
extern void Ov000_DispatchLogoAction(int selector, int argument);
extern u64 OS_GetTick(void);
extern void Ov000_SetupWorkArea(int slot);
extern void Ov000_ModeSelect_ShowGroup(int mode);
extern void Ov000_PlaceModeMarker(int mode);

void Ov000_SelectMenuSubMode(void) {
    int mode = -1;
    Ov000SubSceneContext *context = data_ov000_0205ac28;
    int previousBaseMode = context->baseMode;

    if (context->inputCode == 0x10) {
        goto input_10;
    }
    if (context->inputCode != 0x20) {
        goto input_other;
    }
    if (previousBaseMode != 0) {
        context->baseMode = 0;
        PlaySound(0, 0);
    }
    goto input_done;

input_10:
    if (previousBaseMode != 1) {
        context->baseMode = 1;
        PlaySound(0, 0);
    }
    goto input_done;

input_other:
    {
        u16 externalCode = gPadPressed;

    switch (externalCode) {
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
    }
    }

input_done:
    switch (mode) {
    case 2:
        kh_debug_stage("mode confirm: transition mode 2", previousBaseMode, mode);
        data_ov000_0205ac28->transitionMode = mode;
        Ov000_DispatchLogoAction(
            0, data_ov000_0205ac28->transitionMode);
        Ov000_DispatchLogoAction(
            1, data_ov000_0205ac28->transitionMode);
        Ov000_DispatchLogoAction(
            2, data_ov000_0205ac28->transitionMode);
        break;
    case 5:
    {
        u64 tick;
        kh_debug_stage("mode confirm: accepted/start", previousBaseMode, mode);

        context = data_ov000_0205ac28;
        tick = OS_GetTick();
        context->tickLow = (u32)tick;
        context->tickHigh = (u32)(tick >> 32);
        context->transitionMode = mode;
        Ov000_SetupWorkArea(data_ov000_0205ac28->setupSlot);
    }
        break;
    }

    if (mode != -1) {
        Ov000_ModeSelect_ShowGroup(mode);
        Ov000_PlaceModeMarker(mode);
    }

    {
        Ov000SubSceneContext *finalContext =
            data_ov000_0205ac28;

        if (previousBaseMode != finalContext->baseMode) {
            Ov000_PlaceModeMarker(finalContext->transitionMode);
        }
    }
}

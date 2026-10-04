/* Mode-select navigator: D-pad up (0x40) / down (0x80) moves the selection with wraparound over
 * three entries, restarts the fade (PlaySound) and refreshes the mode slots via
 * Ov000_DispatchLogoAction. */

#include "nitro/types.h"
#include "game/engine.h"
#include "platform/kh_platform.h"

extern void kh_debug_stage(const char *stage, int a, int b);

typedef struct Ov000ModeSelectContext {
    u8 pad_0000[0x14];
    u32 actionTickLow;
    u32 actionTickHigh;
    u16 inputMask;
    u8 pad_001e[2];
    int selection;
    int actionAccepted;
    u8 pad_0028[0x4b9c];
    int activeMode;
    int pendingMode;
} Ov000ModeSelectContext;

extern Ov000ModeSelectContext *data_ov000_0205ac28;
extern u16 gPadPressed;

extern void Ov000_DispatchLogoAction(int marker, int mode);
extern u64 OS_GetTick(void);
extern void Ov000_ModeSelect_ShowGroup(int mode);
extern void Ov000_PlaceModeMarker(int mode);

void Ov000_NavigateModeSelect(void) {
    Ov000ModeSelectContext *context = data_ov000_0205ac28;
    int action = -1;
    int oldSelection = context->selection;

    switch (context->inputMask) {
    case 0x40:
        context->selection = oldSelection - 1;
        if (data_ov000_0205ac28->selection < 0) {
            data_ov000_0205ac28->selection = 2;
        }
        PlaySound(0, 0);
        Ov000_DispatchLogoAction(2, data_ov000_0205ac28->activeMode);
        Ov000_DispatchLogoAction(1, data_ov000_0205ac28->activeMode);
        break;
    case 0x80:
        context->selection = oldSelection + 1;
        if (data_ov000_0205ac28->selection > 2) {
            data_ov000_0205ac28->selection = 0;
        }
        PlaySound(0, 0);
        Ov000_DispatchLogoAction(2, data_ov000_0205ac28->activeMode);
        Ov000_DispatchLogoAction(1, data_ov000_0205ac28->activeMode);
        break;
    default:
        switch (gPadPressed) {
        case 1:
            action = 3;
            PlaySound(0, 1);
            break;
        case 2: {
            u64 tick = OS_GetTick();
            Ov000ModeSelectContext *current;

            action = 4;
            current = data_ov000_0205ac28;
            current->actionTickLow = (u32)tick;
            current->actionTickHigh = (u32)(tick >> 32);
            PlaySound(0, 3);
            break;
        }
        default:
            break;
        }
        break;
    }

    switch (action) {
    case 3:
        kh_debug_stage("mode select: confirm difficulty", data_ov000_0205ac28->selection, action);
        data_ov000_0205ac28->actionAccepted = 1;
        data_ov000_0205ac28->activeMode = action;
        Ov000_DispatchLogoAction(0, data_ov000_0205ac28->activeMode);
        Ov000_DispatchLogoAction(1, data_ov000_0205ac28->activeMode);
        Ov000_DispatchLogoAction(2, data_ov000_0205ac28->activeMode);
        break;
    case 4: {
        Ov000ModeSelectContext *current = data_ov000_0205ac28;
        kh_debug_stage("mode select: cancel/back", current->selection, action);
        u64 tick = OS_GetTick();

        current->actionTickLow = (u32)tick;
        current->actionTickHigh = (u32)(tick >> 32);
        current->activeMode = 0;
        data_ov000_0205ac28->pendingMode = action;
        break;
    }
    default:
        break;
    }

    if (action != -1) {
        Ov000_ModeSelect_ShowGroup(action);
        Ov000_PlaceModeMarker(action);
    }

    if (oldSelection != data_ov000_0205ac28->selection) {
        Ov000_PlaceModeMarker(data_ov000_0205ac28->activeMode);
    }
}

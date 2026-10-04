/* PS2 debug override: trace the selector that requests the opening scene. */
#include "nitro/types.h"
#include "game/engine.h"
#include "game/scene.h"
#include "platform/kh_platform.h"

typedef u32 FSOverlayID;
extern u32 OVERLAY_28_ID[1];
#define FS_OVERLAY_ID_ov028 ((FSOverlayID)(u32)&OVERLAY_28_ID)

typedef struct Ov000DisplayConfig {
    int enabled;
    int visible;
    int reserved0;
    int reserved1;
} Ov000DisplayConfig;

typedef struct Ov000ModeConfig {
    int enabledMode;
    int reserved;
} Ov000ModeConfig;

typedef struct BootModeState {
    u8 flags;
    u8 state;
    u16 elapsed;
    u16 resetWord;
} BootModeState;

typedef struct Ov000BootContext {
    u8 pad_0000[0x4c40];
    int bootRequestPending;
} Ov000BootContext;

extern void kh_debug_stage(const char *stage, int a, int b);
extern Ov000BootContext *NNSi_FndGetCurrentRootHeap(void);
extern void Ov000_ResetPartyMemberAndLayout(int a, int b);
extern int func_ov028_0208b490(int a);
extern int func_ov028_0208b120(int a);
extern int func_ov028_0208b2e0(int a);
extern void Ov000_RequestScene11(void);
extern BootModeState data_0204c240;

int Ov000_BootRunSelector(void)
{
    Ov000BootContext *ctx = NNSi_FndGetCurrentRootHeap();
    Ov000ModeConfig mode;
    Ov000DisplayConfig display;
    int selector = 0;
    int p0, p1, p2;

    kh_debug_stage("boot selector: entered", ctx->bootRequestPending, 0);

    display.enabled = 1;
    display.visible = 1;
    Session_StoreSetup(&display);
    mode.enabledMode = 1;
    mode.reserved = 0;
    CopyToSlotTable8(&mode, 0);
    EnsureServiceInstance();

    kh_debug_stage("boot selector: reset party/layout", 0, 0);
    Ov000_ResetPartyMemberAndLayout(0, 0);
    kh_debug_stage("boot selector: reset returned", 0, 0);

    if (ctx->bootRequestPending != 0)
        selector = GameState_GetField(0, 9);

    kh_debug_stage("boot selector: selector value", selector, ctx->bootRequestPending);

    if (selector == 0x191) {
        data_0204c240.resetWord = 0;
        data_0204c240.state = 0;

        kh_debug_stage("boot selector: load ov028", 28, 0);
        LoadOverlaySync(0, FS_OVERLAY_ID_ov028);
        kh_debug_stage("boot selector: ov028 loaded", 28, 0);

        p0 = func_ov028_0208b490(0);
        kh_debug_stage("boot selector: protect check 1", p0, 0);
        p1 = func_ov028_0208b120(0);
        kh_debug_stage("boot selector: protect check 2", p1, 0);
        p2 = func_ov028_0208b2e0(0);
        kh_debug_stage("boot selector: protect check 3", p2, 0);

        if (p0 != 0 && p1 != 0 && p2 != 0) {
            data_0204c240.elapsed = 0x2710;
            kh_debug_stage("boot selector: request opening", SCENE_OPENING, 0);
            Ov000_RequestScene11();
            kh_debug_stage("boot selector: opening requested", SCENE_OPENING, 0);
        }

        kh_debug_stage("boot selector: unload ov028", 28, 0);
        UnloadOverlaySync(0, FS_OVERLAY_ID_ov028);
        kh_debug_stage("boot selector: ov028 unloaded", 28, 0);
    } else {
        kh_debug_stage("boot selector: request calendar", SCENE_CALENDAR, selector);
        Scene_RequestPending(SCENE_CALENDAR, selector);
        kh_debug_stage("boot selector: calendar requested", SCENE_CALENDAR, selector);
    }

    kh_debug_stage("boot selector: complete", selector, 0);
    return -2;
}

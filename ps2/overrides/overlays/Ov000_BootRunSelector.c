/* PS2 override: finish New Game without running Nintendo DS cartridge protection.
 *
 * The original selector loads ov028 and only requests the opening scene when three DS Protect
 * cartridge predicates succeed.  That overlay probes DS ROM mirroring and the DS wireless MAC;
 * it has no meaningful implementation on a PS2.  Its failure path deliberately requests no
 * scene at all, leaving the title task alive on its final loading frame forever.  Preserve the
 * game/session setup and selector semantics, but treat the PS2 build as the supported platform
 * and enter the opening movie directly.
 */
#include "nitro/types.h"
#include "game/engine.h"
#include "game/scene.h"

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

extern Ov000BootContext *NNSi_FndGetCurrentRootHeap(void);
extern void Ov000_ResetPartyMemberAndLayout(int a, int b);
extern void Ov000_RequestScene11(void);
extern BootModeState data_0204c240;

int Ov000_BootRunSelector(void)
{
    Ov000BootContext *ctx = NNSi_FndGetCurrentRootHeap();
    Ov000ModeConfig mode;
    Ov000DisplayConfig display;
    int selector = 0;

    display.enabled = 1;
    display.visible = 1;
    Session_StoreSetup(&display);
    mode.enabledMode = 1;
    mode.reserved = 0;
    CopyToSlotTable8(&mode, 0);
    EnsureServiceInstance();
    Ov000_ResetPartyMemberAndLayout(0, 0);

    if (ctx->bootRequestPending != 0)
        selector = GameState_GetField(0, 9);

    if (selector == 0x191) {
        data_0204c240.resetWord = 0;
        data_0204c240.state = 0;
        data_0204c240.elapsed = 0x2710;
        Ov000_RequestScene11();
    } else {
        Scene_RequestPending(SCENE_CALENDAR, selector);
    }
    return -2;
}

/* PS2 override: retain the title fade without ever blocking on streamed-audio teardown.
 *
 * The stock transition waits for stream slot 1 to disappear.  On DS the stop tears the player
 * down synchronously; on PS2 a physical USB/MMCE refill can own the stream mutex for much longer.
 * Use the PS2 non-blocking stop helper instead.  A completed fade invalidates the public handle
 * immediately, and frame 0x40 is a hard logical-stop fallback, so this state cannot wait forever.
 */
#include "nitro/types.h"
#include "game/engine.h"
#include "platform/ps2/decomp_prefix.h"

typedef void (*Ov000StateFn)(void);

typedef struct Ov000BootContext {
    int frame;
    u8 pad_0004[8];
    u8 renderNode[0x108];
    u8 scrollBounds[0x4b19];
    s8 transitionMode;
    u8 pad_4c2e[0x43e];
    void *sharingHandle;
    void *sharingAux;
} Ov000BootContext;

extern Ov000BootContext *NNSi_FndGetCurrentRootHeap(void);
extern void Ov000_FadeStateHookNoOp(void);
extern void Camera_CommitMatricesEx(void *bounds, int right, int left, int top, int bottom);
extern void Scene_DrawNode(void *renderNode);
extern void G2x_SetBlendBrightness_(unsigned short *dst, unsigned int attr, int value);
extern void kh_ps2_snd_stop_slot_nonblocking(int slot, int frames);
extern void func_02023ad0(void *handle);
extern void Ov000_BootDispatch(void);
extern volatile const char *kh_watchdog_mark;

Ov000StateFn Ov000_TickBootFadeTransition(void)
{
    Ov000BootContext *context = NNSi_FndGetCurrentRootHeap();

    Ov000_FadeStateHookNoOp();
    Camera_CommitMatricesEx(context->scrollBounds, 0x3b33, -0x3b33, -0x4d9a, 0x4d9a);
    Scene_DrawNode(context->renderNode);

    if (context->frame <= 0x20) {
        kh_watchdog_mark = "newgame: brightness fade";
        SetMasterBrightnessMain(context->frame / 2);
        SetMasterBrightnessSub(context->frame / 2);
    } else if (context->frame < 0x28) {
        kh_watchdog_mark = "newgame: sub display switch";
        /*
         * This is a PS2 override, so prep_sources.py never rewrites literal DS MMIO
         * addresses for us.  Touch the PS2-side DS display shadow instead of the real
         * EE address 0x04001000, which is unmapped on hardware and bus-errors here.
         */
        volatile u32 *sub_dispcnt = (volatile u32 *)(kh_ds_io + 0x1000);
        *sub_dispcnt = (*sub_dispcnt & ~0x1f00u) | 0x1200u;
    } else if (context->frame < 0x30) {
        kh_watchdog_mark = "newgame: blend fade";
        G2x_SetBlendBrightness_((unsigned short *)(kh_ds_io + 0x50), 3, 0x10);
        G2x_SetBlendBrightness_((unsigned short *)(kh_ds_io + 0x1050), 0x12, 0x10);
        SetMasterBrightnessMain((-(context->frame - 0x28)) << 1);
        SetMasterBrightnessSub((-(context->frame - 0x28)) << 1);
    } else {
        *(volatile u16 *)(kh_ds_io + 0x50) = 0;
        *(volatile u16 *)(kh_ds_io + 0x1050) = 0;
        SetMasterBrightnessMain(-0x10);
        SetMasterBrightnessSub(-0x10);

        if (context->frame == 0x30) {
            kh_watchdog_mark = "newgame: fade slot1";
            kh_ps2_snd_stop_slot_nonblocking(1, 0x10);
        } else if (context->frame == 0x40 && SoundStrm_HasPlaybackPos(1) != 0) {
            kh_watchdog_mark = "newgame: slot1 fallback";
            kh_ps2_snd_stop_slot_nonblocking(1, 0);
        }

        if (SoundStrm_HasPlaybackPos(1) == 0) {
            context->frame = 0;
            switch (context->transitionMode) {
            case 0:
                if (context->sharingHandle != 0) {
                    func_02023ad0(context->sharingHandle);
                    context->sharingHandle = 0;
                }
                break;
            case 1:
            case 2:
                if (context->sharingHandle != 0) {
                    func_02023ad0(context->sharingHandle);
                    context->sharingHandle = 0;
                }
                if (context->sharingAux != 0) {
                    func_02023ad0(context->sharingAux);
                    context->sharingAux = 0;
                }
                break;
            }
            kh_watchdog_mark = "newgame: boot dispatch";
            return Ov000_BootDispatch;
        }
    }

    context->frame++;
    return 0;
}

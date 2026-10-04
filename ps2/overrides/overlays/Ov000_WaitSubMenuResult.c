/* PS2 override: consume New Game confirmation before touching another title-frame draw.
 *
 * The DS path starts/stops streamed audio synchronously in the frame that consumes result 5.
 * On PS2 physical storage that can wait behind a refill mutex, leaving the difficulty screen
 * visibly frozen even though the user's Yes input was accepted.
 *
 * Read result 5 first, publish the title transition immediately, and request the title stream
 * fade through the PS2 non-blocking stream-stop helper.  The helper never sleeps on USB/MMCE I/O.
 */
#include "nitro/types.h"

typedef void (*OverlayCallback)(void);

typedef struct {
    int state_0;
    u8 pad_0004[8];
    u8 update_object[0x4bdc];
    u8 overlay_image[0x58];
    int transition_flag;
    u8 pad_4c44[0x42c];
    int sharing_handle;
    int sharing_state;
} OverlayContext;

extern OverlayContext *NNSi_FndGetCurrentRootHeap(void);
extern void KeyRepeat_Step(void *image);
extern unsigned short Mem_ReadU16(void *image);
extern void Ov000_SetSubSceneHalf1C(int value);
extern void Scene_DrawNode(void *object);
extern int Ov000_GetSubSceneResult(void);
extern void func_02023ad0(int handle);
extern void Ov000_PreloadLogoResources(void);
extern void Ov000_Title_CreateLogoObjects(void);
extern void Ov000_ReleaseLogoResources(void);
extern void kh_ps2_snd_stop_slot_nonblocking(int slot, int frames);
extern void Ov000_ReentryState(void);
extern void Ov000_TickBootFadeTransition(void);

OverlayCallback Ov000_WaitSubMenuResult(void)
{
    OverlayContext *context = NNSi_FndGetCurrentRootHeap();
    int result = Ov000_GetSubSceneResult();

    /* Once Yes has published result 5, do not run one more menu update/draw before leaving. */
    if (result == 5) {
        context->state_0 = 0;
        context->transition_flag = 1;
        kh_ps2_snd_stop_slot_nonblocking(0, 30);
        return Ov000_TickBootFadeTransition;
    }

    KeyRepeat_Step(context->overlay_image);
    Ov000_SetSubSceneHalf1C(Mem_ReadU16(context->overlay_image));
    Scene_DrawNode(context->update_object);

    if (result == 4) {
        func_02023ad0(context->sharing_handle);
        context->sharing_handle = context->sharing_state = 0;
        Ov000_PreloadLogoResources();
        Ov000_Title_CreateLogoObjects();
        Ov000_ReleaseLogoResources();
        return Ov000_ReentryState;
    }

    return 0;
}

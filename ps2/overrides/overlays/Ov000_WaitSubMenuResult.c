/* PS2 override: begin the New Game fade without synchronously opening a bridge stream.
 *
 * Stream 3 is a short title-to-opening transition cue.  Starting it is optional to scene state,
 * but NitroSystem opens and prepares it synchronously in the frame that consumes result 5.  A
 * physical-device refill can therefore stop the entire frame loop before any fade is visible.
 * Keep fading the title stream and enter the normal transition state; the opening scene starts
 * and owns its audio through the regular scene path.
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
extern void Table_TailCallWithEntry(int slot, int frames);
extern void Ov000_ReentryState(void);
extern void Ov000_TickBootFadeTransition(void);

OverlayCallback Ov000_WaitSubMenuResult(void)
{
    OverlayContext *context = NNSi_FndGetCurrentRootHeap();

    KeyRepeat_Step(context->overlay_image);
    Ov000_SetSubSceneHalf1C(Mem_ReadU16(context->overlay_image));
    Scene_DrawNode(context->update_object);

    switch (Ov000_GetSubSceneResult()) {
    case 4:
        func_02023ad0(context->sharing_handle);
        context->sharing_handle = context->sharing_state = 0;
        Ov000_PreloadLogoResources();
        Ov000_Title_CreateLogoObjects();
        Ov000_ReleaseLogoResources();
        return Ov000_ReentryState;
    case 5:
        context->state_0 = 0;
        context->transition_flag = 1;

        /* Do not synchronously start the optional stream-3 bridge on physical hardware. */
        Table_TailCallWithEntry(0, 30);
        return Ov000_TickBootFadeTransition;
    default:
        return 0;
    }
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_TickTitleFadeOut.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov011_TickTitleFadeOut -- Ov011_TickTitleFadeOut (MATCHED, 280 B, 9 relocs).
 *
 * Title fade-out per-frame tick (a state-table entry). Interpolates a blend alpha from
 * the global frame timer between the scene's fade start/end times (pScene+0x2cf4c /
 * +0x2cf4e, kh_rt_u32_divmod, clamped to 0x10), then drives BOTH engines' master brightness
 * to -alpha (SetMasterBrightnessMain / SetMasterBrightnessSub) to fade the title out. Once the timer passes
 * the fade end and SoundStrm_HasPlaybackPos(0) is clear, it (optionally) fires VBlank_UnregisterCallback when the
 * mode field is 1 and sets the next state (pScene+4) to 5 or 6 depending on pScene+0x23ac4.
 * Finally it dispatches the current sub-state via data_ov011_0205e8cc[pScene->mode](), and
 * when the mode is 3 forwards pScene+0x28508 to the scene-transition helper DispObjList_UpdateImmediate.
 *
 * Match idiom (mirrors the tick cb18, opposite of the fade tick c884): access
 * data_ov011_0205e960 BY NAME every time (the ROM reloads the globals pointer on each use;
 * caching pScene/timer in locals promotes them to callee-saved regs and inflates the frame).
 * The fade-complete threshold is written timer-first (`nTimer >= end`) to get the ROM's
 * `cmp timer,end; blo` rather than the reversed `cmp end,timer; bhi`.
 */

#include "nitro/types.h"

typedef void (*Ov011StateFn)(void);

typedef struct Ov011FadePane { u16 startTime; u16 endTime; } Ov011FadePane;
typedef struct Ov011Globals { u32 nTimer; u8 *pScene; } Ov011Globals;

extern Ov011Globals data_ov011_0205e960;
extern Ov011StateFn data_ov011_0205e8cc[];
extern int  gOv011SfVName;

extern long long kh_rt_u32_divmod(int a, int b);
extern void SetMasterBrightnessMain(int brightness);
extern void SetMasterBrightnessSub(int brightness);
extern int  SoundStrm_HasPlaybackPos(int a);
extern void VBlank_UnregisterCallback(int a, void *b);
extern void DispObjList_UpdateImmediate(void *a);

void Ov011_TickTitleFadeOut(void)
{
    Ov011FadePane *fade;
    u16 end;
    int alpha, neg;

    fade = (Ov011FadePane *)(data_ov011_0205e960.pScene + 0x2cf4c);
    end = fade->endTime;
    if (end <= data_ov011_0205e960.nTimer) {
        alpha = 0x10;
    } else {
        u16 start = fade->startTime;
        alpha = (u16)kh_rt_u32_divmod((int)(data_ov011_0205e960.nTimer - start) * 0x10, end - start);
    }
    neg = -(int)(short)alpha;
    SetMasterBrightnessMain(neg);
    SetMasterBrightnessSub(neg);

    if (data_ov011_0205e960.nTimer >= *(u16 *)(data_ov011_0205e960.pScene + 0x2cf4e) &&
        SoundStrm_HasPlaybackPos(0) == 0) {
        if (*(int *)(data_ov011_0205e960.pScene + 8) == 1) {
            VBlank_UnregisterCallback(1, &gOv011SfVName);
        }
        *(int *)(data_ov011_0205e960.pScene + 4) =
            (*(int *)(data_ov011_0205e960.pScene + 0x23ac4) == 1) ? 5 : 6;
    }

    data_ov011_0205e8cc[*(int *)(data_ov011_0205e960.pScene + 8)]();

    if (*(int *)(data_ov011_0205e960.pScene + 8) == 3) {
        DispObjList_UpdateImmediate(data_ov011_0205e960.pScene + 0x28508);
    }
}

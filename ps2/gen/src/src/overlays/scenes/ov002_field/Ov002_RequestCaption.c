/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_RequestCaption.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_RequestCaption - ask the caption screen to show something.
 *
 * A take that arrives while the screen is already showing that same kind is
 * just appended to the running sequence and the caller is told it was taken.
 *
 * Otherwise the request is refused unless the screen is idle and free. When it
 * is accepted the sub-screen brightness is set - dimmed while no voice is
 * playing - the request is recorded (a fresh sequence for the take kind, the
 * plain argument for everything else), the fade tween is started if a voice is
 * running, and the screen moves to state 1 with the two tag trackers disarmed.
 *
 * THUMB.
 */

#include "nitro/types.h"

typedef struct {
    int nMode;
    int nDuration;
    int nFrom;
    int nTo;
    int aStart[2];
    unsigned int dwFlags;
} Ov002Tween;

typedef struct {
    int hVoice;
    char pad004[4];
    int nState;
    int nLine;
    char pad010[0x48];
    Ov002Tween tweenFade;
    char pad074[0xc];
    int nVoiceArg;
    char pad084[0x150];
    signed char aTake[8];
    u16 nTakeIndex;
    u16 nTakeCount;
} Ov002TextScene;

extern int data_ov002_0207f62c;

extern void G2x_SetBlendBrightness_(int nReg, int nPlane, int nLevel);
extern void Tween_Configure(Ov002Tween *pTween, int nMode, int nFrom, int nTo,
                          int nDuration);
extern void Tween_Start(Ov002Tween *pTween);

extern int Ov002_Hud_IsPanelOpen(void);
extern int Ov002_GetPanelField01b0(void);
extern void Ov002_ForwardToSubDc_5(int a);
extern int Ov002_Ctx_FindActiveEntryByTag(int nTag);
extern void Ov002_Ctx_SetTagTrackerNodeArmed_5(int nEntry, int nValue);
extern void Ov002_PlayCaptionCues(int a, int b);

int Ov002_RequestCaption(int nMode, int nTake)
{
    int nLevel;
    Ov002TextScene *s;

    s = *(Ov002TextScene **)((char *)&data_ov002_0207f62c + 4);
    if (s->nLine == 2 && nMode == 2 && nTake != 0) {
        s->aTake[s->nTakeCount] = (signed char)nTake;
        s->nTakeCount++;
        return 1;
    }

    if (s->nState != 0 || Ov002_Hud_IsPanelOpen() != 0) {
        return 0;
    }

    if (nMode == 2 || s->nLine == 2) {
        if (Ov002_GetPanelField01b0() == 0) {
            if (s->hVoice == 0) {
                nLevel = -0x10;
            } else {
                nLevel = 0;
            }
            G2x_SetBlendBrightness_(((unsigned int)kh_ds_io + 0x1050), 0x2f, nLevel);
        }
        if (nMode == 2) {
            if (nTake != 0) {
                s->aTake[0] = (signed char)nTake;
                s->nTakeCount = 1;
                s->nTakeIndex = 0;
            }
        } else {
            s->nVoiceArg = nTake;
            s->nTakeCount = 0;
            s->nTakeIndex = 0;
        }
    } else {
        if (Ov002_GetPanelField01b0() == 0) {
            if (s->hVoice == 0) {
                nLevel = -0x10;
            } else {
                nLevel = 0;
            }
            G2x_SetBlendBrightness_(((unsigned int)kh_ds_io + 0x1050), 0x2c, nLevel);
        }
        s->nVoiceArg = nTake;
    }

    if (s->hVoice != 0) {
        Tween_Configure(&s->tweenFade, 0, 0, 0xffff0000, 0x32);
        Tween_Start(&s->tweenFade);
    }
    s->nState = 1;
    s->nLine = nMode;

    Ov002_ForwardToSubDc_5(0);
    Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(6), 0);
    Ov002_Ctx_SetTagTrackerNodeArmed_5(Ov002_Ctx_FindActiveEntryByTag(4), 0);
    Ov002_PlayCaptionCues(0, 1);
    return 1;
}

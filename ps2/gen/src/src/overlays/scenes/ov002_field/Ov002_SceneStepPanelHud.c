/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SceneStepPanelHud.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_SceneStepPanelHud - refresh the two numbers the panel's HUD shows.
 *
 * The first is a count that is re-read every frame while it is wanted: it is
 * drawn, and a chime plays whenever it changes into the range 1..10, which is
 * where it starts to matter.
 *
 * The second is a countdown, drawn in whole seconds - rounded up, at whatever
 * frame rate the machine is set to - and then run down by however many frames
 * have passed since the last call, stopping at zero.
 *
 * ARM.
 */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct {
    char pad000[0xd8];
    int bHudDirty;
    int bHudHeld;
    char pad0e0[0x1240];
    int nTicksLeft;
    int nFrameStamp;
    u16 wLastCount;
} Ov002HudScene;

extern int data_ov002_0207f628;
extern int gObjSystem;

extern long long kh_rt_u32_divmod(int nNumer, int nDenom);

extern int Ov002_GetRootField8bc8(void);
extern void Ov002_DrawHudNumber(int nX, int nY, int nValue);

void Ov002_SceneStepPanelHud(void)
{
    Ov002HudScene *s;
    int nFps;
    unsigned int nCount;
    int nNow;
    unsigned int nDelta;

    s = *(Ov002HudScene **)&data_ov002_0207f628;
    nNow = Obj_GetFrameCount();
    nDelta = nNow - s->nFrameStamp;

    if (s->bHudDirty != 0) {
        nCount = (u16)Ov002_GetRootField8bc8();
        Ov002_DrawHudNumber(0, 0x4f000, nCount);
        if (nCount != 0 && nCount <= 10 && nCount != s->wLastCount) {
            s->wLastCount = (u16)nCount;
            PlaySoundChecked(0, 0x67);
        }
    }

    if (s->bHudHeld != 0) {
        switch (*(unsigned char *)&gObjSystem) {
        case 0:
            nFps = 0x1e;
            break;
        case 1:
            nFps = 0x14;
            break;
        case 2:
            nFps = 0x3c;
            break;
        }
        Ov002_DrawHudNumber(0, 0x50000,
                            (int)kh_rt_u32_divmod(s->nTicksLeft + nFps - 1, nFps));
        if (s->nTicksLeft > nDelta) {
            s->nTicksLeft -= nDelta;
        } else {
            s->nTicksLeft = 0;
        }
    }

    s->nFrameStamp = nNow;
}

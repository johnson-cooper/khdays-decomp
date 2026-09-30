/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetPageMarkTarget.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_SetPageMarkTarget - aim the mark at a new value and let it walk there.
 *
 * The target is where the value sits on the twenty-eight cell ruler. How long
 * each step takes falls with the distance still to go - twenty-five steps for a
 * standing start, two at the far end - and the step time is kept beside the
 * timestamp the walk starts from.
 *
 * When the caller asks for it and the mark really has ground to make up, the
 * walk is started and the mark is flagged as moving.
 *
 * ARM.
 */

#include "game/engine.h"

typedef struct {
    int nTotal;
} Ov002PageProgress;

typedef struct {
    int nCurrent;
    int nTarget;
    int bMoving;
    char pad00c[4];
    unsigned long long llStamp;
    unsigned long long llStep;
} Ov002PageMark;

extern int data_ov002_0207f634;

extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern unsigned long long OS_GetTick(void);

extern int Ov002_GetMissionProgress(void);
extern int Ov002_Field_GetBlock194(void);

void Ov002_SetPageMarkTarget(int nValue, int bStart)
{
    int nCtx;
    Ov002PageProgress *p;
    Ov002PageMark *m;
    int nSteps;

    nCtx = data_ov002_0207f634;
    p = (Ov002PageProgress *)Ov002_GetMissionProgress();
    m = (Ov002PageMark *)Ov002_Field_GetBlock194();
    if (nCtx == 0) {
        return;
    }

    m->llStamp = OS_GetTick();
    m->nTarget = (int)kh_rt_s32_divmod(nValue * 0xe0, p->nTotal);
    nSteps = 0x19 - (m->nTarget - m->nCurrent) * 0x19 / 0xe0;
    if (nSteps > 0x19) {
        nSteps = 0x19;
    } else if (nSteps < 2) {
        nSteps = 2;
    }
    m->llStep = (unsigned long long)nSteps * 0x82ea >> 6;

    if (bStart != 0 && m->nTarget > m->nCurrent && m->bMoving == 0) {
        PlaySoundChecked(0, 0x32);
        m->bMoving = 1;
    }
}

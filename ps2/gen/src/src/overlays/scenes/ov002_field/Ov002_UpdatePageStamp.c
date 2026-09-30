/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_UpdatePageStamp.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_UpdatePageStamp - set the stamp that says how the page ended.
 *
 * The stamp only appears once the mark has reached where the count puts it and
 * the row allowance still covers that count; a mark that has gone all the way
 * to the end gets the better of the two stamps. Anything short of that clears
 * the stamp and plays the plainer cue instead.
 *
 * ARM.
 */

#include "nitro/types.h"

typedef struct {
    char pad000[0x30];
    u16 aStamp[2];
} Ov002PageContext;

typedef struct {
    int nTotal;
    int nRows;
    int nCurrent;
} Ov002PageProgress;

extern Ov002PageContext *data_ov002_0207f634;

extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern void GFXi_EnqueueCommand(int nCmd, int nDest, int nSrc, int nSize);

extern int Ov002_ForwardToSubDc(int nCue);
extern void Ov002_Ctx_InvokeTagTrackerCallback(int nHandle);
extern int Ov002_GetMissionProgress(void);
extern int Ov002_Field_GetBlock194(void);

void Ov002_UpdatePageStamp(void)
{
    int nCurrent;
    Ov002PageProgress *p;
    Ov002PageContext *ctx;
    int nMark;
    int nTotal;

    ctx = data_ov002_0207f634;
    p = (Ov002PageProgress *)Ov002_GetMissionProgress();
    nMark = *(int *)Ov002_Field_GetBlock194();
    nCurrent = p->nCurrent;
    nTotal = p->nTotal;

    if (nMark >= (int)kh_rt_s32_divmod(nCurrent * 0xe0, nTotal) &&
        p->nRows >= nCurrent) {
        if (nMark >= (int)kh_rt_s32_divmod(nTotal * 0xe0, nTotal)) {
            ctx->aStamp[0] = 0x163f;
            ctx->aStamp[1] = 0x51b;
        } else {
            ctx->aStamp[0] = 0x13ea;
            ctx->aStamp[1] = 0x1e2;
        }
        GFXi_EnqueueCommand(0x1f, 0x142, (int)ctx->aStamp, 4);
        Ov002_Ctx_InvokeTagTrackerCallback(Ov002_ForwardToSubDc(0x418));
    } else {
        Ov002_Ctx_InvokeTagTrackerCallback(Ov002_ForwardToSubDc(0x417));
    }
}

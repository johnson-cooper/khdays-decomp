/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_DrawMissionGaugeTrack.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_DrawMissionGaugeTrack - lay out the mission gauge's 28-cell track and
 * mark where the goal sits on it.
 *
 * The argument is how far along the bar the filled part reaches, in pixels. It
 * splits into the cell the fill ends on and how far into that cell it goes. The
 * goal has its own position, scaled from the progress record onto the same
 * 224-pixel bar and pinned one pixel short of the end so it never falls off.
 *
 * With nothing done yet the track is all empty cells, except that the goal cell
 * is drawn as the marked style with its own sub-cell bar. With work done, cells
 * before the fill are full, the cell the fill ends on carries the partial gauge,
 * and the cells after it are empty; wherever the goal lands it overrides that
 * cell and redraws the sub-cell bar under it. When the goal is the whole bar
 * there is nothing to mark and the track is drawn plainly.
 *
 * ARM.
 *
 * Two details below are load-bearing for codegen rather than style. The
 * declaration order decides the register assignment on this function - the
 * order here was mapped, not guessed - and the clamp has to be a conditional
 * expression rather than an if statement, which is what keeps the clamped
 * cell on a late callee-saved register instead of an early one.
 */

#include "nitro/types.h"

typedef struct {
    int nTotal;
    int nDone;
    int nGoal;
} Ov002Progress;

typedef struct {
    char pad000[0x5c0];
    u16 aTrack[30];                     /* +0x5c0; the 28 track cells start at [2] */
} Ov002ItemTiles;

extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);

extern Ov002Progress *Ov002_GetMissionProgress(void);
extern Ov002ItemTiles *Ov002_GetItemResource(int nItemId);
extern void Ov002_SelectEntry(int nItemId);
extern void Ov002_DrawPageGauge(int nFilled);
extern void Ov002_DrawPageBar(int nFilled, int nMark);
extern void Ov002_DrawPageNeedle(int nValue);

void Ov002_DrawMissionGaugeTrack(int nFillPixels)
{
    int i;                              /* the loop the goal marker sees */
    int nGoalSub;
    int nGoalCell;
    Ov002ItemTiles *pTiles;
    int nFillCell;
    int j;                              /* the loops it cannot reach */
    int nFillSub;
    Ov002Progress *p;
    int nGoalPixels;
    int bMarkGoal;

    p = Ov002_GetMissionProgress();
    nFillCell = nFillPixels / 8;
    nFillCell = nFillCell > 0x1c ? 0x1c : nFillCell;
    nFillSub = nFillPixels % 8;
    nGoalPixels = (int)kh_rt_s32_divmod(p->nGoal * 0xe0, p->nTotal);
    if (nGoalPixels == 0xe0) {
        nGoalPixels--;
    }
    nGoalCell = nGoalPixels / 8;
    pTiles = Ov002_GetItemResource(0x1a);
    bMarkGoal = p->nGoal != p->nTotal;

    if (p->nDone > 0) {
        for (i = 0; i < nFillCell; i++) {
            if (bMarkGoal && i == nGoalCell) {
                nGoalSub = nGoalPixels % 8;
                Ov002_DrawPageBar(8, nGoalSub);
                pTiles->aTrack[i + 2] = 0xa06d;
            } else {
                pTiles->aTrack[i + 2] = 0xa06e;
            }
        }
        if (nFillCell < 0x1c) {
            if (bMarkGoal && nFillCell == nGoalCell) {
                nGoalSub = nGoalPixels % 8;
                Ov002_DrawPageBar(nFillSub, nGoalSub);
                pTiles->aTrack[nFillCell + 2] = 0xa06d;
            } else {
                Ov002_DrawPageGauge(nFillSub);
                pTiles->aTrack[nFillCell + 2] = 0xa06f;
            }
            for (i = nFillCell + 1; i < 0x1c; i++) {
                if (bMarkGoal && i == nGoalCell) {
                    nGoalSub = nGoalPixels % 8;
                    Ov002_DrawPageBar(0, nGoalSub);
                    pTiles->aTrack[i + 2] = 0xa06d;
                } else {
                    pTiles->aTrack[i + 2] = 0xa06c;
                }
            }
        }
    } else if (bMarkGoal) {
        for (i = 0; i < nGoalCell; i++) {
            pTiles->aTrack[i + 2] = 0xa06c;
        }
        nGoalSub = nGoalPixels % 8;
        Ov002_DrawPageBar(0, nGoalSub);
        pTiles->aTrack[i + 2] = 0xa06d;
        for (j = nGoalCell + 1; j < 0x1c; j++) {
            pTiles->aTrack[j + 2] = 0xa06c;
        }
    } else {
        for (j = 0; j < 0x1c; j++) {
            pTiles->aTrack[j + 2] = 0xa06c;
        }
    }

    if (p->nGoal != p->nTotal) {
        Ov002_DrawPageNeedle(nGoalPixels);
    }
    Ov002_SelectEntry(0x1a);
}

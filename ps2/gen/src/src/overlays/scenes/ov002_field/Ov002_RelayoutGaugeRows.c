/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_RelayoutGaugeRows.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"

typedef struct {
    u8 pad0000[0x3c];
    int nRowCount;                      /* +0x3c */
    u8 pad0040[4];
    int nUnitCost;                      /* +0x44 */
    u8 pad0048[0xa4];
    u16 wTotalUnits;                    /* +0xec */
    u16 wCurrentUnits;                  /* +0xee */
} Ov002SceneCtx;

extern Ov002SceneCtx *data_ov002_0207f618;

/* The quotient is the low half of the helper's long long return; writing `/`
 * emits _s32_div_f, which is not linkable here. */
extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern int Ov002_ForwardToSubDc(int nId);
extern void Ov002_Ctx_SetTagTrackerNodeArmed_2(int nHandle, int nFlag);
extern void Ov002_UpdateGaugeRow(int nRow, int bFilled, int bLast);

void Ov002_RelayoutGaugeRows(void) {
    Ov002SceneCtx *ctx = data_ov002_0207f618;
    int nScale;
    int nTotal;
    int nCurrent;
    int nRowsTotal;
    int nRowsCurrent;
    int i;

    for (i = 0; i < (ctx->nRowCount + 1) / 2; i++) {
        Ov002_Ctx_SetTagTrackerNodeArmed_2(Ov002_ForwardToSubDc((u16)(i + 50000)), 1);
    }

    nScale = ctx->nUnitCost * 250;
    nTotal = (int)kh_rt_s32_divmod(ctx->wTotalUnits * 46, nScale);
    nCurrent = (int)kh_rt_s32_divmod(ctx->wCurrentUnits * 46, nScale);

    if (nTotal <= 0) {
        nRowsTotal = 0;
    } else {
        nRowsTotal = (nTotal - 1) / 46;
    }
    if (nCurrent <= 0) {
        nRowsCurrent = 0;
    } else {
        nRowsCurrent = (nCurrent - 1) / 46;
    }

    if (nRowsTotal > 0) {
        for (i = 0; i < nRowsTotal - 1; i++) {
            Ov002_UpdateGaugeRow(i, i < nRowsCurrent, 0);
        }
        Ov002_UpdateGaugeRow(nRowsTotal - 1, nRowsTotal - 1 < nRowsCurrent, 1);
    }

    ctx->nRowCount = nRowsTotal;
}

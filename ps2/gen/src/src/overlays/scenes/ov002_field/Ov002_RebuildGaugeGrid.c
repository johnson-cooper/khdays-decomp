/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_RebuildGaugeGrid.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"

typedef struct {
    u8 pad0000[0x20];
    int nSurfaceStride;                 /* +0x20, surface10.strideTiles */
    u8 pad0024[0x20];
    int nUnitCost;                      /* +0x44 */
    u8 pad0048[0xa4];
    u16 wTotalUnits;                    /* +0xec */
    u16 wCurrentUnits;                  /* +0xee */
    u8 pad00f0[0x3c];
    u16 wRowsA0;                        /* +0x12c */
    u16 wRowsA1;                        /* +0x12e */
    u8 pad0130[0x18];
    u16 wRowsB0;                        /* +0x148 */
    u16 wRowsB1;                        /* +0x14a */
} Ov002SceneCtx;

extern Ov002SceneCtx *data_ov002_0207f618;

/* The quotient is the low half of the helper's long long return; writing `/`
 * emits _s32_div_f, which is not linkable here. */
extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern void Ov002_BuildGaugeRowMap(int nColumn, int nRows);
extern void Ov002_DrawLayoutRow(int nSurface, int nRow, int nMode);
extern int Ov002_ForwardToSubDc(int nId);
extern void Ov002_Ctx_InvokeTagTrackerCallback(int nHandle);

void Ov002_RebuildGaugeGrid(void) {
    Ov002SceneCtx *ctx = data_ov002_0207f618;
    int nScale = ctx->nUnitCost * 250;
    int nTotal = (int)kh_rt_s32_divmod(ctx->wTotalUnits * 46, nScale);
    u16 wCurrent = ctx->wCurrentUnits;
    int nCurrent = (int)kh_rt_s32_divmod(wCurrent * 46, nScale);
    int nBase;
    int nColumn;
    int nRows;
    int nLast;
    int i;

    if (wCurrent != 0 && nCurrent == 0) { nCurrent = 1; }

    nBase = nTotal / 46 * 46;
    nColumn = 46;
    if (nCurrent > nBase || nCurrent == 0) { nColumn = nTotal - nBase; }

    if (nCurrent <= 0) {
        nRows = 0;
    } else {
        nLast = nCurrent - 1;
        nRows = nLast % 46 + 1;
    }

    Ov002_BuildGaugeRowMap(nColumn, nRows);

    ctx->wRowsA0 = (u16)nCurrent;
    ctx->wRowsB0 = ctx->wRowsA0;
    ctx->wRowsA1 = ctx->wRowsA0;
    ctx->wRowsB1 = ctx->wRowsA0;

    for (i = 0; i < nRows; i++) {
        Ov002_DrawLayoutRow(ctx->nSurfaceStride, i, 0);
    }

    Ov002_Ctx_InvokeTagTrackerCallback(Ov002_ForwardToSubDc(0x32));
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_RebuildGaugeFillRows.c (ps2/tools/prep_sources.py). Do not edit. */
/* Converts total and filled gauge units to 46-unit row counts, updates every row style, and marks
 * the final row so the pending row pair is flushed. */

typedef struct {
    unsigned char pad0000[0x44];
    int nUnitCost;
    unsigned char pad0048[0xa4];
    unsigned short wTotalUnits;
} Ov002GaugeCountContext;

extern Ov002GaugeCountContext *data_ov002_0207f618;
extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern void Ov002_UpdateGaugeRow(int nRow, int bFilled, int bLast);

void Ov002_RebuildGaugeFillRows(int nFilledUnits)
{
    Ov002GaugeCountContext *pContext = data_ov002_0207f618;
    long long nQuotient = kh_rt_s32_divmod(pContext->wTotalUnits * 0x2e,
                                       pContext->nUnitCost * 0xfa);
    int nTotalRows;
    int nFilledRows;
    int nRow;

    if ((int)nQuotient <= 0) {
        nTotalRows = 0;
    } else {
        nTotalRows = ((int)nQuotient - 1) / 0x2e;
    }
    if (nFilledUnits <= 0) {
        nFilledRows = 0;
    } else {
        nFilledRows = (nFilledUnits - 1) / 0x2e;
    }
    if (nTotalRows <= 0) {
        return;
    }

    for (nRow = 0; nRow < nTotalRows - 1; nRow++) {
        Ov002_UpdateGaugeRow(nRow, nRow < nFilledRows, 0);
    }
    Ov002_UpdateGaugeRow(nTotalRows - 1,
                        nTotalRows - 1 < nFilledRows, 1);
}

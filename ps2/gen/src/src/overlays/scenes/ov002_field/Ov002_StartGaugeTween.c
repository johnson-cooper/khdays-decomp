/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_StartGaugeTween.c (ps2/tools/prep_sources.py). Do not edit. */
/* Starts a gauge tween between two panel records and repaints the cells the
 * move uncovers.
 *
 * Growing past the destination's drawn count restarts the destination and, for
 * setup 1, pulls the drawn count up to the start of the row the source's own
 * count falls on. Shrinking restarts the source instead, and only redraws when
 * both counts sit on the same row - that comparison is what stops a drain
 * crossing a row boundary from repainting the row above. The counts are then
 * reconciled and the caller is optionally told to push the panel.
 *
 * The helper returns the quotient in the low word and the remainder in the
 * high word, so a quotient is a plain cast and a remainder is a shift by 32. */

#include "nitro/types.h"

typedef void (*Ov002CellFn)(int nHandle, int nCell, int nMode);

typedef struct {
    u16 wCount;                         /* +0x00 */
    u16 wDrawn;                         /* +0x02 */
    u16 wTarget;                        /* +0x04 */
    u16 wSpan;                          /* +0x06 */
    u8 pad0008[8];
    unsigned long long uStamp;          /* +0x10 */
    int bActive;                        /* +0x18 */
} Ov002PanelRecord;

typedef struct {
    int nField0000;
    int nRowWidth;                      /* +0x04 */
    u8 pad0008[0x14];
} Ov002GaugeSetup;

extern Ov002GaugeSetup data_ov002_0207e98c[];

extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern void Ov002_ArmCountdown(Ov002PanelRecord *pRec, int nSeed, int nTarget);
extern void Ov002_UploadSlotIconPalette(int nIndex, int nMode);

void Ov002_StartGaugeTween(Ov002PanelRecord *pDst, Ov002PanelRecord *pSrc,
                         unsigned int nValue, Ov002CellFn pDraw, int nHandle,
                         int bNotify, int nSetup) {
    Ov002GaugeSetup *pSetup = &data_ov002_0207e98c[nSetup];
    int nDrawn = pDst->wDrawn;
    int bSameRow;
    int nWidth;
    int nEnd;
    int nStart;
    int nLast;

    if (nValue >= (unsigned int)nDrawn) {
        Ov002_ArmCountdown(pDst, nDrawn, nValue);
        if (nSetup != 1) {
            return;
        }
        if (pSrc->bActive == 0) {
            return;
        }
        nWidth = pSetup->nRowWidth;
        nEnd = nWidth * (int)kh_rt_s32_divmod(pSrc->wDrawn - 1, nWidth) + 1;
        nStart = pSrc->wTarget;
        if (nStart <= nEnd) {
            nStart = nEnd;
        }
        pDst->wDrawn = (u16)nStart;
        return;
    }

    bSameRow = 1;
    if (nSetup == 1 && pSrc->bActive != 0) {
        if ((pSrc->wDrawn - 1) / 46 > pSrc->wTarget / 46) {
            bSameRow = 0;
        }
    }
    Ov002_ArmCountdown(pSrc, nDrawn, nValue);

    nEnd = pDst->wDrawn;
    if (nEnd == 0) {
        nEnd = 0;
        nLast = 0;
    } else {
        nLast = nEnd - 1;
        nEnd = (int)(kh_rt_s32_divmod(nLast, pSetup->nRowWidth) >> 32) + 1;
    }

    if (bSameRow != 0) {
        if (nValue == 0) {
            nStart = 0;
        } else {
            nWidth = pSetup->nRowWidth;
            if ((int)kh_rt_s32_divmod(nValue, nWidth)
                    < (int)kh_rt_s32_divmod(nLast, nWidth)) {
                nStart = 0;
            } else if (nValue == 0) {
                nStart = 0;
            } else {
                nStart = (int)(kh_rt_s32_divmod(nValue - 1, nWidth) >> 32) + 1;
            }
        }
        while (nStart < nEnd) {
            pDraw(nHandle, nStart, 1);
            nStart = nStart + 1;
        }
    }

    if (pSrc->wCount < pDst->wDrawn) {
        pSrc->wCount = pDst->wDrawn;
    }
    if (pSrc->wDrawn < pDst->wDrawn) {
        pSrc->wDrawn = pSrc->wCount;
    }
    if (pDst->wDrawn > nValue) {
        pDst->bActive = 0;
        pDst->wDrawn = (u16)nValue;
    } else {
        pDst->wTarget = (u16)nValue;
    }

    if (bNotify != 0) {
        Ov002_UploadSlotIconPalette(0, 1);
    }
}

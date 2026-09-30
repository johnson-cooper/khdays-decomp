/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetGaugeValue.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sets a HUD gauge's value: scales it to cells, flashes the slot icon when it drops, and either
 * tweens the main gauge or redraws the gauge cells (the first gauge also records its shown and
 * target counts). */

#include "nitro/types.h"

typedef struct {
    u16 wTotal;                         /* +0x00 */
    u16 wDrawn;                         /* +0x02 */
} Ov002CountPair;

typedef struct {
    int nField0000;
    int nField0004;
    int nField0008;
} Ov002GaugeRow;

typedef void (*Ov002CellFn)(int nTarget, int nCell, int bClearing);

typedef struct {
    int aHandles[4];                    /* +0x00 */
    u8 pad0010[0x18];
    u8 bStateFlags;                     /* +0x28 */
    u8 pad0029[0x11];
    u16 wShown003a;                     /* +0x3a */
    u8 pad003c[0x10];
    unsigned long long aHold[4];        /* +0x4c */
    u8 pad006c[0x60];
    Ov002CountPair aCounts[4];          /* +0xcc */
    u8 pad00dc[0x18];
    u8 aTweenA[0x1c];                   /* +0xf4 */
    u8 aTweenB[2];                      /* +0x110 */
    u16 wCount0112;                     /* +0x112 */
    u16 wCount0114;                     /* +0x114 */
} Ov002SceneCtx;

extern Ov002SceneCtx *data_ov002_0207f618;
extern const Ov002GaugeRow data_ov002_0207de04[];

extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern unsigned long long OS_GetTick(void);
extern void Ov002_UploadSlotIconPalette(int nIndex, int nMode);
extern void Ov002_SetGaugeSlotShown(int nIndex, int nShown);
extern void Ov002_StartGaugeTween(void *pDst, u16 *pSrc, int nValue, void *pDraw,
                                int nHandle, int nA, int nB);
extern int Ov002_RedrawGaugeCells(int nTarget, int nValue, int nScale,
                               Ov002CountPair *pPair, Ov002CellFn pDraw,
                               int bRebuild);
extern void Ov002_DrawShortLayoutRow(int nHandle, int nCell, int nValue);
extern void Ov002_DrawGaugeTweenCell(int nHandle, int nCell, int nValue);
extern void Ov002_DrawTallLayoutRow(int nHandle, int nCell, int nValue);
extern void Ov002_DrawPromptMarker(int nCount);

void Ov002_SetGaugeValue(int nIndex, unsigned int nValue, int nFlags, int bImmediate,
                         int bRebuild) {
    Ov002SceneCtx *ctx = data_ov002_0207f618;
    u16 wShown;
    int nCells;
    int bChanged = 1;
    int nScale;

    if (bImmediate != 0 && nValue == ctx->aCounts[nIndex].wDrawn) {
        return;
    }
    if (ctx->aCounts[nIndex].wDrawn == 0 && nValue != 0) {
        Ov002_UploadSlotIconPalette(nIndex, 0);
    }

    if (nIndex == 0) {
        if (bImmediate != 0) {
            wShown = ctx->wCount0112;
            nScale = data_ov002_0207de04[nIndex].nField0000;
            nCells = (u16)kh_rt_s32_divmod(nValue * nScale, ctx->aCounts[nIndex].wTotal);
            if (nCells == 0 && nValue != 0) {
                nCells = 1;
            }
            if (wShown == nCells && ctx->aCounts[nIndex].wDrawn > nValue) {
                Ov002_UploadSlotIconPalette(nIndex, 1);
                ctx->aHold[nIndex] = OS_GetTick();
                ctx->aCounts[nIndex].wDrawn = (u16)nValue;
                return;
            }
            ctx->aCounts[nIndex].wDrawn = (u16)nValue;
            Ov002_StartGaugeTween(ctx->aTweenB, (u16 *)ctx->aTweenA, nCells,
                                Ov002_DrawGaugeTweenCell, ctx->aHandles[0], 1, 0);
            ctx->wShown003a = (u16)nCells;
        } else {
            nScale = data_ov002_0207de04[nIndex].nField0000;
            bChanged = Ov002_RedrawGaugeCells(ctx->aHandles[nIndex], nValue, (u16)nScale,
                                           &ctx->aCounts[nIndex], Ov002_DrawTallLayoutRow,
                                           bRebuild);
            nCells = (int)kh_rt_s32_divmod(nValue * nScale,
                                        ctx->aCounts[nIndex].wTotal);
            if (nValue != 0 && nCells == 0) {
                ctx->wShown003a = ctx->wCount0112 = ctx->wCount0114 = 1;
            } else {
                ctx->wCount0114 = (u16)nCells;
                ctx->wShown003a = ctx->wCount0112 = ctx->wCount0114;
            }
        }
    } else {
        if (bImmediate != 0 && ctx->aCounts[nIndex].wDrawn > nValue) {
            if (nValue == 0) {
                Ov002_SetGaugeSlotShown(nIndex, 0);
                Ov002_UploadSlotIconPalette(nIndex, 2);
            } else {
                Ov002_UploadSlotIconPalette(nIndex, 1);
                ctx->aHold[nIndex] = OS_GetTick();
            }
        }
        nScale = data_ov002_0207de04[nIndex].nField0000;
        bChanged = Ov002_RedrawGaugeCells(ctx->aHandles[nIndex], nValue, (u16)nScale,
                                       &ctx->aCounts[nIndex], Ov002_DrawShortLayoutRow,
                                       bRebuild);
    }

    if (bChanged == 0) {
        return;
    }
    if (nFlags >= 0) {
        ctx->bStateFlags = ctx->bStateFlags | (1 << (nIndex + 3));
    }
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_FindSelectableItem.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_FindSelectableItem -- Ov008_FindSelectableItem: starting at item nFrom, walk
 * the current list's item ring (first item, count from Ov008_GetPageItem) in
 * steps of nStep, wrapping with a modulo, until an item whose table byte +7 is
 * set (selectable) is found; returns its index or -1 after a full turn.
 *
 * nFrom is a short parameter and the table index is `(short)nIndex * 0x14`: the
 * cast is absorbed into the ROM's smlabb while the implicit narrowing of
 * `nFrom = nIndex` stays a separate lsl/asr (an explicit (s16) on both would be
 * CSE'd into one truncation and an mla).
 */

#include "nitro/types.h"

typedef struct Ov008MenuContext {
    s16 nSelection;           /* 0x000 */
    u16 nListId;              /* 0x002 */
} Ov008MenuContext;

typedef struct Ov008ItemRing {
    u16 nFirst;               /* 0x00 */
    u16 nCount;               /* 0x02 */
} Ov008ItemRing;

#define ITEM_STRIDE 0x14

typedef struct Ov008MenuSubEntry {
    s16 nId;                  /* 0x00 */
    u8  nText;                /* 0x02 */
    u8  nHelpText;            /* 0x03 */
} Ov008MenuSubEntry;

typedef struct Ov008MenuEntryDef {
    s16 nId;                  /* 0x00 */
    u8  nText;                /* 0x02 */
    u8  nHelpText;            /* 0x03 */
    u8  nAnchor;              /* 0x04 */
    u8  nState;               /* 0x05: lock state */
    u8  bEnabled;             /* 0x06 */
    u8  nSubCount;            /* 0x07 */
    Ov008MenuSubEntry aSub[3]; /* 0x08 */
} Ov008MenuEntryDef;

extern Ov008MenuEntryDef data_ov008_02090598[];

extern Ov008MenuContext *Ov008_GetMenuContext(void);               /* Ov008_GetMenuContext */
extern Ov008ItemRing *Ov008_GetPageItem(int nListId, int nSelection);
extern long long kh_rt_s32_divmod(int nNum, int nDen);               /* _s32_div_f: quotient, remainder in the high word */

int Ov008_FindSelectableItem(s16 nFrom, int nStep)
{
    Ov008MenuContext *pCtx;
    Ov008ItemRing *pRing;
    int nIndex;
    int i;
    int nFirst;
    int nCount;
    int nSel;

    pCtx = Ov008_GetMenuContext();
    pRing = Ov008_GetPageItem(pCtx->nListId, (u16)pCtx->nSelection);
    nCount = pRing->nCount;
    i = 0;
    if (nCount > 0) {
        nFirst = pRing->nFirst;
        do {
            nIndex = nFirst + (int)(kh_rt_s32_divmod(nCount + (nFrom - nFirst), nCount) >> 32);
            nSel = data_ov008_02090598[(short)nIndex].nSubCount;
            nFrom = nIndex;
            if (nSel != 0) {
                break;
            }
            nFrom = nFrom + nStep;
            i = (s16)(i + 1);
        } while (i < nCount);
    }
    if (i == nCount) {
        nFrom = -1;
    }
    return nFrom;
}

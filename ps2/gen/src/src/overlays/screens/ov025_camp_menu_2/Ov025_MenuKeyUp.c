/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_MenuKeyUp.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov025_MenuKeyUp -- Ov008_MenuKeyUp: the menu's "up" key handler.
 * Ignored while a touch is down or while another key (other than up, bit 6)
 * is latched in the key word (+0x2098); latches up.  In modes 0/1 it moves the
 * cursor one row up (Ov025_MoveGridCursor) and, if that took, arms the move,
 * clears the hold counter and plays cue 0x35.  In mode 2 with nothing pending it
 * flips to the previous page ((page + count - 1) mod page count) and, when that took and
 * there is more than one page, plays cue 0.
 */

#include "nitro/types.h"
#include "game/engine.h"

#define KEY_UP     0x40
#define SOUND_MOVE 0x35

typedef struct Ov008MenuContext {
    u8   pad_0000[0x8];
    int  nPending;            /* 0x0008 */
    u8   pad_000c[4];
    u32  nMode;               /* 0x0010 */
    u8   pad_0014[0x38 - 0x14];
    int  bMoveArmed;          /* 0x0038 */
    u8   pad_003c[8];
    int  nHoldCount;          /* 0x0044 */
    u8   pad_0048[0x64 - 0x48];
    u16  nColumn;             /* 0x0064 */
    u16  nRowSel;             /* 0x0066 */
    u8   pad_0068[0x78 - 0x68];
    int  nPageCount;          /* 0x0078 */
    u8   pad_007c[0x9c - 0x7c];
    int  nPage;               /* 0x009c */
    u8   pad_00a0[0x2098 - 0xa0];
    u16  nKeyLatch;           /* 0x2098 */
} Ov008MenuContext;

extern void Ov025_CopySourceBlock(void *pOut);                              /* touch record */
extern int  Ov025_MoveGridCursor(Ov008MenuContext *pCtx, int nColumn, int nRow, int nStep);
extern long long kh_rt_s32_divmod(int nNum, int nDen);                       /* _s32_div_f: remainder in the high word */
extern int  Ov025_SelectListRow(Ov008MenuContext *pCtx, int nPage);       /* switch page */

void Ov025_MenuKeyUp(Ov008MenuContext *pCtx)
{
    u16 touch[4];                 /* the NitroSDK's TPData: x, y, touch, validity */

    Ov025_CopySourceBlock(touch);
    if (touch[2] != 0) {
        return;
    }
    if (pCtx->nKeyLatch != 0 && (pCtx->nKeyLatch & KEY_UP) == 0) {
        return;
    }
    pCtx->nKeyLatch = KEY_UP;
    switch (pCtx->nMode) {
    case 0:
    case 1:
        if (Ov025_MoveGridCursor(pCtx, pCtx->nColumn, pCtx->nRowSel - 1, 1) == 0) {
            return;
        }
        pCtx->bMoveArmed = 1;
        pCtx->nHoldCount = 0;
        PlaySound(0, SOUND_MOVE);
        break;
    case 2:
        if (pCtx->nPending != 0) {
            return;
        }
        if (Ov025_SelectListRow(pCtx, (int)(kh_rt_s32_divmod(pCtx->nPage + pCtx->nPageCount - 1, pCtx->nPageCount) >> 32)) == 0) {
            return;
        }
        if (pCtx->nPageCount > 1) {
            PlaySound(0, 0);
        }
        break;
    }
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_ShowMissionListInfoPanel.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_ShowMissionListInfoPanel -- Ov008_ShowMissionListInfoPanel: expand (bExpand)
 * or collapse the mission list's info panel (twin of Ov008_ShowMissionInfoPanel).
 * Expanding shows entries 0x33, 0x35, 0x36, hides the cursor entry 1, resets
 * the two-slot entries 0x35 / 0x36 to frame 0, re-links 0x36, pushes subitem
 * pair 0 of 0x35, hides 0x37, shows 0x38, sets entries 2..0x13 to frame 1
 * and darkens the sub screen's blend planes (0x04001050, planes 0xf) by 8.
 * Collapsing hides 0x33, 0x35..0x38, shows entry 1, sets 2..0x13 back to
 * frame 0, restores the brightness and locks the list (+0x500).  The request is
 * remembered at +0x4f8 and the armed word (+0x4fc) cleared.
 */

#include "nitro/types.h"

#define ENTRY_INFO_FIRST 2
#define ENTRY_INFO_LAST  0x13
#define ENTRY_CURSOR     1
#define BLEND_PLANES     0xf
#define REG_BLDCNT_SUB   ((void *)((unsigned int)kh_ds_io + 0x1050))

typedef struct Ov008MissionList {
    u8  pad_000[0x4f8];
    int bSelectionPending;    /* 0x4f8 */
    int bSelectionArmed;      /* 0x4fc */
    int bLocked;           /* 0x500 */
} Ov008MissionList;

extern int   Ov008_GetCtxBlock4a80(void);                                   /* Ov008_GetCtxBlock4a80 */
extern void *Ov008_FindEntryById(int nCtx, int nId);                      /* FindEntryById */
extern void  Ov008_SetEntrySlotsVisible(int nCtx, void *pEntry, int bVisible);    /* SetEntrySlotsVisible */
extern void  Ov008_ReleaseTwoSlotsEx(int nCtx, void *pEntry, int nFrame);      /* Ov008_ReleaseTwoSlotsEx */
extern void  Ov008_SwapParamOverrides(int nCtx, void *pEntry);                  /* Ov008_SwapParamOverrides */
extern void  Ov008_PushSubitemPair(int nCtx, void *pEntry, int nPair);       /* Ov008_PushSubitemPair */
extern void  G2x_SetBlendBrightness_(void *pReg, int nPlaneMask, int nBrightness);

void Ov008_ShowMissionListInfoPanel(Ov008MissionList *pList, int bExpand)
{
    int nCtx;
    u16 nId;

    nCtx = Ov008_GetCtxBlock4a80();
    if (bExpand != 0) {
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x33), 1);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x35), 1);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x36), 1);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, ENTRY_CURSOR), 0);
        Ov008_ReleaseTwoSlotsEx(nCtx, Ov008_FindEntryById(nCtx, 0x35), 0);
        Ov008_ReleaseTwoSlotsEx(nCtx, Ov008_FindEntryById(nCtx, 0x36), 0);
        Ov008_SwapParamOverrides(nCtx, Ov008_FindEntryById(nCtx, 0x36));
        Ov008_PushSubitemPair(nCtx, Ov008_FindEntryById(nCtx, 0x35), 0);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x37), 0);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x38), 1);
        for (nId = ENTRY_INFO_FIRST; nId <= ENTRY_INFO_LAST; nId++) {
            Ov008_ReleaseTwoSlotsEx(nCtx, Ov008_FindEntryById(nCtx, nId), 1);
        }
        G2x_SetBlendBrightness_(REG_BLDCNT_SUB, BLEND_PLANES, -8);
    } else {
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x33), 0);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x35), 0);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x36), 0);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, ENTRY_CURSOR), 1);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x37), 0);
        Ov008_SetEntrySlotsVisible(nCtx, Ov008_FindEntryById(nCtx, 0x38), 0);
        for (nId = ENTRY_INFO_FIRST; nId <= ENTRY_INFO_LAST; nId++) {
            Ov008_ReleaseTwoSlotsEx(nCtx, Ov008_FindEntryById(nCtx, nId), 0);
        }
        G2x_SetBlendBrightness_(REG_BLDCNT_SUB, BLEND_PLANES, 0);
        pList->bLocked = 1;
    }
    pList->bSelectionPending = bExpand;
    pList->bSelectionArmed = 0;
}

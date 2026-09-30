/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_ShowMissionInfoPanel.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov025_ShowMissionInfoPanel -- Ov008_ShowMissionInfoPanel: expand (bExpand) or
 * collapse the mission menu's info panel.  Expanding shows entries 0x34..0x36,
 * resets the two-slot entries 0x35 / 0x36 to frame 0, re-links 0x36, pushes
 * subitem pair 0 of 0x35, hides 0x37, shows 0x38, sets entries 0x16..0x20 to
 * frame 1 and darkens the sub screen's blend planes (0x04001050, planes
 * 0xf) by 8.  Collapsing hides 0x34..0x38, sets 0x16..0x20 back to frame 0
 * and restores the brightness.  The request is remembered at +0x180 and the
 * armed word (+0x184) cleared.
 */

#include "nitro/types.h"

#define ENTRY_INFO_FIRST 0x16
#define ENTRY_INFO_LAST  0x20
#define BLEND_PLANES     0xf
#define REG_BLDCNT_SUB   ((void *)((unsigned int)kh_ds_io + 0x1050))

typedef struct Ov008MissionMenu {
    u8  pad_000[0x180];
    int bSelectionPending;    /* 0x180 */
    int bSelectionArmed;      /* 0x184 */
} Ov008MissionMenu;

extern int   Ov025_GetBlock4a80(void);                                   /* Ov008_GetCtxBlock4a80 */
extern void *Ov025_FindEntryById(int nCtx, int nId);                      /* FindEntryById */
extern void  Ov025_SetEntrySlotsVisible(int nCtx, void *pEntry, int bVisible);    /* SetEntrySlotsVisible */
extern void  Ov025_ReleaseTwoSlotsEx_2(int nCtx, void *pEntry, int nFrame);      /* Ov008_ReleaseTwoSlotsEx */
extern void  Ov025_SwapParamOverrides(int nCtx, void *pEntry);                  /* Ov008_SwapParamOverrides */
extern void  Ov025_PushSubitemPair(int nCtx, void *pEntry, int nPair);       /* Ov008_PushSubitemPair */
extern void  G2x_SetBlendBrightness_(void *pReg, int nPlaneMask, int nBrightness);

void Ov025_ShowMissionInfoPanel(Ov008MissionMenu *pMenu, int bExpand)
{
    int nCtx;
    u16 nId;

    nCtx = Ov025_GetBlock4a80();
    if (bExpand != 0) {
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x34), 1);
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x35), 1);
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x36), 1);
        Ov025_ReleaseTwoSlotsEx_2(nCtx, Ov025_FindEntryById(nCtx, 0x35), 0);
        Ov025_ReleaseTwoSlotsEx_2(nCtx, Ov025_FindEntryById(nCtx, 0x36), 0);
        Ov025_SwapParamOverrides(nCtx, Ov025_FindEntryById(nCtx, 0x36));
        Ov025_PushSubitemPair(nCtx, Ov025_FindEntryById(nCtx, 0x35), 0);
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x37), 0);
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x38), 1);
        for (nId = ENTRY_INFO_FIRST; nId <= ENTRY_INFO_LAST; nId++) {
            Ov025_ReleaseTwoSlotsEx_2(nCtx, Ov025_FindEntryById(nCtx, nId), 1);
        }
        G2x_SetBlendBrightness_(REG_BLDCNT_SUB, BLEND_PLANES, -8);
    } else {
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x34), 0);
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x35), 0);
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x36), 0);
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x37), 0);
        Ov025_SetEntrySlotsVisible(nCtx, Ov025_FindEntryById(nCtx, 0x38), 0);
        for (nId = ENTRY_INFO_FIRST; nId <= ENTRY_INFO_LAST; nId++) {
            Ov025_ReleaseTwoSlotsEx_2(nCtx, Ov025_FindEntryById(nCtx, nId), 0);
        }
        G2x_SetBlendBrightness_(REG_BLDCNT_SUB, BLEND_PLANES, 0);
    }
    pMenu->bSelectionPending = bExpand;
    pMenu->bSelectionArmed = 0;
}

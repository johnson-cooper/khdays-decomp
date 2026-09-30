/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/Ov023_OpenDialog.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov023_OpenDialog -- Ov023_OpenDialog: bring up an ov002 dialog over the event scene.  The
 * sub screen's brightness word (+0x114) starts at -16 with a 5-frame timer (+0x10c) and the sub
 * blend registers are set to full darkness (G2x_SetBlendBrightness_ on BLDCNT_SUB, 0x2c, -16);
 * the dialog class (ov002 data 0207eee8) is instantiated on the argument and kept at +0x87588,
 * the ov002 dialog hooks are pointed at the scene's message-box poster (02083d58) and its
 * query (02083d88), the dialog state (+0x875e4) becomes 1 and the global byte 0204bd85 is
 * cleared (02020878 0).  Ov023_StepDialog (02083e08) drives it from there. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov023Scene {
    u8   pad_00000[0x10c];
    int  nDialogTimer;        /* 0x0010c */
    int  nField110;           /* 0x00110 */
    int  nSubBrightness;      /* 0x00114 */
    u8   pad_00118[0x87588 - 0x118];
    void *pDialog;            /* 0x87588 */
    u8   pad_8758c[0x875e4 - 0x8758c];
    int  nDialogState;        /* 0x875e4 */
} Ov023Scene;

typedef struct Ov023SceneRoot {
    int  nField00;            /* 0x00 */
    Ov023Scene *pScene;       /* 0x04 */
} Ov023SceneRoot;

extern void  G2x_SetBlendBrightness_(int nBldCnt, int nPlanes, int nBrightness);
extern void *InstantiateClass(const void *pClass, void *pArg);         /* InstantiateClass */
extern void  Ov002_HudPage_SetDialogCallbacks(void *pPostMessage, void *pQuery); /* set the ov002 dialog hooks */
extern void  Ov023_PostMessageBox(int nA, int nB);                   /* Ov023_PostMessageBox */
extern int   Ov023_DialogFlagBit2(void);                             /* Ov023_QueryDialogFlag */
extern Ov023SceneRoot data_ov023_0208a784;
extern const u8 data_ov002_0207eee8[];                              /* the ov002 dialog class */

void Ov023_OpenDialog(void *pArg)
{
    data_ov023_0208a784.pScene->nSubBrightness = -16;
    data_ov023_0208a784.pScene->nDialogTimer = 5;
    G2x_SetBlendBrightness_(((unsigned int)kh_ds_io + 0x1050), 0x2c, -16);
    data_ov023_0208a784.pScene->pDialog = InstantiateClass(data_ov002_0207eee8, pArg);
    Ov002_HudPage_SetDialogCallbacks(Ov023_PostMessageBox, Ov023_DialogFlagBit2);
    data_ov023_0208a784.pScene->nDialogState = 1;
    PauseMenu_SetAllowed(0);
}

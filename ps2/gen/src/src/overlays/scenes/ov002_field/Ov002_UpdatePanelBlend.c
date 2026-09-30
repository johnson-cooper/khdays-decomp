/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_UpdatePanelBlend.c (ps2/tools/prep_sources.py). Do not edit. */

#include "nitro/types.h"

typedef struct Ov002PanelContext {
    unsigned char gap0000[0x18c];
    int nPanelState;
} Ov002PanelContext;

extern Ov002PanelContext *data_ov002_0207f614;

extern void G2x_SetBlendAlpha_(u32 *pRegister, u32 nFirstTarget,
                               u32 nSecondTarget, u32 nEva, u32 nEvb);
extern void G2x_SetBlendBrightness_(u16 *pRegister, u32 nPlaneMask,
                                    int nBrightness);
extern void Ov002_ScenePanel_UploadSurfaces(void);
extern int Ov002_Field_IsActive(void);

/* Keep the panel blend registers synchronized with the active request state. */
void Ov002_UpdatePanelBlend(void)
{
    int nPanelState;

    if (data_ov002_0207f614 == 0) {
        return;
    }
    nPanelState = data_ov002_0207f614->nPanelState;
    if (nPanelState >= 9 && nPanelState <= 11) {
        G2x_SetBlendAlpha_((u32 *)((unsigned int)kh_ds_io + 0x50), 8, 0x21, 3, 0xd);
        Ov002_ScenePanel_UploadSurfaces();
        return;
    }
    if (Ov002_Field_IsActive() != 0) {
        G2x_SetBlendBrightness_((u16 *)((unsigned int)kh_ds_io + 0x1050), 0x2c, -8);
    }
    G2x_SetBlendAlpha_((u32 *)((unsigned int)kh_ds_io + 0x50), 8, 0x21, 0x10, 0);
}

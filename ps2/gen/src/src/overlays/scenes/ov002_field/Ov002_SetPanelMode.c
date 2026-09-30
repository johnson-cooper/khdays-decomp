/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetPanelMode.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_SetPanelMode - choose which set of per-frame hooks the panel runs, and
 * park the layers they drive.
 *
 * Mode zero is the main-screen panel: BG1 and BG3 of the main engine are put
 * at horizontal 0, vertical 0x18, and all four hooks are installed. Any other
 * mode is the sub-screen one: BG0 and BG1 of the sub engine get the same
 * offsets and only the first two hooks are installed, the other two cleared.
 * The mode is remembered last.
 *
 * ARM.
 */

#include "nitro/types.h"

typedef struct {
    char pad0000[0x60];
    int nPanelMode;                     /* +0x060 */
    char pad0064[0x1cc];
    void *apHandlers[4];                /* +0x230 */
} Ov002PanelCtx;

extern Ov002PanelCtx *data_ov002_0207f614;

extern void Ov002_SetBgOfs_main0014(void);
extern void Ov002_SetBgOfs_main001c(void);
extern void Ov002_SetBgOfs_sub1010(void);
extern void Ov002_SetBgOfs_sub1014(void);
extern void Ov002_SetWindowRegs_sub1040(void);
extern void Ov002_SetWindowRegs_sub1042(void);

void Ov002_SetPanelMode(int nMode)
{
    Ov002PanelCtx *ctx;
    volatile u32 *pOfs;

    ctx = data_ov002_0207f614;
    if (nMode != 0) {
        pOfs = (volatile u32 *)((unsigned int)kh_ds_io + 0x1010);
        pOfs[0] = 0x180000;
        pOfs[1] = 0x180000;
        ctx->apHandlers[0] = (void *)Ov002_SetBgOfs_main0014;
        ctx->apHandlers[1] = (void *)Ov002_SetBgOfs_main001c;
        ctx->apHandlers[2] = 0;
        ctx->apHandlers[3] = 0;
    } else {
        pOfs = (volatile u32 *)((unsigned int)kh_ds_io + 0x14);
        pOfs[0] = 0x180000;
        pOfs[2] = 0x180000;
        ctx->apHandlers[0] = (void *)Ov002_SetBgOfs_sub1010;
        ctx->apHandlers[1] = (void *)Ov002_SetBgOfs_sub1014;
        ctx->apHandlers[2] = (void *)Ov002_SetWindowRegs_sub1040;
        ctx->apHandlers[3] = (void *)Ov002_SetWindowRegs_sub1042;
    }
    ctx->nPanelMode = nMode;
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_ReleasePanelSurface.c (ps2/tools/prep_sources.py). Do not edit. */
/* Reset the BG1 and BG3 scroll registers, release the panel's tiled surface at
 * +0x264, and park the panel state at 8.
 *
 * The two MMIO writes are 32-bit at 0x04000014 and 0x0400001c, which clears
 * BG1HOFS+BG1VOFS and BG3HOFS+BG3VOFS as pairs -- one store per background
 * rather than four halfword stores.
 */
typedef struct {
    char pad0000[0x18c];
    int nField018c;          /* +0x18c */
    char pad0190[0xd4];
    int aSurface0264[1];     /* +0x264 */
} Ov002PanelContext;

extern void Obj_InvokeInnerVtable4(int *surface);
extern void EnqueueObjGfxCommand(int *surface);
extern Ov002PanelContext *data_ov002_0207f614;

void Ov002_ReleasePanelSurface(void) {
    Ov002PanelContext *ctx = data_ov002_0207f614;

    *(unsigned int *)((unsigned int)kh_ds_io + 0x14) = 0;
    *(unsigned int *)((unsigned int)kh_ds_io + 0x1c) = 0;
    Obj_InvokeInnerVtable4(ctx->aSurface0264);
    EnqueueObjGfxCommand(ctx->aSurface0264);
    ctx->nField018c = 8;
}

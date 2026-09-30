/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/Ov011_InitTitleTileSurface.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov011_InitTitleTileSurface -- build one of the title's tile surfaces: a 0x17-tile-wide 4bpp
 * surface of the given height, palette 15, fed from the scene's shared resource storage (+0x23a94)
 * and uploaded to the VRAM target handed in, with the caller's extra word (5 / 0x15) in the
 * config's +0x1c. Four times from Ov011_SetupTitleTileSurfaces. The title file defines the scene
 * globals itself (gOv011Cursor, gpOv011Scene): mwcc addresses them .bss-relative and hoists the
 * scalar pointer load above the config stores. Returns what TileSurface_InitAndUpload4bpp returns.
 */

/* TileSurfaceCfg */

#include "nitro/types.h"
#include "game/engine.h"

struct TileSurfaceCfg {
    int nUnk00;                  /* 0x00 */
    int nUnk04;                  /* 0x04 */
    int nWidthTiles;             /* 0x08 */
    int nHeightTiles;            /* 0x0c */
    int nRowTiles;               /* 0x10 */
    int nPaletteIndex;           /* 0x14 */
    void *pVramTarget;           /* 0x18 */
    int nUnk1c;                  /* 0x1c */
    void *pPixels;               /* 0x20 */
    int nUnk24;                  /* 0x24 */
};

/* Ov011Scene */
struct Scene {
    u8 pad00000[0x23a94];
    u8 aResourceStorage[8];      /* 0x23a94 */
};

#define SURFACE_KIND 9
#define SURFACE_WIDTH 0x17
#define SURFACE_PALETTE 0xf
#define SURFACE_STRIDE 0x20

/* khdays: shared-bss */
int data_ov011_0205e960;                                                  /* gOv011Cursor */
struct Scene *data_ov011_0205e964;                                         /* gpOv011Scene */

int Ov011_InitTitleTileSurface(void *pSurface, int nHeightTiles, void *pVramTarget, int nUnk1c)
{
    struct TileSurfaceCfg cfg;

    cfg.nUnk00 = 0;
    cfg.nUnk04 = SURFACE_KIND;
    cfg.nWidthTiles = SURFACE_WIDTH;
    cfg.nHeightTiles = nHeightTiles;
    cfg.nRowTiles = 0;
    cfg.nPaletteIndex = SURFACE_PALETTE;
    cfg.pVramTarget = pVramTarget;
    cfg.nUnk1c = nUnk1c;
    cfg.pPixels = data_ov011_0205e964->aResourceStorage;
    cfg.nUnk24 = SURFACE_STRIDE;
    return TileSurface_InitAndUpload4bpp(pSurface, &cfg);
}

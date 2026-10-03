/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_surface_0208e84c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 surface configuration data_ov008_0208e84c, 0x0208e84c-0x0208e860 (.rodata).
 *
 * Copied into a 0x4c-byte surface by 0205546c, which allocates the cell,
 * sprite and slot arrays from the three counts and keeps the two hooks.
 * This one configures the campaign menu's surface (Ov008_InitCampaignMenuContext).
 */

typedef void (*Ov008SurfaceFn)(void);

typedef struct Ov008SurfaceConfig {
    int nCellCount;           /* 0x00: 0x38-byte cells */
    int nSpriteCount;         /* 0x04: 0x30-byte sprites */
    int nSlotCount;           /* 0x08: 16-byte slots */
    Ov008SurfaceFn pfnDraw;   /* 0x0c */
    Ov008SurfaceFn pfnRelease; /* 0x10 */
} Ov008SurfaceConfig;

extern void Ov008_ResourceEntryCallback(void);
extern void Ov008_ResourceNodeCallback(void);

const Ov008SurfaceConfig data_ov008_0208e84c __attribute__((aligned(__alignof__(Ov008SurfaceConfig)))) = {
    256,  /* nCellCount */
    32,  /* nSpriteCount */
    32,  /* nSlotCount */
    Ov008_ResourceEntryCallback,  /* pfnDraw */
    Ov008_ResourceNodeCallback,  /* pfnRelease */
};

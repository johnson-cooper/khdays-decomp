/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_surface_0208fec8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 surface configuration data_ov008_0208fec8, 0x0208fec8-0x0208fedc (.rodata).
 *
 * Copied into a 0x4c-byte surface by 0205546c, which allocates the cell,
 * sprite and slot arrays from the three counts and keeps the two hooks.
 * This one configures the shop container's surface (0208247c).
 */

typedef void (*Ov008SurfaceFn)(void);

typedef struct Ov008SurfaceConfig {
    int nCellCount;           /* 0x00: 0x38-byte cells */
    int nSpriteCount;         /* 0x04: 0x30-byte sprites */
    int nSlotCount;           /* 0x08: 16-byte slots */
    Ov008SurfaceFn pfnDraw;   /* 0x0c */
    Ov008SurfaceFn pfnRelease; /* 0x10 */
} Ov008SurfaceConfig;

extern void Ov008_ResourceEntryCallback_3(void);
extern void Ov008_ResourceNodeCallback_3(void);

const Ov008SurfaceConfig data_ov008_0208fec8 __attribute__((aligned(__alignof__(Ov008SurfaceConfig)))) = {
    105,  /* nCellCount */
    1,  /* nSpriteCount */
    28,  /* nSlotCount */
    Ov008_ResourceEntryCallback_3,  /* pfnDraw */
    Ov008_ResourceNodeCallback_3,  /* pfnRelease */
};

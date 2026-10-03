/* PS2: mechanically prepared copy of src/overlays/players/ov056_player_larxene_2/data/ov056_ctorargs_020b7510.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov056 constructor argument block data_ov056_020b7510, 0x020b7510-0x020b7524 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/la/li_e0.p.z') and four parameters the constructor reads.  Used by Ov056_InitEffectSlotsAlt.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv056LarxeneLiE0PackPath;  /* the path string */

const ClassCtorArgs data_ov056_020b7510 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv056LarxeneLiE0PackPath,
    { 3, 0, 0, 0 },
};

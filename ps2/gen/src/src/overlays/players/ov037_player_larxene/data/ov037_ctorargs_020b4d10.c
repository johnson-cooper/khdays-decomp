/* PS2: mechanically prepared copy of src/overlays/players/ov037_player_larxene/data/ov037_ctorargs_020b4d10.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov037 constructor argument block data_ov037_020b4d10, 0x020b4d10-0x020b4d24 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/la/li_e0.p.z') and four parameters the constructor reads.  Used by Ov037_InitEffectSlotsAlt.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv037LarxeneLiE0PackPath;  /* the path string */

const ClassCtorArgs data_ov037_020b4d10 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv037LarxeneLiE0PackPath,
    { 3, 0, 0, 0 },
};

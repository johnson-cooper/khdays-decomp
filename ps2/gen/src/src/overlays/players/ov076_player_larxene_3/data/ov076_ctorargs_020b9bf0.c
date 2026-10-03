/* PS2: mechanically prepared copy of src/overlays/players/ov076_player_larxene_3/data/ov076_ctorargs_020b9bf0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov076 constructor argument block data_ov076_020b9bf0, 0x020b9bf0-0x020b9c04 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/la/li_e0.p.z') and four parameters the constructor reads.  Used by Ov076_InitEffectSlotsAlt.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv076LarxeneLiE0PackPath;  /* the path string */

const ClassCtorArgs data_ov076_020b9bf0 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv076LarxeneLiE0PackPath,
    { 3, 0, 0, 0 },
};

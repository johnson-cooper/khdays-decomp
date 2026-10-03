/* PS2: mechanically prepared copy of src/overlays/players/ov093_player_larxene_4/data/ov093_ctorargs_020bc2b0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov093 constructor argument block data_ov093_020bc2b0, 0x020bc2b0-0x020bc2c4 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/la/li_e0.p.z') and four parameters the constructor reads.  Used by Ov093_InitEffectSlotsAlt.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv093LarxeneLiE0PackPath;  /* the path string */

const ClassCtorArgs data_ov093_020bc2b0 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv093LarxeneLiE0PackPath,
    { 3, 0, 0, 0 },
};

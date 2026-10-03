/* PS2: mechanically prepared copy of src/overlays/players/ov086_player_goofy_3/data/ov086_ctorargs_020b996c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov086 constructor argument block data_ov086_020b996c, 0x020b996c-0x020b9980 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/go/li_e2.p.z') and four parameters the constructor reads.  Used by Ov086_InitThreeEffectSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv086GoofyLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov086_020b996c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv086GoofyLiE2PackPath,
    { 4, 0, 0, 0 },
};

/* PS2: mechanically prepared copy of src/overlays/players/ov030_player_roxas/data/ov030_ctorargs_020b58b0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov030 constructor argument block data_ov030_020b58b0, 0x020b58b0-0x020b58c4 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/ro/w_d00.p.z') and four parameters the constructor reads.  Used by Ov030_SetUpScene.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv030RoxasWD00PackPath;  /* the path string */

const ClassCtorArgs data_ov030_020b58b0 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv030RoxasWD00PackPath,
    { 2, 0, 0, 7 },
};

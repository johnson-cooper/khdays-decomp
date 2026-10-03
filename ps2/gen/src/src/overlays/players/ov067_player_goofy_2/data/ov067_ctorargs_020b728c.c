/* PS2: mechanically prepared copy of src/overlays/players/ov067_player_goofy_2/data/ov067_ctorargs_020b728c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov067 constructor argument block data_ov067_020b728c, 0x020b728c-0x020b72a0 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/go/li_e2.p.z') and four parameters the constructor reads.  Used by Ov067_InitThreeEffectSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv067GoofyLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov067_020b728c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv067GoofyLiE2PackPath,
    { 4, 0, 0, 0 },
};

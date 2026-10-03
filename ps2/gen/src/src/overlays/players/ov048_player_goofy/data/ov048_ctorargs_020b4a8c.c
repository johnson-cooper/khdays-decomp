/* PS2: mechanically prepared copy of src/overlays/players/ov048_player_goofy/data/ov048_ctorargs_020b4a8c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov048 constructor argument block data_ov048_020b4a8c, 0x020b4a8c-0x020b4aa0 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/go/li_e2.p.z') and four parameters the constructor reads.  Used by Ov048_InitThreeEffectSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv048GoofyLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov048_020b4a8c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv048GoofyLiE2PackPath,
    { 4, 0, 0, 0 },
};

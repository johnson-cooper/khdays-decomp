/* PS2: mechanically prepared copy of src/overlays/players/ov103_player_goofy_4/data/ov103_ctorargs_020bc02c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov103 constructor argument block data_ov103_020bc02c, 0x020bc02c-0x020bc040 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/go/li_e2.p.z') and four parameters the constructor reads.  Used by Ov103_InitThreeEffectSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv103GoofyLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov103_020bc02c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv103GoofyLiE2PackPath,
    { 4, 0, 0, 0 },
};

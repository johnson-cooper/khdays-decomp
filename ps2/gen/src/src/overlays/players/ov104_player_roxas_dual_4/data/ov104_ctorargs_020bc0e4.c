/* PS2: mechanically prepared copy of src/overlays/players/ov104_player_roxas_dual_4/data/ov104_ctorargs_020bc0e4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov104 constructor argument block data_ov104_020bc0e4, 0x020bc0e4-0x020bc0f8 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/r2/li_e2.p.z') and four parameters the constructor reads.  Used by Ov104_InitEffectSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv104RoxasDualLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov104_020bc0e4 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv104RoxasDualLiE2PackPath,
    { 4, 0, 0, 0 },
};

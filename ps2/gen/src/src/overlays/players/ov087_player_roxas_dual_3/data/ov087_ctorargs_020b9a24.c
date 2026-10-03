/* PS2: mechanically prepared copy of src/overlays/players/ov087_player_roxas_dual_3/data/ov087_ctorargs_020b9a24.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov087 constructor argument block data_ov087_020b9a24, 0x020b9a24-0x020b9a38 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/r2/li_e2.p.z') and four parameters the constructor reads.  Used by Ov087_InitEffectSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv087RoxasDualLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov087_020b9a24 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv087RoxasDualLiE2PackPath,
    { 4, 0, 0, 0 },
};

/* PS2: mechanically prepared copy of src/overlays/players/ov049_player_roxas_dual/data/ov049_ctorargs_020b4b44.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov049 constructor argument block data_ov049_020b4b44, 0x020b4b44-0x020b4b58 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/r2/li_e2.p.z') and four parameters the constructor reads.  Used by Ov049_InitEffectSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv049RoxasDualLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov049_020b4b44 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv049RoxasDualLiE2PackPath,
    { 4, 0, 0, 0 },
};

/* PS2: mechanically prepared copy of src/overlays/players/ov035_player_sora/data/ov035_ctorargs_020b4b78.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov035 constructor argument block data_ov035_020b4b78, 0x020b4b78-0x020b4b8c (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/so/li_e1.p.z') and four parameters the constructor reads.  Used by Ov035_InitEffectSlotsWithTimings.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv035SoraLiE1PackPath;  /* the path string */

const ClassCtorArgs data_ov035_020b4b78 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv035SoraLiE1PackPath,
    { 1, 0, 0, 0 },
};

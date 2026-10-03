/* PS2: mechanically prepared copy of src/overlays/players/ov052_player_xigbar_2/data/ov052_ctorargs_020b7f24.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov052 constructor argument block data_ov052_020b7f24, 0x020b7f24-0x020b7f38 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/xi/li_e2.p.z') and four parameters the constructor reads.  Used by Ov052_MissionStart.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv052XigbarLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov052_020b7f24 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv052XigbarLiE2PackPath,
    { 3, 0, 0, 0 },
};

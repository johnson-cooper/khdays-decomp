/* PS2: mechanically prepared copy of src/overlays/players/ov032_player_xigbar/data/ov032_ctorargs_020b5724.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov032 constructor argument block data_ov032_020b5724, 0x020b5724-0x020b5738 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/xi/li_e2.p.z') and four parameters the constructor reads.  Used by Ov032_MissionStart.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv032XigbarLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov032_020b5724 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv032XigbarLiE2PackPath,
    { 3, 0, 0, 0 },
};

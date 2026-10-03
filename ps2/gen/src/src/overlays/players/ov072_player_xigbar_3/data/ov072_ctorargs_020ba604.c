/* PS2: mechanically prepared copy of src/overlays/players/ov072_player_xigbar_3/data/ov072_ctorargs_020ba604.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov072 constructor argument block data_ov072_020ba604, 0x020ba604-0x020ba618 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/xi/li_e2.p.z') and four parameters the constructor reads.  Used by Ov072_MissionStart.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv072XigbarLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov072_020ba604 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv072XigbarLiE2PackPath,
    { 3, 0, 0, 0 },
};

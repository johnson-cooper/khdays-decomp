/* PS2: mechanically prepared copy of src/overlays/players/ov040_player_marluxia/data/ov040_ctorargs_020b4a44.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov040 constructor argument block data_ov040_020b4a44, 0x020b4a44-0x020b4a58 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/ma/li_e2.p.z') and four parameters the constructor reads.  Used by Ov040_SetupSequenceSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv040MarluxiaLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov040_020b4a44 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv040MarluxiaLiE2PackPath,
    { 1, 0, 0, 0 },
};

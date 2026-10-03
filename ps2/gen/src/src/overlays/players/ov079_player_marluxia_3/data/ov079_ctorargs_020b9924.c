/* PS2: mechanically prepared copy of src/overlays/players/ov079_player_marluxia_3/data/ov079_ctorargs_020b9924.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov079 constructor argument block data_ov079_020b9924, 0x020b9924-0x020b9938 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/ma/li_e2.p.z') and four parameters the constructor reads.  Used by Ov079_SetupSequenceSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv079MarluxiaLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov079_020b9924 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv079MarluxiaLiE2PackPath,
    { 1, 0, 0, 0 },
};

/* PS2: mechanically prepared copy of src/overlays/players/ov059_player_marluxia_2/data/ov059_ctorargs_020b7244.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov059 constructor argument block data_ov059_020b7244, 0x020b7244-0x020b7258 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/ma/li_e2.p.z') and four parameters the constructor reads.  Used by Ov059_SetupSequenceSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv059MarluxiaLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov059_020b7244 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv059MarluxiaLiE2PackPath,
    { 1, 0, 0, 0 },
};

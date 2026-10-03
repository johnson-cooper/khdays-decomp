/* PS2: mechanically prepared copy of src/overlays/players/ov063_player_xion_2/data/ov063_ctorargs_020b7cd4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov063 constructor argument block data_ov063_020b7cd4, 0x020b7cd4-0x020b7ce8 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/ro/li_e3.p.z') and four parameters the constructor reads.  Used by Ov063_initStateSlotsDispatch.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv063RoxasLiE3PackPath;  /* the path string */

const ClassCtorArgs data_ov063_020b7cd4 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv063RoxasLiE3PackPath,
    { 3, 0, 0, 0 },
};

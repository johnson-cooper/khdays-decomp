/* PS2: mechanically prepared copy of src/overlays/players/ov044_player_xion/data/ov044_ctorargs_020b54d4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov044 constructor argument block data_ov044_020b54d4, 0x020b54d4-0x020b54e8 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/ro/li_e3.p.z') and four parameters the constructor reads.  Used by Ov044_initStateSlotsDispatch.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv044RoxasLiE3PackPath;  /* the path string */

const ClassCtorArgs data_ov044_020b54d4 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv044RoxasLiE3PackPath,
    { 3, 0, 0, 0 },
};

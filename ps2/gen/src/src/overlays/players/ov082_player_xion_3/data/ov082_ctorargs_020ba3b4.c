/* PS2: mechanically prepared copy of src/overlays/players/ov082_player_xion_3/data/ov082_ctorargs_020ba3b4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov082 constructor argument block data_ov082_020ba3b4, 0x020ba3b4-0x020ba3c8 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/ro/li_e3.p.z') and four parameters the constructor reads.  Used by Ov082_initStateSlotsDispatch.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv082RoxasLiE3PackPath;  /* the path string */

const ClassCtorArgs data_ov082_020ba3b4 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv082RoxasLiE3PackPath,
    { 3, 0, 0, 0 },
};

/* PS2: mechanically prepared copy of src/overlays/players/ov038_player_lexaeus/data/ov038_ctorargs_020b4b8c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov038 constructor argument block data_ov038_020b4b8c, 0x020b4b8c-0x020b4ba0 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/le/li_e3.p.z') and four parameters the constructor reads.  Used by Ov038_ResetSequences.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv038LexaeusLiE3PackPath;  /* the path string */

const ClassCtorArgs data_ov038_020b4b8c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv038LexaeusLiE3PackPath,
    { 3, 0, 0, 0 },
};

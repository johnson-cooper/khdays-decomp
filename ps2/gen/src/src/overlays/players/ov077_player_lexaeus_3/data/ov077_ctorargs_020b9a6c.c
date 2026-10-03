/* PS2: mechanically prepared copy of src/overlays/players/ov077_player_lexaeus_3/data/ov077_ctorargs_020b9a6c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov077 constructor argument block data_ov077_020b9a6c, 0x020b9a6c-0x020b9a80 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/le/li_e3.p.z') and four parameters the constructor reads.  Used by Ov077_InitSceneResources.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv077LexaeusLiE3PackPath;  /* the path string */

const ClassCtorArgs data_ov077_020b9a6c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv077LexaeusLiE3PackPath,
    { 3, 0, 0, 0 },
};

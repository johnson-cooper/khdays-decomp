/* PS2: mechanically prepared copy of src/overlays/players/ov094_player_lexaeus_4/data/ov094_ctorargs_020bc12c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov094 constructor argument block data_ov094_020bc12c, 0x020bc12c-0x020bc140 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/le/li_e3.p.z') and four parameters the constructor reads.  Used by Ov094_ResetSequences.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv094LexaeusLiE3PackPath;  /* the path string */

const ClassCtorArgs data_ov094_020bc12c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv094LexaeusLiE3PackPath,
    { 3, 0, 0, 0 },
};

/* PS2: mechanically prepared copy of src/overlays/players/ov039_player_luxord/data/ov039_ctorargs_020b539c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov039 constructor argument block data_ov039_020b539c, 0x020b539c-0x020b53b0 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/lu/li_e1.p.z') and four parameters the constructor reads.  Used by Ov039_SetupSequenceSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv039LuxordLiE1PackPath;  /* the path string */

const ClassCtorArgs data_ov039_020b539c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv039LuxordLiE1PackPath,
    { 2, 0, 0, 0 },
};

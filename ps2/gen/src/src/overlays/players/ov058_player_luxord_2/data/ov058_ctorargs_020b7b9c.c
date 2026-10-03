/* PS2: mechanically prepared copy of src/overlays/players/ov058_player_luxord_2/data/ov058_ctorargs_020b7b9c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov058 constructor argument block data_ov058_020b7b9c, 0x020b7b9c-0x020b7bb0 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/lu/li_e1.p.z') and four parameters the constructor reads.  Used by Ov058_SetupSequenceSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv058LuxordLiE1PackPath;  /* the path string */

const ClassCtorArgs data_ov058_020b7b9c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv058LuxordLiE1PackPath,
    { 2, 0, 0, 0 },
};

/* PS2: mechanically prepared copy of src/overlays/players/ov095_player_luxord_4/data/ov095_ctorargs_020bc93c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov095 constructor argument block data_ov095_020bc93c, 0x020bc93c-0x020bc950 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/lu/li_e1.p.z') and four parameters the constructor reads.  Used by Ov095_SetupSequenceSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv095LuxordLiE1PackPath;  /* the path string */

const ClassCtorArgs data_ov095_020bc93c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv095LuxordLiE1PackPath,
    { 2, 0, 0, 0 },
};

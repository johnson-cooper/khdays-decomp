/* PS2: mechanically prepared copy of src/overlays/players/ov078_player_luxord_3/data/ov078_ctorargs_020ba27c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov078 constructor argument block data_ov078_020ba27c, 0x020ba27c-0x020ba290 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/lu/li_e1.p.z') and four parameters the constructor reads.  Used by Ov078_SetupSequenceSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv078LuxordLiE1PackPath;  /* the path string */

const ClassCtorArgs data_ov078_020ba27c __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv078LuxordLiE1PackPath,
    { 2, 0, 0, 0 },
};

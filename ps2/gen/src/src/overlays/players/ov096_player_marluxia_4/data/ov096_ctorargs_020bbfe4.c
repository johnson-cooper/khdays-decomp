/* PS2: mechanically prepared copy of src/overlays/players/ov096_player_marluxia_4/data/ov096_ctorargs_020bbfe4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov096 constructor argument block data_ov096_020bbfe4, 0x020bbfe4-0x020bbff8 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/ma/li_e2.p.z') and four parameters the constructor reads.  Used by Ov096_SetupSequenceSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv096MarluxiaLiE2PackPath;  /* the path string */

const ClassCtorArgs data_ov096_020bbfe4 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv096MarluxiaLiE2PackPath,
    { 1, 0, 0, 0 },
};

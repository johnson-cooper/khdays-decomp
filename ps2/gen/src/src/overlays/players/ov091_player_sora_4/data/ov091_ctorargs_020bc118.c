/* PS2: mechanically prepared copy of src/overlays/players/ov091_player_sora_4/data/ov091_ctorargs_020bc118.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov091 constructor argument block data_ov091_020bc118, 0x020bc118-0x020bc12c (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/so/li_e1.p.z') and four parameters the constructor reads.  Used by Ov091_InitEffectSlotsWithTimings.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv091SoraLiE1PackPath;  /* the path string */

const ClassCtorArgs data_ov091_020bc118 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv091SoraLiE1PackPath,
    { 1, 0, 0, 0 },
};

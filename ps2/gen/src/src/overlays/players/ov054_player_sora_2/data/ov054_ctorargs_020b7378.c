/* PS2: mechanically prepared copy of src/overlays/players/ov054_player_sora_2/data/ov054_ctorargs_020b7378.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov054 constructor argument block data_ov054_020b7378, 0x020b7378-0x020b738c (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/so/li_e1.p.z') and four parameters the constructor reads.  Used by Ov054_InitEffectSlotsWithTimings.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv054SoraLiE1PackPath;  /* the path string */

const ClassCtorArgs data_ov054_020b7378 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv054SoraLiE1PackPath,
    { 1, 0, 0, 0 },
};

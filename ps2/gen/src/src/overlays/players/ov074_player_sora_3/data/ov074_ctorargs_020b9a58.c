/* PS2: mechanically prepared copy of src/overlays/players/ov074_player_sora_3/data/ov074_ctorargs_020b9a58.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov074 constructor argument block data_ov074_020b9a58, 0x020b9a58-0x020b9a6c (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ch/so/li_e1.p.z') and four parameters the constructor reads.  Used by Ov074_InitEffectSlotsWithTimings.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv074SoraLiE1PackPath;  /* the path string */

const ClassCtorArgs data_ov074_020b9a58 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv074SoraLiE1PackPath,
    { 1, 0, 0, 0 },
};

/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_ctorargs_020b24fc.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov022 constructor argument block data_ov022_020b24fc, 0x020b24fc-0x020b2510 (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('/ba/ef/sh.p.z') and four parameters the constructor reads.  Used by Ov022_SetUpDustEffect.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv022BaEfShPackPath;  /* the path string */

const ClassCtorArgs data_ov022_020b24fc __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv022BaEfShPackPath,
    { 5, 0, 0, 0 },
};

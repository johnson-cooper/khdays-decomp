/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/data/ov022_ctorargs_020b2618.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov022 constructor argument block data_ov022_020b2618, 0x020b2618-0x020b262c (.rodata).
 *
 * Five words handed to a class constructor: the resource path of the object's archive
 * ('ba/ef/appe.p.z') and four parameters the constructor reads.  Used by Ov022_BuildEffectSlots.
 */

typedef struct ClassCtorArgs {
    const char *pszResource;  /* 0x00 */
    int aParam[4];            /* 0x04 */
} ClassCtorArgs;

extern char gOv022BaEfAppePackPath;  /* the path string */

const ClassCtorArgs data_ov022_020b2618 __attribute__((aligned(__alignof__(ClassCtorArgs)))) = {
    &gOv022BaEfAppePackPath,
    { 1, 0, 0, 5 },
};

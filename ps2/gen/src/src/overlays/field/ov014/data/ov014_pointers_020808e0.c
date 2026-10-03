/* PS2: mechanically prepared copy of src/overlays/field/ov014/data/ov014_pointers_020808e0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov014 .data pointer tables, 0x020808e0-0x02080920.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov014_VmCmd0664(void);
extern void Ov014_MarshalFxAndDispatch4(void);
extern void Ov014_VmCmd07c4(void);
extern void Ov014_MarshalFxAndDispatch(void);

Ov_Fn data_ov014_020808e0[16] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov014_VmCmd0664,

    0,

    Ov014_MarshalFxAndDispatch4,

    0,

    0,

    0,

    0,

    0,

    Ov014_VmCmd07c4,

    0,

    Ov014_MarshalFxAndDispatch,

    0,

    0,

    0,

    0,

    0,

};

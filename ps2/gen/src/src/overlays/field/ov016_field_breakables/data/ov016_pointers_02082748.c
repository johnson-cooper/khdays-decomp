/* PS2: mechanically prepared copy of src/overlays/field/ov016_field_breakables/data/ov016_pointers_02082748.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov016 .data pointer tables, 0x02082748-0x0208274c.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov016_KickableStep(void);

Ov_Fn data_ov016_02082748[1] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov016_KickableStep,

};

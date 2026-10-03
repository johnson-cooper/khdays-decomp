/* PS2: mechanically prepared copy of src/engine/data/main_pointers_02041eac.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .rodata pointer tables, 0x02041eac-0x02041ecc.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Bg_SetMainBg2ExtControl(void);
extern void Bg_SetMainBg3ExtControl(void);
extern void Bg_SetSubBg2ExtControl(void);
extern void Bg_SetSubBg3ExtControl(void);

const Ov_Fn data_02041eac[8] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    0,

    Bg_SetMainBg2ExtControl,

    Bg_SetMainBg3ExtControl,

    0,

    0,

    Bg_SetSubBg2ExtControl,

    Bg_SetSubBg3ExtControl,

};

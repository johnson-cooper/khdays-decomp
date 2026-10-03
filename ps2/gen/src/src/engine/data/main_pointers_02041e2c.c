/* PS2: mechanically prepared copy of src/engine/data/main_pointers_02041e2c.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .rodata pointer tables, 0x02041e2c-0x02041e6c.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Bg_SetMainBg0Control(void);
extern void Bg_SetMainBg1Control(void);
extern void Bg_SetMainBg2TextControl(void);
extern void Bg_SetMainBg3TextControl(void);
extern void Bg_SetMainBg2AffineControl(void);
extern void Bg_SetMainBg3AffineControl(void);
extern void Bg_SetSubBg0Control(void);
extern void Bg_SetSubBg1Control(void);
extern void Bg_SetSubBg2TextControl(void);
extern void Bg_SetSubBg3TextControl(void);
extern void Bg_SetSubBg2AffineControl(void);
extern void Bg_SetSubBg3AffineControl(void);

const Ov_Fn data_02041e2c[8] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    0,

    Bg_SetMainBg2AffineControl,

    Bg_SetMainBg3AffineControl,

    0,

    0,

    Bg_SetSubBg2AffineControl,

    Bg_SetSubBg3AffineControl,

};

const Ov_Fn data_02041e4c[8] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Bg_SetMainBg0Control,

    Bg_SetMainBg1Control,

    Bg_SetMainBg2TextControl,

    Bg_SetMainBg3TextControl,

    Bg_SetSubBg0Control,

    Bg_SetSubBg1Control,

    Bg_SetSubBg2TextControl,

    Bg_SetSubBg3TextControl,

};

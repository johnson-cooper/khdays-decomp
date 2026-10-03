/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_pointers_0207e378.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 .rodata pointer tables, 0x0207e378-0x0207e388.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov002_OptionsActionNoOp(void);
extern void Ov002_StartCaptionVoice(void);
extern void Ov002_StartSlideOut(void);
extern void Ov002_RedrawOptionsPage(void);

const Ov_Fn data_ov002_0207e378[4] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov002_OptionsActionNoOp,

    Ov002_StartCaptionVoice,

    Ov002_StartSlideOut,

    Ov002_RedrawOptionsPage,

};

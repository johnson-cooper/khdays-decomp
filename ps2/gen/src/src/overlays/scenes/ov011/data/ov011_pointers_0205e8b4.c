/* PS2: mechanically prepared copy of src/overlays/scenes/ov011/data/ov011_pointers_0205e8b4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov011 .data pointer tables, 0x0205e8b4-0x0205e8e4.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov011_RunSetupThenInvokeIfState3(void);
extern void Ov011_UpdateTitleFrame(void);
extern void Ov011_TitleStateNoOp(void);
extern void Ov011_TickTitleFadeOut(void);

Ov_Fn data_ov011_0205e8b4[6] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov011_RunSetupThenInvokeIfState3,

    Ov011_UpdateTitleFrame,

    Ov011_RunSetupThenInvokeIfState3,

    Ov011_RunSetupThenInvokeIfState3,

    Ov011_TickTitleFadeOut,

    Ov011_TitleStateNoOp,

};

Ov_Fn data_ov011_0205e8cc[6] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov011_RunSetupThenInvokeIfState3,

    Ov011_UpdateTitleFrame,

    Ov011_RunSetupThenInvokeIfState3,

    Ov011_RunSetupThenInvokeIfState3,

    Ov011_TickTitleFadeOut,

    Ov011_TitleStateNoOp,

};

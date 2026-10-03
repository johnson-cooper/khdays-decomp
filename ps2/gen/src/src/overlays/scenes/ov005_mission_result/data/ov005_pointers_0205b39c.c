/* PS2: mechanically prepared copy of src/overlays/scenes/ov005_mission_result/data/ov005_pointers_0205b39c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov005 .rodata pointer tables, 0x0205b39c-0x0205b3bc.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern void Ov005_FadeInResultScreen(void);
extern void Ov005_AnimateResultCounters(void);
extern void Ov005_LatchTickState3(void);
extern void Ov005_FadeOutResultScreen(void);
extern void Ov005_SetFlag4B78(void);
extern int gOv005UiCmMslPath;

void *const data_ov005_0205b39c[5] __attribute__((aligned(__alignof__(void *)))) = {

    (void *)Ov005_FadeInResultScreen,

    (void *)Ov005_AnimateResultCounters,

    (void *)Ov005_LatchTickState3,

    (void *)Ov005_FadeOutResultScreen,

    (void *)Ov005_SetFlag4B78,

};

void *const data_ov005_0205b3b0[3] __attribute__((aligned(__alignof__(void *)))) = {

    &gOv005UiCmMslPath,

    0,

    0,

};

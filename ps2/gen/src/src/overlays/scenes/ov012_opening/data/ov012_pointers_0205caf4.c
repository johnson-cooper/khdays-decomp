/* PS2: mechanically prepared copy of src/overlays/scenes/ov012_opening/data/ov012_pointers_0205caf4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov012 .data pointer tables, 0x0205caf4-0x0205cb20.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov012_StartOpeningMovieFromScriptArgs(void);
extern void Ov012_RegisterTwoAndReturn6(void);
extern void Ov012_IsStreamFinished(void);
extern void Ov012_MayWaitFrames(void);
extern void Ov012_InitAndDispatchTriple(void);
extern void Ov012_thumbStep(void);
extern void Ov012_thumbStep_2(void);

Ov_Fn data_ov012_0205caf4[11] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov012_RegisterTwoAndReturn6,

    0,

    Ov012_InitAndDispatchTriple,

    0,

    Ov012_StartOpeningMovieFromScriptArgs,

    0,

    Ov012_IsStreamFinished,

    Ov012_MayWaitFrames,

    Ov012_thumbStep,

    Ov012_thumbStep_2,

    0,

};

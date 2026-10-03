/* PS2: mechanically prepared copy of src/overlays/scenes/ov000_title/data/ov000_pointers_0205a710.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov000 .rodata pointer tables, 0x0205a710-0x0205a734.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov000_TickFadeOutFromCtxTimer(void);
extern void Ov000_TickFadeThenEnterState2(void);
extern void Ov000_HandleSaveSlotInput(void);
extern void Ov000_HandleLoadConfirm(void);
extern void Ov000_PollSaveCheckState(void);
extern void Ov000_TickMarkerMenuInput(void);
extern void Ov000_BeginSaveCheck(void);
extern void Ov000_LatchTickState7(void);
extern void Ov000_MarkerMenuStateNoOp(void);

const Ov_Fn data_ov000_0205a710[9] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov000_TickFadeOutFromCtxTimer,

    Ov000_TickFadeThenEnterState2,

    Ov000_HandleSaveSlotInput,

    Ov000_HandleLoadConfirm,

    Ov000_PollSaveCheckState,

    Ov000_TickMarkerMenuInput,

    Ov000_BeginSaveCheck,

    Ov000_LatchTickState7,

    Ov000_MarkerMenuStateNoOp,

};

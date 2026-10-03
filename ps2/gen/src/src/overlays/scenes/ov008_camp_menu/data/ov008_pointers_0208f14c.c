/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/data/ov008_pointers_0208f14c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov008 .rodata pointer tables, 0x0208f14c-0x0208f15c.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov008_FinishMenuModeSwitch(void);
extern void Ov008_UpdateReadyState(void);
extern void Ov008_TryAcquire14ThenClearField4(void);

const Ov_Fn data_ov008_0208f14c[4] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov008_FinishMenuModeSwitch,

    Ov008_UpdateReadyState,

    Ov008_TryAcquire14ThenClearField4,

};

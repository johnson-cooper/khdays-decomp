/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/data/ov025_pointers_020b3cac.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov025 .rodata pointer tables, 0x020b3cac-0x020b3cbc.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov025_FinishMenuModeSwitch(void);
extern void Ov025_UpdateReadyState(void);
extern void Ov025_TryAcquire14ThenClearField4(void);

const Ov_Fn data_ov025_020b3cac[4] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov025_FinishMenuModeSwitch,

    Ov025_UpdateReadyState,

    Ov025_TryAcquire14ThenClearField4,

};

/* PS2: mechanically prepared copy of src/overlays/scenes/ov004_calendar/data/ov004_pointers_020510b8.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov004 .rodata pointer tables, 0x020510b8-0x020510cc.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov004_FadeInTransition(void);
extern void Ov004_UpdateRollingTransition(void);
extern void Ov004_RunDelayedProtectionChecks(void);
extern void Ov004_FadeOutTransition(void);
extern void Ov004_MarkTransitionComplete(void);

const Ov_Fn data_ov004_020510b8[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov004_FadeInTransition,

    Ov004_UpdateRollingTransition,

    Ov004_RunDelayedProtectionChecks,

    Ov004_FadeOutTransition,

    Ov004_MarkTransitionComplete,

};

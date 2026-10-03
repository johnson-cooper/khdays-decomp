/* PS2: mechanically prepared copy of src/overlays/players/ov092_player_demyx_4/data/ov092_pointers_020bc3f4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov092 .data pointer tables, 0x020bc3f4-0x020bc408.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov092_BeginHitEffectIfEnabled(void);
extern void Ov092_PartAttackStep(void);

Ov_Fn data_ov092_020bc3f4[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov092_BeginHitEffectIfEnabled,

    Ov022_NullStep,

    Ov092_PartAttackStep,

};

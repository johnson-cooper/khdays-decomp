/* PS2: mechanically prepared copy of src/overlays/players/ov036_player_demyx/data/ov036_pointers_020b4e54.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov036 .data pointer tables, 0x020b4e54-0x020b4e68.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov036_BeginHitEffectIfEnabled(void);
extern void Ov036_PartAttackStep(void);

Ov_Fn data_ov036_020b4e54[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov036_BeginHitEffectIfEnabled,

    Ov022_NullStep,

    Ov036_PartAttackStep,

};

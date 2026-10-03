/* PS2: mechanically prepared copy of src/overlays/players/ov075_player_demyx_3/data/ov075_pointers_020b9d34.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov075 .data pointer tables, 0x020b9d34-0x020b9d48.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov075_BeginHitEffectIfEnabled(void);
extern void Ov075_PartAttackStep(void);

Ov_Fn data_ov075_020b9d34[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov075_BeginHitEffectIfEnabled,

    Ov022_NullStep,

    Ov075_PartAttackStep,

};

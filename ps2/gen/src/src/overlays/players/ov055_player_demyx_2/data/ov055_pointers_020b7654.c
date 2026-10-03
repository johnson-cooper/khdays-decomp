/* PS2: mechanically prepared copy of src/overlays/players/ov055_player_demyx_2/data/ov055_pointers_020b7654.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov055 .data pointer tables, 0x020b7654-0x020b7668.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov055_BeginHitEffectIfEnabled(void);
extern void Ov055_PartAttackStep(void);

Ov_Fn data_ov055_020b7654[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov055_BeginHitEffectIfEnabled,

    Ov022_NullStep,

    Ov055_PartAttackStep,

};

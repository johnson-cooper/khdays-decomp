/* PS2: mechanically prepared copy of src/overlays/players/ov039_player_luxord/data/ov039_pointers_020b5568.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov039 .data pointer tables, 0x020b5568-0x020b557c.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov039_SetMode4Delay3000(void);
extern void Ov039_TickThrowArcWithLimit(void);

Ov_Fn data_ov039_020b5568[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov039_SetMode4Delay3000,

    Ov022_NullStep,

    Ov039_TickThrowArcWithLimit,

};

/* PS2: mechanically prepared copy of src/overlays/players/ov078_player_luxord_3/data/ov078_pointers_020ba448.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov078 .data pointer tables, 0x020ba448-0x020ba45c.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov078_SetMode4Delay3000(void);
extern void Ov078_TickThrowArcWithLimit(void);

Ov_Fn data_ov078_020ba448[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov078_SetMode4Delay3000,

    Ov022_NullStep,

    Ov078_TickThrowArcWithLimit,

};

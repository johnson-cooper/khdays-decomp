/* PS2: mechanically prepared copy of src/overlays/players/ov062_player_xemnas_2/data/ov062_pointers_020b8014.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov062 .data pointer tables, 0x020b8014-0x020b8028.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov062_UpdateSwingRequestEffect(void);

Ov_Fn data_ov062_020b8014[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov062_UpdateSwingRequestEffect,

    Ov022_NullStep,

    0,

};

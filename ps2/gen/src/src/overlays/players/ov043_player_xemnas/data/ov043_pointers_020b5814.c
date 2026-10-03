/* PS2: mechanically prepared copy of src/overlays/players/ov043_player_xemnas/data/ov043_pointers_020b5814.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov043 .data pointer tables, 0x020b5814-0x020b5828.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov022_AdvanceTimerAndLatchDone(void);
extern void Ov022_NullStep(void);
extern void Ov043_UpdateSwingRequestEffect(void);

Ov_Fn data_ov043_020b5814[5] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    Ov022_AdvanceTimerAndLatchDone,

    Ov043_UpdateSwingRequestEffect,

    Ov022_NullStep,

    0,

};

/* PS2: mechanically prepared copy of src/overlays/field/ov016_field_breakables/data/ov016_pointers_020826e0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov016 .data pointer tables, 0x020826e0-0x02082740.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov016_VmCmdCreateEntry(void);
extern void Ov016_ScriptOpCreateLift(void);
extern void Ov016_InitTripleAndDispatch(void);
extern void Ov016_ScriptOpCreateFollower(void);
extern void Ov016_VmCmd1200(void);
extern void Ov016_ScriptOpCreateBreakable(void);
extern void Ov016_VmCmdCreateEntryClass80(void);
extern void Ov016_ScriptOpCreateKickable(void);
extern void Ov016_VmCmd14b0(void);
extern void Ov016_ScriptOpCreateHazard(void);

Ov_Fn data_ov016_020826e0[24] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    0,

    0,

    0,

    0,

    Ov016_VmCmdCreateEntry,

    0,

    Ov016_ScriptOpCreateLift,

    0,

    Ov016_InitTripleAndDispatch,

    0,

    Ov016_ScriptOpCreateFollower,

    0,

    Ov016_VmCmd1200,

    0,

    Ov016_ScriptOpCreateBreakable,

    0,

    Ov016_VmCmdCreateEntryClass80,

    0,

    Ov016_ScriptOpCreateKickable,

    0,

    Ov016_VmCmd14b0,

    0,

    Ov016_ScriptOpCreateHazard,

    0,

};

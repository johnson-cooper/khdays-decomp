/* PS2: mechanically prepared copy of src/overlays/field/ov017_field_deposits/data/ov017_pointers_02080e40.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov017 .data pointer tables, 0x02080e40-0x02080ea0.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov017_MarshalAndDispatch(void);
extern void Ov017_MarshalFxAndDispatch2(void);
extern void Ov017_MarshalFxAndDispatch(void);
extern void Ov017_ScriptOpRetirePiece(void);
extern void Ov017_SetFlagBit62OnFour(void);
extern void Ov017_VmCmd0cb4(void);
extern void Ov017_MarshalFxAndDispatch3(void);
extern void Ov017_RegisterPairAndDispatch(void);
extern void Ov017_ScriptOpPostScoreAndClearBgPriority(void);

Ov_Fn data_ov017_02080e40[24] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov017_MarshalAndDispatch,

    0,

    Ov017_MarshalFxAndDispatch2,

    0,

    Ov017_MarshalFxAndDispatch,

    0,

    Ov017_ScriptOpRetirePiece,

    0,

    Ov017_SetFlagBit62OnFour,

    0,

    Ov017_VmCmd0cb4,

    0,

    Ov017_MarshalFxAndDispatch3,

    0,

    Ov017_RegisterPairAndDispatch,

    0,

    Ov017_ScriptOpPostScoreAndClearBgPriority,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

};

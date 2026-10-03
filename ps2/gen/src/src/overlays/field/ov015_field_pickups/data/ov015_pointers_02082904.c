/* PS2: mechanically prepared copy of src/overlays/field/ov015_field_pickups/data/ov015_pointers_02082904.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov015 .data pointer tables, 0x02082904-0x02082960.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov015_VmCmd22b0(void);
extern void Ov015_ScriptOpSpawnPickup(void);
extern void Ov015_ScriptOpBuildAndPublish(void);
extern void Ov015_ScriptOpSpawnTrigger(void);
extern void Ov015_ScriptOpCreateSpots(void);
extern void Ov015_ScriptOpSpawnPoint(void);
extern void Ov015_VmCmd27a0(void);
extern void Ov015_MarshalFxAndDispatch2(void);

Ov_Fn data_ov015_02082904[23] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov015_VmCmd22b0,

    0,

    Ov015_ScriptOpSpawnPickup,

    0,

    Ov015_ScriptOpBuildAndPublish,

    0,

    Ov015_ScriptOpSpawnTrigger,

    0,

    Ov015_ScriptOpCreateSpots,

    0,

    Ov015_ScriptOpSpawnPoint,

    0,

    Ov015_VmCmd27a0,

    0,

    Ov015_MarshalFxAndDispatch2,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

};

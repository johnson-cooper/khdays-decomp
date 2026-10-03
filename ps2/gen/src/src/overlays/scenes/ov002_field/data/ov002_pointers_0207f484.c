/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_pointers_0207f484.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov002 .data pointer tables, 0x0207f484-0x0207f58c.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov002_VmCmd7ceac(void);
extern void Ov002_ScriptCmdSpawnElement(void);
extern void Ov002_VmCmd7d004(void);
extern void Ov002_ScriptCmdSpawnTimedElement(void);
extern void Ov002_ConstReturn1_4(void);
extern void Ov002_VmCmd7d18c(void);
extern void Ov002_ScriptCmdSpawnActorElement(void);
extern void Ov002_ConstReturn1_5(void);
extern void Ov002_ConstReturn1_6(void);
extern void Ov002_VmCmd7d334(void);
extern void Ov002_ScriptCmdSpawnPlacedPiece(void);
extern void Ov002_ScriptDriveWidget(void);
extern void Ov002_CmdCreateModuleSlot(void);
extern void Ov002_ScriptCmdSpawnSpareEntry(void);
extern void Ov002_VmCmdList7d610(void);
extern void Ov002_ScriptCmdSpawnLineElement(void);
extern void Ov002_ScriptCmd_RetireAllEntries(void);
extern void Ov002_ScriptCmd_StoreIfFree(void);
extern void Ov002_ScriptIsEntryFree(void);
extern void Ov002_ScriptCmdInvokeObjectCallback48(void);
extern void Ov002_ScriptCmdPublishSpawnRequest(void);
extern void Ov002_ScriptCmd_NotifyNodesOfKind(void);
extern void Ov002_ScriptCmd_SwitchPanelOverlay(void);
extern void Ov002_VmCmd7d950(void);
extern void Ov002_ScriptCmdSpawnTravelElement(void);
extern void Ov002_ScriptCmd_ClearListTable(void);
extern void Ov002_VmSetEntryValue(void);
extern void Ov002_ScriptCmd_SetListRowMode(void);

Ov_Fn data_ov002_0207f484[66] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov002_VmCmd7ceac,

    0,

    Ov002_ScriptCmdSpawnElement,

    0,

    Ov002_VmCmd7d004,

    0,

    Ov002_ScriptCmdSpawnTimedElement,

    0,

    Ov002_ConstReturn1_4,

    0,

    Ov002_VmSetEntryValue,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    Ov002_VmCmd7d18c,

    0,

    Ov002_ScriptCmdSpawnActorElement,

    0,

    Ov002_ConstReturn1_5,

    0,

    Ov002_ConstReturn1_6,

    0,

    Ov002_VmCmd7d334,

    0,

    Ov002_ScriptCmdSpawnPlacedPiece,

    0,

    Ov002_ScriptDriveWidget,

    0,

    Ov002_CmdCreateModuleSlot,

    0,

    Ov002_ScriptCmdSpawnSpareEntry,

    0,

    Ov002_VmCmdList7d610,

    0,

    Ov002_ScriptCmdSpawnLineElement,

    0,

    Ov002_ScriptCmd_RetireAllEntries,

    0,

    Ov002_ScriptCmd_StoreIfFree,

    Ov002_ScriptIsEntryFree,

    Ov002_ScriptCmdInvokeObjectCallback48,

    0,

    Ov002_ScriptCmdPublishSpawnRequest,

    0,

    Ov002_ScriptCmd_NotifyNodesOfKind,

    0,

    Ov002_ScriptCmd_SwitchPanelOverlay,

    0,

    Ov002_VmCmd7d950,

    0,

    Ov002_ScriptCmdSpawnTravelElement,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    Ov002_ScriptCmd_ClearListTable,

    0,

    Ov002_ScriptCmd_SetListRowMode,

    0,

};

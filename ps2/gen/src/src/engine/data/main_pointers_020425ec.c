/* PS2: mechanically prepared copy of src/engine/data/main_pointers_020425ec.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .data pointer tables, 0x020425ec-0x02042730.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern void Game_ActionAssign(void);
extern void ScriptCmd_SetElemOffset(void);
extern void Game_ActionLoadArenaResource(void);
extern void Game_ActionInitObject(void);
extern void Game_ActionReturn4(void);
extern void ResetRequestRecord(void);
extern void CommitRecordCursor(void);
extern void ScriptVm_RunCallback(void);
extern void IsDerefZero(void);
extern void ConstReturn0(void);
extern void ConstReturn0_2(void);
extern void Game_ActionSetBranchTarget(void);
extern void ScriptCmd_StoreDoubled(void);
extern void DeferredAction_Gate(void);
extern void ScriptCmd_ArmPair(void);
extern void Game_ActionSetupNamedCall(void);
extern void CommitCachedByteIfChanged(void);
extern void ScriptCmd_RequestKind3(void);
extern void Game_ActionEnqueueCmdIfChanged(void);
extern void RegisterActorOrRetry(void);
extern void ScriptCmd_SetSelection(void);
extern void ScriptCmd_QueueSoundKind1(void);
extern void Script_StepAndIsIdle(void);
extern void ScriptCmd_PlaySound(void);
extern void ScriptCmd_ForwardThreeToHandler(void);
extern void ScriptCmd_DispatchToHandler(void);
extern void ScriptCmd_RequestResPair(void);
extern void ScriptCmd_StartStream(void);
extern void QueryAndRegisterNode(void);
extern void ScriptCmd_FreeSlotEntry(void);
extern void Script_Cmd_PlayEntityCutsceneCamWait(void);
extern void Game_ActionTurnHandler(void);
extern void Script_Cmd_PlayEntityCutsceneCam(void);
extern void DispatchTrackEntryIfReady(void);
extern void *data_02042640[60];

void *data_020425ec[21] __attribute__((aligned(__alignof__(void *)))) = {

    data_02042640,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

};

void *data_02042640[60] __attribute__((aligned(__alignof__(void *)))) = {

    (void *)Game_ActionAssign,

    0,

    (void *)ScriptCmd_SetElemOffset,

    0,

    (void *)Game_ActionLoadArenaResource,

    (void *)Game_ActionInitObject,

    (void *)Game_ActionReturn4,

    0,

    (void *)ResetRequestRecord,

    0,

    (void *)CommitRecordCursor,

    0,

    0,

    0,

    0,

    0,

    (void *)ScriptVm_RunCallback,

    (void *)IsDerefZero,

    (void *)ConstReturn0,

    (void *)ConstReturn0_2,

    (void *)Game_ActionSetBranchTarget,

    0,

    (void *)ScriptCmd_StoreDoubled,

    (void *)DeferredAction_Gate,

    (void *)ScriptCmd_ArmPair,

    0,

    (void *)Game_ActionSetupNamedCall,

    0,

    (void *)CommitCachedByteIfChanged,

    0,

    (void *)ScriptCmd_RequestKind3,

    0,

    (void *)Game_ActionEnqueueCmdIfChanged,

    0,

    (void *)RegisterActorOrRetry,

    (void *)ScriptCmd_SetSelection,

    (void *)ScriptCmd_QueueSoundKind1,

    (void *)Script_StepAndIsIdle,

    0,

    0,

    (void *)ScriptCmd_PlaySound,

    0,

    (void *)ScriptCmd_ForwardThreeToHandler,

    0,

    (void *)ScriptCmd_DispatchToHandler,

    0,

    (void *)ScriptCmd_RequestResPair,

    0,

    (void *)ScriptCmd_StartStream,

    0,

    (void *)QueryAndRegisterNode,

    0,

    (void *)ScriptCmd_FreeSlotEntry,

    0,

    (void *)Script_Cmd_PlayEntityCutsceneCamWait,

    (void *)Game_ActionTurnHandler,

    (void *)Script_Cmd_PlayEntityCutsceneCam,

    (void *)DispatchTrackEntryIfReady,

    0,

    0,

};

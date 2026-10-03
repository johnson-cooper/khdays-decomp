/* PS2: mechanically prepared copy of src/overlays/field/ov023_event_script/data/ov023_pointers_0208a66c.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov023 .data pointer tables, 0x0208a66c-0x0208a69c.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov023_ScriptCmd_SetGateFlagWithSound(void);
extern void Ov023_CmdSetGateFlag(void);
extern void Ov023_CmdStartLightFade(void);
extern void Ov023_CmdStepLightFade(void);
extern void Ov023_CmdSetGameMode2(void);
extern void Ov023_ConstReturn0(void);
extern void Ov023_CmdSaveGame(void);
extern void Ov023_VmCmdApplyByFlag(void);

Ov_Fn data_ov023_0208a66c[12] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov023_ScriptCmd_SetGateFlagWithSound,

    0,

    Ov023_CmdSetGateFlag,

    0,

    Ov023_CmdStartLightFade,

    Ov023_CmdStepLightFade,

    Ov023_CmdSetGameMode2,

    0,

    Ov023_ConstReturn0,

    Ov023_CmdSaveGame,

    Ov023_VmCmdApplyByFlag,

    0,

};

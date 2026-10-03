/* PS2: mechanically prepared copy of src/overlays/screens/ov106/data/ov106_pointers_020b8af0.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov106 .data pointer tables, 0x020b8af0-0x020b8b20.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov106_ScriptCmd_SetGateFlagWithSound(void);
extern void Ov106_CmdSetGateFlag(void);
extern void Ov106_CmdStartLightFade(void);
extern void Ov106_CmdStepLightFade(void);
extern void Ov106_CmdSetGameMode2(void);
extern void Ov106_ConstReturn0(void);
extern void Ov106_CmdSaveGame(void);
extern void Ov106_VmCmdApplyByFlag(void);

Ov_Fn data_ov106_020b8af0[12] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov106_ScriptCmd_SetGateFlagWithSound,

    0,

    Ov106_CmdSetGateFlag,

    0,

    Ov106_CmdStartLightFade,

    Ov106_CmdStepLightFade,

    Ov106_CmdSetGameMode2,

    0,

    Ov106_ConstReturn0,

    Ov106_CmdSaveGame,

    Ov106_VmCmdApplyByFlag,

    0,

};

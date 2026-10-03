/* PS2: mechanically prepared copy of src/overlays/system/ov024_mobiclip/data/ov024_pointers_02093974.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov024 .data pointer tables, 0x02093974-0x020939ac.
 *
 * 1 table, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov024_MobiClip_ResolveStreamDescs(void);
extern void Ov024_MobiClip_OpenDescPair(void);
extern void Ov024_IsStreamFinished(void);
extern void Ov024_MayWaitFrames(void);
extern void Ov024_MobiClip_StopPlayerStream(void);
extern void Ov024_thumbStep(void);
extern void Ov024_thumbStep_2(void);
extern void Ov024_CmdPrepareStream(void);
extern void Ov024_CmdSetStreamByte(void);

Ov_Fn data_ov024_02093974[14] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov024_MobiClip_OpenDescPair,

    0,

    Ov024_MobiClip_StopPlayerStream,

    0,

    Ov024_MobiClip_ResolveStreamDescs,

    0,

    Ov024_IsStreamFinished,

    Ov024_MayWaitFrames,

    Ov024_thumbStep,

    Ov024_thumbStep_2,

    Ov024_CmdPrepareStream,

    0,

    Ov024_CmdSetStreamByte,

    0,

};

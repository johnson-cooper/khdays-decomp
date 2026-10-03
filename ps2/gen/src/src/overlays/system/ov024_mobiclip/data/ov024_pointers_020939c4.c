/* PS2: mechanically prepared copy of src/overlays/system/ov024_mobiclip/data/ov024_pointers_020939c4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov024 .data pointer tables, 0x020939c4-0x02093a20.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Ov024_MobiClip_DecoderFreeBuffers(void);
extern void Ov024_StackAllocVtblSlot0NoOp(void);
extern void Ov024_FreeStackAllocPassthrough(void);
extern void Ov024_MobiClip_StreamSeek(void);
extern void Ov024_MobiClip_StreamRead(void);
extern void Ov024_MobiClip_StreamReadAhead(void);
extern void Ov024_MobiClip_CursorWaitAsync(void);
extern void Ov024_CallVirt14(void);
extern void Ov024_MobiClip_StreamVtblSlot0NoOp(void);

Ov_Fn data_ov024_020939c4[9] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov024_StackAllocVtblSlot0NoOp,

    Ov024_FreeStackAllocPassthrough,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

};

Ov_Fn data_ov024_020939e8[14] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Ov024_MobiClip_StreamVtblSlot0NoOp,

    Ov024_MobiClip_DecoderFreeBuffers,

    Ov024_MobiClip_StreamSeek,

    Ov024_MobiClip_StreamRead,

    Ov024_MobiClip_StreamReadAhead,

    Ov024_MobiClip_CursorWaitAsync,

    Ov024_CallVirt14,

    0,

    0,

    0,

    0,

    0,

    0,

    0,

};

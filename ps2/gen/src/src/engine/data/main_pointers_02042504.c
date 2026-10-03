/* PS2: mechanically prepared copy of src/engine/data/main_pointers_02042504.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .data pointer tables, 0x02042504-0x0204252c.
 *
 * 3 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void UploadNodeMatricesByFlags(void);
extern void Node_SeedTransform(void);
extern void func_01ffa218(void);
extern void func_01ffa2dc(void);
extern void NNSi_G3dSendTexMtxMode0(void);
extern void NNSi_G3dSendTexMtxMode2(void);

Ov_Fn data_02042504[3] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    UploadNodeMatricesByFlags,

    func_01ffa218,

    0,

};

Ov_Fn data_02042510[3] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Node_SeedTransform,

    func_01ffa2dc,

    0,

};

Ov_Fn data_0204251c[4] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    NNSi_G3dSendTexMtxMode0,

    0,

    NNSi_G3dSendTexMtxMode2,

    0,

};

/* PS2: mechanically prepared copy of src/engine/data/main_pointers_02042494.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .data pointer tables, 0x02042494-0x020424b4.
 *
 * 8 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void NNSi_G3dAnmBlendMat(void);
extern void func_02015df0(void);
extern void NNSi_G3dAnmBlendVis(void);
extern void NNSi_G3dAnmCalcNsBca(void);
extern void NNSi_G3dAnmCalcNsBma(void);
extern void NNSi_G3dAnmCalcNsBta(void);
extern void NNSi_G3dAnmCalcNsBtp(void);
extern void NNSi_G3dAnmCalcNsBva(void);

Ov_Fn data_02042494[1] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    NNSi_G3dAnmCalcNsBva,

};

Ov_Fn data_02042498[1] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    NNSi_G3dAnmCalcNsBca,

};

Ov_Fn data_0204249c[1] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    NNSi_G3dAnmCalcNsBta,

};

Ov_Fn data_020424a0[1] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    NNSi_G3dAnmCalcNsBtp,

};

Ov_Fn data_020424a4[1] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    NNSi_G3dAnmCalcNsBma,

};

Ov_Fn data_020424a8[1] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    NNSi_G3dAnmBlendVis,

};

Ov_Fn data_020424ac[1] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    func_02015df0,

};

Ov_Fn data_020424b0[1] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    NNSi_G3dAnmBlendMat,

};

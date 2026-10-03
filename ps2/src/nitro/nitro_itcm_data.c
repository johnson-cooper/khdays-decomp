/* Data the DS keeps inside ITCM code, which the decomp reads through literal ITCM addresses
 * (prep_sources.py R15 points those literals here).
 *
 * NitroSystem G3D's texture-matrix builders, one per SRT flag combination (index = the material
 * animation's flag bits & 7), for the two texture matrix modes:
 *   0x01ffa1f8  used by NNSi_G3dSendTexMtxMode0
 *   0x01ffa598  used by NNSi_G3dSendTexMtxMode2
 * Entries read from the YKGP ARM9 ITCM autoload.  Two builders carry other names in the decomp
 * (NNSi_G3dCalcTexMtxRot, G3d_ComputeFrustumPlanes); what counts is the address they were
 * delinked from, which is the table entry.
 */
extern void texmtxCalc_flag_(void);
extern void NNSi_G3dCalcTexMtxRot(void);
extern void texmtxCalc_flagR_(void);
extern void texmtxCalc_flagRS_(void);
extern void texmtxCalc_flagT_(void);
extern void G3d_ComputeFrustumPlanes(void);
extern void texmtxCalc_flagTR_(void);
extern void Mtx44_SetIdentity2D(void);
extern void texmtxCalc_flag__2(void);
extern void texmtxCalc_flagS_(void);
extern void texmtxCalc_flagR__2(void);
extern void texmtxCalc_flagRS__2(void);
extern void texmtxCalc_flagT__2(void);
extern void texmtxCalc_flagTS_(void);
extern void texmtxCalc_flagTR__2(void);
extern void Mtx44_SetIdentity2D_2(void);

void (*const kh_itcm_data_01ffa1f8[8])(void) = {
    texmtxCalc_flag_,     /* 0x02019218 */
    NNSi_G3dCalcTexMtxRot, /* 0x02019320 */
    texmtxCalc_flagR_,    /* 0x020193f0 */
    texmtxCalc_flagRS_,   /* 0x0201946c */
    texmtxCalc_flagT_,    /* 0x020194b4 */
    G3d_ComputeFrustumPlanes, /* 0x02019594 */
    texmtxCalc_flagTR_,   /* 0x0201964c */
    Mtx44_SetIdentity2D,  /* 0x01ffa42c */
};

void (*const kh_itcm_data_01ffa598[8])(void) = {
    texmtxCalc_flag__2,   /* 0x02019690 */
    texmtxCalc_flagS_,    /* 0x02019798 */
    texmtxCalc_flagR__2,  /* 0x02019880 */
    texmtxCalc_flagRS__2, /* 0x02019900 */
    texmtxCalc_flagT__2,  /* 0x02019948 */
    texmtxCalc_flagTS_,   /* 0x02019a40 */
    texmtxCalc_flagTR__2, /* 0x02019b18 */
    Mtx44_SetIdentity2D_2, /* 0x01ffa5b8 */
};

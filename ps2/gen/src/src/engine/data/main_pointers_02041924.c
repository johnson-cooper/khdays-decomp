/* PS2: mechanically prepared copy of src/engine/data/main_pointers_02041924.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .rodata pointer tables, 0x02041924-0x020419c4.
 *
 * 2 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

extern void AllocatorAllocForSDKHeap(void);
extern void AllocatorFreeForSDKHeap(void);
extern void Gfd_LoadTex(void);
extern void Gfd_LoadTexPltt(void);
extern void DoTransfer2dObjExtPlttMain(void);
extern void DoTransfer2dBGExtPlttMain(void);
extern void Gfd_LoadSubObjExtPltt(void);
extern void Gfd_LoadSubBgExtPltt(void);
extern int AllocatorAllocForExpHeap;
extern int AllocatorFreeForExpHeap;
extern int GXS_LoadBG0Char;
extern int GXS_LoadBG0Scr;
extern int GXS_LoadBG1Char;
extern int GXS_LoadBG1Scr;
extern int GXS_LoadBG2Char;
extern int GXS_LoadBG2Scr;
extern int GXS_LoadBG3Char;
extern int GXS_LoadBG3Scr;
extern int GXS_LoadBGPltt;
extern int GXS_LoadOAM;
extern int GXS_LoadOBJ;
extern int GXS_LoadOBJPltt;
extern int GX_LoadBG0Char;
extern int GX_LoadBG0Scr;
extern int GX_LoadBG1Char;
extern int GX_LoadBG1Scr;
extern int GX_LoadBG2Char;
extern int GX_LoadBG2Scr;
extern int GX_LoadBG3Char;
extern int GX_LoadBG3Scr;
extern int GX_LoadBGPltt;
extern int GX_LoadOAM;
extern int GX_LoadOBJ;
extern int GX_LoadOBJPltt;

void *const data_02041924[4] __attribute__((aligned(__alignof__(void *)))) = {

    &AllocatorAllocForExpHeap,

    &AllocatorFreeForExpHeap,

    (void *)AllocatorAllocForSDKHeap,

    (void *)AllocatorFreeForSDKHeap,

};

void *const data_02041934[36] __attribute__((aligned(__alignof__(void *)))) = {

    (void *)Gfd_LoadTex,

    (void *)Gfd_LoadTexPltt,

    0,

    0,

    &GX_LoadBG0Char,

    &GX_LoadBG1Char,

    &GX_LoadBG2Char,

    &GX_LoadBG3Char,

    &GX_LoadBG0Scr,

    &GX_LoadBG1Scr,

    &GX_LoadBG2Scr,

    &GX_LoadBG3Scr,

    0,

    0,

    &GX_LoadOBJPltt,

    &GX_LoadBGPltt,

    (void *)DoTransfer2dObjExtPlttMain,

    (void *)DoTransfer2dBGExtPlttMain,

    &GX_LoadOAM,

    &GX_LoadOBJ,

    &GXS_LoadBG0Char,

    &GXS_LoadBG1Char,

    &GXS_LoadBG2Char,

    &GXS_LoadBG3Char,

    &GXS_LoadBG0Scr,

    &GXS_LoadBG1Scr,

    &GXS_LoadBG2Scr,

    &GXS_LoadBG3Scr,

    0,

    0,

    &GXS_LoadOBJPltt,

    &GXS_LoadBGPltt,

    (void *)Gfd_LoadSubObjExtPltt,

    (void *)Gfd_LoadSubBgExtPltt,

    &GXS_LoadOAM,

    &GXS_LoadOBJ,

};

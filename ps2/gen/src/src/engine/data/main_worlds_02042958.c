/* PS2: mechanically prepared copy of src/engine/data/main_worlds_02042958.c (ps2/tools/prep_sources.py). Do not edit. */
#include "nitro/types.h"

#include "game/class_descriptor.h"
/* main .data, 0x02042958-0x02042a70: a 16-colour text palette, three task descriptors, the sound
 * archive and message database paths and the two-letter world directory codes.
 */

extern void Session_Init(void);
extern void Session_Destroy(void);
extern void Session_Init_2(void);
extern void CloseDebugOverlay(void);
extern void MsgQueue_Init_2(void);
extern void Session_Shutdown(void);
extern int data_0204c024;   /* the main heap arena the three tasks allocate from */

/* A 16-colour BGR555 palette (black, white, black, red, green, blue, yellow, magenta, cyan, ...)
 * loaded by ov000 0204d358 and ov027 02083ccc. */
u16 data_02042958[16] __attribute__((aligned(__alignof__(u16)))) = {
    0x0000, 0x7fff, 0x0000, 0x001f, 0x03e0, 0x7c00, 0x7fe0, 0x7c1f, 0x03ff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2108
};

/* -1 when idle; 020305d8 / 02030610 read and set it. */
int data_02042978 __attribute__((aligned(__alignof__(int)))) = -1;

GameClassDescriptor data_0204297c __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    0x3e,  /* nClassId */
    0xf,   /* nGroupId */
    Session_Init,  /* pfnCtor */
    Session_Destroy,  /* pfnMethod */
    0x30,  /* nAuxSize */
    &data_0204c024,  /* pArena */
};

GameClassDescriptor data_02042990 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    0x3e,  /* nClassId */
    0xf,   /* nGroupId */
    Session_Init_2,  /* pfnCtor */
    CloseDebugOverlay,  /* pfnMethod */
    0x74,  /* nAuxSize */
    &data_0204c024,  /* pArena */
};

GameClassDescriptor data_020429a4 __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    0x2,   /* nClassId */
    0xf,   /* nGroupId */
    MsgQueue_Init_2,  /* pfnCtor */
    Session_Shutdown,  /* pfnMethod */
    0x760, /* nAuxSize */
    &data_0204c024,  /* pArena */
};

/* Two small tables read by 02031600 / 02031618. */
int gSessionSetup[4] __attribute__((aligned(__alignof__(int)))) = { 1, 1, 0, 0 };
int data_020429c8[8] __attribute__((aligned(__alignof__(int)))) = { 1, 0, 0, 1, 0, 2, 0, 3 };

/* A flag 020329e8 tests. */
int data_020429e8 __attribute__((aligned(__alignof__(int)))) = 1;

char gSndSoundDataPath[24] __attribute__((aligned(__alignof__(char)))) = "/snd/sound_data.sdat";
char gDbDbPath[12] __attribute__((aligned(__alignof__(char)))) = "/db/db.p2";
char gDbDbPath_2[12] __attribute__((aligned(__alignof__(char)))) = "/db/db_&.p2";   /* '&' takes the language code */

/* A byte flag and a 3-byte record, 0xff when unset; ov002 / ov022 and 020352cc read them. */
u8 data_02042a1c[1] __attribute__((aligned(__alignof__(u8)))) = { 0xff };   /* a lone byte; the array spelling keeps it out of the word-aligned run */
u8 data_02042a1d[3] __attribute__((aligned(__alignof__(u8)))) = { 0xff, 0, 0 };

/* The two-letter world directory codes ("%s/%s/lv.b.z" of 02035730 takes one). */
char gDoName[4] __attribute__((aligned(__alignof__(char)))) = "do";
char gLuName[4] __attribute__((aligned(__alignof__(char)))) = "lu";
char gMaName[4] __attribute__((aligned(__alignof__(char)))) = "ma";
char gLeName[4] __attribute__((aligned(__alignof__(char)))) = "le";
char gDeName_2[4] __attribute__((aligned(__alignof__(char)))) = "de";
char gR2Name[4] __attribute__((aligned(__alignof__(char)))) = "r2";
char gXaName[4] __attribute__((aligned(__alignof__(char)))) = "xa";
char gVeName[4] __attribute__((aligned(__alignof__(char)))) = "ve";
char gXeName[4] __attribute__((aligned(__alignof__(char)))) = "xe";
char gXoName[4] __attribute__((aligned(__alignof__(char)))) = "xo";
char gZeName[4] __attribute__((aligned(__alignof__(char)))) = "ze";
char gMiName[4] __attribute__((aligned(__alignof__(char)))) = "mi";
char gRoName[4] __attribute__((aligned(__alignof__(char)))) = "ro";
char gRiName[4] __attribute__((aligned(__alignof__(char)))) = "ri";
char gGoName[4] __attribute__((aligned(__alignof__(char)))) = "go";
char gAxName[4] __attribute__((aligned(__alignof__(char)))) = "ax";
char gXiName[4] __attribute__((aligned(__alignof__(char)))) = "xi";
char gLaName[4] __attribute__((aligned(__alignof__(char)))) = "la";
char gSaName[4] __attribute__((aligned(__alignof__(char)))) = "sa";
char gSoName[4] __attribute__((aligned(__alignof__(char)))) = "so";

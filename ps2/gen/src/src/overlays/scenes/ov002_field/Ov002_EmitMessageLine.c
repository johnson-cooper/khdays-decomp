/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_EmitMessageLine.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_EmitMessageLine - expand a message and hand it to the caption emitter,
 * optionally after a fixed opening line.
 *
 * The opening line comes from a constant string; the message itself is expanded
 * into a 128-byte buffer through the scene's message context, with a 0x40 cap
 * and the caller's trailing arguments.
 *
 * Variadic: mwcc has no stdarg.h on this include path, so the APCS va macros
 * are inlined. va_start aligns &last down to 4 then adds 4.
 *
 * ARM.
 */

typedef char *va_list;
#define va_start(ap, last) ((ap) = (char *)(((int)&(last) & ~3) + 4))

typedef struct {
    char pad000[0x1b4];
    char msgCtx[0x40];
} Ov002TextScene;

extern int data_ov002_0207f62c;
extern const char data_ov002_0207ed1c[];

extern void Ov002_AppendU16List(void *pSink, const char *pText, int nArg);
extern int Ov002_MapAndForwardEntry(void *pMsg, unsigned int nId, char *pOut,
                               unsigned int nSize, void *pVa);

void Ov002_EmitMessageLine(void *pSink, unsigned int nId, int bOpen, int nArg,
                         ...)
{
    /* PS2 R14: the DS argument block `&nArg` pointed at */
    unsigned int __kh_va[1 + 12];
    { __builtin_va_list __kh_ap; int __kh_i; __builtin_memcpy(__kh_va, &nArg, 4); __builtin_va_start(__kh_ap, nArg); for (__kh_i = 1; __kh_i <= 12; __kh_i++) __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int); __builtin_va_end(__kh_ap); }

    va_list ap;
    char aText[0x80];
    Ov002TextScene *s;

    s = *(Ov002TextScene **)((char *)&data_ov002_0207f62c + 4);
    if (bOpen != 0) {
        Ov002_AppendU16List(pSink, data_ov002_0207ed1c, nArg);
    }

    va_start(ap, *((__typeof__(nArg) *)__kh_va));
    Ov002_MapAndForwardEntry(s->msgCtx, nId, aText, 0x40, ap);
    Ov002_AppendU16List(pSink, aText, nArg);
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_VariadicMapForward.c (ps2/tools/prep_sources.py). Do not edit. */
/*
 * Ov002_VariadicMapForward -- x3 (ov002/...). Variadic thin wrapper around Ov002_MapAndForwardEntry
 * (020528b8): forward the four named args plus a va_list pointing at the trailing arguments, and
 * return the entry pointer `c`.
 *
 * Variadic: mwcc has no stdarg.h on this include path, so the APCS va macros are inlined. va_start
 * here aligns &last down to 4 then adds 4 -- that is the ROM's `add r3,sp,#0x1c; bic r3,#3; add r3,#4`.
 */
typedef char *va_list;
#define va_start(ap, last) ((ap) = (char *)(((int)&(last) & ~3) + 4))

extern int Ov002_MapAndForwardEntry(int a, unsigned b, unsigned short *c, unsigned d, void *va);

unsigned short *Ov002_VariadicMapForward(int a, unsigned b, unsigned short *c, unsigned d, ...) {
    /* PS2 R14: the DS argument block `&d` pointed at */
    unsigned int __kh_va[1 + 12];
    { __builtin_va_list __kh_ap; int __kh_i; __builtin_memcpy(__kh_va, &d, 4); __builtin_va_start(__kh_ap, d); for (__kh_i = 1; __kh_i <= 12; __kh_i++) __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int); __builtin_va_end(__kh_ap); }

    va_list ap;

    va_start(ap, *((__typeof__(d) *)__kh_va));
    Ov002_MapAndForwardEntry(a, b, c, d, ap);
    return c;
}

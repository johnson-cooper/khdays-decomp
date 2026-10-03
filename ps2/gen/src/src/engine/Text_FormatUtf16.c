/* PS2: mechanically prepared copy of src/engine/Text_FormatUtf16.c (ps2/tools/prep_sources.py). Do not edit. */
/* Variadic forwarder: passes its three named arguments plus a va_list onto Text_VSNPrintfWide. */
extern void Text_VSNPrintfWide(int a, int b, int c, void *ap);

void Text_FormatUtf16(int a, int b, int c, ...) {
    /* PS2 R14: the DS argument block `&c` pointed at */
    unsigned int __kh_va[1 + 12];
    { __builtin_va_list __kh_ap; int __kh_i; __builtin_memcpy(__kh_va, &c, 4); __builtin_va_start(__kh_ap, c); for (__kh_i = 1; __kh_i <= 12; __kh_i++) __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int); __builtin_va_end(__kh_ap); }

    Text_VSNPrintfWide(a, b, c, (void *)(((unsigned int)((__typeof__(c) *)__kh_va) & ~3u) + 4));
}

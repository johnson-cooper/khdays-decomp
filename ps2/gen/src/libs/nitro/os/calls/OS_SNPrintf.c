/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_SNPrintf.c (ps2/tools/prep_sources.py). Do not edit. */
/* Bounded sprintf; same va_list spelling as OS_SPrintf.  The core's SDK name is one of the
 * misattributed ones. */
extern void *Text_VSNPrintf();

void *OS_SNPrintf(char *dst, unsigned int len, const char *fmt, ...) {
    /* PS2 R14: the DS argument block `&fmt` pointed at */
    unsigned int __kh_va[1 + 12];
    { __builtin_va_list __kh_ap; int __kh_i; __builtin_memcpy(__kh_va, &fmt, 4); __builtin_va_start(__kh_ap, fmt); for (__kh_i = 1; __kh_i <= 12; __kh_i++) __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int); __builtin_va_end(__kh_ap); }

    return Text_VSNPrintf(dst, len, fmt, (void *)(((unsigned int)((__typeof__(fmt) *)__kh_va) & ~3u) + 4));
}

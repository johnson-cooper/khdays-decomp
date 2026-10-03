/* PS2: mechanically prepared copy of libs/nitro/os/calls/OS_SPrintf.c (ps2/tools/prep_sources.py). Do not edit. */
/* Unbounded sprintf: hands the variadic tail to OS_VSPrintf.
 * The va_list is spelled out as the SDK's macro expands it -- align the address of the last
 * named parameter down to a word and step past it.  That is the ROM's
 * `add r2,sp,#0xc / bic r2,#3 / add r2,#4`, and taking `&fmt` is also what makes mwcc read
 * `fmt` back out of its homed slot instead of keeping the incoming register. */
extern void *OS_VSPrintf();

void *OS_SPrintf(char *dst, const char *fmt, ...) {
    /* PS2 R14: the DS argument block `&fmt` pointed at */
    unsigned int __kh_va[1 + 12];
    { __builtin_va_list __kh_ap; int __kh_i; __builtin_memcpy(__kh_va, &fmt, 4); __builtin_va_start(__kh_ap, fmt); for (__kh_i = 1; __kh_i <= 12; __kh_i++) __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int); __builtin_va_end(__kh_ap); }

    return OS_VSPrintf(dst, fmt, (void *)(((unsigned int)((__typeof__(fmt) *)__kh_va) & ~3u) + 4));
}

/* PS2: mechanically prepared copy of src/overlays/screens/ov025_camp_menu_2/Ov025_DrawPageBElement.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov025_DrawPageBElement -- draw a page-B element while the gate at data_ov025_020b575c is set,
 * handing the draw its 5th argument as a pointer to the trailing arguments; returns 1 when drawn.
 *
 * VARIADIC, and the old note had the diagnosis exactly right and only needed the spelling: the ROM
 * builds the extra-argument pointer with the APCS aligned form (`add r2,sp,#0x14 ; bic r2,#3 ;
 * add r2,#4`) and RE-READS param_2 out of the home-save block, which is what `va_start(ap, param_2)`
 * does -- taking the parameter's address is what puts it in memory. Note this is the opposite of the
 * ov221 pair, where the re-read was the residue to remove: here the ROM wants it, so the naive
 * va_start is correct and naming the vararg would be wrong.
 *
 * The last 4 bytes were nothing to do with varargs: the gate was declared `int *` and tested as
 * `*g`, which is two loads. It is a plain `int` global -- one `ldr` for the address, one for the
 * value. */
typedef char *va_list;
#define va_start(ap, last) ((ap) = (char *)(((int)&(last) & ~3) + 4))

extern int Ov025_GetPageB(void);
extern void Ov025_DrawStatusField(int page, int p1, int a, int p2, void *pExtra, int one);
extern int data_ov025_020b575c;

int Ov025_DrawPageBElement(int param_1, int param_2, ...) {
    /* PS2 R14: the DS argument block `&param_2` pointed at */
    unsigned int __kh_va[1 + 12];
    { __builtin_va_list __kh_ap; int __kh_i; __builtin_memcpy(__kh_va, &param_2, 4); __builtin_va_start(__kh_ap, param_2); for (__kh_i = 1; __kh_i <= 12; __kh_i++) __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int); __builtin_va_end(__kh_ap); }

    va_list ap;
    int page = Ov025_GetPageB();
    if (data_ov025_020b575c != 0) {
        va_start(ap, *((__typeof__(param_2) *)__kh_va));
        Ov025_DrawStatusField(page, param_1, 0, param_2, ap, 1);
        return 1;
    }
    return 0;
}

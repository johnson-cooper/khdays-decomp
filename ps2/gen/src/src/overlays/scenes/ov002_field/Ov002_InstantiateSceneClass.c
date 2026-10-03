/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_InstantiateSceneClass.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov002_InstantiateSceneClass -- instantiate the ov002 scene class at data_ov002_0207e830, forwarding this
 * function's own trailing arguments to the constructor, and latch the instance at +4 of
 * data_ov002_0207f600. Always reports 1.
 *
 * VARIADIC with NO fixed argument of its own: the ROM's va_list is `add r1,sp,#8`, which is the
 * address of the FIRST incoming word, not of anything past it. C needs one named parameter, so the
 * first vararg is named and its own address is the list -- the same trick the ov221 pair needed,
 * and it costs nothing because the parameter is never read. */
extern int InstantiateClass(void *classDesc, void *va);
extern int data_ov002_0207e830;
extern int data_ov002_0207f600;

int Ov002_InstantiateSceneClass(int first, ...) {
    /* PS2 R14: the DS argument block `&first` pointed at */
    unsigned int __kh_va[1 + 12];
    { __builtin_va_list __kh_ap; int __kh_i; __builtin_memcpy(__kh_va, &first, 4); __builtin_va_start(__kh_ap, first); for (__kh_i = 1; __kh_i <= 12; __kh_i++) __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int); __builtin_va_end(__kh_ap); }

    *(int *)((char *)&data_ov002_0207f600 + 4) =
        InstantiateClass(&data_ov002_0207e830, (char *)((__typeof__(first) *)__kh_va));
    return 1;
}

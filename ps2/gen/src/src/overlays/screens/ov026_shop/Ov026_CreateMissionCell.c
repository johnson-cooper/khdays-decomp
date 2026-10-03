/* PS2: mechanically prepared copy of src/overlays/screens/ov026_shop/Ov026_CreateMissionCell.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov026_CreateMissionCell -- create and place a mission cell object, ov008.
 * Instantiates a cell in slot `slot` from resource `res` (func_02032444),
 * makes it visible (Slot_SetMode2Bit), commits it (Slot_ClearFlagBit1), sets frame 0 (Slot_SetVisible)
 * and applies the transform passed by value via Slot_SetPosition. Returns the new object.
 *
 * Parked as a "frame-layout tie": the ROM homes all four arguments with `push {r0,r1,r2,r3}`
 * and passes `&xform` INTO that block (`add r2,sp,#0x1c`), while a plain 4-argument function
 * has to copy the transform to a stack slot of its own first. The push of r0-r3 is not a frame
 * choice, it is the VARIADIC prologue -- declare the function `...` and the block exists for
 * free, so `&xform` is the block slot and the copy disappears. `push {r0,r1,r2,r3}` at the top
 * of a function is always that tell. */
extern int  func_02032444(int *mgr, unsigned int res, int slot);
extern void Slot_SetMode2Bit(int mgr, int obj, int a);
extern void Slot_ClearFlagBit1(int mgr, int obj);
extern void Slot_SetVisible(int mgr, int obj, int a);
extern void Slot_SetPosition(int mgr, int obj, int *xform);

int Ov026_CreateMissionCell(int *mgr, unsigned int res, int slot, int xform, ...) {
    /* PS2 R14: the DS argument block `&xform` pointed at */
    unsigned int __kh_va[1 + 12];
    { __builtin_va_list __kh_ap; int __kh_i; __builtin_memcpy(__kh_va, &xform, 4); __builtin_va_start(__kh_ap, xform); for (__kh_i = 1; __kh_i <= 12; __kh_i++) __kh_va[__kh_i] = __builtin_va_arg(__kh_ap, unsigned int); __builtin_va_end(__kh_ap); }

    int obj = func_02032444(mgr, res, slot);
    Slot_SetMode2Bit((int)mgr, obj, 1);
    Slot_ClearFlagBit1((int)mgr, obj);
    Slot_SetVisible((int)mgr, obj, 0);
    Slot_SetPosition((int)mgr, obj, ((__typeof__(xform) *)__kh_va));
    return obj;
}

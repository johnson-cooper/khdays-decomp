/* PS2: mechanically prepared copy of src/engine/data/main_exception_index_0203d1bc.c (ps2/tools/prep_sources.py). Do not edit. */
/* main 0x0203d1bc-0x0203d210: the CodeWarrior exception-table index of the seven functions built
 * with exception tables.  Each entry names the function, its code size with bit 0 set (the
 * unwind descriptor is stored inline, not through a pointer) and that inline descriptor. */

#include "nitro/types.h"

typedef struct ExceptionTableIndex {
    void (*function)(void);   /* 0x00: function start */
    u32 sizeAndFlags;         /* 0x04: code size | 1 (descriptor stored inline) */
    u32 descriptor;           /* 0x08: inline unwind descriptor */
} ExceptionTableIndex;

extern void StackAlloc_FreeIfSet(void);
extern void __strtoul(void);
extern void strtol(void);
extern void func_020200b4(void);
extern void OSi_FreeStackAlloc(void);
extern void StackAlloc_FreeIfSetB(void);
extern void __call_static_initializers(void);

const ExceptionTableIndex data_0203d1bc[7] __attribute__((aligned(__alignof__(ExceptionTableIndex)))) = {
    { StackAlloc_FreeIfSet, 0x14 | 1, 0x00100000 },
    { __strtoul, 0x3e8 | 1, 0x0060ff00 },
    { strtol, 0xc8 | 1, 0x00600300 },
    { func_020200b4, 0x14 | 1, 0x00000000 },
    { OSi_FreeStackAlloc, 0x18 | 1, 0x00000000 },
    { StackAlloc_FreeIfSetB, 0x14 | 1, 0x00100000 },
    { __call_static_initializers, 0x2c | 1, 0x00100100 },
};

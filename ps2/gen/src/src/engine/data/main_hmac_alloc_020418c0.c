/* PS2: mechanically prepared copy of src/engine/data/main_hmac_alloc_020418c0.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .rodata 0x020418c0-0x02041924, one dsd symbol spanning two SDK objects:
 * - the three HMAC-SHA1 driver templates of NitroSDK math (MATHiHMACFuncs initialisers
 *   { MATH_SHA1_DIGEST_SIZE, 512 / 8 }); MATH_CalcHMACSHA1 (MATH_CalcHMACSHA1) copies the first one
 *   and fills in its context, buffer and SHA-1 callbacks;
 * - the NitroSystem FND allocator callback pairs of the expanded heap (allocate / free), which
 *   NNS_FndInitAllocatorFor*Heap install into an NNSFndAllocator. */

#include "nitro/types.h"

typedef struct MATHiHMACFuncs {
    u32 dlength;
    u32 blength;
    void *context;
    void *hash_buf;
    void (*HashReset)(void *context);
    void (*HashSetSource)(void *context, const void *input, u32 length);
    void (*HashGetDigest)(void *context, void *digest);
} MATHiHMACFuncs;

typedef void *(*NNSFndFuncAllocatorAlloc)(void *allocator, u32 size);
typedef void (*NNSFndFuncAllocatorFree)(void *allocator, void *memBlock);

typedef struct NNSFndAllocatorFunc {
    NNSFndFuncAllocatorAlloc pfAlloc;
    NNSFndFuncAllocatorFree pfFree;
} NNSFndAllocatorFunc;

#define MATH_SHA1_DIGEST_SIZE 20

extern void *AllocatorAllocForUnitHeap(void *allocator, u32 size);
extern void AllocatorFreeForUnitHeap(void *allocator, void *memBlock);
extern void *AllocatorAllocForFrmHeap(void *allocator, u32 size);
extern void AllocatorFreeForFrmHeap(void *allocator, void *memBlock);

const struct {
    MATHiHMACFuncs hmacSha1[3];
    NNSFndAllocatorFunc allocator[2];
} data_020418c0 __attribute__((aligned(4))) = {
    {
        { MATH_SHA1_DIGEST_SIZE, 512 / 8 },
        { MATH_SHA1_DIGEST_SIZE, 512 / 8 },
        { MATH_SHA1_DIGEST_SIZE, 512 / 8 },
    },
    {
        { AllocatorAllocForUnitHeap, AllocatorFreeForUnitHeap },
        { AllocatorAllocForFrmHeap, AllocatorFreeForFrmHeap },
    },
};

/* Diagnostics for the game's two NitroSystem expanded heaps (main 800 KiB, sub = rest of the
 * arena; func_ov001_0204cf5c keeps their handles in data_0204c044 / data_0204c030).
 *
 * kh_nns_heap_report() walks each heap's free and used block lists (the NitroSystem layout from
 * include/nnsys/fnd.h) and logs free space, the largest free block and the used total; with
 * `detail` it also lists the most common used-block sizes, which is what exposes a leak.  Called
 * every 1200 VBlanks and before OS_Terminate stops the game.
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <stdlib.h>

typedef struct MBlock {
    u16 signature, attribute;
    u32 size;
    struct MBlock *prev, *next;
} MBlock;

typedef struct {
    u32 signature;
    void *link[2];
    void *child_head, *child_tail;
    u16 child_count, child_offset;
    u8 *start, *end;
    u32 attribute;
    MBlock *free_head, *free_tail, *used_head, *used_tail;
    u16 group, feature;
} ExpHeap;

extern void *data_0204c044;   /* main heap */
extern void *data_0204c030;   /* sub heap (default) */

static void report(const char *name, const ExpHeap *h, int detail)
{
    enum { NSIZES = 64, NSHOW = 8 };
    u32 free_total = 0, free_max = 0, used_total = 0, used_n = 0, i;
    u32 sizes[NSIZES] = { 0 }, counts[NSIZES] = { 0 };
    const MBlock *b;
    int guard;

    if (!h || h->signature != 0x45585048u) {      /* "EXPH" */
        KH_INFO("heap", "%s: not an expanded heap (%p)", name, (const void *)h);
        return;
    }
    for (b = h->free_head, guard = 0; b && guard < 100000; b = b->next, guard++) {
        free_total += b->size;
        if (b->size > free_max)
            free_max = b->size;
    }
    for (b = h->used_head, guard = 0; b && guard < 100000; b = b->next, guard++) {
        used_total += b->size;
        used_n++;
        if (!detail)
            continue;
        for (i = 0; i < NSIZES && sizes[i] && sizes[i] != b->size; i++)
            ;
        if (i < NSIZES) {
            sizes[i] = b->size;
            counts[i]++;
        }
    }
    KH_INFO("heap", "%s %u KiB: free %u (largest %u), used %u in %u blocks", name,
            (unsigned)((h->end - h->start) / 1024), (unsigned)free_total, (unsigned)free_max,
            (unsigned)used_total, (unsigned)used_n);
    if (detail) {
        int shown;
        for (shown = 0; shown < NSHOW; shown++) {       /* the most frequent sizes first */
            u32 best = 0, j;
            for (j = 1; j < NSIZES; j++)
                if (counts[j] * sizes[j] > counts[best] * sizes[best])
                    best = j;
            if (!counts[best])
                break;
            KH_INFO("heap", "  %s: %u blocks of %u bytes", name, (unsigned)counts[best], (unsigned)sizes[best]);
            counts[best] = 0;
        }
    }
}

void kh_nns_heap_report(int detail)
{
    report("main", data_0204c044, detail);
    report("sub", data_0204c030, detail);
}

/* Every free to an expanded heap goes through here first (-Wl,--wrap=NNS_FndFreeToExpHeap).  A
 * block that is not on the heap's used list - NULL, freed twice, or left over from a heap that
 * HeapState_Recreate rebuilt since - would be linked into the free list and later handed out
 * while still overlapping live memory.  Such a free is logged with its caller and dropped. */
void __real_NNS_FndFreeToExpHeap(void *heap, void *mem);
void kh_nns_heap_check(const char *when);
static struct LastOp { const char *op; void *heap, *ptr, *caller; u32 size; } g_last, g_prev;

void __wrap_NNS_FndFreeToExpHeap(void *heap, void *mem)
{
    const ExpHeap *h = heap;
    const MBlock *want = (const MBlock *)((const u8 *)mem - sizeof(MBlock));

    /* Membership in O(1): a used block carries the 'UD' signature, lies inside the heap, and its
     * neighbours point back at it (or the heap's head/tail does).  This was a walk of the whole
     * used list per free - O(live blocks) - which grew with every enemy and effect on screen. */
    if (mem && h && h->signature == 0x45585048u &&
        (const u8 *)want >= h->start && (const u8 *)mem <= h->end &&
        !((uintptr_t)want & 3u) && want->signature == 0x5544u /* 'UD' */ &&
        (want->prev ? ((const u8 *)want->prev >= h->start && (const u8 *)want->prev < h->end &&
                       want->prev->next == want)
                    : h->used_head == want) &&
        (want->next ? ((const u8 *)want->next >= h->start && (const u8 *)want->next < h->end &&
                       want->next->prev == want)
                    : h->used_tail == want)) {
        __real_NNS_FndFreeToExpHeap(heap, mem);
        g_prev = g_last; g_last.op = "free"; g_last.heap = heap; g_last.ptr = mem; g_last.size = want->size;
        g_last.caller = __builtin_return_address(0);
#if defined(KH_PS2_HEAP_CHECK) && KH_PS2_HEAP_CHECK
        kh_prof_begin(KH_PROF_DEBUG);
        kh_nns_heap_check("after a free");
        kh_prof_end(KH_PROF_DEBUG);
#endif
        return;
    }
    KH_ERR("heap", "free of %p (header %04x size %u) that heap %p does not hold, from %p: ignored",
           mem, mem ? (unsigned)want->signature : 0u, mem ? (unsigned)want->size : 0u, heap,
           __builtin_return_address(0));
}

/* ---- consistency checking (opt-in KH_PS2_HEAP_CHECK builds) ----------------------------
 * After every allocation and free (both wrapped) and once per VBlank, walk both lists of the two
 * game heaps: signatures, back links, and that the head walk ends at the recorded tail.  The
 * first failure is reported with the operation that preceded it; checking then stops. */
void *__real_NNS_FndAllocFromExpHeapEx(void *heap, u32 size, int align);

static int g_heap_broken;
static const MBlock *g_bad_block;

static const char *list_error(const MBlock *head, const MBlock *tail, u16 sig, const u8 *lo, const u8 *hi)
{
    const MBlock *b, *prev = NULL;
    int guard = 0;
    for (b = head; b; prev = b, b = b->next) {
        g_bad_block = b;
        if ((const u8 *)b < lo || (const u8 *)b >= hi)
            return "block outside the heap";
        if (b->signature != sig)
            return "bad block signature";
        if (b->prev != prev)
            return "broken back link";
        if (++guard > 100000)
            return "cycle";
    }
    return prev == tail ? NULL : "walk does not end at the tail";
}

/* No two blocks may overlap: [block, block + 16 + size) for every free and used block. */
static const MBlock *g_blocks[4096];

static int by_address(const void *a, const void *b)
{
    const MBlock *x = *(const MBlock *const *)a, *y = *(const MBlock *const *)b;
    return x < y ? -1 : x > y;
}

static const char *overlap_error(const ExpHeap *h)
{
    int n = 0, i;
    const MBlock *b;
    for (b = h->free_head; b && n < 4096; b = b->next)
        g_blocks[n++] = b;
    for (b = h->used_head; b && n < 4096; b = b->next)
        g_blocks[n++] = b;
    qsort(g_blocks, n, sizeof g_blocks[0], by_address);
    for (i = 0; i + 1 < n; i++)
        if ((const u8 *)g_blocks[i] + sizeof(MBlock) + g_blocks[i]->size > (const u8 *)g_blocks[i + 1]) {
            g_bad_block = g_blocks[i + 1];
            KH_ERR("heap", "block %p (%04x, %u bytes) overlaps block %p (%04x)", (const void *)g_blocks[i],
                   (unsigned)g_blocks[i]->signature, (unsigned)g_blocks[i]->size,
                   (const void *)g_blocks[i + 1], (unsigned)g_blocks[i + 1]->signature);
            return "overlapping blocks";
        }
    return NULL;
}

static void check_heap(const char *name, const ExpHeap *h, const char *when)
{
    const char *err;
    if (g_heap_broken || !h || h->signature != 0x45585048u)
        return;
    err = list_error(h->free_head, h->free_tail, 0x4652, h->start, h->end);
    if (!err)
        err = list_error(h->used_head, h->used_tail, 0x5544, h->start, h->end);
    if (!err)
        err = overlap_error(h);
    if (!err)
        return;
    g_heap_broken = 1;
    KH_ERR("heap", "%s heap %p corrupt at block %p [%08x %08x %08x %08x] (%s) %s", name,
           (const void *)h, (const void *)g_bad_block, ((const u32 *)g_bad_block)[0],
           ((const u32 *)g_bad_block)[1], ((const u32 *)g_bad_block)[2], ((const u32 *)g_bad_block)[3],
           err, when);
    KH_ERR("heap", "  this op: %s %p size %u from %p", g_last.op ? g_last.op : "-", g_last.ptr,
           (unsigned)g_last.size, g_last.caller);
    KH_ERR("heap", "  last good op: %s %p size %u from %p", g_prev.op ? g_prev.op : "-", g_prev.ptr,
           (unsigned)g_prev.size, g_prev.caller);
    kh_nns_heap_report(1);
}

void kh_nns_heap_check(const char *when)
{
    check_heap("main", data_0204c044, when);
    check_heap("sub", data_0204c030, when);
}

void *__wrap_NNS_FndAllocFromExpHeapEx(void *heap, u32 size, int align)
{
    void *p = __real_NNS_FndAllocFromExpHeapEx(heap, size, align);
    g_prev = g_last; g_last.op = "alloc"; g_last.heap = heap; g_last.ptr = p; g_last.size = size;
    g_last.caller = __builtin_return_address(0);
    if (!p && heap)
        KH_WARN("heap", "allocation of %u bytes from %p failed (from %p)", (unsigned)size, heap,
                __builtin_return_address(0));
#if defined(KH_PS2_HEAP_CHECK) && KH_PS2_HEAP_CHECK
    kh_prof_begin(KH_PROF_DEBUG);
    kh_nns_heap_check("after an allocation");
    kh_prof_end(KH_PROF_DEBUG);
#endif
    return p;
}

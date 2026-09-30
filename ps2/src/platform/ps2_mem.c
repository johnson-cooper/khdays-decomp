/* EE memory management and diagnostics.
 *
 * Layout decided at kh_mem_init():
 *   - the ELF (text/data/bss) as linked;
 *   - the game arena: one fixed block the DS game's OS arena is carved from (its FND heaps live
 *     inside it, exactly as on the DS, only larger);
 *   - one bump arena per non-global lifetime (scene, mission, room, character, enemy, effect,
 *     frame), each reset as a whole when its lifetime ends -- no fragmentation leaves them;
 *   - newlib malloc for KH_LIFE_GLOBAL allocations, headers track category and size.
 * The budgets below are deliberately conservative; kh_mem_report() prints real usage.
 */
#include "platform/kh_platform.h"
#include "ps2_internal.h"

#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define KB 1024u
#define MB (1024u * 1024u)
#define EE_RAM (32u * MB)

#ifndef KH_GAME_ARENA_SIZE
#define KH_GAME_ARENA_SIZE (4u * MB)   /* the DS had < 4 MiB for code AND data */
#endif

static const uint32_t k_life_cap[KH_LIFE_COUNT] = {
    [KH_LIFE_GLOBAL] = 0,           /* malloc */
    [KH_LIFE_SCENE] = 1536 * KB,
    [KH_LIFE_MISSION] = 256 * KB,
    [KH_LIFE_ROOM] = 512 * KB,
    [KH_LIFE_CHARACTER] = 256 * KB,
    [KH_LIFE_ENEMY] = 256 * KB,
    [KH_LIFE_EFFECT] = 256 * KB,
    [KH_LIFE_FRAME] = 256 * KB,
};

typedef struct Arena { uint8_t *base; uint32_t cap, used, peak; } Arena;

typedef struct GlobalHdr { uint32_t size; uint16_t cat; uint16_t magic; uint32_t pad[2]; } GlobalHdr; /* 16 bytes */
#define HDR_MAGIC 0x4b48

static Arena g_arena[KH_LIFE_COUNT];
static uint8_t *g_game_arena;
static int32_t g_cat_used[KH_MEM_COUNT];
static int g_inited;

extern char _ftext[], _end[];

void kh_mem_init(void)
{
    int i;
    if (g_inited)
        return;
    g_inited = 1;
    /* First allocation, so it sits right after the ELF.  The game packs pointers into 24-bit
     * handles (see KH_DS_PACKED_PTR_BASE), which only works if the ELF's data and the arena all
     * lie in the first 16 MiB. */
    g_game_arena = memalign(64, KH_GAME_ARENA_SIZE);
    if (!g_game_arena)
        kh_panic("cannot reserve the %u KiB game arena", KH_GAME_ARENA_SIZE / KB);
    if ((uintptr_t)g_game_arena + KH_GAME_ARENA_SIZE > 0x01000000u)
        kh_panic("game arena %p-%p crosses 16 MiB (ELF too large: ends at %p)", (void *)g_game_arena,
                 (void *)(g_game_arena + KH_GAME_ARENA_SIZE), (void *)_end);
    memset(g_game_arena, 0, KH_GAME_ARENA_SIZE);
    g_cat_used[KH_MEM_GAME_HEAP] += KH_GAME_ARENA_SIZE;
    for (i = 1; i < KH_LIFE_COUNT; i++) {
        g_arena[i].cap = k_life_cap[i];
        g_arena[i].base = memalign(64, k_life_cap[i]);
        if (!g_arena[i].base)
            kh_panic("cannot reserve arena %d (%u KiB)", i, k_life_cap[i] / KB);
    }
}

void *kh_mem_game_arena(size_t *size_out)
{
    if (size_out)
        *size_out = KH_GAME_ARENA_SIZE;
    return g_game_arena;
}

void *kh_alloc(size_t size, size_t align, KhLifetime life, KhMemCat cat)
{
    if (align < 16)
        align = 16;
    if (life == KH_LIFE_GLOBAL) {
        /* header sits directly in front of the returned block */
        uint8_t *raw = memalign(align, size + align);
        GlobalHdr *h;
        if (!raw) {
            KH_ERR("mem", "out of memory: %u bytes (cat %d)", (unsigned)size, cat);
            kh_mem_report();
            return NULL;
        }
        h = (GlobalHdr *)(raw + align - sizeof(GlobalHdr));
        h->size = (uint32_t)size;
        h->cat = (uint16_t)cat;
        h->magic = HDR_MAGIC;
        h->pad[0] = (uint32_t)(align - sizeof(GlobalHdr)); /* offset back to raw */
        g_cat_used[cat] += (int32_t)size;
        return raw + align;
    } else {
        Arena *a = &g_arena[life];
        uint32_t off = (a->used + (uint32_t)align - 1) & ~((uint32_t)align - 1);
        if (off + size > a->cap) {
            KH_ERR("mem", "arena %d exhausted: need %u, used %u of %u", life, (unsigned)size, a->used, a->cap);
            return NULL;
        }
        a->used = off + (uint32_t)size;
        if (a->used > a->peak)
            a->peak = a->used;
        g_cat_used[cat] += (int32_t)size;
        return a->base + off;
    }
}

void kh_free(void *p)
{
    GlobalHdr *h;
    if (!p)
        return;
    h = (GlobalHdr *)((uint8_t *)p - sizeof(GlobalHdr));
    if (h->magic != HDR_MAGIC) {
        KH_ERR("mem", "kh_free of a non-global or corrupt block %p", p);
        return;
    }
    g_cat_used[h->cat] -= (int32_t)h->size;
    h->magic = 0;
    free((uint8_t *)h - h->pad[0]);
}

void kh_mem_reset_lifetime(KhLifetime life)
{
    if (life == KH_LIFE_GLOBAL)
        return;
    g_arena[life].used = 0;
}

void kh_mem_account(KhMemCat cat, int32_t delta) { g_cat_used[cat] += delta; }

void kh_mem_get_stats(KhMemStats *o)
{
    struct mallinfo mi = mallinfo();
    uint32_t elf = (uint32_t)(_end - _ftext);
    uint32_t heap_top;
    int i;

    memset(o, 0, sizeof *o);
    o->ee_elf = elf;
    o->ee_total = EE_RAM - 0x100000u; /* kernel below 1 MiB */
    o->heap_used = (uint32_t)mi.uordblks;
    /* What sbrk can still hand out plus what malloc holds free. */
    heap_top = (uint32_t)(uintptr_t)sbrk(0);
    o->heap_free = (uint32_t)mi.fordblks + (EE_RAM - 128u * KB - heap_top);
    o->heap_largest = EE_RAM - 128u * KB - heap_top; /* contiguous top of heap; an under-estimate */
    for (i = 0; i < KH_MEM_COUNT; i++)
        o->cat_used[i] = (uint32_t)g_cat_used[i];
    for (i = 0; i < KH_LIFE_COUNT; i++) {
        o->life_used[i] = g_arena[i].used;
        o->life_cap[i] = g_arena[i].cap;
    }
    o->gs_vram_used = ps2_gs_vram_used();
    o->gs_vram_total = ps2_gs_vram_total();
}

void kh_mem_report(void)
{
    static const char *const cat_name[KH_MEM_COUNT] = {
        "misc", "game-heap", "texture", "geometry", "animation", "resource", "audio", "file-cache", "render"
    };
    static const char *const life_name[KH_LIFE_COUNT] = {
        "global", "scene", "mission", "room", "character", "enemy", "effect", "frame"
    };
    KhMemStats s;
    int i;
    kh_mem_get_stats(&s);
    KH_INFO("mem", "EE: elf %u KiB, heap used %u KiB, free %u KiB, largest free %u KiB",
            s.ee_elf / KB, s.heap_used / KB, s.heap_free / KB, s.heap_largest / KB);
    for (i = 0; i < KH_MEM_COUNT; i++)
        if (s.cat_used[i])
            KH_INFO("mem", "  %-10s %7u KiB", cat_name[i], s.cat_used[i] / KB);
    for (i = 1; i < KH_LIFE_COUNT; i++)
        KH_INFO("mem", "  arena %-9s %6u / %6u KiB (peak %u)", life_name[i], s.life_used[i] / KB,
                s.life_cap[i] / KB, g_arena[i].peak / KB);
    KH_INFO("mem", "GS VRAM: %u / %u KiB", s.gs_vram_used / KB, s.gs_vram_total / KB);
}

/* NitroSDK GX / G2 on the PS2: display control, VRAM bank mapping, BG/OBJ/palette/texture
 * loads, BG pointers, blending and brightness.
 *
 * Model.  The nine DS VRAM banks live in kh_ds_vram in LCDC order.  Each DS VRAM "view" (BG and
 * OBJ of both engines, texture image and palette, extended palettes) is a list of banks placed
 * consecutively in bank order -- this is how every mapping the SDK offers lays them out.  Loads
 * copy into the mapped banks; G2_GetBG*Ptr return CPU pointers into them.  Display registers are
 * written into kh_ds_io exactly as the SDK writes them to the hardware.  The GS compositor
 * (ps2/src/gfx) reads banks + registers each frame; nothing here draws.
 */
#include "platform/kh_platform.h"
#include "nitro_internal.h"

#include <string.h>

#define REG16(off) (*(volatile u16 *)(kh_ds_io + (off)))
#define REG32(off) (*(volatile u32 *)(kh_ds_io + (off)))

enum { BANK_A, BANK_B, BANK_C, BANK_D, BANK_E, BANK_F, BANK_G, BANK_H, BANK_I, NBANKS };
static const u32 k_bank_size[NBANKS] = { 0x20000, 0x20000, 0x20000, 0x20000, 0x10000, 0x4000, 0x4000, 0x8000, 0x4000 };

/* views */
enum {
    VIEW_BG, VIEW_OBJ, VIEW_SUB_BG, VIEW_SUB_OBJ, VIEW_TEX, VIEW_TEXPLTT,
    VIEW_BGEXT, VIEW_OBJEXT, VIEW_SUB_BGEXT, VIEW_SUB_OBJEXT, VIEW_LCDC, NVIEWS
};
static u32 g_view_banks[NVIEWS];   /* GX_VRAM_* bank masks */
/* where each bank sits inside the view, fixed when the view is set: taking a bank away (to LCDC,
 * to another engine) leaves the others where they are, as on the DS - texture slot 3 stays slot 3
 * when slot 2's bank is reassigned.  (Recomputing a packed layout shifted every texture.) */
static u32 g_view_ofs[NVIEWS][NBANKS];
uint32_t kh_vram_page_gen[KH_VRAM_PAGES];

/* ------------------------------------------------------------------ banks */

/* Byte offset in kh_ds_vram for offset `ofs` inside a view; ~0 if not mapped. */
uint32_t kh_nitro_view_to_vram(int view, uint32_t ofs)
{
    u32 banks = g_view_banks[view];
    int b;
    for (b = 0; b < NBANKS; b++) {
        u32 base = g_view_ofs[view][b];
        if ((banks & (1u << b)) && ofs >= base && ofs - base < k_bank_size[b])
            return kh_ds_vram_bank_ofs[b] + (ofs - base);
    }
    return 0xffffffffu;
}

u8 *kh_nitro_view_ptr(int view, uint32_t ofs)
{
    uint32_t v = kh_nitro_view_to_vram(view, ofs);
    return v == 0xffffffffu ? NULL : kh_ds_vram + v;
}

uint32_t kh_nitro_view_banks(int view) { return g_view_banks[view]; }

/* The bank holding kh_ds_vram byte `ofs` (-1: none) and where that bank ends. */
int kh_nitro_vram_bank_at(uint32_t ofs, uint32_t *end)
{
    int b;
    for (b = 0; b < NBANKS; b++)
        if (ofs >= kh_ds_vram_bank_ofs[b] && ofs < kh_ds_vram_bank_ofs[b] + k_bank_size[b]) {
            *end = kh_ds_vram_bank_ofs[b] + k_bank_size[b];
            return b;
        }
    *end = ofs + 1;
    return -1;
}

/* Whether the CPU can write bank b: mapped to LCDC or to a 2D engine's BG/OBJ memory.  Banks used
 * as texture / texture-palette / extended-palette memory are not in the CPU's address space. */
int kh_nitro_bank_cpu_visible(int b)
{
    u32 m = g_view_banks[VIEW_LCDC] | g_view_banks[VIEW_BG] | g_view_banks[VIEW_OBJ] |
            g_view_banks[VIEW_SUB_BG] | g_view_banks[VIEW_SUB_OBJ];
    return b >= 0 && ((m >> b) & 1);
}

/* Whether bank b is mapped to the LCDC window (0x06800000..) */
int kh_nitro_bank_in_lcdc(int b)
{
    return b >= 0 && ((g_view_banks[VIEW_LCDC] >> b) & 1);
}

/* window 0..3 = BG-A, BG-B, OBJ-A, OBJ-B (the 0x06000000.. CPU windows) */
uint32_t kh_nitro_vram_window_ofs(int window)
{
    static const int k_view[4] = { VIEW_BG, VIEW_SUB_BG, VIEW_OBJ, VIEW_SUB_OBJ };
    uint32_t v = kh_nitro_view_to_vram(k_view[window & 3], 0);
    return v == 0xffffffffu ? 0 : v;
}

static void unmap_banks(u32 banks)
{
    int v;
    for (v = 0; v < NVIEWS; v++)
        g_view_banks[v] &= ~banks;
}

/* A texture / texture-palette view changing its banks changes what every cached 3D texture reads */
static void view_changed(u32 before_tex, u32 before_pltt)
{
    extern void kh_tex3d_invalidate(void);
    if (g_view_banks[VIEW_TEX] != before_tex || g_view_banks[VIEW_TEXPLTT] != before_pltt)
        kh_tex3d_invalidate();
}

/* Mapping (or unmapping) extended-palette banks switches the engine's extended palettes on (off):
 * DISPCNT bit 30 for BG, bit 31 for OBJ, as the SDK's bgExtPlttOn_ / objExtPlttOn_ do. */
static void ext_pltt_enable(int view, int on)
{
    u32 off, bit;
    switch (view) {
    case VIEW_BGEXT:      off = 0;      bit = 0x40000000u; break;
    case VIEW_OBJEXT:     off = 0;      bit = 0x80000000u; break;
    case VIEW_SUB_BGEXT:  off = 0x1000; bit = 0x40000000u; break;
    case VIEW_SUB_OBJEXT: off = 0x1000; bit = 0x80000000u; break;
    default: return;
    }
    if (on)
        REG32(off) |= bit;
    else
        REG32(off) &= ~bit;
}

static u32 set_view(int view, u32 banks)
{
    u32 old = g_view_banks[view], t = g_view_banks[VIEW_TEX], pl = g_view_banks[VIEW_TEXPLTT];
    u32 at = 0;
    int b;
    unmap_banks(banks);
    g_view_banks[view] = banks;
    /* GX_VRAM_BGEXTPLTT_23_G: bank G holds slots 2-3 (offset 0x4000) */
    if (view == VIEW_BGEXT && banks == (1u << BANK_G))
        at = 0x4000;
    for (b = 0; b < NBANKS; b++)        /* the SDK's mappings place the banks consecutively, in bank order */
        if (banks & (1u << b)) {
            g_view_ofs[view][b] = at;
            at += k_bank_size[b];
        }
    ext_pltt_enable(view, banks != 0);
    view_changed(t, pl);
    return old;
}

static u32 reset_view(int view)
{
    u32 old = g_view_banks[view], t = g_view_banks[VIEW_TEX], pl = g_view_banks[VIEW_TEXPLTT];
    g_view_banks[view] = 0;
    ext_pltt_enable(view, 0);
    view_changed(t, pl);
    return old;
}

void GX_SetBankForBG(u32 b)            { set_view(VIEW_BG, b); }
void GX_SetBankForOBJ(u32 b)           { set_view(VIEW_OBJ, b); }
void GX_SetBankForBGExtPltt(u32 b)     { set_view(VIEW_BGEXT, b); }
void GX_SetBankForOBJExtPltt(u32 b)    { set_view(VIEW_OBJEXT, b); }
void GX_SetBankForTex(u32 b)           { set_view(VIEW_TEX, b); }
void GX_SetBankForTexPltt(u32 b)       { set_view(VIEW_TEXPLTT, b); }
void GX_SetBankForSubBG(u32 b)         { set_view(VIEW_SUB_BG, b); }
void GX_SetBankForSubOBJ(u32 b)        { set_view(VIEW_SUB_OBJ, b); }
void GX_SetBankForSubBGExtPltt(u32 b)  { set_view(VIEW_SUB_BGEXT, b); }
void GX_SetBankForSubOBJExtPltt(u32 b) { set_view(VIEW_SUB_OBJEXT, b); }
/* LCDC: the banks leave whatever view they had (VRAMCNT holds one use per bank) */
void GX_SetBankForLCDC(u32 b)
{
    u32 t = g_view_banks[VIEW_TEX], pl = g_view_banks[VIEW_TEXPLTT], lcdc = g_view_banks[VIEW_LCDC];
    unmap_banks(b);
    g_view_banks[VIEW_LCDC] = lcdc | b;
    view_changed(t, pl);
}

u32 GX_ResetBankForBG(void)            { return reset_view(VIEW_BG); }
u32 GX_ResetBankForOBJ(void)           { return reset_view(VIEW_OBJ); }
u32 GX_ResetBankForBGExtPltt(void)     { return reset_view(VIEW_BGEXT); }
u32 GX_ResetBankForOBJExtPltt(void)    { return reset_view(VIEW_OBJEXT); }
u32 GX_ResetBankForTex(void)           { return reset_view(VIEW_TEX); }
u32 GX_ResetBankForTexPltt(void)       { return reset_view(VIEW_TEXPLTT); }
u32 GX_ResetBankForSubBG(void)         { return reset_view(VIEW_SUB_BG); }
u32 GX_ResetBankForSubOBJ(void)        { return reset_view(VIEW_SUB_OBJ); }
u32 GX_ResetBankForSubBGExtPltt(void)  { return reset_view(VIEW_SUB_BGEXT); }
u32 GX_ResetBankForSubOBJExtPltt(void) { return reset_view(VIEW_SUB_OBJEXT); }

/* "Disable" = the banks are switched off (VRAMCNT enable bit clear) and unlocked, exactly as the
 * SDK's disableBankForX_ does - their contents stay.  (This used to clear them, which destroyed
 * textures and characters whenever a game unmapped a bank to remap it.) */
static u32 disable_view(int view)
{
    return reset_view(view);
}

u32 GX_DisableBankForBG(void)            { return disable_view(VIEW_BG); }
u32 GX_DisableBankForOBJ(void)           { return disable_view(VIEW_OBJ); }
u32 GX_DisableBankForBGExtPltt(void)     { return disable_view(VIEW_BGEXT); }
u32 GX_DisableBankForOBJExtPltt(void)    { return disable_view(VIEW_OBJEXT); }
u32 GX_DisableBankForTex(void)           { return disable_view(VIEW_TEX); }
u32 GX_DisableBankForTexPltt(void)       { return disable_view(VIEW_TEXPLTT); }
u32 GX_DisableBankForSubBG(void)         { return disable_view(VIEW_SUB_BG); }
u32 GX_DisableBankForSubOBJ(void)        { return disable_view(VIEW_SUB_OBJ); }
u32 GX_DisableBankForSubBGExtPltt(void)  { return disable_view(VIEW_SUB_BGEXT); }
u32 GX_DisableBankForSubOBJExtPltt(void) { return disable_view(VIEW_SUB_OBJEXT); }
u32 GX_DisableBankForLCDC(void)          { u32 o = g_view_banks[VIEW_LCDC]; g_view_banks[VIEW_LCDC] = 0; return o; }

u32 GX_GetBankForBG(void)         { return g_view_banks[VIEW_BG]; }
u32 GX_GetBankForOBJ(void)        { return g_view_banks[VIEW_OBJ]; }
u32 GX_GetBankForBGExtPltt(void)  { return g_view_banks[VIEW_BGEXT]; }
u32 GX_GetBankForOBJExtPltt(void) { return g_view_banks[VIEW_OBJEXT]; }
u32 GX_GetBankForTex(void)        { return g_view_banks[VIEW_TEX]; }
u32 GX_GetBankForTexPltt(void)    { return g_view_banks[VIEW_TEXPLTT]; }
u32 GX_GetBankForSubBG(void)      { return g_view_banks[VIEW_SUB_BG]; }
u32 GX_GetBankForSubOBJ(void)     { return g_view_banks[VIEW_SUB_OBJ]; }
u32 GX_GetBankForLCDC(void)       { return g_view_banks[VIEW_LCDC]; }

/* ------------------------------------------------------------------ loads */

/*
 * Source check for every GX_Load* (they are the loaders behind the NNS GFD VRAM-transfer queue,
 * drained by FrameStep_UpdateTaskQueue each frame).  On the DS a transfer queued with a NULL (or
 * small) source address reads the ITCM mapped at 0 and copies junk without faulting; on the EE
 * the same read is a TLB miss below the kernel/ELF.  Hardware: the field's first gameplay frame
 * in Roxas's room crashed in memcpy(dst, NULL, 0x40) from the transfer queue.  Treat such a
 * source as nothing to load (the destination keeps its previous contents), and log it.
 */
static int load_src_ok(const void *src, u32 ofs, u32 size, int view)
{
    static int warned;
    if ((uintptr_t)src >= 0x00100000u)          /* EE user memory starts above the kernel */
        return 1;
    if (size && warned < 16) {
        warned++;
        KH_WARN("gx", "GX load from invalid source %p (view %d, offset 0x%x, %u bytes) skipped; "
                "caller %p", src, view, (unsigned)ofs, (unsigned)size, __builtin_return_address(0));
    }
    return 0;
}

static void view_copy_tag(int view, const void *src, u32 ofs, u32 size, char tag)
{
    const u8 *s = src;
    if (!load_src_ok(src, ofs, size, view))
        return;
    while (size) {
        u32 v = kh_nitro_view_to_vram(view, ofs);
        u32 n;
        int b;
        if (v == 0xffffffffu) {
            KH_WARN("gx", "load to unmapped VRAM (view %d, offset 0x%x)", view, (unsigned)ofs);
            if (view == VIEW_BG)
                kh_vram_log('U', tag, ofs, size, -1);   /* BG offset, not a VRAM one */
            return;
        }
        /* bytes left in this bank */
        for (b = 0; b < NBANKS && !(v >= kh_ds_vram_bank_ofs[b] && v < kh_ds_vram_bank_ofs[b] + k_bank_size[b]); b++)
            ;
        n = kh_ds_vram_bank_ofs[b] + k_bank_size[b] - v;
        if (n > size)
            n = size;
        memcpy(kh_ds_vram + v, s, n);
        kh_vram_mark(v, n);
        kh_vram_log_src('L', tag, v, n, s);
        s += n;
        ofs += n;
        size -= n;
    }
}

/* BG char/screen base from BGxCNT: char base 16 KiB units (bits 2-5), screen base 2 KiB (8-12),
 * plus DISPCNT's 64 KiB char/screen offsets on engine A. */
static u32 bg_char_base(int sub, int bg)
{
    u16 cnt = REG16((sub ? 0x1008 : 0x008) + bg * 2);
    u32 base = ((cnt >> 2) & 0xf) * 0x4000;
    if (!sub)
        base += ((REG32(0) >> 24) & 7) * 0x10000;
    return base;
}

static u32 bg_scr_base(int sub, int bg)
{
    u16 cnt = REG16((sub ? 0x1008 : 0x008) + bg * 2);
    int mode = REG32(sub ? 0x1000 : 0) & 7;
    u32 base;
    /* an extended BG in a bitmap form (BG3 in modes 3-5, BG2 in mode 5, BGxCNT bit 7) has its
     * bitmap in 16 KiB units and no DISPCNT screen offset (as G2_GetBGxScrPtr); mode 6's large
     * bitmap starts at the BG memory */
    if (((bg == 3 && mode >= 3 && mode <= 5) || (bg == 2 && mode == 5)) && (cnt & 0x80))
        return ((cnt >> 8) & 0x1f) * 0x4000;
    if (bg == 2 && mode == 6)
        return 0;
    base = ((cnt >> 8) & 0x1f) * 0x800;
    if (!sub)
        base += ((REG32(0) >> 27) & 7) * 0x10000;
    return base;
}

/* Game-visible BG pointers.  On the DS an unmapped VRAM window still has an address and writes
 * through it are dropped; a NULL here would instead turn tilemap/character writes into stores to
 * low EE memory, which is the kernel's (the opening subtitles did exactly that before the MobiClip
 * display setup was linked).  Unmapped windows therefore hand out a scratch sink. */
#define VRAM_SINK_BYTES 0x20000
static u8 g_vram_sink[VRAM_SINK_BYTES] __attribute__((aligned(64)));
static void *bg_ptr(int view, u32 ofs)
{
    u8 *p = kh_nitro_view_ptr(view, ofs);
    if (p)
        return p;
    KH_WARN("gx", "BG pointer into unmapped VRAM (view %d, offset 0x%x): writes are dropped", view, (unsigned)ofs);
    return g_vram_sink + (ofs & (VRAM_SINK_BYTES / 2 - 1));
}

/* VRAM write log for the register readout (nitro_render.c): the last writes that touched the
 * first 32 KiB of engine A's BG memory (where the dialog box characters live), so a screenshot
 * shows whether those characters were never written, written blank, or written and then wiped.
 * kind: L GX_Load*, F MI fill, M MI copy, D MI write dropped (bank not CPU-visible),
 * U GX_Load* into unmapped BG memory; tag: '0'..'3' BGn char load, '4'..'7' BG(n-4) screen load,
 * '-' other; nz: 1 data had a non-zero byte, 0 all zero, -1 unknown; repeat: how many identical
 * consecutive writes the entry stands for. */
KhVramLogEnt kh_vram_log_ring[KH_VRAM_LOG_N];
uint32_t kh_vram_log_count;
int kh_vram_log_on = 1;            /* cheap: filtered to 32 KiB of BG memory */

void kh_vram_log(char kind, char tag, uint32_t vofs, uint32_t size, int nz)
{
    KhVramLogEnt *e;
    u32 b0, ofs = vofs;
    if (!kh_vram_log_on)
        return;
    if (kind != 'U') {                      /* vofs is a kh_ds_vram offset: make it a BG one */
        b0 = kh_nitro_view_to_vram(VIEW_BG, 0);
        if (b0 == 0xffffffffu || vofs + size <= b0 || vofs >= b0 + 0x8000)
            return;
        ofs = vofs - b0;
    } else if (ofs >= 0x8000) {
        return;
    }
    if (kh_vram_log_count) {
        /* a write identical to the newest one (the dialogue text canvas is re-sent every few
         * frames) only refreshes it, so it cannot push the older writes out of the ring */
        e = &kh_vram_log_ring[(kh_vram_log_count - 1) % KH_VRAM_LOG_N];
        if (e->kind == kind && e->tag == tag && e->ofs == ofs && e->size == size && e->nz == nz) {
            e->vb = kh_vblank_count();
            e->repeat++;
            return;
        }
    }
    e = &kh_vram_log_ring[kh_vram_log_count++ % KH_VRAM_LOG_N];
    e->repeat = 1;
    e->vb = kh_vblank_count();
    e->ofs = ofs;
    e->size = size;
    e->kind = kind;
    e->tag = tag;
    e->nz = (signed char)nz;
}

void kh_vram_log_src(char kind, char tag, uint32_t vofs, uint32_t size, const void *src)
{
    const u8 *p = src;
    u32 i;
    int nz = 0;
    if (!kh_vram_log_on)
        return;
    for (i = 0; i < size && !nz; i++)
        nz = p[i] != 0;
    kh_vram_log(kind, tag, vofs, size, nz);
}

static void view_copy(int view, const void *src, u32 ofs, u32 size) { view_copy_tag(view, src, ofs, size, '-'); }

#define BGLOADS(N)                                                                                         \
    void GX_LoadBG##N##Char(const void *s, u32 o, u32 n)  { view_copy_tag(VIEW_BG, s, bg_char_base(0, N) + o, n, (char)('0' + N)); }     \
    void GX_LoadBG##N##Scr(const void *s, u32 o, u32 n)   { view_copy_tag(VIEW_BG, s, bg_scr_base(0, N) + o, n, (char)('4' + N)); }      \
    void GXS_LoadBG##N##Char(const void *s, u32 o, u32 n) { view_copy(VIEW_SUB_BG, s, bg_char_base(1, N) + o, n); } \
    void GXS_LoadBG##N##Scr(const void *s, u32 o, u32 n)  { view_copy(VIEW_SUB_BG, s, bg_scr_base(1, N) + o, n); }  \
    void *G2_GetBG##N##CharPtr(void)  { return bg_ptr(VIEW_BG, bg_char_base(0, N)); }                    \
    void *G2_GetBG##N##ScrPtr(void)   { return bg_ptr(VIEW_BG, bg_scr_base(0, N)); }                     \
    void *G2S_GetBG##N##CharPtr(void) { return bg_ptr(VIEW_SUB_BG, bg_char_base(1, N)); }                \
    void *G2S_GetBG##N##ScrPtr(void)  { return bg_ptr(VIEW_SUB_BG, bg_scr_base(1, N)); }
BGLOADS(0)
BGLOADS(1)
BGLOADS(2)
BGLOADS(3)

void GX_LoadOBJ(const void *s, u32 o, u32 n)  { view_copy(VIEW_OBJ, s, o, n); }
void GXS_LoadOBJ(const void *s, u32 o, u32 n) { view_copy(VIEW_SUB_OBJ, s, o, n); }
void GX_LoadOAM(const void *s, u32 o, u32 n)  { if (load_src_ok(s, o, n, -1)) memcpy(kh_ds_oam + o, s, n); }
void GXS_LoadOAM(const void *s, u32 o, u32 n) { if (load_src_ok(s, o, n, -2)) memcpy(kh_ds_oam + 0x400 + o, s, n); }
void GX_LoadBGPltt(const void *s, u32 o, u32 n)   { if (load_src_ok(s, o, n, -3)) memcpy(kh_ds_pal + o, s, n); }
void GX_LoadOBJPltt(const void *s, u32 o, u32 n)  { if (load_src_ok(s, o, n, -4)) memcpy(kh_ds_pal + 0x200 + o, s, n); }
void GXS_LoadBGPltt(const void *s, u32 o, u32 n)  { if (load_src_ok(s, o, n, -5)) memcpy(kh_ds_pal + 0x400 + o, s, n); }
void GXS_LoadOBJPltt(const void *s, u32 o, u32 n) { if (load_src_ok(s, o, n, -6)) memcpy(kh_ds_pal + 0x600 + o, s, n); }

/* extended palettes: loaded between Begin/End (the banks go to LCDC meanwhile on the DS) */
void GX_BeginLoadBGExtPltt(void) { }
void GX_LoadBGExtPltt(const void *s, u32 o, u32 n) { view_copy(VIEW_BGEXT, s, o, n); }
void GX_EndLoadBGExtPltt(void) { }
void GX_BeginLoadOBJExtPltt(void) { }
void GX_LoadOBJExtPltt(const void *s, u32 o, u32 n) { view_copy(VIEW_OBJEXT, s, o, n); }
void GX_EndLoadOBJExtPltt(void) { }
void GXS_BeginLoadBGExtPltt(void) { }
void GXS_LoadBGExtPltt(const void *s, u32 o, u32 n) { view_copy(VIEW_SUB_BGEXT, s, o, n); }
void GXS_EndLoadBGExtPltt(void) { }
void GXS_BeginLoadOBJExtPltt(void) { }
void GXS_LoadOBJExtPltt(const void *s, u32 o, u32 n) { view_copy(VIEW_SUB_OBJEXT, s, o, n); }
void GXS_EndLoadOBJExtPltt(void) { }

/* textures: the texture views change -> the GS residency cache must re-read what it cached */
void GX_BeginLoadTex(void) { }
void GX_LoadTex(const void *s, u32 o, u32 n) { view_copy(VIEW_TEX, s, o, n); kh_gfx_tex_dirty(o, n); }
void GX_EndLoadTex(void) { }
void GX_BeginLoadTexPltt(void) { }
void GX_LoadTexPltt(const void *s, u32 o, u32 n) { view_copy(VIEW_TEXPLTT, s, o, n); kh_gfx_pltt_dirty(o, n); }
void GX_EndLoadTexPltt(void) { }
void GX_BeginLoadClearImage(void) { }
void GX_EndLoadClearImage(void) { }

/* ---------------------------------------------------------- display control */

void GX_Init(void)
{
    memset(kh_ds_io, 0, 0x1100);
    memset(g_view_banks, 0, sizeof g_view_banks);
    REG16(0x304) = 0x820f;                 /* POWCNT: both engines + 3D on, A on top */
    KH_INFO("gx", "GX_Init");
}

void GX_SetGraphicsMode(u32 dispMode, u32 bgMode, u32 bg0_3d)
{
    u32 v = REG32(0);
    v = (v & ~0x000f000fu) | (dispMode << 16) | bgMode | (bg0_3d << 3);   /* display mode + VRAM block */
    REG32(0) = v;
}

void GXS_SetGraphicsMode(u32 bgMode)
{
    REG32(0x1000) = (REG32(0x1000) & ~7u) | bgMode;
}

/* DS behaviour: DispOff keeps the mode but blanks the screen until DispOn */
static u32 g_saved_dispmode;
static int g_disp_off;

void GX_DispOff(void)
{
    u32 v = REG32(0);
    if (!g_disp_off) {
        g_saved_dispmode = (v >> 16) & 3;
        g_disp_off = 1;
    }
    REG32(0) = v & ~0x30000u;
}

void GX_DispOn(void)
{
    if (g_disp_off) {
        REG32(0) = (REG32(0) & ~0x30000u) | (g_saved_dispmode << 16);
        g_disp_off = 0;
    }
}

int GX_IsDispOn(void) { return !g_disp_off; }

void GXS_DispOff(void) { REG32(0x1000) &= ~0x10000u; }
void GXS_DispOn(void)  { REG32(0x1000) |= 0x10000u; }

/* The SDK's register helpers take the register's address.  Game code passes the DS address as a
 * literal, which prep_sources.py (R5) has already turned into a pointer into kh_ds_io; a raw DS
 * address (should one still arrive) is translated the same way. */
static u32 io_off(u32 reg)
{
    if (reg >= 0x04000000u && reg < 0x04001100u)
        return reg - 0x04000000u;
    return reg - (u32)(uintptr_t)kh_ds_io;
}

void GXx_SetMasterBrightness_(u32 reg, int brightness)
{
    u16 v;
    if (brightness == 0)
        v = 0;
    else if (brightness > 0)
        v = (u16)(0x4000 | brightness);
    else
        v = (u16)(0x8000 | -brightness);
    REG16(io_off(reg)) = v;
}

int GXx_GetMasterBrightness_(u32 reg)
{
    u16 v = REG16(io_off(reg));
    if (v & 0x4000)
        return v & 0x1f;
    if (v & 0x8000)
        return -(int)(v & 0x1f);
    return 0;
}

/* blending: reg = BLDCNT address of the engine */
void G2x_SetBlendAlpha_(u32 reg, int plane1, int plane2, int ev1, int ev2)
{
    REG16(io_off(reg)) = (u16)((0x1 << 6) | plane1 | (plane2 << 8));
    REG16(io_off(reg) + 2) = (u16)(ev1 | (ev2 << 8));
}

void G2x_SetBlendBrightness_(u32 reg, int plane, int brightness)
{
    if (brightness < 0) {
        REG16(io_off(reg)) = (u16)((0x3 << 6) | plane);
        REG16(io_off(reg) + 4) = (u16)(-brightness);
    } else {
        REG16(io_off(reg)) = (u16)((0x2 << 6) | plane);
        REG16(io_off(reg) + 4) = (u16)brightness;
    }
}

void G2x_SetBlendBrightnessExt_(u32 reg, int plane1, int plane2, int ev1, int ev2, int brightness)
{
    REG16(io_off(reg) + 2) = (u16)(ev1 | (ev2 << 8));
    G2x_SetBlendBrightness_(reg, plane1, brightness);
    REG16(io_off(reg)) |= (u16)(plane2 << 8);
}

void G2x_ChangeBlendBrightness_(u32 reg, int brightness)
{
    u16 cnt = REG16(io_off(reg));
    if (brightness < 0) {
        if ((cnt & 0xc0) == 0x80)
            REG16(io_off(reg)) = (u16)((cnt & ~0xc0) | 0xc0);
        REG16(io_off(reg) + 4) = (u16)(-brightness);
    } else {
        if ((cnt & 0xc0) == 0xc0)
            REG16(io_off(reg)) = (u16)((cnt & ~0xc0) | 0x80);
        REG16(io_off(reg) + 4) = (u16)brightness;
    }
}

/* affine BG parameters: reg = BGxPA address; mtx is MtxFx22 (4 fx32), centre, offset */
typedef struct { s32 _00, _01, _10, _11; } MtxFx22;
void G2x_SetBGyAffine_(u32 reg, const MtxFx22 *m, int cx, int cy, int x1, int y1)
{
    s32 dx, dy;
    u32 o = io_off(reg);
    REG16(o + 0) = (u16)(m->_00 >> 4);
    REG16(o + 2) = (u16)(m->_01 >> 4);
    REG16(o + 4) = (u16)(m->_10 >> 4);
    REG16(o + 6) = (u16)(m->_11 >> 4);
    dx = x1 - cx;
    dy = y1 - cy;
    REG32(o + 8) = (u32)(((cx << 8) + (m->_00 * dx >> 4) + (m->_01 * dy >> 4)) & 0x0fffffff);
    REG32(o + 12) = (u32)(((cy << 8) + (m->_10 * dx >> 4) + (m->_11 * dy >> 4)) & 0x0fffffff);
}

void GX_VBlankIntr(int enable) { REG16(0x4) = (u16)((REG16(0x4) & ~0x8) | (enable ? 8 : 0)); }
void GX_HBlankIntr(int enable) { REG16(0x4) = (u16)((REG16(0x4) & ~0x10) | (enable ? 0x10 : 0)); }
void GX_SetDispSelect(int sel) { REG16(0x304) = (u16)((REG16(0x304) & ~0x8000) | (sel ? 0x8000 : 0)); }
void GX_SetPower(int p) { REG16(0x304) = (u16)((REG16(0x304) & ~0x20e) | (p & 0x20e)); }

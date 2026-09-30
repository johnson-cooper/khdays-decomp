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

/* ------------------------------------------------------------------ banks */

/* Byte offset in kh_ds_vram for offset `ofs` inside a view; ~0 if not mapped. */
uint32_t kh_nitro_view_to_vram(int view, uint32_t ofs)
{
    u32 banks = g_view_banks[view];
    int b;
    for (b = 0; b < NBANKS; b++) {
        if (!(banks & (1u << b)))
            continue;
        if (ofs < k_bank_size[b])
            return kh_ds_vram_bank_ofs[b] + ofs;
        ofs -= k_bank_size[b];
    }
    return 0xffffffffu;
}

u8 *kh_nitro_view_ptr(int view, uint32_t ofs)
{
    uint32_t v = kh_nitro_view_to_vram(view, ofs);
    return v == 0xffffffffu ? NULL : kh_ds_vram + v;
}

uint32_t kh_nitro_view_banks(int view) { return g_view_banks[view]; }

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

static u32 set_view(int view, u32 banks)
{
    u32 old = g_view_banks[view];
    unmap_banks(banks);
    g_view_banks[view] = banks;
    return old;
}

static u32 reset_view(int view)
{
    u32 old = g_view_banks[view];
    g_view_banks[view] = 0;
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
void GX_SetBankForLCDC(u32 b)          { g_view_banks[VIEW_LCDC] |= b; }

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

/* "Disable" = reset and give the banks to the LCDC view with their contents cleared, as the SDK
 * does (it maps them to LCDC and clears them). */
static u32 disable_view(int view)
{
    u32 old = reset_view(view);
    int b;
    for (b = 0; b < NBANKS; b++)
        if (old & (1u << b))
            memset(kh_ds_vram + kh_ds_vram_bank_ofs[b], 0, k_bank_size[b]);
    return old;
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

static void view_copy(int view, const void *src, u32 ofs, u32 size)
{
    const u8 *s = src;
    while (size) {
        u32 v = kh_nitro_view_to_vram(view, ofs);
        u32 n;
        int b;
        if (v == 0xffffffffu) {
            KH_WARN("gx", "load to unmapped VRAM (view %d, offset 0x%x)", view, (unsigned)ofs);
            return;
        }
        /* bytes left in this bank */
        for (b = 0; b < NBANKS && !(v >= kh_ds_vram_bank_ofs[b] && v < kh_ds_vram_bank_ofs[b] + k_bank_size[b]); b++)
            ;
        n = kh_ds_vram_bank_ofs[b] + k_bank_size[b] - v;
        if (n > size)
            n = size;
        memcpy(kh_ds_vram + v, s, n);
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
    u32 base = ((cnt >> 8) & 0x1f) * 0x800;
    if (!sub)
        base += ((REG32(0) >> 27) & 7) * 0x10000;
    return base;
}

#define BGLOADS(N)                                                                                         \
    void GX_LoadBG##N##Char(const void *s, u32 o, u32 n)  { view_copy(VIEW_BG, s, bg_char_base(0, N) + o, n); }     \
    void GX_LoadBG##N##Scr(const void *s, u32 o, u32 n)   { view_copy(VIEW_BG, s, bg_scr_base(0, N) + o, n); }      \
    void GXS_LoadBG##N##Char(const void *s, u32 o, u32 n) { view_copy(VIEW_SUB_BG, s, bg_char_base(1, N) + o, n); } \
    void GXS_LoadBG##N##Scr(const void *s, u32 o, u32 n)  { view_copy(VIEW_SUB_BG, s, bg_scr_base(1, N) + o, n); }  \
    void *G2_GetBG##N##CharPtr(void)  { return kh_nitro_view_ptr(VIEW_BG, bg_char_base(0, N)); }                    \
    void *G2_GetBG##N##ScrPtr(void)   { return kh_nitro_view_ptr(VIEW_BG, bg_scr_base(0, N)); }                     \
    void *G2S_GetBG##N##CharPtr(void) { return kh_nitro_view_ptr(VIEW_SUB_BG, bg_char_base(1, N)); }                \
    void *G2S_GetBG##N##ScrPtr(void)  { return kh_nitro_view_ptr(VIEW_SUB_BG, bg_scr_base(1, N)); }
BGLOADS(0)
BGLOADS(1)
BGLOADS(2)
BGLOADS(3)

void GX_LoadOBJ(const void *s, u32 o, u32 n)  { view_copy(VIEW_OBJ, s, o, n); }
void GXS_LoadOBJ(const void *s, u32 o, u32 n) { view_copy(VIEW_SUB_OBJ, s, o, n); }
void GX_LoadOAM(const void *s, u32 o, u32 n)  { memcpy(kh_ds_oam + o, s, n); }
void GXS_LoadOAM(const void *s, u32 o, u32 n) { memcpy(kh_ds_oam + 0x400 + o, s, n); }
void GX_LoadBGPltt(const void *s, u32 o, u32 n)   { memcpy(kh_ds_pal + o, s, n); }
void GX_LoadOBJPltt(const void *s, u32 o, u32 n)  { memcpy(kh_ds_pal + 0x200 + o, s, n); }
void GXS_LoadBGPltt(const void *s, u32 o, u32 n)  { memcpy(kh_ds_pal + 0x400 + o, s, n); }
void GXS_LoadOBJPltt(const void *s, u32 o, u32 n) { memcpy(kh_ds_pal + 0x600 + o, s, n); }

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
    v = (v & ~0x0003000fu) | (dispMode << 16) | bgMode | (bg0_3d << 3);
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

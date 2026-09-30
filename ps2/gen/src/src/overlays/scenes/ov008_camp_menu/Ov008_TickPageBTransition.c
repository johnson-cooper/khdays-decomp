/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_TickPageBTransition.c (ps2/tools/prep_sources.py). Do not edit. */
/* Ov008_TickPageBTransition -- Ov008_TickPageBTransition (632 B, 23 relocs).
 * Per-frame tick for a menu page-B slide transition. Saves the coprocessor context, then (unless
 * bit 2 of ctx->field34 is set) samples the slide tween, publishes the scroll amount to the sub
 * engine's BG2 scroll registers (0x04001010), and lays out three list entries (ids 0x2c, 0x2b, 2)
 * whose horizontal offsets come from two fixed-point products: FX_Inv of ((0xf0000 - entry.f4) *
 * sample) and FX_Inv(sample) scaled by -0x30000, both accumulated into a shared offset point. When
 * ctx->field04 is set it reloads the BG2 screen/char VRAM (DC_FlushRange + GXS_LoadBG2Scr/Char).
 * Finally it writes the BG3 scroll registers (0x04001018) from field38/field3c, restores the
 * coprocessor context, and spins until the hardware divider (DIVCNT 0x04000280) is idle.
 *
 * Ov008_OffsetActiveListEntries is 3-arg (owner, &point, 0): the ROM's r3 at the call still holds the
 * 0x04001010 register pointer from the scroll writes, which Ghidra mistook for a 4th argument. */

#include "nitro/types.h"

typedef struct Point { int x, y; } Point;

typedef struct BlockB { u8 pad00[0x20]; void *vram; } BlockB;

typedef struct Ov008PageBCtx {
    u8   pad00[4];
    int  field04;                 /* 0x04 */
    u8   pad08[0x1c - 0x08];
    u8   fade[0x18];              /* 0x1c: slide tween object */
    struct { unsigned lo : 2; unsigned bit2 : 1; unsigned hi : 29; } field34; /* 0x34 */
    int  field38;                 /* 0x38 */
    int  field3c;                 /* 0x3c */
    int  field40;                 /* 0x40 */
    u8   pad44[0x1a8 - 0x44];
    BlockB *field1a8;             /* 0x1a8 */
} Ov008PageBCtx;

extern Ov008PageBCtx *Ov008_GetPageB(void);
extern void  CP_SaveContext(void *ctx);
extern void  CPi_RestoreContext(const void *ctx);
extern void  Tween_Sample(void *tween, int *out);
extern void  Ov008_SetCtxField962c(int a);
extern int   Ov008_GetCtxBlock4a80(void);
extern void  Ov008_OffsetActiveListEntries(int a, Point *pt, int b);
extern int   Ov008_FindEntryById(int ctx, int id);
extern int   Ov008_GetEntryBlock2c(int ctx, int entry);
extern int   FX_Div(int num, int den);
extern void  Ov008_ApplyOffsetSum(int ctx, int entry, Point *pt);
extern void  Ov008_CallSelectionHandler(int ctx, int a);
extern void *Ov008_ResetEntry(int id);
extern void  DC_FlushRange(u32 addr, u32 size);
extern void  GXS_LoadBG2Scr(const void *src, u32 off, u32 size);
extern void  GXS_LoadBG2Char(const void *src, u32 off, u32 size);

int Ov008_TickPageBTransition(void)
{
    u32   cpctx[7];
    struct { Point pt; int sample; } v;
    Ov008PageBCtx *ctx = Ov008_GetPageB();

    CP_SaveContext(cpctx);
    if (ctx->field34.bit2 == 0) {
        volatile u32 *reg = (volatile u32 *)((unsigned int)kh_ds_io + 0x1010);
        int e2c, e2b, iVar9, inv, owner;

        Tween_Sample(&ctx->fade, &v.sample);
        ctx->field38 = v.sample >> 0xc;
        reg[0] = 0x1ff & ctx->field38;
        reg[1] = ctx->field38 & 0x1ff;
        v.pt.y = 0;
        v.pt.x = -v.sample;
        owner = Ov008_GetCtxBlock4a80();
        Ov008_OffsetActiveListEntries(owner, &v.pt, 0);
        e2c = Ov008_FindEntryById(owner, 0x2c);
        e2b = Ov008_FindEntryById(owner, 0x2b);
        iVar9 = 0xf0000 - *(int *)(Ov008_GetEntryBlock2c(owner, e2c) + 4);
        inv = FX_Div((int)(((long long)iVar9 * v.sample + 0x800) >> 0xc), 0xc0000);
        v.pt.x = -v.sample;
        v.pt.y = inv;
        Ov008_ApplyOffsetSum(owner, e2c, &v.pt);
        v.pt.y = iVar9 - inv;
        Ov008_ApplyOffsetSum(owner, e2b, &v.pt);
        inv = FX_Div(v.sample, 0xc0000);
        v.pt.x = v.pt.x + (int)(((long long)inv * -0x30000 + 0x800) >> 0xc);
        v.pt.y = 0;
        Ov008_ApplyOffsetSum(owner, Ov008_FindEntryById(owner, 2), &v.pt);
        Ov008_CallSelectionHandler(owner, 0);
    } else {
        Ov008_SetCtxField962c(0);
    }
    if (ctx->field04 != 0) {
        void *p;
        ctx->field3c = ctx->field40;
        p = Ov008_ResetEntry(0x1a);
        DC_FlushRange((u32)p, 0x800);
        DC_FlushRange((u32)ctx->field1a8->vram, 0xa000);
        p = Ov008_ResetEntry(0x1a);
        GXS_LoadBG2Scr(p, 0, 0x800);
        GXS_LoadBG2Char(ctx->field1a8->vram, 0x4000, 0xa000);
        ctx->field04 = 0;
    }
    {
        volatile u32 *reg = (volatile u32 *)((unsigned int)kh_ds_io + 0x1018);
        reg[0] = ((ctx->field38 - 8) & 0x1ff) | (0x1ff0000 & ((ctx->field3c - 8) << 16));
        reg[1] = (ctx->field38 & 0x1ff) | (0x1ff0000 & ((ctx->field3c - 8) << 16));
    }
    CPi_RestoreContext(cpctx);
    while (*(volatile u16 *)((unsigned int)kh_ds_io + 0x280) & 0x8000)
        ;
    return 1;
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_HudSetSlotValue.c (ps2/tools/prep_sources.py). Do not edit. */
/* Publish one key/value pair into the session's slot table, and if it actually
 * changed, re-run the screen refresh for the local player.
 *
 * The `>> 32` below is NOT arithmetic on a real 64-bit value.  kh_rt_s32_divmod is
 * the MetroWerks runtime signed divide (`_s32_div_f`): it returns the QUOTIENT in
 * r0 and the REMAINDER in r1, so it is really the same routine mwcc emits for
 * both `/` and `%`.  Declaring it `long long` and taking the high half is how you
 * name the remainder from C.  The statement means
 *
 *     Ov002_PanelRepaintForKind(ctx->bPlayer, ctx->bChannel % 6, 0);
 *
 * and writing it that way compiles to the identical 216 bytes -- but mwcc then
 * names the reloc `_s32_div_f`, while config/arm9/symbols.txt calls 0x02020400
 * `kh_rt_s32_divmod`, so the link would not resolve.  ~90 matched files already call
 * this helper by address for the quotient; this is the first one that needed the
 * remainder.  See dudas.md.
 *
 * The refresh block is emitted twice inside the case-1 arm, which is why it is a
 * `static inline` helper rather than a plain call: -inline on,noauto expands it
 * at both sites the way the original does. */
typedef struct {
    unsigned char bKey;      /* +0x00 */
    unsigned char bValue;    /* +0x01 */
} Ov002HudSlot;

typedef struct {
    char pad00[1];
    unsigned char bPlayer;    /* +0x01 */
    unsigned char bChannel;   /* +0x02 */
    char pad03[0x2d];
    unsigned char bSlotCount; /* +0x30 */
    unsigned char bRow;       /* +0x31 */
    Ov002HudSlot aSlots[1];      /* +0x32 -- bSlotCount entries, stride 2 */
} Ov002HudContext;

extern void Ov002_RebuildPanelSlots(int player);
extern int Ov002_ClassifyCode(int *out, int player);
extern void Ov002_PanelRefreshAllRowHeaders(void);
extern void Ov002_RedrawPartyStrip(void);
extern void Ov002_PanelRepaintGroup(int a);
extern void Ov002_DrawListSpanStrip(int a, int b, int c, int d);
extern int Ov002_Ctx_FindActiveEntryByTag(int id);
extern int Ov002_ForwardToSubDc_4(int hEntry);
extern int Ov002_ForwardToSubDc(int id);
extern void Ov002_ForwardToSubDc_2(int handle);
extern long long kh_rt_s32_divmod(int a, int b);
extern void Ov002_PanelRepaintForKind(int a, int b, int c);

extern Ov002HudContext *data_ov002_0207f620;

static inline void Ov002_HudRefreshMemberPanel(Ov002HudContext *ctx) {
    if (Ov002_ForwardToSubDc_4(Ov002_Ctx_FindActiveEntryByTag(0xe)) == 0) {
        Ov002_ForwardToSubDc_2(Ov002_ForwardToSubDc(0x79));
    }
    Ov002_PanelRepaintForKind(ctx->bPlayer,
                        (int)(kh_rt_s32_divmod(ctx->bChannel, 6) >> 32), 0);
}

void Ov002_HudSetSlotValue(int key, int value) {
    Ov002HudContext *ctx = data_ov002_0207f620;
    int changed = 0;
    int i;
    int out;

    for (i = 0; i < ctx->bSlotCount; i++) {
        if (key == ctx->aSlots[i].bKey) {
            if (value == ctx->aSlots[i].bValue) break;
            ctx->aSlots[i].bValue = value;
            changed = 1;
            break;
        }
    }
    if (changed == 0) return;

    Ov002_RebuildPanelSlots(ctx->bPlayer);
    switch (Ov002_ClassifyCode(&out, ctx->bPlayer)) {
    case 0:
        Ov002_PanelRefreshAllRowHeaders();
        break;
    case 1:
        Ov002_HudRefreshMemberPanel(ctx);
        Ov002_PanelRepaintGroup(out);
        Ov002_DrawListSpanStrip(out + 1, ctx->bRow, 7, 0xb);
        Ov002_HudRefreshMemberPanel(ctx);
        break;
    case 4:
        Ov002_RedrawPartyStrip();
        break;
    }
}

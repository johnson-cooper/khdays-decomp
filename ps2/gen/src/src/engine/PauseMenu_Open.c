/* PS2: mechanically prepared copy of src/engine/PauseMenu_Open.c (ps2/tools/prep_sources.py). Do not edit. */
#pragma thumb on
/* PauseMenu_Open -- pause menu opening step, MAIN (THUMB). Without gPauseAllowed or game field 0x20ef
 * it pushes step 0 and stops; while the opening delay (+0xcc) runs it counts down. Otherwise it sets
 * the menu BG (BG3, or BG2 in mode bit 1) to text 256x256 at screen base 0xf800 / char base 0xc000
 * and clears its screen, redraws the panels (unless game field 0x2483 is set), loads the menu screen
 * (func_02013408 with the resources at +0xa0/+0xa4/+0xa8), shows BG0 with the menu BG, darkens
 * both screens by 8 (the sub screen to the overlay's darker level in mode 0x2a), pauses the channels
 * outside the mode-bit-1 case of data_0204c240 bit 2, plays sound 2 and queues "pause_refresh". */

#include "nitro/types.h"

typedef struct {
    int debounce;                       /* +0x00 */
    int selected;                       /* +0x04 */
} PanelSlot;

typedef struct {
    char pad0000[0xc];
    char panel[0x94];                   /* +0x0c -- opaque, passed to SubObject_NudgeAndRedraw */
    void *screenData;                   /* +0xa0 */
    void *charData;                     /* +0xa4 */
    void *plttData;                     /* +0xa8 */
    PanelSlot slots[3];                 /* +0xac */
    int field_c4;
    int timer;                          /* +0xc8 */
    int openDelay;                      /* +0xcc */
    int count;                          /* +0xd0 */
} PauseContext;

typedef struct {
    char pad0000[4];
    PauseContext *pCtx;                 /* +0x04 */
} Root0204be08;

extern Root0204be08 data_0204be08;
extern unsigned char gPauseAllowed;
extern unsigned char data_0204c240;
extern char gPauseRefreshName[];            /* "pause_refresh" */

extern int GameState_IsFlagSet(int flag);                 /* GameState_IsFlagSet */
extern void PauseMenu_SetMode(int step);
extern int LoadGlobalU16At0(void);
extern void *G2_GetBG3ScrPtr(void);
extern void *G2_GetBG2ScrPtr(void);
extern void MIi_CpuClearFast(u32 value, void *dst, u32 size);
extern void SubObject_SetupDrawsForMode(void);
extern void SubObject_NudgeAndRedraw(void *panel, int index, int state);
extern void func_02013408(int bg, const void *pScreenData, const void *pCharacterData, const void *pPaletteData,
                          const void *pPositionInfo, const void *pCompressInfo, int screenBase, int characterBase);
extern void G2x_SetBlendBrightness_(vu16 *reg, int plane, int brightness);
extern int Ov106_GetBrightness(void);
extern void SNDi_BroadcastChannelOp(int op);
extern void PlaySound(int a, int b);            /* play menu sound */
extern void VBlank_UnregisterCallback(int mode, const void *descriptor);

#define reg_GX_DISPCNT      (*(vu32 *)((unsigned int)kh_ds_io + 0x0))
#define reg_G2_BG2CNT       (*(vu16 *)((unsigned int)kh_ds_io + 0xc))
#define reg_G2_BG3CNT       (*(vu16 *)((unsigned int)kh_ds_io + 0xe))
#define reg_G2_BLDCNT       (*(vu16 *)((unsigned int)kh_ds_io + 0x50))
#define reg_GXS_DB_DISPCNT  (*(vu32 *)((unsigned int)kh_ds_io + 0x1000))
#define reg_G2S_DB_BLDCNT   (*(vu16 *)((unsigned int)kh_ds_io + 0x1050))

static inline void G2_SetBG2Control(int screenSize, int colorMode, int screenBase, int charBase, int bgExtPltt)
{
    reg_G2_BG2CNT = (u16)((reg_G2_BG2CNT & 0x43) | (screenSize << 14) | (colorMode << 7) |
                          (screenBase << 8) | (charBase << 2) | (bgExtPltt << 13));
}

static inline void G2_SetBG3Control(int screenSize, int colorMode, int screenBase, int charBase, int bgExtPltt)
{
    reg_G2_BG3CNT = (u16)((reg_G2_BG3CNT & 0x43) | (screenSize << 14) | (colorMode << 7) |
                          (screenBase << 8) | (charBase << 2) | (bgExtPltt << 13));
}

static inline void GX_SetVisiblePlane(int plane)
{
    reg_GX_DISPCNT = (reg_GX_DISPCNT & ~0x1f00) | (plane << 8);
}

static inline int GXS_GetVisiblePlane(void)
{
    return (int)((reg_GXS_DB_DISPCNT & 0x1f00) >> 8);
}

static inline void G2_SetBlendBrightness(int plane, int brightness)
{
    G2x_SetBlendBrightness_(&reg_G2_BLDCNT, plane, brightness);
}

static inline void G2S_SetBlendBrightness(int plane, int brightness)
{
    G2x_SetBlendBrightness_(&reg_G2S_DB_BLDCNT, plane, brightness);
}

void PauseMenu_Open(void)
{
    PauseContext *ctx = data_0204be08.pCtx;
    int i;

    if (gPauseAllowed == 0 && GameState_IsFlagSet(0x20ef) == 0) {
        PauseMenu_SetMode(0);
        return;
    }
    if (ctx->openDelay == 0) {
        if (!(LoadGlobalU16At0() & 2)) {
            G2_SetBG3Control(0, 0, 0x1f, 3, 0);
            MIi_CpuClearFast(0, G2_GetBG3ScrPtr(), 0x800);
        } else {
            G2_SetBG2Control(0, 0, 0x1f, 3, 0);
            MIi_CpuClearFast(0, G2_GetBG2ScrPtr(), 0x800);
        }
        SubObject_SetupDrawsForMode();
        if (GameState_IsFlagSet(0x2483) == 0) {
            for (i = 0; i < ctx->count; i++) {
                SubObject_NudgeAndRedraw(ctx->panel, i, ctx->slots[i].selected);
            }
        }
        func_02013408(3, ctx->screenData, ctx->charData, ctx->plttData, 0, 0, 0x1f, 3);
        if (!(LoadGlobalU16At0() & 2)) {
            GX_SetVisiblePlane(9);
        } else {
            GX_SetVisiblePlane(5);
        }
        G2_SetBlendBrightness(1, -8);
        G2S_SetBlendBrightness(GXS_GetVisiblePlane(), -8);
        if (LoadGlobalU16At0() == 0x2a && Ov106_GetBrightness() < -8) {
            G2S_SetBlendBrightness(1, Ov106_GetBrightness());
        }
        if (!(data_0204c240 & 4) || !(LoadGlobalU16At0() & 2)) {
            SNDi_BroadcastChannelOp(1);
        }
        PlaySound(0, 2);
        VBlank_UnregisterCallback(1, gPauseRefreshName);
    } else {
        ctx->openDelay--;
    }
}
#pragma thumb off

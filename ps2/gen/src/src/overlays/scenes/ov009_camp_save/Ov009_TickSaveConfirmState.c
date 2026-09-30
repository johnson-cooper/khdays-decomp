/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/Ov009_TickSaveConfirmState.c (ps2/tools/prep_sources.py). Do not edit. */
/* Save confirm state: shows the confirmation, and on confirm prepares and starts the save; handles
 * cancel. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov009SaveChoiceVisual {
    int value;
    u8 pad04[0x04];
} Ov009SaveChoiceVisual;

typedef struct Ov009SaveContext {
    int variant;
    int nextVariant;
    int state;
    u8 pad00c[0x5c];
    int pending;
    u8 pad06c[0xd8];
    Ov009SaveChoiceVisual visuals[3];
    u8 pad15c[0xe4];
    int interactionLock;
    int field244;
} Ov009SaveContext;

extern void Ov009_GetContext(void);
extern void Ov009_SetMenuEntriesVisible(int enabled, int mode);
extern void Ov009_DrawMenuText(Ov009SaveContext *ctx, int mode);
extern int Ov009_CommitSaveFields(Ov009SaveContext *ctx, int slot);
extern void Ov009_SetCtxField95fc(int value);
extern void Ov009_StartScreenTransition(int a, int b);

#define REG_DISPCNT (*(volatile u32 *)((unsigned int)kh_ds_io + 0x0))

void Ov009_TickSaveConfirmState(Ov009SaveContext *ctx)
{
    int sound = 1;
    int i;

    Ov009_GetContext();
    if (ctx->interactionLock != 0) {
        return;
    }

    switch (ctx->state) {
    case 0:
        Ov009_SetMenuEntriesVisible(0, 1);
        Ov009_DrawMenuText(ctx, 1);
        ctx->pending = 0;
        for (i = 0; i < 3; i++) {
            if (i == ctx->variant) {
                ctx->visuals[i].value = 0;
            } else {
                ctx->visuals[i].value = 0x100000;
            }
        }
        ctx->state = 1;
        break;

    case 1:
        sound = ctx->pending;
        if (sound != 0) {
            Sleep_Block();
            if (Ov009_CommitSaveFields(ctx, ctx->variant) == 0) {
                ctx->interactionLock = 1;
                ctx->field244 = 1;
            }
            ctx->state = 2;
            Ov009_SetMenuEntriesVisible(0, 0);
            Ov009_DrawMenuText(ctx, 2);
            Ov009_SetCtxField95fc(0);
        } else {
            Ov009_SetMenuEntriesVisible(1, 0);
            for (i = 0; i < 3; i++) {
                ctx->visuals[i].value = 0;
            }
            Ov009_DrawMenuText(ctx, 0);
            ctx->state = 0;
        }
        break;

    case 2:
    case 3:
        break;

    case 4:
        Ov009_StartScreenTransition(-1, -1);
        break;

    case 5:
        break;

    case 6:
        sound = ctx->pending;
        if (sound != 0) {
            Ov009_StartScreenTransition(-1, -1);
        } else {
            Ov009_SetMenuEntriesVisible(1, 0);
            Ov009_DrawMenuText(ctx, 0);
            for (i = 0; i < 3; i++) {
                ctx->visuals[i].value = 0;
            }
            ctx->variant = ctx->nextVariant;
            ctx->state = 0;
            {
                u32 displayMode = (REG_DISPCNT & 0x1f00) >> 8;
                u32 clearedDisplay = REG_DISPCNT & 0xffffe0ff;

                REG_DISPCNT =
                    clearedDisplay | (displayMode | 4) << 8;
            }
        }
        break;
    }

    PlaySound(0, sound != 0 ? 1 : 3);
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov008_camp_menu/Ov008_UpdateBrightnessAndCallbacks.c (ps2/tools/prep_sources.py). Do not edit. */
/* Per-frame camp-menu update: samples the brightness tween and applies it to the screens it drives
 * (dimming the main screen in the save mode), then runs the update callbacks, dropping the ones
 * that finish. */

#include "nitro/types.h"
#include "game/engine.h"

typedef union Ov008DisplayFlags {
    u32 raw;
    struct {
        u32 bits_0_1 : 2;
        u32 skipBrightnessUpdate : 1;
        u32 rest : 29;
    } bits;
} Ov008DisplayFlags;

typedef struct Ov008UpdateNode {
    int (*update)(void);
} Ov008UpdateNode;

extern int data_ov008_02090f04[];

#define CTXV (*(volatile int *)((char *)data_ov008_02090f04 + 4))

extern void CP_SaveContext(void *context);
extern void CPi_RestoreContext(void *context);
extern void Tween_Sample(void *value, void *state);
extern int Ov008_GetCtxField95cc(void);
extern int Ov008_IsContextMode4(void);
extern void *NNS_FndGetNextListObject(void *list, void *previous);
extern void Ov008_ListRemoveAndFree(void *node);

#define REG_DIV_CNT (*(volatile u16 *)((unsigned int)kh_ds_io + 0x280))

void Ov008_UpdateBrightnessAndCallbacks(void)
{
    u32 cpContext[7];
    volatile u16 *divControl;
    int brightness;
    Ov008UpdateNode *node;
    Ov008UpdateNode *next;

    *(int *)(CTXV + 0x95f4) = 0;

    if (((Ov008DisplayFlags *)(CTXV + 0x95f0))->bits.skipBrightnessUpdate == 0) {
        int tweenContext;

        CP_SaveContext(cpContext);
        tweenContext = CTXV;
        Tween_Sample((void *)(tweenContext + 0x95d8),
                      (void *)(tweenContext + 0x95d4));
        CPi_RestoreContext(cpContext);

        divControl = &REG_DIV_CNT;
        while ((*divControl & 0x8000) != 0) {
        }

        brightness = *(int *)(CTXV + 0x95d4) >> 12;
        if (Ov008_GetCtxField95cc() == 6 ||
            Ov008_GetCtxField95cc() == 7) {
            SetMasterBrightnessSub(brightness);
        } else {
            if (*(int *)(CTXV + 0x95f8) != 0) {
                SetMasterBrightnessSub(brightness);
            }
            if (Ov008_IsContextMode4() != 0 &&
                *(int *)(CTXV + 0x974c) == 0 && brightness > -8) {
                brightness = -8;
            }
            SetMasterBrightnessMain(brightness);
        }
    }

    node = NNS_FndGetNextListObject((void *)(CTXV + 0x9660), 0);
    if (node == 0) {
        return;
    }

    do {
        next = NNS_FndGetNextListObject((void *)(CTXV + 0x9660), node);
        if (node->update() == 0) {
            Ov008_ListRemoveAndFree(node);
        }
        node = next;
    } while (next != 0);
}

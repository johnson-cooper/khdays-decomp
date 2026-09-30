/* PS2: mechanically prepared copy of src/overlays/scenes/ov009_camp_save/Ov009_Menu_VBlankTick.c (ps2/tools/prep_sources.py). Do not edit. */
/* Applies the brightness tween to the screens and updates the active widgets (dropping the finished
 * ones). */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Ov009MenuContext {
    u8 pad_0000[0x95d4];
    int brightness;
    u8 brightnessTween[0x18];
    u32 field_95f0_bits_0_1 : 2;
    u32 skipBrightnessUpdate : 1;
    u32 field_95f0_bits_3_31 : 29;
    int field_95f4;
    int mirrorSubBrightness;
    u8 pad_95fc[0x9660 - 0x95fc];
    u8 activeWidgetList[1];
} Ov009MenuContext;

typedef struct Ov009Widget {
    int (*update)(void);
} Ov009Widget;

extern Ov009MenuContext *volatile data_ov009_020563e4[];

#define OV009_CONTEXT (data_ov009_020563e4[1])

extern void  CP_SaveContext(void *context);
extern void  CPi_RestoreContext(const void *context);
extern void  Tween_Sample(void *tween, int *value);
extern int   Ov009_GetCtxField95cc(void);
extern void *NNS_FndGetNextListObject(void *list, void *previous);
extern void  Ov009_ListRemoveAndFree(void *widget);

static volatile unsigned short *const REG_DIVCNT =
    (volatile unsigned short *)((unsigned int)kh_ds_io + 0x280);

void Ov009_Menu_VBlankTick(void)
{
    u32 cpContext[7];
    int brightness;
    Ov009Widget *widget;
    Ov009Widget *next;

    OV009_CONTEXT->field_95f4 = 0;

    if (OV009_CONTEXT->skipBrightnessUpdate == 0) {
        CP_SaveContext(cpContext);
        {
            Ov009MenuContext *context = OV009_CONTEXT;
            Tween_Sample(
                context->brightnessTween,
                &context->brightness);
        }
        CPi_RestoreContext(cpContext);

        while ((*REG_DIVCNT & 0x8000) != 0) {
        }

        brightness = OV009_CONTEXT->brightness >> 12;
        if (Ov009_GetCtxField95cc() == 6 ||
            Ov009_GetCtxField95cc() == 7) {
            SetMasterBrightnessSub(brightness);
        } else {
            if (OV009_CONTEXT->mirrorSubBrightness != 0) {
                SetMasterBrightnessSub(brightness);
            }
            SetMasterBrightnessMain(brightness);
        }
    }

    widget = NNS_FndGetNextListObject(
        OV009_CONTEXT->activeWidgetList, 0);
    if (widget == 0) {
        return;
    }

    do {
        next = NNS_FndGetNextListObject(
            OV009_CONTEXT->activeWidgetList, widget);
        if (widget->update() == 0) {
            Ov009_ListRemoveAndFree(widget);
        }
        widget = next;
    } while (next != 0);
}

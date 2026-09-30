/* PS2: mechanically prepared copy of src/overlays/scenes/ov006_mission_mode_select/Ov006_UpdateCharacterSelectTweens.c (ps2/tools/prep_sources.py). Do not edit. */
/* Sample active Mission Mode UI tweens and apply their values. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct Tween {
    int mode;
    int duration;
    int from;
    int to;
    long long startTick;
    u32 flags;
} Tween;

typedef struct Ov006CharacterSelectContext {
    u8 opaque0000[0x94f4];
    int currentState;
    u8 reserved94f8[0x4c];
    Tween masterBrightnessTween;
    u8 reserved9560[8];
    u32 masterBrightnessSettled;
    u32 transitionPending;
    u8 dirtyCellBufferMask;
    u8 reserved9571[3];
    Tween blendAlphaTween;
    Tween bgOffsetTween;
    u8 reserved95ac[0x3c];
    int primarySlotIds[4];
    int secondarySlotIds[3];
    u8 reserved9604[0x10];
    Tween primarySlotTweens[4];
    Tween secondarySlotTweens[4];
} Ov006CharacterSelectContext;

extern Ov006CharacterSelectContext *data_ov006_02056664;
extern void Tween_Sample(Tween *tween, int *value);
extern void G2x_SetBlendAlpha_(u32 reg, int plane1, int plane2, int eva, int evb);
extern void Slot_ClearFlagBit1(void *slots, int entryId);

void Ov006_UpdateCharacterSelectTweens(void) {
    int masterBrightness = 0;
    int slotValue = 0;
    int blendAlpha = 0;
    int bgOffset = 0;
    int index;
    Ov006CharacterSelectContext *context;
    int brightnessSettled;
    int tweenOffset;
    Tween *slotTween;

    brightnessSettled = 1;
    if (((data_ov006_02056664->masterBrightnessTween.flags << 0x1d) >> 0x1f) == 0) {
        Tween_Sample(&data_ov006_02056664->masterBrightnessTween, &masterBrightness);
        SetMasterBrightnessMain(masterBrightness >> 12);
        SetMasterBrightnessSub(masterBrightness >> 12);
        brightnessSettled = 0;
    }

    context = data_ov006_02056664;
    switch (context->currentState) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        break;
    case 5:
    case 6:
        if (((context->blendAlphaTween.flags << 0x1d) >> 0x1f) == 0) {
            Tween_Sample(&context->blendAlphaTween, &blendAlpha);
            G2x_SetBlendAlpha_(((unsigned int)kh_ds_io + 0x50), 1, 4, blendAlpha >> 12,
                               16 - (blendAlpha >> 12));
        }
        if (((data_ov006_02056664->bgOffsetTween.flags << 0x1d) >> 0x1f) == 0) {
            Tween_Sample(&data_ov006_02056664->bgOffsetTween, &bgOffset);
            *(volatile u32 *)((unsigned int)kh_ds_io + 0x10) = 0x1ff & (bgOffset >> 12);
        }

        index = 0;
        tweenOffset = 0;
        do {
            slotTween = (Tween *)((u8 *)&data_ov006_02056664->primarySlotTweens[0] + tweenOffset);
            if (((slotTween->flags << 0x1d) >> 0x1f) == 0) {
                Tween_Sample(slotTween, &slotValue);
                Slot_ForwardToEntry((u8 *)data_ov006_02056664 + 0x54,
                              data_ov006_02056664->primarySlotIds[index],
                              (u32)(slotValue << 4) >> 16);
                Slot_ClearFlagBit1((u8 *)data_ov006_02056664 + 0x54,
                              data_ov006_02056664->primarySlotIds[index]);
            }
            index++;
            tweenOffset += 0x1c;
        } while (index < 4);

        tweenOffset = 0;
        index = 0;
        do {
            slotTween = (Tween *)((u8 *)&data_ov006_02056664->secondarySlotTweens[0] + index);
            if (((slotTween->flags << 0x1d) >> 0x1f) == 0) {
                Tween_Sample(slotTween, &slotValue);
                Slot_ForwardToEntry((u8 *)data_ov006_02056664 + 0x54,
                              data_ov006_02056664->secondarySlotIds[tweenOffset],
                              (u32)(slotValue << 4) >> 16);
                Slot_ClearFlagBit1((u8 *)data_ov006_02056664 + 0x54,
                              data_ov006_02056664->secondarySlotIds[tweenOffset]);
            }
            tweenOffset++;
            index += 0x1c;
        } while (tweenOffset < 3);
        break;
    }

    data_ov006_02056664->masterBrightnessSettled = brightnessSettled;
}
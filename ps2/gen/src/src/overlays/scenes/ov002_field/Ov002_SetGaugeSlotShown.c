/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SetGaugeSlotShown.c (ps2/tools/prep_sources.py). Do not edit. */
/* Turn one fade slot on or off.
 *
 * The value the slot reports is the gauge percentage: slot 0 reads it straight
 * out of the context, the others scale their current amount against their
 * maximum through the 64-bit divide helper. A zero result is nudged to 1 so a
 * non-empty gauge never reads as empty.
 *
 * Turning a slot on stamps the tick and arms it for half a second; turning it
 * off hands the value to the redraw with the slot's own callback. Slot 0 also
 * owns the looping sound: it starts on the way up and stops on the way down,
 * unless the shutdown hook has already taken over.
 */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct {
    unsigned long long qwStart;         /* +0x00 */
    int nDuration;                      /* +0x08 */
    int nPhase;                         /* +0x0c */
    int bActive;                        /* +0x10 */
    int nField0014;                     /* +0x14 */
} Ov002FadeSlot;

extern char *data_ov002_0207f618;
extern int data_ov002_0207de04;

extern void Ov002_DrawGaugeTweenCell(void);
extern void Ov002_DrawShortLayoutRow(void);
extern long long kh_rt_s32_divmod(int nNumerator, int nDenominator);
extern unsigned long long OS_GetTick(void);
extern void Ov002_AdvanceGaugeSlot(int nIndex, Ov002FadeSlot *pSlot);
extern void Ov002_RunPresetCallbacks(int nHandle, int nIndex, unsigned int nValue,
                                void *pfnDraw, int bEmpty);
extern int Ov002_RunShutdownHook(void);

void Ov002_SetGaugeSlotShown(int nIndex, unsigned int bOn) {
    char *ctx = data_ov002_0207f618;
    Ov002FadeSlot *slot = (Ov002FadeSlot *)(ctx + 0x6c + nIndex * 0x18);
    unsigned int nValue;
    void *pfnDraw;
    int nPlaying;

    if (nIndex == 0) {
        nValue = *(u16 *)(ctx + 0x112);
        pfnDraw = (void *)Ov002_DrawGaugeTweenCell;
        if (nValue == 0 && *(u16 *)(ctx + 0xce) != 0) {
            nValue = 1;
        }
    } else {
        unsigned int nAmount = *(u16 *)(ctx + nIndex * 4 + 0xce);

        pfnDraw = (void *)Ov002_DrawShortLayoutRow;
        nValue = (unsigned int)kh_rt_s32_divmod(
                     nAmount * *(int *)((char *)&data_ov002_0207de04
                                        + nIndex * 0xc),
                     *(u16 *)(ctx + nIndex * 4 + 0xcc)) & 0xffff;
        if (nValue == 0 && nAmount != 0) {
            nValue = 1;
        }
    }

    if (slot->bActive != bOn) {
        if (bOn != 0) {
            slot->bActive = 1;
            slot->nField0014 = 0;
            slot->nDuration = 261828;
            slot->nPhase = 0;
            slot->qwStart = OS_GetTick();
            Ov002_AdvanceGaugeSlot(nIndex, slot);
        } else {
            int bEmpty = 0;

            slot->bActive = 0;
            if (*(u16 *)(ctx + nIndex * 4 + 0xce) == 0) {
                bEmpty = 1;
            }
            Ov002_RunPresetCallbacks(((int *)ctx)[nIndex], nIndex, nValue,
                                pfnDraw, bEmpty);
        }
    }

    if (nIndex != 0) {
        return;
    }

    nPlaying = *(int *)(ctx + 0x30);

    if (bOn != 0) {
        if (nPlaying != 0) {
            return;
        }
        if (Ov002_RunShutdownHook() != 0) {
            return;
        }
        PlaySoundChecked(0, 8);
        *(int *)(ctx + 0x30) = 1;
    } else {
        if (nPlaying == 0) {
            return;
        }
        ForwardToHandlerOrCurrentObject(0, 8, 0);
        *(int *)(ctx + 0x30) = 0;
    }
}

/* PS2: mechanically prepared copy of src/overlays/players/ov090_player_xaldin_4/Ov090_HandleMessage.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Message handler of the ov034 enemy (x4: ov034/052/072/090). 0x21 flags the local player's
 * +0x464 bit 16, tells the slot callback 0x2f and hands over to the idle step; 0x22 refreshes
 * the target, tells 0x31 (and flags bit 31) when the effect context is busy or 0x30 otherwise,
 * picking the matching step, and turns the node to the cached heading +0x2abc plus the +0x478
 * offset unless the facing is locked; 0x23 tells 0x33, rewinds the animation to 0xb000, raises
 * bit 49, shows the node and caches the height in the rig; 0x24 clears the rig's two counters
 * and sets the fall speed; 0x25 tells 0x32. Each accepted message returns its step. */

#include "nitro/types.h"
#include "game/engine.h"

extern void Ov022_FillEightHalvesMinus1At0x2bd4(char *self);
extern int Ov022_IsSlotReady(char *context);
extern void Anim_SetFrameWrapped(void *animation, int track, int frame);              /* Anim_SetFrameWrapped */
extern char *data_ov090_020bcc00;
extern void Ov090_IdleStep(void);
extern void Ov090_StepAndCheckTimer(void);
extern void Ov090_AttackPhaseStep(void);
extern void Ov090_ApproachStep(void);
extern void Ov090_DiveStep(void);
extern void Ov090_LandingStep(void);

void *Ov090_HandleMessage(char *self, int msg)
{
    char *rig = data_ov090_020bcc00 + 0xe4 + 0x2c00;
    void *next = 0;
    u16 a;
    u32 *node;

    switch (msg - 0x21) {
    case 0:
        if (Session_GetLocalPlayerIndex() == 0) {
            *(kh_unaligned_u64 *)(self + 0x464) |= 0x10000;
        }
        next = (void *)&Ov090_IdleStep;
        (*(void (**)(char *, int))(self + 0x664))(self, 0x2f);
        break;
    case 1:
        Ov022_FillEightHalvesMinus1At0x2bd4(self);
        if (Ov022_IsSlotReady(self + 0x2f8 + 0x2000) != 0) {
            next = (void *)&Ov090_StepAndCheckTimer;
            (*(void (**)(char *, int))(self + 0x664))(self, 0x31);
            if (Session_GetLocalPlayerIndex() == 0) {
                *(kh_unaligned_u64 *)(self + 0x464) |= 0x80000000;
            }
        } else {
            next = (void *)&Ov090_AttackPhaseStep;
            (*(void (**)(char *, int))(self + 0x664))(self, 0x30);
        }
        if (*(int *)(self + 0x2abc) != -1) {
            a = (u16)(*(int *)(self + 0x2abc) + *(short *)(self + 0x478));
            node = *(u32 **)(self + 0x20);
            if ((*node & 0x20) == 0) {
                *(u16 *)((char *)node + 0x80) = a + 0x8000;
                *(u16 *)((char *)node + 4) |= 0x20;
            }
        }
        break;
    case 2:
        next = (void *)&Ov090_ApproachStep;
        (*(void (**)(char *, int))(self + 0x664))(self, 0x33);
        Anim_SetFrameWrapped(*(char **)(self + 0x20) + 4, 0, 0xb000);
        *(int *)(self + 0x7b0) = 0xb000;
        *(kh_unaligned_u64 *)self |= 0x2000000000000ULL;
        node = *(u32 **)(self + 0x20);
        if ((*node & 0x20) == 0) {
            SceneNode_Enable(node + 1);
        }
        *(int *)(rig + 0x1c) = *(int *)(self + 0x490);
        break;
    case 3:
        next = (void *)&Ov090_DiveStep;
        *(int *)(rig + 4) = 0;
        *(int *)(rig + 8) = 0;
        *(int *)(self + 0x58) = -0xe66;
        break;
    case 4:
        next = (void *)&Ov090_LandingStep;
        (*(void (**)(char *, int))(self + 0x664))(self, 0x32);
        break;
    }
    return next;
}

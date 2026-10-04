/* PS2: mechanically prepared copy of src/overlays/enemies/ov253_enemy_leechgrave/Ov253_MsgHookSlots.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov253_MsgHookSlots -- message hook for kind 5 messages, by slot: 0 / 3 spawn the +0x3b0
 * block's first / fourth effect child (020c08cc, kind 5, scale 1.0, payload at byte 5); 1
 * spawns the handle child (020d1114); 2 / 7 spawn the third / fifth effect at the +0x3ac item's
 * +4 (020c09a0, kinds 0xd / 5); 4 spawns the item child (020d182c); 5 looks the +4 handle up
 * (020c9b68) and, when its +0x18c rider is not 0x10000-flagged and belongs to the current
 * player (02030788), links it as +0x3c0 (020d1b2c); 6 releases +0x3c0. Then the base hook. */

#include "game/enemy_common.h"

extern int Ov107_CreateNodeXformTaskFx24(int list, int parent, int kind, int a, int scale, unsigned char *payload);
extern int Ov107_AiState_OnMessage(int self, unsigned char *msg, int extra);
extern int Ov253_SpawnHandleChild(int self);
extern int Ov253_SpawnItemChild(int self);
extern int Session_GetLocalPlayerIndex(void);
extern int Ov253_CreateAimTask(int self, int target);
extern void TaskList_FinishByTag(int scene, int object);

struct Ov253Pair { int pEffect; int pChild; };

int Ov253_MsgHookSlots(int self, unsigned char *msg, int extra) {
    int target;
    int rider;

    if (msg[2] == 5) {
        switch (msg[3]) {
        case 1:
            (*(struct Ov253Pair **)(self + 0x3b0))[1].pChild = Ov253_SpawnHandleChild(self);
            break;
        case 2:
            (*(struct Ov253Pair **)(self + 0x3b0))[2].pChild =
                Ov107_CreateNodeBodyTask(*(int *)(self + 0x3c), (*(struct Ov253Pair **)(self + 0x3b0))[2].pEffect, 0xd, (void *)(*(int *)(self + 0x3ac) + 4), 0, 1);
            break;
        case 0:
            (*(struct Ov253Pair **)(self + 0x3b0))[0].pChild =
                Ov107_CreateNodeXformTaskFx24(*(int *)(self + 0x3c), (*(struct Ov253Pair **)(self + 0x3b0))[0].pEffect, 5, 0, 0x1000, msg + 5);
            break;
        case 3:
            (*(struct Ov253Pair **)(self + 0x3b0))[3].pChild =
                Ov107_CreateNodeXformTaskFx24(*(int *)(self + 0x3c), (*(struct Ov253Pair **)(self + 0x3b0))[3].pEffect, 5, 0, 0x1000, msg + 5);
            break;
        case 7:
            (*(struct Ov253Pair **)(self + 0x3b0))[4].pChild =
                Ov107_CreateNodeBodyTask(*(int *)(self + 0x3c), (*(struct Ov253Pair **)(self + 0x3b0))[4].pEffect, 5, (void *)(*(int *)(self + 0x3ac) + 4), 0, 0);
            break;
        case 4:
            Ov253_SpawnItemChild(self);
            break;
        case 5:
            target = Ov107_FindMessageHandler(*(unsigned short *)(msg + 4));
            if (target != 0) {
                rider = *(int *)(target + 0x18c);
                if ((*(kh_unaligned_u64 *)rider & 0x10000ULL) == 0) {
                    if (*(unsigned char *)(rider + 8) == Session_GetLocalPlayerIndex()) {
                        *(int *)(self + 0x3c0) = Ov253_CreateAimTask(self, target);
                    }
                }
            }
            break;
        case 6:
            TaskList_FinishByTag(*(int *)(self + 0x3c), *(int *)(self + 0x3c0));
            *(int *)(self + 0x3c0) = 0;
            break;
        }
    }
    return Ov107_AiState_OnMessage(self, msg, extra);
}

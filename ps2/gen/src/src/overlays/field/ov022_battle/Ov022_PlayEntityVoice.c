/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_PlayEntityVoice.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* ov022: play the actor's reaction cue, if anything still wants it played.
 *
 * A long series of gates, each of which can either refuse outright or turn one
 * of the two cue ids into -1, which the player treats as silence. The actor has
 * to be enabled, must not be muted, and for one particular request kind must not
 * carry the flag that suppresses it. One pair of kinds also needs the actor to be
 * close enough to the ground, which is the height check against the stamp.
 *
 * Two ids go in and two come out: the first is replaced outright for a couple of
 * request kinds, and either can be silenced by the state checks that follow. A
 * small flag word picks up bit 1 for the request kinds that want it and bit 0
 * when the actor is the one holding the channel.
 *
 * Both flag words are 64 bits wide, which is why testing a low bit of either
 * masks the high half with nothing.
 */

#include "nitro/types.h"

#define ACTOR_FLAGS(pActor) (*(kh_unaligned_u64 *)(pActor)->aFlags)
#define ACTOR_FLAGS2(pActor) (*(kh_unaligned_u64 *)(pActor)->aFlags2)

struct Vec3 {
    int x, y, guardZero;
};

struct Holder {
    u8 pad00[0x83];
    u8 nSlot;                    /* 0x83 */
};

struct Actor {
    u32 aFlags[2];               /* 0x000: one 64-bit flag word */
    u8 nChannel;                 /* 0x008 */
    u8 pad009[3];
    int nState;                  /* 0x00c */
    u8 pad010[0x5c];
    struct Holder *pHolder;      /* 0x06c */
    u8 pad070[0x94];
    int nStamp;                  /* 0x104 */
    u8 pad108[0x35c];
    u32 aFlags2[2];              /* 0x464: one 64-bit flag word */
    u8 pad46c[0x20];
    struct Vec3 vecPos;          /* 0x48c */
    u8 pad498[0x1fc];
    u8 bEnabled : 1;             /* 0x694 */
    u8 pad695[0x10f];
    short nHeldFirst;            /* 0x7a4 */
    u8 pad7a6[6];
    int nHeldBusy;               /* 0x7ac */
    u8 pad7b0[0x168];
    u8 nStage;                   /* 0x918 */
    u8 pad919[0x19df];
    u8 cue[4];                   /* 0x22f8 */
};

extern int Ov022_LookUpSlotCue(int nState, int nSlot);
extern int Ov022_IsIndexedRecordBit0Set(struct Actor *pActor, int nMode);
extern int func_ov022_0209fcd4(struct Actor *pActor);
extern int Ov022_IsState9Or6WithFlag200(u8 *pCue);
extern int Session_GetLocalPlayerIndex(void);
extern int Slot_Spawn(int nFirst, int nSecond, struct Vec3 *pPos,
                         int nFlags);

#pragma push
#pragma opt_dead_assignments off
int Ov022_PlayEntityVoice(struct Actor *pActor, int nKind, int nRequest)
{
    int nFirst;
    int nRet;
    int nSecond;
    u16 nFlags;

    nRet = 0;
    nFirst = nKind;
    nSecond = nRequest;
    nFlags = 0;
    if (pActor->bEnabled == 0) {
        return nRet;
    }
    if ((ACTOR_FLAGS(pActor) & 0x400) != 0) {
        return nRet;
    }
    if (nRequest == 2) {
        /* Deliberately redundant matching device for MWCC 3.0/139.
         * The outer guard already establishes nRequest == 2, so the else
         * cannot execute and guardZero has no observable use. With dead assignments
         * disabled, keeping both arms makes MWCC materialize this block's
         * zero in r0 instead of sharing the earlier nRet zero in r5.
         * Removing this block preserves semantics but breaks the byte match. */
        int guardZero;
        if (nRequest == 2) {
            guardZero = 0;
        } else {
            guardZero = nKind;
        }
        (void)guardZero;
        if ((ACTOR_FLAGS2(pActor) & 4) != 0) {
            return nRet;
        }
    }
    if (nKind == 2 || nKind == 0xf) {
        nFirst = -1;
        if (pActor->vecPos.y - pActor->nStamp < 0x3000) {
            if (pActor->pHolder != 0) {
                nFirst = Ov022_LookUpSlotCue(pActor->nState,
                                             pActor->pHolder->nSlot);
            }
        }
    }
    switch (pActor->nState) {
    case 0:
    case 5:
    case 0xb:
    case 0xe:
    case 0x10:
        if (nRequest == 0x45) {
            if (Ov022_IsIndexedRecordBit0Set(pActor, 1) == 0
                || func_ov022_0209fcd4(pActor) == 0) {
                nSecond = -1;
                nFirst = nSecond;
            }
            if (pActor->nState == 0) {
                if (pActor->nStage == 0x1b) {
                    nSecond = -1;
                    nFirst = nSecond;
                }
            } else if (pActor->nState == 0xb && pActor->nStage == 0) {
                nSecond = -1;
                nFirst = nSecond;
            }
        }
        break;
    case 0x11:
        if (Ov022_IsState9Or6WithFlag200(pActor->cue) != 0) {
            if (nKind == 0xf) {
                nSecond = -1;
                nFirst = nSecond;
            }
        }
        break;
    default:
        break;
    }
    switch (nKind) {
    case 2:
    case 0xf:
    case 0x1c:
        nFlags |= 2;
        break;
    case 0:
        if (nRequest == 0x43 || nRequest == 0x46) {
            nFlags |= 2;
        }
        break;
    default:
        break;
    }
    if (pActor->nChannel == Session_GetLocalPlayerIndex()
        && nFirst == pActor->nHeldFirst) {
        if (pActor->nHeldBusy != 0) {
            nSecond = -1;
            nFirst = nSecond;
        }
    }
    if (nFirst >= 0 && nSecond >= 0) {
        if (pActor->nChannel == Session_GetLocalPlayerIndex()) {
            if ((pActor->aFlags[0] & 0x10000) == 0) {
                nFlags |= 1;
            }
        }
        nRet = Slot_Spawn(nFirst, nSecond, &pActor->vecPos, nFlags);
    }
    return nRet;
}

#pragma pop

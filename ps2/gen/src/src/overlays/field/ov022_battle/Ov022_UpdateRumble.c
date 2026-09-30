/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_UpdateRumble.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov022: drive the rumble pak for one actor.
 *
 * Runs from the per-frame tick's common tail. A burst counter throttles how
 * often a fresh jolt may start, and a six-state machine converts the requested
 * strength into a 64-bit deadline tick: starting a rumble sets the deadline
 * ahead by one unit per strength step, and raising the strength while one is
 * already running extends it rather than restarting it. The deadline is capped
 * so a runaway request cannot pin the motor on.
 *
 * States 4 and 2 fall through into 1 and 3, and 5 falls through into 0, which
 * is why those bodies are laid out in that order rather than by case number.
 *
 * Whatever the state, the result is mirrored into the per-player status table
 * the rest of the game reads, converting the remaining deadline back into a
 * strength.
 */

#include "nitro/types.h"

#define RUMBLE_TICKS_PER_STEP  0x01ff6210u
#define RUMBLE_MAX_AHEAD       0x004fe752
#define RUMBLE_REPORT_DIV      0x82ea
#define RUMBLE_ROUND           0x7fd87

struct Rumble {
    unsigned long long nDeadline; /* 0x00 */
    int nCooldown;                /* 0x08 */
    u8 nLastLevel;                /* 0x0c */
    u8 nBurst;                    /* 0x0d */
    u8 nApplied;                  /* 0x0e */
    u8 nLevel;                    /* 0x0f */
    u8 nState;                    /* 0x10 */
    u8 nRequest;                  /* 0x11 */
};

struct RumbleStatus {
    u8 pad00[0x30];
    signed char nStrength;        /* 0x30 */
    signed char nState;           /* 0x31 */
    signed char nLevel;           /* 0x32 */
    u8 pad33[0x11];
};

struct Actor {
    unsigned long long nFlags;    /* 0x0000 */
    u8 nOwner;                    /* 0x0008 */
    u8 nId;                       /* 0x0009 */
    u8 pad00a[0x2a9a];
    struct Rumble rumble;         /* 0x2aa4 */
};

extern struct RumbleStatus data_0204c3d8[];

extern unsigned long long OS_GetTick(void);
extern int Session_GetLocalPlayerIndex(void);
extern int Ov022_GetGlobal34(void);
extern void Ov002_HudWidgets_IncCounter(void);
extern void Ov002_HudWidgets_SetField50(int nOn);
extern void Ov002_StopAllEmitters(void);
extern int Ov002_HudWidgets_IsFieldE4One(void);
extern void Ov002_ArmShake__ll(unsigned long long nTicks);

void Ov022_UpdateRumble(struct Actor *pActor)
{
    struct Rumble *pRum;
    struct RumbleStatus *pStatus;
    int bStarted;
    unsigned long long nNow;
    unsigned long long nCap;
    int nExtra;
    int nLevel;

    pRum = &pActor->rumble;
    bStarted = 0;
    nNow = OS_GetTick();
    if (pActor->nOwner == Session_GetLocalPlayerIndex()
        && (pActor->nFlags & (1ULL << 16)) == 0) {
        if (pRum->nLastLevel != pRum->nLevel) {
            if (pRum->nLevel > 1) {
                pRum->nBurst += pRum->nLevel - 1;
            }
            pRum->nLastLevel = pRum->nLevel;
        }
        if (pRum->nBurst != 0) {
            pRum->nCooldown -= Ov022_GetGlobal34();
            if (pRum->nCooldown <= 0) {
                Ov002_HudWidgets_IncCounter();
                pRum->nBurst = 0;
                pRum->nCooldown = 0x4000;
            }
        }
    }

    switch (pRum->nState) {
    case 5:
        Ov002_HudWidgets_SetField50(0);
        Ov002_StopAllEmitters();
        pRum->nBurst = 0;
        pRum->nRequest = 0;
        if (pActor->nOwner == Session_GetLocalPlayerIndex()
            && (pActor->nFlags & (1ULL << 16)) == 0) {
            pRum->nApplied = 0;
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            pRum->nState = 0;
        }
        /* fall through */
    case 0:
        pRum->nDeadline = 0;
        break;
    case 4:
        if (pActor->nOwner == Session_GetLocalPlayerIndex()
            && (pActor->nFlags & (1ULL << 16)) == 0) {
            Ov002_HudWidgets_SetField50(0);
            pRum->nApplied = 0;
        }
        /* fall through */
    case 1:
        if (pActor->nOwner == Session_GetLocalPlayerIndex()
            && (pActor->nFlags & (1ULL << 16)) == 0
            && Ov002_HudWidgets_IsFieldE4One() != 0) {
            Ov002_HudWidgets_SetField50(0);
            pRum->nApplied = 0;
        }
        if (pRum->nRequest != pRum->nApplied) {
            nExtra = 0;
            nCap = nNow + RUMBLE_MAX_AHEAD;
            if (pRum->nRequest > pRum->nApplied) {
                nExtra = pRum->nRequest - pRum->nApplied;
            }
            if (pRum->nRequest != 0) {
                bStarted = 1;
                if (pRum->nDeadline == 0 || pRum->nDeadline < nNow) {
                    pRum->nDeadline = nNow
                        + ((unsigned long long)pRum->nRequest
                           * RUMBLE_TICKS_PER_STEP >> 6);
                } else {
                    pRum->nDeadline +=
                        (unsigned long long)((long long)nExtra
                                             * (long long)RUMBLE_TICKS_PER_STEP) >> 6;
                }
                if (pRum->nDeadline > nCap) {
                    pRum->nDeadline = nCap;
                }
                if (pActor->nOwner == Session_GetLocalPlayerIndex()
                    && (pActor->nFlags & (1ULL << 16)) == 0) {
                    Ov002_ArmShake__ll(((pRum->nDeadline - nNow) << 6)
                                        / RUMBLE_REPORT_DIV);
                }
            }
            pRum->nApplied = pRum->nRequest;
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            pRum->nState = 1;
            if (bStarted == 0 && nNow < pRum->nDeadline) {
                nLevel = (int)(((pRum->nDeadline - nNow) << 6)
                               / RUMBLE_TICKS_PER_STEP);
                if (nLevel < pRum->nRequest) {
                    pRum->nRequest = (u8)nLevel;
                }
            }
            if (pRum->nDeadline == 0) {
                pRum->nState = 5;
            } else if (nNow > pRum->nDeadline) {
                pRum->nLevel = 0;
                pRum->nRequest = 0;
                pRum->nState = 0;
            }
        }
        break;
    case 2:
        if (pActor->nOwner == Session_GetLocalPlayerIndex()
            && (pActor->nFlags & (1ULL << 16)) == 0) {
            Ov002_HudWidgets_SetField50(1);
        }
        /* fall through */
    case 3:
        if (pActor->nOwner == Session_GetLocalPlayerIndex()
            && (pActor->nFlags & (1ULL << 16)) == 0
            && Ov002_HudWidgets_IsFieldE4One() == 0) {
            Ov002_HudWidgets_SetField50(1);
        }
        if (pRum->nRequest != pRum->nApplied) {
            pRum->nDeadline = nNow
                + ((unsigned long long)pRum->nRequest
                   * RUMBLE_TICKS_PER_STEP >> 6);
            pRum->nApplied = pRum->nRequest;
        }
        if (Session_GetLocalPlayerIndex() == 0) {
            pRum->nState = 3;
            if (pRum->nDeadline == 0) {
                pRum->nState = 5;
            } else if (nNow > pRum->nDeadline) {
                pRum->nRequest = 0;
                pRum->nLevel = 0;
                pRum->nState = 4;
            }
        }
        break;
    default:
        break;
    }

    pStatus = &data_0204c3d8[pActor->nId];
    pStatus->nState = pRum->nState;
    pStatus->nLevel = pRum->nLevel;
    if (!(pStatus->nState != 1 && pStatus->nState != 3
          && pStatus->nState != 4)) {
        if (nNow >= pRum->nDeadline) {
            pStatus->nStrength = 0;
        } else {
            pStatus->nStrength = (signed char)
                (((pRum->nDeadline - nNow + RUMBLE_ROUND) << 6)
                 / RUMBLE_TICKS_PER_STEP);
        }
    } else {
        pStatus->nStrength = 0;
    }
}

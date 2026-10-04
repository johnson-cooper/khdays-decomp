/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_FillMissionResult.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"

/* One entry of the tally at the tail of the result. */

#include "nitro/types.h"
#include "game/engine.h"

typedef struct {
    u16 wId;                /* 0x00 */
    s16 nCount;             /* 0x02 */
} Ov002ResultTally;

/* The mission result the session leaves behind for the results screen in
 * ov005.  Every access below goes through the record's address rather than
 * through its fields, because that is what keeps the single literal-pool
 * entry the ROM carries: the offsets are derived from it, never folded into
 * further address constants.
 */
typedef struct {
    u16 wPanelTotal;        /* 0x00 */
    s8  bRequestId;         /* 0x02 */
    u8  pad03;
    s16 nRequestParam;      /* 0x04 */
    u16 pad06;
    int nValue;             /* 0x08 */
    int nDuration;          /* 0x0c */
    int nScaled;            /* 0x10 */
    int nRaw;               /* 0x14 */
    u64 nStampTicks;        /* 0x18 */
    u8  bFlags;             /* 0x20 */
    u8  pad21;
    u8  nMemberMask;        /* 0x22 */
    u8  pad23;
    s16 aMemberValues[4];   /* 0x24 */
    Ov002ResultTally aTally[0x12];  /* 0x2c */
} Ov002MissionResult;

/* One seat's row in the roster block; four of them, 0x2c apart. */
typedef struct {
    void *pInstance;        /* 0x00 */
    void *pSlots;           /* 0x04 */
    int aPad08[2];
    int nPanelTotal;        /* 0x10 */
    int pad14;
    int nRaw;               /* 0x18 */
    int nScaleIn;           /* 0x1c */
    int aTail[3];           /* 0x20 */
} Ov002SeatRow;             /* 0x2c */

extern u8 data_0204c240;                /* g_modeAndDayClock; bit 1 marks a linked run */
extern char data_0204c254[];            /* g_ov002PanelThresholds */
extern Ov002MissionResult data_0204c32c;

extern char *NNSi_FndGetCurrentRootHeap(void);  /* the heap the overlay context lives in */
extern void Ov002_FetchPanelMetric(char *pOut);    /* Ov002_FetchPanelMetric */
extern int Ov002_ScaleValueByModifiers(int nSeat, int nValue);   /* scale by the modifiers */
extern u64 Ov002_GetTimeoutTicks(void);           /* the configured timeout, in ticks */
extern u16 *Slot_GetEntryIfCounted(int nSeat, int nIndex);   /* one slot entry, or null */

/* Fills the mission result the results screen reads back in ov005.
 *
 * The head of the record depends on the mode.  Outside a linked run -- bit 1
 * of g_modeAndDayClock clear -- the request id, its value, its parameter and
 * its duration are copied straight off the pending request at root +0x8c94.
 * Inside one the duration is fetched instead, and the value is only taken
 * from the panel, and only while the panel's metric is two.
 *
 * The tail is the same either way: the local seat's value scaled by the
 * modifiers, the same value raw, the configured timeout in ticks, and a flag
 * byte.  That byte gains its low bit while game flag 0x2087 is set, and
 * setting it also walks the nine-bit field 0 back from 358 to 357 if it had
 * already reached its cap.
 *
 * Then the local seat's eighteen slots are tallied into the record: an id not
 * seen yet is appended with a count of one, and an id already there has its
 * count raised.  The slot rows are cleared afterwards, so the tally is what
 * survives.
 *
 * Called from Ov002_StepSessionMenu.
 */
void Ov002_FillMissionResult(void)
{
    int j;
    u16 *pEntry;
    Ov002SeatRow *pSeats;
    char *pReq;
    int nCount;
    char *pRep;
    char *pScan;
    char *pSlot;
    u16 wId;
    int nSeat;
    int nIndex;
    char *pRoot;
    int bNew;

    pRep = (char *)&data_0204c32c;
    pRoot = NNSi_FndGetCurrentRootHeap();
    pSeats = (Ov002SeatRow *)(pRoot + 0x8bcc);
    pReq = pRoot + 0x8bcc + 0xc8;
    nSeat = Session_GetLocalPlayerIndex();

    *(u16 *)(pRoot + 0x8da2) = 0;
    *(u16 *)(pRep + 0) = (u16)pSeats->nPanelTotal;

    if ((data_0204c240 & 2) != 0) {
        if ((data_0204c240 & 2) != 0 && *(u16 *)&data_0204c254[0xe] == 2) {
            *(int *)(pRep + 8) = *(int *)&data_0204c254[0x10];
        }
        Ov002_FetchPanelMetric(pRep + 0xc);
    } else {
        *(s8 *)(pRep + 2) = *(s8 *)(pReq + 0x1a);
        *(int *)(pRep + 8) = *(int *)(pReq + 0xc);
        *(s16 *)(pRep + 4) = *(s16 *)(pReq + 0x10);
        *(int *)(pRep + 0xc) = *(int *)(pReq + 0x14);
    }

    *(int *)(pRep + 0x10) =
        Ov002_ScaleValueByModifiers(nSeat, pSeats[nSeat].nScaleIn);
    *(int *)(pRep + 0x14) = pSeats[nSeat].nRaw;
    *(kh_unaligned_u64 *)(pRep + 0x18) = Ov002_GetTimeoutTicks();
    *(u8 *)(pRep + 0x20) = 0;

    if (GameState_IsFlagSet(0x2087) != 0) {
        *(u8 *)(pRep + 0x20) = (u8)(*(u8 *)(pRep + 0x20) | 1);
        if (GameState_GetField(0, 9) == 0x166) {
            GameState_SetField(0, 9, 0x166 - 1);
        }
    }

    PartyState_ClearHeldItem();
    nSeat = Session_GetLocalPlayerIndex();
    nCount = 0;
    pSlot = pRep;
    for (nIndex = 0; nIndex < 0x12; nIndex++) {
        pEntry = Slot_GetEntryIfCounted(nSeat, nIndex);
        if (pEntry != 0) {
            bNew = 1;
            j = 0;
            if (nCount > 0) {
                wId = *pEntry;
                pScan = pRep;
                do {
                    if (wId == *(u16 *)(pScan + 0x2c)) {
                        (*(s16 *)(pRep + j * 4 + 0x2e))++;
                        bNew = 0;
                        break;
                    }
                    j++;
                    pScan += 4;
                } while (j < nCount);
            }
            if (bNew != 0) {
                nCount++;
                *(u16 *)(pSlot + 0x2c) = *pEntry;
                *(s16 *)(pSlot + 0x2e) = 1;
                pSlot += 4;
            }
        }
    }
    PartyMember_ClearEntryPairs(0);
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_ElementGatherActors.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"

#include "nitro/types.h"

extern const short data_ov002_0207e6fc[];

extern int Ov002_GetRootField8bc4(void);
extern int Session_GetLocalPlayerIndex(void);
extern int func_ov022_020882f8(void);
extern void *GetEntryField20ByIndex(int nIndex);
extern int Ov002_ElementActorInRange(char *pElement, void *pEntry);
extern int Ov002_GetRootField8d94(void);
extern void Ov002_AddMissionTally(int nIndex, int nKind, int nValue);
extern int Ov002_RecordElementHit(void *pElement, void *pMsg, int nKind);
extern int Ov002_RunShutdownHook(void);
extern void Ov002_BeginSessionTeardown(int nMode);
extern void Ov002_DoneTick(void);
extern int Ov002_IsSessionOpen(void);
extern int QueryActiveStateOrDelegate(void);
extern int Ov022_GetEntryField66(int nIndex);
extern int Ov002_GetSlotTableByte(int nHandle);

static inline int FX_Mul(int a, int b)
{
    return (int)(((long long)a * b + 0x800) >> 12);
}

/* Gather the actors an element wants and tell them to come over.
 *
 * While the element is waiting it looks at every entry that has not been
 * claimed yet: an actor in range is claimed, and unless the owner already has
 * it, it is sent walking to a distance that grows with how many have been
 * claimed so far. Only when every entry it looked at was in range does the
 * element start. On the second step it waits for the transition and hands over
 * to the idle handler. Whatever happened, each claimed actor that has not been
 * told yet is sent its message.
 */
void *Ov002_ElementGatherActors(char *pElement)
{
    unsigned char aStart[6];
    unsigned char aMsg[6];
    char *pOwner;
    void *pEntry;
    int i;
    int bAllInRange;
    int nClaimed;
    int j;
    int nDist;
    int nSlot;

    pOwner = *(char **)(pElement + 8);

    switch (*(u8 *)(pElement + 0x1b6)) {
    case 0:
        if (Ov002_GetRootField8bc4() != 0 && Session_GetLocalPlayerIndex() == 0) {
            bAllInRange = 1;

            for (i = 0; i < func_ov022_020882f8(); i++) {
                pEntry = GetEntryField20ByIndex(i);
                if ((*(kh_unaligned_u64 *)pEntry & 0x800) != 0) {
                    continue;
                }

                if (Ov002_ElementActorInRange(pElement, pEntry) != 0) {
                    *(kh_unaligned_u64 *)pEntry |= 0x800;

                    if ((*(u8 *)(pOwner + 0x88) & (1 << i)) == 0) {
                        nClaimed = 0;
                        for (j = 0; j < func_ov022_020882f8(); j++) {
                            if ((*(u8 *)(pOwner + 0x88) & (1 << j)) != 0) {
                                nClaimed++;
                            }
                        }

                        nDist = (int)(((long long)Ov002_GetRootField8d94()
                                       * 0x14000 + 0x800) >> 12);
                        Ov002_AddMissionTally(
                            i, 7,
                            (FX_Mul(data_ov002_0207e6fc[nClaimed], nDist)
                             + 0xfff) >> 12);

                        *(u8 *)(pOwner + 0x88) |= 1 << i;
                    }
                } else {
                    bAllInRange = 0;
                }
            }

            if (bAllInRange) {
                aStart[0] = 1;
                if (Ov002_RecordElementHit(pElement, aStart, 4) != 0) {
                    *(u8 *)(pElement + 0x1b6) = 1;
                }
            }
        }
        break;

    case 2:
        if (Ov002_RunShutdownHook() == 0) {
            if (Session_GetLocalPlayerIndex() == 0) {
                Ov002_BeginSessionTeardown(0);
            }
            *(u8 *)(pElement + 0x1b6) = 3;
            return Ov002_DoneTick;
        }
        break;
    }

    for (i = 0; i < func_ov022_020882f8(); i++) {
        if ((*(u8 *)(pOwner + 0x88) & (0x10 << i)) == 0
            && (*(u8 *)(pOwner + 0x88) & (1 << i)) != 0) {
            aMsg[0] = 2;
            if (Ov002_IsSessionOpen() != 0) {
                nSlot = Ov002_GetSlotTableByte(
                    Ov022_GetEntryField66(QueryActiveStateOrDelegate()));
            } else {
                nSlot = -1;
            }
            *(short *)(aMsg + 4) = (short)nSlot;
            if (Ov002_RecordElementHit(pElement, aMsg, 6) != 0) {
                *(u8 *)(pOwner + 0x88) |= 0x10 << i;
            }
        }
    }

    return 0;
}

/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/Ov002_SpareEntryStep.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"

#include "nitro/types.h"

extern int Ov002_IsSessionOpen(void);
extern int Ov002_IsPlayerInTriggerRadius(char *pEntry);
extern unsigned short QueryActiveStateOrDelegate(void);
extern void *GetEntryField20ByIndex(unsigned int nIndex);
extern void func_ov022_020ad2e4(void *pEntry, int nMode);
extern void GameState_SetField(int nField, int nKind, int nValue);
extern unsigned int GameState_GetField(int nField, int nKind);
extern int Session_GetLocalPlayerIndex(void);
extern int Session_IsActive(void);
extern int Ov002_SetLeaveRequest(int bOn);
extern void Ov002_StreamFormattedLine(char *pName, void *pText);
extern void Ov002_SetRootFields8b44And8b48(void *pfnDone, char *pEntry);
extern void Ov002_SetRootField8b41(int nBits);
extern int Ov002_GetRootField8b41(void);
extern void Ov002_SetRootField8b40(void);
extern int Ov002_GetPhaseWord(void);
extern int func_ov022_02083f0c(void);
extern int func_ov022_02083f5c(void);
extern void func_ov022_02086818(int nHandle, int nMode);
extern void Ov002_SetOrClearFlag200(int nHandle, int nMode);
extern void Ov002_Camera_SetMode(int nHandle, int nMode, void *pExtra);
extern void *func_ov022_020881f8(int nIndex);
extern int func_ov022_020882f8(void);
extern int VEC_Distance(void *pA, void *pB);
extern void Ov022_RequestGuardBreak(void *pEntry);
extern void Ov002_AnnounceState(char *pEntry);
extern int Ov002_RecordElementHit(void *pEntry, void *pReq, int nKind);
extern void func_ov022_020888ec(int nIndex, int bOn);
extern void Ov002_List_SetSlot(int arg0, int arg1);
extern void Ov002_SetFieldBit0(char *pEntry, int nMode);

/* Run one step of a spare entry's state machine.
 *
 * Whoever is standing in the trigger radius gets nudged first. Then the phase
 * decides: phase 2 is the hand-over, where the owner's game-state field is
 * published and, for the player who owns the entry, either the plain
 * announcement or the full line - which streams the text, hands over the done
 * callback, adjusts the session flags and, in the one phase word that asks for
 * it, pushes every distant entry away. Phases 4 and 5 queue a record and drop
 * back to phase 1, and phase 6 tears the entry down and writes its state back.
 */
int Ov002_SpareEntryStep(char *pEntry)
{
    char *pOwner;
    int nHandle;
    int nOther;
    int i;
    void *pSelf;
    void *pEntryI;
    unsigned int nState;

    pOwner = *(char **)(pEntry + 8);

    if (Ov002_IsSessionOpen() != 0 && Ov002_IsPlayerInTriggerRadius(pEntry) != 0) {
        func_ov022_020ad2e4(GetEntryField20ByIndex(QueryActiveStateOrDelegate()), 1);
    }

    switch (*(unsigned char *)(pEntry + 0x2c)) {
    case 2:
        GameState_SetField(0x20dd, 3, *(unsigned char *)(pEntry + 0x3f));

        if (*(unsigned char *)(pEntry + 0x3f) == QueryActiveStateOrDelegate()) {
            if (*(unsigned char *)(pOwner + 0x5a) != 0) {
                GameState_SetField(*(u16 *)(pOwner + 0x58),
                              *(unsigned char *)(pOwner + 0x5a),
                              *(unsigned char *)(pEntry + 0x2d));
            }

            if (*(signed char *)(pEntry + 0x37) != 0) {
                if (Session_GetLocalPlayerIndex() != 0) {
                    Ov002_SetLeaveRequest(1);
                }

                Ov002_StreamFormattedLine(*(signed char *)(pEntry + 0x2f) != 0
                                        ? pEntry + 0x2f : 0,
                                    pEntry + 0x37);
                Ov002_SetRootFields8b44And8b48(Ov002_AnnounceState, pEntry);

                if (Session_IsActive() != 0) {
                    Ov002_SetRootField8b41(0);
                } else {
                    Ov002_SetRootField8b41((unsigned char)(Ov002_GetRootField8b41() & ~0xa));
                }
                Ov002_SetRootField8b40();

                if (Ov002_GetPhaseWord() == 1) {
                    nHandle = func_ov022_02083f0c();
                    func_ov022_02086818(func_ov022_02083f5c(), 0);
                    Ov002_SetOrClearFlag200(nHandle, 1);

                    if ((*(signed char *)(pEntry + 0x40) & 0x20) != 0) {
                        Ov002_Camera_SetMode(nHandle, 1, 0);

                        if (Session_IsActive() == 0) {
                            pSelf = func_ov022_020881f8(QueryActiveStateOrDelegate());
                            for (i = 0; i < func_ov022_020882f8(); i++) {
                                pEntryI = GetEntryField20ByIndex(i);
                                if ((*(kh_unaligned_u64 *)pEntryI & 0x10000)
                                    != 0) {
                                    nOther = VEC_Distance(
                                        pSelf, func_ov022_020881f8(i));
                                    if (nOther >= 0x3000) {
                                        Ov022_RequestGuardBreak(pEntryI);
                                    }
                                }
                            }
                        }
                    }
                }

                *(unsigned char *)(pEntry + 0x2c) = 3;
            } else {
                Ov002_AnnounceState(pEntry);
                Ov002_SetLeaveRequest(0);
            }
        } else {
            *(unsigned char *)(pEntry + 0x17) = 0;
            Ov002_SetLeaveRequest(0);
        }
        break;

    case 4:
        if (Session_GetLocalPlayerIndex() == 0) {
            unsigned char aReq[6];

            aReq[0] = 3;
            aReq[4] = *(unsigned char *)(pEntry + 0x3f);
            if (Ov002_RecordElementHit(pEntry, aReq, 6) == 0) {
                return 0;
            }
        } else {
            unsigned char aReq[6];

            aReq[0] = 2;
            aReq[4] = *(unsigned char *)(pEntry + 0x3f);
            if (Ov002_RecordElementHit(pEntry, aReq, 6) == 0) {
                return 0;
            }
        }

        *(unsigned char *)(pEntry + 0x17) = *(signed char *)(pEntry + 0x2e);
        if (Ov002_IsSessionOpen() != 0) {
            func_ov022_020888ec(*(unsigned char *)(pEntry + 0x3f), 0);
        }
        *(unsigned char *)(pEntry + 0x2c) = 1;
        break;

    case 5:
        {
            unsigned char aReq[6];

            aReq[0] = 3;
            aReq[4] = *(unsigned char *)(pEntry + 0x3f);
            if (Ov002_RecordElementHit(pEntry, aReq, 6) != 0) {
                *(unsigned char *)(pEntry + 0x2c) = 1;
            }
        }
        break;

    case 6:
        *(unsigned char *)(pEntry + 0x17) = *(signed char *)(pEntry + 0x2e);
        Ov002_List_SetSlot(0, *(unsigned char *)(pEntry + 0x3f));
        *(unsigned char *)(pEntry + 0x2c) = 0;

        if ((*(signed char *)(pEntry + 0x40) & 0x10) == 0
            && (*(signed char *)(pEntry + 0x40) & 0xf) == 0) {
            *(u16 *)(pEntry + 0x12) &= ~8;
            Ov002_SetFieldBit0(pEntry, 0);
        }

        if (*(signed char *)(pEntry + 0x37) != 0) {
            func_ov022_020888ec(*(unsigned char *)(pEntry + 0x3f), 0);

            if (Ov002_GetPhaseWord() == 1) {
                nHandle = func_ov022_02083f0c();
                nOther = func_ov022_02083f5c();

                if ((*(signed char *)(pEntry + 0x40) & 0x20) != 0) {
                    Ov002_Camera_SetMode(nHandle, 0, 0);
                }
                Ov002_SetOrClearFlag200(nHandle, 0);
                func_ov022_02086818(nOther, 1);
            }
        }

        nState = GameState_GetField(*(u16 *)(pEntry + 0x14),
                               *(unsigned char *)(pEntry + 0x16));
        GameState_SetField(*(u16 *)(pEntry + 0x14),
                      *(unsigned char *)(pEntry + 0x16),
                      (u16)((nState & ~0xfffe) | 2));
        break;
    }

    return 0;
}

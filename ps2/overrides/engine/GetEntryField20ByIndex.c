/* PS2 override of GetEntryField20ByIndex (src/engine/GetEntryField20ByIndex.c): the actor of
 * player entry `idx` of the ov022 battle root (*(data_ov022_020b2e78 + 4)), or 0.
 *
 * The root holds exactly four 0xc-byte entries at +4 (entryCount follows at +0x34).  Some callers
 * pass an index that is not a player slot - Ov107_AiState_OnDefeat passes the kill-credit actor's
 * `kind` to Ov022_SetPlayerScale - and the DS then reads the count/flag words as an "entry", follows
 * them to a junk "actor" and reads a byte through it.  The DS bus tolerates that (the value only
 * nudges a mission tally); the EE takes a TLB miss.  Hardware: the second Heartless wave of the
 * first Marluxia mission crashed in Ov022_SetPlayerScale reading base->step at 0x03e02abb.
 *
 * Same lookup for valid indices; an out-of-range index or a result that cannot be an EE object
 * pointer is treated as "no actor" (0), which every caller already handles. */

#include "platform/kh_platform.h"

extern int data_ov022_020b2e78;

#define KH_PLAYER_ENTRIES 4

static int kh_plausible_object(unsigned int p)
{
    return p >= 0x00100000u && p < 0x02000000u && (p & 3u) == 0;
}

int GetEntryField20ByIndex(int idx)
{
    static int warned;
    int base = *(int *)((char *)&data_ov022_020b2e78 + 4);
    int entry;
    int actor;

    if (base == 0)
        return 0;
    if ((unsigned int)idx >= KH_PLAYER_ENTRIES) {
        if (warned < 8) {
            warned++;
            KH_WARN("ov022", "GetEntryField20ByIndex(%d): not a player slot, no actor (caller %p)",
                    idx, __builtin_return_address(0));
        }
        return 0;
    }
    entry = *(int *)(idx * 0xc + base + 4);
    if (entry == 0)
        return 0;
    if (!kh_plausible_object((unsigned int)entry))
        return 0;
    actor = *(int *)(entry + 0x20);
    if (actor != 0 && !kh_plausible_object((unsigned int)actor)) {
        if (warned < 8) {
            warned++;
            KH_WARN("ov022", "GetEntryField20ByIndex(%d): invalid actor %08x, no actor (caller %p)",
                    idx, (unsigned int)actor, __builtin_return_address(0));
        }
        return 0;
    }
    return actor;
}

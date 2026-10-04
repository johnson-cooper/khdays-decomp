/* PS2: mechanically prepared copy of src/overlays/players/ov104_player_roxas_dual_4/Ov104_UpdateParts.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* ov049 enemy (x4: ov049/068/087/104): drive one update pass over the group's parts. Walks the
 * parts (0x1c8 apart, count at +0x19) and dispatches each by the signed kind byte at +2 through
 * the handler table; kind 0 is skipped. A kind-2 part whose +4 word is zero arms the actor's
 * +0x47a/+0x47b pair (3, and 1 or 0 after bit 0x200 of the part's +0x138 target) when 02030788
 * reports idle and the actor's flag 0x10000 is clear. */

#include "nitro/types.h"
#include "game/engine.h"

extern void (*data_ov104_020bc1e8[])(char *pGroup, char *pPart, int nArg);

void Ov104_UpdateParts(char *pGroup, int nArg)
{
    char *pActor = *(char **)(pGroup + 8);
    char *pPart;
    int i = 0;
    int off;
    int nKind;

    if ((int)*(u8 *)(pGroup + 0x19) <= 0) {
        return;
    }
    off = 0;
do {
    pPart = *(char **)(pGroup + 0xc) + off;
    nKind = *(signed char *)(pPart + 2);

        if (nKind != 0) {
            if (nKind == 2 && *(int *)(pPart + 4) == 0 && Session_GetLocalPlayerIndex() == 0
                && (int)(*(kh_unaligned_s64 *)pActor & 0x10000) == 0) {
                *(char *)(pActor + 0x47a) = 3;
                if ((**(u32 **)(pPart + 0x138) & 0x200) != 0) {
                    *(char *)(pActor + 0x47b) = 1;
                } else {
                    *(char *)(pActor + 0x47b) = 0;
                }
            }
            data_ov104_020bc1e8[*(signed char *)(pPart + 2)](pGroup, pPart, nArg);
        }
        i++;
        off += 0x1c8;
    } while (i < (int)*(u8 *)(pGroup + 0x19));
}

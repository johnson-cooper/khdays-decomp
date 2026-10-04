/* PS2: mechanically prepared copy of src/overlays/field/ov022_battle/Ov022_PlayCueForKind.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Adds an item to the actor's save slot and, for the local player's actor, tags the panel entry
 * with the item's display pair (or the raw id). */

extern void ForwardWithFlag1(int kind, unsigned short *pair);
extern unsigned short *Table_FindKey(int kind, unsigned int id);
extern int Session_GetLocalPlayerIndex(void);
extern void Ov002_PanelSetEntryTag(int a, int b);

void Ov022_PlayCueForKind(int obj, unsigned int id, unsigned short arg2) {
    unsigned short pair[2];
    unsigned short *e;
    pair[0] = (unsigned short)id;
    pair[1] = arg2;
    ForwardWithFlag1(*(unsigned char *)(obj + 9), pair);
    e = Table_FindKey(*(unsigned char *)(obj + 9), id);
    if (*(unsigned char *)(obj + 8) != Session_GetLocalPlayerIndex()) return;
    if ((*(kh_unaligned_u64 *)obj & 0x10000LL) != 0) return;
    if (e != 0) Ov002_PanelSetEntryTag(e[0], e[1]);
    else Ov002_PanelSetEntryTag((unsigned short)id, 0);
}

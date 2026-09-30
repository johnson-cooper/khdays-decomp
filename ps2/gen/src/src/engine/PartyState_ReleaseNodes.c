/* PS2: mechanically prepared copy of src/engine/PartyState_ReleaseNodes.c (ps2/tools/prep_sources.py). Do not edit. */
/* Releases the node of each of the two party records. */

#include "game/engine.h"

extern char data_0204c500;

void PartyState_ReleaseNodes(void) {
    register int index = 0;
    register char *ptr = &data_0204c500;
    register int zero = index;

    for (; index < 2; index++) {
        if (*(int *)(ptr + 0x44) != 0) {
            DispatchByNodeKind(ptr + 0x44);
            *(int *)(ptr + 0x44) = zero;
        }
        ptr += 0x48;
    }
}

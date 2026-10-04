/* PS2: mechanically prepared copy of src/overlays/players/ov032_player_xigbar/Ov032_HandleMessage.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Message handler of the ov032 enemy (and its byte-identical twins): forwards the message to the
 * +0x2644 item and its +0x30 sub-item, runs the overlay's own handling (46e4) and, on the local
 * player's session only, mirrors the item's +0x18 owner byte into bit 27 of the 64-bit flag
 * word (set for owner 0, cleared otherwise) and into the shared byte at +0x110 of 0204c3d8. */

#include "game/engine.h"

extern void Ov022_ForwardToNodeHandler(int item, int msg);
extern void Ov032_InvokeBothGroupCallbacks(int obj);
extern void Ov002_Panel_SetOwnerAndRefresh(int owner);
extern char data_0204c3d8[];

void Ov032_HandleMessage(int obj, int msg)
{
    int owner;

    Ov022_ForwardToNodeHandler(*(int *)(obj + 0x2644), msg);
    Ov022_ForwardToNodeHandler(*(int *)(obj + 0x2644) + 0x30, msg);
    Ov032_InvokeBothGroupCallbacks(obj);
    owner = *(unsigned char *)(*(int *)(obj + 0x2644) + 0x18);
    Ov002_Panel_SetOwnerAndRefresh(owner);
    if (Session_GetLocalPlayerIndex() == 0) {
        if (owner <= 0) {
            *(kh_unaligned_u64 *)obj |= 0x8000000LL;
        } else {
            *(kh_unaligned_u64 *)obj &= ~0x8000000LL;
        }
    }
    data_0204c3d8[0x110] = (char)owner;
}

/* PS2: mechanically prepared copy of src/overlays/players/ov057_player_lexaeus_2/data/ov057_descriptors.c (ps2/tools/prep_sources.py). Do not edit. */
#include "game/class_descriptor.h"
/* ov057 semantic descriptors.
 *
 * gOv057SlotInitParams initializes actor slot 5 from the le/li_e3 resource.
 * gOv057SceneClass registers scene class 10/group 6 with a 0x3180-byte
 * auxiliary state block and the overlay's THUMB open/close hooks.
 */

typedef struct {
    char *pszResourcePath;
    int resourceKind;
    int reserved[3];
} Ov022SlotInitParams;

extern char gOv057LexaeusLiE3PackPath;
extern void Ov057_InitAndReturnNextState(void);
extern void Ov057_setupTriple(void);

const Ov022SlotInitParams data_ov057_020b738c __attribute__((aligned(__alignof__(Ov022SlotInitParams)))) = {
    &gOv057LexaeusLiE3PackPath,
    3,
    {0, 0, 0},
};

GameClassDescriptor gOv057LexaeusClass __attribute__((aligned(__alignof__(GameClassDescriptor)))) = {
    10,
    6,
    Ov057_InitAndReturnNextState,
    Ov057_setupTriple,
    0x3180,
    0,
};

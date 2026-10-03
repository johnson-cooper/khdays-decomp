/* PS2: mechanically prepared copy of src/overlays/field/ov015_field_pickups/data/ov015_pickup_kinds_020828d4.c (ps2/tools/prep_sources.py). Do not edit. */
/* ov015 .rodata 0x020828d4-0x020828e4: the per-kind pickup rows read by Ov015_SpawnPickup
 * (020801c8: handler block and rise scale), Ov015_PickupLoad (0207fac0) and
 * Ov015_PickupShow (0207fb8c): kind 0 is a pickup with a model driven by
 * Ov015_PickupUpdate (0207fe0c, rise scale 0x3c), kind 1 a model-less pickup whose
 * state function is Ov015_PickupTakenStep (0207ffd0).  The rise byte of row 0 is also
 * addressed as data_ov015_020828d8. */

#include "nitro/types.h"

typedef struct Ov015PickupKindRow {
    void *pHandlers;          /* 0x00: state function of the kind */
    u8   nRise;               /* 0x04: rise speed scale (random range) */
    u8   pad_05[3];
} Ov015PickupKindRow;

extern void Ov015_PickupUpdate(void);   /* Ov015_PickupUpdate */
extern void Ov015_PickupTakenStep(void);   /* Ov015_PickupTakenStep */

const Ov015PickupKindRow data_ov015_020828d4[2] __attribute__((aligned(__alignof__(Ov015PickupKindRow)))) = {
    { (void *)Ov015_PickupUpdate, 0x3c },
    { (void *)Ov015_PickupTakenStep, 0 },
};

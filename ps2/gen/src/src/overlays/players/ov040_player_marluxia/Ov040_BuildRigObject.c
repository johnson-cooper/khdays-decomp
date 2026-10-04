/* PS2: mechanically prepared copy of src/overlays/players/ov040_player_marluxia/Ov040_BuildRigObject.c (ps2/tools/prep_sources.py). Do not edit. */
#include "platform/kh_unaligned.h"
/* Ov040_BuildRigObject -- overlay boot: builds the enemy rig object and publishes it.
 *
 * Grabs the current root heap block as the object, latches it in the overlay global, copies the
 * three identity fields out of the caller's config (type, slot id, bit index), opens the slot
 * with Entity_ForwardToSlot (the 3-word parameter block is {1, 0x1f00, 0x900}), binds the animation
 * table -- ...4bfc when the config's alt flag is set, ...4be8 otherwise -- fills the ten-entry
 * handler vtable at +0x664, attaches the scene node, invalidates the five bone handles at
 * +0x514 and then resolves four of them by name, and finally folds three optional capability
 * bits into the 64-bit flag word before handing the object to Ov022_InitActor.
 *
 * Head of a 16-member family (ov038/040/041/042/...); the members differ only in the vtable
 * entries and the four bone-name descriptors.
 *
 * Dos formas del fuente que NO son cosmeticas y sin las cuales no casa:
 *   - El bucle va con `goto` explicito. Escrito como `for`, mwcc lo ROTA (entra directo al
 *     cuerpo porque puede probar 0 < 5) y el ROM salta antes al test. Ninguna grafia de
 *     for/while/do lo evita; el goto reproduce el orden de bloques exacto.
 *   - `params` tiene CINCO campos y solo se escriben tres. Los dos muertos son los 8 bytes de
 *     pila que separan `sub sp,#0x18` (ROM) de `sub sp,#0x10`, y son ademas los que obligan a
 *     mwcc a empujar el r3 de relleno en el `push`. Un campo de mas que nadie lee no es ruido:
 *     es la unica prueba que queda de como era la estructura original.
 *   - `i * sizeof(int)` en vez de `i * 4`: con `* 4` mwcc crea una variable de induccion y gasta
 *     un callee-saved de mas; con el sizeof recalcula el desplazamiento como hace el ROM.
 */

#include "game/engine.h"

extern void *NNSi_FndGetCurrentRootHeap(void);
extern int  NNS_G3dGetResDictIdxByName(void *node, void *desc);
extern void Ov022_InitActor(void *obj);

extern void Ov040_SwitchAnimationSet(void);
extern void func_ov040_020b3614(void);
extern void Ov040_InvokeSubHandlerPairWithGlobalBuffer(void);
extern void Ov040_initSharedObjSetReadyFlag(void);
extern void Ov040_MapSlotKindToAnim(void);
extern void Ov040_HandleMessage(void);
extern void Ov040_BuildRenderHandles(void);
extern void Ov040_PublishAndEnterState21(void);

extern void *data_ov040_020b4b20;
extern int gOv040MarluxiaDefPackPath;
extern int gOv040MarluxiaDefHPackPath;
extern int gOv040MarluxiaTgName;
extern int gOv040MaHRName;
extern int gOv040MarluxiaRName;
extern int gOv040Bip01Name;

/* The rig hangs off the scene node at +0x28; each bone block starts 0x40 further in. */
typedef struct { int pad[1]; int f4; } RigHdr;

static inline int bone(char *obj) {
    int p = ((RigHdr *)(*(int *)(obj + 0x20) + 0x24))->f4;
    return p != 0 ? p + 0x40 : 0;
}

void Ov040_BuildRigObject(int *cfg) {
    struct { int a, b, c, d, e; } params;
    char *obj = (char *)NNSi_FndGetCurrentRootHeap();
    int i;
    int b;

    data_ov040_020b4b20 = obj;
    obj[9] = (char)cfg[0];
    obj[0x4bc] = (char)cfg[1];
    obj[8] = *(unsigned char *)((char *)cfg + 8);
    *(int *)(obj + 0xc) = 10;
    *(kh_unaligned_s64 *)obj = 0;

    params.a = 1;
    params.c = 9 << 8;
    params.b = 0x17 << 8;
    Entity_ForwardToSlot(*(signed char *)(obj + 0x4bc),
                         (unsigned short)(1 << *(unsigned char *)(obj + 8)), 0, &params, 0);

    if (cfg[6] == 0) {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv040MarluxiaDefPackPath, 1, cfg[0] + 7);
    } else {
        TailForwardTrackEntry(*(signed char *)(obj + 0x4bc), &gOv040MarluxiaDefHPackPath, 1, cfg[0] + 7);
    }

    *(void **)(obj + 0x664 + 0x00) = (void *)&Ov040_SwitchAnimationSet;
    *(void **)(obj + 0x664 + 0x04) = (void *)&func_ov040_020b3614;
    *(void **)(obj + 0x664 + 0x08) = (void *)&Ov040_InvokeSubHandlerPairWithGlobalBuffer;
    *(void **)(obj + 0x664 + 0x0c) = 0;
    *(void **)(obj + 0x664 + 0x10) = 0;
    *(void **)(obj + 0x664 + 0x14) = (void *)&Ov040_initSharedObjSetReadyFlag;
    *(void **)(obj + 0x664 + 0x18) = (void *)&Ov040_MapSlotKindToAnim;
    *(void **)(obj + 0x664 + 0x20) = (void *)&Ov040_HandleMessage;
    *(void **)(obj + 0x664 + 0x28) = (void *)&Ov040_BuildRenderHandles;
    *(void **)(obj + 0x664 + 0x24) = (void *)&Ov040_PublishAndEnterState21;

    Actor_InitEntityLink(obj + 0x20, ArrayEntryPtrD0(*(signed char *)(obj + 0x4bc)));

    i = 0;
    goto test;
body:
    *(int *)(obj + i * sizeof(int) + 0x514) = -1;
    i++;
test:
    if (i < 5) {
        goto body;
    }

    b = bone(obj);
    *(int *)(obj + 0x520) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv040MarluxiaTgName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x518) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv040MaHRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x51c) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv040MarluxiaRName) : -1;
    b = bone(obj);
    *(int *)(obj + 0x524) = b != 0 ? NNS_G3dGetResDictIdxByName((void *)b, &gOv040Bip01Name) : -1;

    if (cfg[3] != 0) {
        *(kh_unaligned_s64 *)obj |= 0x20;
    }
    if (cfg[4] != 0) {
        *(kh_unaligned_s64 *)obj |= 0x10000;
    }
    if (cfg[5] != 0) {
        *(kh_unaligned_s64 *)obj |= 0x1000000000LL;
    }
    Ov022_InitActor(obj);
}

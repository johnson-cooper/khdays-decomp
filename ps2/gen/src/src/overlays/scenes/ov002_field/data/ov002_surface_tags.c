/* PS2: mechanically prepared copy of src/overlays/scenes/ov002_field/data/ov002_surface_tags.c (ps2/tools/prep_sources.py). Do not edit. */
/* Seven resource names and their tag IDs. */

#include "nitro/types.h"

extern char gOv002NohitName;
extern char gOv002NocatchName;
extern char gOv002SeaName;
extern char gOv002NocamName;
extern char gOv002SlideName;
extern char gOv002Sea2Name;
extern char gOv002SiconName;

typedef struct {
    const char *name;
    u8 tag;
    u8 pad[3];
} Ov002SurfaceTag;

const Ov002SurfaceTag data_ov002_0207e640[7] __attribute__((aligned(__alignof__(Ov002SurfaceTag)))) = {
    { &gOv002NohitName, 2, {0} },
    { &gOv002NocatchName, 3, {0} },
    { &gOv002SeaName, 4, {0} },
    { &gOv002NocamName, 5, {0} },
    { &gOv002SlideName, 6, {0} },
    { &gOv002Sea2Name, 7, {0} },
    { &gOv002SiconName, 8, {0} },
};

/* PS2: mechanically prepared copy of src/engine/data/main_pointers_02042910.c (ps2/tools/prep_sources.py). Do not edit. */
/* main .data pointer tables, 0x02042910-0x02042958.
 *
 * 3 tables, all zero in the ROM image because every entry is a relocation;
 * a zero word is a null entry.
 */

typedef void (*Ov_Fn)(void);

extern void Coll_TestRayAgainstFlatCap(void);
extern void Coll_TestSphereAgainstCylinder(void);
extern void Coll_TestRayAgainstRoundedCap(void);
extern void Bounds_FromCenterRadius(void);
extern void Collider_Slot1NoOp_2(void);
extern void Collider_InitPlaneY(void);
extern void Collider_InitCylinderContact(void);
extern void Collider_Slot1NoOp(void);
extern void Collider_InitSphereContact(void);
extern void RoomBox_UpdateBounds(void);
extern void RoomBox_Slot1NoOp(void);
extern void RoomBox_HitTop(void);
extern void RoomBox_HitSides(void);
extern void RoomBox_WallPlane(void);

Ov_Fn data_02042910[6] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Bounds_FromCenterRadius,

    Collider_Slot1NoOp,

    Coll_TestRayAgainstRoundedCap,

    Coll_TestSphereAgainstCylinder,

    Collider_InitSphereContact,

    Collider_InitCylinderContact,

};

Ov_Fn data_02042928[6] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    RoomBox_UpdateBounds,

    RoomBox_Slot1NoOp,

    RoomBox_HitTop,

    RoomBox_HitSides,

    Collider_InitPlaneY,

    RoomBox_WallPlane,

};

Ov_Fn data_02042940[6] __attribute__((aligned(__alignof__(Ov_Fn)))) = {

    Bounds_FromCenterRadius,

    Collider_Slot1NoOp_2,

    Coll_TestRayAgainstFlatCap,

    Coll_TestSphereAgainstCylinder,

    Collider_InitPlaneY,

    Collider_InitCylinderContact,

};
